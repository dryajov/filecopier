#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStringList>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QThreadPool>

#include "copierworker.h"

#define SRC_TEXT "Source path"
#define DEST_TEXT "Destination path"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->progressBar->setMinimum(0);
    ui->progressBar->reset();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::progress(qint64 bytes)
{
    m_written += bytes;
    ui->progressBar->setValue( m_written / 1024 );

    if (m_total == m_written)
    {
        m_written = m_total = 0;
        ui->txtDest->setText(DEST_TEXT);
        ui->txtSource->setText(SRC_TEXT);
        ui->btnCopy->setEnabled(false);
    }

    return;
}

void MainWindow::onError(QString file)
{
    QString errorMsg;
    errorMsg = errorMsg.arg("An error ocured, the file %1 was not copied!", file);
    QMessageBox::information(this,
                            "Error", errorMsg,
                            QMessageBox::Ok);
}

void MainWindow::on_btnSource_clicked()
{
    m_sourceFiles = QFileDialog::getOpenFileNames(this, "Select source files", "~", "*.*");
    if (!m_sourceFiles.isEmpty())
    {
        QFileInfo fileInfo(m_sourceFiles.at(0));
        ui->txtSource->setText(fileInfo.absoluteDir().absolutePath());

        foreach (const QString &fileName, m_sourceFiles)
        {
             m_total += QFileInfo(fileName).size();
        }
        ui->progressBar->setMaximum(m_total / 1024);
    }

    if (!m_sourceFiles.isEmpty() && !m_destDir.isEmpty())
        ui->btnCopy->setEnabled(true);

    return;
}

void MainWindow::on_btnDest_clicked()
{
    m_destDir = QFileDialog::getExistingDirectory(this, "Select destination directory", "/");
    ui->txtDest->setText(m_destDir);

    if (!m_sourceFiles.isEmpty() && !m_destDir.isEmpty())
        ui->btnCopy->setEnabled(true);
}

void MainWindow::on_btnCopy_clicked()
{   foreach (const QString &fileName, m_sourceFiles)
    {
        if (QFile::exists(fileName))
        {
            QFileInfo fileInfo(fileName);
            QString destFile(m_destDir + "/" + fileInfo.fileName());

            CopierWorker *copierWorker = new CopierWorker(fileName, destFile);
            connect(copierWorker, SIGNAL(bytesWritten(qint64)), this, SLOT(progress(qint64)));
            connect(copierWorker, SIGNAL(error(QString)), this, SLOT(onError(QString)));

            QThreadPool::globalInstance()->start(copierWorker);
        }
    }

    m_sourceFiles.clear(); // remove everything from the list..

    return;
}
