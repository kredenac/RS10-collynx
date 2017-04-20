#include "transform.h"



void Transform::shift(const int x, const int y, Drawable &object)
{
    for(QPoint &i : object.poly){
        i.setX(i.x()+x);
        i.setY(i.y()+y);
    }
    //qDebug() << object.poly.length();
    return;
}
