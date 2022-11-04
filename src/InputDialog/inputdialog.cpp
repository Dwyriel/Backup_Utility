#include "inputdialog.h"
#include "./ui_inputdialog.h"

InputDialog::InputDialog(QWidget *parent) : QDialog(parent), ui(new Ui::InputDialog){
    ui->setupUi(this);
    ui->btnOk->setDisabled(true);
    connectSignals();
}

InputDialog::InputDialog(QString title, QWidget *parent) : QDialog(parent), ui(new Ui::InputDialog){
    ui->setupUi(this);
    ui->btnOk->setDisabled(true);
    ui->lblTitle->setText(title);
    connectSignals();
}

InputDialog::~InputDialog(){
    delete ui;
}

void InputDialog::connectSignals(){
    connect(ui->btnOk, &QPushButton::clicked, this, &InputDialog::btnOkClicked);
    connect(ui->btnCancel, &QPushButton::clicked, this, &InputDialog::btnCancelClicked);
    connect(ui->inputText, &QLineEdit::textEdited, this, &InputDialog::inputTextEdited);
}

void InputDialog::btnOkClicked(){
    done(QDialog::Accepted);
}

void InputDialog::btnCancelClicked(){
    done(QDialog::Rejected);
}

void InputDialog::inputTextEdited(QString text){
    ui->btnOk->setDisabled(text == "");
    OutputString = text;
}
