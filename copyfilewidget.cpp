#include "copyfilewidget.h"
#include "ui_copyfilewidget.h"

#include "QFileDialog"

CopyFileWidget::CopyFileWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CopyFileWidget)
{
    ui->setupUi(this);
}

CopyFileWidget::~CopyFileWidget()
{
    delete ui;
}

void CopyFileWidget::on_btnBrowseSource_clicked()
{
    m_sSource = QFileDialog::getOpenFileName(this, tr("Open File"),"~",tr("All Files (*.*)"));
    ui->lineEdit->setText(m_sSource);
}

void CopyFileWidget::on_btnBrowseDest_clicked()
{
    m_sDestination = QFileDialog::getExistingDirectory(this, tr("Open File"),"~");
    ui->lineEdit_2->setText(m_sDestination);
}
