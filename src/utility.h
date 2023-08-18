#ifndef UTILITY_H
#define UTILITY_H

#include <QMessageBox>
#include <QFileDialog>

namespace Utility {
    void showError(QWidget *parent, const QString &title, const QString &body);

    void showWarning(QWidget *parent, const QString &title, const QString &body);

    int showWarningWithButtons(QWidget *parent, const QString &title, const QString &body);

    void SetMultiFileDialog(QFileDialog &qFileDialog);

    void SetSingleDirectoryDialog(QFileDialog &qFileDialog);
};

#endif // UTILITY_H
