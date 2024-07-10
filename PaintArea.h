#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QObject>
#include <QWidget>

class PaintArea : public QWidget
{
    Q_OBJECT
public:
    enum ShapeType {
        None,
        Line,
        Rectangle,
        Ellipse,
        Circle,
        Polygon
    }shapeToDraw;

    explicit PaintArea(QWidget *parent = nullptr);

    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);

    bool isModified() const{return modified;}
    QColor penColor() const{return myPenColor;}
    int penWidth() const{return myPenWidth;}
    void setShapeToDraw(ShapeType shape);

public slots:
    void clearImage();
    void drawShape(const QPoint &startPoint, const QPoint &endPoint, ShapeType shapeType);

protected:
    void mousePressEvent(QMouseEvent *event)override;
    void mouseMoveEvent(QMouseEvent *event)override;
    void mouseReleaseEvent(QMouseEvent *event)override;

    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawLineTo(const QPoint &endPoint);
    void resizeImage(QImage *image, const QSize &newSize);

    bool modified;
    bool scribbling;
    int myPenWidth;
    QColor myPenColor;
    QImage image;
    bool drawing;
    QPoint startPosition;
    QPoint endPosition;
    QPoint lastPoint;

};

#endif // PAINTAREA_H
