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

//弧度
#define π 3.1415926
const float Radian0 = 0;
const float Radian30 = π / 6;
const float Radian60 = π / 3;
const float Radian90 = π / 2;

const float Radian120 = 2 * π / 3;
const float Radian150 = 5 * π / 6;
const float Radian180 = π;

const float Radian181 = -π;
const float Radian210 = -5 * π / 6;
const float Radian240 = -2 * π / 3;
const float Radian270 = -π / 2;


const float Radian300 = -π / 3;
const float Radian330 = -π / 6;
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
    , m_pConfig(pConfig) {
    m_musicBtControl = new MusicButtonControl(this);

    m_serialPort = new SerialPort;

    m_ftTask = new FtTask();
    m_ftTask->start();

    m_timeB3 = new QTimer(this);
    connect(m_timeB3, SIGNAL(timeout()), this, SLOT(handleTimeoutB3()));
    m_timeB3->start(m_timeOutB3);

    m_timeB4 = new QTimer(this);
    connect(m_timeB4, SIGNAL(timeout()), this, SLOT(handleTimeoutB4()));
    m_timeB4->start(m_timeOutB4);

    m_timeB5 = new QTimer(this);
    connect(m_timeB5, SIGNAL(timeout()), this, SLOT(handleTimeoutB5()));
    m_timeB5->start(m_timeOutB5);

    m_timeB9 = new QTimer(this);
    connect(m_timeB9, SIGNAL(timeout()), this, SLOT(handleTimeoutB9()));
    m_timeB9->start(m_timeOutB9);

    m_timeB10 = new QTimer(this);
    connect(m_timeB10, SIGNAL(timeout()), this, SLOT(handleTimeoutB10()));
    m_timeB10->start(m_timeOutB10);

    m_timeB11 = new QTimer(this);
    connect(m_timeB11, SIGNAL(timeout()), this, SLOT(handleTimeoutB11()));
    m_timeB11->start(m_timeOutB11);

    m_timeB17 = new QTimer(this);
    connect(m_timeB17, SIGNAL(timeout()), this, SLOT(handleTimeoutB17()));
    m_timeB17->start(m_timeOutB17);

    m_timeB19 = new QTimer(this);
    connect(m_timeB19, SIGNAL(timeout()), this, SLOT(handleTimeoutB19()));
    m_timeB19->start(m_timeOutB19);

    m_timeB21 = new QTimer(this);
    connect(m_timeB21, SIGNAL(timeout()), this, SLOT(handleTimeoutB21()));
    m_timeB21->start(m_timeOutB21);

    m_timeB23 = new QTimer(this);
    connect(m_timeB23, SIGNAL(timeout()), this, SLOT(handleTimeoutB23()));
    m_timeB23->start(m_timeOutB23);

    m_timeB18 = new QTimer(this);
    connect(m_timeB18, SIGNAL(timeout()), this, SLOT(handleTimeoutB18()));
    m_timeB18->start(m_timeOutB18);

    m_timeB20 = new QTimer(this);
    connect(m_timeB20, SIGNAL(timeout()), this, SLOT(handleTimeoutB20()));
    m_timeB20->start(m_timeOutB20);

    m_timeB22 = new QTimer(this);
    connect(m_timeB22, SIGNAL(timeout()), this, SLOT(handleTimeoutB22()));
    m_timeB22->start(m_timeOutB22);

    m_timeB24 = new QTimer(this);
    connect(m_timeB24, SIGNAL(timeout()), this, SLOT(handleTimeoutB24()));
    m_timeB24->start(m_timeOutB24);

    m_timeB25 = new QTimer(this);
    connect(m_timeB25, SIGNAL(timeout()), this, SLOT(handleTimeoutB25()));
    m_timeB25->start(m_timeOutB25);

}

SkinLoader::~SkinLoader() {
    LegacySkinParser::freeChannelStrings();
    delete m_ftTask;
    m_ftTask = NULL;
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
    //解析坐标文件

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
void SkinLoader::dealWithLED(WidgetType type, QString objName, int x, int y, QRect rct, QWidget *widget)
{

    QPoint centerPoint = rct.center();
    QPoint point = QPoint(x - centerPoint.x(), centerPoint.y() - y);
    float  radian = 0;
    switch (type) {
    case type_WSpinny:
        //一圈26个灯，顺时针依次为：D38~D32;D31~D28,D233,D76;D73,D47~D44;D43~D39
        //30度区域
        //计算反正切值
        radian = qAtan2(point.y(), point.x() );
        qDebug()<<"x:"<<x;
        qDebug()<<"y:"<<y;
        qDebug()<<"rct:"<<rct;
        qDebug()<<"centerPoint:"<<centerPoint;
        qDebug()<<"point:"<<point;
        qDebug()<<"radian:"<<radian;
        //A3区
        if(objName == "SpinnySingletonNoCover[Channel1]_handh")
        {
            //常亮D25,D26,D27
            m_ftTask->setBuff(3, 0x01);
            //亮D32,D33
            if(radian > Radian0 && radian < Radian30)
            {
                m_ftTask->setBuff(3, 0x01);
                m_ftTask->update();

                m_ftTask->setBuff(3, 0x00);
                m_ftTask->update();
            }
            //亮D34,D35
            else if(radian > Radian30 && radian < Radian60)
            {

            }
            //亮D36,D37,D38
            else if(radian > Radian60 && radian < Radian90)
            {

            }
            //亮D39,D40
            else if(radian > Radian90 && radian < Radian120)
            {

            }
            //亮D41,D42
            else if(radian > Radian120 && radian < Radian150)
            {

            }
            //亮D43
            else if(radian > Radian150 && radian < Radian180)
            {

            }
            //亮D44,D45
            else if(radian > Radian181 && radian < Radian210)
            {

            }
            //亮D46,D47
            else if(radian > Radian210 && radian < Radian240)
            {

            }
            //亮D73,D76
            else if(radian > Radian240 && radian < Radian270)
            {

            }
            //亮D223,D28
            else if(radian > Radian270 && radian < Radian300)
            {

            }
            //亮D29,D30
            else if(radian > Radian300 && radian < Radian330)
            {

            }
            //亮D31,D32
            else if(radian > Radian330 && radian < Radian360)
            {

            }
        }
        //A9区
        else if(objName == "SpinnySingletonNoCover[Channel2]_handh")
        {
            //常亮D178,D179,D180
            m_ftTask->setBuff(3, 0x01);
            //亮D166,D167
            if(radian > Radian0 && radian < Radian30)
            {
                m_ftTask->setBuff(3, 0x01);
                m_ftTask->update();

                m_ftTask->setBuff(3, 0x00);
                m_ftTask->update();
            }
            //亮D168,D169
            else if(radian > Radian30 && radian < Radian60)
            {

            }
            //亮D170,D171,D172
            else if(radian > Radian60 && radian < Radian90)
            {

            }
            //亮D173,D174
            else if(radian > Radian90 && radian < Radian120)
            {

            }
            //亮D175,D176
            else if(radian > Radian120 && radian < Radian150)
            {

            }
            //亮D177
            else if(radian > Radian150 && radian < Radian180)
            {

            }
            //亮D225,D92
            else if(radian > Radian181 && radian < Radian210)
            {

            }
            //亮D224,D158
            else if(radian > Radian210 && radian < Radian240)
            {

            }
            //亮D159
            else if(radian > Radian240 && radian < Radian270)
            {

            }
            //亮D160,D161
            else if(radian > Radian270 && radian < Radian300)
            {

            }
            //亮D162,D163
            else if(radian > Radian300 && radian < Radian330)
            {

            }
            //亮D164,D165,D166
            else if(radian > Radian330 && radian < Radian360)
            {

            }
        }
        break;
    case type_WKnobComposed:
    {
        float posW = x - rct.x();
        float ratio = posW / rct.width();

        //B3区
        if(objName == "KnobComposed_[EffectRack1_EffectUnit1_Effect1]_meta")
        {
             if(ratio < 0.3)
             {
                 m_timeOutB3 = 1000;
             }
             else if(ratio < 0.6)
             {
                 m_timeOutB3 = 100;
             }
             else
             {
                 m_timeOutB3 = 10;
             }
        }
        //B4区
        else if(objName == "KnobComposed_[EffectRack1_EffectUnit1_Effect2]_meta")
        {
             if(ratio < 0.3)
             {
                 m_timeOutB4 = 1000;
             }
             else if(ratio < 0.6)
             {
                 m_timeOutB4 = 100;
             }
             else
             {
                 m_timeOutB4 = 10;
             }
        }
        //B5区
        else if(objName == "KnobComposed_[EffectRack1_EffectUnit1_Effect3]_meta")
        {
             if(ratio < 0.3)
             {
                 m_timeOutB5 = 1000;
             }
             else if(ratio < 0.6)
             {
                 m_timeOutB5 = 100;
             }
             else
             {
                 m_timeOutB5 = 10;
             }
        }
        //B9区
        else if(objName == "KnobComposed_[EffectRack1_EffectUnit2_Effect1]_meta")
        {
             if(ratio < 0.3)
             {
                 m_timeOutB9 = 1000;
             }
             else if(ratio < 0.6)
             {
                 m_timeOutB9 = 100;
             }
             else
             {
                 m_timeOutB9 = 10;
             }
        }
        //B10区
        else if(objName == "KnobComposed_[EffectRack1_EffectUnit2_Effect2]_meta")
        {
             if(ratio < 0.3)
             {
                 m_timeOutB10 = 1000;
             }
             else if(ratio < 0.6)
             {
                 m_timeOutB10 = 100;
             }
             else
             {
                 m_timeOutB10 = 10;
             }
        }
        //B11区
        else if(objName == "KnobComposed_[EffectRack1_EffectUnit2_Effect3]_meta")
        {
             if(ratio < 0.3)
             {
                 m_timeOutB11 = 1000;
             }
             else if(ratio < 0.6)
             {
                 m_timeOutB11 = 100;
             }
             else
             {
                 m_timeOutB11 = 10;
             }
        }
        //B17区
        else if(objName == "EffectKnobComposed_[Channel1]_3")
        {
             if(ratio < 0.3)
             {
                 m_timeOutB17 = 1000;
             }
             else if(ratio < 0.6)
             {
                 m_timeOutB17 = 100;
             }
             else
             {
                 m_timeOutB17 = 10;
             }
        }
        //B19区
        else if(objName == "EffectKnobComposed_[Channel1]_2")
        {
             if(ratio < 0.3)
             {
                 m_timeOutB19 = 1000;
             }
             else if(ratio < 0.6)
             {
                 m_timeOutB19 = 100;
             }
             else
             {
                 m_timeOutB19 = 10;
             }
        }
        //B21区
        else if(objName == "EffectKnobComposed_[Channel1]_1")
        {
             if(ratio < 0.3)
             {
                 m_timeOutB21 = 1000;
             }
             else if(ratio < 0.6)
             {
                 m_timeOutB21 = 100;
             }
             else
             {
                 m_timeOutB21 = 10;
             }
        }

        //B23区
        else if(objName == "KnobComposed_[QuickEffectRack1_[Channel1]]_super1")
        {
             if(ratio < 0.3)
             {
                 m_timeOutB23 = 1000;
             }
             else if(ratio < 0.6)
             {
                 m_timeOutB23 = 100;
             }
             else
             {
                 m_timeOutB23 = 10;
             }
        }
        //B18区
        else if(objName == "EffectKnobComposed_[Channel2]_3")
        {
             if(ratio < 0.3)
             {
                 m_timeOutB18 = 1000;
             }
             else if(ratio < 0.6)
             {
                 m_timeOutB18 = 100;
             }
             else
             {
                 m_timeOutB18 = 10;
             }
        }
        //B20区
        else if(objName == "EffectKnobComposed_[Channel2]_2")
        {
             if(ratio < 0.3)
             {
                 m_timeOutB20 = 1000;
             }
             else if(ratio < 0.6)
             {
                 m_timeOutB20 = 100;
             }
             else
             {
                 m_timeOutB20 = 10;
             }
        }
        //B22区
        else if(objName == "EffectKnobComposed_[Channel2]_1")
        {
             if(ratio < 0.3)
             {
                 m_timeOutB22 = 1000;
             }
             else if(ratio < 0.6)
             {
                 m_timeOutB22 = 100;
             }
             else
             {
                 m_timeOutB22 = 10;
             }
        }

        //B24区
        else if(objName == "KnobComposed_[QuickEffectRack1_[Channel2]]_super1")
        {
             if(ratio < 0.3)
             {
                 m_timeOutB24 = 1000;
             }
             else if(ratio < 0.6)
             {
                 m_timeOutB24 = 100;
             }
             else
             {
                 m_timeOutB24 = 10;
             }
        }
        break;

    }

    case type_WPushButton:
    {
        if(WPushButton *pushbutton = dynamic_cast<WPushButton *>(widget))
        {
            //这里亮灯的状态需要根据pushbutton的点击状态来决定
            //B36
            if(objName == "DeckCue_Deck1_hotcue")
            {
                //亮D50,D51

            }
            //B50
            else if(objName == "DeckCue_Deck2_hotcue")
            {
                //亮D85，D86
            }
            //B37
            else if(objName == "PlayToggle_Deck1_hotcue")
            {
                //亮D52，D53
            }
            //B51
            else if(objName == "PlayToggle_Deck1_hotcue")
            {
                //亮D187，D188
            }
        }
        break;
    }
    case type_WSliderComposed:
        //A6区,主推子,led从中间向两边扩展
        if(objName == "MainSliderComposed")
        {
            //亮D128，D129
            m_ftTask->setBuff(4, 0x01);
            //根据触控点在推子中的位置确定亮灯
            float halfW = (rct.width() / 2);
            float poswidth = 0;
            if(point.x() < 0)
            {
                 poswidth = -point.x();
                 //小于1/4处
                 if( (poswidth / halfW) < 0.25 )
                 {
                      m_ftTask->setBuff(4, 0x02);
                 }
                 else if((poswidth / halfW) < 0.5)
                 {
                     m_ftTask->setBuff(4, 0x04);
                 }
                 else if((poswidth / halfW) < 0.75)
                 {
                     m_ftTask->setBuff(4, 0x08);
                 }
                 else
                 {
                     m_ftTask->setBuff(4, 0x10);
                 }
            }
            if(point.x() > 0)
            {
                 poswidth = point.x();
                 //小于1/4处
                 if( (poswidth / halfW) < 0.25 )
                 {
                      m_ftTask->setBuff(4, 0x02);
                 }
                 else if((poswidth / halfW) < 0.5)
                 {
                     m_ftTask->setBuff(4, 0x04);
                 }
                 else if((poswidth / halfW) < 0.75)
                 {
                     m_ftTask->setBuff(4, 0x08);
                 }
                 else
                 {
                     m_ftTask->setBuff(4, 0x10);
                 }
            }

        }
        //A2区
        else if(objName == "ChannelVolume_[Channel1]")
        {
            //亮D24
            m_ftTask->setBuff(5, 0x10);
            float posH = (y - rct.bottom())/ rct.height();
            if(posH < 0.3)
            {
                //亮D22,D23
                m_ftTask->setBuff(5, 0x10);
            }
            else if(posH < 0.6)
            {
                //亮D20,D21
                m_ftTask->setBuff(5, 0x10);
            }
            else
            {
                //亮D18,D19
                m_ftTask->setBuff(5, 0x10);
            }
        }
        //A7区
        else if(objName == "ChannelVolume_[Channel2]")
        {
            //亮D150
            m_ftTask->setBuff(5, 0x10);
            float posH = (y - rct.bottom())/ rct.height();
            if(posH < 0.3)
            {
                //亮D148,D149
                m_ftTask->setBuff(5, 0x10);
            }
            else if(posH < 0.6)
            {
                //亮D146,D147
                m_ftTask->setBuff(5, 0x10);
            }
            else
            {
                //亮D144,D145
                m_ftTask->setBuff(5, 0x10);
            }
        }
        //A1区
        else if(objName == "RateDisplay1_handh")
        {
            //亮D17
            m_ftTask->setBuff(5, 0x10);
            float posH = (y - rct.bottom())/ rct.height();
            if(posH < 0.3)
            {
                //亮D15,D16
                m_ftTask->setBuff(5, 0x10);
            }
            else if(posH < 0.6)
            {
                //亮D13,D14
                m_ftTask->setBuff(5, 0x10);
            }
            else
            {
                //亮D11,D12
                m_ftTask->setBuff(5, 0x10);
            }
        }
        //A8区
        else if(objName == "RateDisplay2_handh")
        {
            //亮D157
            m_ftTask->setBuff(5, 0x10);
            float posH = (y - rct.bottom())/ rct.height();
            if(posH < 0.3)
            {
                //亮D155,D156
                m_ftTask->setBuff(5, 0x10);
            }
            else if(posH < 0.6)
            {
                //亮D153,D154
                m_ftTask->setBuff(5, 0x10);
            }
            else
            {
                //亮D151,D152
                m_ftTask->setBuff(5, 0x10);
            }
        }
        break;
    default:
        break;
    }

}

void SkinLoader::getComingData(QByteArray data)
{
    int  x = (data[3] << 8) + data[2];
    int  y = (data[5] << 8) + data[4];

    //遍历各个控件的区域，看(x,y)所在的位置是哪个控件
    QMap<QString, QRect>::iterator iter = m_mapRects.begin();
    while(iter != m_mapRects.end())
    {
        QRect rct = iter.value();
        if(rct.contains(x, y))
        {
            //获取控件的objectName
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
                m_timeB25->start(10);
                m_timeOutB25 = false;
            }

            QString objectNameTemp = objectName.replace('{','[');
            objectNameTemp = objectNameTemp.replace('}',']');
            //根据objectName获取具体对应的控件
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
                //B25定时器超时，说明shift处于非按压状态
                WBaseWidget::OperateType operateType = WBaseWidget::M_leftPress;
                if(m_timeOutB25 == true)
                {
                    operateType = WBaseWidget::M_leftPress;
                }
                else
                {
                    operateType = WBaseWidget::M_rightPress;
                }
                //operateType来控制按钮是 左键 还是 右键 点击（shift+触控板点击 被认为是右键）
                pushbutton->getComingData(data, rct, operateType);
            }
            else if(WSliderComposed *sliderComposed = dynamic_cast<WSliderComposed *>(widget))
            {
                 m_widgetType = type_WSliderComposed;
                sliderComposed->getComingData(data, rct);
            }
            //给led灯板发信号
            dealWithLED(m_widgetType,objectNameTemp, x, y, rct,widget);
            break;
        }
        iter++;
    }
}

void SkinLoader::handleTimeoutB3()
{
    if(m_timeB3->isActive())
    {
        //亮D5,D6,D205
        //....
        m_timeB3->stop();
    }
    m_timeB3->start(m_timeOutB3);
}

void SkinLoader::handleTimeoutB4()
{
    if(m_timeB4->isActive())
    {
        //亮D7,D8,D206
        //....
        m_timeB4->stop();
    }
    m_timeB4->start(m_timeOutB4);
}

void SkinLoader::handleTimeoutB5()
{
    if(m_timeB5->isActive())
    {
        //亮D9,D10,D207
        //....
        m_timeB5->stop();
    }
    m_timeB5->start(m_timeOutB5);
}

void SkinLoader::handleTimeoutB9()
{
    if(m_timeB9->isActive())
    {
        //亮D137,D138,D208
        //....
        m_timeB9->stop();
    }
    m_timeB9->start(m_timeOutB9);
}

void SkinLoader::handleTimeoutB10()
{
    if(m_timeB10->isActive())
    {
        //亮D139,D140,D209
        //....
        m_timeB10->stop();
    }
    m_timeB10->start(m_timeOutB10);
}

void SkinLoader::handleTimeoutB11()
{
    if(m_timeB11->isActive())
    {
        //亮D141,D142,D210
        //....
        m_timeB11->stop();
    }
    m_timeB11->start(m_timeOutB11);

}

void SkinLoader::handleTimeoutB17()
{
    if(m_timeB17->isActive())
    {
        //亮D105,D106,D215
        //....
        m_timeB17->stop();
    }
    m_timeB17->start(m_timeOutB17);
}

void SkinLoader::handleTimeoutB19()
{
    if(m_timeB19->isActive())
    {
        //亮D107,D108,D217
        //....
        m_timeB19->stop();
    }
    m_timeB19->start(m_timeOutB19);
}

void SkinLoader::handleTimeoutB21()
{
    if(m_timeB21->isActive())
    {
        //亮D109,D110,D219
        //....
        m_timeB21->stop();
    }
    m_timeB21->start(m_timeOutB21);
}

void SkinLoader::handleTimeoutB23()
{
    if(m_timeB23->isActive())
    {
        //亮D111,D112,D221
        //....
        m_timeB23->stop();
    }
    m_timeB23->start(m_timeOutB23);
}

void SkinLoader::handleTimeoutB18()
{
    if(m_timeB18->isActive())
    {
        //亮D113,D114,D216
        //....
        m_timeB18->stop();
    }
    m_timeB18->start(m_timeOutB18);
}

void SkinLoader::handleTimeoutB20()
{
    if(m_timeB20->isActive())
    {
        //亮D115,D116,D218
        //....
        m_timeB20->stop();
    }
    m_timeB20->start(m_timeOutB20);
}

void SkinLoader::handleTimeoutB22()
{
    if(m_timeB22->isActive())
    {
        //亮D117,D118,D220
        //....
        m_timeB22->stop();
    }
    m_timeB22->start(m_timeOutB22);
}

void SkinLoader::handleTimeoutB24()
{
    if(m_timeB24->isActive())
    {
        //亮D119,D120,D222
        //....
        m_timeB24->stop();
    }
    m_timeB24->start(m_timeOutB24);

}

void SkinLoader::handleTimeoutB25()
{
    if(m_timeB25->isActive())
    {
        m_timeOutB25 = true;
        m_timeB25->stop();
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

    m_player = new QMediaPlayer(NULL);//初始化音乐

    m_playlist = new QMediaPlaylist(NULL);//初始化播放列表

    m_playlist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);//设置播放模式(顺序播放，单曲循环，随机播放等)

    loadMusicList();

    m_player->setPlaylist(m_playlist);  //设置播放列表
}

void MusicButtonControl::play(int index)
{
    m_timer->stop();
    m_timer->start(100);
    if(!m_inMove)
    {
        m_playlist->setCurrentIndex(index);
        m_player->play();//播放歌曲
        m_inMove =true;
    }
}

void MusicButtonControl::loadMusicList()
{
    //解析歌曲列表
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
    //查找可用的串口
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
    //关闭串口
    serial->clear();
    serial->close();
    serial->deleteLater();
}

void SerialPort::OpenSerial(QString portName)
{
    serial = new QSerialPort;
    //设置串口名
    serial->setPortName(portName);
    //打开串口
    if(serial->open(QIODevice::ReadWrite))
    {
        //设置波特率
        serial->setBaudRate(QSerialPort::Baud115200);//设置波特率为115200
        //设置数据位数
        serial->setDataBits(QSerialPort::Data8);//设置数据位8
        //设置校验位
        serial->setParity(QSerialPort::NoParity); //无校验
        //设置停止位
        serial->setStopBits(QSerialPort::OneStop);//停止位设置为1
        //设置流控制
        serial->setFlowControl(QSerialPort::NoFlowControl);//设置为无流控制

        QMessageBox::about(NULL, "warning", QStringLiteral("连接串口成功"));
    }
    else
    {
        QMessageBox::about(NULL, "warning", QStringLiteral("连接串口成功"));
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
