#include "objectinfowidget.h"
#include <QLabel>
#include <QGridLayout>

ObjectInfoWidget::ObjectInfoWidget(QWidget *parent) : QWidget(parent)
{
    pname_label_ = new QLabel(this);
    pcreattime_label_ = new QLabel(this);
    psize_label_ = new QLabel(this);
    pduration_label_ = new QLabel(this);
    pchannels_label_ = new QLabel(this);
    psamplepersec_label_ = new QLabel(this);
    pname_ = new QLabel(this);
    pcreattime_ = new QLabel(this);
    psize_ = new QLabel(this);
    pduration_ = new QLabel(this);
    pchannels_ = new QLabel(this);
    psamplepersec_ = new QLabel(this);
    QGridLayout* playout = new QGridLayout(this);

    setLayout(playout);
    playout->addWidget(pname_label_,0,0);
    playout->addWidget(pcreattime_label_,1,0);
    playout->addWidget(psize_label_,2,0);
    playout->addWidget(pduration_label_,3,0);
    playout->addWidget(pchannels_label_,4,0);
    playout->addWidget(psamplepersec_label_,5,0);
    playout->addWidget(pname_,0,1);
    playout->addWidget(pcreattime_,1,1);
    playout->addWidget(psize_,2,1);
    playout->addWidget(pduration_,3,1);
    playout->addWidget(pchannels_,4,1);
    playout->addWidget(psamplepersec_,5,1);

    pname_label_->setText(tr("Name: "));
    pcreattime_label_->setText(tr("Creat time: "));
    psize_label_->setText(tr("Size: "));
    pduration_label_->setText(tr("Duration: "));
    pchannels_label_->setText(tr("Channels: "));
    psamplepersec_label_->setText(tr("Sample Per Sec: "));

    pname_label_->setStyleSheet("font: 75 15pt Adobe Gothic Std B;");
    pcreattime_label_->setStyleSheet("font: 75 15pt Adobe Gothic Std B;");
    psize_label_->setStyleSheet("font: 75 15pt Adobe Gothic Std B;");
    pduration_label_->setStyleSheet("font: 75 15pt Adobe Gothic Std B;");
    pchannels_label_->setStyleSheet("font: 75 15pt Adobe Gothic Std B;");
    psamplepersec_label_->setStyleSheet("font: 75 15pt Adobe Gothic Std B;");
    pname_->setStyleSheet("font: 75 15pt Adobe Gothic Std B;");
    pcreattime_->setStyleSheet("font: 75 15pt Adobe Gothic Std B;");
    psize_->setStyleSheet("font: 75 15pt Adobe Gothic Std B;");
    pduration_->setStyleSheet("font: 75 15pt Adobe Gothic Std B;");
    pchannels_->setStyleSheet("font: 75 15pt Adobe Gothic Std B;");
    psamplepersec_->setStyleSheet("font: 75 15pt Adobe Gothic Std B;");
}

void ObjectInfoWidget::SetName(const QString name)
{
    pname_->setText(name);
}

void ObjectInfoWidget::SetCreatTime(const QString time)
{
    pcreattime_->setText(time);
}

void ObjectInfoWidget::SetSize(const QString size)
{
    psize_->setText(size);
}

void ObjectInfoWidget::SetDuration(const QString duration)
{
    pduration_->setText(duration);
}

void ObjectInfoWidget::SetChannels(const QString channel)
{
    pchannels_->setText(channel);
}

void ObjectInfoWidget::SetSamplePerSec(const QString spc)
{
    psamplepersec_->setText(spc);
}

void ObjectInfoWidget::UpdateTranslation()
{
    pname_label_->setText(tr("Name: "));
    pcreattime_label_->setText(tr("Creat time: "));
    psize_label_->setText(tr("Size: "));
    pduration_label_->setText(tr("Duration: "));
    pchannels_label_->setText(tr("Channels: "));
    psamplepersec_label_->setText(tr("Sample Per Sec: "));

}
