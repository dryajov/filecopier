#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_btnCopy_clicked();

    void on_btnAdd_clicked();

    void on_btnRemove_clicked();

private:
    void copyFile();
    void enableCopy();

    void remove(QLayout* layout);

private:
    Ui::MainWindow *ui;

private:
    QList<QWidget*> *m_widgetList;
};

#endif // MAINWINDOW_H
