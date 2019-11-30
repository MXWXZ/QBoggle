#include "Cube.h"

#include <QHBoxLayout>
#include <qevent.h>

Cube::Cube(QWidget* parent) : QWidget(parent) {
    label = new QLabel();
    label->setText("");
    label->setAlignment(Qt::AlignCenter);
    QFont font = label->font();
    font.setPointSize(20);
    label->setFont(font);

    this->setFixedSize(75, 75);
    this->setStyleSheet(
        "background-color: white; border-radius: 15px; border: 2px solid");

    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(label);
    setLayout(layout);
}

void Cube::setLetter(QString l) { label->setText(l); }

void Cube::highlight(int degree) {
    if (degree != 0)
        this->setStyleSheet(QString("background-color: rgba(255, 0, 0, %1); "
                                    "border-radius: 15px; border: 2px solid")
                                .arg(degree));
    else
        this->setStyleSheet(
            "background-color: white; border-radius: 15px; border: 2px solid");
}

void Cube::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        selected(posi, posj, label->text(), this);
    } else {
        QWidget::mousePressEvent(event);
    }
}

void Cube::setPos(int i, int j) {
    posi = i;
    posj = j;
}
