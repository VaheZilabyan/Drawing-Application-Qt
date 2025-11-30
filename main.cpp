#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

/* for test
 * Line:
 *      create_line -name {line1} -coord_1 {0,-70} -coord_2 {100,-70}
 *
 * Triangle:
 *      create_triangle -name {triangle1} -coord_1 {0,0} -coord_2 {100,100} -coord_3 {-100,100}
 *
 * Rectangle:
 *      create_rectangle -name {rectangle1} -coord_1 {150,50} -coord_2 {250,200}
 *      create_rectangle -name {rectangle2} -coord_1 {150,-50} -coord_2 {250,-50} -coord_3 {250,0} -coord_4 {150,0}
 *
 * Square:
 *      create_square -name {square1} -coord_1 {0,-50} -coord_2 {50,-50} -coord_3 {50,0} -coord_4 {0,0}
 *      create_square -name {square2} -coord_1 {-200,-200} -coord_2 {-100,-100}
 *
 * Connect:
 *      connect -object_name_1 {triangle1} -object_name_2 {rectangle1}
 * File:
 *      execute_file -file_path {../DrawingApplication/execute_file_test.txt}
*/
