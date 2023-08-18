#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QTimer>

#include <src/CustomLineEdit/customlineedit.h>
#include <src/InputDialog/inputdialog.h>
#include <src/FilesDialog/filesdialog.h>
#include <src/presetandconfig.h>
#include <src/backupmanager.h>
#include <src/utility.h>
#include <src/qrunnables.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    QThreadPool *threadPool = nullptr;
    QTimer *labelTimer = nullptr;
    QThread *backupThread = nullptr;
    bool isBackupInProgress = false;
    int btnBackupClickCounter = 0;

    void setUiWidgetVisibility();

    void setUiWidgetValues();

    void setWidgetEnabled();

    void connectSignals();

    void prepareLocalInstances();

    void setStartupText();

private slots:

    void actionMultithreadedToggled(bool checked);

    void actionAllPresetsToggled(bool checked);

    void inputBackupFolderLostFocus();

    void btnNewPresetClicked();

    void btnDeletePresetClicked();

    void btnSearchFolderClicked();

    void btnFilesClicked();

    void checkBoxAllPresetsStateChanged(int state);

    void btnBackupClicked();

    void backupComplete(bool wasSuccessful);

    void labelTimerTimeout();
};

#endif // MAINWINDOW_H
