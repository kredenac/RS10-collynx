#include "lines.h"

Lines::Lines()
{
    newLine();
}


QVector<Drawable>& Lines::getLines()
{
    return lines;
}

QPolygon& Lines::addPoint(QPoint newPoint)
{
    lines.last().poly << newPoint;
    return lines.last().poly;
}

QPolygon& Lines::newLine(QPen newPen, int brushSize)
{
    Drawable newPoly;
    newPen.setCapStyle(Qt::RoundCap);
    newPen.setWidth(brushSize);
    newPoly.pen = newPen;
    newPen.setCosmetic(true);
    lines.append(newPoly);
    return lines.last().poly;
}

QPolygon& Lines::newLine()
{
    Drawable newPoly;
    QPen newPen(Qt::red);
    newPen.setCapStyle(Qt::RoundCap);
    newPen.setWidth(7);
    newPen.setCosmetic(true);
    newPoly.pen = newPen;
    lines.append(newPoly);
    return lines.last().poly;
}

QPolygon& Lines::newLine(QPen newPen)
{
    Drawable newPoly;
    newPoly.pen = newPen;

    lines.append(newPoly);
    return lines.last().poly;
}

void Lines::setPen(QPen newPen)
{
    lines.last().pen = newPen;
}

void Lines::undo()
{
    //the last poly is empty, so remove the 2nd last
    if (lines.length() > 1){
        //qDebug() << lines.length() << "before";
        QPen lastPen = lines.last().pen;
        lines.remove(lines.length() - 2);
        //newLine(lastPen);
        //qDebug() << lines.length() << "after";
    }
}
