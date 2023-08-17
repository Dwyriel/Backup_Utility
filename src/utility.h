#ifndef UTILITY_H
#define UTILITY_H

#include <QMessageBox>
#include <QFileDialog>

namespace Utility {
    void showError(QWidget *parent, QString title, QString body);

    void showWarning(QWidget *parent, QString title, QString body);

    int showWarningWithButtons(QWidget *parent, QString title, QString body);

    void SetMultiFileDialog(QFileDialog &qFileDialog);

    void SetSingleDirectoryDialog(QFileDialog &qFileDialog);
};

#endif // UTILITY_H
