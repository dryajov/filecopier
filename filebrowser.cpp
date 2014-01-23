#include "filebrowser.h"
#include "ui_filebrowser.h"

#include <QDebug>
#include <QFileDialog>
#include <QThreadPool>
#include <QMessageBox>
#include <QDirIterator>

#include "copierworker.h"

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

    m_total = 0;
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
    this->m_total = 0;
}

void FileBrowser::on_btnSource_clicked()
{
    m_sourceFiles.clear();
    if (ui->btnDirectory->isChecked())
    {
        QString dir = QFileDialog::getExistingDirectory(this, "Select source directory", "~/");
        QDir curDir(dir);
        scanDir(curDir);
        curDir.cdUp();

        m_basePath = curDir.absolutePath();
    }
    else
        m_sourceFiles = QFileDialog::getOpenFileNames(this, "Select source files", "~/", "*.*");

    if (!m_sourceFiles.isEmpty())
    {       
        if (m_basePath.isEmpty())
        {
            ui->txtSource->setText(QFileInfo(m_sourceFiles.at(0))
                                   .absoluteDir().canonicalPath());
        }
        else
        {
            ui->txtSource->setText(m_basePath);
        }

        foreach (const QString &fileName, m_sourceFiles)
        {
            qDebug() << "File name: " << fileName;
            m_total += QFileInfo(fileName).size();
        }

        emit updateTotals(m_total);
        emitCopyReady();
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
    emitCopyReady();
}

void FileBrowser::scanDir(const QDir &dir)
{
   qDebug() << "Dir is: " << dir.dirName();
   QDirIterator iterator(dir.absolutePath(),                         
                         QDirIterator::Subdirectories);

   if  (QCoreApplication::hasPendingEvents())
   {
       QCoreApplication::processEvents(QEventLoop::AllEvents);
       QCoreApplication::flush();
   }

   while (iterator.hasNext()) {
      iterator.next();

      if (iterator.fileName() == "." || iterator.fileName() == "..")
          continue;

      if (!iterator.fileInfo().isDir()) {
         m_sourceFiles.append(iterator.filePath());
      }
      else
      {
        scanDir(iterator.fileName());
      }
   }
}


