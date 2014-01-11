#ifndef COPYFILEWIDGET_H
#define COPYFILEWIDGET_H

#include <QWidget>

namespace Ui {
class CopyFileWidget;
}

class CopyFileWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CopyFileWidget(QWidget *parent = 0);
    ~CopyFileWidget();

public:
    QString& getSource() { return m_sSource; }
    QString& getDesitnation() { return m_sDestination; }

private slots:
    void on_btnBrowseSource_clicked();

    void on_btnBrowseDest_clicked();

private:
    Ui::CopyFileWidget *ui;

private:
    QString m_sSource;
    QString m_sDestination;
};

#endif // COPYFILEWIDGET_H
