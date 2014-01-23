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
    ui(new Ui::MainWindow),
    m_copying(false)
{
    ui->setupUi(this);

    // set a ZERO timer, which will only trigger
    // when there are no more events/signals in
    // the qt event queue.
    // this will prevent signals flood from constant progress updates...
    connect(&m_statsTimer, SIGNAL(timeout()), this, SLOT(updateGuiStats()));
    m_statsTimer.setInterval(0);

//    ui->statusBar->setSizeGripEnabled(false);
//    this->setFixedSize(this->size());

    initFileBrowser();
    reset();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fileDone(QWidget *widget)
{
    updateGuiStats();

    int i = ui->progressLayout->indexOf(widget);
    QLayoutItem *item = ui->progressLayout->takeAt(i);
    if (item)
    {
        delete item->widget();
        delete item;
    }

    if (ui->progressLayout->count() <= 0)
        reset();
}

void MainWindow::reset()
{
    m_statsTimer.stop();
    if (m_fileBrowser)
    {
        m_fileBrowser->setEnabled(true);
        m_fileBrowser->reset();
    }

    ui->progressBar->setMinimum(0);
    ui->progressBar->reset();
    ui->btnCopy->setEnabled(false);
    m_total = 0; m_written = 0;
    m_copying = false;

    updateGuiStats();
}

void MainWindow::progress(qint64 bytes)
{
    m_written += bytes;
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
    m_copying = true;
    m_fileBrowser->setEnabled(false);
    startCopy();
}

void MainWindow::startCopy()
{
    if (!m_copying)
        return;

    if (!m_statsTimer.isActive())
        m_statsTimer.start();

    emit copy();
}

void  MainWindow::initFileBrowser()
{
    m_fileBrowser = new FileBrowser(this);
    ui->mainVerticalLayout->insertWidget(0, m_fileBrowser);
    connect(m_fileBrowser, SIGNAL(updateTotals(qint64)), this, SLOT(updateTotals(qint64)));
    connect(m_fileBrowser, SIGNAL(readyToCopy(QStringList&, QString&, QString&)),
            this, SLOT(copyReady(QStringList&, QString&, QString&)));
}

void MainWindow::copyReady(QStringList &source, QString &dest, QString &basePath)
{
    ui->btnCopy->setEnabled(true);
    foreach (const QString &fileName, source)
    {
        FileCopyWidget *progressWidget = new FileCopyWidget(fileName, dest, basePath, this);
        connect(this, SIGNAL(copy()), progressWidget, SLOT(copy()));
        connect(progressWidget, SIGNAL(fileDone(QWidget*)), this, SLOT(fileDone(QWidget*)));
        connect(progressWidget, SIGNAL(updateStats(qint64)), this, SLOT(progress(qint64)));
        ui->progressLayout->addWidget(progressWidget);

        if  (QCoreApplication::hasPendingEvents())
        {
            QCoreApplication::processEvents(QEventLoop::AllEvents);
            QCoreApplication::flush();
        }

        startCopy();
        qDebug() << "Adding filecopywidget for file - " << fileName;
    }
}

void MainWindow::on_toolButton_clicked()
{
    ui->boxCopy->setVisible(!ui->boxCopy->isVisible());
}

void MainWindow::updateTotals(qint64 bytes)
{
    m_total = bytes;
    ui->progressBar->reset();
    ui->progressBar->setMaximum(m_total);

    updateGuiStats();
}

void MainWindow::updateGuiStats()
{
    qint64 written = m_written, total = m_total;
    QString stats;
    stats.sprintf(STATS_LABEL, written, total, "KBs");
    ui->lblMainStats->setText(stats);
    ui->progressBar->setValue(written);
}
