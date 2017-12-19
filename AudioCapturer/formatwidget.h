#ifndef FORMATWIDGET_H
#define FORMATWIDGET_H

#include <QWidget>

class QLineEdit;
class QLabel;

class FormatWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FormatWidget(QWidget *parent = nullptr);

private:
    QLabel* pformattag_label_;
    QLabel* pchannels_label_;
    QLabel* psamplepersec_label_;
    QLabel* pavgbytepersec_label_;
    QLabel* pblockalign_label_;
    QLabel* pbitspersample_label_;
    QLabel* pcbsize_label_;
    QLabel* psample_label_;
    QLabel* pdwchannelmask_label_;
    QLabel* psubformat_label_;
    QLineEdit* pformattag_edit_;
    QLineEdit* pchannels_edit_;
    QLineEdit* psamplepersec_edit_;
    QLineEdit* pavgbytepersec_edit_;
    QLineEdit* pblockalign_edit_;
    QLineEdit* pbitspersample_edit_;
    QLineEdit* pcbsize_edit_;
    QLineEdit* psample_edit_;
    QLineEdit* pdwchannelmask_edit_;
    QLineEdit* psubformat_edit_;

public:
    void UpdateTranslation();

    bool IsEmpty();
    unsigned short get_formattag();
    unsigned short get_channels();
    unsigned long get_samplepersec();
    unsigned long get_avgbytepersec();
    unsigned short get_blockalign();
    unsigned short get_bitspersample();
    unsigned short get_cbsize();
    unsigned short get_sample();
    unsigned long get_dwchannelmask();
    QString get_subformat();
};

#endif // FORMATWIDGET_H
