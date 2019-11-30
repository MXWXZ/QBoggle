#ifndef BOGGLEWINDOW_H
#define BOGGLEWINDOW_H

#include "Board.h"
#include "Console.h"
#include "WordListWidget.h"
#include "lexicon.h"

#include <QMainWindow>

class BoggleWindow : public QMainWindow {
    Q_OBJECT

public:
    BoggleWindow(QWidget* parent = 0);
    ~BoggleWindow();

public slots:
    void command(QString cmd);
    void click(int posi, int posj, QString l, Cube* c);

private:
    void clear();
    int start = 1;
    WordListWidget* me;
    WordListWidget* computer;
    Board* board;
    Console* console;
    Lexicon* lex;
    int lasti = -1;
    int lastj = -1;
    bool vis[5][5] = {0};
    QString laststr;

    static const int BOGGLE_WINDOW_WIDTH = 800;
    static const int BOGGLE_WINDOW_HEIGHT = 600;
};

#endif  // BOGGLEWINDOW_H
