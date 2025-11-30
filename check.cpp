#include "check.h"

Check::Check() {}

bool Check::isSquare(const QList<QPointF>& coords) {
    return isQuadrilateral(coords, true);
}

bool Check::isRectangle(const QList<QPointF>& coords) {
    return isQuadrilateral(coords, false);
}

bool Check::isTriangle(const QList<QPointF>& coords)
{
    if (coords.size() != 3)
        return false;

    const QPointF& p1 = coords.at(0);
    const QPointF& p2 = coords.at(1);
    const QPointF& p3 = coords.at(2);

    double area = 0.5 * std::abs(p1.x() * (p2.y() - p3.y()) + p2.x() * (p3.y() - p1.y()) + p3.x() * (p1.y() - p2.y()));

    return area > 0.0;
}

bool Check::isQuadrilateral(const QList<QPointF>& coords, bool requireAllSidesEqual)
{
    if (coords.size() != 2 && coords.size() != 4) return false;

    if (coords.size() == 2) {
        const QPointF& p1 = coords.at(0);
        const QPointF& p2 = coords.at(1);
        if (p1 == p2 || (!requireAllSidesEqual && (p1.x() == p2.x() || p1.y() == p2.y()))) {
            return false;
        }
        return true;
    }

    auto dist2 = [](QPointF a, QPointF b){
        double dx = a.x() - b.x();
        double dy = a.y() - b.y();
        return dx*dx + dy*dy;
    };

    const QPointF& p1 = coords.at(0);
    const QPointF& p2 = coords.at(1);
    const QPointF& p3 = coords.at(2);
    const QPointF& p4 = coords.at(3);

    double d12 = dist2(p1, p2);
    double d23 = dist2(p2, p3);
    double d34 = dist2(p3, p4);
    double d41 = dist2(p4, p1);

    if (requireAllSidesEqual) {
        // square: all sides equal
        if (fabs(d12 - d23) > 1e-6 || fabs(d23 - d34) > 1e-6 || fabs(d34 - d41) > 1e-6)
            return false;
    } else {
        // rectangle: opposite sides equal
        if (fabs(d12 - d34) > 1e-6 || fabs(d23 - d41) > 1e-6)
            return false;
    }

    // diagonals equal
    double d13 = dist2(p1, p3);
    double d24 = dist2(p2, p4);
    if (fabs(d13 - d24) > 1e-6) return false;

    if (d12 < 1e-6) return false;

    return true;
}

