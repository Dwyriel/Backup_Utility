#ifndef BACKUPMANAGER_H
#define BACKUPMANAGER_H

#include <QWidget>
#include <QThreadPool>

#include <src/presetandconfig.h>

class BackupManager : public QObject{
    Q_OBJECT
public:

    BackupManager(QWidget *parent = nullptr);

    void Backup();

private:
    void BackupMT(const int index = -1);

    bool BackupPresetMT(Preset &presetToBackup);

    bool BackupFileMT(const QDir &backupFolder, const QString &fileToBackup);

    bool BackupEverythingInDirMT(const QDir &backupFolder, const QString folderToBackup);

    void BackupST(const int index = -1);

    bool BackupPresetST(Preset &presetToBackup);

    bool BackupFileST(const QDir &backupFolder, const QString &fileToBackup);

    bool BackupEverythingInDirST(const QDir &backupFolder, const QString folderToBackup);

signals:

    void backupComplete(bool wasSuccessful);
};

#endif // BACKUPMANAGER_H
