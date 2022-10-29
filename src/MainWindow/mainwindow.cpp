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

void MainWindow::btnSearchFolderPressed(){}

void MainWindow::btnFilesPressed(){}

void MainWindow::btnBackupPressed(){}

void MainWindow::checkBoxAllPresets(int state){}
