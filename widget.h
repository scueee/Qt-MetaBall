#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QThread>
#include <QCloseEvent>

#include "worker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::Widget *ui;

    QImage image;
    QGraphicsPixmapItem *pixitem;
    Worker *worker;
    QThread workerThread;

    QImage image2;
    QGraphicsPixmapItem *pixitem2;
    Worker *worker2;
    QThread workerThread2;

    QImage imgCalcPotential(QGraphicsPixmapItem *,QImage,QGraphicsPixmapItem *,QImage);
    QImage img2Grey(QImage);

private slots:
    void on_btn_start_clicked();
    void scene_update();
    void slot_move(double, double);
    void slot_move2(double, double);

signals:
    void signal_start(double, double);
    void signal_start2(double, double);
};
#endif // WIDGET_H
