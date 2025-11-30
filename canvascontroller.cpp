#include "canvascontroller.h"
#include "shape.h"
#include "command.h"

CanvasController::CanvasController(QGraphicsScene *scene, QObject *parent)
    : QObject(parent), scene(scene)
{

}

void CanvasController::createShape(const QString& name, const Type& type, const QList<QPointF>& points)
{
    auto shape = ShapeFactory::createShape(type, points);
    if (!shape) {
        qDebug() << "Invalid shape type";
        return;
    }

    QGraphicsItem* item = shape->createGraphicsItem();
    scene->addItem(item);
    objects[name] = item;

    auto *line = qgraphicsitem_cast<QGraphicsLineItem*>(item);
    if (!line) addName(item, name);
}

void CanvasController::connectObjects(const QString &objectName1, const QString &objectName2)
{
    auto *item1 = objects[objectName1];
    auto *item2 = objects[objectName2];


    QPointF from = getCenter(item1);
    QPointF to   = getCenter(item2);

    scene->addLine(QLineF(from, to), QPen(Qt::black, 2));
}

bool CanvasController::hasObjectNamed(const QString &name)
{
    if (objects.find(name) != objects.end()) return true;
    return false;
}

QPointF CanvasController::getCenter(QGraphicsItem *item)
{
    if (!item) return {};

    if (auto *rect = qgraphicsitem_cast<QGraphicsRectItem*>(item)) {
        return rect->mapToScene(rect->rect().center());
    }

    if (auto *line = qgraphicsitem_cast<QGraphicsLineItem*>(item)) {
        QPointF p1 = line->line().p1();
        QPointF p2 = line->line().p2();
        return line->mapToScene((p1 + p2) / 2);
    }

    if (auto *poly = qgraphicsitem_cast<QGraphicsPolygonItem*>(item)) {
        return poly->mapToScene(poly->polygon().boundingRect().center());
    }

    return item->mapToScene(item->boundingRect().center());
}

void CanvasController::addName(QGraphicsItem *item, const QString& name)
{
    auto *label = scene->addText(name);
    QFont font = label->font();
    font.setBold(true);
    label->setFont(font);
    label->setDefaultTextColor(Qt::green);
    QPointF pos = getCenter(item);
    QRectF r = label->boundingRect();
    label->setPos(pos.x() - r.width() / 2, pos.y() - r.height() / 2);
}
