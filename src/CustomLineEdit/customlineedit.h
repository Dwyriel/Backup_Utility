#ifndef CUSTOMLINEEDIT_H
#define CUSTOMLINEEDIT_H

#include <QLineEdit>
#include <QFocusEvent>

class CustomLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit CustomLineEdit(QWidget *parent = nullptr);

signals:
    void lostFocus();

protected:
    void focusOutEvent(QFocusEvent *e) override;
};

#endif // CUSTOMLINEEDIT_H
