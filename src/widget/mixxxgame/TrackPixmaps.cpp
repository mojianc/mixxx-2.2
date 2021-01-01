#include "TrackPixmaps.h"

TrackPixmaps::TrackPixmaps()
    :
      m_pShallowBtn(nullptr),
      m_pDeepBtn(nullptr),
      m_pWhiteBtn(nullptr),
      m_pCircularBtn(nullptr),
      m_pLeftArrow(nullptr),
      m_pRightArrow(nullptr),
      m_pUpArrow(nullptr),
      m_pSliderBtn(nullptr),
      m_pDownArrow(nullptr),
      m_pSliderBtnClick(nullptr)
{
    
}

void TrackPixmaps::initState()
{
    if(m_pShallowBtn)  m_pShallowBtn->setVisible(true);
    if(m_pDeepBtn)  m_pDeepBtn->setVisible(false);
    if(m_pWhiteBtn)  m_pWhiteBtn->setVisible(false);
    if(m_pCircularBtn)  m_pCircularBtn->setVisible(false);
    if(m_pLeftArrow)  m_pLeftArrow->setVisible(false);
    if(m_pRightArrow)  m_pRightArrow->setVisible(false);
    if(m_pUpArrow)  m_pUpArrow->setVisible(false);
    if(m_pDownArrow)  m_pDownArrow->setVisible(false);
    if(m_pSliderBtn)  m_pSliderBtn->setVisible(false);
    if(m_pSliderBtnClick)  m_pSliderBtnClick->setVisible(false);
}

void TrackPixmaps::addToScene(QGraphicsScene *pScene)
{
    if(m_pShallowBtn)  pScene->addItem(m_pShallowBtn);
    if(m_pDeepBtn) pScene->addItem(m_pDeepBtn);
    if(m_pWhiteBtn)  pScene->addItem(m_pWhiteBtn);
    if(m_pCircularBtn) pScene->addItem(m_pCircularBtn);
    if(m_pLeftArrow)  pScene->addItem(m_pLeftArrow);
    if(m_pRightArrow)  pScene->addItem(m_pRightArrow);
    if(m_pUpArrow)  pScene->addItem(m_pUpArrow);
    if(m_pDownArrow)  pScene->addItem(m_pDownArrow);
    if(m_pSliderBtn)  pScene->addItem(m_pSliderBtn);
    if(m_pSliderBtnClick)  pScene->addItem(m_pSliderBtnClick);
}
