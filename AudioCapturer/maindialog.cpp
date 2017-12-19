#include "maindialog.h"
#include "ui_maindialog.h"
#include <QLabel>
#include <QListWidget>
#include <QBoxLayout>
#include <QPushButton>
#include "ncframelesshelper.h"
#include <QPixmap>
#include "settingdialog.h"
#include "newdialog.h"
#include "objectminiwidget.h"
#include "titlebuttons.h"
#include "objectdialog.h"
#include <QFileDialog>
#include "objectminiwidget.h"
#include <QMessageBox>
#include "dllfunction.h"

#include <QDebug>

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);

    picon_label_ = new QLabel(this);
    ptitle_label_ = new QLabel(this);
    ptitle_btn_ = new TitleButtons(this);
    plist_ = new QListWidget(this);
    pnew_btn_ = new QPushButton(this);
    popen_btn_ = new QPushButton(this);
    psetting_dialog_ = nullptr;
    pnew_dialog_ = nullptr;
    pobject_dialog_ = nullptr;
    QVBoxLayout* playout = new QVBoxLayout(this);
    QHBoxLayout* puplayout = new QHBoxLayout(this);
    QHBoxLayout* pdownlayout = new QHBoxLayout(this);
    QVBoxLayout* ptitlebtn_layout = new QVBoxLayout(this);

    //this dialog
    setWindowFlags( Qt::FramelessWindowHint );
    NcFramelessHelper* pframeless = new NcFramelessHelper;
    pframeless->activateOn(this);
    setStyleSheet("QWidget#MainDialog{border: 1px solid #1296db;}"
                  "QWidget#MainDialog{background-color:white}");

    //layout
    setLayout(playout);
    playout->addLayout(puplayout);
    playout->addWidget(plist_);
    playout->addLayout(pdownlayout);
    puplayout->addWidget(picon_label_);
    puplayout->addWidget(ptitle_label_);
    puplayout->addStretch();
    puplayout->addLayout(ptitlebtn_layout);
    ptitlebtn_layout->addWidget(ptitle_btn_);
    ptitlebtn_layout->addSpacing(76);
    pdownlayout->addWidget(pnew_btn_,0,Qt::AlignCenter);
    pdownlayout->addWidget(popen_btn_,0,Qt::AlignCenter);

    //margin
    playout->setContentsMargins(3,3,3,3);
    puplayout->setContentsMargins(0,0,0,0);

    //titlebutton
    ptitle_btn_->AddSettingButton();
    ptitle_btn_->AddMiniButton();
    ptitle_btn_->AddCloseButton();
    connect(ptitle_btn_,&TitleButtons::CloseButtonClick_Signal,[this](){this->close();});
    connect(ptitle_btn_,&TitleButtons::MiniButtonClick_Signal,[this](){this->showMinimized();});
    connect(ptitle_btn_,SIGNAL(SettingButtonClick_Signal(bool)),this,SLOT(Setting_Slot()));

    //icon
    QPixmap mainicon(":/rec/img/MainIcon.png");
    picon_label_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    picon_label_->setMaximumSize(96,96);
    picon_label_->setMinimumSize(96,96);
    picon_label_->setPixmap(mainicon);

    //title
    ptitle_label_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    ptitle_label_->setText("AudioCapturer");
    ptitle_label_->setStyleSheet("font: 75 20pt Adobe Gothic Std B;");

    //list
    connect(plist_,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(Double_Click_Object_Slot(QListWidgetItem*)));

    //new
    pnew_btn_->setStyleSheet("QPushButton{border-image: url(:/rec/img/MainDialog-New.png); width: 50px; height: 50px;}"
                            "QPushButton:hover{border-image: url(:/rec/img/MainDialog-New-hover.png)}");
    connect(pnew_btn_,SIGNAL(clicked(bool)),this,SLOT(New_Slot()));

    //open
    popen_btn_->setStyleSheet("QPushButton{border-image: url(:/rec/img/MainDialog-Open.png); width: 50px; height: 36px;}"
                            "QPushButton:hover{border-image: url(:/rec/img/MainDialog-Open-hover.png)}");
    connect(popen_btn_,SIGNAL(clicked(bool)),this,SLOT(Open_Slot()));

    Initialize();
}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::Setting_Slot()
{
    if(psetting_dialog_ == nullptr){
        psetting_dialog_ = new SettingDialog;
        connect(psetting_dialog_,SIGNAL(Translate()),this,SLOT(UpdateTranslation()));
    }
    psetting_dialog_->Reset();
    psetting_dialog_->exec();
}

void MainDialog::New_Slot()
{
    if(pnew_dialog_ == nullptr){
        pnew_dialog_ = new NewDialog;
        connect(pnew_dialog_,SIGNAL(NewObject_Signal(QString)),this,SLOT(AddObject_Slot(QString)));
        connect(pnew_dialog_,SIGNAL(CleanObject_Signal(QString)),this,SLOT(CleanObjectWithoutConfirm_Slot(QString)));
    }
    pnew_dialog_->Reset();
    pnew_dialog_->exec();
}

void MainDialog::Open_Slot()
{
    QString path = QFileDialog::getOpenFileName(this,tr("Select a file"),QDir::homePath(),"*.ca");
    if(QFile(path).open(QFile::ReadOnly)){
        qDebug() << "judge file is in list?" ;
        if(!IsCAObjectInList(path.toStdString().c_str())){
            qDebug() << "New file in dll" ;
            NewCAObject(path.toStdString().c_str());
            qDebug() << "Read file stream";
            ReadDataFromCAFile(path.toStdString().c_str());
            if(IsCAObjectValid(path.toStdString().c_str())){
                AddObject_Slot(path);
            }
            else{
                qDebug() << "Wrong file";
                RemoveCAObject(path.toStdString().c_str());
            }
        }
    }
}

void MainDialog::Double_Click_Object_Slot(QListWidgetItem* pitem)
{
    if(pobject_dialog_ == nullptr){
        pobject_dialog_ = new ObjectDialog;
        connect(pobject_dialog_,SIGNAL(Delete_Signal(const QString)),this,SLOT(CleanObjectWithConfirm_Slot(QString)));
        pobject_dialog_->Initialize();
    }
    pobject_dialog_->Reset();
    ObjectMiniWidget* pwidget = dynamic_cast<ObjectMiniWidget*>(plist_->itemWidget(pitem));
    pobject_dialog_->UpdateObjectInfo(pwidget->get_filepath());
    pobject_dialog_->exec();
}

void MainDialog::UpdateTranslation()
{
    if(psetting_dialog_){
        psetting_dialog_->UpdateTranslation();
    }
    if(pnew_dialog_){
        pnew_dialog_->UpdateTranslation();
    }
    if(pobject_dialog_){
        pobject_dialog_->UpdateTranslation();
    }
}

void MainDialog::Initialize()
{
    resize(0,600);
    picon_label_->setMaximumSize(96,96);
    picon_label_->setMinimumSize(96,96);
    qDebug() << "Load local cafile";
    SetDefaultFolder("D:\\");
    UpdateList();
}

ObjectMiniWidget* MainDialog::GetWidget(const QString path)
{
    for(int i = 0; i<plist_->count(); i++){
        ObjectMiniWidget* pwidget = dynamic_cast<ObjectMiniWidget*>(plist_->itemWidget(plist_->item(i)));
        if(pwidget->get_filepath() == path){
            return pwidget;
        }
    }
    return nullptr;
}

QListWidgetItem *MainDialog::GetItem(const QString path)
{
    for(int i = 0; i<plist_->count(); i++){
        ObjectMiniWidget* pwidget = dynamic_cast<ObjectMiniWidget*>(plist_->itemWidget(plist_->item(i)));
        if(pwidget->get_filepath() == path){
            return plist_->item(i);
        }
    }
    return nullptr;
}

//Add in GUI not dll
void MainDialog::AddObject_Slot(const QString path)
{
    QListWidgetItem* pitem = new QListWidgetItem(plist_);
    ObjectMiniWidget* pwidget = new ObjectMiniWidget(path,this);
    connect(pwidget,SIGNAL(DeletebtnClick_Signal(const QString)),this,SLOT(CleanObjectWithConfirm_Slot(QString)));
    plist_->addItem(pitem);
    plist_->setItemWidget(pitem,pwidget);
    pitem->setSizeHint(QSize(0,50));
}

void MainDialog::CleanObjectWithConfirm_Slot(const QString path)
{
    QMessageBox mes(QMessageBox::Warning,tr("Delete file"),tr("Are you sure to delete this file?"),QMessageBox::Yes|QMessageBox::No, nullptr, Qt::FramelessWindowHint);
        mes.setStyleSheet("QMessageBox{border: 1px solid #1296db;}"
                          "QMessageBox{background-color:white}");
        if(mes.exec() == QMessageBox::Yes){
            CleanObjectWithoutConfirm_Slot(path);
            if(pobject_dialog_){
                pobject_dialog_->close();
            }
        }
}

//delete file and object in dll and delete object in gui
void MainDialog::CleanObjectWithoutConfirm_Slot(const QString path)
{
    QListWidgetItem* pitem = GetItem(path);
    ObjectMiniWidget* pwidget = GetWidget(path);
    if(pitem != nullptr && pwidget != nullptr){
        plist_->removeItemWidget(pitem);
        delete pitem;
    }
    if(IsCAFileExist(path.toStdString().c_str())){
        DeleteCAFile(path.toStdString().c_str());
    }
    if(IsCAObjectInList(path.toStdString().c_str())){
        RemoveCAObject(path.toStdString().c_str());
    }
}

//update GUI and dll
void MainDialog::UpdateList()
{
    UpdateMap();
    using namespace std;
    vector<string> namesvec = ChangeCharppToVector(GetListNames());
    vector<string>::iterator iter;
    for(iter = namesvec.begin();iter!=namesvec.end();iter++){
        AddObject_Slot(QString::fromStdString(*iter));
    }
}
