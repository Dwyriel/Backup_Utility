#include "backupmanager.h"

BackupManager::BackupManager(QWidget *parent) : QObject(parent){}

void BackupManager::Backup(){

    emit backupComplete(false);
}
