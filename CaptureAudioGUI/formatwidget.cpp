#include "formatwidget.h"
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QValidator>

FormatWidget::FormatWidget(QWidget *parent) : QWidget(parent)
{
    pformattag_label_ = new QLabel(this);
    pchannels_label_ = new QLabel(this);
    psamplepersec_label_ = new QLabel(this);
    pavgbytepersec_label_ = new QLabel(this);
    pblockalign_label_ = new QLabel(this);
    pbitspersample_label_ = new QLabel(this);
    pcbsize_label_ = new QLabel(this);
    psample_label_ = new QLabel(this);
    pdwchannelmask_label_ = new QLabel(this);
    psubformat_label_ = new QLabel(this);
    pformattag_edit_ = new QLineEdit(this);
    pchannels_edit_ = new QLineEdit(this);
    psamplepersec_edit_ = new QLineEdit(this);
    pavgbytepersec_edit_ = new QLineEdit(this);
    pblockalign_edit_ = new QLineEdit(this);
    pbitspersample_edit_ = new QLineEdit(this);
    pcbsize_edit_ = new QLineEdit(this);
    psample_edit_ = new QLineEdit(this);
    pdwchannelmask_edit_ = new QLineEdit(this);
    psubformat_edit_ = new QLineEdit(this);
    QGridLayout* playout = new QGridLayout(this);

    setLayout(playout);
    playout->addWidget(pformattag_label_,0,0);
    playout->addWidget(pformattag_edit_,0,1);
    playout->addWidget(pchannels_label_,1,0);
    playout->addWidget(pchannels_edit_,1,1);
    playout->addWidget(psamplepersec_label_,2,0);
    playout->addWidget(psamplepersec_edit_,2,1);
    playout->addWidget(pavgbytepersec_label_,3,0);
    playout->addWidget(pavgbytepersec_edit_,3,1);
    playout->addWidget(pblockalign_label_,4,0);
    playout->addWidget(pblockalign_edit_,4,1);
    playout->addWidget(pbitspersample_label_,5,0);
    playout->addWidget(pbitspersample_edit_,5,1);
    playout->addWidget(pcbsize_label_,6,0);
    playout->addWidget(pcbsize_edit_,6,1);
    playout->addWidget(psample_label_,7,0);
    playout->addWidget(psample_edit_,7,1);
    playout->addWidget(pdwchannelmask_label_,8,0);
    playout->addWidget(pdwchannelmask_edit_,8,1);
    playout->addWidget(psubformat_label_,9,0);
    playout->addWidget(psubformat_edit_,9,1);


    //labels
    pformattag_label_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    pchannels_label_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    psamplepersec_label_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    pavgbytepersec_label_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    pblockalign_label_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    pbitspersample_label_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    pcbsize_label_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    psample_label_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    pdwchannelmask_label_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    psubformat_label_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    pformattag_label_->setText(tr("FormatTag: "));
    pchannels_label_->setText(tr("Channels: "));
    psamplepersec_label_->setText(tr("Sample Per Second: "));
    pavgbytepersec_label_->setText(tr("Average Byte Per Second: "));
    pblockalign_label_->setText(tr("Block Align: "));
    pbitspersample_label_->setText(tr("Bits Per Sample: "));
    pcbsize_label_->setText(tr("cbSize: "));
    psample_label_->setText(tr("Sample: "));
    pdwchannelmask_label_->setText(tr("dwChannelMask: "));
    psubformat_label_->setText(tr("SubFormat: "));


    pformattag_label_->setStyleSheet("font: 75 10pt Adobe Gothic Std B;");
    pchannels_label_->setStyleSheet("font: 75 10pt Adobe Gothic Std B;");
    psamplepersec_label_->setStyleSheet("font: 75 10pt Adobe Gothic Std B;");
    pavgbytepersec_label_->setStyleSheet("font: 75 10pt Adobe Gothic Std B;");
    pblockalign_label_->setStyleSheet("font: 75 10pt Adobe Gothic Std B;");
    pbitspersample_label_->setStyleSheet("font: 75 10pt Adobe Gothic Std B;");
    pcbsize_label_->setStyleSheet("font: 75 10pt Adobe Gothic Std B;");
    psample_label_->setStyleSheet("font: 75 10pt Adobe Gothic Std B;");
    pdwchannelmask_label_->setStyleSheet("font: 75 10pt Adobe Gothic Std B;");
    psubformat_label_->setStyleSheet("font: 75 10pt Adobe Gothic Std B;");

    //edit
    pformattag_edit_->setValidator(new QIntValidator(0,65535,this));
    pchannels_edit_->setValidator(new QIntValidator(0,2,this));
    psamplepersec_edit_->setValidator(new QIntValidator(this));
    pavgbytepersec_edit_->setValidator(new QIntValidator(this));
    pblockalign_edit_->setValidator(new QIntValidator(this));
    pbitspersample_edit_->setValidator(new QIntValidator(this));
    pcbsize_edit_->setValidator(new QIntValidator(this));
    psample_edit_->setValidator(new QIntValidator(this));
    pdwchannelmask_edit_->setValidator(new QIntValidator(this));
    psubformat_edit_->setMaxLength(16);

    pformattag_edit_->setStyleSheet("font: 75 10pt Adobe Gothic Std B;");
    pchannels_edit_->setStyleSheet("font: 75 10pt Adobe Gothic Std B;");
    psamplepersec_edit_->setStyleSheet("font: 75 10pt Adobe Gothic Std B;");
    pavgbytepersec_edit_->setStyleSheet("font: 75 10pt Adobe Gothic Std B;");
    pblockalign_edit_->setStyleSheet("font: 75 10pt Adobe Gothic Std B;");
    pbitspersample_edit_->setStyleSheet("font: 75 10pt Adobe Gothic Std B;");
    pcbsize_edit_->setStyleSheet("font: 75 10pt Adobe Gothic Std B;");
    psample_edit_->setStyleSheet("font: 75 10pt Adobe Gothic Std B;");
    pdwchannelmask_edit_->setStyleSheet("font: 75 10pt Adobe Gothic Std B;");
    psubformat_edit_->setStyleSheet("font: 75 10pt Adobe Gothic Std B;");
}

void FormatWidget::UpdateTranslation()
{
    pformattag_label_->setText(tr("FormatTag: "));
    pchannels_label_->setText(tr("Channels: "));
    psamplepersec_label_->setText(tr("Sample Per Second: "));
    pavgbytepersec_label_->setText(tr("Average Byte Per Second: "));
    pblockalign_label_->setText(tr("Block Align: "));
    pbitspersample_label_->setText(tr("Bits Per Sample: "));
    pcbsize_label_->setText(tr("cbSize: "));
    psample_label_->setText(tr("Sample: "));
    pdwchannelmask_label_->setText(tr("dwChannelMask: "));
    psubformat_label_->setText(tr("SubFormat: "));
}

bool FormatWidget::IsEmpty()
{
    return pformattag_edit_->text().isEmpty() ||
            pchannels_edit_->text().isEmpty() ||
            psamplepersec_edit_->text().isEmpty() ||
            pavgbytepersec_edit_->text().isEmpty() ||
            pblockalign_edit_->text().isEmpty() ||
            pbitspersample_edit_->text().isEmpty() ||
            pcbsize_edit_->text().isEmpty() ||
            psample_edit_->text().isEmpty() ||
            pdwchannelmask_edit_->text().isEmpty() ||
            psubformat_edit_->text().isEmpty();
}

unsigned short FormatWidget::get_formattag()
{
    bool ok;
    return pformattag_edit_->text().toUShort(&ok,10);
}

unsigned short FormatWidget::get_channels()
{
    bool ok;
    return pchannels_edit_->text().toUShort(&ok,10);
}

unsigned long FormatWidget::get_samplepersec()
{
    bool ok;
    return psamplepersec_edit_->text().toULong(&ok,10);
}

unsigned long FormatWidget::get_avgbytepersec()
{
    bool ok;
    return pavgbytepersec_edit_->text().toULong(&ok,10);
}

unsigned short FormatWidget::get_blockalign()
{
    bool ok;
    return pblockalign_edit_->text().toUShort(&ok,10);
}

unsigned short FormatWidget::get_bitspersample()
{
    bool ok;
    return pbitspersample_edit_->text().toUShort(&ok,10);
}

unsigned short FormatWidget::get_cbsize()
{
    bool ok;
    return pcbsize_edit_->text().toUShort(&ok,10);
}

unsigned short FormatWidget::get_sample()
{
    bool ok;
    return psample_edit_->text().toUShort(&ok,10);
}

unsigned long FormatWidget::get_dwchannelmask()
{
    bool ok;
    return pdwchannelmask_edit_->text().toULong(&ok,10);
}

QString FormatWidget::get_subformat()
{
    return psubformat_edit_->text();
}

