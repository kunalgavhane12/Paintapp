#include "PaintArea.h"
#include <QMouseEvent>
#include <QPainter>
#include <QPolygon>

PaintArea::PaintArea(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    modified = false;
    scribbling = false;
    myPenWidth = 1;
    myPenColor = Qt::black;
}

bool PaintArea::openImage(const QString &fileName)
{
    QImage loadedImage;
    if(!loadedImage.load(fileName))
    {
        return false;
    }
    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    image = loadedImage;
    modified = false;
    update();
    return true;
}

bool PaintArea::saveImage(const QString &fileName, const char *fileFormat)
{
    QImage visiableImage = image;
    resizeImage(&visiableImage,size());
    if(visiableImage.save(fileName,fileFormat))
    {
        modified = false;
        return true;
    }
    else
    {
        return true;
    }
}

void PaintArea::setPenColor(const QColor &newColor)
{
    myPenColor = newColor;
}

void PaintArea::setPenWidth(int newWidth)
{
    myPenWidth = newWidth;
}

void PaintArea::clearImage()
{
    image.fill(qRgb(255,255,255));
    modified = true;
    update();
}

//void PaintArea::drawLines(const QPoint &endPoint)
//{
//    n = 1;
//    QPainter painter(&image);
//    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
//    painter.drawLine(lastPoint,endPoint);
//    modified = true;
//    update();
//}

//void PaintArea::drawRects(const QPoint &endPoint)
//{
//    n = 2;
//    QPainter painter(&image);
//    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
//    painter.drawRect(lastPoint,endPoint);
//    modified = true;
//    update();
//}

//void PaintArea::drawEllipses(const QPoint &endPoint)
//{
//    n = 3;
//    QPainter painter(&image);
//    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
//    painter.drawEllipse(lastPoint,endPoint);
//    modified = true;
//    update();
//}

//void PaintArea::drawCircles(const QPoint &endPoint)
//{
//    n = 4;
//    QPainter painter(&image);
//    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
//    painter.drawEllipse(lastPoint,endPoint);
//    modified = true;
//    update();
//}

//void PaintArea::drawPolygons(const QPoint &endPoint)
//{
//    n = 5;
//    QPainter painter(&image);
//    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
//    painter.drawPolygon(lastPoint,endPoint);
//    modified = true;
//    update();
//}

void PaintArea::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        startPosition = event->pos();
        endPosition = startPosition;

        drawing = true;
//        update();
//        lastPoint = event->pos();
//        scribbling = true;
    }
}

void PaintArea::mouseMoveEvent(QMouseEvent *event)
{
    if (drawing)
    {
        endPosition = event->pos();
//        drawShape(endPosition, event->pos(), shapeToDraw);
//        update();
    }
}

void PaintArea::mouseReleaseEvent(QMouseEvent *event)
{
    if(drawing)
    {
        drawing = false;
        endPosition = event->pos();
        drawShape(startPosition, endPosition, shapeToDraw);
//        update();
    }

    //    if (event->button() == Qt::LeftButton && scribbling) {
    //        drawShape(lastPoint, event->pos(), shapeToDraw);
    //        scribbling = false;
    //    }
}


void PaintArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);

}

void PaintArea::resizeEvent(QResizeEvent *event)
{
    if(width() > image.width() || height() > image.height())
    {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth,newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void PaintArea::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor,myPenWidth,Qt::SolidLine, Qt::RoundCap,Qt::RoundJoin));
    painter.drawLine(lastPoint,endPoint);
    modified = true;
    int rad = (myPenWidth/2) + 2;
    update(QRect(lastPoint,endPoint).normalized().adjusted(-rad,-rad, +rad,+rad));
    lastPoint = endPoint;

}

void PaintArea::resizeImage(QImage *image, const QSize &newSize)
{
    if(image->size() == newSize)
    {
        return;
    }
    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255,255,255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0,0), *image);
    *image = newImage;
}

void PaintArea::drawShape(const QPoint &startPoint, const QPoint &endPoint, ShapeType shapeType)
{
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    switch (shapeType) {
    case Line:
        painter.drawLine(startPoint, endPoint);
        break;
    case Rectangle:
        painter.drawRect(QRect(startPoint, endPoint));
        break;
    case Ellipse:
        painter.drawEllipse(QRect(startPoint, endPoint));
        break;
    case Circle:
        painter.drawEllipse(QRect(startPoint, endPoint));
        break;
    case Polygon: {
        QPolygon polygon;
        polygon << startPoint << QPoint(endPoint.x(), startPoint.y()) << endPoint
                << QPoint(startPoint.x(), endPoint.y());
        painter.drawPolygon(polygon);
        break;
    }
    default:
        break;
    }

    modified = true;
    update();
}


void PaintArea::setShapeToDraw(ShapeType shape)
{
    shapeToDraw = shape;
}
