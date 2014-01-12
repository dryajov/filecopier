#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
public slots:
    void progress(qint64 bytes);
    void onError(QString file);

private slots:
    void on_btnSource_clicked();

    void on_btnDest_clicked();

    void on_btnCopy_clicked();

private:
    Ui::MainWindow *ui;

private:
    QStringList m_sourceFiles;
    QString     m_destDir;
    qint64      m_written;
    qint64      m_total;

    QVector<QThread*> m_threads;
};

#endif // MAINWINDOW_H
