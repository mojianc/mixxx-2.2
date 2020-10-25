#pragma once

#include <QThread>
#include "FtUnitl.h"
#include <QMutex>

class FtTask : public QThread
{
	Q_OBJECT

public:
	FtTask(QObject *parent = NULL);
	~FtTask();

	void stop();
	/* index--0-29
	*/
	void setBuff(int index, unsigned char data);
	unsigned char getBuff(int index);
	void update();
	void clearBuff();

signals:
	void mouseSpeed(int speed);
	void mouseDirect(int direct);

protected:
	void run();
	void updateBuff();
	void updateTest();
	void readMouseSpeed();
	void readMouseDirect();
	void ftReset();
	void flashLed();
	void setUpdate(bool enable);
private:
	bool			m_stopped;
	FtUnitl         m_ftUnitl;
	bool            m_connected;
	unsigned char   *m_displayBuf;
    unsigned char   *m_displaySwapBuf;
	bool            m_update;
	QMutex          m_mutex;
};
