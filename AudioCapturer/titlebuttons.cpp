#include "titlebuttons.h"
#include <QPushButton>
#include <QBoxLayout>

TitleButtons::TitleButtons(QWidget *parent) : QWidget(parent)
{
    playout_ = new QHBoxLayout(this);
    pclose_btn_ = nullptr;
    pmini_btn_ = nullptr;
    psetting_btn_ = nullptr;
    setLayout(playout_);
    playout_->setContentsMargins(0,0,0,0);

    playout_->setSizeConstraint(QLayout::SetMinimumSize);
}

void TitleButtons::AddCloseButton()
{
    if(pclose_btn_==nullptr){
        pclose_btn_ = new QPushButton(this);
        pclose_btn_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        pclose_btn_->setStyleSheet("QPushButton{border-image: url(:/rec/img/Titlebtns-Close.png); width: 20px; height: 20px;}"
                                  "QPushButton:hover{border-image: url(:/rec/img/Titlebtns-Close-hover.png)}");
        playout_->addWidget(pclose_btn_);
        connect(pclose_btn_,&QPushButton::clicked,[this](bool b){emit CloseButtonClick_Signal(b);});
    }
}

void TitleButtons::AddMiniButton()
{
    if(pmini_btn_==nullptr){
        pmini_btn_ = new QPushButton(this);
        pmini_btn_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        pmini_btn_->setStyleSheet("QPushButton{border-image: url(:/rec/img/Titlebtns-Minimum.png); width: 20px; height: 20px;}"
                                 "QPushButton:hover{border-image: url(:/rec/img/Titlebtns-Minimum-hover.png)}");
        playout_->addWidget(pmini_btn_);
        connect(pmini_btn_,&QPushButton::clicked,[this](bool b){emit MiniButtonClick_Signal(b);});
    }
}

void TitleButtons::AddSettingButton()
{
    if(psetting_btn_==nullptr){
        psetting_btn_ = new QPushButton(this);
        psetting_btn_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        psetting_btn_->setStyleSheet("QPushButton{border-image: url(:/rec/img/Titlebtns-Setting.png); width: 20px; height: 20px;}"
                                 "QPushButton:hover{border-image: url(:/rec/img/Titlebtns-Setting-hover.png)}");
        playout_->addWidget(psetting_btn_);
        connect(psetting_btn_,&QPushButton::clicked,[this](bool b){emit SettingButtonClick_Signal(b);});
    }
}
