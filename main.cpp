#include <QApplication>
#include <QPushButton>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QWidget Window = QWidget(nullptr);
    Window.setFixedSize(450, 200);
    Window.show();
    return QApplication::exec();
}
