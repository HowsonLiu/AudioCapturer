#ifndef CAPTUREDIALOG_H
#define CAPTUREDIALOG_H

#include <QDialog>

class QLabel;
class QLCDNumber;
class QPushButton;

namespace Ui {
class CaptureDialog;
}

class CaptureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CaptureDialog(QWidget *parent = 0);
    ~CaptureDialog();

private:
    Ui::CaptureDialog *ui;

    QLabel* pcountdown_label_;
    QLCDNumber* pcountdown_lcd_;
    QLabel* pruntime_label_;
    QLCDNumber* pruntime_lcd_;
    QPushButton* pbtn_;

    bool isstart_;
    bool isstop_;

public:
    void Initialize();
    void Reset();
    void UpdateTranslation();

    inline bool* get_isstart(){
        return &isstart_;
    }

    inline bool* get_isstop_(){
        return &isstop_;
    }

signals:
    void Success_Signal();
    void Failed_Signal();

public slots:
    void Capture_Slot(const QString,const QString);

private slots:
    void Success_Slot();
    void Fail_Slot();
    void Start_Slot();
    void Stop_Slot();

private:
    void UpdateTime();
    void Capturing();
    void SetPreparingMode();
    void SetCapturingMode();
};

#endif // CAPTUREDIALOG_H
