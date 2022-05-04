#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    QGraphicsScene *scene = new QGraphicsScene;
    ui->view->setScene(scene);
    ui->view->show();
    connect(scene, &QGraphicsScene::changed, this, &Widget::scene_update);
    qsrand(uint(QTime::currentTime().second())); // 设定随机数种子

    // 导入图片
    image = QImage(":/img/img/red.png");
    pixitem = new QGraphicsPixmapItem;
    pixitem->setPixmap(QPixmap::fromImage(image));
    pixitem->setFlag(QGraphicsItem::ItemIsMovable);
    pixitem->setPos(qrand()%778,qrand()%521);
    scene->addItem(pixitem);
    image2 = QImage(":/img/img/blue.png");
    pixitem2 = new QGraphicsPixmapItem;
    pixitem2->setPixmap(QPixmap::fromImage(image2));
    pixitem2->setFlag(QGraphicsItem::ItemIsMovable);
    pixitem2->setPos(qrand()%778,qrand()%521);
    scene->addItem(pixitem2);

    // 开启多线程
    worker = new Worker(qrand()); // 生成随机数作为线程ID
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &Widget::signal_start, worker, &Worker::doWork);
    connect(worker, &Worker::move, this, &Widget::slot_move);
    workerThread.start();
    worker2 = new Worker(qrand());
    worker2->moveToThread(&workerThread2);
    connect(&workerThread2, &QThread::finished, worker2, &QObject::deleteLater);
    connect(this, &Widget::signal_start2, worker2, &Worker::doWork);
    connect(worker2, &Worker::move, this, &Widget::slot_move2);
    workerThread2.start();
}

Widget::~Widget()
{
    worker->stop();
    workerThread.quit();
    workerThread.wait();
    worker2->stop();
    workerThread2.quit();
    workerThread2.wait();
    delete ui;
}

void Widget::on_btn_start_clicked(){
    emit signal_start(pixitem->x(),pixitem->y());
    emit signal_start2(pixitem2->x(),pixitem2->y());
}

void Widget::slot_move(double x, double y){
    pixitem->setPos(x,y);
}

void Widget::slot_move2(double x, double y){
    pixitem2->setPos(x,y);
}

// MetaBall实现
void Widget::scene_update(){
    int R = 50;
    double x = pixitem->x()+R;double y = pixitem->y()+R;
    double x2 = pixitem2->x()+R;double y2 = pixitem2->y()+R;
    auto convert = [](int color,double light){double tmp=color*light;return int(tmp)%255;};
    QRgb *line;QRgb *line2;QRgb *new_line;QRgb *new_line2;
    QImage new_image = image;QImage new_image2 = image2;
    for(int py=0;py<2*R;py++){
        line = reinterpret_cast<QRgb *>(image.scanLine(py));
        line2 = reinterpret_cast<QRgb *>(image2.scanLine(py));
        new_line = reinterpret_cast<QRgb *>(new_image.scanLine(py));
        new_line2 = reinterpret_cast<QRgb *>(new_image2.scanLine(py));
        for(int px=0;px<2*R;px++){
            // metaball公式与万有引力公式相近 f(x,y)=R/sqrt((X-x)^2+(Y-y)^2)
            int dx=R-px;int dy=R-py;
            // 计算权重
            double light = R/std::sqrt(dx*dx+dy*dy);
            double light2 = R/std::sqrt((x-x2-dx)*(x-x2-dx)+(y-y2-dy)*(y-y2-dy));
            double light3 = R/std::sqrt((x2-x-dx)*(x2-x-dx)+(y2-y-dy)*(y2-y-dy));
            double sum = light + light2;
            double sum2 = light + light3;
            // 上色
            QColor color = QColor(line[px]);QColor color2 = QColor(line2[px]);
            new_line[px] = qRgb(convert(color.red(),sum),convert(color.green(),sum),convert(color.yellow(),sum));
            new_line2[px] = qRgb(convert(color2.red(),sum2),convert(color2.green(),sum2),convert(color2.yellow(),sum2));
        }
    }
    // 渲染
    pixitem->setPixmap(QPixmap::fromImage(new_image));
    pixitem2->setPixmap(QPixmap::fromImage(new_image2));
}

// QImage转灰度图
QImage Widget::img2Grey(QImage img){
    int height = img.height();
    int width = img.width();
    qDebug()<<"width:"<<width<<"height:"<<height;
    QRgb *line;
    for(int i=0;i<height;i++){
        line = reinterpret_cast<QRgb *>(img.scanLine(i));
        for(int j=0;j<width;j++){
//            qDebug()<<QString::number(*line,16);
//            qDebug()<<hex<<*line;
            QColor color = QColor(line[j]);
            if(color.red()!=0 || color.blue()!=0 || color.green()!=0){
                int av = int(color.red()*0.3+color.green()*0.6+color.blue()*0.1);
                line[j] = qRgb(av,av,av);
            }
        }
    }
    return img;
}

void Widget::closeEvent(QCloseEvent *event)
{
    qDebug()<<"ending";
    event->accept();
}
