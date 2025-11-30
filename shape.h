#ifndef SHAPE_H
#define SHAPE_H

#include <QGraphicsItem>
#include <QPointF>
#include <QPen>
#include <QBrush>

#include "command.h"

class Shape {
public:
    virtual ~Shape() = default;

    virtual QGraphicsItem* createGraphicsItem() = 0;
};

class LineShape : public Shape {
public:
    QPointF p1, p2;

    LineShape(QPointF a, QPointF b) : p1(a), p2(b) {}

    QGraphicsItem* createGraphicsItem() override {
        return new QGraphicsLineItem(QLineF(p1, p2));
    }
};

class TriangleShape : public Shape {
public:
    QPointF a, b, c;

    TriangleShape(QPointF p1, QPointF p2, QPointF p3)
        : a(p1), b(p2), c(p3) {}

    QGraphicsItem* createGraphicsItem() override {
        QPolygonF poly;
        poly << a << b << c;

        auto* item = new QGraphicsPolygonItem(poly);
        item->setPen(QPen(Qt::blue, 1));
        item->setBrush(QBrush(Qt::blue));
        return item;
    }
};

class RectShape : public Shape {
public:
    QPointF p1, p2, p3, p4;
    bool four = false;

    RectShape(QPointF a, QPointF b) : p1(a), p2(b) {}
    RectShape(QPointF a, QPointF b, QPointF c, QPointF d) : p1(a), p2(b), p3(c), p4(d) {
        four = true;
    }

    QGraphicsItem* createGraphicsItem() override {
        QGraphicsItem* item = nullptr;
        if (!four) {
            QRectF rect(
                QPointF(std::min(p1.x(), p2.x()), std::min(p1.y(), p2.y())),
                QPointF(std::max(p1.x(), p2.x()), std::max(p1.y(), p2.y()))
                );
            auto* rectItem = new QGraphicsRectItem(rect);
            rectItem->setPen(QPen(Qt::blue, 1));
            rectItem->setBrush(QBrush(Qt::blue));
            item = rectItem;
        } else {
            QPolygonF poly;
            poly << p1 << p2 << p3 << p4;
            auto* polyItem = new QGraphicsPolygonItem(poly);
            polyItem->setPen(QPen(Qt::blue, 1));
            polyItem->setBrush(QBrush(Qt::blue));
            item = polyItem;
        }
        return item;
    }
};

class SquareShape : public Shape {
public:
    QPointF p1, p2, p3, p4;
    bool four = false;

    SquareShape(QPointF a, QPointF b) : p1(a), p2(b) {}
    SquareShape(QPointF a, QPointF b, QPointF c, QPointF d) : p1(a), p2(b), p3(c), p4(d) {
        four = true;
    }

    QGraphicsItem* createGraphicsItem() override {
        QGraphicsItem* item = nullptr;

        if (!four) {
            QPointF mid = (p1 + p2) / 2.0;
            QPointF half = p1 - mid;
            QPointF rot(-half.y(), half.x());
            QPointF p3 = mid + rot;
            QPointF p4 = mid - rot;

            QPolygonF square;
            square << p1 << p3 << p2 << p4;

            auto* squareItem = new QGraphicsPolygonItem(square);
            squareItem->setPen(QPen(Qt::blue, 1));
            squareItem->setBrush(QBrush(Qt::blue));
            item = squareItem;
        } else {
            QPolygonF poly;
            poly << p1 << p2 << p3 << p4;

            auto* polyItem = new QGraphicsPolygonItem(poly);
            polyItem->setPen(QPen(Qt::blue, 1));
            polyItem->setBrush(QBrush(Qt::blue));
            item = polyItem;
        }

        return item;
    }

};

class ShapeFactory {
public:
    static std::unique_ptr<Shape> createShape(const Type &type,
        const QVector<QPointF> &points
        );
};

#endif // SHAPE_H
