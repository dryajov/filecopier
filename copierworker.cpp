#include "copierworker.h"

#include <QFile>
#include <QDebug>

#define BUFF_SIZE 1024*4

CopierWorker::CopierWorker(QString source, QString dest)
    : m_Source(source),
      m_Dest(dest)
{
}

void CopierWorker::run()
{
    QFile fromFile(m_Source);
    QFile toFile(m_Dest);

    if(!fromFile.open(QFile::ReadOnly))
        emit error(m_Source);

    if (toFile.open(QFile::WriteOnly))
        emit error(m_Dest);

    QScopedArrayPointer<char> buffer(new char[BUFF_SIZE]);
    qint64 readBytes = 0;
    while ((readBytes = fromFile.read(buffer.data(), BUFF_SIZE)) && readBytes >= 0)
    {
        qint64 writtenBytes = 0;
        while(writtenBytes < readBytes)
        {
            qint64 tmpWritten = toFile.write(buffer.data() + writtenBytes, readBytes - writtenBytes);
            writtenBytes += tmpWritten;
            emit bytesWritten(tmpWritten);
        }

        writtenBytes = 0;
    }
}
