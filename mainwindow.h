#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QColorDialog>
#include <QAction>
#include <QMenu>
#include <QByteArray>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class PaintArea;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

protected:
    void closeEvent(QCloseEvent *event)override;

private slots:
    void open();
    void save();
    void penColor();
    void penWidth();
    void drawRect();

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

    QAction *openAct;

    QList<QAction*> saveAsActs;
    QAction *penColorAct;
    QAction *penWidthAct;
    QAction *clearScreenAct;
    QAction *exitAct;
    QAction *drawRectAct;
    QAction *drawEllipseAct;

};
#endif // MAINWINDOW_H
