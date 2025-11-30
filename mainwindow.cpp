#include "mainwindow.h"
#include "parser.h"
#include "check.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QFile>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QDir>
#include <QListWidgetItem>
#include <QFileDialog>
#include <QVector>
#include <QList>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(800, 650);
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    setCentralWidget(centralWidget);

    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);
    controller = new CanvasController(scene, this);

    view->setStyleSheet("QGraphicsView { border: 1px solid black; }");
    scene->addLine(0,1000,0,-1000);
    scene->addLine(1000,0,-1000,0);

    // Header
    QLabel* logHeader = new QLabel("Log");
    logHeader->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    logHeader->setStyleSheet(
        "background-color: black;"
        "color: white;"
        "padding: 6px;"
        "font-weight: bold;"
        "font-size: 14px;"
    );

    logWindow = new QPlainTextEdit;
    logWindow->setStyleSheet(
        "background-color: white;"
        "color: black;"
        "font-size: 13px;"
    );
    logWindow->setReadOnly(true);
    logWindow->setMaximumHeight(90);
    logWindow->setMinimumHeight(90);

    commandLine = new QLineEdit;
    commandLine->setPlaceholderText("Commands console");

    mainLayout->addWidget(view);
    mainLayout->addWidget(logHeader);
    mainLayout->addWidget(logWindow);
    mainLayout->addWidget(commandLine);

    QMenu* File = menuBar()->addMenu("File");
    QMenu* Help = menuBar()->addMenu("Help");
    QAction* openFileAction = new QAction("Open File", this);
    QAction* helpAction = new QAction("help...", this);
    File->addAction(openFileAction);
    Help->addAction(helpAction);

    connect(openFileAction, &QAction::triggered, this, &MainWindow::openFileClicked);
    connect(helpAction, &QAction::triggered, this, &MainWindow::helpWindowSlot);
    connect(commandLine, &QLineEdit::returnPressed, this, &MainWindow::processCommand);
}

MainWindow::~MainWindow() {}

void MainWindow::processCommand()
{
    QString input = commandLine->text();
    commandLine->clear();
    Parser parser;
    Check check;

    if (input.startsWith("create_line")) {

        auto result = parser.parseCreateLine(input);
        if (result.errorMessage.isEmpty()) {
            log(input);
            QString name = result.object.name;
            controller->createShape(name, Type::Line, result.object.coords);
        } else {
            log(result.errorMessage);
        }

    } else if (input.startsWith("create_triangle")) {

        auto result = parser.parseCreateTriangle(input);
        if (result.errorMessage.isEmpty()) {
            QString name = result.object.name;
            QList<QPointF> coords = result.object.coords;
            if (check.isTriangle(coords)) {
                controller->createShape(name, Type::Triangle, result.object.coords);
                log(input);
            } else {
                log("[ERROR] Triangle: Wrong coordinates.");
            }
        } else {
            log(result.errorMessage);
        }

    } else if (input.startsWith("create_rectangle")) {

        auto result = parser.parseCreateRect(input);
        if (result.errorMessage.isEmpty()) {
            QString name = result.object.name;
            if (check.isRectangle(result.object.coords)) {
                controller->createShape(name, Type::Rectangle, result.object.coords);
                log(input);
            } else {
                log("[ERROR] Rectangle: Wrong coordinates.");
            }
        } else {
            log(result.errorMessage);
        }

    }  else if (input.startsWith("create_square")) {

        auto result = parser.parseCreateSquare(input);
        if (result.errorMessage.isEmpty()) {
            QString name = result.object.name;
            if (check.isSquare(result.object.coords)) {
                controller->createShape(name, Type::Square, result.object.coords);
                log(input);
            } else {
                log("[ERROR] Square: Wrong coordinates.");
            }
        } else {
            log(result.errorMessage);
        }

    } else if (input.startsWith("connect")) {

        auto result = parser.parseConnection(input);
        if (result) {
            QString name1 = result->first;
            QString name2 = result->second;
            if (controller->hasObjectNamed(name1) && controller->hasObjectNamed(name2)) {
                controller->connectObjects(name1, name2);
                log(input);
            } else {
                log("[ERROR] Connection: Wrong names.");
            }
        } else {
            log("[ERROR] Connection: Syntax error");
        }

    } else if (input.startsWith("execute_file")) {

        auto result = parser.parseCommandsFromFile(input);
        if (result.errors.isEmpty()) {
            qDebug() << "[0 errors]...";
            for (auto line : result.lines) {
                log(line);
            }
        } else {
            int i = 0;
            for (auto error : result.errors) {
                qDebug() << i++ << ": " << error;
            }
            return;
        }
        //create and connect objects
        for (auto parseResult : result.lineResult) {
            QString name = parseResult.object.name;
            Type type = parseResult.object.type;
            auto coords = parseResult.object.coords;

            if (type == Type::Connection) {
                controller->connectObjects(parseResult.object.obj1, parseResult.object.obj2);
                continue;
            }

            controller->createShape(name, type, coords);
        }

    } else {
        log("[ERROR] Unknown command");
    }
}

void MainWindow::helpWindowSlot()
{
    if (!helpWindow)
        helpWindow = new HelpWindow();

    helpWindow->show();
    helpWindow->raise();
    helpWindow->activateWindow();
}

void MainWindow::openFileClicked()
{
    QString path = QFileDialog::getOpenFileName(this, "Choose File");

    if(path.isEmpty())
        return;

    QFile file(path);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        return;
    QTextStream in(&file);
    QString fileContent = in.readAll();
    file.close();

    Parser parser;
    auto result = parser.executeFromFile(path);
    if (result.errors.isEmpty()) {
        qDebug() << "[0 errors]...";
        for (auto line : result.lines) {
            log(line);
        }
    } else {
        for (auto error : result.errors) {
            qDebug() << error;
            log(error);
        }
        return;
    }
    //create and connect objects
    for (auto parseResult : result.lineResult) {
        QString name = parseResult.object.name;
        Type type = parseResult.object.type;
        auto coords = parseResult.object.coords;

        if (type == Type::Connection) {
            controller->connectObjects(parseResult.object.obj1, parseResult.object.obj2);
            continue;
        }

        controller->createShape(name, type, coords);
    }

}

void MainWindow::log(const QString& text)
{
    logWindow->appendPlainText(text);
}
