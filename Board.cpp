#include "Board.h"
#include "Cube.h"

#include <QGridLayout>
#include "BoggleWindow.h"

const QString Board::STANDARD_CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS", "AOOTTW", "CIMOTU",
    "DEILRX", "DELRVY", "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"};

const QString Board::BIG_BOGGLE_CUBES[25] = {
    "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM", "AEEGMU", "AEGMNN",
    "AFIRSY", "BJKQXZ", "CCNSTW", "CEIILT", "CEILPT", "CEIPST", "DDLNOR",
    "DDHNOT", "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU", "FIPRSY",
    "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"};

Board::Board(QWidget* parent, int size, const QString* cubeLetters)
    : QWidget(parent) {
    this->size = size;
    this->cubes = new Cube*[size * size];
    this->letters = new QString[size * size];
    for (int i = 0; i < size * size; ++i)
        this->letters[i] = cubeLetters[i];

    QGridLayout* layout = new QGridLayout();
    layout->setMargin(20);
    layout->setSpacing(10);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            this->cubes[index(i, j)] = new Cube(this);
            this->cubes[index(i, j)]->setPos(i, j);
            QObject::connect(this->cubes[index(i, j)], &Cube::selected,
                             (BoggleWindow*)parent, &BoggleWindow::click);
            layout->addWidget(this->cubes[index(i, j)], i, j,
                              Qt::AlignmentFlag::AlignCenter);
        }
    }
    setLayout(layout);

    shake();
    // this->setStyleSheet("background-color:grey; border: 3px solid");
}

Board::~Board() {
    if (cubes)
        delete[] cubes;
    if (letters)
        delete[] letters;
}

void Board::shake() {
    // Shake Cubes
    for (int i = 0; i < this->letters->size(); ++i) {
        int r = qrand() % (this->letters->size() - i) + i;
        qSwap(this->letters[i], this->letters[r]);
    }
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j) {
            shown[i][j] = this->letters[index(i, j)].at(qrand() % 6);
            if (shown[i][j] == "Q")
                shown[i][j] = "QU";
            this->cubes[index(i, j)]->setLetter(shown[i][j]);
        }
}

bool Board::checkWord(QString word, int i, int j) {
    if (word.left(shown[i][j].length()) != shown[i][j])
        return false;
    if (word.length() == shown[i][j].length()) {
        path.push_back(qMakePair(i, j));
        return true;
    }

    vis[i][j] = 1;
    for (int p = i - 1; p <= i + 1; ++p)
        for (int q = j - 1; q <= j + 1; ++q)
            if (p >= 0 && p < 5 && q >= 0 && q < 5 && !vis[p][q])
                if (checkWord(word.mid(shown[i][j].length()), p, q)) {
                    path.push_back(qMakePair(i, j));
                    vis[i][j] = 0;
                    return true;
                }
    vis[i][j] = 0;
    return false;
}

int Board::findAll(Lexicon* lex, QString now, int i, int j) {
    QString nowstr = (now + shown[i][j]).toLower();
    if (!lex->containsPrefix(nowstr.toStdString()))
        return 0;

    int ret = 0;
    vis[i][j] = 1;
    if (nowstr.length() >= 4 && lex->contains(nowstr.toStdString()) &&
        !allres.contains(nowstr)) {
        ret += nowstr.length() - 3;
        allres.push_back(nowstr);
    }

    for (int p = i - 1; p <= i + 1; ++p)
        for (int q = j - 1; q <= j + 1; ++q)
            if (p >= 0 && p < 5 && q >= 0 && q < 5 && !vis[p][q])
                ret += findAll(lex, nowstr, p, q);

    vis[i][j] = 0;
    return ret;
}

void Board::highlight(bool all) {
    if (all) {
        for (int i = 0; i < 5; ++i)
            for (int j = 0; j < 5; ++j)
                cubes[index(i, j)]->highlight(0);
    } else {
        int tmp;
        for (int i = 0; i < 5; ++i)
            for (int j = 0; j < 5; ++j)
                if ((tmp = path.indexOf(qMakePair(i, j))) != -1)
                    cubes[index(i, j)]->highlight(
                        255 -
                        (int)(255 / path.length()) * (path.length() - tmp - 1));
                else
                    cubes[index(i, j)]->highlight(0);
        path.clear();
    }
}

bool Board::input(QString str) {
    if (str.length() < 25) {
        return false;
    }
    str = str.toUpper();
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j) {
            shown[i][j] = str[index(i, j)];
            if (shown[i][j] == "Q")
                shown[i][j] = "QU";
            this->cubes[index(i, j)]->setLetter(shown[i][j]);
        }
    return true;
}
