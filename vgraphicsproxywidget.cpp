#include "vgraphicsproxywidget.h"

#include <QDebug>
#include <QPointF>
#include <QGraphicsSceneMouseEvent>
#include <QPointer>

vGraphicsProxyWidget::vGraphicsProxyWidget(QGraphicsItem * parent  ) :
    QGraphicsProxyWidget(parent)
{
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setWidget(new vNeuralAnalyse);
}

void vGraphicsProxyWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
        QGraphicsItem::mousePressEvent(event);
}

void vGraphicsProxyWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
        QGraphicsProxyWidget::mouseReleaseEvent(event);

}

void vGraphicsProxyWidget::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    QGraphicsItem::mouseMoveEvent(event);
}
