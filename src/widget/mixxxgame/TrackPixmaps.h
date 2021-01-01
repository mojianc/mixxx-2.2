#ifndef TRACKPIXMAPMANAGER_H
#define TRACKPIXMAPMANAGER_H
//TrackPixmaps


#include "Pixmap.h"

class TrackPixmaps
{
public:
    TrackPixmaps();
    
    void initState();
    
    void addToScene(QGraphicsScene *pScene);
    
    Pixmap *m_pShallowBtn;
    Pixmap *m_pDeepBtn;
    Pixmap *m_pWhiteBtn;
    Pixmap *m_pCircularBtn;
    Pixmap *m_pLeftArrow;
    Pixmap *m_pRightArrow;
    Pixmap *m_pUpArrow;
    Pixmap *m_pDownArrow;
    Pixmap *m_pSliderBtn;
    Pixmap *m_pSliderBtnClick;
};
#endif // TRACKPIXMAPMANAGER_H
