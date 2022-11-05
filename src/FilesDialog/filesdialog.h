#ifndef FILESDIALOG_H
#define FILESDIALOG_H

#include <QDialog>
#include <QFileDialog>

#include <src/presetandconfig.h>
#include <src/utility.h>

namespace Ui {
class FilesDialog;
}

class FilesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilesDialog(QWidget *parent = nullptr);
    ~FilesDialog();

private:
    Ui::FilesDialog *ui;

    void setFileList();

    void setFolderList();

    void connectSignals();

private slots:
    void btnAddFileClicked();
    void btnAddFolderClicked();
    void btnClearFileClicked();
    void btnClearFolderClicked();
    void btnCloseClicked();
    void btnRemoveFileClicked();
    void btnRemoveFolderClicked();
    void btnResetBackupsClicked();
};

#endif // FILESDIALOG_H
