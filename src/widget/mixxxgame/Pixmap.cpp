#include "Pixmap.h"

Pixmap::Pixmap(const QPixmap &oPix, QRectF oInitGeo, QGraphicsItem *parent)
    : QGraphicsWidget(parent), m_oOriginPix(oPix), m_oScaledPix(oPix),
      m_oInitGeo(oInitGeo),
      m_oTargetGeo(oInitGeo),
      m_strPicName("")
{
    setGeometry(oInitGeo);
}

Pixmap::Pixmap(const QString &strPixDir, QRectF oGeo, QGraphicsItem *parent)
    : QGraphicsWidget(parent),
      m_oInitGeo(oGeo),
      m_oTargetGeo(oGeo),
      m_strPicName(strPixDir)
{
     setGeometry(oGeo);
    m_oOriginPix = QPixmap(strPixDir).scaled(oGeo.width(),oGeo.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    m_oScaledPix = QPixmap(strPixDir).scaled(oGeo.width(),oGeo.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

int Pixmap::RotateAngle()
{
    return m_dAngle;
}

void Pixmap::setRotateAngle(int nAngle)
{
    m_dAngle = nAngle;
    setTransform(QTransform().
                 translate(this->boundingRect().width()/2,this->boundingRect().height()/2).
                 rotate(nAngle, Qt::ZAxis).
                 translate(0-this->boundingRect().width()/2,0-this->boundingRect().height()/2));
}

void Pixmap::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->drawPixmap(QPointF(), m_oScaledPix);
}

void Pixmap::setGeometry(const QRectF &oRect)
{
    QGraphicsWidget::setGeometry(oRect);
    
    if (oRect.size().width() > m_oOriginPix.size().width())
        m_oScaledPix = m_oOriginPix.scaled(oRect.size().toSize());
    else
        m_oScaledPix = m_oOriginPix;
}

void Pixmap::resetPix(const QPixmap &oPix)
{
    m_oOriginPix = oPix;
    m_oScaledPix = oPix;
}

void Pixmap::returnToInitGeo()
{
    if(!m_strPicName.isEmpty())
    {
        resetPix(QPixmap(m_strPicName).scaled(m_oInitGeo.width(),m_oInitGeo.height()
                                              , Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }
    setGeometry(m_oInitGeo);
}

void Pixmap::setTargetGeo(const QRectF &oTarget)
{
    m_oTargetGeo = oTarget;
}

QPropertyAnimation *Pixmap::getMoveAnimation(int nMsecs)
{
    setVisible(true);
    returnToInitGeo();
    QPropertyAnimation* pScaleAnimation = new QPropertyAnimation(this, "geometry");
    pScaleAnimation->setStartValue(m_oInitGeo);
    pScaleAnimation->setEndValue(m_oTargetGeo);
    pScaleAnimation->setEasingCurve(QEasingCurve::Linear);
    pScaleAnimation->setDuration(nMsecs);    
    connect(pScaleAnimation, SIGNAL(finished()), this, SLOT(moveAnimationFinish()));    
    return pScaleAnimation;
}

QPropertyAnimation *Pixmap::getOpacityAnimation(int nMsecs)
{
    setVisible(true);
    QPropertyAnimation* pAlphaAnimation = new QPropertyAnimation(this, "opacity");
    pAlphaAnimation->setStartValue(0);
    pAlphaAnimation->setEndValue(1);
    pAlphaAnimation->setDuration(nMsecs);
    return pAlphaAnimation;
}

QRectF Pixmap::targetGeo()
{
    return m_oTargetGeo;
}

void Pixmap::moveAnimationFinish()
{
    if(!m_strPicName.isEmpty())
    {
        resetPix(QPixmap(m_strPicName).scaled(m_oTargetGeo.width(),m_oTargetGeo.height()
                                              , Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }
}
