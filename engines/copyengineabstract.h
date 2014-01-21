#ifndef COPYENGINEABSTRACT_H
#define COPYENGINEABSTRACT_H

#include <QMutex>
#include <QWaitCondition>
#include <QFileInfo>
#include <QDir>

#include "../icopyengine.h"

#define CTRL_EXT ".gfc"

class CopyEngineAbstract : public ICopyEngine
{
public:

    CopyEngineAbstract(QString source, QString dest, QString basePath, bool paused = false)
        : m_source(source),
          m_dest(dest),
          m_basePath(basePath)
    {
        m_pause = paused;
        m_cancel = false;

        if (m_pause) pause();
    }

    virtual ~CopyEngineAbstract() {}

    virtual void resume()
    {
        m_sync.lock();
        m_pause = false;
        m_sync.unlock();
        m_pauseCond.wakeAll();
    }

    virtual void pause()
    {
        m_sync.lock();
        m_pause = true;
        m_sync.unlock();
    }

    virtual void cancel()
    {
        m_sync.lock();
        m_cancel = true;
        m_sync.unlock();
        m_pauseCond.wakeAll();
    }

protected:

    /**
     * Recreate the directory structure
     *
     * @brief mkDestDirPath
     * @return
     */
    bool mkDestDirPath()
    {
        QFileInfo sourceInfo(m_source);
        QString sourceDir = sourceInfo.absolutePath();
        QString newDir = sourceDir.right(sourceDir.size() - m_basePath.size());

        m_dest = m_dest + newDir;
        QDir dir(m_dest);

        bool res = false;
        if (!dir.exists())
            res = dir.mkpath(m_dest);

        return res;
    }

    /**
     * Get or create the control file
     *
     * @brief getCtrlFile
     * @param pos
     * @return
     */
    bool getCtrlFile(long long &pos)
    {
        QFileInfo fromFile(m_source);
        // On windows we need to set the hidden flag explicitelly
        m_ctrlFile.setFileName(m_dest + "/." + fromFile.baseName() + CTRL_EXT);
        bool res = m_ctrlFile.open(QIODevice::ReadWrite);

        if (res)
        {
            // read the current pos
            QByteArray data = m_ctrlFile.readAll();
            bool ok;
            pos = data.toLongLong(&ok);
            if (!ok) pos = 0;
        }

        return res;
    }

    /**
     * Remove the control file
     *
     * @brief removeCtrlFile
     * @return
     */
    bool removeCtrlFile() { return m_ctrlFile.remove(); }

    /**
     * Update the ctrl file with the current pos
     *
     * @brief updateCtrlFile
     * @param writtenBytes
     * @return
     */
    bool updateCtrlFile(long long writtenBytes)
    {
        if (!m_ctrlFile.isOpen() &&
                !m_ctrlFile.open(QIODevice::ReadWrite))
            return false;

        QByteArray bytes = QString::number(writtenBytes).toLatin1();
        m_ctrlFile.seek(0);
        if (!m_ctrlFile.write(bytes, bytes.size()) > 0)
            return false;

        m_ctrlFile.flush();
        return true;
    }

protected:
    QMutex          m_sync;
    QWaitCondition  m_pauseCond;
    bool            m_pause;
    bool            m_cancel;

    QString         m_source;
    QString         m_dest;
    QString         m_basePath;

    QFile           m_ctrlFile;
};

#endif // COPYENGINEABSTRACT_H
