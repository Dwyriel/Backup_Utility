#ifndef PRESETANDCONFIG_H
#define PRESETANDCONFIG_H
#include <QCoreApplication>
#include <QString>
#include <QList>
#include <QDir>

struct Preset
{
    QString PresetName;
    unsigned long BackupNumber = 1;
    QList<QString> FilesToSave;
    QList<QString> FoldersToSave;
};

struct PresetsAndConfig
{
    bool Multithreaded = true;
    QString BackupFolderPath;
    int CurrentPresetIndex = 0;
    QList<Preset> Presets;
};

class PresetManager
{
public:
    PresetManager() = delete;
    ~PresetManager() = delete;

    static void Initialize();

    //static Preset CurrentPreset();

    //static PresetsAndConfig PresetsAndConfig;

private:
    static QString PresetsAndConfigFileName;
    static QDir currentDir;
    static QString PresetsAndConfigFilePath;

};

#endif // PRESETANDCONFIG_H
