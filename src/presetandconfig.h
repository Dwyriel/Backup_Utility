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

struct Preset {
    QString PresetName;
    qint64 BackupNumber = 0;
    QStringList FilesToSave;
    QStringList FoldersToSave;

    Preset();

    explicit Preset(QString presetName);

    explicit Preset(QString presetName, qint64 backupNumber, QStringList filesToSave, QStringList FoldersToSave);
};

struct PresetsAndConfig {
    bool Multithreaded = true;
    bool BackupAllPresets = false;
    QString BackupFolderPath;
    qint64 CurrentPresetIndex = 0;
    QList<Preset> Presets;

    PresetsAndConfig();
};

class ConfigManager {
    static PresetsAndConfig defaultPresetsAndConfig;
    static QString PresetsAndConfigFileName;
    static QFileInfo PresetsAndConfigFile;

    static void errorLoadingConfig();

public:
    static PresetsAndConfig presetsAndConfig;

    static bool fileLoaded;

    ConfigManager() = delete;

    ~ConfigManager() = delete;

    static void Initialize();

    static Preset &CurrentPreset();

    static void Load();

    static void Save(int index = -1);

    static void CheckFilesIntegrity();

    static void AddNewPreset(const QString &presetName);

    static void RemovePresetAt(int index);

    static bool isFileNameValid(const QString &name);

    static bool isThereItemsToSave();

    static bool doesPresetAlreadyExists(const QString &presetName);
};

#endif // PRESETANDCONFIG_H
