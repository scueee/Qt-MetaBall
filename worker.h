#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>
#include <QTime>
#include <QDebug>

class Worker : public QObject
{
    Q_OBJECT
public:
    Worker(QObject *parent = nullptr);
    Worker(int id);
    void stop();

public slots:
    void doWork(double, double);

private:
    bool quit;
    int random;
    const int TICKS_PER_FRAME = 1000 / 60;
    void collisionDetection(double,double,double,double,double*,double*);

signals:
    void move(double, double);
};

#endif // WORKER_H
