#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "dp.h"
#include <QFormLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

#define NB_BUTTONS 20

extern std::vector<std::string> button_text;

class CalButton : public QPushButton
{
    Q_OBJECT
public:
    explicit CalButton(const QString &text);
signals:
    // signal to send when the button is clicked,
    // with the current text as argument
    void clickedWithText(const QString &text);
};

class Calculator : public QWidget
{
    Q_OBJECT
public:
    explicit Calculator(QWidget *parent);

public slots:
    void on_button_clicked(const QString &text);

private:
    void init_button();
    void check_button()
    {
        assert((button_text.size() == NB_BUTTONS));
    };
    std::vector<CalButton *> buttons;
    QLineEdit *expr_line_edit;
    QLineEdit *result_line_edit;
    std::string msg;
};

#endif
