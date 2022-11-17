#include "presetandconfig.h"

/*Preset*/
Preset::Preset(){}

Preset::Preset(QString presetName) : PresetName(presetName){}

Preset::Preset(QString presetName, qint64 backupNumber, QList<QString> filesToSave, QList<QString> FoldersToSave) : PresetName(presetName), BackupNumber(backupNumber), FilesToSave(filesToSave), FoldersToSave(FoldersToSave){}

/*PresetsAndConfig*/
PresetsAndConfig::PresetsAndConfig(){}

PresetsAndConfig::PresetsAndConfig(bool multithreaded, QString backupFolderPath, int currentPresetIndex, QList<Preset> presets) : Multithreaded(multithreaded), BackupFolderPath(backupFolderPath), CurrentPresetIndex(currentPresetIndex), Presets(presets){}

/*ConfigManager*/
PresetsAndConfig ConfigManager::defaultPresetsAndConfig = PresetsAndConfig(true, "", 0, QList<Preset>());
QString ConfigManager::PresetsAndConfigFileName = "Backup_Utility.conf";
QDir ConfigManager::currentDir;
QFileInfo ConfigManager::PresetsAndConfigFile;
PresetsAndConfig ConfigManager::presetsAndConfig;
bool ConfigManager::fileLoaded = false;

void ConfigManager::Initialize(){
    currentDir = QDir(QCoreApplication::applicationDirPath());
    PresetsAndConfigFile = QFileInfo(ConfigManager::currentDir.absolutePath() + QDir::separator() + ConfigManager::PresetsAndConfigFileName);
    if(PresetsAndConfigFile.exists())
        Load();
    else
        presetsAndConfig = defaultPresetsAndConfig;
    CheckFilesIntegrity();
}

Preset& ConfigManager::CurrentPreset(){
    return presetsAndConfig.Presets[presetsAndConfig.CurrentPresetIndex];
}

void ConfigManager::Load(){
    QFile file(PresetsAndConfigFile.absoluteFilePath());
    if(!file.open(QIODeviceBase::ReadOnly))
        return;
    QByteArray bytes = file.readAll();
    file.close();
    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(bytes, &jsonError);
    if(jsonError.error != QJsonParseError::NoError){
        auto result = Utility::showWarningWithButtons(nullptr, QObject::tr("Error"), QObject::tr("Could not load configuration file, load default settings and continue?"));
        if(result != QMessageBox::Yes)
            exit(1);
        presetsAndConfig = defaultPresetsAndConfig;
        return;
    }
    QJsonObject jsonObj;
    if(doc.isObject())
        jsonObj = doc.object();
    presetsAndConfig.Multithreaded = jsonObj.take("Multithreaded").toBool();
    presetsAndConfig.BackupAllPresets = jsonObj.take("BackupAllPresets").toBool();
    presetsAndConfig.BackupFolderPath = jsonObj.take("BackupFolderPath").toString();
    presetsAndConfig.CurrentPresetIndex = jsonObj.take("CurrentPresetIndex").toInt();
    auto arrValue = jsonObj.take("Presets").toArray();
    while(!arrValue.isEmpty()){
        Preset preset;
        auto presetValue = arrValue.at(0).toObject();
        arrValue.removeAt(0);
        preset.PresetName = presetValue.take("PresetName").toString();
        preset.BackupNumber = presetValue.take("BackupNumber").toInteger();
        auto filesToSaveArr = presetValue.take("FilesToSave").toArray();
        QStringList filesToSaveList;
        while(!filesToSaveArr.isEmpty()){
            filesToSaveList.push_back(std::move(filesToSaveArr.at(0).toString()));
            filesToSaveArr.removeAt(0);
        }
        preset.FilesToSave = filesToSaveList;
        auto foldersToSaveArr = presetValue.take("FoldersToSave").toArray();
        QStringList foldersToSaveList;
        while (!foldersToSaveArr.isEmpty()){
            foldersToSaveList.push_back(std::move(foldersToSaveArr.at(0).toString()));
            foldersToSaveArr.removeAt(0);
        }
        preset.FoldersToSave = foldersToSaveList;
        presetsAndConfig.Presets.push_back(std::move(preset));
    }
    fileLoaded = true;
}

void ConfigManager::Save(int index){
    if(index >= 0)
        presetsAndConfig.CurrentPresetIndex = index;
    QFile file(PresetsAndConfigFile.absoluteFilePath());
    if(!file.open(QIODeviceBase::ReadWrite | QIODeviceBase::Truncate | QIODeviceBase::Text))
        return;
    QJsonObject pacJsonObj;
    pacJsonObj.insert("Multithreaded", presetsAndConfig.Multithreaded);
    pacJsonObj.insert("BackupAllPresets", presetsAndConfig.BackupAllPresets);
    pacJsonObj.insert("BackupFolderPath", presetsAndConfig.BackupFolderPath);
    pacJsonObj.insert("CurrentPresetIndex", presetsAndConfig.CurrentPresetIndex);
    QJsonArray presetsJsonArr;
    for(int i = 0; i < presetsAndConfig.Presets.size(); i++){
        QJsonObject presetObj;
        presetObj.insert("BackupNumber", presetsAndConfig.Presets[i].BackupNumber);
        presetObj.insert("PresetName", presetsAndConfig.Presets[i].PresetName);
        QJsonArray filesToSaveJsonArr;
        for(int j = 0; j < presetsAndConfig.Presets[i].FilesToSave.size(); j++)
            filesToSaveJsonArr.push_back(presetsAndConfig.Presets[i].FilesToSave[j]);
        presetObj.insert("FilesToSave", filesToSaveJsonArr);
        QJsonArray foldersToSaveJsonArr;
        for(int j = 0; j < presetsAndConfig.Presets[i].FoldersToSave.size(); j++)
            foldersToSaveJsonArr.push_back(presetsAndConfig.Presets[i].FoldersToSave[j]);
        presetObj.insert("FoldersToSave", foldersToSaveJsonArr);
        presetsJsonArr.insert(i, presetObj);
    }
    pacJsonObj.insert("Presets", presetsJsonArr);
    QJsonDocument doc;
    doc.setObject(pacJsonObj);
    QByteArray bytes = doc.toJson(QJsonDocument::Indented);
    QTextStream outStream(&file);
    outStream << bytes;
    file.close();
}

void ConfigManager::CheckFilesIntegrity(){
    bool configChanged = false;
    if(!presetsAndConfig.Presets.isEmpty() && presetsAndConfig.CurrentPresetIndex >= presetsAndConfig.Presets.size()){
        presetsAndConfig.CurrentPresetIndex = presetsAndConfig.Presets.size() - 1;
        configChanged = true;
    }
    if(presetsAndConfig.CurrentPresetIndex < 0){
        presetsAndConfig.CurrentPresetIndex = 0;
        configChanged = true;
    }
    if(presetsAndConfig.Presets.isEmpty() && presetsAndConfig.CurrentPresetIndex != 0){
        presetsAndConfig.CurrentPresetIndex = 0;
        configChanged = true;
    }
    if(presetsAndConfig.BackupFolderPath != ""){
        QFileInfo backupFolder(presetsAndConfig.BackupFolderPath);
        if(!backupFolder.exists() || !backupFolder.isDir()){
            presetsAndConfig.BackupFolderPath = "";
            configChanged = true;
        }
    }
    QList<int> indexOfPresetsWithInvalidName;
    for(int i = 0; i < presetsAndConfig.Presets.size(); i++){
        if(!isFileNameValid(presetsAndConfig.Presets[i].PresetName)){
            indexOfPresetsWithInvalidName.push_back(i);//! Not using push_front as push_back is always constant time
            continue;
        }
        QList<int> indexOfInvalidFiles;
        for(int j = 0; j < presetsAndConfig.Presets[i].FilesToSave.size(); j++){
            QFileInfo fileInfo(presetsAndConfig.Presets[i].FilesToSave[j]);
            if(!fileInfo.exists() || !fileInfo.isFile())
                indexOfInvalidFiles.push_back(j);
        }
        if(!indexOfInvalidFiles.isEmpty()){
            for(int index = indexOfInvalidFiles.size() - 1; index >= 0; index--)
                presetsAndConfig.Presets[i].FilesToSave.removeAt(indexOfInvalidFiles[index]);
            configChanged = true;
        }
        QList<int> indexOfInvalidFolders;
        for(int j = 0; j < presetsAndConfig.Presets[i].FoldersToSave.size(); j++){
            QFileInfo folderInfo(presetsAndConfig.Presets[i].FoldersToSave[j]);
            if(!folderInfo.exists() || !folderInfo.isDir())
                indexOfInvalidFolders.push_back(j);
        }
        if(!indexOfInvalidFolders.isEmpty()){
            for(int index = indexOfInvalidFolders.size() - 1; index >= 0; index--)
                presetsAndConfig.Presets[i].FoldersToSave.removeAt(indexOfInvalidFolders[index]);
            configChanged = true;
        }
    }
    if(!indexOfPresetsWithInvalidName.isEmpty()){
        for(int i = indexOfPresetsWithInvalidName.size() - 1; i >= 0; i--)
            presetsAndConfig.Presets.removeAt(indexOfPresetsWithInvalidName[i]);
        configChanged = true;
    }
    if(configChanged)
        Save();
}

void ConfigManager::AddNewPreset(QString presetName){
    presetsAndConfig.Presets.push_back(Preset(presetName));
    presetsAndConfig.CurrentPresetIndex = presetsAndConfig.Presets.size() - 1;
}

void ConfigManager::RemovePresetAt(int index){
    if(presetsAndConfig.Presets.isEmpty())
        return;
    presetsAndConfig.CurrentPresetIndex = index;
    presetsAndConfig.Presets.removeAt(presetsAndConfig.CurrentPresetIndex);
    presetsAndConfig.CurrentPresetIndex = (presetsAndConfig.CurrentPresetIndex >= presetsAndConfig.Presets.size()) ? (presetsAndConfig.Presets.size() - 1) : presetsAndConfig.CurrentPresetIndex;
}

bool ConfigManager::isFileNameValid(QString name){
#ifdef Q_OS_WIN
    return !(name.contains(R"(/)") || name.contains(R"(\)") || name.contains(R"(:)") || name.contains(R"(?)") || name.contains(R"(")") || name.contains(R"(<)") || name.contains(R"(>)") || name.contains(R"(|)") || name.contains(R"(*)"));
#else
    return !name.contains(R"(/)");
#endif
}

bool ConfigManager::isThereItemsToSave(){
    if(presetsAndConfig.Presets.isEmpty())
        return false;
    if(!presetsAndConfig.BackupAllPresets)
        return (!CurrentPreset().FilesToSave.isEmpty() || !CurrentPreset().FoldersToSave.isEmpty());
    bool filesToSave = false;
    for(auto& preset : presetsAndConfig.Presets){
        filesToSave = (!preset.FilesToSave.isEmpty() || !preset.FoldersToSave.isEmpty());
        if(filesToSave)
            break;
    }
    return filesToSave;
}
