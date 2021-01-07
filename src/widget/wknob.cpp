/***************************************************************************
                          wknob.cpp  -  description
                             -------------------
    begin                : Fri Jun 21 2002
    copyright            : (C) 2002 by Tue & Ken Haste Andersen
    email                : haste@diku.dk
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include <QtDebug>
#include <QMouseEvent>
#include <QWheelEvent>

#include "util/duration.h"
#include "widget/wknob.h"

WKnob::WKnob(QWidget* pParent)
        : WDisplay(pParent),
          m_renderTimer(mixxx::Duration::fromMillis(20),
                        mixxx::Duration::fromSeconds(1)) {
    connect(&m_renderTimer, SIGNAL(update()),
            this, SLOT(update()));
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeupdate()));
}

void WKnob::resizeEvent(QResizeEvent *pEvent)
{
    m_size = pEvent->size();
    WWidget::resizeEvent(pEvent);
}


void WKnob::getComingData(QByteArray data, QRect rect)
{
    unsigned short  x = (data[3] << 8) + data[2];
    unsigned short  y = (data[5] << 8) + data[4];

    double ratioX = (x - rect.left())*1.0000 / rect.width();
    double ratioy = (y - rect.top())*1.0000 / rect.height();
    int PointX = ratioX * m_size.width();
    int Pointy = ratioy * m_size.height();

    m_timer->stop();
    m_timer->start(100);
    if(!m_inMove)
    {
        m_inMove =true;
        m_lastPoint = QPointF(PointX, Pointy);
        QMouseEvent event(QEvent::MouseButtonPress, QPointF(PointX, Pointy), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
        m_handler.mousePressEvent(this, &event);
    }
    QMouseEvent event(QEvent::MouseMove, QPointF(PointX, Pointy), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    m_handler.mouseMoveEvent(this, &event);
    m_lastPoint = QPointF(PointX, Pointy);
}

void WKnob::timeupdate()
{
    qDebug()<<this->metaObject()->className()<<":WKnobComposed::timeupdate() ";
    m_timer->stop();
    m_inMove = false;

    QMouseEvent event(QEvent::MouseButtonRelease, m_lastPoint, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    m_handler.mouseReleaseEvent(this, &event);
}

void WKnob::mouseMoveEvent(QMouseEvent* e) {
    m_handler.mouseMoveEvent(this, e);
}

void WKnob::mousePressEvent(QMouseEvent* e) {
    m_handler.mousePressEvent(this, e);
}

void WKnob::mouseReleaseEvent(QMouseEvent* e) {
    m_handler.mouseReleaseEvent(this, e);
}

void WKnob::wheelEvent(QWheelEvent* e) {
    m_handler.wheelEvent(this, e);
}

void WKnob::inputActivity() {
#ifdef __APPLE__
    m_renderTimer.activity();
#else
    update();
#endif
}
