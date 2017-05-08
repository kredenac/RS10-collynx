#include "chooser.h"
#include "ui_chooser.h"
#include "painter.h"

chooser::chooser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chooser)
{
//    QPushButton* pushButton = findChild<QPushButton*>("pushButton");
//    ui->pushButton

    setStyleSheet("background : rgba(0,0,200,0.7);");
    hide();
    ui->setupUi(this);
}

chooser::~chooser()
{
    delete ui;
}

void chooser::cshow(QPoint pos, int w, int h)
{
    int y = pos.y();
    int x = pos.x();
    int xOffset = 0, yOffset = 0;

    if(y + height() > h)
        yOffset = height();

    if(x + width() > w)
        xOffset = width();

    move(pos.x() - xOffset, pos.y() - yOffset);
    show();
}

// FIXME !! ! !
QPushButton* chooser::getButton(int index){
    // TODO foreach ...
    switch (index) {
    case 1:
        return ui->pushButton_1;
        break;
    case 2:
        return ui->pushButton_2;
        break;
    case 3:
        return ui->pushButton_3;
        break;
    case 4:
        return ui->pushButton_4;
        break;
    case 5:
        return ui->pushButton_5;
        break;
    default:
        return nullptr;
        break;
    }

}



