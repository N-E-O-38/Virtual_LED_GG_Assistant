#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QPen>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setLedStatus(QString led_status)
{
    m_led_status = led_status;
}

void MainWindow::setLedColor(QString led_color){
    m_led_color = led_color;
}

void MainWindow::paintEvent(QPaintEvent *){
    QPainter ellipsePainter(this);
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(5);
    ellipsePainter.setPen(pen);
    ellipsePainter.setBrush(QColor(m_led_color));
    ellipsePainter.drawEllipse(QRect(150,150,200,200));
}
