#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/resources/icon/icon.ico"));
    connectSignals();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectSignals(){
    connect(ui->actionQuit, &QAction::triggered, &QApplication::quit);
    connect(ui->actionMultithreaded, &QAction::toggled, this, &MainWindow::actionMultithreadedToggled);
    connect(ui->inputBackupFolder, &QLineEdit::editingFinished, this, &MainWindow::inputBackupFolderEditingFinished);
    connect(ui->comboBoxPresets, &QComboBox::currentIndexChanged, this, &MainWindow::comboBoxPresetsChanged);//pre Qt6: static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged) or QOverload<int>(&QComboBox::currentIndexChanged)
    connect(ui->btnNewPreset, &QPushButton::pressed, this, &MainWindow::btnNewPresetPressed);
    connect(ui->BtnDeletePreset, &QPushButton::pressed, this, &MainWindow::btnDeletePresetPressed);
    connect(ui->btnSearch, &QPushButton::pressed, this, &MainWindow::btnSearchFolderPressed);
    connect(ui->btnFiles, &QPushButton::pressed, this, &MainWindow::btnFilesPressed);
    connect(ui->btnBackup, &QPushButton::pressed, this, &MainWindow::btnBackupPressed);
    connect(ui->btnBackup, &QPushButton::pressed, this, &MainWindow::btnBackupPressed);
    connect(ui->checkBoxAllPresets, &QCheckBox::stateChanged, this, &MainWindow::checkBoxAllPresets);
}

void MainWindow::actionMultithreadedToggled(bool checked){}

void MainWindow::inputBackupFolderEditingFinished(){}

void MainWindow::comboBoxPresetsChanged(int index){}

void MainWindow::btnNewPresetPressed(){}

void MainWindow::btnDeletePresetPressed(){}

void MainWindow::btnSearchFolderPressed(){
    QString folder = QFileDialog::getExistingDirectory(this, MainWindow::tr("Open Directory"), QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(folder == "")
        return;
    QFileInfo fileInfo(folder);
    if(!fileInfo.isDir()){//Shouldn't be needed, still verifying to make sure
        Utility::showError(this, tr("Error"), tr("Needs to be a directory"));
        return;
    }
    if(fileInfo.isRoot()){
        Utility::showError(this, tr("Error"), tr("Backup Folder can't be the root directory") + " \"" + QDir::root().absolutePath() + "\"");
        return;
    }
    if(!fileInfo.isWritable()){
        Utility::showError(this, tr("Error"), tr("Program does not have permission to write to that directory"));
        return;
    }
    ui->inputBackupFolder->setText(folder);
    //TODO config stuff
}

void MainWindow::btnFilesPressed(){}

void MainWindow::btnBackupPressed(){}

void MainWindow::checkBoxAllPresets(int state){}
