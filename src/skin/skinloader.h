#ifndef SKINLOADER_H
#define SKINLOADER_H

#include <QWidget>
#include <QList>
#include <QDir>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "preferences/usersettings.h"
#include "widget\wbasewidget.h"
class KeyboardEventFilter;
class PlayerManager;
class ControllerManager;
class Library;
class VinylControlManager;
class EffectsManager;
class RecordingManager;
class LaunchImage;
class LegacySkinParser;
class QMediaPlayer;
class QMediaPlaylist;
class SkinLoader;
class VideoWidget;
class FtTask;

class SerialPort : public QObject
{
    Q_OBJECT
public:
    explicit SerialPort();
    ~SerialPort();
    QStringList portList();
    void sendData(int type);
    bool isOpen();
public slots:
    void OpenSerial(QString portName);
    void sendData(QByteArray array);

private:
    QSerialPort *serial;
    QStringList m_portList;
};

class MusicButtonControl: public QObject
{
    Q_OBJECT
public:
    MusicButtonControl(SkinLoader *skinLoder);
    void play(int index);
    void loadMusicList();
public slots:
    void timeupdate();
private:
    QTimer *m_timer;
    bool m_inMove;
    QMediaPlayer  *m_player;
    QMediaPlaylist *m_playlist;
    SkinLoader *m_skinLoader;
};
#include <QTimer>
class SkinLoader : public QObject{
    Q_OBJECT
public:
    enum WidgetType{
        type_None,
        type_WSpinny,
        type_WKnobComposed,
        type_WPushButton,
        type_WSliderComposed
    };

    SkinLoader(UserSettingsPointer pConfig);
    virtual ~SkinLoader();

    QWidget* loadConfiguredSkin(QWidget* pParent,
                                KeyboardEventFilter* pKeyboard,
                                PlayerManager* pPlayerManager,
                                ControllerManager* pControllerManager,
                                Library* pLibrary,
                                VinylControlManager* pVCMan,
                                EffectsManager* pEffectsManager,
                                RecordingManager* pRecordingManager);

    LaunchImage* loadLaunchImage(QWidget* pParent);

    QString getSkinPath(const QString& skinName) const;
    QPixmap getSkinPreview(const QString& skinName) const;
    QString getConfiguredSkinPath() const;
    QString getDefaultSkinName() const;
    QList<QDir> getSkinSearchPaths() const;

    void loadConfigCoordinate();
    void connectHid(ControllerManager* pControllerManager);
    void setVideoWidget(VideoWidget *widget);
    SerialPort *getSerialPort();
    void dealWithLED(WidgetType type, QString objName, int x, int y, QRect rct, QWidget *widget = NULL);
    void setLED_ON(int byte, int bit);
    void setLED_OFF(int byte, int bit);
    void led_update();
  public slots:
    void getComingData(QByteArray data);
    void handleTimeoutB3();  //超时处理函数
    void handleTimeoutB4();
    void handleTimeoutB5();
    void handleTimeoutB9();
    void handleTimeoutB10();
    void handleTimeoutB11();

    void handleTimeoutB17();
    void handleTimeoutB19();
    void handleTimeoutB21();
    void handleTimeoutB23();

    void handleTimeoutB18();
    void handleTimeoutB20();
    void handleTimeoutB22();
    void handleTimeoutB24();
    void handleTimeoutB25();
    void internalHandleTimeout(QTimer* timer, int interval, int* light,
                                           int group1, int byte11, int byte12, int group2, int byte21);
    void internalDealWithLED(float ratio, QTimer* timer, int* internal, int group1, int byte11, int byte12, int group2, int byte21);
  private:
    QString pickResizableSkin(QString oldSkin) const;

    UserSettingsPointer m_pConfig;

    QMap<QString, QRect> m_mapRects;
    QMap<QString, QWidget *> m_mapWidget;

    QRect m_knobScale;
    MusicButtonControl *m_musicBtControl;

    VideoWidget *m_videoWidget;

    SerialPort *m_serialPort;
    WidgetType m_widgetType;
    //B3,B4,B5旋钮的定时器
    QTimer *m_timeB3;
    int m_timeOutB3;
    int m_lightB3;
    QTimer *m_timeB4;
    int m_timeOutB4;
    int m_lightB4;
    QTimer *m_timeB5;
    int m_timeOutB5;
    int m_lightB5;

    //B9,B10,B11旋钮的定时器
    QTimer *m_timeB9;
    int m_timeOutB9;
    int m_lightB9;
    QTimer *m_timeB10;
    int m_timeOutB10;
    int m_lightB10;
    QTimer *m_timeB11;
    int m_timeOutB11;
    int m_lightB11;

    //B17,B19,B21,B23旋钮的定时器
    QTimer *m_timeB17;
    int m_timeOutB17;
    int m_lightB17;
    QTimer *m_timeB19;
    int m_timeOutB19;
    int m_lightB19;
    QTimer *m_timeB21;
    int m_timeOutB21;
    int m_lightB21;
    QTimer *m_timeB23;
    int m_timeOutB23;
    int m_lightB23;

    //B18,B20,B22,B24旋钮的定时器
    QTimer *m_timeB18;
    int m_timeOutB18;
    int m_lightB18;
    QTimer *m_timeB20;
    int m_timeOutB20;
    int m_lightB20;
    QTimer *m_timeB22;
    int m_timeOutB22;
    int m_lightB22;
    QTimer *m_timeB24;
    int m_timeOutB24;
    int m_lightB24;

    //判断B25 shift键是否被按下
    QTimer *m_timeB25;
    bool m_timeOutB25;
    int m_lightB25;
};


#endif /* SKINLOADER_H */
