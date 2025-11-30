#ifndef CANVASCONTROLLER_H
#define CANVASCONTROLLER_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QVector>

#include "parser.h"

class CanvasController : public QObject
{
    Q_OBJECT
public:
    CanvasController(QGraphicsScene *scene, QObject *parent = nullptr);

    void createShape(const QString& name, const Type &type, const QList<QPointF>& points);
    void connectObjects(const QString& objectName1, const QString& objectName2);
    bool hasObjectNamed(const QString& name);

private:
    QPointF getCenter(QGraphicsItem *item);
    void addName(QGraphicsItem* item, const QString &name);

private:
    QGraphicsScene *scene;
    QMap<QString, QGraphicsItem*> objects;
};

#endif // CANVASCONTROLLER_H
