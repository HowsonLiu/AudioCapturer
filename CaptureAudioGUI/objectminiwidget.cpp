#include "objectminiwidget.h"
#include <QLabel>
#include <QPushButton>
#include <QBoxLayout>

ObjectMiniWidget::ObjectMiniWidget(const QString path, QWidget *parent) : QWidget(parent), filepath_(path)
{
    pname_label_ = new QLabel(this);
    pdelete_btn_ = new QPushButton(this);
    QHBoxLayout* playout = new QHBoxLayout(this);

    setLayout(playout);
    playout->addWidget(pname_label_);
    playout->addStretch();
    playout->addWidget(pdelete_btn_);

    //name
    pname_label_->setStyleSheet("font: 75 10pt Adobe Gothic Std B;");
    pname_label_->setText(path);

    //delete
    pdelete_btn_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    pdelete_btn_->setMinimumSize(QSize(30,30));
    pdelete_btn_->setMaximumSize(QSize(30,30));
    pdelete_btn_->setStyleSheet("QPushButton{border-image: url(:/rec/img/ObjectDialog-Delete.png);}"
                               "QPushButton:hover{border-image: url(:/rec/img/ObjectDialog-Delete-hover.png)}");
    connect(pdelete_btn_,&QPushButton::clicked,[this](){emit DeletebtnClick_Signal(filepath_);});
}
