#include "utility.h"

#define PIXMAP_SIZE 60
#define MSGBOX_STYLE_SHEET "font-size: 16px;"

void Utility::showError(QWidget *parent, QString title, QString body){
    auto msgBox = QMessageBox(QMessageBox::Icon::Critical, title, body, QMessageBox::Ok, parent);
    QPixmap pixmap(":/resources/icon/dialog-error.ico");
    pixmap = pixmap.scaled(PIXMAP_SIZE, PIXMAP_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    msgBox.setIconPixmap(pixmap);
    msgBox.setStyleSheet(MSGBOX_STYLE_SHEET);
    msgBox.exec();
}

void Utility::showWarning(QWidget *parent, QString title, QString body){
    auto msgBox = QMessageBox(QMessageBox::Icon::Warning, title, body, QMessageBox::Ok, parent);
    QPixmap pixmap(":/resources/icon/dialog-warning.ico");
    pixmap = pixmap.scaled(PIXMAP_SIZE, PIXMAP_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    msgBox.setIconPixmap(pixmap);
    msgBox.setStyleSheet(MSGBOX_STYLE_SHEET);
    msgBox.exec();
}


int Utility::showWarningWithButtons( QWidget *parent, QString title, QString body){
    auto msgBox = QMessageBox(QMessageBox::Icon::Warning, title, body, QMessageBox::Yes | QMessageBox::No, parent);
    QPixmap pixmap(":/resources/icon/dialog-warning.ico");
    pixmap = pixmap.scaled(PIXMAP_SIZE, PIXMAP_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    msgBox.setIconPixmap(pixmap);
    msgBox.setStyleSheet(MSGBOX_STYLE_SHEET);
    return msgBox.exec();
}
