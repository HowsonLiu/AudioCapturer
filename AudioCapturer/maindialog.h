#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>

class QListWidget;
class SettingDialog;
class NewDialog;
class QListWidgetItem;
class TitleButtons;
class QLabel;
class ObjectDialog;
class ObjectMiniWidget;

namespace Ui {
class MainDialog;
}

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = 0);
    ~MainDialog();

private:
    Ui::MainDialog *ui;
    QLabel* picon_label_;
    QLabel* ptitle_label_;
    TitleButtons* ptitle_btn_;
    QListWidget* plist_;
    QPushButton* pnew_btn_;
    QPushButton* popen_btn_;

    SettingDialog* psetting_dialog_;
    NewDialog* pnew_dialog_;
    ObjectDialog* pobject_dialog_;

public slots:
    void UpdateTranslation();

public:
    void Initialize();

private slots:
    void Setting_Slot();
    void New_Slot();
    void Open_Slot();
    void Double_Click_Object_Slot(QListWidgetItem* item);

    void AddObject_Slot(const QString path);
    void CleanObjectWithConfirm_Slot(const QString path);
    void CleanObjectWithoutConfirm_Slot(const QString path);

private:
    void UpdateList();
    ObjectMiniWidget* GetWidget(const QString path);
    QListWidgetItem* GetItem(const QString path);
};

#endif // MAINDIALOG_H
