#include "customlineedit.h"

CustomLineEdit::CustomLineEdit(QWidget *parent) : QLineEdit(parent){}

void CustomLineEdit::focusOutEvent(QFocusEvent *e){
    if(e->lostFocus())
        emit lostFocus();
    QLineEdit::focusOutEvent(e);
}
