#ifndef COPYCOORDINATOR_H
#define COPYCOORDINATOR_H

#include <QObject>
#include <QStringList>
#include <QMutex>

class CopyCoordinator : public QObject
{
    Q_OBJECT
public:
    explicit CopyCoordinator(QObject *parent = 0);

    inline void addFiles(QStringList &files)
    {
        QMutexLocker locker(&m_mutex);
        m_files = m_files + files;
    }

    QStringList getFiles()
    {
        QMutexLocker locker(&m_mutex);
        return m_files;
    }

    void resetFiles()
    {
        QMutexLocker locker(&m_mutex);
        m_files.clear();
    }

private:
    QStringList m_files;
    QMutex      m_mutex;
};

#endif // COPYCOORDINATOR_H
