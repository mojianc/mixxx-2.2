// skinloader.cpp
// Created 6/21/2010 by RJ Ryan (rryan@mit.edu)

#include "skin/skinloader.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QDir>
#include <QString>
#include <QtDebug>
#include <QVector>
#include <QtMath>
#include <QtWidgets/qwidget.h>

#include "vinylcontrol/vinylcontrolmanager.h"
#include "skin/legacyskinparser.h"
#include "controllers/controllermanager.h"
#include "library/library.h"
#include "effects/effectsmanager.h"
#include "mixer/playermanager.h"
#include "util/debug.h"
#include "skin/launchimage.h"
#include "util/timer.h"
#include "recording/recordingmanager.h"
#include "mixxx.h"
#include "FtTask.h"

//����
#define �� 3.1415926
const float Radian0 = 0;
const float Radian30 = �� / 6;
const float Radian60 = �� / 3;
const float Radian90 = �� / 2;

const float Radian120 = 2 * �� / 3;
const float Radian150 = 5 * �� / 6;
const float Radian180 = ��;

const float Radian181 = -��;
const float Radian210 = -5 * �� / 6;
const float Radian240 = -2 * �� / 3;
const float Radian270 = -�� / 2;


const float Radian300 = -�� / 3;
const float Radian330 = -�� / 6;
const float Radian360 = -0;

SkinLoader::SkinLoader(UserSettingsPointer pConfig)
    : QObject(NULL)
    , m_widgetType(type_None)
    ,m_timeOutB3(1000)
    ,m_timeOutB4(1000)
    ,m_timeOutB5(1000)
    ,m_timeOutB9(1000)
    ,m_timeOutB10(1000)
    ,m_timeOutB11(1000)
    ,m_timeOutB17(1000)
    ,m_timeOutB19(1000)
    ,m_timeOutB21(1000)
    ,m_timeOutB23(1000)
    ,m_timeOutB18(1000)
    ,m_timeOutB20(1000)
    ,m_timeOutB22(1000)
    ,m_timeOutB24(1000)
    ,m_timeOutB25(true)
    ,m_bLongPress(false)
    ,m_bRightClick(false)
    ,m_pConfig(pConfig) {
    m_musicBtControl = new MusicButtonControl(this);

    m_serialPort = new SerialPort;

    m_lightB3 = false;
    m_timeB3 = new QTimer(this);
    connect(m_timeB3, SIGNAL(timeout()), this, SLOT(handleTimeoutB3()));
    m_timeB3->setSingleShot(true);
    m_timeB3->start(m_timeOutB3);

    m_lightB4 = false;
    m_timeB4 = new QTimer(this);
    connect(m_timeB4, SIGNAL(timeout()), this, SLOT(handleTimeoutB4()));
    m_timeB4->setSingleShot(true);
    m_timeB4->start(m_timeOutB4);

    m_lightB5 = false;
    m_timeB5 = new QTimer(this);
    connect(m_timeB5, SIGNAL(timeout()), this, SLOT(handleTimeoutB5()));
    m_timeB5->setSingleShot(true);
    m_timeB5->start(m_timeOutB5);

    m_lightB9 = false;
    m_timeB9 = new QTimer(this);
    connect(m_timeB9, SIGNAL(timeout()), this, SLOT(handleTimeoutB9()));
    m_timeB9->setSingleShot(true);
    m_timeB9->start(m_timeOutB9);

    m_lightB10 = false;
    m_timeB10 = new QTimer(this);
    connect(m_timeB10, SIGNAL(timeout()), this, SLOT(handleTimeoutB10()));
    m_timeB10->setSingleShot(true);
    m_timeB10->start(m_timeOutB10);

    m_lightB11 = false;
    m_timeB11 = new QTimer(this);
    connect(m_timeB11, SIGNAL(timeout()), this, SLOT(handleTimeoutB11()));
    m_timeB11->setSingleShot(true);
    m_timeB11->start(m_timeOutB11);

    m_lightB17 = false;
    m_timeB17 = new QTimer(this);
    connect(m_timeB17, SIGNAL(timeout()), this, SLOT(handleTimeoutB17()));
    m_timeB17->setSingleShot(true);
    m_timeB17->start(m_timeOutB17);

    m_lightB19 = false;
    m_timeB19 = new QTimer(this);
    connect(m_timeB19, SIGNAL(timeout()), this, SLOT(handleTimeoutB19()));
    m_timeB19->setSingleShot(true);
    m_timeB19->start(m_timeOutB19);

    m_lightB21 = false;
    m_timeB21 = new QTimer(this);
    connect(m_timeB21, SIGNAL(timeout()), this, SLOT(handleTimeoutB21()));
    m_timeB21->setSingleShot(true);
    m_timeB21->start(m_timeOutB21);

    m_lightB23 = false;
    m_timeB23 = new QTimer(this);
    connect(m_timeB23, SIGNAL(timeout()), this, SLOT(handleTimeoutB23()));
    m_timeB23->setSingleShot(true);
    m_timeB23->start(m_timeOutB23);

    m_lightB18 = false;
    m_timeB18 = new QTimer(this);
    connect(m_timeB18, SIGNAL(timeout()), this, SLOT(handleTimeoutB18()));
    m_timeB18->setSingleShot(true);
    m_timeB18->start(m_timeOutB18);

    m_lightB20 = false;
    m_timeB20 = new QTimer(this);
    connect(m_timeB20, SIGNAL(timeout()), this, SLOT(handleTimeoutB20()));
    m_timeB20->setSingleShot(true);
    m_timeB20->start(m_timeOutB20);

    m_lightB22 = false;
    m_timeB22 = new QTimer(this);
    connect(m_timeB22, SIGNAL(timeout()), this, SLOT(handleTimeoutB22()));
    m_timeB22->setSingleShot(true);
    m_timeB22->start(m_timeOutB22);

    m_lightB24 = false;
    m_timeB24 = new QTimer(this);
    connect(m_timeB24, SIGNAL(timeout()), this, SLOT(handleTimeoutB24()));
    m_timeB24->setSingleShot(true);
    m_timeB24->start(m_timeOutB24);

    m_lightB25 = false;
    m_timeB25 = new QTimer(this);
    connect(m_timeB25, SIGNAL(timeout()), this, SLOT(handleTimeoutB25()));
    m_timeB25->setSingleShot(true);
    m_timeB25->start(m_timeOutB25);

    m_lightB37 = false;
    m_timeB37 = new QTimer(this);
    connect(m_timeB37, SIGNAL(timeout()), this, SLOT(handleTimeoutB37()));
    m_timeB37->setSingleShot(true);
    m_timeB37->start(2000);

    m_lightB51 = false;
    m_timeB51 = new QTimer(this);
    connect(m_timeB51, SIGNAL(timeout()), this, SLOT(handleTimeoutB51()));
    m_timeB51->setSingleShot(true);
    m_timeB51->start(2000);


    m_timeLongPress = new QTimer(this);
    connect(m_timeLongPress, SIGNAL(timeout()), this, SLOT(handleTimeoutLongPress()));
    m_timeLongPress->setSingleShot(true);

    m_timeRightClick = new QTimer(this);
    connect(m_timeRightClick,SIGNAL(timeout()), this, SLOT(handleTimeoutRightClick()));
    m_timeRightClick->setSingleShot(true);

    //��ʼ״̬����
    //��D12,D17,D151,D157
    unsigned char result = FtTask::getInstance()->getBuff(1);
    result |= (1 << 3);
    FtTask::getInstance()->setBuff(1, result);
    result = FtTask::getInstance()->getBuff(2);
    result |= (1 << 2);
    FtTask::getInstance()->setBuff(2, result);
    result = FtTask::getInstance()->getBuff(21);
    result |= (1 << 2);
    FtTask::getInstance()->setBuff(21, result);
    result = FtTask::getInstance()->getBuff(22);
    result |= (1 << 2);
    FtTask::getInstance()->setBuff(22, result);
    //��D24
    result = FtTask::getInstance()->getBuff(3);
    result |= (1 << 1);
     FtTask::getInstance()->setBuff(3, result);
    //��D18,D19 ��VOL����-A2
    result = FtTask::getInstance()->getBuff(2);
    result |= (1 << 3);
    result |= (1 << 4);
    FtTask::getInstance()->setBuff(2, result);
   //��D25,D26,D27 ��ת��-A3
    result = FtTask::getInstance()->getBuff(3);
    result |= (1 << 2);
    result |= (1 << 3);
    result |= (1 << 4);
    FtTask::getInstance()->setBuff(3, result);
    //����D178,D179,D180 ��ת��-A9
    result = FtTask::getInstance()->getBuff(25);
    result |= (1 << 1);
    result |= (1 << 2);
    result |= (1 << 3);
    FtTask::getInstance()->setBuff(25, result);
    //����D78,D79,D80��D81 master����
    setLED_ON(10,7);
    setLED_ON(11,0);
    setLED_ON(11,1);
    setLED_ON(11,2);
    //����D91,D93,D94��D95 headphone����
    setLED_ON(12,6);
    setLED_ON(13,0);
    setLED_ON(13,1);
    setLED_ON(13,2);
    //����D128,D121��D122 ˮƽ�����Ӽ�L,R��ĸ���ʶ
    setLED_ON(18,1);
    setLED_ON(17,0);
    setLED_ON(17,1);
    //����D1,D2��D3,D211,D212,D213,D214,D134,D135,D136, ָʾ��
    setLED_ON(0,0);
    setLED_ON(0,1);
    setLED_ON(0,2);
    setLED_ON(13,6);
    setLED_ON(15,6);
    setLED_ON(17,6);
    setLED_ON(19,6);
    setLED_ON(18,7);
    setLED_ON(19,0);
    setLED_ON(19,1);
    led_update();

}

SkinLoader::~SkinLoader() {
    LegacySkinParser::freeChannelStrings();
}

QList<QDir> SkinLoader::getSkinSearchPaths() const {
    QList<QDir> searchPaths;
    // If we can't find the skins folder then we can't load a skin at all. This
    // is a critical error in the user's Mixxx installation.
    QDir skinsPath(m_pConfig->getResourcePath());
    if (!skinsPath.cd("skins")) {
        reportCriticalErrorAndQuit("Skin directory does not exist: " +
                                   skinsPath.absoluteFilePath("skins"));
    }
    searchPaths.append(skinsPath);

    QDir developerSkinsPath(m_pConfig->getResourcePath());
    if (developerSkinsPath.cd("developer_skins")) {
        searchPaths.append(developerSkinsPath);
    }

    return searchPaths;
}

void SkinLoader::loadConfigCoordinate()
{
    //���������ļ�

    m_mapRects.clear();
    QString skinPath = getConfiguredSkinPath();
    QSettings *iniSetting = new QSettings(skinPath + "/coordinate.ini", QSettings::IniFormat);
    QStringList groups = iniSetting->childGroups();
    for (int i=0; i<groups.size(); ++i)
    {
        QString strGroup = groups.at(i);
        iniSetting->beginGroup(strGroup);
        QRect rect;
        int x1 =  iniSetting->value("x1").toInt();
        int x2 =  iniSetting->value("x2").toInt();
        int y1 =  iniSetting->value("y1").toInt();
        int y2 =  iniSetting->value("y2").toInt();
        rect.setTopLeft(QPoint(x1, y1));
        rect.setBottomRight(QPoint(x2, y2));
        iniSetting->endGroup();

        if(strGroup == "KnobComposed_scale")
        {
            m_knobScale = rect;
        }
        else
        {
            m_mapRects.insert(strGroup, rect);
        }
    }
}

void SkinLoader::connectHid(ControllerManager *pControllerManager)
{
    QList<Controller *> contrllerList = pControllerManager->getControllerList(false, true);
    for(int i = 0; i < contrllerList.count(); i++)
    {
        Controller *controller = contrllerList.at(i);
        if(controller->getControllerId() == (0x222A + 0x0001))
        {
            if(!controller->isOpen())
            {
                controller->open();
            }
            connect(controller, SIGNAL(incomingData(QByteArray)), this, SLOT(getComingData(QByteArray)), Qt::UniqueConnection);
        }
    }

}

void SkinLoader::setVideoWidget(VideoWidget *widget)
{
    m_videoWidget = widget;
}

void SkinLoader::internalDealWithLED(float ratio, QTimer* timer, int* internal, int group1, int byte11, int byte12, int group2, int byte21) {
    if(ratio < 0.1)
    {
        unsigned char result = FtTask::getInstance()->getBuff(group1);
        result &= ~(1 << byte11);
        result &= ~(1 << byte12);
        FtTask::getInstance()->setBuff(group1, result);
        result = FtTask::getInstance()->getBuff(group2);
        result &= ~(1 << byte21);
        FtTask::getInstance()->setBuff(group2, result);
        FtTask::getInstance()->update();
        if(timer->isActive())
        {
            timer->stop();
        }
    }
    else
    {
        if(ratio < 0.3)
        {
            *internal = 1000;
        }
        else if(ratio < 0.6)
        {
            *internal = 200;
        }
        else
        {
            *internal = 50;
        }
        timer->stop();
        timer->setInterval(*internal);
        timer->start(*internal);
    }
}

SerialPort* SkinLoader::getSerialPort()
{
    return m_serialPort;
}
#include "widget\wspinny.h"
#include "widget\wknobcomposed.h"
#include "widget\wpushbutton.h"
#include "widget\wslidercomposed.h"
#include <QPoint>
#include <QObject>
#include <iostream>
#include <math.h>
#include<windows.h>>
void SkinLoader::dealWithLED(WidgetType type, QString objName, int x, int y, QRect rct, QWidget *widget)
{

    QPoint centerPoint = rct.center();
    QPoint point = QPoint(x - centerPoint.x(), centerPoint.y() - y);
    float  radian = 0;
    switch (type) {
    //ת��
    case type_WSpinny:
        //һȦ26���ƣ�˳ʱ������Ϊ��D38~D32;D31~D28,D233,D76;D73,D47~D44;D43~D39
        //30������
        //���㷴����ֵ
        radian = qAtan2(point.y(), point.x() );
        //A3��
        if(objName == "SpinnySingletonNoCover[Channel1]_handh")
        {

            //��D32,D33
            if(radian > Radian0 && radian < Radian30)
            {
                unsigned char result = FtTask::getInstance()->getBuff(4);
                result |= (1 << 3);
                result |= (1 << 4);
                FtTask::getInstance()->setBuff(4, result);
                //��D34,D35
                 result = FtTask::getInstance()->getBuff(4);
                 result &= ~(1 << 5);
                 result &= ~(1 << 7);
                 FtTask::getInstance()->setBuff(4, result);
                 FtTask::getInstance()->update();
            }

            //��D34,D35
            else if(radian > Radian30 && radian < Radian60)
            {
                //��D32,D33
                unsigned char result = FtTask::getInstance()->getBuff(4);
                result &= ~(1 << 3);
                result &= ~(1 << 4);
                FtTask::getInstance()->setBuff(4, result);

                //��D34,D35
                result = FtTask::getInstance()->getBuff(4);
                result |= (1 << 5);
                result |= (1 << 7);
                FtTask::getInstance()->setBuff(4, result);
                //��D36,D37,D38
                result = FtTask::getInstance()->getBuff(4);
                result &= ~(1 << 6);
                FtTask::getInstance()->setBuff(4, result);
                result = FtTask::getInstance()->getBuff(5);
                result &= ~(1 << 0);
                result &= ~(1 << 1);
                 FtTask::getInstance()->setBuff(5, result);
                FtTask::getInstance()->update();
            }
            //��D36,D37,D38
            else if(radian > Radian60 && radian < Radian90)
            {
               //��D34,D35
                unsigned char result = FtTask::getInstance()->getBuff(4);
                result &= ~(1 << 5);
                result &= ~(1 << 7);
                FtTask::getInstance()->setBuff(4, result);
                //��D36,D37,D38
                result = FtTask::getInstance()->getBuff(4);
                result |= (1 << 6);
                FtTask::getInstance()->setBuff(4, result);
                result = FtTask::getInstance()->getBuff(5);
                result |= (1 << 0);
                result |= (1 << 1);
                FtTask::getInstance()->setBuff(5, result);
                //��D39,D40
                 result = FtTask::getInstance()->getBuff(5);
                 result &= ~(1 << 2);
                 result &= ~(1 << 3);
                 FtTask::getInstance()->setBuff(5, result);
                 FtTask::getInstance()->update();
            }
            //��D39,D40
            else if(radian > Radian90 && radian < Radian120)
            {
                //��D36,D37,D38
                 unsigned char result = FtTask::getInstance()->getBuff(4);
                 result &= ~(1 << 6);
                 FtTask::getInstance()->setBuff(4, result);
                 result = FtTask::getInstance()->getBuff(5);
                 result &= ~(1 << 0);
                 result &= ~(1 << 1);
                 FtTask::getInstance()->setBuff(5, result);
                 //��D39,D40
                 result = FtTask::getInstance()->getBuff(5);
                 result |= (1 << 2);
                 result |= (1 << 3);
                 FtTask::getInstance()->setBuff(5, result);
                 //��D41,D42
                  result = FtTask::getInstance()->getBuff(5);
                  result &= ~(1 << 4);
                  result &= ~(1 << 5);
                  FtTask::getInstance()->setBuff(5, result);
                  FtTask::getInstance()->update();
             }
            //��D41,D42
            else if(radian > Radian120 && radian < Radian150)
            {
                //��D39,D40
                 unsigned char result = FtTask::getInstance()->getBuff(5);
                 result &= ~(1 << 2);
                 result &= ~(1 << 3);
                 FtTask::getInstance()->setBuff(5, result);
                 //��D41,D42
                 result = FtTask::getInstance()->getBuff(5);
                 result |= (1 << 4);
                 result |= (1 << 5);
                 FtTask::getInstance()->setBuff(5, result);
                 //��D43
                 result = FtTask::getInstance()->getBuff(6);
                 result &= ~(1 << 0);
                 FtTask::getInstance()->setBuff(6, result);
                 FtTask::getInstance()->update();
            }
            //��D43
            else if(radian > Radian150 && radian < Radian180)
            {
                //��D41,D42
                 unsigned char result = FtTask::getInstance()->getBuff(5);
                 result &= ~(1 << 4);
                 result &= ~(1 << 5);
                 FtTask::getInstance()->setBuff(5, result);
                 //��D43
                 result = FtTask::getInstance()->getBuff(6);
                 result |= (1 << 0);
                 FtTask::getInstance()->setBuff(6, result);
                 //��D44,D45
                  result = FtTask::getInstance()->getBuff(6);
                  result &= ~(1 << 1);
                  result &= ~(1 << 1);
                  FtTask::getInstance()->setBuff(6, result);
                  FtTask::getInstance()->update();
            }
            //��D44,D45
            else if(radian > Radian181 && radian < Radian210)
            {
                //��D43
                 unsigned char result = FtTask::getInstance()->getBuff(6);
                 result &= ~(1 << 0);
                 FtTask::getInstance()->setBuff(6, result);
                 //��D44,D45
                 result = FtTask::getInstance()->getBuff(6);
                 result |= (1 << 1);
                 result |= (1 << 2);
                 FtTask::getInstance()->setBuff(6, result);
                 //��D46,D47
                  result = FtTask::getInstance()->getBuff(6);
                  result &= ~(1 << 3);
                  result &= ~(1 << 4);
                  FtTask::getInstance()->setBuff(6, result);
                  FtTask::getInstance()->update();
            }
            //��D46,D47
            else if(radian > Radian210 && radian < Radian240)
            {
                //��D44,D45
                 unsigned char result = FtTask::getInstance()->getBuff(6);
                 result &= ~(1 << 1);
                 result &= ~(1 << 1);
                 FtTask::getInstance()->setBuff(6, result);
                 //��D46,D47
                 result = FtTask::getInstance()->getBuff(6);
                 result |= (1 << 3);
                 result |= (1 << 4);
                 FtTask::getInstance()->setBuff(6, result);
                 //��D73,D76
                 result = FtTask::getInstance()->getBuff(10);
                 result &= ~(1 << 2);
                 result &= ~(1 << 5);
                 FtTask::getInstance()->setBuff(10, result);
                 FtTask::getInstance()->update();
            }
            //��D73,D76
            else if(radian > Radian240 && radian < Radian270)
            {
                //��D46,D47
                 unsigned char result = FtTask::getInstance()->getBuff(6);
                 result &= ~(1 << 3);
                 result &= ~(1 << 4);
                 FtTask::getInstance()->setBuff(6, result);
                 //��D73,D76
                 result = FtTask::getInstance()->getBuff(10);
                 result |= (1 << 2);
                 result |= (1 << 5);
                 FtTask::getInstance()->setBuff(10, result);
                 //��D223,D28
                 result = FtTask::getInstance()->getBuff(3);
                 result &= ~(1 << 5);
                 FtTask::getInstance()->setBuff(3, result);
                 result = FtTask::getInstance()->getBuff(29);
                 result &= ~(1 << 3);
                 FtTask::getInstance()->setBuff(29, result);
                 FtTask::getInstance()->update();
            }
            //��D223,D28
            else if(radian > Radian270 && radian < Radian300)
            {
                //��D73,D76
                 unsigned char result = FtTask::getInstance()->getBuff(10);
                 result &= ~(1 << 2);
                 result &= ~(1 << 5);
                 FtTask::getInstance()->setBuff(10, result);
                 //��D223,D28
                 result = FtTask::getInstance()->getBuff(3);
                 result |= (1 << 5);
                 FtTask::getInstance()->setBuff(3, result);
                 result = FtTask::getInstance()->getBuff(29);
                 result |= (1 << 3);
                 FtTask::getInstance()->setBuff(29, result);
                 //��D29,D30
                  result = FtTask::getInstance()->getBuff(4);
                  result &= ~(1 << 0);
                  result &= ~(1 << 1);
                  FtTask::getInstance()->setBuff(4, result);
                  FtTask::getInstance()->update();
            }
            //��D29,D30
            else if(radian > Radian300 && radian < Radian330)
            {
                //��D223,D28
                unsigned char result = FtTask::getInstance()->getBuff(3);
                result &= ~(1 << 5);
                FtTask::getInstance()->setBuff(3, result);
                result = FtTask::getInstance()->getBuff(29);
                result &= ~(1 << 3);
                FtTask::getInstance()->setBuff(29, result);
                //��D29,D30
                result = FtTask::getInstance()->getBuff(4);
                result |= (1 << 0);
                result |= (1 << 1);
                FtTask::getInstance()->setBuff(4, result);
                //��D31,D32
                 result = FtTask::getInstance()->getBuff(4);
                 result &= ~(1 << 2);
                 result &= ~(1 << 3);
                 FtTask::getInstance()->setBuff(4, result);
                 FtTask::getInstance()->update();
            }
            //��D31,D32
            else if(radian > Radian330 && radian < Radian360)
            {
                //��D29,D30
                 unsigned char result = FtTask::getInstance()->getBuff(4);
                 result &= ~(1 << 0);
                 result &= ~(1 << 1);
                 FtTask::getInstance()->setBuff(4, result);
                 //��D31,D32
                 result = FtTask::getInstance()->getBuff(4);
                 result |= (1 << 2);
                 result |= (1 << 3);
                 FtTask::getInstance()->setBuff(4, result);
                 //��D33
                 result = FtTask::getInstance()->getBuff(4);
                 result &= ~(1 << 4);
                 FtTask::getInstance()->setBuff(4, result);
                 FtTask::getInstance()->update();
            }
        }
        //A9�� ��ת��
        else if(objName == "SpinnySingletonNoCover[Channel2]_handh")
        {

            unsigned char result;
            //��D166,D167
            if(radian > Radian0 && radian < Radian30)
            {
                result = FtTask::getInstance()->getBuff(23);
                result |= (1 << 3);
                result |= (1 << 4);
                FtTask::getInstance()->setBuff(23, result);
                //��D168,D169
                 result = FtTask::getInstance()->getBuff(23);
                 result &= ~(1 << 5);
                 FtTask::getInstance()->setBuff(23, result);
                 result = FtTask::getInstance()->getBuff(24);
                 result &= ~(1 << 0);
                 FtTask::getInstance()->setBuff(24, result);
                 FtTask::getInstance()->update();
            }
            //��D168,D169
            else if(radian > Radian30 && radian < Radian60)
            {
                //��D166,D167
                result = FtTask::getInstance()->getBuff(23);
                result &= ~(1 << 3);
                result &= ~(1 << 4);
                FtTask::getInstance()->setBuff(23, result);
                //��D168,D169
                result = FtTask::getInstance()->getBuff(23);
                result |= (1 << 5);
                FtTask::getInstance()->setBuff(23, result);
                result = FtTask::getInstance()->getBuff(24);
                result |= (1 << 0);
                FtTask::getInstance()->setBuff(24, result);
                //��D170,D171,D172
                 result = FtTask::getInstance()->getBuff(24);
                 result &= ~(1 << 1);
                 result &= ~(1 << 2);
                 result &= ~(1 << 3);
                 FtTask::getInstance()->setBuff(24, result);
                 FtTask::getInstance()->update();
            }
            //��D170,D171,D172
            else if(radian > Radian60 && radian < Radian90)
            {
                 //��D168,D169
                result = FtTask::getInstance()->getBuff(23);
                result &= ~(1 << 5);
                result = FtTask::getInstance()->getBuff(24);
                result &= ~(1 << 0);
                FtTask::getInstance()->setBuff(23, result);
                //��D170,D171,D172
                result = FtTask::getInstance()->getBuff(24);
                result |= (1 << 1);
                result |= (1 << 2);
                result |= (1 << 3);
                FtTask::getInstance()->setBuff(24, result);
                //��D173,D174
                 result = FtTask::getInstance()->getBuff(24);
                 result &= ~(1 << 4);
                 result &= ~(1 << 5);
                 FtTask::getInstance()->setBuff(24, result);
                 FtTask::getInstance()->update();
            }
            //��D173,D174
            else if(radian > Radian90 && radian < Radian120)
            {
                //��D170,D171,D172
                result = FtTask::getInstance()->getBuff(24);
                result &= ~(1 << 1);
                result &= ~(1 << 2);
                result &= ~(1 << 3);
                FtTask::getInstance()->setBuff(24, result);
                //��D173,D174
                result = FtTask::getInstance()->getBuff(24);
                result |= (1 << 4);
                result |= (1 << 5);
                FtTask::getInstance()->setBuff(24, result);
                //��D175,D176
                 result = FtTask::getInstance()->getBuff(24);
                 result &= ~(1 << 6);
                 result &= ~(1 << 7);
                 FtTask::getInstance()->setBuff(24, result);
                 FtTask::getInstance()->update();
            }
            //��D175,D176
            else if(radian > Radian120 && radian < Radian150)
            {    
                //��D173,D174
                result = FtTask::getInstance()->getBuff(24);
                result &= ~(1 << 4);
                result &= ~(1 << 5);
                FtTask::getInstance()->setBuff(24, result);
                //��D175,D176
                result = FtTask::getInstance()->getBuff(24);
                result |= (1 << 6);
                result |= (1 << 7);
                FtTask::getInstance()->setBuff(24, result);
                //��D177
                 result = FtTask::getInstance()->getBuff(25);
                 result &= ~(1 << 0);
                 FtTask::getInstance()->setBuff(25, result);
                 FtTask::getInstance()->update();
            }
            //��D177
            else if(radian > Radian150 && radian < Radian180)
            {
                //��D175,D176
                result = FtTask::getInstance()->getBuff(24);
                result &= ~(1 << 6);
                result &= ~(1 << 7);
                FtTask::getInstance()->setBuff(24, result);
                //��D177
                result = FtTask::getInstance()->getBuff(25);
                result |= (1 << 0);
                FtTask::getInstance()->setBuff(25, result);
                //��D225,D92
                 result = FtTask::getInstance()->getBuff(29);
                 result &= ~(1 << 5);
                 FtTask::getInstance()->setBuff(29, result);
                 result = FtTask::getInstance()->getBuff(12);
                 result &= ~(1 << 7);
                 FtTask::getInstance()->setBuff(12, result);
                 FtTask::getInstance()->update();
            }
            //��D225,D92
            else if(radian > Radian181 && radian < Radian210)
            {
                //��D177
                result = FtTask::getInstance()->getBuff(25);
                result &= ~(1 << 0);
                FtTask::getInstance()->setBuff(25, result);
                //��D225,D92
                result = FtTask::getInstance()->getBuff(29);
                result |= (1 << 5);
                FtTask::getInstance()->setBuff(29, result);
                result = FtTask::getInstance()->getBuff(12);
                result |= (1 << 7);
                FtTask::getInstance()->setBuff(12, result);
                //��D224,D158
                 result = FtTask::getInstance()->getBuff(29);
                 result &= ~(1 << 4);
                 FtTask::getInstance()->setBuff(29, result);
                 result = FtTask::getInstance()->getBuff(22);
                 result &= ~(1 << 3);
                 FtTask::getInstance()->setBuff(22, result);
                 FtTask::getInstance()->update();
            }
            //��D224,D158
            else if(radian > Radian210 && radian < Radian240)
            {
                //��D225,D92
                result = FtTask::getInstance()->getBuff(29);
                result &= ~(1 << 5);
                FtTask::getInstance()->setBuff(29, result);
                result = FtTask::getInstance()->getBuff(12);
                result &= ~(1 << 7);
                FtTask::getInstance()->setBuff(12, result);
                //��D224,D158
                result = FtTask::getInstance()->getBuff(29);
                result |= (1 << 4);
                FtTask::getInstance()->setBuff(29, result);
                result = FtTask::getInstance()->getBuff(22);
                result |= (1 << 3);
                FtTask::getInstance()->setBuff(22, result);
                //��D159,D160
                 result = FtTask::getInstance()->getBuff(22);
                 result &= ~(1 << 4);
                 result &= ~(1 << 5);
                 FtTask::getInstance()->setBuff(22, result);
                 FtTask::getInstance()->update();
            }
            //��D159,D160
            else if(radian > Radian240 && radian < Radian270)
            {
                //��D224,D158
                result = FtTask::getInstance()->getBuff(29);
                result &= ~(1 << 4);
                FtTask::getInstance()->setBuff(29, result);
                result = FtTask::getInstance()->getBuff(22);
                result &= ~(1 << 3);
                FtTask::getInstance()->setBuff(22, result);
                //��D159,D160
                result = FtTask::getInstance()->getBuff(22);
                result |= (1 << 4);
                result |= (1 << 5);
                FtTask::getInstance()->setBuff(22, result);
                //��D161,D162
                 result = FtTask::getInstance()->getBuff(22);
                 result &= ~(1 << 6);
                 result &= ~(1 << 7);
                 FtTask::getInstance()->setBuff(22, result);
                 FtTask::getInstance()->update();
            }
            //��D161,D162
            else if(radian > Radian270 && radian < Radian300)
            {
                //��D159,D160
                result = FtTask::getInstance()->getBuff(22);
                result &= ~(1 << 4);
                result &= ~(1 << 5);
                FtTask::getInstance()->setBuff(22, result);
                //��D161,D162
                result = FtTask::getInstance()->getBuff(22);
                result |= (1 << 6);
                result |= (1 << 7);
                FtTask::getInstance()->setBuff(22, result);
                //��D163,D164
                 result = FtTask::getInstance()->getBuff(23);
                 result &= ~(1 << 0);
                 result &= ~(1 << 1);
                 FtTask::getInstance()->setBuff(23, result);
                 FtTask::getInstance()->update();
            }
            //��D163,D164
            else if(radian > Radian300 && radian < Radian330)
            {
                //��D161,D162
                result = FtTask::getInstance()->getBuff(22);
                result &= ~(1 << 6);
                result &= ~(1 << 7);
                FtTask::getInstance()->setBuff(22, result);
                //��D163,D164
                result = FtTask::getInstance()->getBuff(23);
                result |= (1 << 0);
                result |= (1 << 1);
                FtTask::getInstance()->setBuff(23, result);
                //��D165
                 result = FtTask::getInstance()->getBuff(23);
                 result &= ~(1 << 2);
                 FtTask::getInstance()->setBuff(23, result);
                 FtTask::getInstance()->update();
            }
            //��D165��D166
            else if(radian > Radian330 && radian < Radian360)
            {

                //��D163,D164
                result = FtTask::getInstance()->getBuff(23);
                result &= ~(1 << 0);
                result &= ~(1 << 1);
                FtTask::getInstance()->setBuff(23, result);
                //��DD165��D166
                result = FtTask::getInstance()->getBuff(23);
                result |= (1 << 2);
                result |= (1 << 3);
                FtTask::getInstance()->setBuff(23, result);
                //��D167
                 result = FtTask::getInstance()->getBuff(23);
                 result &= ~(1 << 4);
                 FtTask::getInstance()->setBuff(23, result);
                 FtTask::getInstance()->update();
            }
        }
        break;
     //��ť
    case type_WKnobComposed:
    {
        float posW = x - rct.x();
        float ratio = posW / rct.width();
        qDebug()<<"rct:"<<rct;
        qDebug()<<"x:"<<x;
        qDebug()<<"ratio:"<<ratio;
        //B3��
        if(objName == "KnobComposed_[EffectRack1_EffectUnit1_Effect1]_meta")
        {
            //D5,D6,D205
            internalDealWithLED(ratio, m_timeB3, &m_timeOutB3, 0, 4, 5, 1, 6);
        }
        //B4��
        else if(objName == "KnobComposed_[EffectRack1_EffectUnit1_Effect2]_meta")
        {
            //D7,D8,D206
            internalDealWithLED(ratio, m_timeB4, &m_timeOutB4, 0, 6, 7, 3, 6);
        }
        //B5��
        else if(objName == "KnobComposed_[EffectRack1_EffectUnit1_Effect3]_meta")
        {
             //D9,D10,D207
            internalDealWithLED(ratio, m_timeB5, &m_timeOutB5, 1, 0, 1, 5, 6);
        }
        //B9��
        else if(objName == "KnobComposed_[EffectRack1_EffectUnit2_Effect1]_meta")
        {
             //D137,D138,D208
             internalDealWithLED(ratio, m_timeB9, &m_timeOutB9, 19, 2, 3, 7, 6);
        }
        //B10��
        else if(objName == "KnobComposed_[EffectRack1_EffectUnit2_Effect2]_meta")
        {
            //D139,D140,D209
            internalDealWithLED(ratio, m_timeB10, &m_timeOutB10, 19, 4, 5, 9, 6);
        }
        //B11��
        else if(objName == "KnobComposed_[EffectRack1_EffectUnit2_Effect3]_meta")
        {
            //D141,D142,D210
            internalDealWithLED(ratio, m_timeB11, &m_timeOutB11, 20, 0, 1, 11, 6);
        }
        //B17��
        else if(objName == "EffectKnobComposed_[Channel1]_3")
        {
            //D105,D106,D215
            internalDealWithLED(ratio, m_timeB17, &m_timeOutB17, 14, 6, 7, 21, 6);
        }
        //B19��
        else if(objName == "EffectKnobComposed_[Channel1]_2")
        {
            //D107,D108,D217
            internalDealWithLED(ratio, m_timeB19, &m_timeOutB19, 15, 0, 1, 25, 6);
        }
        //B21��
        else if(objName == "EffectKnobComposed_[Channel1]_1")
        {
            //D109,D110,D219
            internalDealWithLED(ratio, m_timeB21, &m_timeOutB21, 15, 2, 3, 29, 6);
        }
        //B23��
        else if(objName == "KnobComposed_[QuickEffectRack1_[Channel1]]_super1")
        {
            //D111,D112,D221
            internalDealWithLED(ratio, m_timeB23, &m_timeOutB23, 15, 4, 5, 29, 1);
        }
        //B18��
        else if(objName == "EffectKnobComposed_[Channel2]_3")
        {
            //D113,D114,D216
            internalDealWithLED(ratio, m_timeB18, &m_timeOutB18, 16, 0, 1, 23, 6);
        }
        //B20��
        else if(objName == "EffectKnobComposed_[Channel2]_2")
        {
            //D115,D116,D218
            internalDealWithLED(ratio, m_timeB20, &m_timeOutB20, 16, 2, 3, 27, 6);
        }
        //B22��
        else if(objName == "EffectKnobComposed_[Channel2]_1")
        {
            //D117,D118,D220
            internalDealWithLED(ratio, m_timeB22, &m_timeOutB22, 16, 4, 5, 29, 0);
        }
        //B24��
        else if(objName == "KnobComposed_[QuickEffectRack1_[Channel2]]_super1")
        {
            //D119,D120,D222
            internalDealWithLED(ratio, m_timeB24, &m_timeOutB24, 16, 6, 7, 29, 2);
        }
        break;

    }
//    //��ť
//    case type_WPushButton:
//    {
//        if(WPushButton *pushbutton = dynamic_cast<WPushButton *>(widget))
//        {
//            //�������Ƶ�״̬��Ҫ����pushbutton�ĵ��״̬������
//            //B36
//            if(objName == "DeckCue_Deck1_hotcue")
//            {
//                if(pushbutton->isPressed())
//                {
//                    //��D50,D51
//                    FtTask::getInstance()->setBuff(6, 0x40);
//                    FtTask::getInstance()->setBuff(7, 0x01);
//                    FtTask::getInstance()->update();
//                }
//               else
//                {
//                    //��D50,D51
//                    FtTask::getInstance()->setBuff(6, 0x00);
//                   FtTask::getInstance()->setBuff(7, 0x00);
//                   FtTask::getInstance()->update();
//                }

//            }
//            //B50
//            else if(objName == "DeckCue_Deck2_hotcue")
//            {
//                //��D185��D186
//                FtTask::getInstance()->setBuff(26, 0x0c);
//                FtTask::getInstance()->update();
//            }
//            //B37
//            else if(objName == "PlayToggle_Deck1_hotcue")
//            {
//                //��D52��D53
//                    FtTask::getInstance()->setBuff(7, 0x06);
//                    FtTask::getInstance()->update();
//            }
//            //B51
//            else if(objName == "PlayToggle_Deck1_hotcue")
//            {
//                //��D187��D188
//                        FtTask::getInstance()->setBuff(26, 0x30);
//                        FtTask::getInstance()->update();
//            }
//            else
//            {
//                        //mieD187,D188
//                        FtTask::getInstance()->setBuff(26, 0x00);
//                        FtTask::getInstance()->update();
//            }
//        }
//        break;
//    }
    case type_WSliderComposed:
        //A6��,������,led���м���������չ
        if(objName == "MainSliderComposed")
        {
            //���ݴ��ص��������е�λ��ȷ������
            float halfW = (rct.width() / 2);
            float pos = 0;
            if( x < rct.center().x())
            {
                 pos =  abs(rct.center().x() -x) / halfW;
                 if( (pos) < 0.01 )
                 {
                     //��127
                     setLED_OFF(18,0);
                     led_update();
                 }
                 else if(pos < 0.25)
                 {
                     //��127
                     setLED_ON(18,0);
                     //��126
                     setLED_OFF(17,5);
                     led_update();
                 }
                 else if(pos < 0.5)
                 {
                     //��126
                     setLED_ON(17,5);
                     //��125
                     setLED_OFF(17,4);
                     led_update();
                 }
                 else if(pos < 0.75)
                 {
                     //��125
                     setLED_ON(17,4);
                     //��123,D124
                     setLED_OFF(17,3);
                     setLED_OFF(17,2);
                     led_update();
                 }
                 else
                 {
                     //��123,D124
                     setLED_ON(17,3);
                     setLED_ON(17,2);
                     led_update();
                 }
            }
            if(x > rct.center().x())
            {
                 pos =  abs(x - rct.center().x()) / halfW;
                 if( (pos) < 0.01 )
                 {
                     //��129
                     setLED_OFF(18,2);
                     led_update();
                 }
                 else if(pos < 0.25)
                 {
                     //��129
                     setLED_ON(18,2);
                     //��130
                     setLED_OFF(18,3);
                     led_update();
                 }
                 else if(pos < 0.5)
                 {
                     //��130
                     setLED_ON(18,3);
                     //��131
                     setLED_OFF(18,4);
                     led_update();
                 }
                 else if(pos < 0.75)
                 {
                     //��131
                     setLED_ON(18,4);
                     //��132��D133
                     setLED_OFF(18,5);
                     setLED_OFF(18,6);
                     led_update();
                 }
                 else
                 {
                     //��132��D133
                     setLED_ON(18,5);
                     setLED_ON(18,6);
                     led_update();
                 }
            }

        }
        //A1��
        else if(objName == "RateDisplay1_handh")
        {
            float posH = fabs(y - rct.bottom())/ rct.height();
            if(posH < 0.01)
            {
                //��D14,D13,D11,D15,D16
                setLED_OFF(1, 5);
                setLED_OFF(1, 4);
                setLED_OFF(1, 2);
                setLED_OFF(2, 0);
                setLED_OFF(2, 1);
                led_update();
            }
            else if(posH < 0.2)
            {
                //��D15
                setLED_OFF(2, 0);
                //��D16
                setLED_ON(2, 1);
                led_update();
            }
           else if(posH < 0.4)
            {
                //��D14
                setLED_OFF(1, 5);
                //��D15
                setLED_ON(2, 0);
                led_update();
            }
            else if(posH < 0.6)
            {
                //��D13
                setLED_OFF(1, 4);
                //��D14
                setLED_ON(1, 5);
                led_update();
            }
            else if(posH < 0.8)
            {
                //��D11
                setLED_OFF(1, 2);
                //��D13
                setLED_ON(1, 4);
                led_update();
            }
            else
            {
                //��D11
                setLED_ON(1, 2);
                led_update();
            }
        }
        //A8��
        else if(objName == "RateDisplay2_handh")
        {
            float posH = fabs(y - rct.bottom())/ rct.height();
            if(posH < 0.01)
                {
                    //��D152,D153,D154,D155��D156
                    setLED_OFF(21, 3);
                    setLED_OFF(21, 4);
                    setLED_OFF(21, 5);
                    setLED_OFF(22, 1);
                    setLED_OFF(22, 0);
                    led_update();
                }
                else if(posH < 0.2)
                {
                    //��D155
                    setLED_OFF(22, 0);
                    //��D156
                    setLED_ON(22, 1);
                    led_update();
                }
               else if(posH < 0.4)
                {
                    //��D154
                    setLED_OFF(21, 5);
                    //��D155
                    setLED_ON(22, 0);
                    led_update();
                }
                else if(posH < 0.6)
                {
                    //��D153
                    setLED_OFF(21, 4);
                    //��D154
                    setLED_ON(21, 5);
                    led_update();
                }
                else if(posH < 0.8)
                {
                    //��D152
                    setLED_OFF(21, 3);
                    //��D153
                    setLED_ON(21, 4);
                    led_update();
                }
                else
                {
                    //��D152
                    setLED_ON(21, 3);
                    led_update();
                }
        }
        //A2��
        else if(objName == "ChannelVolume_[Channel1]")
        {
            float posH = (y - rct.bottom())/ rct.height();
            if(posH < 0.01)
                {
                    //��D19,D20,D21,D22,D23
                    setLED_OFF(2, 4);
                    setLED_OFF(2, 5);
                    setLED_OFF(2, 6);
                    setLED_OFF(2, 7);
                    setLED_OFF(3, 0);
                    led_update();
                }
                else if(posH < 0.2)
                {
                    //��D22
                    setLED_OFF(2, 6);
                    //��D23
                    setLED_ON(3, 0);
                    led_update();
                }
               else if(posH < 0.4)
                {
                    //��D21
                    setLED_OFF(2, 7);
                    //��D22
                    setLED_ON(2, 6);
                    led_update();
                }
                else if(posH < 0.6)
                {
                    //��D20
                    setLED_OFF(2, 5);
                    //��D21
                    setLED_ON(2, 7);
                    led_update();
                }
                else if(posH < 0.8)
                {
                    //��D19
                    setLED_OFF(2, 4);
                    //��D20
                    setLED_ON(2, 5);
                    led_update();
                }
                else
                {
                    //��D19
                    setLED_ON(2, 4);
                    led_update();
                }
        }
        //A7��
        else if(objName == "ChannelVolume_[Channel2]")
        {
            float posH = (y - rct.bottom())/ rct.height();
            if(posH < 0.01)
                {
                    //��D144,D146,D147,D148��D149
                    setLED_OFF(20, 3);
                    setLED_OFF(20, 5);
                    setLED_OFF(20, 6);
                    setLED_OFF(20, 7);
                    setLED_OFF(21, 0);
                    led_update();
                }
                else if(posH < 0.2)
                {
                    //��D148
                    setLED_OFF(20, 7);
                    //��D149
                    setLED_ON(21, 0);
                    led_update();
                }
               else if(posH < 0.4)
                {
                    //��D147
                    setLED_OFF(20, 6);
                    //��D148
                    setLED_ON(20, 7);
                    led_update();
                }
                else if(posH < 0.6)
                {
                    //��D146
                    setLED_OFF(20, 5);
                    //��D147
                    setLED_ON(20, 6);
                    led_update();
                }
                else if(posH < 0.8)
                {
                    //��D144
                    setLED_OFF(20, 3);
                    //��D146
                    setLED_ON(20, 5);
                    led_update();
                }
                else
                {
                    //��D144
                    setLED_ON(20, 3);
                    led_update();
                }
        }


        //A4��...
        else if(objName == "RateDisplay1_handh")
        {
            float posH = fabs(x - rct.left())/ rct.width();
            if(posH < 0.01)
            {
                //��D82
                setLED_OFF(11,3);
                led_update();
            }
            else if(posH < 0.125)
            {
                //��D82
                setLED_ON(11,3);
                //��D83
                setLED_OFF(11,4);
                led_update();
            }
            else if(posH < 0.25)
            {
                //��D83
                setLED_ON(11,4);
                //��D84
                setLED_OFF(11,5);
                led_update();
            }
            else if(posH < 0.375)
            {
                //��D84
                setLED_ON(11,6);
                //��D85
                setLED_OFF(12,0);
                led_update();
            }
            else if(posH < 0.5)
            {
                //��D85
                setLED_ON(12,0);
                //��D86
                setLED_OFF(12,1);
                led_update();
            }
            else if(posH < 0.625)
            {
                //��D86
                setLED_ON(12,1);
                //��D87
                setLED_OFF(12,2);
                led_update();
            }
            else if(posH < 0.75)
            {
                //��D87
                setLED_ON(12,2);
                //��D88
                setLED_OFF(12,3);
                led_update();
            }
            else if(posH < 0.875)
            {
                //��D88
                setLED_ON(12,3);
                //��D89,D90
                setLED_OFF(12,4);
                setLED_OFF(12,5);
                led_update();
            }
            else
            {
                //��D89,D90
                setLED_ON(12,4);
                setLED_ON(12,5);
                led_update();
            }

        }
        break;
    default:
        break;
    }
}

void SkinLoader::setLED_ON(int byte, int bit)
{
    unsigned char result = FtTask::getInstance()->getBuff(byte);
    result |= (1 << bit);
    FtTask::getInstance()->setBuff(byte, result);
}

void SkinLoader::setLED_OFF(int byte, int bit)
{
    unsigned char result = FtTask::getInstance()->getBuff(byte);
    result &= ~(1 << bit);
    FtTask::getInstance()->setBuff(byte, result);
}

void SkinLoader::led_update()
{
    FtTask::getInstance()->update();
}

void SkinLoader::getComingData(QByteArray data)
{
    int  x = (data[3] << 8) + data[2];
    int  y = (data[5] << 8) + data[4];

    //���������ؼ������򣬿�(x,y)���ڵ�λ�����ĸ��ؼ�
    QMap<QString, QRect>::iterator iter = m_mapRects.begin();
    while(iter != m_mapRects.end())
    {
        QRect rct = iter.value();
        if(rct.contains(x, y))
        {
            //��ȡ�ؼ���objectName
            QString objectName = iter.key();
            if(objectName.contains("MusicCube"))
            {
                QStringList strList = objectName.split('-');
                int index = strList.at(1).toInt();
                m_musicBtControl->play(index);
                break;
            }

            if(objectName.contains("VideoNext"))
            {
                m_videoWidget->playNext();
                break;
            }

            if(objectName.contains("VideoStop"))
            {
                m_videoWidget->stop();
                break;
            }

            if(objectName.contains("shift"))
            {
                if(m_timeB25->isActive())
                {
                    m_timeB25->stop();
                }
                m_timeB25->start(3000);
                m_timeOutB25 = false;
            }

            QString objectNameTemp = objectName.replace('{','[');
            objectNameTemp = objectNameTemp.replace('}',']');
            //����objectName��ȡ�����Ӧ�Ŀؼ�
            QWidget *widget = m_mapWidget.value(objectNameTemp);

            if(WSpinny *spinny = dynamic_cast<WSpinny *>(widget))
            {
                m_widgetType = type_WSpinny;
                spinny->getComingData(data, rct);
            }
            else if(WKnobComposed *knobCompose = dynamic_cast<WKnobComposed *>(widget))
            {
                m_widgetType = type_WKnobComposed;
                knobCompose->getComingData(data, m_knobScale);
            }
            else if(WPushButton *pushbutton = dynamic_cast<WPushButton *>(widget))
            {
                m_widgetType = type_WPushButton;
                if(pushbutton->objectName() == "PlayToggle_Deck1_hotcue")
                {
                     pushbutton->setTimer(m_timeB37);
                     qDebug()<<__FUNCTION__<<"m_bLongPress";
                }
                else if(pushbutton->objectName() == "PlayToggle_Deck2_hotcue")
                {
                    pushbutton->setTimer(m_timeB51);
                    qDebug()<<__FUNCTION__<<"m_bLongPress";
                }
                pushbutton->getComingData(data, rct);
            }
            else if(WSliderComposed *sliderComposed = dynamic_cast<WSliderComposed *>(widget))
            {
                 m_widgetType = type_WSliderComposed;
                sliderComposed->getComingData(data, rct);
            }
            //��led�ư巢�ź�
            dealWithLED(m_widgetType,objectNameTemp, x, y, rct,widget);
            break;
        }
        iter++;
    }
}

void SkinLoader::internalHandleTimeout(QTimer* timer, int interval, int* light,
                                       int group1, int byte11, int byte12, int group2, int byte21) {

    if (*light) {
        unsigned char result = FtTask::getInstance()->getBuff(group1);
        result |= (1 << byte11);
        result |= (1 << byte12);
        FtTask::getInstance()->setBuff(group1, result);
        result = FtTask::getInstance()->getBuff(group2);
        result |= (1 << byte21);
        FtTask::getInstance()->setBuff(group2, result);
        FtTask::getInstance()->update();
    } else {
        unsigned char result = FtTask::getInstance()->getBuff(group1);
        result &= ~(1 << byte11);
        result &= ~(1 << byte12);
        FtTask::getInstance()->setBuff(group1, result);
        result = FtTask::getInstance()->getBuff(group2);
        result &= ~(1 << byte21);
        FtTask::getInstance()->setBuff(group2, result);
        FtTask::getInstance()->update();
    }
    *light = !(*light);

    timer->start(interval);
}

void SkinLoader::handleTimeoutB3()
{
    internalHandleTimeout(m_timeB3, m_timeOutB3, &m_lightB3, 0, 4, 5, 1, 6);

}

void SkinLoader::handleTimeoutB4()
{
    //��D7,D8,D206
    internalHandleTimeout(m_timeB4, m_timeOutB4, &m_lightB4, 0, 6, 7, 3, 6);
}

void SkinLoader::handleTimeoutB5()
{
    //��D9,D10,D207
    internalHandleTimeout(m_timeB5, m_timeOutB5, &m_lightB5, 1, 0, 1, 5, 6);
}

void SkinLoader::handleTimeoutB9()
{
    //��D137,D138,D208
    internalHandleTimeout(m_timeB9, m_timeOutB9, &m_lightB9, 19, 2, 3, 7, 6);
}

void SkinLoader::handleTimeoutB10()
{
    //��D139,D140,D209
    internalHandleTimeout(m_timeB10, m_timeOutB10, &m_lightB10, 19, 4, 5, 9, 6);
}

void SkinLoader::handleTimeoutB11()
{
    //��D141,D142,D210
    internalHandleTimeout(m_timeB11, m_timeOutB11, &m_lightB11, 20, 0, 1, 11, 6);
}

void SkinLoader::handleTimeoutB17()
{
    //��D105,D106,D215
    internalHandleTimeout(m_timeB17, m_timeOutB17, &m_lightB17, 14, 6, 7, 21, 6);
}

void SkinLoader::handleTimeoutB19()
{
    //��D107,D108,D217
    internalHandleTimeout(m_timeB19, m_timeOutB19, &m_lightB19, 15, 0, 1, 25, 6);
}

void SkinLoader::handleTimeoutB21()
{
    //��D109,D110,D219
    internalHandleTimeout(m_timeB21, m_timeOutB21, &m_lightB21, 15, 2, 3, 29, 6);
}

void SkinLoader::handleTimeoutB23()
{
    //��D111,D112,D221
    internalHandleTimeout(m_timeB23, m_timeOutB23, &m_lightB23, 15, 4, 5, 29, 1);
}

void SkinLoader::handleTimeoutB18()
{
    //��D113,D114,D216
    internalHandleTimeout(m_timeB18, m_timeOutB18, &m_lightB18, 16, 0, 1, 23, 6);
}

void SkinLoader::handleTimeoutB20()
{
    //��D115,D116,D218
    internalHandleTimeout(m_timeB20, m_timeOutB20, &m_lightB20, 16, 2, 3, 27, 6);
}

void SkinLoader::handleTimeoutB22()
{
    //��D117,D118,D220
    internalHandleTimeout(m_timeB22, m_timeOutB22, &m_lightB22, 16, 4, 5, 29, 0);
}

void SkinLoader::handleTimeoutB24()
{
     //��D119,D120,D222
    internalHandleTimeout(m_timeB24, m_timeOutB24, &m_lightB24, 16, 6, 7, 29, 2);
}

void SkinLoader::handleTimeoutB25()
{
    if(m_timeB25->isActive())
    {
        m_timeOutB25 = true;
        m_timeB25->stop();
    }
}

void SkinLoader::handleTimeoutB37()
{
    internalHandleTimeout(m_timeB37, 2000, &m_lightB37, 7, 1, 2, 7, 2);
    qDebug()<<__FUNCTION__<<"m_bLongPress";
}

void SkinLoader::handleTimeoutB51()
{
    internalHandleTimeout(m_timeB51, 2000, &m_lightB51, 26, 4,5, 26, 4);
    qDebug()<<__FUNCTION__<<"m_bLongPress";
}

void SkinLoader::handleTimeoutLongPress()
{
    if(m_timeLongPress->isActive())
    {
        m_bLongPress = false;
        qDebug()<<__FUNCTION__<<"m_bLongPress"<<m_bLongPress;
    }
}

void SkinLoader::handleTimeoutRightClick()
{
    if(m_bLongPress == true)
    {
        m_bRightClick = true;
        qDebug()<<__FUNCTION__<<"m_bRightClick"<<m_bRightClick;
    }
}

QString SkinLoader::getSkinPath(const QString& skinName) const {
    for (QDir dir : getSkinSearchPaths()) {
        if (dir.cd(skinName)) {
            return dir.absolutePath();
        }
    }
    return QString();
}

QPixmap SkinLoader::getSkinPreview(const QString& skinName) const {
    QPixmap preview(getSkinPath(skinName) + "/preferences_preview_screenshot.png");
    if (!preview.isNull()){
        return preview;
    }
    return QPixmap(":/images/skin_preview_placeholder.png");
}

QString SkinLoader::getConfiguredSkinPath() const {
    QString configSkin = m_pConfig->getValueString(ConfigKey("[Config]", "ResizableSkin"));

    // If we don't have a skin defined, we might be migrating from 1.11 and
    // should pick the closest-possible skin.
    if (configSkin.isEmpty()) {
        QString oldSkin = m_pConfig->getValueString(ConfigKey("[Config]", "Skin"));
        if (!oldSkin.isEmpty()) {
            configSkin = pickResizableSkin(oldSkin);
        }
        // If the old skin was empty or we couldn't guess a skin, go with the
        // default.
        if (configSkin.isEmpty()) {
            configSkin = getDefaultSkinName();
        }
    }

    QString skinPath = getSkinPath(configSkin);

    if (skinPath.isEmpty()) {
        skinPath = getSkinPath(getDefaultSkinName());
        qDebug() << "Could not find the user's configured skin."
                 << "Falling back on the default skin:" << skinPath;
    }
    return skinPath;
}

QString SkinLoader::getDefaultSkinName() const {
    return "Deere";
}

QWidget* SkinLoader::loadConfiguredSkin(QWidget* pParent,
                                        KeyboardEventFilter* pKeyboard,
                                        PlayerManager* pPlayerManager,
                                        ControllerManager* pControllerManager,
                                        Library* pLibrary,
                                        VinylControlManager* pVCMan,
                                        EffectsManager* pEffectsManager,
                                        RecordingManager* pRecordingManager) {
    ScopedTimer timer("SkinLoader::loadConfiguredSkin");
    QString skinPath = getConfiguredSkinPath();

    // If we don't have a skin path then fail.
    if (skinPath.isEmpty()) {
        return NULL;
    }

    LegacySkinParser legacy(m_pConfig, pKeyboard, pPlayerManager,
                            pControllerManager, pLibrary, pVCMan,
                            pEffectsManager, pRecordingManager);
    QWidget* w = legacy.parseSkin(skinPath, pParent);
    m_mapWidget = legacy.getWidget();
    return w;
}

LaunchImage* SkinLoader::loadLaunchImage(QWidget* pParent) {
    QString skinPath = getConfiguredSkinPath();
    LegacySkinParser parser(m_pConfig);
    LaunchImage* pLaunchImage = parser.parseLaunchImage(skinPath, pParent);
    if (pLaunchImage == nullptr) {
        // Construct default LaunchImage
        pLaunchImage = new LaunchImage(pParent, QString());
    }
    return pLaunchImage;
}

QString SkinLoader::pickResizableSkin(QString oldSkin) const {
    if (oldSkin.contains("latenight", Qt::CaseInsensitive)) {
        return "LateNight";
    }
    if (oldSkin.contains("deere", Qt::CaseInsensitive)) {
        return "Deere";
    }
    if (oldSkin.contains("shade", Qt::CaseInsensitive)) {
        return "Shade";
    }
    return QString();
}

#include <QMediaPlayer>
#include <QMediaPlaylist>
MusicButtonControl::MusicButtonControl(SkinLoader *skinLoder)
    : QObject(NULL),
      m_skinLoader(skinLoder)
{
    m_inMove = false;
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeupdate()));

    m_player = new QMediaPlayer(NULL);//��ʼ������

    m_playlist = new QMediaPlaylist(NULL);//��ʼ�������б�

    m_playlist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);//���ò���ģʽ(˳�򲥷ţ�����ѭ����������ŵ�)

    loadMusicList();

    m_player->setPlaylist(m_playlist);  //���ò����б�
}

void MusicButtonControl::play(int index)
{
    m_timer->stop();
    m_timer->start(100);
    if(!m_inMove)
    {
        m_playlist->setCurrentIndex(index);
        m_player->play();//���Ÿ���
        m_inMove =true;
    }
}

void MusicButtonControl::loadMusicList()
{
    //���������б�
    QString skinPath = m_skinLoader->getConfiguredSkinPath();
    QSettings *iniSetting = new QSettings(skinPath + "/music/musicList.ini", QSettings::IniFormat);
    QStringList groups = iniSetting->childGroups();
    for (int i=0; i<groups.size(); ++i)
    {
        QString strGroup = groups.at(i);
        iniSetting->beginGroup(strGroup);
        QString path = iniSetting->value("path").toString();
        m_playlist->addMedia(QUrl::fromLocalFile(skinPath + "/music/" + path));
        iniSetting->endGroup();
    }
}

void MusicButtonControl::timeupdate()
{
    m_inMove = false;
}


SerialPort::SerialPort()
{
    serial = NULL;
    //���ҿ��õĴ���
    foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            m_portList.append(serial.portName());
            serial.close();
        }
    }
}

SerialPort::~SerialPort()
{
    //�رմ���
    serial->clear();
    serial->close();
    serial->deleteLater();
}

void SerialPort::OpenSerial(QString portName)
{
    serial = new QSerialPort;
    //���ô�����
    serial->setPortName(portName);
    //�򿪴���
    if(serial->open(QIODevice::ReadWrite))
    {
        //���ò�����
        serial->setBaudRate(QSerialPort::Baud115200);//���ò�����Ϊ115200
        //��������λ��
        serial->setDataBits(QSerialPort::Data8);//��������λ8
        //����У��λ
        serial->setParity(QSerialPort::NoParity); //��У��
        //����ֹͣλ
        serial->setStopBits(QSerialPort::OneStop);//ֹͣλ����Ϊ1
        //����������
        serial->setFlowControl(QSerialPort::NoFlowControl);//����Ϊ��������

        QMessageBox::about(NULL, "warning", QStringLiteral("���Ӵ��ڳɹ�"));
    }
    else
    {
        QMessageBox::about(NULL, "warning", QStringLiteral("���Ӵ��ڳɹ�"));
    }
}

void SerialPort::sendData(QByteArray array)
{
    serial->write(array);
}

QStringList SerialPort::portList()
{
    return m_portList;
}

void SerialPort::sendData(int type)
{
    QByteArray array;
    array[0] = 0xff;
    switch (type) {
    case 0:
    {
        array[1] = 0x00;
        break;
    }
    case 1:
    {
        array[1] = 0x01;
        break;
    }
    case 2:
    {
        array[1] = 0x02;
        break;
    }
    case 3:
    {
        array[1] = 0x03;
        break;
    }
    case 4:
    {
        array[1] = 0x04;
        break;
    }
    case 5:
    {
        array[1] = 0x05;
        break;
    }
    default:
        break;
    }

    sendData(array);
}

bool SerialPort::isOpen()
{
    if(serial && serial->isOpen())
        return true;
    return false;
}
