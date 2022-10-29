#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/resources/icon/icon.ico"));
    connect(ui->actionQuit, &QAction::triggered, &QApplication::quit);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::backupBtnPressed(){}

void MainWindow::newPresetBtnPressed(){}

void MainWindow::deletePresetBtnPressed(){}

void MainWindow::searchFolderBtnPressed(){}

void MainWindow::filesBtnPressed(){}
