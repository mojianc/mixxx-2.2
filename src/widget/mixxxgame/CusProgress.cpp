#include "CusProgress.h"
#include <QPainter>
#include <QtMath>
#include <QDebug>

void CusProgress::timerEvent(QTimerEvent *event)
{
    QMutexLocker lock(&m_mutex);
    if (m_visiable && m_listTask.count() > 0)
    {
        mstimer.start();           // 调用函数
        m_visiable = false;
        Task *task = m_listTask.takeFirst();
        this->setVisible(true);
        this->setValue(task->startValue);
        m_startValue = task->startValue;
        m_endValue = task->endValue;
        m_duration = task->duration;
        m_controlVisible = task->controlVisible;
        m_msec = ((float)task->duration / (float)qAbs(task->startValue - task->endValue));
        if (m_startValue > m_endValue)
            this->setStyleSheet(m_minusStyle);
        else
            this->setStyleSheet(m_plusStyle);
        m_timer->setInterval(m_msec);
        m_timer->start();
        delete task;
    }
}

CusProgress::CusProgress(QWidget *parent)
    : QProgressBar(parent)
    , m_startValue(0)
    , m_endValue(0)
    , m_duration(0)
    , m_consume(0)
    , m_visiable(true)
{
    m_plusStyle = "QProgressBar  \
                    {    \
                        border: 2px solid #3178C4;    \
                        border-radius: 5px;    \
                        background-color: rgb(255, 255, 255, 0); \
                        text-align:center; \
                        color: transparent; \
                        height: 30px;\
                        width: 230px;\
                    } \
                    QProgressBar::chunk  \
                    {    \
                        background-color: #35679C;    \
                        border-radius: 5px;    \
                    }";
    m_minusStyle = m_plusStyle;
    this->setMinimum(0);
    this->setMaximum(100);
    this->setStyleSheet(m_plusStyle);
    this->setVisible(false);

    startTimer(10);

    m_timer = new QTimer(this);
    m_timer->setTimerType(Qt::PreciseTimer);
    connect(m_timer, &QTimer::timeout, this, &CusProgress::updateState);
}

CusProgress::~CusProgress()
{
    
}

void CusProgress::activate(int startValue, int endValue, int duration, bool isVisible)
{
    QMutexLocker lock(&m_mutex);
    Task *task = new Task;
    task->startValue = startValue;
    task->endValue = endValue;
    task->duration = duration;
    task->controlVisible = isVisible;
    m_listTask.append(task);
}

void CusProgress::updateState()
{
    QMutexLocker lock(&m_mutex);
    m_consume += m_msec;
    int value = this->value();
    this->setValue(m_startValue > m_endValue ? (value - 1) : (value+1));
    if (m_consume >= m_duration)
    {
        m_timer->stop();
        if (m_controlVisible)
            this->setVisible(false);
        m_startValue = 0;
        m_endValue = 0;
        m_consume = 0;
        m_duration = 0;
        m_visiable = true;
        m_controlVisible = false;
        float time = (double)mstimer.nsecsElapsed()/(double)1000000;
        qDebug() << QString::fromLocal8Bit("程序耗时：") << time;
    }
}
