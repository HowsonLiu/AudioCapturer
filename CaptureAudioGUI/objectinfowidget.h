#ifndef OBJECTINFOWIDGET_H
#define OBJECTINFOWIDGET_H

#include <QWidget>

class QLabel;

class ObjectInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ObjectInfoWidget(QWidget *parent = nullptr);

private:
    QLabel* pname_label_;
    QLabel* pcreattime_label_;
    QLabel* psize_label_;
    QLabel* pduration_label_;
    QLabel* pchannels_label_;
    QLabel* psamplepersec_label_;

    QLabel* pname_;
    QLabel* pcreattime_;
    QLabel* psize_;
    QLabel* pduration_;
    QLabel* pchannels_;
    QLabel* psamplepersec_;

public:
    void SetName(const QString);
    void SetCreatTime(const QString);
    void SetSize(const QString);
    void SetDuration(const QString);
    void SetChannels(const QString);
    void SetSamplePerSec(const QString);
    void UpdateTranslation();
};

#endif // OBJECTINFOWIDGET_H
