#ifndef OBJECTDIALOG_H
#define OBJECTDIALOG_H

#include <QDialog>

class ObjectInfoWidget;
class TitleButtons;
class TransformWidget;
class QCheckBox;
class QPropertyAnimation;
class QParallelAnimationGroup;
class QListWidgetItem;

namespace Ui {
class ObjectDialog;
}

class ObjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ObjectDialog(QWidget *parent = 0);
    ~ObjectDialog();

private:
    Ui::ObjectDialog *ui;

    ObjectInfoWidget* pinfo_widget_;
    QPushButton* pdelete_btn_;
    QCheckBox* pplay_btn_;
    QCheckBox* ptransform_btn_;
    TitleButtons* ptitle_btns_;
    TransformWidget* ptransform_widget_;

    QPropertyAnimation* ptrans_animation_;
    QPropertyAnimation* panimation_;
    QParallelAnimationGroup* panigroup_;

    QString currentpath_;

    bool isstop_;

signals:
    void Delete_Signal(const QString);

public:
    void Initialize();
    void Reset();
    void UpdateTranslation();
    void UpdateObjectInfo(const QString path);

private slots:
    void Delete_Slot();
    void SetTransformEnable_Slot(bool);
    void Play_Slot(bool);
    void Transform_Slot(const QString,const QString);

private:
    void Play();
};

#endif // OBJECTDIALOG_H
