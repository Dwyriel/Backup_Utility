#ifndef BACKUPMANAGER_H
#define BACKUPMANAGER_H

#include <QWidget>
#include <QThreadPool>

#include <src/presetandconfig.h>

class BackupManager : public QObject{
    Q_OBJECT
public:
    [[nodiscard]] static BackupManager& Instance();

    void Backup();

signals:
    void backupComplete(bool wasSuccessful);

private:
    static BackupManager backupManagerInstance;

    QThreadPool *threadPool;

    BackupManager(QWidget *parent = nullptr);

    ~BackupManager();

    void BackupMT(const int index = -1);

    bool BackupPresetMT(Preset &presetToBackup);

    bool BackupEverythingInDirMT(const QDir &backupFolder, const QString &folderToBackup);

    bool BackupFileMT(const QDir &backupFolder, const QString &fileToBackup);

    void BackupST(const int index = -1);

    bool BackupPresetST(Preset &presetToBackup);

    bool BackupEverythingInDirST(const QDir &backupFolder, const QString &folderToBackup);

    bool BackupFileST(const QDir &backupFolder, const QString &fileToBackup);

    class BackupFileTask : public QRunnable
    {
        QDir _backupFolder;
        QString _fileToBackup;
    public:
        BackupFileTask(const QDir &backupFolder, const QString &fileToBackup);

        void run() override;
    };
};

#endif // BACKUPMANAGER_H
