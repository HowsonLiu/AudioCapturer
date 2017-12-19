#include "objectdialog.h"
#include "ui_objectdialog.h"
#include "objectinfowidget.h"
#include "titlebuttons.h"
#include <QBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include "transformwidget.h"
#include "ncframelesshelper.h"
#include "QPropertyAnimation"
#include <QParallelAnimationGroup>
#include <QMessageBox>
#include <thread>

#include <Windows.h>
#include <QDebug>

ObjectDialog::ObjectDialog(QWidget *parent) :
    QDialog(parent),currentpath_(QString()),
    ui(new Ui::ObjectDialog)
{
    ui->setupUi(this);

    pinfo_widget_ = new ObjectInfoWidget(this);
    pdelete_btn_ = new QPushButton(this);
    pplay_btn_ = new QCheckBox(this);
    ptransform_btn_ = new QCheckBox(this);
    ptitle_btns_ = new TitleButtons(this);
    ptransform_widget_ = new TransformWidget(this);
    ptrans_animation_ = new QPropertyAnimation(ptransform_widget_);
    panimation_ = new QPropertyAnimation(this);
    panigroup_ = new QParallelAnimationGroup;
    QHBoxLayout* playout = new QHBoxLayout(this);
    QVBoxLayout* pmiddlelayout = new QVBoxLayout(this);
    QVBoxLayout* prightlayout = new QVBoxLayout(this);

    //noframe
    NcFramelessHelper* nc = new NcFramelessHelper;
    nc->activateOn(this);
    nc->setWidgetResizable(false);
    setStyleSheet("QWidget#ObjectDialog{border: 1px solid #1296db;}"
                  "QWidget#ObjectDialog{background-color:white}");

    //set layout
    setLayout(playout);
    playout->addWidget(pinfo_widget_);
    playout->addSpacing(50);
    playout->addLayout(pmiddlelayout);
    playout->addWidget(ptransform_widget_);
    playout->addLayout(prightlayout);
    pmiddlelayout->addStretch();
    pmiddlelayout->addWidget(pdelete_btn_);
    pmiddlelayout->addSpacing(10);
    pmiddlelayout->addWidget(pplay_btn_);
    pmiddlelayout->addSpacing(10);
    pmiddlelayout->addWidget(ptransform_btn_);
    pmiddlelayout->addStretch();
    prightlayout->addWidget(ptitle_btns_);
    prightlayout->addStretch();

    //set animation
    panigroup_->addAnimation(ptrans_animation_);
    panigroup_->addAnimation(panimation_);
    ptrans_animation_->setTargetObject(ptransform_widget_);
    panimation_->setTargetObject(this);

    //close
    ptitle_btns_->AddCloseButton();
    connect(ptitle_btns_,SIGNAL(CloseButtonClick_Signal(bool)),this,SLOT(reject()));

    //delete
    pdelete_btn_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    pdelete_btn_->setMinimumSize(35,35);
    pdelete_btn_->setMaximumSize(35,35);
    pdelete_btn_->setStyleSheet("QPushButton{border-image: url(:/rec/img/ObjectDialog-Delete.png)}"
                               "QPushButton:hover{border-image: url(:/rec/img/ObjectDialog-Delete-hover.png)}");
    connect(pdelete_btn_,SIGNAL(clicked(bool)),this,SLOT(Delete_Slot()));

    //play
    pplay_btn_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    pplay_btn_->setMinimumSize(35,35);
    pplay_btn_->setMaximumSize(35,35);
    pplay_btn_->setStyleSheet("QCheckBox::indicator:unchecked{image: url(:/rec/img/ObjectDialog-Listen-uncheck.png);width: 35px;height: 35px;}"
                               "QCheckBox::indicator:checked{image: url(:/rec/img/ObjectDialog-Listen-check.png);width: 35px;height: 35px}");
    connect(pplay_btn_,SIGNAL(toggled(bool)),this,SLOT(Play_Slot(bool)));

    //transform
    ptransform_btn_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    ptransform_btn_->setStyleSheet("QCheckBox::indicator {width: 35px;height: 35px;}"
                                  "QCheckBox::indicator:unchecked {image: url(:/rec/img/ObjectDialog-Transform-uncheck.png);}"
                                  "QCheckBox::indicator:checked {image: url(:/rec/img/ObjectDialog-Transform-check.png);}");
    connect(ptransform_btn_,SIGNAL(toggled(bool)),this,SLOT(SetTransformEnable_Slot(bool)));

    //quit
    connect(this,&ObjectDialog::rejected,[this](){this->pplay_btn_->setChecked(false);});
    connect(this,&ObjectDialog::accepted,[this](){this->pplay_btn_->setChecked(false);});


    //initialize
    Initialize();
}

ObjectDialog::~ObjectDialog()
{
    delete ui;
}

void ObjectDialog::SetTransformEnable_Slot(bool b)
{
    if(panigroup_->Running){
        panigroup_->stop();
    }
    int animationduration = 200;
    if(b){
        ptransform_widget_->setMinimumWidth(0);
        ptransform_widget_->setMaximumWidth(500);
        ptrans_animation_->setPropertyName("maximumWidth");
        ptrans_animation_->setDuration(animationduration);
        ptrans_animation_->setStartValue(ptransform_widget_->width());
        ptrans_animation_->setEndValue(500);

        panimation_->setPropertyName("size");
        panimation_->setDuration(animationduration);
        panimation_->setStartValue(size());
        panimation_->setEndValue(QSize(0,height()));

        panigroup_->start();
    }else{
        ptransform_widget_->setMinimumWidth(0);
        ptransform_widget_->setMaximumWidth(500);
        ptrans_animation_->setPropertyName("maximumWidth");
        ptrans_animation_->setDuration(animationduration);
        ptrans_animation_->setStartValue(ptransform_widget_->width());
        ptrans_animation_->setEndValue(0);

        panimation_->setPropertyName("size");
        panimation_->setDuration(animationduration);
        panimation_->setStartValue(size());
        panimation_->setEndValue(QSize(0,height()));

        panigroup_->start();
    }
}

void ObjectDialog::Play_Slot(bool b)
{
    if(b){
        qDebug() << "Click Play";
        isstop_ = false;
        std::thread playth(&ObjectDialog::Play,this);
        playth.detach();
    }
    else{
        qDebug() << "Stop";
        isstop_ = true;
    }
}

void ObjectDialog::Play()
{
    //custom stop
    while(!isstop_){
        qDebug() << "Rendering...";
    }

//    //auto stop
//    Sleep(3000);

    pplay_btn_->setChecked(false);
}

void ObjectDialog::UpdateTranslation()
{
    pinfo_widget_->UpdateTranslation();
    ptransform_widget_->UpdateTranslation();
}

void ObjectDialog::UpdateObjectInfo()
{
    qDebug() << "Update Object Info" << endl;
}

void ObjectDialog::Delete_Slot()
{
    pplay_btn_->setChecked(false);
    emit Delete_Signal(currentpath_);
}

void ObjectDialog::Initialize()
{
    ptransform_btn_->setChecked(false);
    ptransform_widget_->setMaximumWidth(0);
    ptransform_widget_->Initialize();
    resize(0,0);
}

void ObjectDialog::Reset()
{

}

void ObjectDialog::SetCurrentObject(const QString path)
{
    currentpath_ = path;
}
