#include "csignal.h"

CSignal::CSignal(QObject *parent) :
    QObject(parent)
{
}

void CSignal::logMessage(Level level, QString message){
    emit signalLogMessage(level, message);
}

void CSignal::analyseMouseClicked(Analyse* analyse){
    emit signalAnalyseMouseClicked(analyse);
}
