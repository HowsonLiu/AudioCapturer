#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include <QDialog>

class TimeWidget;
class FormatWidget;
class QLabel;
class QLineEdit;
class QRadioButton;
class QCheckBox;
class QPropertyAnimation;
class QParallelAnimationGroup;
class CaptureDialog;

namespace Ui {
class NewDialog;
}

class NewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewDialog(QWidget *parent = 0);
    ~NewDialog();

private:
    Ui::NewDialog *ui;

    QLabel* pname_label_;
    QLineEdit* pname_edit_;
    QCheckBox* psettime_check_;
    QCheckBox* psetformat_check_;
    QRadioButton* pmicro_rbtn_;
    QRadioButton* ploopback_rbtn_;
    TimeWidget* ptime_widget_;
    FormatWidget* pformat_widget_;
    QPushButton* pback_btn_;
    QPushButton* pstart_btn_;

    QPropertyAnimation* ptime_animation_;
    QPropertyAnimation* pformat_animation_;
    QPropertyAnimation* panimation_with_time_;
    QPropertyAnimation* panimation_with_format_;
    QParallelAnimationGroup* ptime_animation_group_;
    QParallelAnimationGroup* pformat_animation_group_;

    CaptureDialog* pcapture_dialog_;

signals:
    void NewObject_Signal(const QString);
    void Capture_Signal(const QString,const QString);

public:
    void UpdateTranslation();
    void Initialize();
    void Reset();

private slots:
    void SetTimeWidgetEnable_Slot(bool);
    void SetFormatWidgetEnable_Slot(bool);
    void Start_Slot();
};

#endif // NEWDIALOG_H
