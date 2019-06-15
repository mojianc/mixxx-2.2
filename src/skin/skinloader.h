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

class SerialPort : public QObject
{
    Q_OBJECT
public:
    explicit SerialPort();
    ~SerialPort();
    QStringList portList();
    void sendData(int type);
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

class SkinLoader : public QObject{
    Q_OBJECT
public:
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
  public slots:
    void getComingData(QByteArray data);
  private:
    QString pickResizableSkin(QString oldSkin) const;

    UserSettingsPointer m_pConfig;

    QMap<QString, QRect> m_mapRects;
    QMap<QString, QWidget *> m_mapWidget;

    MusicButtonControl *m_musicBtControl;

    VideoWidget *m_videoWidget;

    SerialPort *m_serialPort;
};


#endif /* SKINLOADER_H */
