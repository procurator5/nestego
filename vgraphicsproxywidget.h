#ifndef VGRAPHICSPROXYWIDGET_H
#define VGRAPHICSPROXYWIDGET_H

#include <QGraphicsProxyWidget>
#include "vneuralanalyse.h"

class vGraphicsProxyWidget : public QGraphicsProxyWidget
{
    Q_OBJECT
public:
    explicit vGraphicsProxyWidget(QGraphicsItem * parent = 0);

signals:

public slots:


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

};

#endif // VGRAPHICSPROXYWIDGET_H
