#ifndef PRESETANDCONFIG_H
#define PRESETANDCONFIG_H

#include <QCoreApplication>
#include <QStandardPaths>
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
    qint64 BackupNumber = 0;
    QStringList FilesToSave;
    QStringList FoldersToSave;

    Preset();

    Preset(QString presetName);

    Preset(QString presetName, qint64 backupNumber, QList<QString> filesToSave, QList<QString> FoldersToSave);
};

struct PresetsAndConfig
{
    bool Multithreaded = true;
    bool BackupAllPresets = false;
    QString BackupFolderPath;
    int CurrentPresetIndex = 0;
    QList<Preset> Presets;

    PresetsAndConfig();

    PresetsAndConfig(bool multithreaded, QString backupFolderPath, int CurrentPresetIndex, QList<Preset> presets);
};

class ConfigManager
{
public:
    static PresetsAndConfig presetsAndConfig;

    static bool fileLoaded;

    ConfigManager() = delete;

    ~ConfigManager() = delete;

    static void Initialize();

    static Preset& CurrentPreset();

    static void Load();

    static void Save(int index = -1);

    static void CheckFilesIntegrity();

    static void AddNewPreset(QString presetName);

    static void RemovePresetAt(int index);

    static bool isFileNameValid(QString name);

    static bool isThereItemsToSave();

private:
    static PresetsAndConfig defaultPresetsAndConfig;
    static QString PresetsAndConfigFileName;
    static QFileInfo PresetsAndConfigFile;
};

#endif // PRESETANDCONFIG_H
