#ifndef BACKUPMANAGER_H
#define BACKUPMANAGER_H

#include <QWidget>
#include <QThreadPool>
#include <QtConcurrent>

#include <src/presetandconfig.h>

class BackupManager : public QObject {
Q_OBJECT

public:
    [[nodiscard]] static BackupManager &Instance();

    void Backup();

signals:

    void backupComplete(bool wasSuccessful);

private:
    static BackupManager backupManagerInstance;

    QThreadPool *threadPool;

    explicit BackupManager(QWidget *parent = nullptr);

    ~BackupManager() override;

    void BackupMT(qint64 index = -1);

    bool BackupPresetMT(Preset &presetToBackup, QList<QFuture<bool>> &tasks);

    bool BackupEverythingInDirMT(const QDir &backupFolder, const QString &folderToBackup, QList<QFuture<bool>> &tasks);

    void BackupST(qint64 index = -1);

    bool BackupPresetST(Preset &presetToBackup);

    bool BackupEverythingInDirST(const QDir &backupFolder, const QString &folderToBackup);

    bool BackupFile(const QDir &backupFolder, const QString &fileToBackup);
};

#endif // BACKUPMANAGER_H
