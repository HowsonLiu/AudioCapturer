#ifndef OBJECTMINIWIDGET_H
#define OBJECTMINIWIDGET_H

#include <QWidget>

class QLabel;
class QPushButton;

class ObjectMiniWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ObjectMiniWidget(const QString filepath_, QWidget *parent = nullptr);

private:
    QLabel* pname_label_;
    QPushButton* pdelete_btn_;

    QString filepath_;

signals:
    void DeletebtnClick_Signal(const QString);

public:
    inline const QString get_filepath(){
        return filepath_;
    }
};

#endif // OBJECTMINIWIDGET_H
