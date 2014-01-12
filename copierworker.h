#ifndef COPIERWORKER_H
#define COPIERWORKER_H

#include <QObject>
#include <QRunnable>

class CopierWorker  : public QObject, public QRunnable
{
    Q_OBJECT

public:
    CopierWorker(QString source, QString dest);

signals:
    void bytesWritten(qint64);
    void done();
    void error(QString file);

public:
    void run();

private:
    QString m_Source;
    QString m_Dest;
};

#endif // COPIERWORKER_H
