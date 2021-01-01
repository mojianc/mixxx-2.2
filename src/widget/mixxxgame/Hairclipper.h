#ifndef HAIRCLIPPER_H
#define HAIRCLIPPER_H

#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QLayout>
#include <QTimerEvent>
#include <QTimer>
#include <QMutex>

enum Direction {
    ArrowUp,
    ArrowDown
};

class HairclipperTask : public QObject
{
    Q_OBJECT
public:
    int currentValue;
    int maxValue;
    int duration;
    Qt::Orientation ori;
    bool controlVisible;
    Direction direction;
};

class UpHairclipper : public QWidget
{
    Q_OBJECT
public:
    explicit UpHairclipper(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    void setValue(int value);
    int value();

private:
    void initLabel(QLabel *label, QString res);
    void initSlider(QString glide, QString original, QString handle);

private:
    QSlider *m_slider;
    QLabel *m_upArrow;
};

class LeftRightHairclipper : public QWidget
{
    Q_OBJECT
public:
    explicit LeftRightHairclipper(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    void setValue(int value);
    int value();

private:
    void initLabel(QLabel *label, QString res);
    void initSlider(QString glide, QString original, QString handle);

private:
    QSlider *m_slider;
    QLabel *m_leftArrow;
    QLabel *m_rightArrow;
};

class Hairclipper : public QWidget
{
    Q_OBJECT
public:
    explicit Hairclipper(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~Hairclipper();

    void setOrientation(Qt::Orientation ori);
    void activate(int currentValue, int maxValue, int nDuration, Qt::Orientation ori, bool isVisible = false, Direction direction = Direction::ArrowDown);

private slots:
    void  updateState();

protected:
    void timerEvent(QTimerEvent *event);

private:
    Qt::Orientation m_orientation;
    Direction m_direction;

    QList<QLayout*> m_listLayout;
    QList<HairclipperTask*> m_listTask;
    HairclipperTask *m_task;
    int m_msec;
    //计算定时器当前已经耗时
    int m_consume;

    bool m_visiable;

    QTimer *m_timer;
    QMutex m_mutex;

    LeftRightHairclipper *m_lrHairclipper;
    UpHairclipper *m_upHairclipper;
};

#endif // HAIRCLIPPER_H
