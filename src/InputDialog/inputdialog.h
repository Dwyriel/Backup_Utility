#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>

namespace Ui {
class InputDialog;
}

class InputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InputDialog(QWidget *parent = nullptr);

    explicit InputDialog(QString title, QWidget *parent = nullptr);

    ~InputDialog();

    QString OutputString;

private:
    Ui::InputDialog *ui;

    QString *outputString;

    void connectSignals();

private slots:
    void btnOkPressed();
    void btnCancelPressed();
    void inputTextEdited(QString text);
};

#endif // INPUTDIALOG_H
