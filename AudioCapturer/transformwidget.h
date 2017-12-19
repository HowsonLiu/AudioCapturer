#ifndef TRANSFORMWIDGET_H
#define TRANSFORMWIDGET_H

#include <QWidget>

class QRadioButton;
class QLineEdit;
class QPushButton;
class QLabel;

class TransformWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TransformWidget(QWidget *parent = nullptr);

private:
    QLabel* plabel_;
    QRadioButton* pwav_rbtn_;
    QLineEdit* psave_edit_;
    QPushButton* pbrowse_btn_;
    QPushButton* pok_btn_;

signals:
    void Transform_Signal(const QString,const QString);

public:
    void Initialize();
    void UpdateTranslation();

private slots:
    void BrowsebtnClick_Slot();
    void OKClick_Slot();
};

#endif // TRANSFORMWIDGET_H
