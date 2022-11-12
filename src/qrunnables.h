#ifndef QRUNNABLES_H
#define QRUNNABLES_H

#include <QRunnable>

#include <src/presetandconfig.h>

class SaveConfigToFileTask : public QRunnable
{
    int _indexArg;
public:

    SaveConfigToFileTask(int indexArg = -1);

    void run() override;
};

#endif // QRUNNABLES_H
