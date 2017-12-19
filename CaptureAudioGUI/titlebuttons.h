#ifndef TITLEBUTTONS_H
#define TITLEBUTTONS_H

#include <QWidget>

class QPushButton;
class QHBoxLayout;

class TitleButtons : public QWidget
{
    Q_OBJECT
public:
    explicit TitleButtons(QWidget *parent = nullptr);

private:
    QPushButton* pclose_btn_;
    QPushButton* pmini_btn_;
    QPushButton* psetting_btn_;
    QHBoxLayout* playout_;

signals:
    void SettingButtonClick_Signal(bool);
    void MiniButtonClick_Signal(bool);
    void CloseButtonClick_Signal(bool);

public:
    void AddCloseButton();
    void AddMiniButton();
    void AddSettingButton();
};

#endif // TITLEBUTTONS_H
