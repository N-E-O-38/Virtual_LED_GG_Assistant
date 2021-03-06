#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    QString m_led_status;
    QString m_led_color;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    virtual void paintEvent(QPaintEvent *event);
    void setLedStatus(QString led_status);
    void setLedColor(QString led_color);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
