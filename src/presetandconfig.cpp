#include "presetandconfig.h"

/*Preset*/
Preset::Preset(){}

Preset::Preset(QString presetName, qint64 backupNumber, QList<QString> filesToSave, QList<QString> FoldersToSave) : PresetName(presetName), BackupNumber(backupNumber), FilesToSave(filesToSave), FoldersToSave(FoldersToSave){}

/*PresetsAndConfig*/
PresetsAndConfig::PresetsAndConfig(){}

PresetsAndConfig::PresetsAndConfig(bool multithreaded, QString backupFolderPath, int currentPresetIndex, QList<Preset> presets) : Multithreaded(multithreaded), BackupFolderPath(backupFolderPath), CurrentPresetIndex(currentPresetIndex), Presets(presets){}

/*PresetManager*/
PresetsAndConfig PresetManager::defaultPresetsAndConfig = PresetsAndConfig(true, "", 0, QList<Preset>());
QString PresetManager::PresetsAndConfigFileName = "Backup_Utility.data";
QDir PresetManager::currentDir = QDir(QCoreApplication::applicationDirPath());
QFileInfo PresetManager::PresetsAndConfigFile = QFileInfo(PresetManager::currentDir.absolutePath() + QDir::separator() + PresetManager::PresetsAndConfigFileName);
PresetsAndConfig PresetManager::presetsAndConfig;

void PresetManager::Initialize(){
    if(PresetsAndConfigFile.exists())
        Load();
    else
        presetsAndConfig = defaultPresetsAndConfig;
    CheckFilesIntegrity();
}

Preset PresetManager::CurrentPreset(){ return Preset(); }//TODO

void PresetManager::Load(){
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
    presetsAndConfig.backupAllPresets = jsonObj.take("backupAllPresets").toBool();
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

void PresetManager::Save(){
    QFile file(PresetsAndConfigFile.absoluteFilePath());
    if(!file.open(QIODeviceBase::ReadWrite | QIODeviceBase::Truncate | QIODeviceBase::Text))
        return;
    QJsonObject pacJsonObj;
    pacJsonObj.insert("Multithreaded", presetsAndConfig.Multithreaded);
    pacJsonObj.insert("backupAllPresets", presetsAndConfig.backupAllPresets);
    pacJsonObj.insert("BackupFolderPath", presetsAndConfig.BackupFolderPath);
    pacJsonObj.insert("CurrentPresetIndex", presetsAndConfig.CurrentPresetIndex);
    QJsonArray presetsJsonArr;
    for(int i = 0; i < presetsAndConfig.Presets.length(); i++){
        QJsonObject presetObj;
        presetObj.insert("BackupNumber", presetsAndConfig.Presets[i].BackupNumber);
        presetObj.insert("PresetName", presetsAndConfig.Presets[i].PresetName);
        QJsonArray filesToSaveJsonArr;
        for(int j = 0; j < presetsAndConfig.Presets[i].FilesToSave.length(); j++)
            filesToSaveJsonArr.push_back(presetsAndConfig.Presets[i].FilesToSave[j]);
        presetObj.insert("FilesToSave", filesToSaveJsonArr);
        QJsonArray foldersToSaveJsonArr;
        for(int j = 0; j < presetsAndConfig.Presets[i].FoldersToSave.length(); j++)
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

void PresetManager::CheckFilesIntegrity(){
    bool configChanged = false;
    if(presetsAndConfig.CurrentPresetIndex >= presetsAndConfig.Presets.size()){
        presetsAndConfig.CurrentPresetIndex = presetsAndConfig.Presets.size() - 1;
        configChanged = true;
    }
    if(presetsAndConfig.Presets.size() < 0){
        presetsAndConfig.CurrentPresetIndex = 0;
        configChanged = true;
    }
    if(configChanged)
        Save();
}

bool PresetManager::CheckIfFileNameIsValid(QString name){
#ifdef Q_OS_WIN
    return !(name.contains(R"(/)") || name.contains(R"(\)") || name.contains(R"(:)") || name.contains(R"(?)") || name.contains(R"(")") || name.contains(R"(<)") || name.contains(R"(>)") || name.contains(R"(|)") || name.contains(R"(*)"));
#else
    return !name.contains(R"(/)");
#endif
}
