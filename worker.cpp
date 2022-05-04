#include "worker.h"
#include <math.h>

Worker::Worker(QObject *parent) : QObject(parent)
{
}

Worker::Worker(int id){
    random = id;
}

void Worker::doWork(double x, double y){
    quit = false;
    QTime time;
    time.start();
    double angle = random % 90 * 3.14 / 180;
    double dx = 100/15 * (random%2*2-1) * cos(angle);
    double dy = 100/15 * (random%2*2-1) * sin(angle);
    while(!quit){
        int time1 = time.elapsed();
        emit move(x,y);
        collisionDetection(x+50,y+50,478-11,421-11,&dx,&dy);
        x += dx;
        y += dy;
        int time2 = time.elapsed();
        int sleep_time = TICKS_PER_FRAME-(time2-time1);
        if(sleep_time>0)
            QThread::msleep(static_cast<unsigned long>(sleep_time));
    }
}

void Worker::stop(){
    quit = true;
}

void Worker::collisionDetection(double x, double y, double w, double h, double *dx, double *dy){
    if(x>=0 && x<=w){
        if(y>=0 && y<=h)
            return;
        else
            *dy = -*dy;
    }
    else
        *dx = -*dx;
}
