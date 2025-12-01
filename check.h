#ifndef CHECK_H
#define CHECK_H

#include <QPointF>
#include <QVector>
#include <QList>
#include <QPair>

#include "command.h"

class Check
{
public:
    Check();
    QPair<bool, QString> check(const Type& type, const QList<QPointF> &coords);
    bool isSquare(const QList<QPointF> &coords);
    bool isRectangle(const QList<QPointF> &coords);
    bool isTriangle(const QList<QPointF>& coords);

private:
    bool isQuadrilateral(const QList<QPointF>& coords, bool requireAllSidesEqual);

};

#endif // CHECK_H
