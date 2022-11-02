#ifndef UTILITY_H
#define UTILITY_H

#include <QMessageBox>

namespace Utility
{
void showError( QWidget *parent, QString title, QString body);
void showWarning( QWidget *parent, QString title, QString body);
int showWarningWithButtons( QWidget *parent, QString title, QString body);
};

#endif // UTILITY_H
