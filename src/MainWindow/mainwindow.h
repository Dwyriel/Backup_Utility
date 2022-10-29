#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

    void connectSignals();

private slots:
    void actionMultithreadedToggled(bool checked);
    void inputBackupFolderEditingFinished();
    void comboBoxPresetsChanged(int index);
    void btnNewPresetPressed();
    void btnDeletePresetPressed();
    void btnSearchFolderPressed();
    void btnFilesPressed();
    void btnBackupPressed();
    void checkBoxAllPresets(int state);
};

#endif // MAINWINDOW_H
