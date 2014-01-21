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

    qint64 getTotal() { return m_total; }

private slots:
    void on_btnSource_clicked();
    void on_btnDest_clicked();

    void setRecurse(bool recurse) { m_recurse = recurse; }

private:
    void scanDir(const QDir &dir);
    void emitCopyReady();

signals:
    void updateTotals(qint64);
    void readyToCopy(QStringList&, QString&, QString&);

private:
    Ui::FileBrowser *ui;

    QStringList m_sourceFiles;
    QString     m_destDir;

    qint64      m_total;
    bool        m_recurse;
    QString     m_basePath;
};

#endif // FILEBROWSER_H
