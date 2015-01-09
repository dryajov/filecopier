#include "dirscanner.h"

#include <QDirIterator>
#include <QDebug>
#include <QCoreApplication>

DirScanner::DirScanner(QString dir, QObject *parent)
    : QObject(parent)
    ,  m_dir(dir)
{
}

DirScanner::~DirScanner()
{
    if (!m_files.isEmpty())
    {
        emit scannedFiles(m_files);
        m_files.clear();
    }
}

void DirScanner::run()
{
    scanDir(QDir(m_dir));
}

void DirScanner::scanDir(const QDir &dir)
{
    qDebug() << "Dir is: " << dir.absolutePath();
    QDirIterator iterator(dir.absolutePath(),
    QDirIterator::Subdirectories);

    QStringList files;
    while (iterator.hasNext())
    {
        // TODO: detect symlincs instead
        // of just ignoring them
        iterator.next();
        if (iterator.fileName() == "." ||
            iterator.fileName() == ".." ||
            iterator.fileInfo().isSymLink())
        continue;

        if (iterator.fileInfo().isDir())
        {
            scanDir(iterator.filePath());
        }
        else
        {
            files.append(iterator.filePath());
        }
    }

    m_files = m_files + files;
    if (m_files.size() >= 100)
    {
        emit scannedFiles(m_files);
        m_files.clear();
    }
}
