#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    setWindowIcon(QIcon(":/resources/icon/icon.ico"));
    ConfigManager::Initialize();
    setWidgetVisibility();
    setWidgetValues();
    connectSignals();
}

MainWindow::~MainWindow(){
    ConfigManager::Save(ui->comboBoxPresets->currentIndex());
    delete ui;
}

void MainWindow::setWidgetVisibility(){
    ui->actionAll_presets->setVisible(!ui->menubar->isNativeMenuBar());
    ui->checkBoxAllPresets->setVisible(ui->menubar->isNativeMenuBar());
}

void MainWindow::setWidgetValues(){
    ui->actionMultithreaded->setChecked(ConfigManager::presetsAndConfig.Multithreaded);
    ui->actionAll_presets->setChecked(ConfigManager::presetsAndConfig.BackupAllPresets);
    ui->inputBackupFolder->setText(ConfigManager::presetsAndConfig.BackupFolderPath);
    ui->checkBoxAllPresets->setChecked(ConfigManager::presetsAndConfig.BackupAllPresets);
    ui->comboBoxPresets->clear();
    for(const auto &item : ConfigManager::presetsAndConfig.Presets)
        ui->comboBoxPresets->addItem(item.PresetName);
    ui->comboBoxPresets->setCurrentIndex(ui->comboBoxPresets->count() > 0 ? ConfigManager::presetsAndConfig.CurrentPresetIndex : -1);
    setWidgetEnabled();
}

void MainWindow::connectSignals(){
    connect(ui->actionQuit, &QAction::triggered, &QApplication::quit);
    connect(ui->actionMultithreaded, &QAction::toggled, this, &MainWindow::actionMultithreadedToggled);
    connect(ui->actionAll_presets, &QAction::toggled, this, &MainWindow::actionAllPresetsToggled);
    connect(ui->inputBackupFolder, &CustomLineEdit::lostFocus, this, &MainWindow::inputBackupFolderLostFocus);
    connect(ui->btnNewPreset, &QPushButton::clicked, this, &MainWindow::btnNewPresetClicked);
    connect(ui->BtnDeletePreset, &QPushButton::clicked, this, &MainWindow::btnDeletePresetClicked);
    connect(ui->btnSearch, &QPushButton::clicked, this, &MainWindow::btnSearchFolderClicked);
    connect(ui->btnFiles, &QPushButton::clicked, this, &MainWindow::btnFilesClicked);
    connect(ui->btnBackup, &QPushButton::clicked, this, &MainWindow::btnBackupClicked);
    connect(ui->checkBoxAllPresets, &QCheckBox::stateChanged, this, &MainWindow::checkBoxAllPresetsStateChanged);
}

void MainWindow::setWidgetEnabled(){
    ui->actionQuit->setDisabled(isBackupInProgress);
    ui->actionMultithreaded->setDisabled(isBackupInProgress);
    ui->actionAll_presets->setDisabled(isBackupInProgress);
    ui->inputBackupFolder->setDisabled(isBackupInProgress);
    ui->comboBoxPresets->setDisabled(isBackupInProgress);
    ui->btnNewPreset->setDisabled(isBackupInProgress);
    ui->BtnDeletePreset->setDisabled(isBackupInProgress || ui->comboBoxPresets->count() < 1);
    ui->btnSearch->setDisabled(isBackupInProgress);
    ui->btnFiles->setDisabled(isBackupInProgress || ui->comboBoxPresets->count() < 1);
    ui->btnBackup->setDisabled(isBackupInProgress);
    ui->checkBoxAllPresets->setDisabled(isBackupInProgress);
}

void MainWindow::actionMultithreadedToggled(bool checked){
    ConfigManager::presetsAndConfig.Multithreaded = checked;
    ConfigManager::Save(ui->comboBoxPresets->currentIndex());
}

void MainWindow::actionAllPresetsToggled(bool checked){
    ConfigManager::presetsAndConfig.BackupAllPresets = checked;
    ConfigManager::Save(ui->comboBoxPresets->currentIndex());
}

void MainWindow::inputBackupFolderLostFocus(){
    QString inputtedFolder = ui->inputBackupFolder->text().trimmed();
    if(inputtedFolder == "")
        return;
    QFileInfo fileInfo(inputtedFolder);
    if(!fileInfo.exists()){
        Utility::showError(this, tr("Error"), tr("Inserted directory does not exists"));
        ui->inputBackupFolder->setText(ConfigManager::presetsAndConfig.BackupFolderPath);
        return;
    }
    if(!fileInfo.isDir()){
        Utility::showError(this, tr("Error"), tr("Needs to be a directory"));
        ui->inputBackupFolder->setText(ConfigManager::presetsAndConfig.BackupFolderPath);
        return;
    }
    if(fileInfo.isRoot()){
        Utility::showError(this, tr("Error"), tr("Backup Folder can't be the root directory") + " \"" + QDir::root().absolutePath() + "\"");
        ui->inputBackupFolder->setText(ConfigManager::presetsAndConfig.BackupFolderPath);
        return;
    }
    if(!fileInfo.isWritable()){
        #ifdef Q_OS_WIN
        Utility::showWarning(this, tr("Warning"), tr("Program does not have permission to write to that directory, run the program as administrator or pick a different directory"));
        #else
        Utility::showWarning(this, tr("Warning"), tr("Program does not have permission to write to that directory, run the program as root or pick a different directory"));
        #endif
        ui->inputBackupFolder->setText(ConfigManager::presetsAndConfig.BackupFolderPath);
        return;
    }
    ui->inputBackupFolder->setText(inputtedFolder);
    ConfigManager::presetsAndConfig.BackupFolderPath = inputtedFolder;
    ConfigManager::Save(ui->comboBoxPresets->currentIndex());
}

void MainWindow::btnNewPresetClicked(){
    InputDialog dialog(tr("New preset name:"), this);
    if(!dialog.exec())
        return;
    if(dialog.OutputString == "")
        return;
    if(!ConfigManager::isFileNameValid(dialog.OutputString)){
        #ifdef Q_OS_WIN
        Utility::showWarning(this, tr("Warning"), tr("Invalid name, cannot contain / \\ : ? \" < > | *"));
        #else
        Utility::showWarning(this, tr("Warning"), tr("Invalid name, cannot contain /"));
        #endif
        btnNewPresetClicked();
        return;
    }
    ConfigManager::AddNewPreset(dialog.OutputString);
    setWidgetValues();
    ConfigManager::Save(ui->comboBoxPresets->currentIndex());
}

void MainWindow::btnDeletePresetClicked(){
    auto result = Utility::showWarningWithButtons(nullptr, QObject::tr("Confirm"), QObject::tr("Are you sure you want to delete this preset?"));
    if(result != QMessageBox::Yes)
        return;
    ConfigManager::RemovePresetAt(ui->comboBoxPresets->currentIndex());
    setWidgetValues();
    ConfigManager::Save(ui->comboBoxPresets->currentIndex());
}

void MainWindow::btnSearchFolderClicked(){
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
    ConfigManager::presetsAndConfig.BackupFolderPath = folder;
    ConfigManager::Save(ui->comboBoxPresets->currentIndex());
}

void MainWindow::btnFilesClicked(){}//use index from combobox

void MainWindow::btnBackupClicked(){}//use index from combobox

void MainWindow::checkBoxAllPresetsStateChanged(int state){
    ConfigManager::presetsAndConfig.BackupAllPresets = state;
    ConfigManager::Save(ui->comboBoxPresets->currentIndex());
}
