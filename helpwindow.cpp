#include "helpwindow.h"
#include <QLabel>
#include <QVBoxLayout>

HelpWindow::HelpWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Help");
    setMinimumSize(300, 200);

    auto *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel(this);
    label->setTextFormat(Qt::RichText);
    label->setText(R"(
    <b>Command Interface</b><br><br>

    <b>1. Line</b><br>
    create_line -name {line_name} -coord_1 {x1,y1} -coord_2 {x2,y2}<br>
    Creates a line from coord_1 to coord_2.<br><br>

    <b>2. Triangle</b><br>
    create_triangle -name {triangle_name} -coord_1 {x1,y1} -coord_2 {x2,y2} -coord_3 {x3,y3}<br>
    Creates a triangle with given vertices.<br><br>

    <b>3. Rectangle</b><br>
    <b>Diagonal:</b> create_rectangle -name {rect_name} -coord_1 {x1,y1} -coord_2 {x2,y2}<br>
    <b>4-Points:</b> create_rectangle -name {rect_name} -coord_1 {x1,y1} -coord_2 {x2,y2} -coord_3 {x3,y3} -coord_4 {x4,y4}<br><br>

    <b>4. Square</b><br>
    <b>Diagonal:</b> create_square -name {sqr_name} -coord_1 {x1,y1} -coord_2 {x2,y2}<br>
    <b>4-Points:</b> create_square -name {sqr_name} -coord_1 {x1,y1} -coord_2 {x2,y2} -coord_3 {x3,y3} -coord_4 {x4,y4}<br><br>

    <b>5. Connecting Objects</b><br>
    connect -object_name_1 {name1} -object_name_2 {name2}<br>
    Connects centers of two shapes.<br><br>

    <b>6. Batch Execution</b><br>
    execute_file -file_path {path/to/script.txt}<br>
    Executes multiple commands from a file.
    )");
    label->setWordWrap(true);

    layout->addWidget(label);
}

