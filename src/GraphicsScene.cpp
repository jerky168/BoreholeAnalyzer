#include "GraphicsScene.h"

GraphicsScene::Mode GraphicsScene::curMode = GraphicsScene::MoveItem;

GraphicsScene::GraphicsScene(QObject *parent) :
    QGraphicsScene(parent),
    item(Q_NULLPTR),
    pixmap_start(0.0),
    pixmap_end(1.0),
    pixmap_width(200.0),
    pixmap_height(1000.0),
    pixmap_diameter(0.1),
    showInfo(false)
{

}

GraphicsScene::~GraphicsScene()
{

}


void GraphicsScene::setCurMode(Mode mode)
{
    curMode = mode;
    emit modeChanged(mode);

    if (mode != MoveItem && item != Q_NULLPTR)
    {
        showInfo = true;
        item = Q_NULLPTR;
    }
}

GraphicsScene::Mode GraphicsScene::getCurMode()
{
    return curMode;
}



void GraphicsScene::clearScene()
{
    pixmap_start = 0.0;
    pixmap_end = 1.0;
    pixmap_width = 200.0;
    pixmap_height = 0.0;
    pixmap_diameter = 0.1;
    GraphicsSettings::instance()->setRatio(1.0, 1.0);
    setCurMode(MoveItem);
    clearItemData();
    update();
}



void GraphicsScene::updateIndexData(QPixmap pixmap, qreal start, qreal end, qreal diameter, QMap<QString, QGraphicsItem *> items)
{
    clearScene();
    showInfo = true;

    qreal perimeter = diameter * M_PI;

    pixmap_diameter = diameter;
    pixmap_start = start;
    pixmap_end = end;
    pixmap_width = pixmap.width();
    pixmap_height = pixmap.height();

    GraphicsSettings::instance()->setRatio((qreal)(pixmap.width()) / perimeter, (qreal)(pixmap.height()) / (end - start));

    qreal realHeight = (qreal)pixmap.height() / (end - start);

    setSceneRect(0, 0, pixmap.width() + 2 * Border, realHeight + 2 * Border);
    QGraphicsPixmapItem *pixmapItem = addPixmap(pixmap);
    pixmapItem->setPos(Border, Border);


    for (int i = 0; i < items.count(); i++)
    {
        addItem(items.values().at(i));
        addItemData(items.keys().at(i), items.values().at(i), true);
    }
    updateTable();
    emit update3DImage(getSceneImageFor3D(), pixmap_start, pixmap_end);
    update();
}



void GraphicsScene::itemFinished(QString content)
{
    showInfo = false;
    emit showRealInfo(getShowString(item));

    item->ungrabMouse();
    setCurMode(MoveItem);

    addItemData(QUuid::createUuid(), item, false);
    emit update3DImage(getSceneImageFor3D(), pixmap_start, pixmap_end);
    updateTable();
}

void GraphicsScene::itemAborted()
{
    item->ungrabMouse();
    setCurMode(MoveItem);
    delete item;
}


QImage GraphicsScene::getPixmapImage()
{
    qreal realHeight = (qreal)pixmap_height / (pixmap_end - pixmap_start);

    QImage image(pixmap_width + Border, realHeight + Border, QImage::Format_RGB32);
    QPainter painter(&image);
    painter.fillRect(image.rect(), Qt::white);
    render(&painter, QRect(Border, Border, pixmap_width, realHeight), QRect(Border, Border, pixmap_width, realHeight));

    QPen thisPen(Qt::black);
    thisPen.setWidth(8);

    painter.setPen(thisPen);
    QFont font = GraphicsSettings::instance()->getFont();
    font.setPointSize(80);
    painter.setFont(font);

    QVector<QLineF> lines;
    QLineF line;

    for (int i = 0; i < 11; i++)
    {
        qreal x = Border;
        qreal y = Border + (image.height() - Border) / 10 * i;
        line.setLine(x - Segment, y , x, y);
        lines << line;

        painter.drawText(QPointF(0, y - Segment), QString::number(pixmap_start + i * 0.1, 'f', 1));
    }

    for (int i = 0; i < 5; i++)
    {
        qreal x = Border + (image.width() - Border) / 4 * i;
        qreal y = Border;

        line.setLine(x, y-Segment, x, y);
        lines << line;

        switch(i)
        {
            case 0:
                painter.drawText(QPointF(x-2*Segment, y - 4*Segment), tr("N"));
                break;
            case 1:
                painter.drawText(QPointF(x-2*Segment, y - 4*Segment), tr("E"));
                break;
            case 2:
                painter.drawText(QPointF(x-2*Segment, y - 4*Segment), tr("S"));
                break;
            case 3:
                painter.drawText(QPointF(x-2*Segment, y - 4*Segment), tr("W"));
                break;
            case 4:
                painter.drawText(QPointF(x-2*Segment, y - 4*Segment), tr("N"));
                break;
            default:
                break;
        }
    }

    painter.drawLines(lines);

    return image;
}


QImage GraphicsScene::getSceneImage()
{
    QImage image(sceneRect().width(), sceneRect().height(), QImage::Format_RGB32);
    QPainter painter(&image);
    render(&painter);
    return image;
}

QImage GraphicsScene::getSceneImageFor3D()
{
    QImage image(sceneRect().width() - 2*Border, sceneRect().height() - 2*Border, QImage::Format_RGB32);
    QPainter painter(&image);
    render(&painter, image.rect(), QRect(Border, Border, image.width(), image.height()));

    QPen thisPen(Qt::yellow);
    thisPen.setWidth(8);

    painter.setPen(thisPen);
    QFont font = GraphicsSettings::instance()->getFont();
    font.setPointSize(40);
    painter.setFont(font);

    QVector<QLineF> lines;
    QLineF line;

    line.setLine(sceneRect().x() + Interval, 0, sceneRect().x() + Interval, pixmap_height);
    lines << line;

    for (int i = 0; i < 11; i++)
    {
        qreal x = sceneRect().x() + Interval;
        qreal y = image.height() / 10 * i;
        line.setLine(x, y , x + Segment, y);
        lines << line;

        painter.drawText(QPointF(x + Interval / 2, y + 2 * Segment), QString::number(pixmap_start + i * 0.1, 'f', 1));
    }

    painter.drawLines(lines);

    return image.copy(0, 0, pixmap_width, pixmap_height);
}


void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (curMode == MoveItem && item != Q_NULLPTR)
    {
        showInfo = true;
        item = Q_NULLPTR;
    }

    if (item != Q_NULLPTR)
    {

    }
    else if (curMode != MoveItem && (mouseEvent->buttons() & Qt::RightButton))
    {
        setCurMode(MoveItem);
    }
    else if (curMode != MoveItem && (mouseEvent->buttons() & Qt::LeftButton))
    {
        switch (curMode)
        {
            case InsertTextBox :
            {
                item = new GraphicsTextItem(mouseEvent->scenePos());
                addItem(item);
                item->grabMouse();
                break;
            }
            case InsertLine :
            {
                item = new GraphicsLineItem(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
                addItem(item);
                item->grabMouse();
                break;
            }
            case InsertRectangle :
            {
                item = new GraphicsRectItem(QRectF(mouseEvent->scenePos(), mouseEvent->scenePos()));
                addItem(item);
                item->grabMouse();
                break;
            }

            case InsertAnyShape :
            {
                item = new GraphicsAnyshape(mouseEvent->scenePos());
                addItem(item);
                item->grabMouse();
                break;
            }

            case InsertOccurance :
            {
                item = new GraphicsOccurance(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
                addItem(item);
                item->grabMouse();
                break;
            }

            default:
            {
                break;
            }
        }
    }

    QGraphicsScene::mousePressEvent(mouseEvent);
}


void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QPointF realPos = scene2Real(mouseEvent->scenePos());

    qreal width = realPos.x() / 360 * pixmap_width / GraphicsSettings::instance()->getXRatio();
    qreal depth = realPos.y();
    qreal degree = realPos.x();

    QString degree_str = getAngleString(degree);


    QString message;
    message +=  tr("Width: ") + QString::number(width, 'f', 3) + "m "
                + tr("Depth: ") + QString::number(depth, 'f', 3) + "m\n"
                + degree_str;

    if (showInfo)
        emit showRealInfo(message);
    emit showStatus(message);

    QGraphicsScene::mouseMoveEvent(mouseEvent);
}


void GraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->fillRect(rect, QBrush(Qt::lightGray));
    painter->setBrush(QBrush(Qt::white));


    if (items().isEmpty())
        return;

    QPointF topLeft = QPointF(sceneRect().x() + Border, sceneRect().y() + Border);
    QPointF bottomRight = QPointF(width() - Border, height() - Border);

    painter->fillRect(QRectF(topLeft, bottomRight), QBrush(Qt::white));


    QPen thisPen(Qt::black);
    thisPen.setWidth(8);

    painter->setPen(thisPen);
    QFont font = GraphicsSettings::instance()->getFont();
    font.setPointSize(50);
    painter->setFont(font);


    QVector<QLineF> lines;
    QLineF line;

    line.setLine(sceneRect().x() + Interval, Border, sceneRect().x() + Interval, height() - Border);
    lines << line;
    line.setLine(Border, Interval, width() - Border, Interval);
    lines << line;


    for (int i = 0; i < 11; i++)
    {
        qreal x = sceneRect().x() + Interval;
        qreal y = Border + (height() - 2 * Border) / 10 * i;
        line.setLine(x, y , x + Segment, y);
        lines << line;

        painter->drawText(QPointF(x + Interval / 2, y + 2 * Segment), QString::number(pixmap_start + i * 0.1, 'f', 1));
    }

    font.setPointSize(60);
    painter->setFont(font);

    for (int i = 0; i < 5; i++)
    {
        qreal x = Border + (width() - 2 * Border) / 4 * i;
        qreal y = sceneRect().y() + Interval;
        line.setLine(x, y , x, y + Segment);
        lines << line;

        switch(i)
        {
            case 0:
                painter->drawText(QPointF(x+Segment, y + 3*Segment), tr("N"));
                break;
            case 1:
                painter->drawText(QPointF(x+Segment, y + 3*Segment), tr("E"));
                break;
            case 2:
                painter->drawText(QPointF(x+Segment, y + 3*Segment), tr("S"));
                break;
            case 3:
                painter->drawText(QPointF(x+Segment, y + 3*Segment), tr("W"));
                break;
            case 4:
                painter->drawText(QPointF(x+Segment, y + 3*Segment), tr("N"));
                break;
            default:
                break;
        }
    }
    painter->drawLines(lines);

//    painter->drawText(QPointF(Border - Interval, Interval + 3*Segment), QString::number(0));
//    painter->drawText(QPointF(pixmap_width, Interval + 3*Segment), QString::number(pixmap_diameter * M_PI, 'f', 3));
}


QImage GraphicsScene::getImageFromData(QPixmap pixmap, qreal start, qreal end, qreal diameter, QMap<QString, QGraphicsItem *> items)
{
    GraphicsScene *scene = new GraphicsScene;
    scene->updateIndexData(pixmap, start, end, diameter, items);
    QImage image = scene->getSceneImage();
    delete scene;
    return image;
}

QImage GraphicsScene::getPixmapImageFromData(QPixmap pixmap, qreal start, qreal end, qreal diameter, QMap<QString, QGraphicsItem *> items)
{
    GraphicsScene *scene = new GraphicsScene;
    scene->updateIndexData(pixmap, start, end, diameter, items);
    QImage image = scene->getPixmapImage();
    delete scene;
    return image;
}


QVector<GraphicsScene::TableData> GraphicsScene::getSavedTableData()
{
    QVector<TableData> tableDatas;
    for (int i = 0; i < savedItems.count(); i++)
    {
        TableData tableData;
        QGraphicsItem *savedItem = savedItems.values().at(i);
        tableData.depth = getItemDepth(savedItem);
        tableData.data = getShowString(savedItem);
        tableData.isSaved = tr("Yes");
        switch(savedItem->type())
        {
            case Rect:
            {
                GraphicsRectItem *i = dynamic_cast<GraphicsRectItem *>(savedItem);
                tableData.remark = i->getRemark();
                tableData.type = tr("Rectangle");
                break;
            }

            case AnyShape:
            {
                GraphicsAnyshape *i = dynamic_cast<GraphicsAnyshape *>(savedItem);
                tableData.remark = i->getRemark();
                tableData.type = tr("AnyShape");
                break;
            }

            case Ruler:
            {
                GraphicsLineItem *i = dynamic_cast<GraphicsLineItem *>(savedItem);
                tableData.remark = i->getRemark();
                tableData.type = tr("Width");
                break;
            }

            case Occurance:
            {
                GraphicsOccurance *i = dynamic_cast<GraphicsOccurance *>(savedItem);
                tableData.remark = i->getRemark();
                tableData.type = tr("Occurance");
                break;
            }

            case Text:
            {
                GraphicsTextItem *i = dynamic_cast<GraphicsTextItem *>(savedItem);
                tableData.remark = i->getRemark();
                tableData.type = tr("Text");
                break;
            }

            default:
                break;
        }
        tableDatas.append(tableData);
    }
    return tableDatas;
}


QVector<GraphicsScene::TableData> GraphicsScene::getTableDataFromData(QPixmap pixmap, qreal start, qreal end, qreal diameter, QMap<QString, QGraphicsItem *> items)
{
    GraphicsScene *scene = new GraphicsScene;
    scene->updateIndexData(pixmap, start, end, diameter, items);
    QVector<TableData> tableDatas = scene->getSavedTableData();
    delete scene;
    return tableDatas;
}



QPointF GraphicsScene::scene2Real(QPointF scenePos)
{
    qreal x = scenePos.x() - Border;
    qreal y = scenePos.y() - Border;

    QPointF realPos;
    realPos.setX(x / pixmap_width * 360);
    realPos.setY(pixmap_start + y / pixmap_height * (pixmap_end - pixmap_start));


    return realPos;
}

QPointF GraphicsScene::real2Scene(QPointF realPos)
{
    qreal degree = realPos.x();
    qreal depth = realPos.y() - pixmap_start;

    QPointF scenePos;
    scenePos.setX(degree / 360 * pixmap_width + Border);
    scenePos.setY(depth / (pixmap_end - pixmap_start) * pixmap_height + Border);

    return scenePos;
}


QString GraphicsScene::getShowString(QGraphicsItem *item)
{
    QString str;
    switch(item->type())
    {
        case Rect:
        {
            GraphicsRectItem *i = dynamic_cast<GraphicsRectItem *>(item);
            QPointF pos1 = scene2Real(i->rect().topLeft());
            QPointF pos2 = scene2Real(i->rect().bottomRight());

            qreal area = i->rect().width() * i->rect().height() / GraphicsSettings::instance()->getXRatio() / GraphicsSettings::instance()->getYRatio() * 10000;
            str += QString::number(pos1.y(), 'f', 3) + "m-" + QString::number(pos2.y(), 'f', 3) + "m  ";
            str += tr("Area:  ") + QString::number(area, 'f', 2) + "cm2\n";
            str += tr("Top-left:  ") + QString::number(pos1.y(), 'f', 3) + "m  " + getAngleString(pos1.x()) + "\n";
            str += tr("Bottom-right:  ") + QString::number(pos2.y(), 'f', 3) + "m  " + getAngleString(pos2.x());
            break;
        }

        case AnyShape:
        {
            GraphicsAnyshape *i = dynamic_cast<GraphicsAnyshape *>(item);

            qreal area = 0.0;
            qreal x1, y1, x2, y2;

            QPolygonF polygon = i->polygon();
            polygon.append(polygon.first());
            int count = polygon.count();

            for (int i = 0; i < count - 1; i++)
            {
                x1 = polygon.at(i).x();
                y1 = polygon.at(i).y();
                x2 = polygon.at(i+1).x();
                y2 = polygon.at(i+1).y();
                area += x1 * y2 - x2 * y1;
            }
            area = qFabs(area);
            area /= 2 * GraphicsSettings::instance()->getXRatio() * GraphicsSettings::instance()->getYRatio();
            area *= 10000;

            qreal min = scene2Real(polygon.at(0)).y();
            qreal max = scene2Real(polygon.at(0)).y();

            for (int i = 1; i < polygon.count(); i++)
            {
                min = (min < scene2Real(polygon.at(i)).y()) ? min : scene2Real(polygon.at(i)).y();
                max = (max > scene2Real(polygon.at(i)).y()) ? max : scene2Real(polygon.at(i)).y();
            }

            str += QString::number(min, 'f', 3) + "m-" + QString::number(max, 'f', 3) + "m  ";
            str += tr("Area:  ") + QString::number(area, 'f', 2) + "cm2\n";

            polygon.removeLast();
            for (int i = 0; i < polygon.count(); i++)
            {
                QPointF pos = scene2Real(polygon.at(i));
                str += tr("Point ") + QString::number(i+1) + ":  " + QString::number(pos.y(), 'f', 3) + "m  " + getAngleString(pos.x()) + "\n";
            }

            break;
        }

        case Ruler:
        {
            GraphicsLineItem *i = dynamic_cast<GraphicsLineItem *>(item);
            QPointF pos1 = scene2Real(i->line().p1());
            QPointF pos2 = scene2Real(i->line().p2());

            qreal x = qFabs(i->line().p1().x() - i->line().p2().x()) / GraphicsSettings::instance()->getXRatio();
            qreal y = qFabs(i->line().p1().y() - i->line().p2().y()) / GraphicsSettings::instance()->getYRatio();
            qreal length = QLineF(x, 0, 0, y).length();

            if (pos1.y() < pos2.y())
                str += QString::number(pos1.y(), 'f', 3) + "m-" + QString::number(pos2.y(), 'f', 3) + "m  ";
            else
                str += QString::number(pos2.y(), 'f', 3) + "m-" + QString::number(pos1.y(), 'f', 3) + "m  ";
            str += tr("Length:  ") + QString::number(length * 100, 'f', 2) + "cm\n";
            str += tr("Start:  ") + QString::number(pos1.y(), 'f', 3) + "m  " + getAngleString(pos1.x()) + "\n";
            str += tr("End:  ") + QString::number(pos2.y(), 'f', 3) + "m  " + getAngleString(pos2.x());

            break;
        }

        case Occurance:
        {
            GraphicsOccurance *i = dynamic_cast<GraphicsOccurance *>(item);
            QPointF pos1 = scene2Real(i->line().p1());
            QPointF pos2 = scene2Real(i->line().p2());

            qreal radius = pixmap_diameter / 2;

            qreal angle1;   // 等腰三角形的顶角
            qreal angle2;   // 等腰三角形的顶角角平分线的方位角
            qreal angle3;   // 倾向方位角

            angle1 = qFabs(pos2.x() - pos1.x());

            // 如果方位角之差大于180°
            if (angle1 > 180)
            {
                angle1 = 360 - angle1;
                qreal min = (pos2.x() > pos1.x()) ? pos1.x() : pos2.x();
                qreal max = (pos2.x() > pos1.x()) ? pos2.x() : pos1.x();
                min += 360;
                angle2 = (min + max) / 2;
                angle3 = angle2 - 90;
            }
            // 如果方位角之差小于等于180°
            else
            {
                angle2 = (pos2.x() - pos1.x()) / 2;
                angle3 = angle2 + 90;
                if (angle3 >= 360)
                    angle3 -= 360;
            }

            if ((pos1.x() > pos2.x() && pos1.y() < pos2.y()) || (pos1.x() < pos2.x() && pos1.y() > pos2.y()))
            {
                angle3 += 180;
                if (angle3 >= 360)
                    angle3 -= 360;
            }

            qreal hemline = qSqrt(2 * qPow(radius, 2) - 2 * qPow(radius, 2) * qCos(qDegreesToRadians(angle1)));
            qreal height = qFabs(pos1.y() - pos2.y());
            qreal angle = qRadiansToDegrees(qAtan(height / hemline));
            qreal realLength = qSqrt(qPow(hemline, 2) + qPow(height, 2));


            qreal x = qFabs(i->line().p1().x() - i->line().p2().x()) / GraphicsSettings::instance()->getXRatio();
            qreal y = qFabs(i->line().p1().y() - i->line().p2().y()) / GraphicsSettings::instance()->getYRatio();
            qreal length = QLineF(x, 0, 0, y).length();

            if (pos1.y() < pos2.y())
                str += QString::number(pos1.y(), 'f', 3) + "m-" + QString::number(pos2.y(), 'f', 3) + "m  ";
            else
                str += QString::number(pos2.y(), 'f', 3) + "m-" + QString::number(pos1.y(), 'f', 3) + "m  ";
            str += tr("Inclination angle:  ") + getAngleString(angle3) + tr(" ") + QString::number(angle, 'f', 2) + "°\n";
            str += tr("Length:  ") + QString::number(length * 100, 'f', 2) + "cm\n";
            str += tr("Real length:  ") + QString::number(realLength * 100, 'f', 2) + "cm\n";
            str += tr("Start:  ") + QString::number(pos1.y(), 'f', 3) + "m  " + getAngleString(pos1.x()) + "\n";
            str += tr("End:  ") + QString::number(pos2.y(), 'f', 3) + "m  " + getAngleString(pos2.x());

            break;
        }

        case Text:
        {
            GraphicsTextItem *i = dynamic_cast<GraphicsTextItem *>(item);
            str += QString::number(scene2Real(i->scenePos()).y(), 'f', 3) + "m  ";
            str += tr("Text:  ") + i->text();
            break;
        }


        default:
            break;
    }
    return str;
}

QString GraphicsScene::getAngleString(qreal angle)
{
    if (angle < 0 || angle >= 360)
        return QString();

    QString angle_str;

    if (angle >= 0 && angle < 90)
    {
        angle_str += tr("North-northeast ") + QString::number(angle, 'f', 1) + "°";
    }

    else if (angle >= 90 && angle < 180)
    {
        angle_str += tr("South-southeast ") + QString::number(180 - angle, 'f', 1) + "°";
    }

    else if (angle >= 180 && angle < 270)
    {
        angle_str += tr("South-southwest ") + QString::number(angle - 180, 'f', 1) + "°";
    }

    else if (angle >= 270 && angle < 360)
    {
        angle_str += tr("North-northwest ") + QString::number(360 - angle, 'f', 1) + "°";
    }

    return angle_str;
}



bool GraphicsScene::hasNewItem()
{
    return !newItems.isEmpty();
}

QMap<QString, QGraphicsItem *> GraphicsScene::getNewItems()
{
    return newItems;
}

void GraphicsScene::saveNewItems()
{
    for (int i = 0; i < newItems.count(); i++)
    {
        addItemData(QUuid(newItems.keys().at(i)), newItems.values().at(i), true);
    }
    newItems.clear();
    updateTable();
}


void GraphicsScene::addItemData(QUuid uuid, QGraphicsItem *item, bool saved)
{
    if (!saved)
    {
        newItems.insert(uuid.toString(), item);
    }
    else
    {
        savedItems.insert(uuid.toString(), item);
    }
}

void GraphicsScene::deleteItemData(QUuid uuid)
{
    if (newItems.contains(uuid.toString()))
    {
        removeItem(newItems.value(uuid.toString()));
        newItems.remove(uuid.toString());
    }
    else if (savedItems.contains(uuid.toString()))
    {
        removeItem(savedItems.value(uuid.toString()));
        savedItems.remove(uuid.toString());
        emit deleteSavedItem(uuid);
    }
    updateTable();
}

void GraphicsScene::clearItemData()
{
    clear();
    newItems.clear();
    savedItems.clear();
    updateTable();
}

void GraphicsScene::updateTable()
{
    QVector<TableData> tableDatas;

    for (int i = 0; i < newItems.count(); i++)
    {
        TableData tableData;
        QGraphicsItem *newItem = newItems.values().at(i);
        tableData.uuid = QUuid(newItems.keys().at(i));
        tableData.depth = getItemDepth(newItem);
        tableData.data = getShowString(newItem);
        tableData.isSaved = tr("No");
        switch(newItem->type())
        {
            case Rect:
            {
                GraphicsRectItem *i = dynamic_cast<GraphicsRectItem *>(newItem);
                tableData.remark = i->getRemark();
                tableData.type = tr("Rectangle");
                break;
            }

            case AnyShape:
            {
                GraphicsAnyshape *i = dynamic_cast<GraphicsAnyshape *>(newItem);
                tableData.remark = i->getRemark();
                tableData.type = tr("AnyShape");
                break;
            }

            case Ruler:
            {
                GraphicsLineItem *i = dynamic_cast<GraphicsLineItem *>(newItem);
                tableData.remark = i->getRemark();
                tableData.type = tr("Width");
                break;
            }

            case Occurance:
            {
                GraphicsOccurance *i = dynamic_cast<GraphicsOccurance *>(newItem);
                tableData.remark = i->getRemark();
                tableData.type = tr("Occurance");
                break;
            }

            case Text:
            {
                GraphicsTextItem *i = dynamic_cast<GraphicsTextItem *>(newItem);
                tableData.remark = i->getRemark();
                tableData.type = tr("Text");
                break;
            }

            default:
                break;
        }
        if (tableDatas.isEmpty())
            tableDatas.append(tableData);
        else
        {
            int count = tableDatas.count();
            for (int j = 0; j < count; j++)
            {
                if (tableData.depth < tableDatas.at(j).depth)
                {
                    tableDatas.insert(j, tableData);
                    break;
                }
                else if (tableDatas.count() - 1 == j)
                    tableDatas.append(tableData);
            }
        }

    }

    for (int i = 0; i < savedItems.count(); i++)
    {
        TableData tableData;
        QGraphicsItem *savedItem = savedItems.values().at(i);
        tableData.uuid = QUuid(savedItems.keys().at(i));
        tableData.depth = getItemDepth(savedItem);
        tableData.data = getShowString(savedItem);
        tableData.isSaved = tr("Yes");
        switch(savedItem->type())
        {
            case Rect:
            {
                GraphicsRectItem *i = dynamic_cast<GraphicsRectItem *>(savedItem);
                tableData.remark = i->getRemark();
                tableData.type = tr("Rectangle");
                break;
            }

            case AnyShape:
            {
                GraphicsAnyshape *i = dynamic_cast<GraphicsAnyshape *>(savedItem);
                tableData.remark = i->getRemark();
                tableData.type = tr("AnyShape");
                break;
            }

            case Ruler:
            {
                GraphicsLineItem *i = dynamic_cast<GraphicsLineItem *>(savedItem);
                tableData.remark = i->getRemark();
                tableData.type = tr("Width");
                break;
            }

            case Occurance:
            {
                GraphicsOccurance *i = dynamic_cast<GraphicsOccurance *>(savedItem);
                tableData.remark = i->getRemark();
                tableData.type = tr("Occurance");
                break;
            }

            case Text:
            {
                GraphicsTextItem *i = dynamic_cast<GraphicsTextItem *>(savedItem);
                tableData.remark = i->getRemark();
                tableData.type = tr("Text");
                break;
            }

            default:
                break;
        }
        if (tableDatas.isEmpty())
            tableDatas.append(tableData);
        else
        {
            int count = tableDatas.count();
            for (int j = 0; j < count; j++)
            {
                if (tableData.depth < tableDatas.at(j).depth)
                {
                    tableDatas.insert(j, tableData);
                    break;
                }
                else if (tableDatas.count() - 1 == j)
                    tableDatas.append(tableData);
            }
        }
    }
    emit emitTableData(tableDatas);
}


void GraphicsScene::updateItemRemark(QUuid uuid, QString remark)
{
    if (newItems.contains(uuid.toString()))
    {
        QGraphicsItem *item = newItems.value(uuid.toString());
        switch (item->type())
        {
            case AnyShape:
            {
                GraphicsAnyshape *i = dynamic_cast<GraphicsAnyshape *>(item);
                i->setRemark(remark);
                break;
            }
            case Ruler:
            {
                GraphicsLineItem *i = dynamic_cast<GraphicsLineItem *>(item);
                i->setRemark(remark);
                break;
            }
            case Occurance:
            {
                GraphicsOccurance *i = dynamic_cast<GraphicsOccurance *>(item);
                i->setRemark(remark);
                break;
            }
            case Rect:
            {
                GraphicsRectItem *i = dynamic_cast<GraphicsRectItem *>(item);
                i->setRemark(remark);
                break;
            }

            case Text:
            {
                GraphicsTextItem *i = dynamic_cast<GraphicsTextItem *>(item);
                i->setRemark(remark);
                break;
            }

            default:
            {
                break;
            }
        }
    }
    else if (savedItems.contains(uuid.toString()))
    {
        QGraphicsItem *item = savedItems.value(uuid.toString());
        switch (item->type())
        {
            case AnyShape:
            {
                GraphicsAnyshape *i = dynamic_cast<GraphicsAnyshape *>(item);
                i->setRemark(remark);
                break;
            }
            case Ruler:
            {
                GraphicsLineItem *i = dynamic_cast<GraphicsLineItem *>(item);
                i->setRemark(remark);
                break;
            }
            case Occurance:
            {
                GraphicsOccurance *i = dynamic_cast<GraphicsOccurance *>(item);
                i->setRemark(remark);
                break;
            }
            case Rect:
            {
                GraphicsRectItem *i = dynamic_cast<GraphicsRectItem *>(item);
                i->setRemark(remark);
                break;
            }

            case Text:
            {
                GraphicsTextItem *i = dynamic_cast<GraphicsTextItem *>(item);
                i->setRemark(remark);
                break;
            }

            default:
            {
                break;
            }
        }
        emit updateSavedItemRemark(uuid, remark);
    }
}


QStringList GraphicsScene::getAllItemString()
{
    QStringList strList;
    for (int i = 0; i < savedItems.count(); i++)
    {
        QString remark;
        QGraphicsItem *item = savedItems.values().at(i);
        switch (item->type())
        {
            case AnyShape:
            {
                GraphicsAnyshape *i = dynamic_cast<GraphicsAnyshape *>(item);
                remark = i->getRemark();
                break;
            }
            case Ruler:
            {
                GraphicsLineItem *i = dynamic_cast<GraphicsLineItem *>(item);
                remark = i->getRemark();
                break;
            }
            case Occurance:
            {
                GraphicsOccurance *i = dynamic_cast<GraphicsOccurance *>(item);
                remark = i->getRemark();
                break;
            }
            case Rect:
            {
                GraphicsRectItem *i = dynamic_cast<GraphicsRectItem *>(item);
                remark = i->getRemark();
                break;
            }

            case Text:
            {
                GraphicsTextItem *i = dynamic_cast<GraphicsTextItem *>(item);
                remark = i->getRemark();
                break;
            }

            default:
            {
                break;
            }
        }

        QString content = getShowString(savedItems.values().at(i)).section("\n", 0, 0);
        if (!remark.isEmpty())
            content += " " + tr("remark: ") + remark +"\n";
        else
            content += "\n";

        strList << content;
    }
    return strList;
}


qreal GraphicsScene::getItemDepth(QGraphicsItem *item)
{
    qreal depth = 0.0;

    switch (item->type())
    {
        case AnyShape:
        {
            GraphicsAnyshape *i = dynamic_cast<GraphicsAnyshape *>(item);
            QPolygonF polygon = i->polygon();
            qreal min = scene2Real(polygon.at(0)).y();
            qreal max = scene2Real(polygon.at(0)).y();
            for (int j = 1; j < polygon.count(); j++)
            {
                min = (min < scene2Real(polygon.at(j)).y()) ? min : scene2Real(polygon.at(j)).y();
                max = (max > scene2Real(polygon.at(j)).y()) ? max : scene2Real(polygon.at(j)).y();
            }
            depth = min;
            break;
        }
        case Ruler:
        {
            GraphicsLineItem *i = dynamic_cast<GraphicsLineItem *>(item);
            QPointF pos1 = scene2Real(i->line().p1());
            QPointF pos2 = scene2Real(i->line().p2());
            if (pos1.y() < pos2.y())
                depth = pos1.y();
            else
                depth = pos2.y();
            break;
        }
        case Occurance:
        {
            GraphicsOccurance *i = dynamic_cast<GraphicsOccurance *>(item);
            QPointF pos1 = scene2Real(i->line().p1());
            QPointF pos2 = scene2Real(i->line().p2());
            if (pos1.y() < pos2.y())
                depth = pos1.y();
            else
                depth = pos2.y();
            break;
        }
        case Rect:
        {
            GraphicsRectItem *i = dynamic_cast<GraphicsRectItem *>(item);
            depth = scene2Real(i->rect().topLeft()).y();
            break;
        }

        case Text:
        {
            GraphicsTextItem *i = dynamic_cast<GraphicsTextItem *>(item);
            depth = scene2Real(i->scenePos()).y();
            break;
        }

        default:
        {
            break;
        }
    }

    return depth;
}

