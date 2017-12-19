#include "transformwidget.h"
#include <QRadioButton>
#include <QLineEdit>
#include <QBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPropertyAnimation>
#include <QFileDialog>
#include <QButtonGroup>

TransformWidget::TransformWidget(QWidget *parent) : QWidget(parent)
{
    plabel_ = new QLabel(this);
    pwav_rbtn_ = new QRadioButton(this);
    psave_edit_ = new QLineEdit(this);
    pbrowse_btn_ = new QPushButton(this);
    pok_btn_ = new QPushButton(this);
    QVBoxLayout* playout = new QVBoxLayout(this);
    QHBoxLayout* puplayout = new QHBoxLayout(this);
    QHBoxLayout* pformatlayout = new QHBoxLayout(this);
    QHBoxLayout* psavelayout = new QHBoxLayout(this);
    QHBoxLayout* pdownlayout = new QHBoxLayout(this);
    QButtonGroup* pbtngroup = new QButtonGroup;

    setLayout(playout);
    playout->addLayout(puplayout);
    playout->addLayout(pformatlayout);
    playout->addLayout(psavelayout);
    playout->addLayout(pdownlayout);
    puplayout->addWidget(plabel_);
    puplayout->addSpacing(100);
    pformatlayout->addWidget(pwav_rbtn_);
    pformatlayout->addStretch();
    psavelayout->addWidget(psave_edit_);
    psavelayout->addWidget(pbrowse_btn_);
    pdownlayout->addStretch();
    pdownlayout->addWidget(pok_btn_);
    pbtngroup->addButton(pwav_rbtn_);

    playout->setContentsMargins(0,0,0,0);

    //label
    plabel_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    plabel_->setText(tr("Choose a format: "));
    plabel_->setStyleSheet("font: 75 20pt Adobe Gothic Std B;");

    //WAV
    pwav_rbtn_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    pwav_rbtn_->setStyleSheet("QRadioButton::indicator {width: 30px;height: 30px;}"
                             "QRadioButton::indicator::unchecked{image: url(:/rec/img/TransformWidget-WAV-uncheck.png)}"
                             "QRadioButton::indicator::checked{image: url(:/rec/img/TransformWidget-WAV-check.png)}");

    //edit
    psave_edit_->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    psave_edit_->setStyleSheet("QLineEdit{font: 75 10pt Adobe Gothic Std B;}");

    //browse
    pbrowse_btn_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    pbrowse_btn_->setText(tr("Browse"));
    pbrowse_btn_->setStyleSheet("QPushButton{font: 75 10pt Adobe Gothic Std B;}"
                               "QPushButton{background: white;border: 1px solid #1296db;width: 80px;height: 20px;}"
                               "QPushButton:hover{border: 2px solid #d81e06;}");
    connect(pbrowse_btn_,SIGNAL(clicked(bool)),this,SLOT(BrowsebtnClick_Slot()));

    //ok
    pok_btn_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    pok_btn_->setStyleSheet("QPushButton{border-image: url(:/rec/img/TransformWidget-OK.png);width: 30px;height: 22px}"
                           "QPushButton:hover{border-image: url(:/rec/img/TransformWidget-OK-hover.png)}");
}

void TransformWidget::Initialize()
{
    pwav_rbtn_->setChecked(true);
}

void TransformWidget::BrowsebtnClick_Slot()
{
    QString path = QFileDialog::getExistingDirectory(this,tr("Selete a path"),QDir::homePath());
    if(path.length()!=0){
        psave_edit_->setText(path);
    }
}

void TransformWidget::UpdateTranslation()
{
    plabel_->setText(tr("Choose a format: "));
    pbrowse_btn_->setText(tr("Browse"));
}
