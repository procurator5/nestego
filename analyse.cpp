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
#include "analyse.h"
#include "buffer.h"
#include "vanalyserform.h"

#include "csignal.h"

extern CSignal* signal;

#include <QDebug>

//! [0]
Analyse::Analyse(QGraphicsView *graphWidget)
    : Node(graphWidget), num_input(0), name("noname"), fann(NULL)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
}
//! [0]

//! [1]
void Analyse::addEdge(Edge *edge)
{
    edgeList << edge;
    edge->adjust();
}

QList<Edge *> Analyse::edges() const
{
    return edgeList;
}
//! [1]

//! [2]
void Analyse::calculateForces()
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
        Analyse *node = qgraphicsitem_cast<Analyse *>(item);
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

//! [4]
    // Now subtract all forces pulling items together
    double weight = (edgeList.size() + 1) * 10;
    foreach (Edge *edge, edgeList) {
        QPointF vec;
        if (edge->sourceNode() == this)
            vec = mapToItem(edge->destNode(), 0, 0);
        else
            vec = mapToItem(edge->sourceNode(), 0, 0);
        xvel -= vec.x() / weight;
        yvel -= vec.y() / weight;
    }
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
bool Analyse::advance()
{
    if (newPos == pos())
        return false;

    setPos(newPos);
    return true;
}
//! [7]

//! [8]
QRectF Analyse::boundingRect() const
{
#if defined(Q_OS_SYMBIAN) || defined(Q_WS_MAEMO_5)
    // Add some extra space around the circle for easier touching with finger
    qreal adjust = 30;
    return QRectF( -10 - adjust, -10 - adjust,
                  50 + adjust * 2, 80 + adjust * 2);
#else
    qreal adjust = 2;
    return QRectF( -10 - adjust, -10 - adjust,
                  63 + adjust, 93 + adjust);
#endif
}
//! [8]

//! [9]
QPainterPath Analyse::shape() const
{
    QPainterPath path;
#if defined(Q_OS_SYMBIAN) || defined(Q_WS_MAEMO_5)
    // Add some extra space around the circle for easier touching with finger
    path.addEllipse( -40, -40, 80, 80);
#else
    path.addEllipse(-10, -10, 60, 60);
#endif
    return path;
}
//! [9]

//! [10]
void Analyse::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
    painter->drawEllipse(-7, -7, 60, 60);

    QRadialGradient gradient(32, 32, 10);
    if (option->state & QStyle::State_Sunken) {
        gradient.setCenter(32, 32);
        gradient.setFocalPoint(32, 32);
        gradient.setColorAt(1, QColor(Qt::yellow).light(120));
        gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
    } else {
        gradient.setColorAt(0, Qt::yellow);
        gradient.setColorAt(1, Qt::darkYellow);
    }
    painter->setBrush(gradient);

    painter->drawEllipse(-10, -10, 60, 60);

    painter->setPen(QPen(Qt::black, 0));

    painter->fillRect(QRectF(-7,50, boundingRect().width(), 30), Qt::white);

    painter->setBrush(Qt::white);
    painter->drawRect(QRectF(-6,51, boundingRect().width()-10, 28));

    painter->drawText(QRectF(-6,51, boundingRect().width()-10, 28), Qt::AlignCenter, name);
}
//! [10]

//! [11]
QVariant Analyse::itemChange(GraphicsItemChange change, const QVariant &value)
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
void Analyse::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    signal->analyseMouseClicked(this);
    QGraphicsItem::mousePressEvent(event);
}

void Analyse::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
//! [12]

void Analyse::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event){
    vAnalyserForm * af = new vAnalyserForm();
    af->setAnalyseName(name);
    af->setLibFannParametrs(3, 3, FANN_GAUSSIAN_SYMMETRIC);
    af->setDataSource(edgeList);
    af->exec();

    if(af->isAcceptButtonClicked()){

    }
    delete af;
}

bool Analyse::createNeuralNetwork(){
    return createLibFann();

}

vLibFannClass* Analyse::getLibFann(){
    if(!createLibFann())
        return NULL;
    return fann;
}

bool Analyse::createLibFann(){

    if(fann != NULL){
        delete fann;
        num_input = 0;
        good = 0;
        bad = 0;
    }
    //Подсчитываем количество входных параметров (в байтах)

    for(QList<Edge*>::iterator it=edgeList.begin(); it!=edgeList.end();++it){
        Node* source = (*it)->sourceNode();
        Buffer* buff = dynamic_cast<Buffer*> (source);
        if(buff != NULL)
            num_input = num_input > buff->getBufferSize()? num_input :buff->getBufferSize();
    }
    if(num_input ==0)
        return false;

    const unsigned int num_output = 1;

//Настройки нейронной стети, которые задаются соответствующими параметрами
//Из пользовательской формы
    const unsigned int num_layers = 3;
    const unsigned int num_neurons_hidden = 3;

    fann = new vLibFannClass(num_input);

    return true;
}

int Analyse::trainFann(vIBuffer * buffer){

    if(fann == NULL)
        return -1;
   fann->startTrainThread(buffer);
    return 0;

}
