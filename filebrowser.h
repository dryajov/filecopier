#ifndef FILEBROWSER_H
#define FILEBROWSER_H

#include <QWidget>
#include <QDir>

namespace Ui {
class FileBrowser;
}

class FileBrowser : public QWidget
{
    Q_OBJECT

public:
    explicit FileBrowser(QWidget *parent = 0);
    ~FileBrowser();
    void reset();

private slots:
    void on_btnSource_clicked();
    void on_btnDest_clicked();

    void setRecurse(bool recurse) { m_recurse = recurse; }
    void filesAdded(QStringList files);

private:
    QStringList scanDir(const QDir &dir);
    void emitCopyReady();

signals:
    void updateTotals(qint64);
    void readyToCopy(QStringList&, QString&, QString&);
    void addFiles(QStringList&);
    void setDestination(QString &dest, QString &basePath);

private:
    Ui::FileBrowser *ui;

    QStringList m_sourceFiles;
    QString     m_destDir;
    QString     m_basePath;
    bool        m_recurse;
};

#endif // FILEBROWSER_H
