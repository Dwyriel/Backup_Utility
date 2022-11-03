#include "inputdialog.h"
#include "./ui_inputdialog.h"

InputDialog::InputDialog(QWidget *parent) : QDialog(parent), ui(new Ui::InputDialog)
{
    ui->setupUi(this);
    ui->btnOk->setDisabled(true);
    connectSignals();
}

InputDialog::InputDialog(QString title, QWidget *parent) : QDialog(parent), ui(new Ui::InputDialog)
{
    ui->setupUi(this);
    ui->btnOk->setDisabled(true);
    ui->lblTitle->setText(title);
    connectSignals();
}

InputDialog::~InputDialog()
{
    delete ui;
}

void InputDialog::connectSignals(){
    connect(ui->btnOk, &QPushButton::pressed, this, &InputDialog::btnOkPressed);
    connect(ui->btnCancel, &QPushButton::pressed, this, &InputDialog::btnCancelPressed);
    connect(ui->inputText, &QLineEdit::textEdited, this, &InputDialog::inputTextEdited);
}

void InputDialog::btnOkPressed(){
    done(QDialog::Accepted);
}

void InputDialog::btnCancelPressed(){
    done(QDialog::Rejected);
}

void InputDialog::inputTextEdited(QString text){
    ui->btnOk->setDisabled(text == "");
    OutputString = text;
}
