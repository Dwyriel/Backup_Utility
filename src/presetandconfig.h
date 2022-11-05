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

    ConfigManager() = delete;

    ~ConfigManager() = delete;

    static void Initialize();

    static Preset& CurrentPreset();

    static void Load();

    static void Save();

    static void Save(int index);

    static void CheckFilesIntegrity();

    static void AddNewPreset(QString presetName);

    static int RemovePresetAt(int index);

    static bool isFileNameValid(QString name);

private:
    static PresetsAndConfig defaultPresetsAndConfig;
    static QString PresetsAndConfigFileName;
    static QDir currentDir;
    static QFileInfo PresetsAndConfigFile;
};

#endif // PRESETANDCONFIG_H
