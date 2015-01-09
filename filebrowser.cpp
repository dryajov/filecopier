#include "filebrowser.h"
#include "ui_filebrowser.h"

#include <QDebug>
#include <QFileDialog>
#include <QThreadPool>
#include <QMessageBox>
#include <QDirIterator>
#include <QFuture>
#include <QFutureWatcher>
//#include <QtConcurrent>

#include "copierworker.h"
#include "dirscanner.h"
#include "mainwindow.h"

#define SRC_TEXT tr("Source path")
#define DEST_TEXT tr("Destination path")

FileBrowser::FileBrowser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileBrowser),
    m_recurse(true)
{
    ui->setupUi(this);
    ui->boxOptions->setVisible(false);
    connect(ui->btnDirectory, SIGNAL(toggled(bool)), ui->boxOptions, SLOT(setVisible(bool)));

    ui->chkRecurse->setChecked(m_recurse);
    connect(ui->chkRecurse, SIGNAL(toggled(bool)), this, SLOT(setRecurse(bool)));
}

FileBrowser::~FileBrowser()
{
    delete ui;
}

void FileBrowser::reset()
{
    ui->txtDest->setText(DEST_TEXT);
    ui->txtSource->setText(SRC_TEXT);
    this->m_basePath = "";
    this->m_destDir = "";
    this->m_sourceFiles.clear();
}

void FileBrowser::on_btnSource_clicked()
{
    m_sourceFiles.clear();
    if (ui->btnDirectory->isChecked())
    {
        QString dir = QFileDialog::getExistingDirectory(this, "Select source directory", "~/");
        DirScanner *scanner = new DirScanner(dir);
        scanner->setAutoDelete(true);
        connect(scanner, SIGNAL(scannedFiles(QStringList)), this, SLOT(filesAdded(QStringList)));
        QThreadPool::globalInstance()->start(scanner);

        m_basePath = dir;
        ui->txtSource->setText(m_basePath);
    }
    else
    {
        m_sourceFiles = QFileDialog::getOpenFileNames(this, "Select source files", "~/", "*.*");
        if (!m_sourceFiles.isEmpty())
            ui->txtSource->setText(QFileInfo(m_sourceFiles.at(0))
                                   .absoluteDir().canonicalPath());
    }

    return;
}

void FileBrowser::emitCopyReady()
{
    if (!m_sourceFiles.isEmpty() && !m_destDir.isEmpty())
        emit readyToCopy(m_sourceFiles, m_destDir, m_basePath);
}

void FileBrowser::on_btnDest_clicked()
{
    m_destDir = QFileDialog::getExistingDirectory(this, "Select destination directory", "/");
    ui->txtDest->setText(m_destDir);
    emit setDestination(m_destDir, m_basePath);
}

void FileBrowser::filesAdded(QStringList files)
{

}
