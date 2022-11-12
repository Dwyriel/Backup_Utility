#include "backupmanager.h"

/*BackupManager*/
BackupManager BackupManager::backupManagerInstance = BackupManager();

BackupManager& BackupManager::Instance(){
    return backupManagerInstance;
}

BackupManager::BackupManager(QWidget *parent) : QObject(parent){
    threadPool = new QThreadPool();
}

BackupManager::~BackupManager(){
    delete threadPool;
}

void BackupManager::Backup(){
    if(ConfigManager::presetsAndConfig.Multithreaded)
        BackupMT(!ConfigManager::presetsAndConfig.BackupAllPresets ? ConfigManager::presetsAndConfig.CurrentPresetIndex : -1);
    else
        BackupST(!ConfigManager::presetsAndConfig.BackupAllPresets ? ConfigManager::presetsAndConfig.CurrentPresetIndex : -1);
}

void BackupManager::BackupMT(const int index){
    bool success = false;

    emit backupComplete(success);
}

bool BackupManager::BackupPresetMT(Preset &presetToBackup){
    return true;
}

bool BackupManager::BackupFileMT(const QDir &backupFolder, const QString &fileToBackup){
    return true;
}

bool BackupManager::BackupEverythingInDirMT(const QDir &backupFolder, const QString folderToBackup){
    return true;
}

void BackupManager::BackupST(const int index){
    bool success = false;
    if(index >= 0){
        success = BackupPresetST(ConfigManager::presetsAndConfig.Presets[index]);
        emit backupComplete(success);
        return;
    }
    for(auto &preset : ConfigManager::presetsAndConfig.Presets){
        success = BackupPresetST(preset);
        if(!success){
            emit backupComplete(success);
            return;
        }
    }
    emit backupComplete(success);
}

bool BackupManager::BackupPresetST(Preset &presetToBackup){
    QDir presetFolder(ConfigManager::presetsAndConfig.BackupFolderPath + QDir::separator() + presetToBackup.PresetName);
    if(!presetFolder.exists())
        if(!presetFolder.mkpath(presetFolder.path()))
            return false;
    QDir backupFolder(presetFolder.path() + QDir::separator() + "Backup " + QString::number(++presetToBackup.BackupNumber));
    if(!backupFolder.exists())
        if(!backupFolder.mkpath(backupFolder.path()))
            return false;
    for(auto &file : presetToBackup.FilesToSave){
        if(!BackupFileST(backupFolder, file))
            return false;
    }
    for(auto &folder : presetToBackup.FoldersToSave){
        if(!BackupEverythingInDirST(backupFolder, folder))
            return false;
    }
    return true;
}

bool BackupManager::BackupFileST(const QDir &backupFolder, const QString &fileToBackup){
    QFileInfo fileInfo(fileToBackup);
    QFileInfo newFileInfo(backupFolder.path() + QDir::separator() + fileInfo.fileName());
    if(newFileInfo.exists())
        QFile::remove(newFileInfo.absoluteFilePath());
    if(!QFile::copy(fileToBackup, newFileInfo.absoluteFilePath()))
        return false;
    return true;
}

bool BackupManager::BackupEverythingInDirST(const QDir &backupFolder, const QString folderToBackup){
    QFileInfo folderInfo(folderToBackup);
    QDir folder(folderToBackup);
    folder.setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);
    QFileInfo newFolderInfo(backupFolder.path() + QDir::separator() + folderInfo.fileName());
    if(newFolderInfo.exists())
        backupFolder.rmpath(newFolderInfo.absoluteFilePath());
    if(!backupFolder.mkpath(newFolderInfo.absoluteFilePath()))
        return false;
    QDir newFolder(newFolderInfo.absoluteFilePath());
    for(auto& entryInfo : folder.entryInfoList()){
        if(entryInfo.isFile())
            if(!BackupFileST(newFolder, entryInfo.absoluteFilePath()))
                return false;
        if(entryInfo.isDir())
            if(!BackupEverythingInDirST(newFolder, entryInfo.absoluteFilePath()))
                return false;
    }
    return true;
}

/*BackupManager::BackupFileTask*/
BackupManager::BackupFileTask::BackupFileTask(const QDir &backupFolder, const QString &fileToBackup) : _backupFolder(backupFolder), _fileToBackup(fileToBackup){
    setAutoDelete(true);
}

void BackupManager::BackupFileTask::run(){
    BackupManager::Instance().BackupFileST(_backupFolder, _fileToBackup);
}
