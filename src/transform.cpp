#include "transform.h"



void Transform::shift(const int x, const int y, Drawable &object)
{
    Q_UNUSED(x);
    Q_UNUSED(y);
    Q_UNUSED(object);
    /*ne radi za novu strukturu lines-a
    for(QPoint &i : object.poly){
        i.setX(i.x()+x);
        i.setY(i.y()+y);
    }*/
    //qDebug() << object.poly.length();
    return;
}
