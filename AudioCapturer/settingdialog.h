#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;
class QRadioButton;

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = 0);
    ~SettingDialog();

private:
    Ui::SettingDialog *ui;

    QLabel* pdiv_label_;
    QLabel* plangage_label_;
    QLineEdit* pdiv_edit_;
    QPushButton* pbrowse_btn_;
    QRadioButton* pchinese_rbtn_;
    QRadioButton* penglish_rbtn_;
    QPushButton* pcancel_btn_;
    QPushButton* pok_btn_;

    bool english_status_;
    bool chinese_status_;

signals:
    void Translate();

public:
    void UpdateTranslation();
    void Initialize();
    void Reset();

public slots:
    void Save_Slot();
    void Browse_Slot();
    void Cancal_Slot();
};

#endif // SETTINGDIALOG_H
