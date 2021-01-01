#include "Hairclipper.h"
#include <QImage>
#include <QPixmap>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>

Hairclipper::Hairclipper(QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent, f)
    , m_orientation(Qt::Vertical)
    , m_visiable(true)
    , m_msec(0)
    , m_consume(0)
    , m_direction(Direction::ArrowUp)
{
    setVisible(false);

    m_lrHairclipper = new LeftRightHairclipper(this);
    m_upHairclipper = new UpHairclipper(this);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(m_lrHairclipper);
    layout->addWidget(m_upHairclipper);
    layout->setMargin(0);
    setLayout(layout);

    startTimer(20);

    m_timer = new QTimer(this);
    m_timer->setTimerType(Qt::PreciseTimer);
    connect(m_timer, &QTimer::timeout, this, &Hairclipper::updateState);
}

Hairclipper::~Hairclipper()
{

}

void Hairclipper::setOrientation(Qt::Orientation ori)
{

}

void Hairclipper::activate(int currentValue, int maxValue, int duration, Qt::Orientation ori, bool isVisible, Direction direction)
{
    QMutexLocker lock(&m_mutex);
    HairclipperTask *task = new HairclipperTask;
    task->currentValue = currentValue;
    task->maxValue = maxValue;
    task->duration = duration;
    task->ori = ori;
    task->controlVisible = isVisible;
    task->direction = direction;
    m_listTask.append(task);
}

void Hairclipper::updateState()
{
    QMutexLocker lock(&m_mutex);
    m_consume += m_msec;
    if (m_task->ori == Qt::Horizontal)
    {
        int value = m_lrHairclipper->value();
        m_lrHairclipper->setValue(m_task->currentValue > 0 ? (value - 1) : (value+1));
    }
    if (m_task->ori == Qt::Vertical)
    {
        int value = m_upHairclipper->value();
        m_upHairclipper->setValue(m_task->currentValue > 0 ? (value - 1) : (value+1));
    }

    if (m_consume >= m_task->duration)
    {
        m_timer->stop();
        if (m_task->controlVisible)
            this->setVisible(false);
        m_consume = 0;
        m_visiable = true;
        delete m_task;
    }
}

void Hairclipper::timerEvent(QTimerEvent *event)
{
    QMutexLocker lock(&m_mutex);
    if (m_visiable && m_listTask.count() > 0)
    {
        m_task = m_listTask.takeFirst();
        m_direction = m_task->direction;


        m_visiable = false;
        if (m_task->ori == Qt::Horizontal)
        {
            m_lrHairclipper->setValue(m_task->currentValue);
            m_lrHairclipper->setVisible(true);
            m_upHairclipper->setVisible(false);
        }
        if (m_task->ori == Qt::Vertical)
        {
            m_upHairclipper->setValue(m_task->currentValue);
            m_upHairclipper->setVisible(true);
            m_lrHairclipper->setVisible(false);
        }
        m_msec = (float)m_task->duration / m_task->maxValue;
        qDebug() << "m_msec:" << m_msec;
        m_timer->start(m_msec);
        this->setVisible(true);
    }
}

LeftRightHairclipper::LeftRightHairclipper(QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{
    m_slider = new QSlider(this);
    m_slider->setFixedHeight(88);
    m_slider->setOrientation(Qt::Horizontal);
    initSlider(":/images/img/slider/glide.png", ":/images/img/slider/original.png", ":/images/img/slider/arrows_shu.png");

    m_leftArrow = new QLabel(this);
    initLabel(m_leftArrow, ":/images/img/slider/triangle_left.png");
    m_rightArrow = new QLabel(this);
    initLabel(m_rightArrow, ":/images/img/slider/triangle_right.png");

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addStretch();
    leftLayout->addWidget(m_leftArrow);
    leftLayout->addStretch();

    QVBoxLayout *sliderLayout = new QVBoxLayout;
    sliderLayout->addStretch();
    sliderLayout->addWidget(m_slider);
    sliderLayout->addStretch();

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addStretch();
    rightLayout->addWidget(m_rightArrow);
    rightLayout->addStretch();

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addLayout(leftLayout);
    layout->addLayout(sliderLayout);
    layout->addLayout(rightLayout);
    layout->setMargin(0);
    setLayout(layout);
}

void LeftRightHairclipper::setValue(int value)
{
    m_slider->setValue(value);
}

int LeftRightHairclipper::value()
{
    return m_slider->value();
}

void LeftRightHairclipper::initLabel(QLabel *label, QString res)
{
    QImage image(res);
    label->setPixmap(QPixmap::fromImage(image));
}

void LeftRightHairclipper::initSlider(QString glide, QString original, QString handle)
{
    QPixmap pix(handle);
    qDebug() << "width:" << pix.width() << "    height:" << pix.height();

    QString styleStr = "\
            QSlider::handle:horizontal \
            {\
                border-image:url("+handle+");\
                width:  35px; \
                height: 88px; \
                margin-top: -44px; \
                margin-left: -2px; \
                margin-bottom: -44px; \
                margin-right: -1px; \
            }\
            QSlider::groove:horizontal {\
            background: #FFD580;\
            height: 5px;\
            }\
            QSlider::sub-page:horizontal {\
            background: #FFD580;\
            }\
            QSlider::add-page:horizontal {\
            background: #FFFFFF;\
            }";
    m_slider->setStyleSheet(styleStr);
}

UpHairclipper::UpHairclipper(QWidget *parent, Qt::WindowFlags f)
{
    m_slider = new QSlider(this);
    m_slider->setFixedWidth(90);
    m_slider->setOrientation(Qt::Vertical);
    initSlider(":/images/img/slider/glide.png", ":/images/img/slider/original.png", ":/images/img/slider/arrows_heng.png");

    m_upArrow = new QLabel(this);
    initLabel(m_upArrow, ":/images/img/slider/triangle_up.png");

    QHBoxLayout *upLayout = new QHBoxLayout;
    upLayout->addStretch();
    upLayout->addWidget(m_upArrow);
    upLayout->addStretch();

    QHBoxLayout *sliderLayout = new QHBoxLayout;
    sliderLayout->addStretch();
    sliderLayout->addWidget(m_slider);
    sliderLayout->addStretch();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(upLayout);
    layout->addLayout(sliderLayout);
    layout->setSpacing(2);
    layout->setMargin(0);
    setLayout(layout);
}

void UpHairclipper::setValue(int value)
{
    m_slider->setValue(value);
}

int UpHairclipper::value()
{
    return m_slider->value();
}

void UpHairclipper::initLabel(QLabel *label, QString res)
{
    QImage image(res);
    label->setPixmap(QPixmap::fromImage(image).scaled(33, 24));
}

void UpHairclipper::initSlider(QString glide, QString original, QString handle)
{
    QString styleStr = "\
            QSlider::handle:vertical \
            {\
                border-image:url("+handle+");\
                width:  55px; \
                height: 22px; \
                margin-top: -1px; \
                margin-left: -22px; \
                margin-bottom: -2px; \
                margin-right: -22px; \
            }\
            QSlider::groove:vertical {\
            background: #FFD580;\
            width: 5px;\
            }\
            QSlider::sub-page:vertical {\
            background: #FFD580;\
            }\
            QSlider::add-page:vertical {\
            background: #FFFFFF;\
            }";
    m_slider->setStyleSheet(styleStr);
}
