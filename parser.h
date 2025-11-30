#ifndef PARSER_H
#define PARSER_H

#include <optional>
#include <QPair>

#include "command.h"

class Parser
{
public:
    Parser();
    ParseResult parseCreateLine(const QString& input);
    ParseResult parseCreateRect(const QString& input);
    ParseResult parseCreateSquare(const QString& input);
    ParseResult parseCreateTriangle(const QString& input);
    std::optional<QPair<QString, QString>> parseConnection(const QString& input);
    FileParseResult parseCommandsFromFile(const QString& input);
    FileParseResult executeFromFile(const QString& path);
};

#endif // PARSER_H
