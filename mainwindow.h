#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QTimer>

class FileBrowser;

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

    void updateTotals(qint64 bytes);
    void copyReady(QStringList& source, QString &dest, QString &basePath);
    void fileDone(QWidget *widget);

private slots:
    void on_btnCopy_clicked();
    void on_toolButton_clicked();
    void updateGuiStats();

signals:
    void copy();

private:
    void initFileBrowser();
    void reset();
    void startCopy();

private:
    Ui::MainWindow *ui;

private:
    qint64      m_written;
    qint64      m_total;

    FileBrowser *m_fileBrowser;

    QTimer      m_statsTimer;

    bool        m_copying;
};

#endif // MAINWINDOW_H
