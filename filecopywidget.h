#ifndef FILECOPYWIDGET_H
#define FILECOPYWIDGET_H

#include <QWidget>
#include <QThread>

class CopierWorker;

namespace Ui {
class FileCopyWidget;
}

class FileCopyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FileCopyWidget(const QString &source, const QString &dest, const QString &basePath, QWidget *parent = 0);
    ~FileCopyWidget();

    qint64 getTotal() { return m_total; }
    qint64 getWritten() { return m_written; }

    void finish();

public slots:
    void copy();
    void progress(qint64 bytes);
    void done();

signals:
    void fileDone(QWidget*);
    void updateStats(qint64);

private slots:
    void on_btnResume_clicked();
    void on_btnCancel_clicked();

private:
    Ui::FileCopyWidget *ui;

    QString m_source;
    QString m_dest;
    QString m_basePath;

    qint64  m_total;
    qint64  m_written;

    QThread         m_thread;
    CopierWorker*   m_copierWorker;
    bool            m_pause;
};

#endif // FILECOPYWIDGET_H
