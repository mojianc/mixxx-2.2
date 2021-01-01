#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H
//GameWidget

#include <QGraphicsView>
#include <QGraphicsWidget>
#include <QThread>
#include <QParallelAnimationGroup>
#include <QDebug>

#include "TrackPixmaps.h"
#include "EnglistLabel.h"
#include "CusProgress.h"
#include "Hairclipper.h"

class QPropertyAnimation;

class GameWidget : public QGraphicsView
{
    Q_OBJECT
    
public:
    GameWidget(double w, double h, QThread *thread, int timedelay, int positionCoefficient, QWidget *parent = Q_NULLPTR);
    ~GameWidget();

	void connectSlots();

protected:
	void timerEvent(QTimerEvent *event);

private:
    void initWidget();
    
private:
    void initBG();
    void initPixmaps();
    void initTrack1Pixmaps();
    void initTrack2Pixmaps();
    void initTrack3Pixmaps();
    void initTrack4Pixmaps();
    
    void initMiddleElement();

    //void animationActivate(QPropertyAnimation *animation);
    //void animationActivate(QParallelAnimationGroup *animationGroup);
    
private slots:
    void startDishesAnimation();
    void Trigger0();
    void Trigger1500();
    void Trigger11000();
    void Trigger14000();
    void Trigger15000();
    void Trigger16000();
    void Trigger22000();
    void Trigger25000();
    void Trigger27500();
    void Trigger30000();
    void Trigger34000();
    void Trigger37000();
    void Trigger38000();
    void Trigger40000();
    void Trigger47000();
    void Trigger50000();
    void Trigger50500();
    void Trigger51000();
    void Trigger72000();
    void Trigger75000();
    void Trigger76000();
    void Trigger77000();
    void Trigger88000();
    void Trigger91000();
    void Trigger91500();
    void Trigger92000();
    
private:
	double m_w;
	double m_h;

    double m_dHradio;
    double m_dVradio;
    
    double m_dBottomBtnH;
    double m_dBottomBtnW;
    double m_dCircualH;
    double m_dCircualW;
    double m_dCircualMoveH;
    double m_dSlideDownBtnY;
    double m_dSlideDownBtnStartW;
    double m_dSlideDownBtnStartH;
    double m_dSlideDownBtnEndW;
    double m_dSlideDownBtnEndH;
    int m_nWindowH;
    int m_nWindowV;
    QGraphicsScene* m_pScene;
    
    CusProgress* m_pProgress;
    Hairclipper* m_pHairClipper;
    EnglistLabel* m_pLabel;
    Pixmap *m_pMiddleDishes;
    Pixmap *m_pMiddleBtn;
    Pixmap *m_pMiddleBtnClick;
    QPropertyAnimation* m_pMidDishesAnimation;
    int m_nDishesRotateNum;
    
    TrackPixmaps* m_pTrack1;
    TrackPixmaps* m_pTrack2;
    TrackPixmaps* m_pTrack3;
    TrackPixmaps* m_pTrack4;

	int m_timeDelay;
	int m_positionCoefficient;
};
#endif // GAMEWIDGET_H
