#include "calculator.h"
#include <cmath>
#include <iostream>

std::vector<std::string> button_text = {"(", ")", "^", "/", "7", "8", "9",
                                        "*", "4", "5", "6", "-", "1", "2",
                                        "3", "+", "C", "0", ".", "="};

CalButton::CalButton(const QString &text) : QPushButton(text)
{
    connect(this, &QPushButton::clicked, this,
            [=]() { emit this->clickedWithText(this->text()); });
}

Calculator::Calculator(QWidget *parent)
    : QWidget(parent), buttons(NB_BUTTONS), expr_line_edit(new QLineEdit("")),
      result_line_edit(new QLineEdit(""))
{
    this->check_button();
    this->init_button();
    QGridLayout *button_layout = new QGridLayout();
    QVBoxLayout *main_layout = new QVBoxLayout();
    QFormLayout *form_layout = new QFormLayout();

    form_layout->addRow("Expression", expr_line_edit);
    expr_line_edit->setReadOnly(true);

    form_layout->addRow("Result", result_line_edit);
    result_line_edit->setReadOnly(true);

    setLayout(main_layout);

    main_layout->addLayout(form_layout);
    int count = 0;
    for (auto button : buttons)
    {
        button_layout->addWidget(button, (int)(count / 4), count % 4);
        button->show();
        button->setText(button_text[count].c_str());
        connect(button, &CalButton::clickedWithText, this,
                &Calculator::on_button_clicked);
        count++;
    }
    main_layout->addLayout(button_layout);
    main_layout->addStretch();
}

void Calculator::on_button_clicked(const QString &text)
{
    if (text == "=")
    {
        double ret;
        if (parse(expr_line_edit->text().toStdString(), &ret) == 0)
        {
            result_line_edit->setText("Error");
        }
        else
        {
            result_line_edit->setText(QString::number(ret));
        }
    }
    else if (text == "C")
    {
        result_line_edit->setText("");
        expr_line_edit->setText("");
    }
    else
    {
        expr_line_edit->setText(
            expr_line_edit->text().append(text).toLocal8Bit());
    }
}

void Calculator::init_button()
{
    for (int i = 0; i < buttons.size(); i++)
    {
        buttons[i] = new CalButton("");
    }
}

// include the moc file for cmake
#include "moc_calculator.cpp"
