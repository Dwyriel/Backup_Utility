#ifndef PRESETANDCONFIG_H
#define PRESETANDCONFIG_H

#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QtGlobal>
#include <QString>
#include <QList>
#include <QDir>
#include <QFileInfo>
#include <QStringList>

#include <src/utility.h>

struct Preset
{
    QString PresetName;
    qint64 BackupNumber = 1;
    QStringList FilesToSave;
    QStringList FoldersToSave;

    Preset();

    Preset(QString presetName, qint64 backupNumber, QList<QString> filesToSave, QList<QString> FoldersToSave);
};

struct PresetsAndConfig
{
    bool Multithreaded = true;
    bool backupAllPresets = false;
    QString BackupFolderPath;
    int CurrentPresetIndex = 0;
    QList<Preset> Presets;

    PresetsAndConfig();

    PresetsAndConfig(bool multithreaded, QString backupFolderPath, int CurrentPresetIndex, QList<Preset> presets);
};

class PresetManager
{
public:
    static PresetsAndConfig presetsAndConfig;

    PresetManager() = delete;

    ~PresetManager() = delete;

    static void Initialize();

    static Preset CurrentPreset();

    static void Load();

    static void Save();

    static void CheckFilesIntegrity();

    static bool CheckIfFileNameIsValid(QString name);

private:
    static PresetsAndConfig defaultPresetsAndConfig;
    static QString PresetsAndConfigFileName;
    static QDir currentDir;
    static QFileInfo PresetsAndConfigFile;
};

#endif // PRESETANDCONFIG_H
