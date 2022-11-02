#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

#include <src/CustomLineEdit/customlineedit.h>
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

    void setWidgetValues();

    void connectSignals();

private slots:
    void actionMultithreadedToggled(bool checked);
    void inputBackupFolderLostFocus();
    void comboBoxPresetsChanged(int index);
    void btnNewPresetPressed();
    void btnDeletePresetPressed();
    void btnSearchFolderPressed();
    void btnFilesPressed();
    void btnBackupPressed();
    void checkBoxAllPresets(int state);
};

#endif // MAINWINDOW_H
