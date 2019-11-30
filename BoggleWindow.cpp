#include "BoggleWindow.h"

#include <QFile>
#include <QHBoxLayout>
#include <QTextEdit>
#include <iostream>

BoggleWindow::BoggleWindow(QWidget* parent) : QMainWindow(parent) {
    this->setWindowTitle("QBoggle!");
    this->setFixedSize(BOGGLE_WINDOW_WIDTH, BOGGLE_WINDOW_HEIGHT);

    me = new WordListWidget(this, "Me");
    computer = new WordListWidget(this, "Computer");
    board = new Board(this);
    console = new Console(this);

    me->setGeometry(20, 20, 230, 300);
    board->setGeometry(230, 0, 300, 300);
    computer->setGeometry(800 - 50 - 200, 20, 230, 300);
    console->setGeometry(30, 320, 740, 260);

    QFile qFile(":/res/EnglishWords.txt");
    if (!qFile.open(QIODevice::ReadOnly)) {
        throw new std::runtime_error("Resource file not found!");
    }
    lex = new Lexicon(qFile);

    /*
    for (std::string s: lex) {
        std::cout << s << std::endl;
    }
    */
    console->write(
        "Welcome to the game Boggle!\nInput your letter if you want:\n");
    QObject::connect(console, &Console::newLineWritten, this,
                     &BoggleWindow::command);
}

BoggleWindow::~BoggleWindow() {}

void BoggleWindow::command(QString cmd) {
    if (start == 0) {
        me->reset();
        computer->reset();
        board->allres.clear();
        board->shake();
        start = 1;
        clear();
        console->write(
            "Welcome to the game Boggle!\nInput your letter if you want:");
        return;
    }
    if (start == 1) {
        if (cmd.isEmpty()) {
            start = 2;
            console->write("[*_*] Game start!");
        } else {
            if (!board->input(cmd)) {
                console->write("[-_-] Letter too short! Try another:");
            } else {
                start = 2;
                console->write("[*_*] Game start!");
            }
        }
        return;
    }

    if (cmd.isEmpty()) {
        int score = 0;
        for (int i = 0; i < 5; ++i)
            for (int j = 0; j < 5; ++j)
                score += board->findAll(lex, "", i, j);
        board->highlight(true);
        computer->setUpdatesEnabled(false);
        for (auto i : board->allres)
            if (me->checkWord(i))
                score -= i.length() - 3;
            else
                computer->addWord(i);
        computer->setUpdatesEnabled(true);
        computer->addScore(score);
        start = 0;
        console->write("[*_*] Press any key to start next game!");
        return;
    }

    cmd = cmd.toLower();
    if (cmd.size() < 4) {
        console->write("[-_-] Word too short!");
        return;
    }
    if (me->checkWord(cmd)) {
        console->write("[-_-] Already exist!");
        return;
    }
    if (!lex->contains(cmd.toStdString())) {
        console->write("[-_-] Invalid word!");
        return;
    }

    QString tmp = cmd.toUpper();
    bool flag = false;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j)
            if (board->checkWord(tmp, i, j)) {
                board->highlight();
                me->addWord(cmd);
                me->addScore(cmd.length() - 3);
                console->write("[^_^] Correct!");
                flag = true;
                break;
            }
        if (flag)
            break;
    }

    if (!flag)
        console->write("[-_-] Word is not in board!");
}

void BoggleWindow::click(int posi, int posj, QString l, Cube* c) {
    if (start == 2) {
        if ((lasti == -1 && lastj == -1) ||
            (qAbs(posi - lasti) <= 1 && qAbs(posj - lastj) <= 1)) {
            if (!vis[posi][posj]) {
                if (lex->containsPrefix(
                        (laststr + l.toLower()).toStdString())) {
                    laststr.append(l.toLower());
                    if (lex->contains(laststr.toStdString()) &&
                        !me->checkWord(laststr) && laststr.size() >= 4) {
                        me->addWord(laststr);
                        me->addScore(laststr.length() - 3);
                        console->write(laststr);
                        console->write("[^_^] Correct!\n");
                        clear();
                        return;
                    }
                    if (lasti == -1 && lastj == -1)
                        board->highlight(true);
                    c->highlight(255 - qrand() % 255);
                    lasti = posi;
                    lastj = posj;
                    vis[posi][posj] = 1;
                } else {
                    clear();
                    console->write("[-_-] Invalid word!\n");
                }
            }
        }
    }
}

void BoggleWindow::clear() {
    lasti = lastj = -1;
    memset(vis, 0, sizeof(vis));
    laststr.clear();
    board->highlight(true);
}
