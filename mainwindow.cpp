#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "PaintArea.h"
#include <QFileDialog>
#include <QDir>
#include <QColorDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QImageWriter>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    paintArea = new PaintArea;
    setCentralWidget(paintArea);
    createActions();
    createMenus();

    setWindowTitle("Painter");
    resize(500,500);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(maybesave())
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::open()
{
    if(maybesave())
    {
        QString fileName = QFileDialog::getOpenFileName(this,"Open File", QDir::currentPath());
        if(!fileName.isEmpty())
        {
            paintArea->openImage(fileName);
        }
    }
}

void MainWindow::save()
{
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}

void MainWindow::penColor()
{
    QColor newColor = QColorDialog::getColor(paintArea->penColor());
    if(newColor.isValid())
    {
        paintArea->setPenColor(newColor);
    }

}

void MainWindow::penWidth()
{
    bool ok;
    int newWidth = QInputDialog::getInt(this, "Paint", "Select pen width : ",
                                        paintArea->penWidth(),1,50,1,&ok);

    if(ok)
    {
        paintArea->setPenWidth(newWidth);
    }
}

void MainWindow::drawRect()
{
    QPainter painter;
//    painter.setPen(QPen(myPenColor,myPenWidth,Qt::SolidLine, Qt::RoundCap,Qt::RoundJoin));
    painter.drawRect(10,10,100,100);

}

void MainWindow::createActions()
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
    foreach (QByteArray format, QImageWriter::supportedImageFormats()) {
        QString text = tr("%1...").arg(QString(format).toUpper());

        QAction *action = new QAction(text, this);
        action->setData(format);
        connect(action, SIGNAL(triggered()), this, SLOT(save()));
        saveAsActs.append(action);
    }

    exitAct = new QAction(tr("Exit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    penColorAct = new QAction(tr("Pen Color..."), this);
    connect(penColorAct, SIGNAL(triggered()), this, SLOT(penColor()));

    penWidthAct = new QAction(tr("Pen Width..."), this);
    connect(penWidthAct, SIGNAL(triggered()), this, SLOT(penWidth()));

    clearScreenAct = new QAction(tr("Clear Screen"), this);
    clearScreenAct->setShortcut(tr("Ctrl+L"));
    connect(clearScreenAct, SIGNAL(triggered()), paintArea, SLOT(clearImage()));

    drawRectAct = new QAction(tr("Draw Rect..."), this);
    connect(drawRectAct, SIGNAL(triggered()), this, SLOT(drawRect()));

    drawEllipseAct = new QAction(tr("Draw Elipse..."), this);
    connect(drawEllipseAct, SIGNAL(triggered()), this, SLOT(drawEllipse()));

}

void MainWindow::createMenus()
{
    saveAsMenu = new QMenu(tr("Save As"), this);
    foreach (QAction *action, saveAsActs)
        saveAsMenu->addAction(action);

    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addMenu(saveAsMenu);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    optionMenu = new QMenu(tr("&Options"), this);
    optionMenu->addAction(penColorAct);
    optionMenu->addAction(penWidthAct);
    optionMenu->addSeparator();
    optionMenu->addAction(clearScreenAct);

    drawMenu = new QMenu(tr("Draw Shape"),this);
    drawMenu->addAction(drawRectAct);
    drawMenu->addAction(drawEllipseAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionMenu);
    menuBar()->addMenu(drawMenu);
}

bool MainWindow::maybesave()
{
    if(paintArea->isModified())
    {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Paint"), tr("The image has been Modified.\n"
                                                            "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if(ret == QMessageBox::Save)
        {
            return saveFile("png");
        }
        else if(ret == QMessageBox::Cancel)
        {
            return false;
        }
    }
    return true;
}

bool MainWindow::saveFile(const QByteArray &fileFormat)
{
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;
    QString fileName = QFileDialog::getSaveFileName(this,tr("Save As"),initialPath,
                                                    tr("%1 Files (*.%2);;All Files(*)")
                                                    .arg(QString::fromLatin1(fileFormat.toUpper()))
                                                    .arg(QString::fromLatin1(fileFormat)));

    if(fileName.isEmpty())
    {
        return false;
    }
    else
    {
        return paintArea->saveImage(fileName, fileFormat.constData());
    }

}

