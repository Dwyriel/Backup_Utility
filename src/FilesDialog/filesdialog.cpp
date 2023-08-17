#include "filesdialog.h"
#include "./ui_filesdialog.h"

FilesDialog::FilesDialog(QWidget *parent) : QDialog(parent), ui(new Ui::FilesDialog){
    ui->setupUi(this);
    setFileList();
    setFolderList();
    ui->inputBackup->setText(QString::number(ConfigManager::CurrentPreset().BackupNumber));
    connectSignals();
}

FilesDialog::~FilesDialog(){
    delete ui;
}

void FilesDialog::setFileList(){
    ui->listFiles->clear();
    for(int i = 0; i < ConfigManager::CurrentPreset().FilesToSave.size(); i++){
        QListWidgetItem *item = new QListWidgetItem(ConfigManager::CurrentPreset().FilesToSave[i], nullptr, QListWidgetItem::UserType);
        item->setData(Qt::UserRole, i);
        ui->listFiles->addItem(item);
    }
}

void FilesDialog::setFolderList(){
    ui->listFolders->clear();
    for(int i = 0; i < ConfigManager::CurrentPreset().FoldersToSave.size(); i++){
        QListWidgetItem *item = new QListWidgetItem(ConfigManager::CurrentPreset().FoldersToSave[i], nullptr, QListWidgetItem::UserType);
        item->setData(Qt::UserRole, i);
        ui->listFolders->addItem(item);
    }
}

void FilesDialog::connectSignals(){
    connect(ui->btnAddFile, &QPushButton::clicked, this, &FilesDialog::btnAddFileClicked);
    connect(ui->btnAddFolder, &QPushButton::clicked, this, &FilesDialog::btnAddFolderClicked);
    connect(ui->btnClearFile, &QPushButton::clicked, this, &FilesDialog::btnClearFileClicked);
    connect(ui->btnClearFolder, &QPushButton::clicked, this, &FilesDialog::btnClearFolderClicked);
    connect(ui->btnClose, &QPushButton::clicked, this, &FilesDialog::btnCloseClicked);
    connect(ui->btnRemoveFile, &QPushButton::clicked, this, &FilesDialog::btnRemoveFileClicked);
    connect(ui->btnRemoveFolder, &QPushButton::clicked, this, &FilesDialog::btnRemoveFolderClicked);
    connect(ui->btnResetBackups, &QPushButton::clicked, this, &FilesDialog::btnResetBackupsClicked);
}

void FilesDialog::btnAddFileClicked(){
    QFileDialog qFileDialog(this);
    qFileDialog.setWindowTitle(tr("Select Files"));
    qFileDialog.setDirectory(QDir::homePath());
    Utility::SetMultiFileDialog(qFileDialog);
    if(qFileDialog.exec() != QDialog::Accepted)
        return;
    auto files = qFileDialog.selectedFiles();
    for(const auto &file : files)
        if(!ConfigManager::CurrentPreset().FilesToSave.contains(file))
            ConfigManager::CurrentPreset().FilesToSave.push_back(file);
    setFileList();
}

void FilesDialog::btnAddFolderClicked(){
    QFileDialog qFileDialog(this);
    qFileDialog.setWindowTitle(tr("Select Folder"));
    qFileDialog.setDirectory(QDir::homePath());
    Utility::SetSingleDirectoryDialog(qFileDialog);
    if(qFileDialog.exec() != QDialog::Accepted)
        return;
    auto folders = qFileDialog.selectedFiles();
    for(const auto &folder : folders)
        if(!ConfigManager::CurrentPreset().FoldersToSave.contains(folder))
            ConfigManager::CurrentPreset().FoldersToSave.push_back(folder);
    setFolderList();
}

void FilesDialog::btnClearFileClicked(){
    if(ui->listFiles->count() < 1)
        return;
    auto result = Utility::showWarningWithButtons(nullptr, QObject::tr("Confirm"), QObject::tr("Are you sure you want to delete all items from this list?"));
    if(result != QMessageBox::Yes)
        return;
    ConfigManager::CurrentPreset().FilesToSave.clear();
    setFileList();
}

void FilesDialog::btnClearFolderClicked(){
    if(ui->listFolders->count() < 1)
        return;
    auto result = Utility::showWarningWithButtons(nullptr, QObject::tr("Confirm"), QObject::tr("Are you sure you want to delete all items from this list?"));
    if(result != QMessageBox::Yes)
        return;
    ConfigManager::CurrentPreset().FoldersToSave.clear();
    setFolderList();
}

void FilesDialog::btnCloseClicked(){
    done(QDialog::Accepted);
}

void FilesDialog::btnRemoveFileClicked(){
    auto itemList = ui->listFiles->selectedItems();
    if(itemList.size() < 1)
        return;
    QListWidgetItem* itemPointer;
    for(const auto &item : itemList)
        itemPointer = item;
    ConfigManager::CurrentPreset().FilesToSave.removeAt(itemPointer->data(Qt::UserRole).toInt());
    setFileList();
}

void FilesDialog::btnRemoveFolderClicked(){
    auto itemList = ui->listFolders->selectedItems();
    if(itemList.size() < 1)
        return;
    QListWidgetItem* itemPointer;
    for(const auto &item : itemList)
        itemPointer = item;
    ConfigManager::CurrentPreset().FoldersToSave.removeAt(itemPointer->data(Qt::UserRole).toInt());
    setFolderList();
}

void FilesDialog::btnResetBackupsClicked(){
    ConfigManager::CurrentPreset().BackupNumber = 0;
    ui->inputBackup->setText(QString::number(ConfigManager::CurrentPreset().BackupNumber));
}
