#ifndef TIMEWIDGET_H
#define TIMEWIDGET_H

#include <QWidget>

class QLabel;
class QTimeEdit;

class TimeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TimeWidget(QWidget *parent = nullptr);

private:
    QLabel* pcdstart_label_;
    QLabel* pcdstop_label_;
    QTimeEdit* pcdstart_timeedit_;
    QTimeEdit* pcdstop_timeedit_;

public:
    void UpdateTranslation();
    long GetStartTime();
    long GetStopTime();
};

#endif // TIMEWIDGET_H
