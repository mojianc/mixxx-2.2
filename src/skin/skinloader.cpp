// skinloader.cpp
// Created 6/21/2010 by RJ Ryan (rryan@mit.edu)

#include "skin/skinloader.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QDir>
#include <QString>
#include <QtDebug>

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

SkinLoader::SkinLoader(UserSettingsPointer pConfig)
    : QObject(NULL)
    , m_pConfig(pConfig) {
    m_musicBtControl = new MusicButtonControl(this);
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

        m_mapRects.insert(strGroup, rect);
        iniSetting->endGroup();
    }
}

void SkinLoader::connectHid(ControllerManager *pControllerManager)
{
    QList<Controller *> contrllerList = pControllerManager->getControllerList(false, true);
    for(int i = 0; i < contrllerList.count(); i++)
    {
        Controller *controller = contrllerList.at(i);
        if(controller->getControllerId() == (0x2575 + 0x0001))
        {
            if(!controller->isOpen())
            {
                controller->open();
                connect(controller, SIGNAL(incomingData(QByteArray)), this, SLOT(getComingData(QByteArray)), Qt::UniqueConnection);
            }
        }
    }

}

void SkinLoader::setVideoWidget(VideoWidget *widget)
{
    m_videoWidget = widget;
}
#include "widget\wspinny.h"
#include "widget\wknobcomposed.h"
#include "widget\wpushbutton.h"
#include "widget\wslidercomposed.h"
void SkinLoader::getComingData(QByteArray data)
{
    int  x = (data[3] << 8) + data[2];
    int  y = (data[5] << 8) + data[4];

    QMap<QString, QRect>::iterator iter = m_mapRects.begin();
    while(iter != m_mapRects.end())
    {
        QRect rct = iter.value();
        if(rct.contains(x, y))
        {
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

            QString objectNameTemp = objectName.replace('{','[');
            objectNameTemp = objectNameTemp.replace('}',']');
            QWidget *widget = m_mapWidget.value(objectNameTemp);
            if(WSpinny *spinny = dynamic_cast<WSpinny *>(widget))
            {
                spinny->getComingData(data, rct);
            }
            else if(WKnobComposed *knobCompose = dynamic_cast<WKnobComposed *>(widget))
            {
                knobCompose->getComingData(data, rct);
            }
            else if(WPushButton *pushbutton = dynamic_cast<WPushButton *>(widget))
            {
                pushbutton->getComingData(data, rct);
            }
            else if(WSliderComposed *sliderComposed = dynamic_cast<WSliderComposed *>(widget))
            {
                sliderComposed->getComingData(data, rct);
            }
            break;
        }
        iter++;
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

//    m_playlist->addMedia(QUrl::fromLocalFile("E:/CloudMusic/迷途.mp3"));//添加歌曲，这里添加的是歌曲的路径

//    m_playlist->addMedia(QUrl::fromLocalFile("E:/CloudMusic/9er_tan.mp3"));//添加歌曲，这里添加的是歌曲的路径

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
        m_playlist->addMedia(QUrl::fromLocalFile(path));
        iniSetting->endGroup();
    }
}

void MusicButtonControl::timeupdate()
{
    m_inMove = false;
}
