#ifndef COPIERWORKER_H
#define COPIERWORKER_H

#include <QObject>
#include <QFile>
#include <QMutex>
#include <QWaitCondition>

#include "engines/copyenginedefault.h"

class CopierWorker  : public QObject, ICopyEngineCallback
{
    Q_OBJECT

public:
    CopierWorker(QString source, QString dest, QString basePath = QString());
    ~CopierWorker();

    void resume()
    {
        if (m_copyEngine)
            m_copyEngine->resume();
    }

    void pause()
    {
        if (m_copyEngine)
            m_copyEngine->pause();
    }

    void cancel()
    {
        if (m_copyEngine)
            m_copyEngine->cancel();
    }

    void writtenBytes(long long &bytes);

signals:
    void bytesWritten(qint64);
    void error(QString file);
    void done();

public slots:
    void run();

private:
    void writtenBytes(long long bytes);

private:
    QString m_source;
    QString m_dest;
    QString m_basePath;

    ICopyEngine *m_copyEngine;
};

#endif // COPIERWORKER_H
