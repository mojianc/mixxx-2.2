#include "GameWidget.h"
#include <QDebug>
#include <QTimer>
#include <QState>
//#include <QScreen>
//#include <QGuiApplication>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>

const static bool bTip = true;


GameWidget::GameWidget(double w, double h, QThread *thread, int timedelay, int positionCoefficient, QWidget *parent):
    QGraphicsView(parent)
{
	m_timeDelay = timedelay;
	m_positionCoefficient = positionCoefficient;

	m_w = w;
	m_h = h;

    initWidget();
    //connectSlots();
    
    setFrameStyle(0);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	this->moveToThread(thread);

	//startTimer(30);
}

GameWidget::~GameWidget()
{

}

void GameWidget::timerEvent(QTimerEvent *event)
{
	repaint();
}

void GameWidget::initWidget()
{
    initBG();
    
    initPixmaps();
    
    initMiddleElement();
}

void GameWidget::connectSlots()
{
//    QTimer::singleShot(0, this, &GameWidget::Trigger88000);
//    return;
    
    QTimer::singleShot(0 + m_timeDelay, this, &GameWidget::Trigger0);
    QTimer::singleShot(1500 + m_timeDelay, this, &GameWidget::Trigger1500);
    QTimer::singleShot(11000 + m_timeDelay, this, &GameWidget::Trigger11000);
    QTimer::singleShot(14000 + m_timeDelay, this, &GameWidget::Trigger14000);
    QTimer::singleShot(15000 + m_timeDelay, this, &GameWidget::Trigger15000);
    QTimer::singleShot(16000 + m_timeDelay, this, &GameWidget::Trigger16000);
    QTimer::singleShot(22000 + m_timeDelay, this, &GameWidget::Trigger22000);
    QTimer::singleShot(25000 + m_timeDelay, this, &GameWidget::Trigger25000);
    QTimer::singleShot(27500 + m_timeDelay, this, &GameWidget::Trigger27500);
    QTimer::singleShot(30000 + m_timeDelay, this, &GameWidget::Trigger30000);
    QTimer::singleShot(34000 + m_timeDelay, this, &GameWidget::Trigger34000);
    QTimer::singleShot(37000 + m_timeDelay, this, &GameWidget::Trigger37000);
    QTimer::singleShot(38000 + m_timeDelay, this, &GameWidget::Trigger38000);
    QTimer::singleShot(40000 + m_timeDelay, this, &GameWidget::Trigger40000);
    QTimer::singleShot(47000 + m_timeDelay, this, &GameWidget::Trigger47000);
    QTimer::singleShot(50000 + m_timeDelay, this, &GameWidget::Trigger50000);
    //    QTimer::singleShot(50500, this, &GameWidget::Trigger50500);
    QTimer::singleShot(72000 + m_timeDelay, this, &GameWidget::Trigger72000);
    QTimer::singleShot(75000 + m_timeDelay, this, &GameWidget::Trigger75000);
    QTimer::singleShot(77000 + m_timeDelay, this, &GameWidget::Trigger77000);
    QTimer::singleShot(88000 + m_timeDelay, this, &GameWidget::Trigger88000);
    QTimer::singleShot(91000 + m_timeDelay, this, &GameWidget::Trigger91000);
}

void GameWidget::initBG()
{
    //QScreen *pScreen=QGuiApplication::primaryScreen ();
    //QRect oWinRect=pScreen->availableGeometry() ;
    //m_dHradio = ((double)oWinRect.width()) / 2560.0;
    //m_dVradio = ((double)oWinRect.height()) / 1600.0;
    QPixmap oPix(":/images/img/Game-BG.png");

	m_dHradio = m_w / ((double)oPix.width());
	m_dVradio = m_h / ((double)oPix.height());

    m_nWindowH =int((double)(oPix.width())*m_dHradio);
    m_nWindowV =int((double)(oPix.height())*m_dVradio);
    setBackgroundBrush(oPix.scaled(m_nWindowH,m_nWindowV));
    //setFixedSize(m_nWindowH,m_nWindowV);
    m_dSlideDownBtnStartW = 120;
    m_dSlideDownBtnStartH = 60;
    m_dSlideDownBtnEndH = 80;
    m_dSlideDownBtnEndW = 220;
    QFrame *line = new QFrame(this);  
    line->setFrameShape(QFrame::HLine);  
    line->setStyleSheet("QFrame{color:#FFFFFF}");
    line->setFixedSize(m_nWindowH - int(40.0*m_dHradio), 2);
    line->setLineWidth(2);
    line->move(int(20.0*m_dHradio+0.5), int(536.0*m_dVradio));
}

void GameWidget::initPixmaps()
{
    m_pScene = new QGraphicsScene(0, 0, m_nWindowH, m_nWindowV);
    QPixmap oPix(":/images/img/Btn1-Blue.png");
    m_dBottomBtnW = oPix.width()*m_dHradio;
    m_dBottomBtnH = oPix.height()*m_dVradio;
    
    QPixmap oPixC(":/images/img/Clicked.png");
    m_dCircualW = oPixC.width() * m_dVradio;
    m_dCircualH = oPixC.height() * m_dVradio;
    
    m_dSlideDownBtnY = m_positionCoefficient * m_dVradio;
    initTrack1Pixmaps();
    initTrack2Pixmaps();
    initTrack3Pixmaps();
    initTrack4Pixmaps();
    setScene(m_pScene);
}

void GameWidget::initTrack1Pixmaps()
{
    m_pTrack1 = new TrackPixmaps();
    
    m_pTrack1->m_pShallowBtn = new Pixmap(":/images/img/Btn1-Blue.png", 
                                          QRectF(60.2*m_dHradio, 604*m_dVradio, m_dBottomBtnW, m_dBottomBtnH));
    m_pTrack1->m_pDeepBtn = new Pixmap(":/images/img/Btn1-Blue-Tip.png",
                                       QRectF(60.2*m_dHradio-4, 604*m_dVradio-4, m_dBottomBtnW+8, m_dBottomBtnH+8));
    m_pTrack1->m_pWhiteBtn = new Pixmap(":/images/img/Btn1-Click.png", 
                                        QRectF(60.2*m_dHradio,   604*m_dVradio, 
                                               m_dBottomBtnW, m_dBottomBtnH));
    m_pTrack1->m_pCircularBtn  = new Pixmap(":/images/img/Clicked.png", 
                                            QRectF(60.2*m_dHradio + (m_dBottomBtnW - m_dCircualW) /2, 549*m_dVradio,
                                                   m_dCircualW, m_dCircualH));
    double dTrack1BtnStartX = 430*m_dHradio;
    double dTrack1BtnEndX = 125*m_dHradio;
    m_pTrack1->m_pLeftArrow  = new Pixmap(":/images/img/Track1Left.png", QRectF( dTrack1BtnStartX, 0,m_dSlideDownBtnStartW, m_dSlideDownBtnStartH));
    m_pTrack1->m_pLeftArrow->setTargetGeo(QRectF(dTrack1BtnEndX, m_dSlideDownBtnY, m_dSlideDownBtnEndW, m_dSlideDownBtnEndH));
    
    m_pTrack1->m_pRightArrow  = new Pixmap(":/images/img/Track1Right.png", QRectF( dTrack1BtnStartX, 0,m_dSlideDownBtnStartW, m_dSlideDownBtnStartH));
    m_pTrack1->m_pRightArrow->setTargetGeo(QRectF(dTrack1BtnEndX, m_dSlideDownBtnY, m_dSlideDownBtnEndW, m_dSlideDownBtnEndH));
    
    m_pTrack1->m_pUpArrow  = new Pixmap(":/images/img/Track1Up.png", QRectF( dTrack1BtnStartX, 0,m_dSlideDownBtnStartW, m_dSlideDownBtnStartH));
    m_pTrack1->m_pUpArrow->setTargetGeo(QRectF(dTrack1BtnEndX, m_dSlideDownBtnY, m_dSlideDownBtnEndW, m_dSlideDownBtnEndH));
    
    m_pTrack1->m_pDownArrow  = new Pixmap(":/images/img/Track1Down.png", QRectF( dTrack1BtnStartX, 0,m_dSlideDownBtnStartW, m_dSlideDownBtnStartH));
    m_pTrack1->m_pDownArrow->setTargetGeo(QRectF(dTrack1BtnEndX, m_dSlideDownBtnY, m_dSlideDownBtnEndW, m_dSlideDownBtnEndH));
    
    m_pTrack1->m_pSliderBtn  = new Pixmap(":/images/img/Track1Btn.png", QRectF( dTrack1BtnStartX, 0,m_dSlideDownBtnStartW, m_dSlideDownBtnStartH));
    m_pTrack1->m_pSliderBtn->setTargetGeo(QRectF(dTrack1BtnEndX, m_dSlideDownBtnY, m_dSlideDownBtnEndW, m_dSlideDownBtnEndH));
    
    m_pTrack1->m_pSliderBtnClick  = new Pixmap(":/images/img/Track1BtnClick.png", QRectF(dTrack1BtnEndX, m_dSlideDownBtnY, m_dSlideDownBtnEndW, m_dSlideDownBtnEndH));
    m_pTrack1->m_pSliderBtnClick->setTargetGeo(QRectF(dTrack1BtnEndX,m_dSlideDownBtnY, m_dSlideDownBtnEndW, m_dSlideDownBtnEndH));
    
    m_pTrack1->initState();
    m_pTrack1->addToScene(m_pScene);
}

void GameWidget::initTrack2Pixmaps()
{
    m_pTrack2 = new TrackPixmaps();
    
    m_pTrack2->m_pShallowBtn = new Pixmap(":/images/img/Btn2-Yellow.png",
                                          QRectF(403*m_dHradio, 604*m_dVradio, m_dBottomBtnW, m_dBottomBtnH));
    
    m_pTrack2->m_pDeepBtn = new Pixmap(":/images/img/Btn2-Yellow-Tip.png", 
                                       QRectF(403*m_dHradio-4,604*m_dVradio-4, m_dBottomBtnW+8, m_dBottomBtnH+8));
    
    m_pTrack2->m_pWhiteBtn = new Pixmap(":/images/img/Btn2-Click.png", 
                                        QRectF(403*m_dHradio,604*m_dVradio, m_dBottomBtnW, m_dBottomBtnH));
    
    m_pTrack2->m_pCircularBtn  = new Pixmap(":/images/img/Clicked.png", 
                                            QRectF(403*m_dHradio+ (m_dBottomBtnW - m_dCircualW) /2,549*m_dVradio,
                                                   m_dCircualW, m_dCircualH));
    double dTrack2BtnStartX = 565*m_dHradio;
    double dTrack3BtnEndX = 415*m_dHradio;
    m_pTrack2->m_pLeftArrow  = new Pixmap(":/images/img/Track2Left.png", QRectF( dTrack2BtnStartX, 2,m_dSlideDownBtnStartW, m_dSlideDownBtnStartH));
    m_pTrack2->m_pLeftArrow->setTargetGeo(QRectF(dTrack3BtnEndX, m_dSlideDownBtnY, m_dSlideDownBtnEndW, m_dSlideDownBtnEndH));
    
    m_pTrack2->m_pRightArrow  = new Pixmap(":/images/img/Track2Right.png", QRectF(  dTrack2BtnStartX, 2,m_dSlideDownBtnStartW, m_dSlideDownBtnStartH));
    m_pTrack2->m_pRightArrow->setTargetGeo(QRectF(dTrack3BtnEndX, m_dSlideDownBtnY, m_dSlideDownBtnEndW, m_dSlideDownBtnEndH));
    
    m_pTrack2->m_pUpArrow  = new Pixmap(":/images/img/Track2Up.png", QRectF( dTrack2BtnStartX, 2,m_dSlideDownBtnStartW, m_dSlideDownBtnStartH));
    m_pTrack2->m_pUpArrow->setTargetGeo(QRectF(dTrack3BtnEndX, m_dSlideDownBtnY, m_dSlideDownBtnEndW, m_dSlideDownBtnEndH));
    
    m_pTrack2->m_pDownArrow  = new Pixmap(":/images/img/Track2Down.png", QRectF(  dTrack2BtnStartX, 2,m_dSlideDownBtnStartW, m_dSlideDownBtnStartH));
    m_pTrack2->m_pDownArrow->setTargetGeo(QRectF(dTrack3BtnEndX, m_dSlideDownBtnY, m_dSlideDownBtnEndW, m_dSlideDownBtnEndH));
    
    m_pTrack2->m_pSliderBtn  = new Pixmap(":/images/img/Track2Btn.png", QRectF(  dTrack2BtnStartX, 2,m_dSlideDownBtnStartW, m_dSlideDownBtnStartH));
    m_pTrack2->m_pSliderBtn->setTargetGeo(QRectF(dTrack3BtnEndX, m_dSlideDownBtnY, m_dSlideDownBtnEndW, m_dSlideDownBtnEndH));
    
    m_pTrack2->m_pSliderBtnClick  = new Pixmap(":/images/img/Track2BtnClick.png", QRectF( dTrack3BtnEndX, m_dSlideDownBtnY, m_dSlideDownBtnEndW, m_dSlideDownBtnEndH));
    m_pTrack2->m_pSliderBtnClick->setTargetGeo(QRectF(dTrack3BtnEndX, m_dSlideDownBtnY, m_dSlideDownBtnEndW, m_dSlideDownBtnEndH));
    
    m_pTrack2->initState();
    m_pTrack2->addToScene(m_pScene);
}

void GameWidget::initTrack3Pixmaps()
{
    m_pTrack3 = new TrackPixmaps();
    
    m_pTrack3->m_pShallowBtn = new Pixmap(":/images/img/Btn3-Blue.png",
                                          QRectF(747*m_dHradio,604*m_dVradio, m_dBottomBtnW, m_dBottomBtnH));
    m_pTrack3->m_pDeepBtn = new Pixmap(":/images/img/Btn3-Blue-Tip.png", 
                                       QRectF(747*m_dHradio -4,604*m_dVradio -4, m_dBottomBtnW+8, m_dBottomBtnH+8));
    m_pTrack3->m_pWhiteBtn = new Pixmap(":/images/img/Btn3-Click.png", 
                                        QRectF(747*m_dHradio,604*m_dVradio, m_dBottomBtnW, m_dBottomBtnH));
    m_pTrack3->m_pCircularBtn  = new Pixmap(":/images/img/Clicked.png",
                                            QRectF(747*m_dHradio+ (m_dBottomBtnW - m_dCircualW) /2, 549*m_dVradio,
                                                   m_dCircualW, m_dCircualH));
    double dTrack3BtnStartX = 710*m_dHradio;
    double dTrack3BtnEndX = 725*m_dHradio;
    m_pTrack3->m_pLeftArrow  = new Pixmap(":/images/img/Track3Left.png", QRectF( dTrack3BtnStartX, 0,m_dSlideDownBtnStartW, m_dSlideDownBtnStartH));
    m_pTrack3->m_pLeftArrow->setTargetGeo(QRectF(dTrack3BtnEndX, m_dSlideDownBtnY, m_dSlideDownBtnEndW, m_dSlideDownBtnEndH));
    
    m_pTrack3->m_pRightArrow  = new Pixmap(":/images/img/Track3Right.png", QRectF( dTrack3BtnStartX, 0,m_dSlideDownBtnStartW, m_dSlideDownBtnStartH));
    m_pTrack3->m_pRightArrow->setTargetGeo(QRectF(dTrack3BtnEndX, m_dSlideDownBtnY, m_dSlideDownBtnEndW, m_dSlideDownBtnEndH));
    
    m_pTrack3->m_pUpArrow  = new Pixmap(":/images/img/Track3Up.png", QRectF(  dTrack3BtnStartX, 0,m_dSlideDownBtnStartW, m_dSlideDownBtnStartH));
    m_pTrack3->m_pUpArrow->setTargetGeo(QRectF(dTrack3BtnEndX, m_dSlideDownBtnY, m_dSlideDownBtnEndW, m_dSlideDownBtnEndH));
    
    m_pTrack3->m_pDownArrow  = new Pixmap(":/images/img/Track3Down.png", QRectF( dTrack3BtnStartX, 0,m_dSlideDownBtnStartW, m_dSlideDownBtnStartH));
    m_pTrack3->m_pDownArrow->setTargetGeo(QRectF(dTrack3BtnEndX, m_dSlideDownBtnY, m_dSlideDownBtnEndW, m_dSlideDownBtnEndH));
    
    m_pTrack3->m_pSliderBtn  = new Pixmap(":/images/img/Track3Btn.png", QRectF( dTrack3BtnStartX, 0,m_dSlideDownBtnStartW, m_dSlideDownBtnStartH));
    m_pTrack3->m_pSliderBtn->setTargetGeo(QRectF(dTrack3BtnEndX, m_dSlideDownBtnY, m_dSlideDownBtnEndW, m_dSlideDownBtnEndH));
    
    m_pTrack3->m_pSliderBtnClick  = new Pixmap(":/images/img/Track3BtnClick.png", QRectF(dTrack3BtnEndX, m_dSlideDownBtnY, m_dSlideDownBtnEndW, m_dSlideDownBtnEndH));
    m_pTrack3->m_pSliderBtnClick->setTargetGeo(QRectF(dTrack3BtnEndX, m_dSlideDownBtnY, m_dSlideDownBtnEndW, m_dSlideDownBtnEndH));
    
    m_pTrack3->initState();
    m_pTrack3->addToScene(m_pScene);
}

void GameWidget::initTrack4Pixmaps()
{
    m_pTrack4 = new TrackPixmaps();
    double dTrack4BtnStartX1 = 1087*m_dHradio;
    double dTrack4BtnEndX1 = 604*m_dVradio;
    m_pTrack4->m_pShallowBtn = new Pixmap(":/images/img/Btn4-Yellow.png", 
                                          QRectF(dTrack4BtnStartX1, dTrack4BtnEndX1, m_dBottomBtnW, m_dBottomBtnH));
    m_pTrack4->m_pDeepBtn = new Pixmap(":/images/img/Btn4-Yellow-Tip.png",
                                       QRectF(dTrack4BtnStartX1-4, dTrack4BtnEndX1-4, m_dBottomBtnW+8, m_dBottomBtnH+8));
    m_pTrack4->m_pWhiteBtn = new Pixmap(":/images/img/Btn4-Click.png",
                                        QRectF(dTrack4BtnStartX1, dTrack4BtnEndX1, m_dBottomBtnW, m_dBottomBtnH));
    m_pTrack4->m_pCircularBtn  = new Pixmap(":/images/img/Clicked.png",
                                            QRectF(dTrack4BtnStartX1+ (m_dBottomBtnW - m_dCircualW) /2,549*m_dVradio,
                                                   m_dCircualW, m_dCircualH));
    double dTrack4BtnStartX = 845*m_dHradio;
    double dTrack4BtnEndX = 1020*m_dHradio;
    m_pTrack4->m_pLeftArrow  = new Pixmap(":/images/img/Track4Left.png", QRectF( dTrack4BtnStartX, 0,m_dSlideDownBtnStartW, m_dSlideDownBtnStartH));
    m_pTrack4->m_pLeftArrow->setTargetGeo(QRectF(dTrack4BtnEndX, m_dSlideDownBtnY, m_dSlideDownBtnEndW, m_dSlideDownBtnEndH));
    
    m_pTrack4->m_pRightArrow  = new Pixmap(":/images/img/Track4Right.png", QRectF( dTrack4BtnStartX, 0,m_dSlideDownBtnStartW, m_dSlideDownBtnStartH));
    m_pTrack4->m_pRightArrow->setTargetGeo(QRectF(dTrack4BtnEndX, m_dSlideDownBtnY, m_dSlideDownBtnEndW, m_dSlideDownBtnEndH));
    
    m_pTrack4->m_pUpArrow  = new Pixmap(":/images/img/Track4Up.png", QRectF( dTrack4BtnStartX, 0,m_dSlideDownBtnStartW, m_dSlideDownBtnStartH));
    m_pTrack4->m_pUpArrow->setTargetGeo(QRectF(dTrack4BtnEndX, m_dSlideDownBtnY, m_dSlideDownBtnEndW, m_dSlideDownBtnEndH));
    
    m_pTrack4->m_pDownArrow  = new Pixmap(":/images/img/Track4Down.png", QRectF( dTrack4BtnStartX, 0,m_dSlideDownBtnStartW, m_dSlideDownBtnStartH));
    m_pTrack4->m_pDownArrow->setTargetGeo(QRectF(dTrack4BtnEndX, m_dSlideDownBtnY, m_dSlideDownBtnEndW, m_dSlideDownBtnEndH));
    
    m_pTrack4->m_pSliderBtn  = new Pixmap(":/images/img/Track4Btn.png", QRectF( dTrack4BtnStartX, 0,m_dSlideDownBtnStartW, m_dSlideDownBtnStartH));
    m_pTrack4->m_pSliderBtn->setTargetGeo(QRectF(dTrack4BtnEndX, m_dSlideDownBtnY, m_dSlideDownBtnEndW, m_dSlideDownBtnEndH));
    
    m_pTrack4->m_pSliderBtnClick  = new Pixmap(":/images/img/Track4BtnClick.png", QRectF( dTrack4BtnEndX, m_dSlideDownBtnY,
                                                                                       m_dSlideDownBtnEndW, m_dSlideDownBtnEndH));
    m_pTrack4->m_pSliderBtnClick->setTargetGeo(QRectF(dTrack4BtnEndX, m_dSlideDownBtnY, m_dSlideDownBtnEndW, m_dSlideDownBtnEndH));
    m_pTrack4->initState();
    m_pTrack4->addToScene(m_pScene);
}

void GameWidget::initMiddleElement()
{
    m_pProgress = new CusProgress(this);
    m_pProgress->move(m_nWindowH /2 - 115, 150*m_dHradio);
    m_pProgress->setVisible(false);
    
    
    m_pHairClipper = new Hairclipper(this);
    m_pHairClipper->setVisible(false);
    
    m_pLabel = new EnglistLabel(this);
    m_pLabel->move(m_nWindowH /2 - 115, 100*m_dHradio);
    m_pLabel->setVisible(false);
    
    m_pMiddleDishes = new Pixmap(":/images/img/dishes.png", QRectF((double)m_nWindowH /2.0 -90.0*m_dVradio , 
                                                                150.0*m_dHradio,180*m_dVradio, 180*m_dVradio));
    m_pMiddleDishes->setVisible(false);
    m_pMidDishesAnimation = new QPropertyAnimation(m_pMiddleDishes, "RotateAngle");
    connect(m_pMidDishesAnimation, SIGNAL(finished()), this, SLOT(startDishesAnimation()));
    m_nDishesRotateNum = 0;
    
    m_pScene->addItem(m_pMiddleDishes);
    
    QPixmap oPix(":/images/img/MiddleBtn.png");
    m_pMiddleBtn = new Pixmap(":/images/img/MiddleBtn.png", QRectF((double)m_nWindowH /2.0 -oPix.width()*m_dHradio/2, 
                                                                150.0*m_dHradio,oPix.width()*m_dHradio, oPix.height()*m_dVradio));
    m_pMiddleBtn->setVisible(false);
    m_pMiddleBtnClick = new Pixmap(":/images/img/MiddleBtnClick.png", QRectF((double)m_nWindowH /2.0 -oPix.width()*m_dHradio/2, 
                                                                          150.0*m_dHradio,oPix.width()*m_dHradio, oPix.height()*m_dVradio));
    m_pMiddleBtnClick->setVisible(false);
    m_pScene->addItem(m_pMiddleBtn);
    m_pScene->addItem(m_pMiddleBtnClick);
}

void GameWidget::startDishesAnimation()
{
    m_pMiddleDishes->setVisible(true);
    m_pMiddleDishes->resetPix(QPixmap(":/images/img/dishes.png").scaled(180*m_dVradio,180*m_dVradio, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    m_pMiddleDishes->setGeometry(QRectF(  (double)m_nWindowH /2.0 -90.0*m_dVradio, 150.0*m_dHradio,180*m_dVradio, 180*m_dVradio));
    
    m_pMidDishesAnimation->setStartValue(0);
    m_pMidDishesAnimation->setEndValue(30);
    m_pMidDishesAnimation->setEasingCurve(QEasingCurve::Linear);
    if(m_nDishesRotateNum%2 == 0)
    {
        m_pMidDishesAnimation->setDirection(QAbstractAnimation::Direction::Forward);
    }
    else
    {
        m_pMidDishesAnimation->setDirection(QAbstractAnimation::Direction::Backward);
    }
    m_pMidDishesAnimation->setDuration(375);              
    m_pMidDishesAnimation->start();
    //animationActivate(m_pMidDishesAnimation);
    m_nDishesRotateNum++;
}

void GameWidget::Trigger0()
{
    m_pLabel->setVisible(true);
    m_pLabel->setText("SCRATCH-RIGHT");
    m_pTrack1->m_pShallowBtn->setVisible(false);
    m_pTrack2->m_pShallowBtn->setVisible(false);
    m_pTrack3->m_pShallowBtn->setVisible(false);
    m_pTrack4->m_pShallowBtn->setVisible(false);
    QParallelAnimationGroup*pGroup = new QParallelAnimationGroup();
    pGroup->addAnimation(m_pTrack1->m_pDeepBtn->getOpacityAnimation(1500));
    pGroup->addAnimation(m_pTrack2->m_pDeepBtn->getOpacityAnimation(1500));
    pGroup->addAnimation(m_pTrack3->m_pDeepBtn->getOpacityAnimation(1500));
    pGroup->addAnimation(m_pTrack4->m_pDeepBtn->getOpacityAnimation(1500));
    pGroup->start();
    //animationActivate(pGroup);
    startDishesAnimation();
}

void GameWidget::Trigger1500()
{
    m_pLabel->setVisible(false);
    m_pMidDishesAnimation->stop();
    m_pTrack1->m_pShallowBtn->setVisible(true);
    m_pTrack2->m_pShallowBtn->setVisible(true);
    m_pTrack3->m_pShallowBtn->setVisible(true);
    m_pTrack4->m_pShallowBtn->setVisible(true);
    m_pMiddleDishes->setVisible(false);
    m_pTrack1->m_pDeepBtn->setVisible(false);
    m_pTrack2->m_pDeepBtn->setVisible(false);
    m_pTrack3->m_pDeepBtn->setVisible(false);
    m_pTrack4->m_pDeepBtn->setVisible(false);
}

void GameWidget::Trigger11000()
{
    m_pTrack4->m_pLeftArrow->getMoveAnimation(3000)->start();
    //animationActivate(m_pTrack4->m_pLeftArrow->getMoveAnimation(3000));
    if(bTip)
    {
        m_pTrack4->m_pDeepBtn->setVisible(true);
    }
}

void GameWidget::Trigger14000()
{
    m_pProgress->setVisible(true);
    m_pProgress->activate(50, 0, 1000);
    m_pLabel->setText("FILTER-RIGHT");
    m_pLabel->setVisible(true);
    m_pTrack4->m_pDeepBtn->setVisible(true);
    m_pTrack4->m_pWhiteBtn->setVisible(true);
    m_pTrack4->m_pCircularBtn->setVisible(true);
}

void GameWidget::Trigger15000()
{
    //m_pProgress->activate(0, 50, 900, true);
}

void GameWidget::Trigger16000()
{
    m_pProgress->setVisible(false);
    m_pLabel->setVisible(false);
    m_pTrack4->m_pLeftArrow->setVisible(false);
    m_pTrack4->m_pDeepBtn->setVisible(false);
    m_pTrack4->m_pWhiteBtn->setVisible(false);
    m_pTrack4->m_pCircularBtn->setVisible(false);
}

void GameWidget::Trigger22000()
{
    m_pTrack3->m_pRightArrow->getMoveAnimation(3000)->start();
    //animationActivate(m_pTrack3->m_pRightArrow->getMoveAnimation(3000));
    if(bTip)
    {
        m_pTrack3->m_pDeepBtn->setVisible(true);
    }
}

void GameWidget::Trigger25000()
{
    m_pProgress->setVisible(true);
    m_pProgress->activate(0, 50, 2000);
    m_pLabel->setText("FX-RIGHT");
    m_pLabel->setVisible(true);
    m_pTrack3->m_pDeepBtn->setVisible(true);
    m_pTrack3->m_pWhiteBtn->setVisible(true);
    m_pTrack3->m_pCircularBtn->setVisible(true);
}

void GameWidget::Trigger27500()
{
    m_pProgress->activate(50, 0, 2000);
}

void GameWidget::Trigger30000()
{
    m_pProgress->setVisible(false);
    m_pLabel->setVisible(false);
    m_pTrack3->m_pRightArrow->setVisible(false);
    m_pTrack3->m_pDeepBtn->setVisible(false);
    m_pTrack3->m_pWhiteBtn->setVisible(false);
    m_pTrack3->m_pCircularBtn->setVisible(false);
}

void GameWidget::Trigger34000()
{
    m_pTrack1->m_pLeftArrow->getMoveAnimation(3000)->start();
    //animationActivate(m_pTrack1->m_pLeftArrow->getMoveAnimation(3000));
    if(bTip)
    {
        m_pTrack1->m_pDeepBtn->setVisible(true);
    }
}

void GameWidget::Trigger37000()
{
    m_pProgress->setVisible(true);
    m_pProgress->activate(50, 0, 1000);
    m_pLabel->setText("FILTER-LEFT");
    m_pLabel->setVisible(true);
    m_pTrack1->m_pDeepBtn->setVisible(true);
    m_pTrack1->m_pWhiteBtn->setVisible(true);
    m_pTrack1->m_pCircularBtn->setVisible(true);
}

void GameWidget::Trigger38000()
{
    m_pProgress->setVisible(false);
    m_pLabel->setVisible(false);
    m_pTrack1->m_pLeftArrow->setVisible(false);
    m_pTrack1->m_pDeepBtn->setVisible(false);
    m_pTrack1->m_pWhiteBtn->setVisible(false);
    m_pTrack1->m_pCircularBtn->setVisible(false);
    
    m_pTrack2->m_pUpArrow->getMoveAnimation(2000)->start();
    //animationActivate(m_pTrack2->m_pUpArrow->getMoveAnimation(2000));
    if(bTip)
    {
        m_pTrack2->m_pDeepBtn->setVisible(true);
    }
}

void GameWidget::Trigger40000()
{
    m_pHairClipper->setVisible(true);
    // TODO  startHairClpiier Up 1000
    m_pHairClipper->activate(0, 100, 10000, Qt::Vertical, true, Direction::ArrowUp);
    m_pHairClipper->move(m_nWindowH /2 - m_pHairClipper->width()/2, 150*m_dHradio);
    
    
    m_pLabel->setVisible(true);
    m_pLabel->setText("VOL-LEFT");
    
    m_pTrack2->m_pDeepBtn->setVisible(true);
    m_pTrack2->m_pWhiteBtn->setVisible(true);
    m_pTrack2->m_pCircularBtn->setVisible(true);
}

void GameWidget::Trigger47000()
{
    m_pTrack4->m_pDownArrow->getMoveAnimation(3500)->start();
    //animationActivate(m_pTrack4->m_pDownArrow->getMoveAnimation(3500));
    if(bTip)
    {
        m_pTrack4->m_pDeepBtn->setVisible(true);
    }
    QTimer::singleShot(3500, this, &GameWidget::Trigger50500);
}

void GameWidget::Trigger50000()
{
    m_pHairClipper->setVisible(false);
    m_pLabel->setVisible(false);
    m_pTrack2->m_pUpArrow->setVisible(false);
    m_pTrack2->m_pDeepBtn->setVisible(false);
    m_pTrack2->m_pWhiteBtn->setVisible(false);
    m_pTrack2->m_pCircularBtn->setVisible(false);
}

void GameWidget::Trigger50500()
{
    m_pHairClipper->setVisible(true);
    // TODO  startHairClpiier Down  500
    m_pHairClipper->activate(0, 100, 450, Qt::Vertical, false, Direction::ArrowDown);
    m_pHairClipper->move(m_nWindowH /2 - m_pHairClipper->width()/2, 150*m_dHradio);
    
    m_pLabel->setVisible(true);
    m_pLabel->setText("VOL-RIGHT");
    
    m_pTrack4->m_pDeepBtn->setVisible(true);
    m_pTrack4->m_pWhiteBtn->setVisible(true);
    m_pTrack4->m_pCircularBtn->setVisible(true);
    
    QTimer::singleShot(500, this, &GameWidget::Trigger51000);
}

void GameWidget::Trigger51000()
{
    m_pHairClipper->setVisible(false);
    m_pLabel->setVisible(false);
    m_pTrack4->m_pDeepBtn->setVisible(false);
    m_pTrack4->m_pWhiteBtn->setVisible(false);
    m_pTrack4->m_pCircularBtn->setVisible(false);
    m_pTrack4->m_pDownArrow->setVisible(false);
}

void GameWidget::Trigger72000()
{
    m_pTrack1->m_pLeftArrow->getMoveAnimation(3000)->start();
    //animationActivate(m_pTrack1->m_pLeftArrow->getMoveAnimation(3000));
    if(bTip)
    {
        m_pTrack1->m_pDeepBtn->setVisible(true);
    }
}

void GameWidget::Trigger75000()
{
    m_pProgress->setVisible(true);
    m_pProgress->activate(50, 0, 900);
    
    m_pLabel->setVisible(true);
    m_pLabel->setText("FILTER-LEFT");
    
    m_pTrack1->m_pDeepBtn->setVisible(true);
    m_pTrack1->m_pWhiteBtn->setVisible(true);
    m_pTrack1->m_pCircularBtn->setVisible(true);
    
    QTimer::singleShot(1000, this, &GameWidget::Trigger76000);
}

void GameWidget::Trigger76000()
{
    m_pProgress->setVisible(true);
    m_pProgress->activate(0, 50, 900);
}

void GameWidget::Trigger77000()
{
    m_pProgress->setVisible(false);
    m_pLabel->setVisible(false);
    m_pTrack1->m_pDeepBtn->setVisible(false);
    m_pTrack1->m_pWhiteBtn->setVisible(false);
    m_pTrack1->m_pCircularBtn->setVisible(false);
    m_pTrack1->m_pLeftArrow->setVisible(false);
}

void GameWidget::Trigger88000()
{
    m_pTrack2->m_pSliderBtn->getMoveAnimation(3000)->start();
    //animationActivate(m_pTrack2->m_pSliderBtn->getMoveAnimation(3000));
    if(bTip)
    {
        m_pTrack2->m_pDeepBtn->setVisible(true);
    }
}

void GameWidget::Trigger91000()
{
    m_pTrack2->m_pSliderBtnClick->setVisible(true);
    m_pTrack2->m_pSliderBtn->setVisible(false);
    m_pTrack2->m_pDeepBtn->setVisible(true);
    m_pTrack2->m_pWhiteBtn->setVisible(true);
    m_pTrack2->m_pCircularBtn->setVisible(true);
    m_pMiddleBtn->setVisible(true);
    QTimer::singleShot(500, this, &GameWidget::Trigger91500);
}

void GameWidget::Trigger91500()
{
    m_pMiddleBtn->setVisible(false);
    m_pMiddleBtnClick->setVisible(true);
    QTimer::singleShot(500, this, &GameWidget::Trigger92000);
}

void GameWidget::Trigger92000()
{
    m_pTrack2->m_pSliderBtnClick->setVisible(false);
    m_pTrack2->m_pDeepBtn->setVisible(false);
    m_pTrack2->m_pWhiteBtn->setVisible(false);
    m_pTrack2->m_pCircularBtn->setVisible(false);
    m_pMiddleBtnClick->setVisible(false);
    
//    
    qDebug() << "Animation Finish";
    // Test
//    QTimer::singleShot(500, this, &GameWidget::Trigger0);
}
