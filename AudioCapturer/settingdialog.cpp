#include "settingdialog.h"
#include "ui_settingdialog.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QBoxLayout>
#include "ncframelesshelper.h"
#include <QTranslator>
#include <QFileDialog>
#include "dllfunction.h"

#include <QDebug>

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);

    pdiv_label_ = new QLabel(this);
    plangage_label_ = new QLabel(this);
    pdiv_edit_ = new QLineEdit(this);
    pbrowse_btn_ = new QPushButton(this);
    pchinese_rbtn_ = new QRadioButton(this);
    penglish_rbtn_ = new QRadioButton(this);
    pcancel_btn_ = new QPushButton(this);
    pok_btn_ = new QPushButton(this);
    QVBoxLayout* playout = new QVBoxLayout(this);
    QHBoxLayout* pdiv_layout = new QHBoxLayout(this);
    QHBoxLayout* plangage_layout = new QHBoxLayout(this);
    QHBoxLayout* pbtns_layout = new QHBoxLayout(this);

    //this dialog
    NcFramelessHelper* pnc = new NcFramelessHelper;
    pnc->activateOn(this);
    resize(350,0);
    setStyleSheet("QWidget#SettingDialog{border: 1px solid #1296db;}"
                  "QWidget#SettingDialog{background-color:white}");
    pnc->setWidgetResizable(false);

    //layout
    setLayout(playout);
    playout->addWidget(pdiv_label_);
    playout->addLayout(pdiv_layout);
    playout->addWidget(plangage_label_);
    playout->addLayout(plangage_layout);
    playout->addStretch();
    playout->addLayout(pbtns_layout);
    pdiv_layout->addWidget(pdiv_edit_);
    pdiv_layout->addWidget(pbrowse_btn_);
    plangage_layout->addWidget(pchinese_rbtn_,0,Qt::AlignLeft);
    plangage_layout->addWidget(penglish_rbtn_,0,Qt::AlignLeft);
    pbtns_layout->addWidget(pcancel_btn_,0,Qt::AlignLeft);
    pbtns_layout->addWidget(pok_btn_,0,Qt::AlignRight);

    //margin
    playout->setContentsMargins(5,5,5,5);

    //div label
    pdiv_label_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    pdiv_label_->setText(tr("Set default folder: "));
    pdiv_label_->setStyleSheet("font: 75 15pt Adobe Gothic Std B;");

    //div edit
    pdiv_edit_->setStyleSheet("QLineEdit{font: 75 10pt Adobe Gothic Std B;}");

    //browse
    pbrowse_btn_->setText(tr("Browse"));
    pbrowse_btn_->setStyleSheet("QPushButton{font: 75 10pt Adobe Gothic Std B;}"
                               "QPushButton{background: white;border: 1px solid #1296db;width: 80px;height: 20px;}"
                               "QPushButton:hover{border: 2px solid #d81e06;}");
    connect(pbrowse_btn_,SIGNAL(clicked(bool)),this,SLOT(Browse_Slot()));

    //langage label
    plangage_label_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    plangage_label_->setText(tr("Set langage: "));
    plangage_label_->setStyleSheet("font: 75 15pt Adobe Gothic Std B;");

    //langage radiobutton
    QButtonGroup* plangage_btng = new QButtonGroup(this);
    plangage_btng->addButton(pchinese_rbtn_);
    plangage_btng->addButton(penglish_rbtn_);
    pchinese_rbtn_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    penglish_rbtn_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    pchinese_rbtn_->setText(QString::fromLocal8Bit("简体中文"));
    penglish_rbtn_->setText("English");
    pchinese_rbtn_->setStyleSheet("QRadioButton{font: 75 10pt Adobe Gothic Std B;}"
                                 "QRadioButton::indicator{width: 20px;height: 20px}"
                                 "QRadioButton::indicator:checked{image: url(:/rec/img/SettingDialog-Check.png)}"
                                 "QRadioButton::indicator:unchecked{image: url(:/rec/img/SettingDialog-Uncheck.png)}");
    penglish_rbtn_->setStyleSheet("QRadioButton{font: 75 10pt Adobe Gothic Std B;}"
                                 "QRadioButton::indicator{width: 20px;height: 20px}"
                                 "QRadioButton::indicator:checked{image: url(:/rec/img/SettingDialog-Check.png)}"
                                 "QRadioButton::indicator:unchecked{image: url(:/rec/img/SettingDialog-Uncheck.png)}");
    //cancel
    pcancel_btn_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    pcancel_btn_->setStyleSheet("QPushButton{border-image: url(:/rec/img/SettingDialog-Cancel.png);width: 20px;height: 20px}"
                               "QPushButton:hover{border-image:url(:/rec/img/SettingDialog-Cancel-hover.png)}");
    connect(pcancel_btn_,SIGNAL(clicked(bool)),this,SLOT(Cancal_Slot()));

    //ok
    pok_btn_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    pok_btn_->setStyleSheet("QPushButton{border-image: url(:/rec/img/SettingDialog-OK.png);width: 25px;height: 25px}"
                           "QPushButton:hover{border-image:url(:/rec/img/SettingDialog-OK-hover.png)}");
    connect(pok_btn_,SIGNAL(clicked(bool)),this,SLOT(Save_Slot()));

    Initialize();
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::UpdateTranslation()
{
    pdiv_label_->setText(tr("Set default folder: "));
    pbrowse_btn_->setText(tr("Browse"));
    plangage_label_->setText(tr("Set langage: "));
}

void SettingDialog::Browse_Slot()
{
    QString path = QFileDialog::getExistingDirectory(this,tr("Select a path"),QDir::homePath());
    if(path.length()!=0){
        pdiv_edit_->setText(path);
    }
}

void SettingDialog::Cancal_Slot()
{
    this->close();
}

void SettingDialog::Initialize()
{
    chinese_status_ = false;
    english_status_ = true;
    pchinese_rbtn_->setChecked(chinese_status_);
    penglish_rbtn_->setChecked(english_status_);
    QTranslator* ptranslator = new QTranslator;
    ptranslator->load(":/rec/AudioCapturer_en_US.qm");
    qApp->installTranslator(ptranslator);
}

void SettingDialog::Reset()
{
    qDebug() << "Get current save path" ;
    pdiv_edit_->setText(ChangeCharpToQString(GetDefaultFolder()));
    pchinese_rbtn_->setChecked(chinese_status_);
    penglish_rbtn_->setChecked(english_status_);
}

void SettingDialog::Save_Slot()
{
    QTranslator* ptranslator = new QTranslator;
    if(pchinese_rbtn_->isChecked()){
        ptranslator->load(":/rec/AudioCapturer_zh_CN.qm");
        qApp->installTranslator(ptranslator);
        emit Translate();
    }
    if(penglish_rbtn_->isChecked()){
        ptranslator->load(":/rec/AudioCapturer_en_US.qm");
        qApp->installTranslator(ptranslator);
        emit Translate();
    }
    qDebug() << "Save path" ;
    SetDefaultFolder(pdiv_edit_->text().toStdString().c_str());
    chinese_status_ = pchinese_rbtn_->isChecked();
    english_status_ = penglish_rbtn_->isChecked();
    close();
}
