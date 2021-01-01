#ifndef PIXMAP_H
#define PIXMAP_H

#include <QGraphicsView>
#include <QGraphicsWidget>
#include <QPropertyAnimation>

class QStateMachine;
class QState;
class QParallelAnimationGroup;

class Pixmap : public QGraphicsWidget
{
    Q_OBJECT
    
    Q_PROPERTY(int RotateAngle READ RotateAngle WRITE setRotateAngle)
    
public:
    Pixmap(const QPixmap &oPix, QRectF oInitGeo, QGraphicsItem *parent = 0);
    Pixmap(const QString& strPixDir, QRectF oGeo, QGraphicsItem *parent = 0);

    int RotateAngle();

    void setRotateAngle(int nAngle);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
    
    void setGeometry(const QRectF &oRect) override;
    
    void resetPix(const QPixmap &oPix);
    
    void setTargetGeo(const QRectF& oTarget);
    
    QPropertyAnimation* getMoveAnimation(int nMsecs);
    QPropertyAnimation* getOpacityAnimation(int nMsecs);
    
    QRectF targetGeo();
private:
        void returnToInitGeo();
private slots:
    void moveAnimationFinish();
private:
    double m_dAngle;
    QPixmap m_oOriginPix;
    QPixmap m_oScaledPix;
    QRectF m_oInitGeo;
    QRectF m_oTargetGeo;
    QString m_strPicName;
};

#endif // PIXMAP_H
