/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

#include "edge.h"
#include "buffer.h"
#include "csignal.h"

extern CSignal *signal;

//! [0]
Buffer::Buffer(QGraphicsView *graphWidget)
    : Node(graphWidget)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
    buffer_size = 2;
    bform = NULL;
}
//! [0]

//! [1]
void Buffer::addEdge(Edge *edge)
{
    edgeList << edge;
    edge->adjust();
}

QList<Edge *> Buffer::edges() const
{
    return edgeList;
}
//! [1]

//! [2]
void Buffer::calculateForces()
{
    if (!scene() || scene()->mouseGrabberItem() == this) {
        newPos = pos();
        return;
    }
//! [2]

//! [3]
    // Sum up all forces pushing this item away
    qreal xvel = 0;
    qreal yvel = 0;
    foreach (QGraphicsItem *item, scene()->items()) {
        Buffer *node = qgraphicsitem_cast<Buffer *>(item);
        if (!node)
            continue;

        QPointF vec = mapToItem(node, 0, 0);
        qreal dx = vec.x();
        qreal dy = vec.y();
        double l = 2.0 * (dx * dx + dy * dy);
        if (l > 0) {
            xvel += (dx * 150.0) / l;
            yvel += (dy * 150.0) / l;
        }
    }
//! [3]

////! [4]
//    // Now subtract all forces pulling items together
//    double weight = (edgeList.size() + 1) * 10;
//    foreach (Edge *edge, edgeList) {
//        QPointF vec;
//        if (edge->sourceBuffer() == this)
//            vec = mapToItem(edge->destBuffer(), 0, 0);
//        else
//            vec = mapToItem(edge->sourceBuffer(), 0, 0);
//        xvel -= vec.x() / weight;
//        yvel -= vec.y() / weight;
//    }
//! [4]

//! [5]
    if (qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1)
        xvel = yvel = 0;
//! [5]

//! [6]
    QRectF sceneRect = scene()->sceneRect();
    newPos = pos() + QPointF(xvel, yvel);
    newPos.setX(qMin(qMax(newPos.x(), sceneRect.left() + 10), sceneRect.right() - 10));
    newPos.setY(qMin(qMax(newPos.y(), sceneRect.top() + 10), sceneRect.bottom() - 10));
}
//! [6]

//! [7]
bool Buffer::advance()
{
    if (newPos == pos())
        return false;

    setPos(newPos);
    return true;
}
//! [7]

//! [8]
QRectF Buffer::boundingRect() const
{
    const int cellW = 30;
    return QRectF( 0, 0,
                  cellW, cellW*buffer_size> 600? 603: cellW*buffer_size + 3);
}
//! [8]

//! [9]
QPainterPath Buffer::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());

    return path;
}
//! [9]

#include <QDebug>
void Buffer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{

    const int cellW = 30;
    QRadialGradient gradient(-3, -3, 10);
    if (option->state & QStyle::State_Sunken) {
        gradient.setColorAt(1, QColor(Qt::yellow).light(120));
        gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
    } else {
        gradient.setColorAt(0, Qt::yellow);
        gradient.setColorAt(1, Qt::darkYellow);
    }

    QRadialGradient red(-3, -3, 10);
    if (option->state & QStyle::State_Sunken) {
        red.setColorAt(1, QColor(Qt::red).light(120));
        red.setColorAt(0, QColor(Qt::darkRed).light(120));
    } else {
        red.setColorAt(0, Qt::red);
        red.setColorAt(1, Qt::darkRed);
    }

    painter->setPen(QPen(Qt::black, 0));
    qreal cellH = cellW*buffer_size> 600? (qreal)600/buffer_size: 30;
    for(unsigned int i=0; i<buffer_size; i++){
        //Рисуем в зависимости от различий
        if(diff.size()<=i)
            painter->setBrush(gradient);
        else if(diff.at(i)!=0){
            painter->setBrush(red);
        }else{
            painter->setBrush(gradient);
        }
        painter->drawRect(0, i*cellH, cellW, (i+1)*cellH);
    }
}
//! [10]

//! [11]
QVariant Buffer::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        foreach (Edge *edge, edgeList)
            edge->adjust();
//        graph->itemMoved();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}
//! [11]

//! [12]
void Buffer::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
    signal->bufferMouseClicked(this);
}

void Buffer::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
//! [12]

void Buffer::loadData(QByteArray stego, QByteArray orig){
    work.clear();
    work.append(stego);
    origin.clear();
    origin.append(orig);
    diff.clear();
    if(origin.size()>0)
        for(int i =0; i<orig.size(); i++){
            unsigned char diff_element = origin[i] - work[i];
            diff.append(diff_element);
        }

}

#include <QTableWidget>

vBufferViewForm* Buffer::getBufferForm(){
    if(bform == NULL){
        bform = new vBufferViewForm;
        bform->setWindowTitle("buffer(" + QString("%1").arg(getBufferSize())+")");
    }

    QTableWidget *table = bform->getTableWidget();
    table->clear();
    table->setRowCount(0);
    table->setRowCount(buffer_size);

    for(int i =0; i< buffer_size;i++){
        unsigned int delta = abs(abs(work[i])- abs(origin[i]));

        QTableWidgetItem* item1 = new QTableWidgetItem(QString("%1(%2)").arg(origin[i]).arg((unsigned char)origin[i]));
        table->setItem(i, 0, item1);
        QTableWidgetItem* item2 = new QTableWidgetItem(QString("%1(%2)").arg(work[i]).arg((unsigned char)work[i]));
        table->setItem(i, 1, item2);

        QTableWidgetItem* item3 = new QTableWidgetItem(QString("%1").arg(delta));
        table->setItem(i, 2, item3);

        if(delta){
            item1->setBackgroundColor(Qt::red);
            item2->setBackgroundColor(Qt::red);
            item3->setBackgroundColor(Qt::red);
        }
    }

    return bform;
}

bool Buffer::haveForm(){
    if(bform == NULL)
        return false;
    if (bform->isHidden()){
        delete bform;
        bform = NULL;
        return false;
    }
    return true;
}

int Buffer::getDiff(){
    int result =0;
    for(QByteArray::iterator it = diff.begin(); it!=diff.end(); ++it)
        result+=*it;
    return result;
}
