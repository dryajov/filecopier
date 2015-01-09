#ifndef DIRSCANNER_H
#define DIRSCANNER_H

#include <QObject>
#include <QRunnable>
#include <QDir>
#include <QTimer>
#include <QStringList>

class DirScanner :  public QObject, public QRunnable
{
    Q_OBJECT

public:
    DirScanner(QString dir, QObject *parent = 0);
    virtual ~DirScanner();

    virtual void run();

signals:
    void scannedFiles(QStringList);

private:
    void scanDir(const QDir &dir);

private:
    QString     m_dir;
    QStringList m_files;
    QTimer      m_timer;
};

#endif // DIRSCANNER_H
