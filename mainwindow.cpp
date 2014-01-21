#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStringList>
#include <QMessageBox>
#include <QDebug>

#include "filebrowser.h"
#include "filecopywidget.h"

#define STATS_LABEL "Copied %lld of %lld %s"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->statusBar->setSizeGripEnabled(false);
    this->setFixedSize(this->size());

    ui->progressBar->setMinimum(0);
    ui->progressBar->reset();

    m_total = 0; m_written = 0;

    addFileBrowser();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fileDone(QWidget *widget)
{
    int i = ui->progressLayout->indexOf(widget);
    QLayoutItem *item = ui->progressLayout->takeAt(i);
    if (item)
    {
        delete item->widget();
        delete item;
    }
}

void MainWindow::progress(qint64 bytes)
{
    m_written += bytes;
    ui->progressBar->setValue( m_written / 1024 );

    return;
}

void MainWindow::onError(QString file)
{
    QString errorMsg = QString("An error ocured, the file %1 was not copied!").arg(file);
    QMessageBox::information(this,
                            "Error", errorMsg,
                            QMessageBox::Ok);
}

void MainWindow::on_btnCopy_clicked()
{
    emit copy();
    return;
}

void  MainWindow::addFileBrowser()
{
    m_fileBrowser = new FileBrowser(this);
    ui->mainVerticalLayout->insertWidget(0, m_fileBrowser);
    connect(m_fileBrowser, SIGNAL(updateTotals(qint64)), this, SLOT(updateTotals(qint64)));
    connect(m_fileBrowser, SIGNAL(readyToCopy(QStringList&, QString&, QString&)),
            this, SLOT(copyReady(QStringList&, QString&, QString&)));
}

void MainWindow::copyReady(QStringList &source, QString &dest, QString &basePath)
{
    foreach (const QString &fileName, source)
    {
        FileCopyWidget *progressWidget = new FileCopyWidget(fileName, dest, basePath, this);
        connect(this, SIGNAL(copy()), progressWidget, SLOT(copy()));
        connect(progressWidget, SIGNAL(fileDone(QWidget*)), this, SLOT(fileDone(QWidget*)));
        connect(progressWidget, SIGNAL(updateStats(qint64)), this, SLOT(progress(qint64)));
        ui->progressLayout->addWidget(progressWidget);
    }

    ui->btnCopy->setEnabled(true);
}

void MainWindow::on_toolButton_clicked()
{
    ui->boxCopy->setVisible(!ui->boxCopy->isVisible());
}

void MainWindow::updateTotals(qint64 bytes)
{
    m_total += bytes;
    QString stats;
    stats.sprintf(STATS_LABEL, (m_written / 1024), (m_total / 1024), "KBs");
    ui->lblMainStats->setText(stats);

    ui->progressBar->setMaximum( m_total / 1024 );
}
