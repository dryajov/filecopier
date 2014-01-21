#include "filecopywidget.h"
#include "ui_filecopywidget.h"

#include <QStringList>
#include <QThreadPool>
#include <QFile>
#include <QFileInfo>

#include "copierworker.h"

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

    m_written = 0; m_total = 0;
}

FileCopyWidget::~FileCopyWidget()
{
    delete ui;
}

void FileCopyWidget::copy()
{
    if (QFile::exists(m_source))
    {
        QFileInfo fileInfo(m_source);
        m_total = fileInfo.size();
        ui->progressBar->setMaximum(m_total / 1024);

        m_copierWorker = new CopierWorker(m_source, m_dest, m_basePath);
        m_copierWorker->moveToThread(&m_thread);
        connect(&m_thread, SIGNAL(started()), m_copierWorker, SLOT(run()));
        connect(m_copierWorker, SIGNAL(bytesWritten(qint64)), this, SLOT(progress(qint64)));
        connect(m_copierWorker, SIGNAL(done()), this, SLOT(done()));
        m_thread.start(); // start the thread
    }
}

void FileCopyWidget::progress(qint64 bytes)
{
    m_written += bytes;
    ui->progressBar->setValue( m_written / 1024 );
    emit updateStats(m_written);

    return;
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
    finish();
}

void FileCopyWidget::finish()
{
    m_thread.exit();
    delete m_copierWorker;
    m_copierWorker = 0;

    emit fileDone(this);
}

void FileCopyWidget::on_btnCancel_clicked()
{
    m_copierWorker->cancel();
    finish();
}
