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

#ifndef ANALYSE_H
#define ANALYSE_H

#include <QGraphicsItem>
#include <QList>
#include <QGraphicsView>

#include "node.h"
#include "vlibfannclass.h"
#include "vtrainthread.h"
#include "vibuffer.h"

class Edge;
QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

//! [0]
class Analyse : public Node
{
public:
    Analyse(QGraphicsView *graphWidget);

    /**
     * @brief Создается нейронная сеть для ализа данных
     * @return bool создание сети прошло успешно/неуспешно
     */
    bool createNeuralNetwork();

    int trainFann(vIBuffer* buffer);

    /**
     * @brief Возвращает значение вычисления НС
     * @param buffer Входные данные
     * @return
     */
    int getResult(QByteArray buffer);

    /**
     * @brief Тренировка НС
     * @param buffer Входные данные
     * @param result Ожидаемый результат
     * @return
     */
    int train(QByteArray buffer, int result);

    void addEdge(Edge *edge);
    QList<Edge *> edges() const;

    QList <train_result>& getTrainResult(){
        if(fann == NULL){
            QList <train_result> empty;
            return empty;
        }
        return fann->getTrainResults();
    };

    /**
     * @brief saveAnalyse сохраняет настройки нейронной сети в файле
     * @param filename имя файла
     * @return результат сохранения
     */
    bool saveAnalyse(QString filename);

    /**
     * @brief loadAnalyseFromFile загрузка НС из файла
     * @param filename -имя файла
     * @return результат загрузки
     */
    bool loadAnalyseFromFile(QString filename);

    void setTrainResult( int output_before_train, int output_after_train, int need_result);

    enum { Type = UserType + 1 };
    int type() const { return Type; }

    void calculateForces();
    bool advance();

    void setName(QString nm){
        name = nm;
        update();
    };

    QString getName(){
        return name;
    };

    unsigned int getNumInput(){
        return num_input;
    };

    QRectF boundingRect() const;
    QPainterPath shape() const;

    /**
     * @brief Передача элемента vLibFannClass для настройки
     * @return vLibFannClass* или NULL
     */
    vLibFannClass* getLibFann();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *);

    void setStructTrainResult(struct train_result step);
    
private:
    QList<Edge *> edgeList;
    QPointF newPos;
    QGraphicsView *graph;
    /**
     * @brief Имя анализатора
     */
    QString name;
    /**
     * @brief Нейронная сеть из библиотеки LIBFANN
     */

    vLibFannClass *fann;
    /**
     * @brief Количество байт на входе нейронной сети
     */
    unsigned int num_input;

    /**
     * @brief Создание нейронной сети в стиле LibFann
     * @return
     */
    bool createLibFann();

    unsigned long good;
    unsigned long bad;
};
//! [0]

#endif
