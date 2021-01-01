#ifndef CUSPROGRESS_H
#define CUSPROGRESS_H

#include <QWidget>
#include <QProgressBar>
#include <QTimer>
#include <QTimerEvent>
#include <QMutex>

#include<QElapsedTimer>

class Task : public QObject
{
    Q_OBJECT
public:
    int startValue;
    //结束时进度的值
    int endValue;
    //定时器运行时间
    int duration;
    //是否控制进度条消失
    bool controlVisible;
};

class CusProgress : public QProgressBar
{
    Q_OBJECT
public:
    explicit CusProgress(QWidget *parent = Q_NULLPTR);
    ~CusProgress();
    void activate(int startValue, int endValue, int duration, bool isVisible = false);

protected:
    void timerEvent(QTimerEvent *event);

private slots:
    void updateState();

private:
    //进度值增加
    QString m_plusStyle;
    QString m_minusStyle;
    QTimer *m_timer;
    //初始进度值
    int m_startValue;
    //结束时进度的值
    int m_endValue;
    //定时器运行时间
    int m_duration;
    //计算定时器当前已经耗时
    float m_consume;
    float m_msec;
    //是否控制进度条消失
    bool m_controlVisible;

    bool m_visiable;
    QMutex m_mutex;

    QList<Task*> m_listTask;

    QElapsedTimer mstimer;     // 定义对象
};

#endif // CUSPROGRESS_H
