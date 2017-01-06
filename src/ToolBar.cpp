#include "ToolBar.h"

ToolBar::ToolBar(QWidget *parent) :
    QToolBar(parent)
{
    setFloatable(false);
    setMovable(false);

    colorCBox = new QComboBox();
    colorCBox->setToolTip(tr("Choose color"));
    colorCBox->setStatusTip(tr("Choose color"));

    QPixmap pix(16, 16);
    QPainter painter(&pix);
    painter.fillRect(0, 0, 16, 16, Qt::black);
    colorCBox->addItem(QIcon(pix), tr("black"));
    painter.fillRect(0, 0, 16, 16, Qt::white);
    colorCBox->addItem(QIcon(pix), tr("white"));
    painter.fillRect(0, 0, 16, 16, Qt::red);
    colorCBox->addItem(QIcon(pix), tr("red"));
    painter.fillRect(0, 0, 16, 16, Qt::green);
    colorCBox->addItem(QIcon(pix), tr("green"));
    painter.fillRect(0, 0, 16, 16, Qt::blue);
    colorCBox->addItem(QIcon(pix), tr("blue"));
    painter.fillRect(0, 0, 16, 16, Qt::yellow);
    colorCBox->addItem(QIcon(pix), tr("yellow"));

    lineSizeSBox = new QSpinBox();
    lineSizeSBox->setRange(1, 50);
    lineSizeSBox->setValue(8);
    lineSizeSBox->setPrefix(tr("Width "));



    addWidget(colorCBox);
    addWidget(lineSizeSBox);
}


ToolBar::~ToolBar()
{

}
