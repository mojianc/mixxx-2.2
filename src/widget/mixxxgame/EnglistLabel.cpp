#include "EnglistLabel.h"
#include <QFont>
#include <QPainter>
#include <QPen>

#include <QFontDatabase>
#include <QCoreApplication>

EnglistLabel::EnglistLabel(QWidget *parent, Qt::WindowFlags f):
    QLabel(parent, f)
{
    int fontId = QFontDatabase::addApplicationFont(QCoreApplication::applicationDirPath() + "/fonts/Bitsumishi.ttf");
    QString msyh = QFontDatabase::applicationFontFamilies(fontId).at(0);
    
    
    setFixedSize(230, 45);
    QFont font (msyh, 22);
    setFont(font);
    setAlignment(Qt::AlignCenter);
}

EnglistLabel::~EnglistLabel()
{
    
}

void EnglistLabel::setText(const QString &strText)
{
    m_strText = strText;
}

void EnglistLabel::paintEvent(QPaintEvent *pEvent)
{
    QPainter oPainter(this);
    QPen oPen;
    oPen.setWidth(1);
    oPen.setColor(Qt::blue);
    
    oPainter.setRenderHint(QPainter::Antialiasing, true);
    oPainter.setPen(oPen);
    
    QLinearGradient linearGrad;
    linearGrad.setColorAt(0, Qt::white);            // 字体利用特定颜色来填充
    
    QFont font = this->font();
    
    QPainterPath textPath;
    QRect painterRect = rect();
    
    QFontMetrics metrics(font);
    int len = metrics.width(m_strText);
    int w = width();
    int px = (len - w) / 2;
    if(px < 0)
    {
        px = -px;
    }
    textPath.addText(px, painterRect.bottom() / 2, font, m_strText);
    oPainter.setBrush(linearGrad);
    oPainter.drawPath(textPath);
    
    QLabel::paintEvent(pEvent);
}
