#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

#include <src/CustomLineEdit/customlineedit.h>
#include <src/InputDialog/inputdialog.h>
#include <src/FilesDialog/filesdialog.h>
#include <src/presetandconfig.h>
#include <src/utility.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    bool isBackupInProgress = false;

    void setWidgetVisibility();

    void setWidgetValues();

    void connectSignals();

    void setWidgetEnabled();

private slots:
    void actionMultithreadedToggled(bool checked);
    void actionAllPresetsToggled(bool checked);
    void inputBackupFolderLostFocus();
    void btnNewPresetPressed();
    void btnDeletePresetPressed();
    void btnSearchFolderPressed();
    void btnFilesPressed();
    void btnBackupPressed();
    void checkBoxAllPresetsStateChanged(int state);
};

#endif // MAINWINDOW_H
