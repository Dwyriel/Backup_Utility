#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QTimer>

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
    QTimer *labelTimer;

    void setUiWidgetVisibility();

    void setUiWidgetValues();

    void connectSignals();

    void setWidgetEnabled();

    void prepareLabelTimer();

private slots:
    void actionMultithreadedToggled(bool checked);
    void actionAllPresetsToggled(bool checked);
    void inputBackupFolderLostFocus();
    void btnNewPresetClicked();
    void btnDeletePresetClicked();
    void btnSearchFolderClicked();
    void btnFilesClicked();
    void btnBackupClicked();
    void checkBoxAllPresetsStateChanged(int state);
    void labelTimerTimeout();
};

#endif // MAINWINDOW_H
