#include "filesdialog.h"
#include "./ui_filesdialog.h"

FilesDialog::FilesDialog(QWidget *parent) : QDialog(parent), ui(new Ui::FilesDialog){
    ui->setupUi(this);
    connectSignals();
}

FilesDialog::~FilesDialog(){
    delete ui;
}
