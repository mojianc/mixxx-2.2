#ifndef ENGLISTLABEL_H
#define ENGLISTLABEL_H
//EnglistLabel


#include <QLabel>

class EnglistLabel : public QLabel
{
    Q_OBJECT
    
public:
    explicit EnglistLabel(QWidget *parent=Q_NULLPTR, Qt::WindowFlags f=Qt::WindowFlags());
    ~EnglistLabel();
    
    
    virtual void setText(const QString& strText);
    
    virtual void paintEvent(QPaintEvent* pEvent);
    
private:
    QString m_strText;
};
#endif // ENGLISTLABEL_H
