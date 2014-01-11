#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QStringList"
#include "QMessageBox"
#include "QDebug"

#include "copyfilewidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_widgetList(new QList<QWidget*>())
{
    ui->setupUi(this);

    // add the initial widget
    ui->fileSelectorsVLayout->addWidget(new CopyFileWidget(this));
    ui->progressBar->setMinimum(0);
    ui->progressBar->reset();
}

MainWindow::~MainWindow()
{
    delete m_widgetList;
    delete ui;
}

void MainWindow::enableCopy() {
//    if (QFile::exists(m_sSource) && QDir().exists(m_sDestination)) {
//        ui->pushButton->setEnabled(true);
//    }
}

void MainWindow::copyFile() {
//    if (QFile::exists(m_sSource)) {
//        QFileInfo fileInfo(m_sSource);
//        m_sDestination += "/" + fileInfo.fileName();
//        if (!QFile::copy(m_sSource, m_sDestination)) {
//            QMessageBox::information(this,
//                                     "Error", "An error ocured, the file was not copied!",
//                                     QMessageBox::Ok);
//        }
//    }

    return;
}

void MainWindow::on_btnCopy_clicked()
{

}

void MainWindow::on_btnAdd_clicked()
{
    QWidget* w = new CopyFileWidget(this);
    m_widgetList->append(w);
    ui->fileSelectorsVLayout->addWidget(w);
}

void MainWindow::on_btnRemove_clicked()
{
    int count = ui->fileSelectorsVLayout->count();

    qDebug() << "Amount of children found :" << count;

    if (count <= 1) {
        return;
    }

    QLayoutItem* w = ui->fileSelectorsVLayout->takeAt(0);
    ui->fileSelectorsVLayout->removeItem(w);
    delete w;
}
