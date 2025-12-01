#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPlainTextEdit>
#include <QLineEdit>

#include "canvascontroller.h"
#include "helpwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void processCommand();
    void helpWindowSlot();
    void openFileClicked();

private:
    void log(const QString& text);
    void executeFile(FileParseResult result);

private:
    CanvasController *controller;

    QGraphicsScene *scene;
    QGraphicsView *view;
    QPlainTextEdit *logWindow;
    QLineEdit *commandLine;

    HelpWindow* helpWindow = nullptr;
};
#endif // MAINWINDOW_H
