#include "shape.h"

#include <QDebug>

std::unique_ptr<Shape> ShapeFactory::createShape(const Type& type, const QVector<QPointF>& points) {
    if (type == Type::Line && points.size() == 2) {
        qDebug() << "creating line";
        return std::make_unique<LineShape>(points[0], points[1]);
    }

    if (type == Type::Rectangle && points.size() == 2) {
        return std::make_unique<RectShape>(points[0], points[1]);
    } else if (type == Type::Rectangle && points.size() == 4) {
        return std::make_unique<RectShape>(points[0], points[1], points[2], points[3]);
    }

    if (type == Type::Square && points.size() == 2) {
        return std::make_unique<SquareShape>(points[0], points[1]);
    } else if (type == Type::Square && points.size() == 4) {
        return std::make_unique<SquareShape>(points[0], points[1], points[2], points[3]);
    }

    if (type == Type::Triangle && points.size() == 3) {
        return std::make_unique<TriangleShape>(points[0], points[1], points[2]);
    }

    return nullptr; // invalid shape
}
