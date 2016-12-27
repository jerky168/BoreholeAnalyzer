#include "GraphicsScene.h"

GraphicsScene::Mode GraphicsScene::curMode = GraphicsScene::MoveItem;

GraphicsScene::GraphicsScene(QObject *parent) :
    QGraphicsScene(parent),
    item(Q_NULLPTR),
    pixmap_start(0.0),
    pixmap_end(1.0),
    pixmap_width(200.0),
    pixmap_height(1000.0),
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
    GraphicsSettings::instance()->setRatio(1.0);
    setCurMode(MoveItem);
    clearItemData();
}



void GraphicsScene::updateIndexData(QPixmap pixmap, qreal start, qreal end, QMap<QString, QGraphicsItem *> items)
{
    clearScene();
    showInfo = true;

    pixmap_start = start;
    pixmap_end = end;
    pixmap_width = pixmap.width();
    pixmap_height = pixmap.height();
    GraphicsSettings::instance()->setRatio((qreal)(pixmap.height()) / (end - start));

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
    render(&painter, image.rect(), QRectF(Border, Border, image.width(), image.height()));

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
            case InsertShift :
            {
                item = new GraphicsAngleItem(mouseEvent->scenePos());
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

    qreal width = realPos.x() / 360 * pixmap_width / GraphicsSettings::instance()->getRatio();
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
    font.setPointSize(36);
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
                painter->drawText(QPointF(x+Segment, y + 3*Segment), "N");
                break;
            case 1:
                painter->drawText(QPointF(x+Segment, y + 3*Segment), "E");
                break;
            case 2:
                painter->drawText(QPointF(x+Segment, y + 3*Segment), "S");
                break;
            case 3:
                painter->drawText(QPointF(x+Segment, y + 3*Segment), "W");
                break;
            case 4:
                painter->drawText(QPointF(x+Segment, y + 3*Segment), "N");
                break;
            default:
                break;
        }
    }
    painter->drawLines(lines);

}


QImage GraphicsScene::getImageFromData(QPixmap pixmap, qreal start, qreal end, QMap<QString, QGraphicsItem *> items)
{
    GraphicsScene *scene = new GraphicsScene;
    scene->updateIndexData(pixmap, start, end, items);
    QImage image = scene->getSceneImage();
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
        tableData.depth = QString::number(scene2Real(savedItem->pos()).y(), 'f', 3) + "m";
        tableData.data = getShowString(savedItem);
        tableData.isSaved = tr("Yes");
        switch(savedItem->type())
        {
            case Rect:
            {
                tableData.type = tr("Rectangle");
                break;
            }

            case AnyShape:
            {
                tableData.type = tr("AnyShape");
                break;
            }

            case Ruler:
            {
                tableData.type = tr("Width");
                break;
            }

            case Occurance:
            {
                tableData.type = tr("Occurance");
                break;
            }

            case Text:
            {
                tableData.type = tr("Text");
                break;
            }

            case Angle:
            {
                tableData.type = tr("Angle");
                break;
            }

            default:
                break;
        }
        tableDatas.append(tableData);
    }
    return tableDatas;
}


QVector<GraphicsScene::TableData> GraphicsScene::getTableDataFromData(QPixmap pixmap, qreal start, qreal end, QMap<QString, QGraphicsItem *> items)
{
    GraphicsScene *scene = new GraphicsScene;
    scene->updateIndexData(pixmap, start, end, items);
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

            qreal area = i->rect().width() * i->rect().height() / qPow(GraphicsSettings::instance()->getRatio(), 2) * 10000;
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
            area /= 2 * qPow(GraphicsSettings::instance()->getRatio(), 2);
            area *= 10000;

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

            qreal length = i->line().length() / GraphicsSettings::instance()->getRatio() * 100;
            str += tr("Length:  ") + QString::number(length, 'f', 2) + "cm\n";
            str += tr("Start:  ") + QString::number(pos1.y(), 'f', 3) + "m  " + getAngleString(pos1.x()) + "\n";
            str += tr("End:  ") + QString::number(pos2.y(), 'f', 3) + "m  " + getAngleString(pos2.x());

            break;
        }

        case Occurance:
        {
            GraphicsOccurance *i = dynamic_cast<GraphicsOccurance *>(item);
            QPointF pos1 = scene2Real(i->line().p1());
            QPointF pos2 = scene2Real(i->line().p2());

            qreal length = i->line().length() / GraphicsSettings::instance()->getRatio() * 100;
            str += tr("Length:  ") + QString::number(length, 'f', 2) + "cm\n";
            str += tr("Start:  ") + QString::number(pos1.y(), 'f', 3) + "m  " + getAngleString(pos1.x()) + "\n";
            str += tr("End:  ") + QString::number(pos2.y(), 'f', 3) + "m  " + getAngleString(pos2.x());

            break;
        }

        case Text:
        {
            GraphicsTextItem *i = dynamic_cast<GraphicsTextItem *>(item);
            str += tr("Text:  ") + i->text();
            break;
        }

        case Angle:
        {
            GraphicsAngleItem *i = dynamic_cast<GraphicsAngleItem *>(item);
            QPointF p1 = i->polygon().at(0);
            QPointF p2 = i->polygon().at(1);
            QPointF p3 = i->polygon().at(2);
            QPointF pos1 = scene2Real(p1);
            QPointF pos2 = scene2Real(p2);
            QPointF pos3 = scene2Real(p3);
            QLineF lineA(p1, p2), lineB(p2, p3);

            qreal lineALength = lineA.length();
            qreal lineBLength = lineB.length();
            qreal lineCLength = QLineF(p1, p3).length();
            qreal angle = qAcos((pow(lineALength, 2) + qPow(lineBLength, 2) - qPow(lineCLength, 2))/(2*lineALength*lineBLength));
            angle = angle * 180 / M_PI;

            str += tr("Angle:  ") + QString::number(angle, 'f', 2) + "°\n";
            str += tr("Vertex A:  ") + QString::number(pos1.y(), 'f', 3) + "m  " + getAngleString(pos1.x()) + "\n";
            str += tr("Vertex B:  ") + QString::number(pos2.y(), 'f', 3) + "m  " + getAngleString(pos2.x()) + "\n";
            str += tr("Vertex C:  ") + QString::number(pos3.y(), 'f', 3) + "m  " + getAngleString(pos3.x());
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
    // 如果删除的是刚添加的
    if (newItems.contains(uuid.toString()))
    {
        delete newItems.value(uuid.toString());
        removeItem(newItems.value(uuid.toString()));
        newItems.remove(uuid.toString());
    }
    // 如果删除的是已经保存的
    else if (savedItems.contains(uuid.toString()))
    {
        delete savedItems.value(uuid.toString());
        removeItem(savedItems.value(uuid.toString()));
        newItems.insert(uuid.toString(), savedItems.value(uuid.toString()));
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
        tableData.depth = QString::number(scene2Real(newItem->pos()).y(), 'f', 3) + "m";
        tableData.data = getShowString(newItem).section('\n', 0, 0);
        tableData.isSaved = tr("No");
        switch(newItem->type())
        {
            case Rect:
            {
                tableData.type = tr("Rectangle");
                break;
            }

            case AnyShape:
            {
                tableData.type = tr("AnyShape");
                break;
            }

            case Ruler:
            {
                tableData.type = tr("Width");
                break;
            }

            case Occurance:
            {
                tableData.type = tr("Occurance");
                break;
            }

            case Text:
            {
                tableData.type = tr("Text");
                break;
            }

            case Angle:
            {
                tableData.type = tr("Angle");
                break;
            }

            default:
                break;
        }
        tableDatas.append(tableData);
    }

    for (int i = 0; i < savedItems.count(); i++)
    {
        TableData tableData;
        QGraphicsItem *savedItem = savedItems.values().at(i);
        tableData.depth = QString::number(scene2Real(savedItem->pos()).y(), 'f', 3) + "m";
        tableData.data = getShowString(savedItem).section('\n', 0, 0);
        tableData.isSaved = tr("Yes");
        switch(savedItem->type())
        {
            case Rect:
            {
                tableData.type = tr("Rectangle");
                break;
            }

            case AnyShape:
            {
                tableData.type = tr("AnyShape");
                break;
            }

            case Ruler:
            {
                tableData.type = tr("Width");
                break;
            }

            case Occurance:
            {
                tableData.type = tr("Occurance");
                break;
            }

            case Text:
            {
                tableData.type = tr("Text");
                break;
            }

            case Angle:
            {
                tableData.type = tr("Angle");
                break;
            }

            default:
                break;
        }
        tableDatas.append(tableData);
    }
    emit emitTableData(tableDatas);
}

