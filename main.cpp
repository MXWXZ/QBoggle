#include "BoggleWindow.h"
#include "lexicon.h"
#include <QApplication>
#include <iostream>

int main(int argc, char* argv[]) {
    qsrand(static_cast<uint>(time(nullptr)));
    QApplication a(argc, argv);
    BoggleWindow w;
    w.show();

    return a.exec();
}
