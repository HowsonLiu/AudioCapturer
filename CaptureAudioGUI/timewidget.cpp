#include "timewidget.h"
#include <QTimeEdit>
#include <QLabel>
#include <QGridLayout>

TimeWidget::TimeWidget(QWidget *parent) : QWidget(parent)
{
    pcdstart_label_ = new QLabel(this);
    pcdstop_label_ = new QLabel(this);
    pcdstart_timeedit_ = new QTimeEdit(this);
    pcdstop_timeedit_ = new QTimeEdit(this);
    QGridLayout* playout = new QGridLayout(this);

    setLayout(playout);
    playout->addWidget(pcdstart_label_,0,0);
    playout->addWidget(pcdstop_label_,0,1);
    playout->addWidget(pcdstart_timeedit_,1,0);
    playout->addWidget(pcdstop_timeedit_,1,1);

    //label
    pcdstart_label_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    pcdstop_label_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    pcdstart_label_->setText(tr("Time to start: "));
    pcdstop_label_->setText(tr("Max Duration: "));
    pcdstart_label_->setStyleSheet("font: 75 10pt Adobe Gothic Std B;");
    pcdstop_label_->setStyleSheet("font: 75 10pt Adobe Gothic Std B;");

    //timeedit
    pcdstart_timeedit_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    pcdstop_timeedit_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    pcdstart_timeedit_->setStyleSheet("QTimeEdit{font: 75 30pt Adobe Gothic Std B;selection-background-color: darkgray;}"
                                     "QTimeEdit::up-button{image: url(:/rec/img/NewDialog-Up.png);width: 15px;height: 15px;}"
                                     "QTimeEdit::down-button{image: url(:/rec/img/NewDialog-Down.png);width: 15px;height: 15px;}"
                                     "QTimeEdit::up-button:hover{image: url(:/rec/img/NewDialog-Up-hover.png)}"
                                     "QTimeEdit::down-button:hover{image: url(:/rec/img/NewDialog-Down-hover.png)}");
    pcdstop_timeedit_->setStyleSheet("QTimeEdit{font: 75 30pt Adobe Gothic Std B;selection-background-color: darkgray;}"
                                     "QTimeEdit::up-button{image: url(:/rec/img/NewDialog-Up.png);width: 15px;height: 15px;}"
                                     "QTimeEdit::down-button{image: url(:/rec/img/NewDialog-Down.png);width: 15px;height: 15px;}"
                                     "QTimeEdit::up-button:hover{image: url(:/rec/img/NewDialog-Up-hover.png)}"
                                    "QTimeEdit::down-button:hover{image: url(:/rec/img/NewDialog-Down-hover.png)}");
}

void TimeWidget::UpdateTranslation()
{
    pcdstart_label_->setText(tr("Time to start: "));
    pcdstop_label_->setText(tr("Max Duration: "));
}
