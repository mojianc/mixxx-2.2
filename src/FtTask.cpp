#include "FtTask.h"

#define COLUMN_SIZE 15   //0-14,but for cpld 4bits is 0-15
#define ROW_SIZE    15
#define DISPLAY_BUFF_SIZE COLUMN_SIZE*2

FtTask::FtTask(QObject *parent)
	: QThread(parent), m_stopped(true),m_connected(false),m_update(false)
{
	m_displayBuf = new unsigned char[DISPLAY_BUFF_SIZE];
	for(int j=0;j<COLUMN_SIZE;j++){
		for (int i = 0; i < 2; i++) {
			m_displayBuf[j*2 + i] = 0x00;
        }
	}
    m_displaySwapBuf = new unsigned char[DISPLAY_BUFF_SIZE];
    for(int j=0;j<COLUMN_SIZE;j++){
        for (int i = 0; i < 2; i++) {
            m_displaySwapBuf[j*2 + i] = 0x00;
        }
    }
}

FtTask::~FtTask()
{
	stop();
	quit();
	wait();
	delete[] m_displayBuf;
    delete[] m_displaySwapBuf;
}

void FtTask::stop()
{
	m_stopped = true;
}

void FtTask::setBuff(int index, unsigned char data)
{
    m_mutex.lock();
	if (index < DISPLAY_BUFF_SIZE) {
		m_displayBuf[index] = data;
	}
    m_mutex.unlock();
}

unsigned char FtTask::getBuff(int index)
{
    m_mutex.lock();
	if (index < DISPLAY_BUFF_SIZE) {
        m_mutex.unlock();
		return m_displayBuf[index];
    }
    m_mutex.unlock();
	return 0;
}

void FtTask::update()
{
	setUpdate(true);
}

void FtTask::clearBuff()
{
    m_mutex.lock();
	for (int j = 0; j < COLUMN_SIZE; j++) {
		for (int i = 0; i < 2; i++) {
			m_displayBuf[j * 2 + i] = 0x00;
		}
	}
    m_mutex.unlock();
}

void FtTask::run()
{
	m_stopped = false;
	while (!m_stopped) {
		if (!m_ftUnitl.isPortAOpen()) {
			m_ftUnitl.checkDevice();
			if (!m_ftUnitl.isPortAOpen()) {
				QThread::currentThread()->sleep(1);
			}
			else {
				if (m_ftUnitl.InitDev() == false) {
					qDebug("init device fail!");
					m_ftUnitl.Close_USB_Device();
				}
				ftReset();
			}
		}
		else {
			if (m_update) {
				setUpdate(false);
				updateBuff();				
			}
            //updateTest();
//            readMouseSpeed();
//            readMouseDirect();
//			flashLed();
			QThread::currentThread()->msleep(20);
		}

	}
	if (m_ftUnitl.isPortAOpen()) {
		m_ftUnitl.Write_Short_Add(0, 0);
	}
}

void FtTask::updateBuff()
{
    unsigned char *pBuf = m_displaySwapBuf;
    m_mutex.lock();
    for(int j=0;j<COLUMN_SIZE;j++){
        for (int i = 0; i < 2; i++) {
            m_displaySwapBuf[j*2 + i] = m_displayBuf[j*2 + i];
        }
    }
    m_mutex.unlock();

	m_ftUnitl.Write_Short_Add(2, 0);
	for (int j = 0; j < COLUMN_SIZE; j++) {
		if (j == 0) {
			m_ftUnitl.Write_Short_Add(0, pBuf[0]);
			//m_ftUnitl.Write_Short_Add(1, pBuf[0]);
			m_ftUnitl.Write_Short_Add(1, pBuf[1]);
		}
		else {
			for (int i = 0; i < 2; i++) {
				m_ftUnitl.Write_Short_Add(1, pBuf[j*2 + i]);
			}
		}
	}
	m_ftUnitl.Write_Short_Add(3, 0);
}

void FtTask::updateTest()
{
//    while(1)
//    {
        //ÁÁD5,D6,D205
       unsigned char result = getBuff(0);
         result |= (1 << 4);
         result |= (1 << 5);
        setBuff(0, result);
       result = getBuff(1);
       result |= (1 << 6);
        setBuff(1, result);
        update();

        //ÃðD5,D6,D205
        result = getBuff(0);
        result &= ~(1 << 4);
        result &= ~(1 << 5);
        setBuff(0, result);
        result = getBuff(1);
        result &= ~(1 << 6);
        setBuff(1, result);
        update();
//    }
}

void FtTask::readMouseSpeed()
{
	//m_ftUnitl.Read_Short_Add(0);
	//m_ftUnitl.GetData();
	//QByteArray datas = m_ftUnitl.readReceivedDatas();
	//if (datas.length() > 0) {
	//	emit mouseSpeed(datas.at(0));
	//}
	int data = m_ftUnitl.Read_Short_Add_Wait(1);
	if (data >= 0) {
		emit mouseSpeed((unsigned char)data);
	}
}

void FtTask::readMouseDirect()
{
	int data = m_ftUnitl.Read_Short_Add_Wait(0);
	if (data >= 0) {
		emit mouseDirect((unsigned char)data);
	}
}

void FtTask::ftReset()
{
	//m_ftUnitl.Read_Ext_Add(2, 0);
	//QThread::currentThread()->sleep(1);
	//m_ftUnitl.Read_Short_Add(0x00);
	m_ftUnitl.Write_Short_Add(0, 0);
	for (int i = 0; i < 30; i++) {
		m_ftUnitl.Write_Short_Add(1, 0);
	}
	update();
}

void FtTask::flashLed()
{
	static unsigned char index = 0;
	if (index > 7) {
		index = 0;
	}
	m_ftUnitl.Write_Short_Add(0x04, 0x01<<index);
	index++;
	//static unsigned char index = 0;
	//if (index == 0) {
	//	index = 1;
	//	m_ftUnitl.Write_Short_Add(0x04, 0x01);
	//}
	//else {
	//	index = 0;
	//	m_ftUnitl.Write_Short_Add(0x04, 0x00);
	//}
}

void FtTask::setUpdate(bool enable)
{
	m_mutex.lock();
	m_update = enable;
	m_mutex.unlock();
}
