#ifndef BOARD_H
#define BOARD_H

#include "Cube.h"

#include "lexicon.h"
#include <QWidget>

class Board : public QWidget {
    Q_OBJECT
public:
    explicit Board(QWidget* parent = nullptr, int size = 5,
                   const QString* cubeLetters = BIG_BOGGLE_CUBES);
    virtual ~Board();
    void shake();
    bool checkWord(QString word, int i, int j);
    int findAll(Lexicon* lex, QString now, int i, int j);
    void highlight(bool all = false);
    bool input(QString str);
    QList<QString> allres;

signals:

public slots:

private:
    int size;
    Cube** cubes;
    QString* letters;
    QString shown[5][5];
    bool vis[5][5] = {0};
    QList<QPair<int, int>> path;
    inline int index(int i, int j) { return i * size + j; }
    static const QString STANDARD_CUBES[16];
    static const QString BIG_BOGGLE_CUBES[25];
};

#endif  // BOARD_H
