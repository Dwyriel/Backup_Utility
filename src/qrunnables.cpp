#include "qrunnables.h"

SaveConfigToFileTask::SaveConfigToFileTask(int indexArg) : _indexArg(indexArg) {
    setAutoDelete(true);
}

void SaveConfigToFileTask::run() {
    ConfigManager::Save(_indexArg);
}
