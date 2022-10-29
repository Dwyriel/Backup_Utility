#include "presetandconfig.h"

QString PresetManager::PresetsAndConfigFileName = "Backup_Utility.ini";
QDir PresetManager::currentDir = QDir(QCoreApplication::applicationDirPath());
QString PresetManager::PresetsAndConfigFilePath = PresetManager::currentDir.absolutePath() + PresetManager::PresetsAndConfigFileName;

void PresetManager::Initialize(){

}
