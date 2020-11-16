#pragma once

#include <QThread>
#include "FtUnitl.h"
#include <QMutex>

class FtTask : public QThread
{
	Q_OBJECT

public:
    static FtTask *getInstance()
    {
       if(m_instance == NULL)
       {
           m_instance = new FtTask;
       }
       return m_instance;
    }
	~FtTask();

	void stop();
	/* index--0-29
	*/
	void setBuff(int index, unsigned char data);
	unsigned char getBuff(int index);
	void update();
	void clearBuff();

    void setLED_ON(int byte, int bit);
    void setLED_OFF(int byte, int bit);
    void led_update();

protected:
	void run();
	void updateBuff();
	void ftReset();
	void setUpdate(bool enable);
private:
    FtTask(QObject *parent = NULL);
	bool			m_stopped;
	FtUnitl         m_ftUnitl;
	bool            m_connected;
	unsigned char   *m_displayBuf;
    unsigned char   *m_displaySwapBuf;
	bool            m_update;
	QMutex          m_mutex;
    static FtTask *m_instance;
};
