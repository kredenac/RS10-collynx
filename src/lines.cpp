#include "lines.h"

Lines::Lines()
{
    newLine();
}


QVector<QPolygon>& Lines::getLines()
{
    return lines;
}

QPolygon& Lines::addPoint(QPoint newPoint)
{
    lines.last() << newPoint;
    return lines.last();
}

QPolygon& Lines::newLine()
{
    QPolygon newPoly;
    lines.append(newPoly);
    return lines.last();
}


