#include "copyenginedefault.h"

#include <QDebug>
#include <QThread>

#define BUFF_SIZE 1024*4

CopyEngineDefault::CopyEngineDefault(QString source, QString dest, QString basePath, bool paused)
    : CopyEngineAbstract(source, dest, basePath, paused)
{
}

bool CopyEngineDefault::copy(QString &errStr, ICopyEngineCallback *callback)
{
    // if base path is passet, then we're copying a dir tree
    if (!m_basePath.isEmpty())
        mkDestDirPath();

    QFile toFile(m_dest + "/" + QFileInfo(m_source).fileName());
    if (!toFile.open(QFile::WriteOnly))
    {
        errStr = QString("Failed to open destination % file for writing!").arg(m_dest);
        qDebug() << errStr;
        return false;
    }

    QFile fromFile(m_source);
    if(!fromFile.open(QFile::ReadOnly))
    {
        errStr = QString("Failed to source destination % file for reading!").arg(m_source);
        qDebug() << errStr;
        return false;
    }

    long long pos;
    getCtrlFile(pos); // get or create the ctrl file
    toFile.seek(pos); fromFile.seek(pos); // set the cursor to the correct pos
    if (callback) callback->writtenBytes(pos);

    QScopedArrayPointer<char> buffer(new char[BUFF_SIZE]);
    qint64 readBytes = pos, totalWritten = pos;
    while ((readBytes = fromFile.read(buffer.data(), BUFF_SIZE)) && readBytes >= 0)
    {
        m_sync.lock();
        if(m_pause)
            m_pauseCond.wait(&m_sync);
        m_sync.unlock();

        if(m_cancel)
        {
            break;
        }

        qDebug() << "Copying file" <<  m_source << "in thread - " << QThread::currentThreadId();

        qint64 writtenBytes = 0;
        while(writtenBytes < readBytes)
        {
            qint64 tmpWritten = toFile.write(buffer.data() + writtenBytes, readBytes - writtenBytes);
            writtenBytes += tmpWritten;            
        }


        if (callback)
            callback->writtenBytes(writtenBytes);

        QThread::yieldCurrentThread(); // give time to other threads

        totalWritten += writtenBytes;
        updateCtrlFile(totalWritten);
    }

    if (m_cancel)
    {
        if (toFile.exists())
            toFile.remove();
    }
    else if (toFile.exists())
    {
        toFile.setPermissions(fromFile.permissions());
    }

    removeCtrlFile();
    return true;
}
