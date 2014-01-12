#ifndef COPIERWORKER_H
#define COPIERWORKER_H

#include <QThread>

class CopierWorker : public QThread
{
    Q_OBJECT

public:
    CopierWorker(QString source, QString dest);

signals:
    void bytesWritten(qint64);
    void done(int m_Id);
    void error(QString file);

public slots:
    void doWork();

private:
    QString m_Source;
    QString m_Dest;
};

#endif // COPIERWORKER_H
