#include "PaintArea.h"
#include <QMouseEvent>
#include <QPainter>

PaintArea::PaintArea(QWidget *parent) : QWidget(parent)
{
//    setAttribute(Qt::WA_StaticContents);
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

void PaintArea::drawRect(const QRect &rect)
{
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawRect(rect);
    modified = true;
    update(rect.normalized());
}

void PaintArea::drawEllipse(const QRect &rect)
{
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawEllipse(rect);
    modified = true;
    update(rect.normalized());
}

void PaintArea::drawCircle(const QRect &rect)
{
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawEllipse(rect.normalized());
    modified = true;
    update(rect.normalized());
}
void PaintArea::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        lastPoint = event->pos();
        scribbling = true;
    }
}

void PaintArea::mouseMoveEvent(QMouseEvent *event)
{
    if((event->buttons() & Qt::LeftButton) && scribbling)
        drawLineTo(event->pos());

}

void PaintArea::mouseReleaseEvent(QMouseEvent *event)
{

    if(event->button() == Qt::LeftButton && scribbling)
    {
        drawLineTo(event->pos());
        scribbling = false;
    }
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
