#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QColorDialog>
#include <QAction>
#include <QMenu>
#include <QByteArray>
#include "PaintArea.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:   

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

//private:
//    Ui::MainWindow *ui;

public slots:
    void drawShape(PaintArea::ShapeType shape);
    void scale();
    void rotate();
    void translate();

protected:
    void closeEvent(QCloseEvent *event)override;

private slots:
    void open();
    void save();
    void penColor();
    void penWidth();

    void drawRectangle();
    void drawLine();
    void drawEllipse();
    void drawCircle();
    void drawPolygon();

private:
    void createActions();
    void createMenus();
    bool maybesave();
    bool saveFile(const QByteArray &fileFormat);

    PaintArea *paintArea;

    QMenu *saveAsMenu;
    QMenu *fileMenu;
    QMenu *optionMenu;
    QMenu *drawMenu;
    QMenu *transformationMenu;

    QAction *openAct;

    QList<QAction*> saveAsActs;
    QAction *penColorAct;
    QAction *penWidthAct;
    QAction *clearScreenAct;
    QAction *exitAct;
    QAction *drawRectAct;
    QAction *drawLineAct;
    QAction *drawEllipseAct;
    QAction *drawCircleAct;
    QAction *polygonAct;
    QAction *scalingAct;
    QAction *rotationAct;
    QAction *translationAct;

    int cnt;

};
#endif // MAINWINDOW_H
