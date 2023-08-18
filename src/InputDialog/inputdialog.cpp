#include "inputdialog.h"
#include "./ui_inputdialog.h"

InputDialog::InputDialog(QWidget *parent) : QDialog(parent), ui(new Ui::InputDialog) {
    ui->setupUi(this);
    ui->btnOk->setDisabled(true);
    connectSignals();
}

InputDialog::InputDialog(const QString &body, QWidget *parent) : QDialog(parent), ui(new Ui::InputDialog) {
    ui->setupUi(this);
    ui->btnOk->setDisabled(true);
    ui->lblBody->setText(body);
    connectSignals();
}

InputDialog::InputDialog(const QString &title, const QString &body, QWidget *parent) : QDialog(parent), ui(new Ui::InputDialog) {
    ui->setupUi(this);
    ui->btnOk->setDisabled(true);
    setWindowTitle(title);
    ui->lblBody->setText(body);
    connectSignals();
}

InputDialog::~InputDialog() {
    delete ui;
}

void InputDialog::connectSignals() {
    connect(ui->btnOk, &QPushButton::clicked, this, &InputDialog::btnOkClicked);
    connect(ui->btnCancel, &QPushButton::clicked, this, &InputDialog::btnCancelClicked);
    connect(ui->inputText, &QLineEdit::textEdited, this, &InputDialog::inputTextEdited);
}

void InputDialog::btnOkClicked() {
    done(QDialog::Accepted);
}

void InputDialog::btnCancelClicked() {
    done(QDialog::Rejected);
}

void InputDialog::inputTextEdited(const QString &text) {
    ui->btnOk->setDisabled(text.isEmpty());
    OutputString = text;
}
