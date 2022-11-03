#include "presetandconfig.h"

/*Preset*/
Preset::Preset(){}

Preset::Preset(QString presetName) : PresetName(presetName), BackupNumber(1), FilesToSave(QStringList()), FoldersToSave(QStringList()){}

Preset::Preset(QString presetName, qint64 backupNumber, QList<QString> filesToSave, QList<QString> FoldersToSave) : PresetName(presetName), BackupNumber(backupNumber), FilesToSave(filesToSave), FoldersToSave(FoldersToSave){}

/*PresetsAndConfig*/
PresetsAndConfig::PresetsAndConfig(){}

PresetsAndConfig::PresetsAndConfig(bool multithreaded, QString backupFolderPath, int currentPresetIndex, QList<Preset> presets) : Multithreaded(multithreaded), BackupFolderPath(backupFolderPath), CurrentPresetIndex(currentPresetIndex), Presets(presets){}

/*ConfigManager*/
PresetsAndConfig ConfigManager::defaultPresetsAndConfig = PresetsAndConfig(true, "", 0, QList<Preset>());
QString ConfigManager::PresetsAndConfigFileName = "Backup_Utility.data";
QDir ConfigManager::currentDir;
QFileInfo ConfigManager::PresetsAndConfigFile;
PresetsAndConfig ConfigManager::presetsAndConfig;

void ConfigManager::Initialize(){
    currentDir = QDir(QCoreApplication::applicationDirPath());
    PresetsAndConfigFile = QFileInfo(ConfigManager::currentDir.absolutePath() + QDir::separator() + ConfigManager::PresetsAndConfigFileName);
    if(PresetsAndConfigFile.exists())
        Load();
    else
        presetsAndConfig = defaultPresetsAndConfig;
    CheckFilesIntegrity();
}

Preset ConfigManager::CurrentPreset(){ return Preset(); }//TODO

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
}

void ConfigManager::Save(){
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

void ConfigManager::Save(int index){
    presetsAndConfig.CurrentPresetIndex = index;
    Save();
}

void ConfigManager::CheckFilesIntegrity(){
    bool configChanged = false;
    if(presetsAndConfig.CurrentPresetIndex >= presetsAndConfig.Presets.size()){
        presetsAndConfig.CurrentPresetIndex = presetsAndConfig.Presets.size() - 1;
        configChanged = true;
    }
    if(presetsAndConfig.CurrentPresetIndex < 0){
        presetsAndConfig.CurrentPresetIndex = 0;
        configChanged = true;
    }
    if(configChanged)
        Save();
}

void ConfigManager::AddNewPreset(QString presetName){
    presetsAndConfig.Presets.push_back(Preset(presetName));
    presetsAndConfig.CurrentPresetIndex = presetsAndConfig.Presets.size() - 1;
}

int ConfigManager::RemovePresetAt(int index){
    if(presetsAndConfig.Presets.size() < 1)
        return -1;
    presetsAndConfig.CurrentPresetIndex = index;
    presetsAndConfig.Presets.removeAt(presetsAndConfig.CurrentPresetIndex);
    presetsAndConfig.CurrentPresetIndex = (presetsAndConfig.CurrentPresetIndex >= presetsAndConfig.Presets.size()) ? (presetsAndConfig.Presets.size() - 1) : presetsAndConfig.CurrentPresetIndex;
    return presetsAndConfig.CurrentPresetIndex;
}

bool ConfigManager::isFileNameValid(QString name){
#ifdef Q_OS_WIN
    return !(name.contains(R"(/)") || name.contains(R"(\)") || name.contains(R"(:)") || name.contains(R"(?)") || name.contains(R"(")") || name.contains(R"(<)") || name.contains(R"(>)") || name.contains(R"(|)") || name.contains(R"(*)"));
#else
    return !name.contains(R"(/)");
#endif
}
