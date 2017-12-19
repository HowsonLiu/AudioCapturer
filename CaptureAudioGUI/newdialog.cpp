#include "newdialog.h"
#include "ui_newdialog.h"
#include "timewidget.h"
#include "formatwidget.h"
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QRadioButton>
#include <QPushButton>
#include <QBoxLayout>
#include "ncframelesshelper.h"
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include "capturedialog.h"
#include <QMessageBox>

#include <QDebug>

NewDialog::NewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewDialog)
{
    ui->setupUi(this);

    pname_label_ = new QLabel(this);
    pname_edit_ = new QLineEdit(this);
    psettime_check_ = new QCheckBox(this);
    psetformat_check_ = new QCheckBox(this);
    pmicro_rbtn_ = new QRadioButton(this);
    ploopback_rbtn_ = new QRadioButton(this);
    ptime_widget_ = new TimeWidget(this);
    pformat_widget_ = new FormatWidget(this);
    pback_btn_ = new QPushButton(this);
    pstart_btn_ = new QPushButton(this);
    ptime_animation_ = new QPropertyAnimation(ptime_widget_);
    pformat_animation_ = new QPropertyAnimation(pformat_widget_);
    panimation_with_time_ = new QPropertyAnimation(this);
    panimation_with_format_ = new QPropertyAnimation(this);
    ptime_animation_group_ = new QParallelAnimationGroup;
    pformat_animation_group_ = new QParallelAnimationGroup;
    pcapture_dialog_ = nullptr;
    QVBoxLayout* playout = new QVBoxLayout(this);
    QHBoxLayout* puplayout = new QHBoxLayout(this);
    QHBoxLayout* pmiddlelayout = new QHBoxLayout(this);
    QHBoxLayout* pdownlayout = new QHBoxLayout(this);
    QVBoxLayout* pmleftlayout = new QVBoxLayout(this);
    QHBoxLayout* pmrightlayout = new QHBoxLayout(this);
    QButtonGroup* prbtns_group = new QButtonGroup;

    //set layout
    playout->addLayout(puplayout);
    playout->addLayout(pmiddlelayout);
    playout->addWidget(ptime_widget_);
    playout->addWidget(pformat_widget_);
    playout->addLayout(pdownlayout);
    puplayout->addWidget(pname_label_);
    puplayout->addWidget(pname_edit_);
    pmiddlelayout->addLayout(pmleftlayout);
    pmiddlelayout->addLayout(pmrightlayout);
    pmleftlayout->addWidget(psettime_check_,0,Qt::AlignVCenter);
    pmleftlayout->addWidget(psetformat_check_,0,Qt::AlignVCenter);
    pmrightlayout->addWidget(pmicro_rbtn_,0,Qt::AlignCenter);
    pmrightlayout->addWidget(ploopback_rbtn_,0,Qt::AlignCenter);
    pdownlayout->addWidget(pback_btn_);
    pdownlayout->addStretch();
    pdownlayout->addWidget(pstart_btn_);

    //set animation
    ptime_animation_->setTargetObject(ptime_widget_);
    pformat_animation_->setTargetObject(pformat_widget_);
    panimation_with_time_->setTargetObject(this);
    panimation_with_format_->setTargetObject(this);
    ptime_animation_group_->addAnimation(ptime_animation_);
    ptime_animation_group_->addAnimation(panimation_with_time_);
    pformat_animation_group_->addAnimation(pformat_animation_);
    pformat_animation_group_->addAnimation(panimation_with_format_);

    //this dialog
    NcFramelessHelper* nc = new NcFramelessHelper;
    nc->activateOn(this);
    nc->setWidgetResizable(false);
    setStyleSheet("QWidget#NewDialog{border: 1px solid #1296db;}"
                  "QWidget#NewDialog{background-color:white}");

    //margin
    pdownlayout->setContentsMargins(0,0,0,0);

    //name
    pname_label_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    pname_edit_->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    pname_label_->setMinimumSize(65,25);
    pname_label_->setMaximumSize(65,25);
    pname_edit_->setMaximumHeight(25);
    pname_edit_->setMinimumHeight(25);
    pname_label_->setText(tr("File Name:"));
    pname_label_->setStyleSheet("font: 75 10pt Adobe Gothic Std B;");
    pname_edit_->setStyleSheet("font: 75 10pt Adobe Gothic Std B;");
    pname_edit_->setMaxLength(15);


    //radiobtns
    prbtns_group->addButton(pmicro_rbtn_);
    prbtns_group->addButton(ploopback_rbtn_);
    pmicro_rbtn_->setStyleSheet("QRadioButton::indicator::checked {image: url(:/rec/img/NewDialog-Micro-check.png);}"
                               "QRadioButton::indicator::unchecked {image: url(:/rec/img/NewDialog-Micro-uncheck.png);}"
                               "QRadioButton::indicator {width: 30px;height: 30px;}");
    ploopback_rbtn_->setStyleSheet("QRadioButton::indicator::checked {image: url(:/rec/img/NewDialog-Computer-check.png);}"
                                  "QRadioButton::indicator::unchecked {image: url(:/rec/img/NewDialog-Computer-uncheck.png);}"
                                  "QRadioButton::indicator {width: 30px;height: 30px;}");

    //checkboxs
    psettime_check_->setText(tr("Set times"));
    psetformat_check_->setText(tr("Set Audio Format"));
    psettime_check_->setStyleSheet("QCheckBox{font: 75 10pt Adobe Gothic Std B;}"
                                  "QCheckBox::indicator {width: 25px;height: 25px;}"
                                  "QCheckBox::indicator:unchecked {image: url(:/rec/img/NewDialog-Checkbox-off.png);}"
                                  "QCheckBox::indicator:checked {image: url(:/rec/img/NewDialog-Checkbox-on.png);}");
    psetformat_check_->setStyleSheet("QCheckBox{font: 75 10pt Adobe Gothic Std B;}"
                                    "QCheckBox::indicator {width: 25px;height: 25px;}"
                                    "QCheckBox::indicator:unchecked {image: url(:/rec/img/NewDialog-Checkbox-off.png);}"
                                    "QCheckBox::indicator:checked {image: url(:/rec/img/NewDialog-Checkbox-on.png);}");
    connect(psettime_check_,SIGNAL(toggled(bool)),this,SLOT(SetTimeWidgetEnable_Slot(bool)));
    connect(psetformat_check_,SIGNAL(toggled(bool)),this,SLOT(SetFormatWidgetEnable_Slot(bool)));

    //back
    pback_btn_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    pback_btn_->setMinimumSize(40,40);
    pback_btn_->setMaximumSize(40,40);
    pback_btn_->setStyleSheet("QPushButton:hover{border-image: url(:/rec/img/NewDialog-Back-hover.png)}"
                             "QPushButton{border-image: url(:/rec/img/NewDialog-Back.png)}");
    connect(pback_btn_,&QPushButton::clicked,[this](){close();});

    //start
    pstart_btn_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    pstart_btn_->setMinimumSize(40,40);
    pstart_btn_->setMaximumSize(40,40);
    pstart_btn_->setStyleSheet("QPushButton:hover{border-image: url(:/rec/img/NewDialog-Start-hover.png)}"
                             "QPushButton{border-image: url(:/rec/img/NewDialog-Start.png)}");
    connect(pstart_btn_,SIGNAL(clicked(bool)),this,SLOT(Start_Slot()));

    //initialize
    Initialize();
}

NewDialog::~NewDialog()
{
    delete ui;
}

void NewDialog::SetTimeWidgetEnable_Slot(bool b)
{
    if(ptime_animation_group_->Running){
        ptime_animation_group_->stop();
    }
    int animationduration = 200;
    if(b){
        ptime_widget_->setMinimumHeight(0);
        ptime_widget_->setMaximumHeight(300);
        ptime_animation_->setDuration(animationduration);
        ptime_animation_->setPropertyName("maximumHeight");
        ptime_animation_->setStartValue(ptime_widget_->height());
        ptime_animation_->setEndValue(100);

        panimation_with_time_->setDuration(animationduration);
        panimation_with_time_->setPropertyName("size");
        panimation_with_time_->setStartValue(size());
        panimation_with_time_->setEndValue(QSize(size().width(),0));

        ptime_animation_group_->start();

    }else{
        ptime_widget_->setMinimumHeight(0);
        ptime_widget_->setMaximumHeight(300);
        ptime_animation_->setDuration(animationduration);
        ptime_animation_->setPropertyName("maximumHeight");
        ptime_animation_->setStartValue(ptime_widget_->height());
        ptime_animation_->setEndValue(0);

        panimation_with_time_->setDuration(animationduration);
        panimation_with_time_->setPropertyName("size");
        panimation_with_time_->setStartValue(size());
        panimation_with_time_->setEndValue(QSize(size().width(),0));

        ptime_animation_group_->start();
    }
}

void NewDialog::SetFormatWidgetEnable_Slot(bool b)
{
    if(pformat_animation_group_->Running){
        pformat_animation_group_->stop();
    }
    int animationduration = 200;
    if(b){
        pformat_widget_->setMinimumHeight(0);
        pformat_widget_->setMaximumHeight(pformat_widget_->sizeHint().height());
        pformat_animation_->setDuration(animationduration);
        pformat_animation_->setPropertyName("maximumHeight");
        pformat_animation_->setStartValue(pformat_widget_->height());
        pformat_animation_->setEndValue(pformat_widget_->sizeHint().height());

        panimation_with_format_->setDuration(animationduration);
        panimation_with_format_->setPropertyName("size");
        panimation_with_format_->setStartValue(size());
        panimation_with_format_->setEndValue(QSize(size().width(),0));

        pformat_animation_group_->start();

    }else{
        pformat_widget_->setMinimumHeight(0);
        pformat_widget_->setMaximumHeight(pformat_widget_->sizeHint().height());
        pformat_animation_->setDuration(animationduration);
        pformat_animation_->setPropertyName("maximumHeight");
        pformat_animation_->setStartValue(pformat_widget_->height());
        pformat_animation_->setEndValue(0);

        panimation_with_format_->setDuration(animationduration);
        panimation_with_format_->setPropertyName("size");
        panimation_with_format_->setStartValue(size());
        panimation_with_format_->setEndValue(QSize(size().width(),0));

        pformat_animation_group_->start();
    }
}

void NewDialog::Start_Slot()
{
    if(pname_edit_->text().isEmpty()){
        QMessageBox mes(QMessageBox::Critical,tr("Error"),tr("Name can not be empty"),QMessageBox::Ok, nullptr, Qt::FramelessWindowHint);
        mes.setStyleSheet("QMessageBox{border: 1px solid #1296db;}"
                          "QMessageBox{background-color:white}");
        mes.exec();
        return;
    }

    if(psettime_check_->isChecked()){
        qDebug() << "Set countdown";
    }
    else{
        qDebug() << "Cancel countdown" ;
    }
    if(psetformat_check_->isChecked()){
        if(pformat_widget_->IsEmpty()){
            QMessageBox mes(QMessageBox::Critical,tr("Error"),tr("Format can not be empty"),QMessageBox::Ok, nullptr, Qt::FramelessWindowHint);
            mes.setStyleSheet("QMessageBox{border: 1px solid #1296db;}"
                              "QMessageBox{background-color:white}");
            mes.exec();
            return;
        }
        qDebug() << "Set customformat";
    }
    else{
        qDebug() << "Cancel customformat";
    }

    QString path;
    QString format;
    qDebug() << "get path from name";
    qDebug() << "judge file existed or object in list.Yes!!";
    QMessageBox mes(QMessageBox::Warning,tr("Delete file"),tr("File exist.Do you want to overwrite?"),QMessageBox::Yes|QMessageBox::No, nullptr, Qt::FramelessWindowHint);
    mes.setStyleSheet("QMessageBox{border: 1px solid #1296db;}"
                      "QMessageBox{background-color:white}");
    if(mes.exec() == QMessageBox::No){
        return;
    }
    qDebug() << "In list, remove";
    qDebug() << "Have file, Delete";
    qDebug() << "New file";
    if(pcapture_dialog_ == nullptr){
        pcapture_dialog_ = new CaptureDialog;
        connect(this,SIGNAL(Capture_Signal(QString,QString)),pcapture_dialog_,SLOT(Capture_Slot(QString,QString)));
        pcapture_dialog_->Initialize();
    }
    pcapture_dialog_->Reset();
    emit Capture_Signal(path, format);
    if(pcapture_dialog_->exec() == QDialog::Accepted){
        emit NewObject_Signal(pname_edit_->text());
        accept();
        QMessageBox mes(QMessageBox::Information,tr(""),tr("Capture successful"),QMessageBox::Ok, nullptr, Qt::FramelessWindowHint);
        mes.setStyleSheet("QMessageBox{border: 1px solid #1296db;}"
                          "QMessageBox{background-color:white}");
        mes.exec();
    }
    else{
        reject();
        QMessageBox mes(QMessageBox::Critical,tr(""),tr("Capture failed"),QMessageBox::Ok, nullptr, Qt::FramelessWindowHint);
        mes.setStyleSheet("QMessageBox{border: 1px solid #1296db;}"
                          "QMessageBox{background-color:white}");
        mes.exec();
    }
}

void NewDialog::UpdateTranslation()
{
    pname_label_->setText(tr("File Name:"));
    psettime_check_->setText(tr("Set times"));
    psetformat_check_->setText(tr("Set Audio Format"));
    ptime_widget_->UpdateTranslation();
    pformat_widget_->UpdateTranslation();
    if(pcapture_dialog_){
        pcapture_dialog_->UpdateTranslation();
    }
}

void NewDialog::Initialize()
{
    pmicro_rbtn_->setChecked(true);
    ploopback_rbtn_->setChecked(false);
    psettime_check_->setChecked(false);
    psetformat_check_->setChecked(false);
    ptime_widget_->setMaximumHeight(0);
    pformat_widget_->setMaximumHeight(0);
    resize(0,0);
}

void NewDialog::Reset()
{
    pname_edit_->setText("");
}
