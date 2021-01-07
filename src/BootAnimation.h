#pragma once

#include <QObject>
#include <QList>
class FtTask;
class QTimer;

class FlashAction
{
public:
	int holdTime; // in ms
	int ledIndex; //start with 1
	bool isAll;
	int objectIndex;
	int action;
};

class BootAnimation : public QObject
{
	Q_OBJECT

public:
	BootAnimation(QObject *parent);
	~BootAnimation();

	void setFtTask(FtTask* task) { m_ftTask = task; };

	void start();
	void stop();

public slots:
	void timeTask();
	void flashTask();

private:
	void addAction(int holdTime,int ledIndex,bool isAll,int objectIndex,int action);
	void generateActions();
	void setButtonState(int button,FlashAction action);
	void getBitAndIndexOfLED(int number, int* bit, int* index);
	void setLedState(int number,int state);

	void testFlash();
	void turnOnAlwaysBright();
	void flashDropBtn();
	void flashBtn31Btn33();
	void flashOver();
	void flashBtn21Btn56();
	
	void doAction(int ledIndex,bool isAll,int objectIndex,int action);
private:

	FtTask* m_ftTask;
	QList<FlashAction> m_actions;
	QTimer *m_pTimer;
	unsigned char m_flashState;
};
