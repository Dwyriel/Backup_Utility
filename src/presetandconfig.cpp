#include "presetandconfig.h"

#include <utility>

/*Json key names*/
const char *Multithreaded = "Multithreaded";
const char *BackupAllPresets = "BackupAllPresets";
const char *BackupFolderPath = "BackupFolderPath";
const char *CurrentPresetIndex = "CurrentPresetIndex";
const char *Presets = "Presets";
const char *PresetName = "PresetName";
const char *BackupNumber = "BackupNumber";
const char *FilesToSave = "FilesToSave";
const char *FoldersToSave = "FoldersToSave";

/*Preset*/
Preset::Preset() = default;

Preset::Preset(QString presetName) : PresetName(std::move(presetName)) {}

Preset::Preset(QString presetName, qint64 backupNumber, QStringList filesToSave, QStringList FoldersToSave) : PresetName(std::move(presetName)), BackupNumber(backupNumber), FilesToSave(std::move(filesToSave)), FoldersToSave(std::move(FoldersToSave)) {}

/*PresetsAndConfig*/
PresetsAndConfig::PresetsAndConfig() = default;

/*ConfigManager*/
PresetsAndConfig ConfigManager::defaultPresetsAndConfig = PresetsAndConfig();
QString ConfigManager::PresetsAndConfigFileName = "Backup_Utility.conf";
QFileInfo ConfigManager::PresetsAndConfigFile;
PresetsAndConfig ConfigManager::presetsAndConfig;
bool ConfigManager::fileLoaded = false;

void ConfigManager::errorLoadingConfig() {
    auto result = Utility::showWarningWithButtons(nullptr, QObject::tr("Error"), QObject::tr("Could not load configuration file, load default settings and continue?"));
    if (result != QMessageBox::Yes)
        exit(1);
    presetsAndConfig = defaultPresetsAndConfig;
}

void ConfigManager::Initialize() {
    PresetsAndConfigFile = QFileInfo(QStandardPaths::writableLocation(QStandardPaths::StandardLocation::ConfigLocation) + QDir::separator() + ConfigManager::PresetsAndConfigFileName);
    if (PresetsAndConfigFile.exists())
        Load();
    else
        presetsAndConfig = defaultPresetsAndConfig;
    CheckFilesIntegrity();
}

Preset &ConfigManager::CurrentPreset() {
    return presetsAndConfig.Presets[presetsAndConfig.CurrentPresetIndex];
}

void ConfigManager::Load() {
    QFile file(PresetsAndConfigFile.absoluteFilePath());
    if (!file.open(QIODeviceBase::ReadOnly))
        return errorLoadingConfig();
    QByteArray bytes = file.readAll();
    file.close();
    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(bytes, &jsonError);
    if (jsonError.error != QJsonParseError::NoError || !doc.isObject())
        return errorLoadingConfig();
    QJsonObject jsonObj = doc.object();
    presetsAndConfig.Multithreaded = jsonObj.take(Multithreaded).toBool(defaultPresetsAndConfig.Multithreaded);
    presetsAndConfig.BackupAllPresets = jsonObj.take(BackupAllPresets).toBool(defaultPresetsAndConfig.BackupAllPresets);
    presetsAndConfig.BackupFolderPath = jsonObj.take(BackupFolderPath).toString();
    presetsAndConfig.CurrentPresetIndex = jsonObj.take(CurrentPresetIndex).toInt();
    auto arrValue = jsonObj.take(Presets).toArray();
    presetsAndConfig.Presets.reserve(arrValue.size());
    while (!arrValue.isEmpty()) {
        Preset preset;
        auto presetValue = arrValue.takeAt(0).toObject();
        preset.PresetName = presetValue.take(PresetName).toString();
        preset.BackupNumber = presetValue.take(BackupNumber).toInteger();
        auto filesToSaveArr = presetValue.take(FilesToSave).toArray();
        QStringList &filesToSaveList = preset.FilesToSave;
        filesToSaveList.reserve(filesToSaveArr.size());
        while (!filesToSaveArr.isEmpty())
            filesToSaveList.append(filesToSaveArr.takeAt(0).toString());
        auto foldersToSaveArr = presetValue.take(FoldersToSave).toArray();
        QStringList &foldersToSaveList = preset.FoldersToSave;
        foldersToSaveList.reserve(foldersToSaveArr.size());
        while (!foldersToSaveArr.isEmpty())
            foldersToSaveList.append(foldersToSaveArr.takeAt(0).toString());
        presetsAndConfig.Presets.append(std::move(preset));
    }
    fileLoaded = true;
}

void ConfigManager::Save(int index) {
    if (index >= 0)
        presetsAndConfig.CurrentPresetIndex = index;
    QFile file(PresetsAndConfigFile.absoluteFilePath());
    if (!file.open(QIODeviceBase::ReadWrite | QIODeviceBase::Truncate | QIODeviceBase::Text))
        return;
    QJsonObject pacJsonObj;
    pacJsonObj.insert(Multithreaded, presetsAndConfig.Multithreaded);
    pacJsonObj.insert(BackupAllPresets, presetsAndConfig.BackupAllPresets);
    pacJsonObj.insert(BackupFolderPath, presetsAndConfig.BackupFolderPath);
    pacJsonObj.insert(CurrentPresetIndex, presetsAndConfig.CurrentPresetIndex);
    QJsonArray presetsJsonArr;
    for (int i = 0; i < presetsAndConfig.Presets.size(); i++) {
        QJsonObject presetObj;
        presetObj.insert(BackupNumber, presetsAndConfig.Presets[i].BackupNumber);
        presetObj.insert(PresetName, presetsAndConfig.Presets[i].PresetName);
        QJsonArray filesToSaveJsonArr;
        for (const auto &fileToSave: presetsAndConfig.Presets[i].FilesToSave)
            filesToSaveJsonArr.append(fileToSave);
        presetObj.insert(FilesToSave, filesToSaveJsonArr);
        QJsonArray foldersToSaveJsonArr;
        for (const auto &folderToSave: presetsAndConfig.Presets[i].FoldersToSave)
            foldersToSaveJsonArr.append(folderToSave);
        presetObj.insert(FoldersToSave, foldersToSaveJsonArr);
        presetsJsonArr.insert(i, presetObj);
    }
    pacJsonObj.insert(Presets, presetsJsonArr);
    QJsonDocument doc;
    doc.setObject(pacJsonObj);
    QByteArray bytes = doc.toJson(QJsonDocument::Indented);
    QTextStream outStream(&file);
    outStream << bytes;
    file.close();
}

void ConfigManager::CheckFilesIntegrity() {
    bool configChanged = false;
    if (!presetsAndConfig.Presets.isEmpty() && presetsAndConfig.CurrentPresetIndex >= presetsAndConfig.Presets.size()) {
        presetsAndConfig.CurrentPresetIndex = presetsAndConfig.Presets.size() - 1;
        configChanged = true;
    }
    if (presetsAndConfig.CurrentPresetIndex < 0) {
        presetsAndConfig.CurrentPresetIndex = 0;
        configChanged = true;
    }
    if (presetsAndConfig.Presets.isEmpty() && presetsAndConfig.CurrentPresetIndex != 0) {
        presetsAndConfig.CurrentPresetIndex = 0;
        configChanged = true;
    }
    if (!presetsAndConfig.BackupFolderPath.isEmpty()) {
        QFileInfo backupFolder(presetsAndConfig.BackupFolderPath);
        if (!backupFolder.exists() || !backupFolder.isDir()) {
            presetsAndConfig.BackupFolderPath = "";
            configChanged = true;
        }
    }
    QList<int> indexOfPresetsWithInvalidName;
    for (int i = 0; i < presetsAndConfig.Presets.size(); i++) {
        if (!isFileNameValid(presetsAndConfig.Presets[i].PresetName)) {
            indexOfPresetsWithInvalidName.push_back(i);
            continue;
        }
        QList<int> indexOfInvalidFiles;
        for (int j = 0; j < presetsAndConfig.Presets[i].FilesToSave.size(); j++) {
            QFileInfo fileInfo(presetsAndConfig.Presets[i].FilesToSave[j]);
            if (!fileInfo.exists() || !fileInfo.isFile())
                indexOfInvalidFiles.push_back(j);
        }
        if (!indexOfInvalidFiles.isEmpty()) {
            for (qint64 index = indexOfInvalidFiles.size() - 1; index >= 0; index--)
                presetsAndConfig.Presets[i].FilesToSave.removeAt(indexOfInvalidFiles[index]);
            configChanged = true;
        }
        QList<int> indexOfInvalidFolders;
        for (int j = 0; j < presetsAndConfig.Presets[i].FoldersToSave.size(); j++) {
            QFileInfo folderInfo(presetsAndConfig.Presets[i].FoldersToSave[j]);
            if (!folderInfo.exists() || !folderInfo.isDir())
                indexOfInvalidFolders.push_back(j);
        }
        if (!indexOfInvalidFolders.isEmpty()) {
            for (qint64 index = indexOfInvalidFolders.size() - 1; index >= 0; index--)
                presetsAndConfig.Presets[i].FoldersToSave.removeAt(indexOfInvalidFolders[index]);
            configChanged = true;
        }
    }
    //todo ask before removing anything from presets (or the preset itself)
    if (!indexOfPresetsWithInvalidName.isEmpty()) {
        for (qint64 i = indexOfPresetsWithInvalidName.size() - 1; i >= 0; i--)
            presetsAndConfig.Presets.removeAt(indexOfPresetsWithInvalidName[i]);
        configChanged = true;
    }
    if (configChanged)
        Save();
}

void ConfigManager::AddNewPreset(const QString &presetName) {
    presetsAndConfig.Presets.push_back(Preset(presetName));
    presetsAndConfig.CurrentPresetIndex = presetsAndConfig.Presets.size() - 1;
}

void ConfigManager::RemovePresetAt(int index) {
    if (presetsAndConfig.Presets.isEmpty())
        return;
    presetsAndConfig.CurrentPresetIndex = index;
    presetsAndConfig.Presets.removeAt(presetsAndConfig.CurrentPresetIndex);
    presetsAndConfig.CurrentPresetIndex = (presetsAndConfig.CurrentPresetIndex >= presetsAndConfig.Presets.size()) ? (presetsAndConfig.Presets.size() - 1) : presetsAndConfig.CurrentPresetIndex;
}

bool ConfigManager::isFileNameValid(const QString &name) {
#ifdef Q_OS_WIN
    return name.contains(QRegularExpression(R"(^[^<\/*?"\\>:|]+$)"));
#else
    return !name.contains('/');
#endif
}

bool ConfigManager::isThereItemsToSave() {
    if (presetsAndConfig.Presets.isEmpty())
        return false;
    if (!presetsAndConfig.BackupAllPresets)
        return (!CurrentPreset().FilesToSave.isEmpty() || !CurrentPreset().FoldersToSave.isEmpty());
    bool filesToSave = false;
    for (auto &preset: presetsAndConfig.Presets) {
        filesToSave = (!preset.FilesToSave.isEmpty() || !preset.FoldersToSave.isEmpty());
        if (filesToSave)
            break;
    }
    return filesToSave;
}

bool ConfigManager::doesPresetAlreadyExists(const QString &presetName) {
    for (auto &preset: presetsAndConfig.Presets)
        if (preset.PresetName == presetName)
            return true;
    return false;
}
