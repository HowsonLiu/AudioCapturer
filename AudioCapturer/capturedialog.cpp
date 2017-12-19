#include "capturedialog.h"
#include "ui_capturedialog.h"
#include <QLabel>
#include <QLCDNumber>
#include <QBoxLayout>
#include <QPushButton>
#include "ncframelesshelper.h"
#include <QDesktopWidget>
#include <thread>
#include <Windows.h>
#include "dllfunction.h"

#include <QDebug>

using namespace std;

CaptureDialog::CaptureDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CaptureDialog)
{
    ui->setupUi(this);

    pcountdown_label_ = new QLabel(this);
    pcountdown_lcd_ = new QLCDNumber(this);
    pruntime_label_ = new QLabel(this);
    pruntime_lcd_ = new QLCDNumber(this);
    pbtn_ = new QPushButton(this);
    QVBoxLayout* playout = new QVBoxLayout(this);

    //this dialog
    NcFramelessHelper* nc = new NcFramelessHelper;
    nc->activateOn(this);
    nc->setWidgetResizable(false);
    setStyleSheet("QWidget#CaptureDialog{border: 2px solid #1296db;}"
                  "QWidget#CaptureDialog{background-color:white}");

    setLayout(playout);
    playout->addWidget(pcountdown_label_);
    playout->addWidget(pcountdown_lcd_);
    playout->addWidget(pruntime_label_);
    playout->addWidget(pruntime_lcd_);
    playout->addWidget(pbtn_,0,Qt::AlignHCenter);

    //cdlabel
    pcountdown_label_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    pcountdown_label_->setStyleSheet("font: 75 15pt Adobe Gothic Std B;");
    pcountdown_label_->setText(tr("Time to start: "));

    //runlabel
    pruntime_label_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    pruntime_label_->setStyleSheet("font: 75 15pt Adobe Gothic Std B;");
    pruntime_label_->setText(tr("Run time: "));

    //lcd
    pcountdown_lcd_->setDigitCount(9);
    pruntime_lcd_->setDigitCount(9);

    //button
    pbtn_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    pbtn_->setStyleSheet("QPushButton{border-image: url(:/rec/img/CaptureDialog-Play.png);width: 40px; height: 40px}"
                        "QPushButton:hover{border-image: url(:/rec/img/CaptureDialog-Play-hover.png)}");
    connect(pbtn_,SIGNAL(clicked(bool)),this,SLOT(Start_Slot()));

    //capture
    connect(this,SIGNAL(Start_Signal()),this,SLOT(Start_Slot()));
    connect(this,SIGNAL(Success_Signal()),this,SLOT(Success_Slot()));
    connect(this,SIGNAL(Failed_Signal()),this,SLOT(Fail_Slot()));

    //quit
    connect(this,SIGNAL(rejected()),this,SLOT(Stop_Slot()));
    connect(this,SIGNAL(accepted()),this,SLOT(Stop_Slot()));

    //initialize
    Initialize();
}

CaptureDialog::~CaptureDialog()
{
    delete ui;
}

void CaptureDialog::UpdateTranslation()
{
    pcountdown_label_->setText(tr("Time to start: "));
    pruntime_label_->setText(tr("Run time: "));
}

void CaptureDialog::Capture_Slot(const QString path, const QString format)
{
    std::thread captureth(&CaptureDialog::Capturing,this,path.toStdString(),format.toStdString());
    captureth.detach();
    std::thread updateth(&CaptureDialog::UpdateTime,this);
    updateth.detach();
}

void CaptureDialog::Success_Slot()
{
    isstart_ = true;
    isstop_ = true;
    accept();
}

void CaptureDialog::Fail_Slot()
{
    isstart_ = true;
    isstop_ = true;
    reject();
}

void CaptureDialog::Start_Slot()
{
    isstart_ = true;
    SetCapturingMode();
    qDebug() << "Click start" ;
}

void CaptureDialog::Stop_Slot()
{
    isstop_ = true;
    SetPreparingMode();
    qDebug() << "Click stop";
}

void CaptureDialog::UpdateTime()
{
    long oldcdstartvalue = 3600000;
    long oldcdstopvalue = 9000000;
    long oldrunvalue = -1;
    bool isstart = false;//出现不计时的情况需要自动跳转
    bool runreset = false;//防止runtime还没有重置的时候就更新oldvalue
    while(!isstop_){
        if(!isstart_){
            if(GetCountdownToStart() != nullptr){
                if(*GetCountdownToStart() < oldcdstartvalue - 40 && *GetCountdownToStart() > 0){
                    qDebug() << "Update count down to start";
                    QString minute = QString::number(*GetCountdownToStart() / 60000,10);
                    QString second = QString::number((*GetCountdownToStart() % 60000) / 1000,10);
                    QString msec = QString::number(*GetCountdownToStart() % 1000,10);
                    QString show = minute + ":" + second + ":" + msec;
                    pcountdown_lcd_->display(show);
                    oldcdstartvalue = *GetCountdownToStart();
                }
            }
            else{
                isstart_ = true;
            }
        }
        else{
            if(GetCountdownToStop() != nullptr){
                if(*GetCountdownToStop() < oldcdstopvalue - 40 && *GetCountdownToStop() > 0){
                    qDebug() << "Update count down to stop";
                    QString cdminute = QString::number(*GetCountdownToStop() / 60000,10);
                    QString cdsecond = QString::number((*GetCountdownToStop() % 60000) / 1000,10);
                    QString cdmsec = QString::number(*GetCountdownToStop() % 1000,10);
                    QString cdshow = cdminute + ":" + cdsecond + ":" + cdmsec;
                    pcountdown_lcd_->display(cdshow);
                    oldcdstopvalue = *GetCountdownToStop();
                }
            }
            if(runreset){
                if(*GetRunTime() > oldrunvalue + 40){
                    qDebug() << "Update runing time";
                    QString minute = QString::number(*GetRunTime() / 60000,10);
                    QString second = QString::number((*GetRunTime() % 60000) / 1000,10);
                    QString msec = QString::number(*GetRunTime() % 1000,10);
                    QString show = minute + ":" + second + ":" + msec;
                    pruntime_lcd_->display(show);
                    oldrunvalue = *GetRunTime();
                }
            }
            else{
                if(*GetRunTime() == 0){
                    runreset = true;
                }
            }
            if(!isstart){
                emit Start_Signal();
                isstart = true;
            }
        }
    }
}

void CaptureDialog::Capturing(const string path,const string format)
{
    bool b = CaptureAudio(path.c_str(),format.c_str(),&isstop_);
    if(b){
        emit Success_Signal();
    }
    else{
        emit Failed_Signal();
    }
}

void CaptureDialog::SetPreparingMode()
{
    disconnect(pbtn_,SIGNAL(clicked(bool)),this,SLOT(Stop_Slot()));
    connect(pbtn_,SIGNAL(clicked(bool)),this,SLOT(Start_Slot()));
    pcountdown_label_->setText(tr("Time to start: "));
    pcountdown_lcd_->display("0:0:000");
    pruntime_lcd_->display("0:0:000");
    pbtn_->setStyleSheet("QPushButton{border-image: url(:/rec/img/CaptureDialog-Play.png);width: 40px; height: 40px}"
                        "QPushButton:hover{border-image: url(:/rec/img/CaptureDialog-Play-hover.png)}");
}

void CaptureDialog::SetCapturingMode()
{
    disconnect(pbtn_,SIGNAL(clicked(bool)),this,SLOT(Start_Slot()));
    connect(pbtn_,SIGNAL(clicked(bool)),this,SLOT(Stop_Slot()));
    pcountdown_label_->setText(tr("Time to stop: "));
    pcountdown_lcd_->display("0:0:000");
    pruntime_lcd_->display("0:0:000");
    pbtn_->setStyleSheet("QPushButton{border-image: url(:/rec/img/CaptureDialog-Stop.png);width: 40px; height: 40px}"
                        "QPushButton:hover{border-image: url(:/rec/img/CaptureDialog-Stop-hover.png)}");
}

void CaptureDialog::Initialize()
{
    resize(0,500);
    move(QApplication::desktop()->width() - 150,QApplication::desktop()->height() / 2 - 250);
}

void CaptureDialog::Reset()
{
    isstart_ = false;
    isstop_ = false;
    SetPreparingMode();
}
