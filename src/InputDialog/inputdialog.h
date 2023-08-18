#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>

namespace Ui {
    class InputDialog;
}

class InputDialog : public QDialog {
Q_OBJECT

public:
    explicit InputDialog(QWidget *parent = nullptr);

    explicit InputDialog(const QString &body, QWidget *parent = nullptr);

    explicit InputDialog(const QString &title, const QString &body, QWidget *parent = nullptr);

    ~InputDialog() override;

    QString OutputString = QString();

private:
    Ui::InputDialog *ui;

    void connectSignals();

private slots:

    void btnOkClicked();

    void btnCancelClicked();

    void inputTextEdited(const QString &text);
};

#endif // INPUTDIALOG_H
