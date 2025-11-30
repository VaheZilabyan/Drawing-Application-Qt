#include "parser.h"
#include "command.h"

#include <QRegularExpression>
#include <QStringList>
#include <QFile>

Parser::Parser() {}

ParseResult Parser::parseCreateLine(const QString &input)
{
    ParseResult result;
    QRegularExpression re(
        R"(create_line\s+-name\s*\{(\w+)\}\s+-coord_1\s*\{\s*([-+]?\d*\.?\d+)\s*,\s*([-+]?\d*\.?\d+)\s*\}\s+-coord_2\s*\{\s*([-+]?\d*\.?\d+)\s*,\s*([-+]?\d*\.?\d+)\s*\})"
    );
    auto m = re.match(input);
    if (!m.hasMatch()) {
        result.errorMessage = "[ERROR] Line: Unknown command or wrong arguments.";
        return result;
    }

    double x1 = m.captured(2).toDouble();
    double y1 = m.captured(3).toDouble();
    double x2 = m.captured(4).toDouble();
    double y2 = m.captured(5).toDouble();

    result.object = Object{
        Type::Line,
        m.captured(1),
        { QPointF(x1, y1), QPointF(x2, y2) }
    };

    return result;
}

ParseResult Parser::parseCreateRect(const QString &input)
{
    ParseResult result;
    QRegularExpression re(
        R"(create_rectangle\s+-name\s*\{(\w+)\})"
        R"(\s+-coord_1\s*\{\s*([-+]?\d*\.?\d+)\s*,\s*([-+]?\d*\.?\d+)\s*\})"
        R"(\s+-coord_2\s*\{\s*([-+]?\d*\.?\d+)\s*,\s*([-+]?\d*\.?\d+)\s*\})"
        R"((?:\s+-coord_3\s*\{\s*([-+]?\d*\.?\d+)\s*,\s*([-+]?\d*\.?\d+)\s*\})?)"
        R"((?:\s+-coord_4\s*\{\s*([-+]?\d*\.?\d+)\s*,\s*([-+]?\d*\.?\d+)\s*\})?)"
    );

    auto m = re.match(input);
    if (!m.hasMatch()) {
        result.errorMessage = "[ERROR] Rectangle: Unknown command or wrong arguments.";
        return result;
    }

    double x1 = m.captured(2).toDouble();
    double y1 = m.captured(3).toDouble();
    double x2 = m.captured(4).toDouble();
    double y2 = m.captured(5).toDouble();

    QPointF p3, p4;
    bool has4Points = m.captured(6).length() > 0;

    if (has4Points) {
        double x3 = m.captured(6).toDouble();
        double y3 = m.captured(7).toDouble();
        double x4 = m.captured(8).toDouble();
        double y4 = m.captured(9).toDouble();
        result.object = Object{
            Type::Rectangle,
            m.captured(1),
            { QPointF(x1, y1), QPointF(x2, y2), QPointF(x3, y3), QPointF(x4, y4) },
        };
        return result;
    }

    result.object = Object{
        Type::Rectangle,
        m.captured(1),
        { QPointF(x1, y1), QPointF(x2, y2) }
    };

    return result;
}

ParseResult Parser::parseCreateSquare(const QString &input)
{
    ParseResult result;
    QRegularExpression re(
        R"(create_square\s+-name\s*\{(\w+)\})"
        R"(\s+-coord_1\s*\{\s*([-+]?\d*\.?\d+)\s*,\s*([-+]?\d*\.?\d+)\s*\})"
        R"(\s+-coord_2\s*\{\s*([-+]?\d*\.?\d+)\s*,\s*([-+]?\d*\.?\d+)\s*\})"
        R"((?:\s+-coord_3\s*\{\s*([-+]?\d*\.?\d+)\s*,\s*([-+]?\d*\.?\d+)\s*\})?)"
        R"((?:\s+-coord_4\s*\{\s*([-+]?\d*\.?\d+)\s*,\s*([-+]?\d*\.?\d+)\s*\})?)"
    );

    auto m = re.match(input);
    if (!m.hasMatch()) {
        result.errorMessage = "[ERROR] Square: Unknown command or wrong arguments.";
        return result;
    }

    double x1 = m.captured(2).toDouble();
    double y1 = m.captured(3).toDouble();
    double x2 = m.captured(4).toDouble();
    double y2 = m.captured(5).toDouble();

    QPointF p3, p4;
    bool has4Points = m.captured(6).length() > 0;

    if (has4Points) {
        double x3 = m.captured(6).toDouble();
        double y3 = m.captured(7).toDouble();
        double x4 = m.captured(8).toDouble();
        double y4 = m.captured(9).toDouble();
        result.object = Object{
                               Type::Square,
                               m.captured(1),
                               { QPointF(x1, y1), QPointF(x2, y2), QPointF(x3, y3), QPointF(x4, y4) },
                               };
        return result;
    }

    result.object = Object{
        Type::Square,
        m.captured(1),
        { QPointF(x1, y1), QPointF(x2, y2) }
    };

    return result;
}

ParseResult Parser::parseCreateTriangle(const QString &input)
{
    ParseResult result;
    QRegularExpression re(
        R"(create_triangle\s+-name\s*\{(\w+)\}\s+-coord_1\s*\{\s*([-+]?\d*\.?\d+)\s*,\s*([-+]?\d*\.?\d+)\s*\}\s+-coord_2\s*\{\s*([-+]?\d*\.?\d+)\s*,\s*([-+]?\d*\.?\d+)\s*\}\s+-coord_3\s*\{\s*([-+]?\d*\.?\d+)\s*,\s*([-+]?\d*\.?\d+)\s*\})"
    );
    auto m = re.match(input);
    if (!m.hasMatch()) {
        result.errorMessage = "[ERROR] Triangle: Unknown command or wrong arguments.";
        return result;
    }

    double x1 = m.captured(2).toDouble();
    double y1 = m.captured(3).toDouble();
    double x2 = m.captured(4).toDouble();
    double y2 = m.captured(5).toDouble();
    double x3 = m.captured(6).toDouble();
    double y3 = m.captured(7).toDouble();

    result.object = Object{
        Type::Triangle,
        m.captured(1),
        { QPointF(x1, y1), QPointF(x2, y2), QPointF(x3, y3) }
    };

    return result;
}

std::optional<QPair<QString, QString>> Parser::parseConnection(const QString &input)
{
    QRegularExpression re(
        R"(connect\s+-object_name_1\s*\{(\w+)\}\s+-object_name_2\s*\{(\w+)\})"
    );
    auto m = re.match(input);
    if (!m.hasMatch()) {
        qDebug() << "no match in parseConnection";
        return std::nullopt;
    }
    return qMakePair(m.captured(1), m.captured(2));
}

FileParseResult Parser::parseCommandsFromFile(const QString &input)
{
    QRegularExpression re(
        R"(execute_file\s(-file_path\s*\{([^}]+)\}))"
        );
    QString path;
    auto m = re.match(input);
    if (m.hasMatch()) {
        path = m.captured(2);
        qDebug() << "Path:" << path;
    }

    return executeFromFile(path);
}

FileParseResult Parser::executeFromFile(const QString &path)
{
    FileParseResult fileResult;
    QFile file(path);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        size_t lineNumber = 0;
        while (!in.atEnd()) {
            ++lineNumber;
            QString line = in.readLine();
            fileResult.lines.push_back(line);
            if (line.startsWith("create_line")) {
                auto currentLineResult = parseCreateLine(line);
                if (currentLineResult.errorMessage.isEmpty()) {
                    qDebug() << "[OK] Line created";
                    fileResult.lineResult.push_back(currentLineResult);
                } else {
                    qDebug() << "[ERROR] Line creation failed. Line " << lineNumber;
                    QString errorString = QString("[ERROR] Line creation failed (line: %1)").arg(lineNumber);
                    fileResult.errors.push_back(errorString);
                }
            } else if (line.startsWith("create_rectangle")) {
                auto currentLineResult = parseCreateRect(line);
                if (currentLineResult.errorMessage.isEmpty()) {
                    qDebug() << "[OK] Rectangle created";
                    fileResult.lineResult.push_back(currentLineResult);
                } else {
                    qDebug() << "[ERROR] Rectangle creation failed. Line " << lineNumber;
                    QString errorString = QString("[ERROR] Rectangle creation failed (line: %1)").arg(lineNumber);
                    fileResult.errors.push_back(errorString);
                }
            } else if (line.startsWith("create_square")) {
                auto currentLineResult = parseCreateSquare(line);
                if (currentLineResult.errorMessage.isEmpty()) {
                    qDebug() << "[OK] Square created";
                    fileResult.lineResult.push_back(currentLineResult);
                } else {
                    qDebug() << "[ERROR] Square creation failed. Line " << lineNumber;
                    QString errorString = QString("[ERROR] Square creation failed (line: %1)").arg(lineNumber);
                    fileResult.errors.push_back(errorString);
                }
            } else if (line.startsWith("create_triangle")) {
                auto currentLineResult = parseCreateTriangle(line);
                if (currentLineResult.errorMessage.isEmpty()) {
                    qDebug() << "[OK] Triangle created";
                    fileResult.lineResult.push_back(currentLineResult);
                } else {
                    qDebug() << "[ERROR] Triangle creation failed. Line " << lineNumber;
                    QString errorString = QString("[ERROR] Triangle creation failed (line: %1)").arg(lineNumber);
                    fileResult.errors.push_back(errorString);
                }
            } else if (line.startsWith("connect")) {
                auto currentLineResult = parseConnection(line);
                if (currentLineResult) {
                    qDebug() << "[OK] Objects connected";
                    ParseResult res;
                    res.object.type = Type::Connection;
                    res.object.obj1 = currentLineResult->first;
                    res.object.obj2 = currentLineResult->second;
                    fileResult.lineResult.push_back(res);
                } else {
                    qDebug() << "[ERROR] Connection failed. Line " << lineNumber;
                    QString errorString = QString("[ERROR] Connection failed (line: %1)").arg(lineNumber);
                    fileResult.errors.push_back(errorString);
                }
            } else {
                QString errorString = QString("(Syntax Error) Unknown command (line: %1)").arg(lineNumber);
                fileResult.errors.push_back(errorString);
            }
        }
    } else {
        fileResult.errors.push_back("Wrong file path");
    }
    return fileResult;
}
