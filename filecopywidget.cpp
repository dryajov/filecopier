#include "filecopywidget.h"
#include "ui_filecopywidget.h"

#include <QStringList>
#include <QThreadPool>
#include <QFile>
#include <QFileInfo>

#include "copierworker.h"

#define STATS_LABEL "Copied %lld of %lld %s"

FileCopyWidget::FileCopyWidget(const QString &source, const QString &dest, const QString &basePath, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileCopyWidget),
    m_source(source),
    m_dest(dest),
    m_basePath(basePath),
    m_pause(false)
{
    ui->setupUi(this);

    ui->progressBar->setMinimum(0);
    ui->progressBar->reset();

    m_written = 0;

    QFileInfo fileInfo(m_source);
    m_total = fileInfo.size();

    ui->groupBox->setTitle(QString("Copying file " + fileInfo.fileName()));

    // set a ZERO timer, which will only trigger
    // when there are no more events/signals in
    // the qt event queue.
    // this will prevent signals flood from constant progress updates...
    connect(&m_statsTimer, SIGNAL(timeout()), this, SLOT(updateGuiStats()));
    m_statsTimer.setInterval(0);

    ui->progressBar->setMaximum(m_total);
    updateGuiStats();
}

FileCopyWidget::~FileCopyWidget()
{
    delete ui;
}

void FileCopyWidget::copy()
{
    if (QFile::exists(m_source))
    {
        m_copierWorker = new CopierWorker(m_source, m_dest, m_basePath);
        m_copierWorker->moveToThread(&m_thread);
        connect(&m_thread, SIGNAL(started()), m_copierWorker, SLOT(run()));
        connect(m_copierWorker, SIGNAL(bytesWritten(qint64)), this, SLOT(progress(qint64)));
        connect(m_copierWorker, SIGNAL(done()), this, SLOT(done()));
        m_thread.start(); // start the thread
        m_statsTimer.start(); // start progress ui update timer
    }
}

void FileCopyWidget::progress(qint64 bytes)
{
    m_written += bytes;
    if (bytes > 0) emit updateStats(bytes);
}

void FileCopyWidget::on_btnResume_clicked()
{
    if (!m_copierWorker)
        return;

    if (!m_pause)
    {
        m_copierWorker->pause();
        ui->btnResume->setText(tr("Resume"));
        m_pause = true;
    }
    else
    {
        m_copierWorker->resume();
        ui->btnResume->setText(tr("Pause"));
        m_pause = false;
    }
}

void FileCopyWidget::done()
{
    m_statsTimer.stop();
    updateGuiStats(); // update gui one last time
    finish();
}

void FileCopyWidget::finish()
{
    m_thread.exit();
    m_thread.wait();
    m_copierWorker->deleteLater();
    m_copierWorker = 0;

    emit fileDone(this);
}

void FileCopyWidget::on_btnCancel_clicked()
{
    m_copierWorker->cancel();
    finish();
}

void FileCopyWidget::updateGuiStats()
{
    if (m_written)
    {
        ui->progressBar->setValue( m_written);
        QString stats;
        stats.sprintf(STATS_LABEL, m_written, m_total, "KBs");
        ui->lblStats->setText(stats);
    }
}
