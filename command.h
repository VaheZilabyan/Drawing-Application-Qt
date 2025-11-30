#ifndef COMMAND_H
#define COMMAND_H

#include <QString>
#include <QPointF>
#include <QVector>
#include <QList>

enum class Type { Line, Rectangle, Square, Triangle, Connection };

struct Object {
    Type type = Type::Line;
    QString name = "";
    QList<QPointF> coords{};
    QString obj1 = ""; //for connect
    QString obj2 = "";
};

struct ParseResult {
    Object object;
    QString errorMessage;
};

struct FileParseResult {
    QVector<ParseResult> lineResult;
    QVector<QString> lines;
    QVector<QString> errors;  // line number + description
};


#endif // COMMAND_H
