#ifndef BACKUPMANAGER_H
#define BACKUPMANAGER_H

#include <QWidget>

class BackupManager : public QObject{
    Q_OBJECT
public:

    BackupManager(QWidget *parent = nullptr);

    void Backup();

signals:

    void backupComplete(bool wasSuccessful);
};

#endif // BACKUPMANAGER_H
