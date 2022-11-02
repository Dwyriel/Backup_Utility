#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/resources/icon/icon.ico"));
    PresetManager::Initialize();
    setWidgetValues();
    //TODO set widget values from config file
    connectSignals();
    qDebug() << ui->menubar->isNativeMenuBar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setWidgetValues(){
    ui->actionMultithreaded->setChecked(PresetManager::presetsAndConfig.Multithreaded);
    ui->inputBackupFolder->setText(PresetManager::presetsAndConfig.BackupFolderPath);
    ui->checkBoxAllPresets->setChecked(PresetManager::presetsAndConfig.backupAllPresets);
    for(const auto &item : PresetManager::presetsAndConfig.Presets)
        ui->comboBoxPresets->addItem(item.PresetName);
    ui->comboBoxPresets->setCurrentIndex(PresetManager::presetsAndConfig.CurrentPresetIndex);
}

void MainWindow::connectSignals(){
    connect(ui->actionQuit, &QAction::triggered, &QApplication::quit);
    connect(ui->actionMultithreaded, &QAction::toggled, this, &MainWindow::actionMultithreadedToggled);
    connect(ui->inputBackupFolder, &CustomLineEdit::lostFocus, this, &MainWindow::inputBackupFolderLostFocus);
    connect(ui->comboBoxPresets, &QComboBox::currentIndexChanged, this, &MainWindow::comboBoxPresetsChanged);//pre Qt6: static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged) or QOverload<int>(&QComboBox::currentIndexChanged)
    connect(ui->btnNewPreset, &QPushButton::pressed, this, &MainWindow::btnNewPresetPressed);
    connect(ui->BtnDeletePreset, &QPushButton::pressed, this, &MainWindow::btnDeletePresetPressed);
    connect(ui->btnSearch, &QPushButton::pressed, this, &MainWindow::btnSearchFolderPressed);
    connect(ui->btnFiles, &QPushButton::pressed, this, &MainWindow::btnFilesPressed);
    connect(ui->btnBackup, &QPushButton::pressed, this, &MainWindow::btnBackupPressed);
    connect(ui->btnBackup, &QPushButton::pressed, this, &MainWindow::btnBackupPressed);
    connect(ui->checkBoxAllPresets, &QCheckBox::stateChanged, this, &MainWindow::checkBoxAllPresets);
}

void MainWindow::actionMultithreadedToggled(bool checked){
    PresetManager::presetsAndConfig.Multithreaded = checked;
    PresetManager::Save();
}

void MainWindow::inputBackupFolderLostFocus(){
    QString inputtedFolder = ui->inputBackupFolder->text().trimmed();
    if(inputtedFolder == "")
        return;
    QFileInfo fileInfo(inputtedFolder);
    if(!fileInfo.exists()){
        Utility::showError(this, tr("Error"), tr("Inserted directory does not exists"));
        ui->inputBackupFolder->setText(PresetManager::presetsAndConfig.BackupFolderPath);
        return;
    }
    if(!fileInfo.isDir()){
        Utility::showError(this, tr("Error"), tr("Needs to be a directory"));
        ui->inputBackupFolder->setText(PresetManager::presetsAndConfig.BackupFolderPath);
        return;
    }
    if(fileInfo.isRoot()){
        Utility::showError(this, tr("Error"), tr("Backup Folder can't be the root directory") + " \"" + QDir::root().absolutePath() + "\"");
        ui->inputBackupFolder->setText(PresetManager::presetsAndConfig.BackupFolderPath);
        return;
    }
    if(!fileInfo.isWritable()){
        #ifdef Q_OS_WIN
        Utility::showWarning(this, tr("Warning"), tr("Program does not have permission to write to that directory, run the program as administrator or pick a different directory"));
        #else
        Utility::showWarning(this, tr("Warning"), tr("Program does not have permission to write to that directory, run the program as root or pick a different directory"));
        #endif
        ui->inputBackupFolder->setText(PresetManager::presetsAndConfig.BackupFolderPath);
        return;
    }
    ui->inputBackupFolder->setText(inputtedFolder);
    PresetManager::presetsAndConfig.BackupFolderPath = inputtedFolder;
    PresetManager::Save();
}

void MainWindow::comboBoxPresetsChanged(int index){
    PresetManager::presetsAndConfig.CurrentPresetIndex = index;
    PresetManager::Save();
}

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
        #ifdef Q_OS_WIN
        Utility::showWarning(this, tr("Warning"), tr("Program does not have permission to write to that directory, run the program as administrator or pick a different directory"));
        #else
        Utility::showWarning(this, tr("Warning"), tr("Program does not have permission to write to that directory, run the program as root or pick a different directory"));
        #endif
        return;
    }
    ui->inputBackupFolder->setText(folder);
    PresetManager::presetsAndConfig.BackupFolderPath = folder;
    PresetManager::Save();
}

void MainWindow::btnFilesPressed(){}

void MainWindow::btnBackupPressed(){}

void MainWindow::checkBoxAllPresets(int state){
    PresetManager::presetsAndConfig.backupAllPresets = state;
    PresetManager::Save();
}
