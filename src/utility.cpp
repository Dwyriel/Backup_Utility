#include "utility.h"

#define PIXMAP_SIZE 60
#define MSGBOX_STYLE_SHEET "font-size: 16px;"

void Utility::showError(QWidget *parent, const QString &title, const QString &body) {
    auto msgBox = QMessageBox(QMessageBox::Icon::Critical, title, body, QMessageBox::Ok, parent);
    QPixmap pixmap(":/resources/icon/dialog-error.ico");
    pixmap = pixmap.scaled(PIXMAP_SIZE, PIXMAP_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    msgBox.setIconPixmap(pixmap);
    msgBox.setStyleSheet(MSGBOX_STYLE_SHEET);
    msgBox.exec();
}

void Utility::showWarning(QWidget *parent, const QString &title, const QString &body) {
    auto msgBox = QMessageBox(QMessageBox::Icon::Warning, title, body, QMessageBox::Ok, parent);
    QPixmap pixmap(":/resources/icon/dialog-warning.ico");
    pixmap = pixmap.scaled(PIXMAP_SIZE, PIXMAP_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    msgBox.setIconPixmap(pixmap);
    msgBox.setStyleSheet(MSGBOX_STYLE_SHEET);
    msgBox.exec();
}

int Utility::showWarningWithButtons(QWidget *parent, const QString &title, const QString &body) {
    auto msgBox = QMessageBox(QMessageBox::Icon::Warning, title, body, QMessageBox::Yes | QMessageBox::No, parent);
    QPixmap pixmap(":/resources/icon/dialog-warning.ico");
    pixmap = pixmap.scaled(PIXMAP_SIZE, PIXMAP_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    msgBox.setIconPixmap(pixmap);
    msgBox.setStyleSheet(MSGBOX_STYLE_SHEET);
    return msgBox.exec();
}

void Utility::SetMultiFileDialog(QFileDialog &qFileDialog) {
    qFileDialog.setFileMode(QFileDialog::ExistingFiles);
    qFileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    qFileDialog.setOptions(QFileDialog::DontResolveSymlinks);
    qFileDialog.setViewMode(QFileDialog::Detail);
    qFileDialog.setFilter(qFileDialog.filter() | QDir::Hidden);
}

void Utility::SetSingleDirectoryDialog(QFileDialog &qFileDialog) {
    qFileDialog.setFileMode(QFileDialog::Directory);
    qFileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    qFileDialog.setOptions(QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly);
    qFileDialog.setViewMode(QFileDialog::List);
    qFileDialog.setFilter(qFileDialog.filter() | QDir::Hidden);
}
