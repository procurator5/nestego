#ifndef CSIGNAL_H
#define CSIGNAL_H

#include <QObject>
#include "analyse.h"
#include "buffer.h"

enum Level{
    DEBUG,
    INFO,
    WARN,
    ERROR,
    CRITICAL
};

class CSignal : public QObject
{
    Q_OBJECT
public:
    explicit CSignal(QObject *parent = 0);
    void logMessage(Level level, QString message);
    /**
     * @brief Однократный клик мышкой на анализатор
     * @param Analyse* указатель на анализатор
     */
    void analyseMouseClicked(Analyse* analyse);

    void bufferMouseClicked(Buffer* buffer);

signals:
    void signalLogMessage(int level, QString message);
    void signalAnalyseMouseClicked(Analyse* analyse);
    void signalBufferMouseClicked(Buffer* analyse);

public slots:

};

#endif // CSIGNAL_H
