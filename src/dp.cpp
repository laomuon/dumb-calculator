#include "dp.h"

#include <assert.h>
#include <clocale>
#include <cstdlib>
#include <optional>

typedef double (*op_func)(double, double);

typedef struct list_item
{
    union {
        double value;
        char c_op;
    };
    op_func func;
    struct list_item *next;
    int is_nb;
} list_item;

typedef struct simple_list
{
    list_item *first_item;
    list_item *last_item;
    int size;
} simple_list;

simple_list op_stack, out_stack;

void add_item(simple_list *lst, double value, char c, op_func func, int is_nb)
{
    list_item *item = (list_item *)malloc(sizeof(list_item));
    if (is_nb)
    {
        item->value = value;
    }
    else
    {
        item->c_op = c;
        item->func = func;
    }
    item->is_nb = is_nb;
    item->next = NULL;
    if (lst->size == 0)
    {
        lst->first_item = item;
        lst->last_item = item;
    }
    else
    {
        lst->last_item->next = item;
        lst->last_item = item;
    }
    lst->size++;
}

std::optional<double> pop_last_item(simple_list *lst)
{
    double c;
    if (lst->size == 0)
    {
        return std::nullopt;
    }
    if (lst->size == 1)
    {
        if (lst->last_item->is_nb)
        {
            c = lst->last_item->value;
        }
        else
        {
            c = lst->last_item->c_op;
        }
        free(lst->last_item);
        lst->first_item = NULL;
        lst->last_item = NULL;
        lst->size = 0;
        return c;
    }
    lst->size--;
    list_item *temp = lst->first_item;
    while (temp->next->next != NULL)
    {
        temp = temp->next;
    }
    if (lst->last_item->is_nb)
    {
        c = lst->last_item->value;
    }
    else
    {
        c = lst->last_item->c_op;
    }
    free(lst->last_item);
    lst->last_item = temp;
    lst->last_item->next = NULL;
    return c;
}

void clear_list(simple_list *lst)
{
    while (lst->first_item != NULL)
        pop_last_item(lst);
}

void op_from_stack_to_output()
{
    std::optional<double> right_nb = pop_last_item(&out_stack);
    std::optional<double> left_nb = pop_last_item(&out_stack);
    if (right_nb.has_value() && left_nb.has_value())
    {
        op_func func = op_stack.last_item->func;
        add_item(&out_stack, func(left_nb.value(), right_nb.value()), ' ', NULL,
                 1);
        pop_last_item(&op_stack);
    }
}

static double add(double a, double b)
{
    return a + b;
}
static double sub(double a, double b)
{
    return a - b;
}
static double mul(double a, double b)
{
    return a * b;
}
static double divide(double a, double b)
{
    return a / b;
}
static double power(double a, double b)
{
    return pow(a, b);
}

typedef struct op_s
{
    char op;
    int precedence;
    op_func func;
} op_s;

op_s ops[] = {
    {'+', 0, &add},    {'-', 0, &sub},   {'*', 1, &mul},
    {'/', 1, &divide}, {'^', 1, &power},
};

std::optional<op_func> get_op_func(char c)
{
    for (int i = 0; i < sizeof(ops) / sizeof(op_s); i++)
    {
        if (c == ops[i].op)
        {
            return ops[i].func;
        }
    }
    return std::nullopt;
}

int is_valid_op(char c)
{
    for (int i = 0; i < sizeof(ops) / sizeof(op_s); i++)
    {
        if (c == ops[i].op)
        {
            return 1;
        }
    }
    return 0;
}

int has_greater_precedent(char op1, char op2)
{
    int pre1 = -1;
    int pre2 = -1;
    for (int i = 0; i < sizeof(ops) / sizeof(op_s); i++)
    {
        if (op1 == ops[i].op)
        {
            pre1 = ops[i].precedence;
        }
        if (op2 == ops[i].op)
        {
            pre2 = ops[i].precedence;
        }
    }
    if (pre1 < 0 || pre2 < 0)
    {
        return 0;
    }
    return pre1 > pre2;
}

int parse(std::string source_string, double *ret)
{
    int init_len = source_string.size();
    const char *s = source_string.c_str();
    list_item *temp_op;
    clear_list(&op_stack);
    clear_list(&out_stack);
    while (*s != '\0')
    {
        if ((*s >= '0' && *s <= '9') || *s == '.' ||
            (*s == '-' && (strlen(s) == init_len || is_valid_op(*(s - 1)) ||
                           *(s - 1) == '(')))
        {
            double temp = strtod(s, (char **)&s);
            add_item(&out_stack, temp, ' ', NULL, 1);
        }
        else
        {
            auto current_func = get_op_func(*s);
            if (current_func.has_value())
            {
                while (op_stack.size > 0 &&
                       has_greater_precedent(op_stack.last_item->c_op, *s))
                {
                    op_from_stack_to_output();
                }
                add_item(&op_stack, 0, *s, current_func.value(), 0);
            }
            else if (*s == '(')
            {
                add_item(&op_stack, 0, *s, NULL, 0);
            }
            else if (*s == ')')
            {
                while (op_stack.last_item->c_op != '(')
                {
                    op_from_stack_to_output();
                }
                pop_last_item(&op_stack);
            }
            else
            {
                return 0;
            }
            s++;
        }
    }
    while (op_stack.size > 0)
    {
        op_from_stack_to_output();
    }
    *ret = out_stack.first_item->value;
    return 1;
}
