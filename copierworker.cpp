#include "copierworker.h"

#include <QFile>
#include <QDir>
#include <QThread>
#include <QDebug>

CopierWorker::CopierWorker(QString source, QString dest, QString basePath)
    : m_source(source),
      m_dest(dest),
      m_basePath(basePath)
{
    m_copyEngine = new CopyEngineDefault(source, dest, basePath); // TODO: we need to use a factory here
}

CopierWorker::~CopierWorker()
{
    delete m_copyEngine;
}

void CopierWorker::run()
{
    if (m_copyEngine)
    {
        QString errorStr;
        if (!m_copyEngine->copy(errorStr, this))
        {
            emit error(errorStr);
            return;
        }
    }

    emit done();
}

void CopierWorker::writtenBytes(long long &bytes)
{
    emit bytesWritten(bytes);
}
