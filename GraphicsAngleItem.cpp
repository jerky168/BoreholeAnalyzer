#include "GraphicsAngleItem.h"

GraphicsAngleItem::GraphicsAngleItem(const QPointF &point, QGraphicsItem *parent) :
    QGraphicsPolygonItem(parent),
    secondPointFinished(false)
{
    polygonPoints.append(point);
    polygonPoints.append(QPointF());
    polygonPoints.append(QPointF());

    setPolygon(QPolygonF(polygonPoints));
    setPen(QPen(GraphicsSettings::instance()->getPenColor(), GraphicsSettings::instance()->getPenWidth()));
    setAcceptHoverEvents(true);

    setFlag(QGraphicsItem::ItemIsSelectable);
}

void GraphicsAngleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    widget = Q_NULLPTR;
    option = Q_NULLPTR;
    painter->setRenderHint(QPainter::Antialiasing);

    QPen thisPen;

    if (isSelected())
    {
        thisPen.setColor(pen().color());
        thisPen.setWidth(pen().width());
        thisPen.setStyle(Qt::DotLine);
    }
    else
    {
        thisPen = pen();
    }
    painter->setPen(thisPen);

    QPointF p1 = polygon().value(0);
    QPointF p2 = (polygon().at(1) == QPointF()) ? p1 : polygon().value(1);
    QPointF p3 = (polygon().at(2) == QPointF()) ? p2 : polygon().value(2);

    QLineF lineA(p1, p2), lineB(p2, p3);
    QVector<QPointF> lineAPoly, lineBPoly;
    lineAPoly << p1 << p2;
    lineBPoly << p2 << p3;
    painter->drawPolyline(lineAPoly);
    painter->drawPolyline(lineBPoly);

    if (polygon().at(2) != QPointF())
    {
        //计算角度大小
        qreal lineALength = lineA.length();//第一条角线长
        qreal lineBLength = lineB.length();//第二条角线长
        qreal averageLength = (lineALength + lineBLength) / 2;
        qreal lineCLength = QLineF(p1, p3).length();//第一个点和第三个点距离
        qreal angle = qAcos((pow(lineALength, 2) + qPow(lineBLength, 2)
            - qPow(lineCLength, 2))/(2*lineALength*lineBLength));//角度

        //计算角边之间的弧线
        qreal cosA = (p2.x() - p1.x() )/ lineALength;
        qreal sinA = (p2.y() - p1.y() ) / lineALength;
        qreal cosB = (p2.x() - p3.x() ) / lineBLength;
        qreal sinB = (p2.y() - p3.y() ) / lineBLength;
        //角标距角顶点的距离
        double l = qMin(lineALength, lineBLength) / 3;
        QPointF startPoint(p2.x() - l * cosA , p2.y() - l * sinA);//弧线起始点
        QPointF endPoint(p2.x() - l * cosB, p2.y() - l * sinB);//弧线终点
        QLineF lineC(startPoint, endPoint);//弧线起始点与终点间连线
        QPointF midPoint = lineC.pointAt(0.5);//连线中点
        qreal x = midPoint.x() - p2.x();//中点和角顶点的x方向距离
        qreal y = midPoint.y() - p2.y();//中点和角顶点的y方向距离
        QPointF controlPoint(midPoint.x() + x, midPoint.y() + y);//二次贝塞尔曲线的控制点
        QPainterPath path;//弧线路径
        path.moveTo(startPoint);
        path.quadTo(controlPoint, endPoint);//生成弧线
        painter->drawPath(path);//画弧线

        //计算角标文字坐标起点
        QPointF textPoint(QLineF(midPoint, controlPoint).pointAt(1.0));
        thisPen.setColor(pen().color());
        painter->setPen(thisPen);
        QFont font;
        font.setPointSize(averageLength / 5 > 50? 50 :(averageLength / 5));
        painter->setFont(font);
        painter->drawText(textPoint, QString().number(angle * 180 / 3.1415,'g', 4));//写标识
    }
}

// 返回path
QPainterPath GraphicsAngleItem::shape() const
{
    QPainterPath path;
    path.moveTo(polygon().value(0));
    for (int i = 1; i < polygon().count(); i++)
    {
        path.lineTo(polygon().value(i));
    }
    return GraphicsLineItem::shapeFromPath(path, pen());
}

// 返回外部边界
QRectF GraphicsAngleItem::boundingRect() const
{
    QRectF thisShape = shape().boundingRect();
    QRectF boungdRect(thisShape.topLeft() - QPoint(50, 50), QSize(thisShape.width() + 200, thisShape.height() + 200));
    return boungdRect;
}

bool GraphicsAngleItem::sceneEvent(QEvent *event)
{
    QGraphicsSceneMouseEvent *e = dynamic_cast<QGraphicsSceneMouseEvent*>(event);
    if (GraphicsScene::getCurMode() == GraphicsScene::InsertShift)
    {
        switch (event->type())
        {
            case QEvent::GraphicsSceneMousePress:
            {
                grabMouse();
                GraphicsSettings::instance()->setIsDrawing(true);
                break;
            }
            case QEvent::GraphicsSceneMouseMove:
            {
                if (!secondPointFinished)
                {
                    polygonPoints.replace(1, e->scenePos());
                    setPolygon(QPolygonF(polygonPoints));
                }
                else
                {
                    polygonPoints.replace(2, e->scenePos());
                    setPolygon(QPolygonF(polygonPoints));
                    GraphicsSettings::instance()->setIsDrawing(false);
                }
                break;
            }
            case QEvent::GraphicsSceneMouseRelease:
            {
                if (polygonPoints.at(1) != QPointF() && polygonPoints.at(2) == QPointF())
                {
                    secondPointFinished = true;
                }
                else if (polygonPoints.at(2) != QPointF())
                {
                    GraphicsScene *scene = dynamic_cast<GraphicsScene *>(this->scene());
                    scene->itemFinished(QString());

                    loadFromString(getDataString());
                }
                break;
            }
        default:
            return false;
        }
    }
    else
    {
        QGraphicsItem::sceneEvent(event);
    }
    return true;
}


GraphicsAngleItem::Data GraphicsAngleItem::getData()
{
    Data data;
    data.points[0] = polygonPoints[0];
    data.points[1] = polygonPoints[1];
    data.points[2] = polygonPoints[2];

    return data;
}

QString GraphicsAngleItem::getDataString()
{
    QString data = QString::number(this->polygon().count());
    for (int i = 0; i < polygon().count(); i++)
    {
        data.append(";");
        QPointF point = polygon().at(i);
        data.append(QString::number(point.x() - Border, 'f', 2));
        data.append(",");
        data.append(QString::number(point.y() - Border, 'f', 2));
    }

    qDebug() << polygon();
    return data;
}

GraphicsAngleItem * GraphicsAngleItem::loadFromString(QString data)
{
    int count = data.section(';', 0, 0).toInt();
    QPolygonF polygon;

    for (int i = 1; i <= count; i++)
    {
        QString section = data.section(';', i, i);
        qreal x = section.section(',', 0, 0).toDouble() + Border;
        qreal y = section.section(',', 1, 1).toDouble() + Border;
        QPointF point(x, y);
        polygon << point;
    }

    GraphicsAngleItem *item = new GraphicsAngleItem (QPoint());
    item->setPolygon(polygon);

    qDebug() << item->polygon();
    return item;
}

