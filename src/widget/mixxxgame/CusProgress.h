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
    //����ʱ���ȵ�ֵ
    int endValue;
    //��ʱ������ʱ��
    int duration;
    //�Ƿ���ƽ�������ʧ
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
    //����ֵ����
    QString m_plusStyle;
    QString m_minusStyle;
    QTimer *m_timer;
    //��ʼ����ֵ
    int m_startValue;
    //����ʱ���ȵ�ֵ
    int m_endValue;
    //��ʱ������ʱ��
    int m_duration;
    //���㶨ʱ����ǰ�Ѿ���ʱ
    float m_consume;
    float m_msec;
    //�Ƿ���ƽ�������ʧ
    bool m_controlVisible;

    bool m_visiable;
    QMutex m_mutex;

    QList<Task*> m_listTask;

    QElapsedTimer mstimer;     // �������
};

#endif // CUSPROGRESS_H
