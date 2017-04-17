#ifndef LINES_H
#define LINES_H
#include <QVector>
#include <QPolygon>
#include <QPoint>
#include <QDebug>
class Lines
{
public:
    Lines();
    QVector<QPolygon>& getLines();
    QPolygon& addPoint(QPoint newPoint);
    QPolygon& newLine();
private:
    QVector<QPolygon> lines;
};

#endif // LINES_H
