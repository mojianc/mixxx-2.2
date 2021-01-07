#include "BootAnimation.h"
#include "FtTask.h"
#include <QTimer>

#define BUTTON1 1
#define BUTTON2 2
#define BUTTON3 3
#define BUTTON4 4
#define BUTTON5 5
#define BUTTON6 6
#define BUTTON7 7
#define BUTTON8 8
#define BUTTON9 9
#define BUTTON10 10
#define BUTTON11 11
#define BUTTON12 12
#define BUTTON13 13
#define BUTTON14 14
#define BUTTON15 15
#define BUTTON16 16
#define BUTTON17 17
#define BUTTON18 18
#define BUTTON19 19
#define BUTTON20 20
#define BUTTON21 21
#define BUTTON22 22
#define BUTTON23 23
#define BUTTON24 24
#define BUTTON25 25
#define BUTTON26 26
#define BUTTON27 27
#define BUTTON28 28
#define BUTTON29 29
#define BUTTON30 30
#define BUTTON31 31
#define BUTTON32 32
#define BUTTON33 33
#define BUTTON34 34
#define BUTTON35 35
#define BUTTON36 36
#define BUTTON37 37
#define BUTTON38 38
#define BUTTON39 39
#define BUTTON40 40
#define BUTTON41 41
#define BUTTON42 42
#define BUTTON43 43
#define BUTTON44 44
#define BUTTON45 45
#define BUTTON46 46
#define BUTTON47 47
#define BUTTON48 48
#define BUTTON49 49
#define BUTTON50 50
#define BUTTON51 51
#define BUTTON52 52
#define BUTTON53 53
#define BUTTON54 54
#define BUTTON55 55
#define BUTTON56 56
#define BUTTON57 57
#define BUTTON58 58
#define BUTTON59 59
#define BUTTON60 60
#define BUTTON61 61
#define BUTTON62 62
#define BUTTON63 63
#define MAX_BUTTON_INDEX 63

#define LED_ON   1
#define LED_OFF  0
#define LED_ALLON  2
#define LED_ALLOFF 3

/**
*  led灯和ram之间的映射数组。[][0]--对应字节中的位，[][1]--对应字节的序号
*  如led2 对应ram中第0字节的bit1。
*  硬件上每行的映射为：
*  ROW 7 -0 -->byte0[7:0]
*  ROW 14-8 -->byte1[6:0]
*/
int led_map[226][2] = {
	{ 0,0 },
	{ 0,0 },  //led1
	{ 1,0 },  //led2
	{ 2,0 },  //led3
	{ 3,0 },  //led4
	{ 4,0 },  //led5
	{ 5,0 },  //led6
	{ 6,0 },  //led7
	{ 7,0 },  //led8
	{ 0,1 },  //led9
	{ 1,1 },  //led10
	{ 2,1 },  //led11
	{ 3,1 },  //led12
	{ 4,1 },  //led13
	{ 5,1 },  //led14
	{ 0,2 },  //led15
	{ 1,2 },  //led16
	{ 2,2 },  //led17
	{ 3,2 },  //led18
	{ 4,2 },  //led19
	{ 5,2 },  //led20
	{ 6,2 },  //led21
	{ 7,2 },  //led22
	{ 0,3 },  //led23
	{ 1,3 },  //led24
	{ 2,3 },  //led25
	{ 3,3 },  //led26
	{ 4,3 },  //led27
	{ 5,3 },  //led28
	{ 0,4 },  //led29
	{ 1,4 },  //led30
	{ 2,4 },  //led31
	{ 3,4 },  //led32
	{ 4,4 },  //led33
	{ 5,4 },  //led34
	{ 6,4 },  //led35
	{ 7,4 },  //led36
	{ 0,5 },  //led37
	{ 1,5 },  //led38
	{ 2,5 },  //led39
	{ 3,5 },  //led40
	{ 4,5 },  //led41
	{ 5,5 },  //led42
	{ 0,6 },  //led43
	{ 1,6 },  //led44
	{ 2,6 },  //led45
	{ 3,6 },  //led46
	{ 4,6 },  //led47
	{ 5,6 },  //led48
	{ 6,6 },  //led49
	{ 7,6 },  //led50
	{ 0,7 },  //led51
	{ 1,7 },  //led52
	{ 2,7 },  //led53
	{ 3,7 },  //led54
	{ 4,7 },  //led55
	{ 5,7 },  //led56
	{ 0,8 },  //led57
	{ 1,8 },  //led58
	{ 2,8 },  //led59
	{ 3,8 },  //led60
	{ 4,8 },  //led61
	{ 5,8 },  //led62
	{ 6,8 },  //led63
	{ 7,8 },  //led64
	{ 0,9 },  //led65
	{ 1,9 },  //led66
	{ 2,9 },  //led67
	{ 3,9 },  //led68
	{ 4,9 },  //led69
	{ 5,9 },  //led70
	{ 0,10 },  //led71
	{ 1,10 },  //led72
	{ 2,10 },  //led73
	{ 3,10 },  //led74
	{ 4,10 },  //led75
	{ 5,10 },  //led76
	{ 6,10 },  //led77
	{ 7,10 },  //led78
	{ 0,11 },  //led79
	{ 1,11 },  //led80
	{ 2,11 },  //led81
	{ 3,11 },  //led82
	{ 4,11 },  //led83
	{ 5,11 },  //led84
	{ 0,12 },  //led85
	{ 1,12 },  //led86
	{ 2,12 },  //led87
	{ 3,12 },  //led88
	{ 4,12 },  //led89
	{ 5,12 },  //led90
	{ 6,12 },  //led91
	{ 7,12 },  //led92
	{ 0,13 },  //led93
	{ 1,13 },  //led94
	{ 2,13 },  //led95
	{ 3,13 },  //led96
	{ 4,13 },  //led97
	{ 5,13 },  //led98
	{ 0,14 },  //led99
	{ 1,14 },  //led100
	{ 2,14 },  //led101
	{ 3,14 },  //led102
	{ 4,14 },  //led103
	{ 5,14 },  //led104
	{ 6,14 },  //led105
	{ 7,14 },  //led106
	{ 0,15 },  //led107
	{ 1,15 },  //led108
	{ 2,15 },  //led109
	{ 3,15 },  //led110
	{ 4,15 },  //led111
	{ 5,15 },  //led112
	{ 0,16 },  //led113
	{ 1,16 },  //led114
	{ 2,16 },  //led115
	{ 3,16 },  //led116
	{ 4,16 },  //led117
	{ 5,16 },  //led118
	{ 6,16 },  //led119
	{ 7,16 },  //led120
	{ 0,17 },  //led121
	{ 1,17 },  //led122
	{ 2,17 },  //led123
	{ 3,17 },  //led124
	{ 4,17 },  //led125
	{ 5,17 },  //led126
	{ 0,18 },  //led127
	{ 1,18 },  //led128
	{ 2,18 },  //led129
	{ 3,18 },  //led130
	{ 4,18 },  //led131
	{ 5,18 },  //led132
	{ 6,18 },  //led133
	{ 7,18 },  //led134
	{ 0,19 },  //led135
	{ 1,19 },  //led136
	{ 2,19 },  //led137
	{ 3,19 },  //led138
	{ 4,19 },  //led139
	{ 5,19 },  //led140
	{ 0,20 },  //led141
	{ 1,20 },  //led142
	{ 2,20 },  //led143
	{ 3,20 },  //led144
	{ 4,20 },  //led145
	{ 5,20 },  //led146
	{ 6,20 },  //led147
	{ 7,20 },  //led148
	{ 0,21 },  //led149
	{ 1,21 },  //led150
	{ 2,21 },  //led151
	{ 3,21 },  //led152
	{ 4,21 },  //led153
	{ 5,21 },  //led154
	{ 0,22 },  //led155
	{ 1,22 },  //led156
	{ 2,22 },  //led157
	{ 3,22 },  //led158
	{ 4,22 },  //led159
	{ 5,22 },  //led160
	{ 6,22 },  //led161
	{ 7,22 },  //led162
	{ 0,23 },  //led163
	{ 1,23 },  //led164
	{ 2,23 },  //led165
	{ 3,23 },  //led166
	{ 4,23 },  //led167
	{ 5,23 },  //led168
	{ 0,24 },  //led169
	{ 1,24 },  //led170
	{ 2,24 },  //led171
	{ 3,24 },  //led172
	{ 4,24 },  //led173
	{ 5,24 },  //led174
	{ 6,24 },  //led175
	{ 7,24 },  //led176
	{ 0,25 },  //led177
	{ 1,25 },  //led178
	{ 2,25 },  //led179
	{ 3,25 },  //led180
	{ 4,25 },  //led181
	{ 5,25 },  //led182
	{ 0,26 },  //led183
	{ 1,26 },  //led184
	{ 2,26 },  //led185
	{ 3,26 },  //led186
	{ 4,26 },  //led187
	{ 5,26 },  //led188
	{ 6,26 },  //led189
	{ 7,26 },  //led190
	{ 0,27 },  //led191
	{ 1,27 },  //led192
	{ 2,27 },  //led193
	{ 3,27 },  //led194
	{ 4,27 },  //led195
	{ 5,27 },  //led196
	{ 0,28 },  //led197
	{ 1,28 },  //led198
	{ 2,28 },  //led199
	{ 3,28 },  //led200
	{ 4,28 },  //led201
	{ 5,28 },  //led202
	{ 7,28 },  //led203
	{ 6,28 },  //led204
	{ 6,1 },  //led205
	{ 6,3 },  //led206
	{ 6,5 },  //led207
	{ 6,7 },  //led208
	{ 6,9 },  //led209
	{ 6,11 },  //led210
	{ 6,13 },  //led211
	{ 6,15 },  //led212
	{ 6,17 },  //led213
	{ 6,19 },  //led214
	{ 6,21 },  //led215
	{ 6,23 },  //led216
	{ 6,25 },  //led217
	{ 6,27 },  //led218
	{ 6,29 },  //led219
	{ 0,29 },  //led220
	{ 1,29 },  //led221
	{ 2,29 },  //led222
	{ 3,29 },  //led223
	{ 4,29 },  //led224
	{ 5,29 }   //led225

};

/**
 * 按键与led灯的映射数组。第一个元素是按键包含的led灯的个数。后面是顺时针组成按键的led灯位号
 */
int btn_map[MAX_BUTTON_INDEX+1][24] = {
	{ 0 },  //0
	{ 5,1,2,211,3,212 },  //1
	{ 1,4 },  //2
	{ 3,5,205,6 },  //3
	{ 3,7,206,8 },  //4
	{ 3,9,207,10 },  //5
	{ 2,72,74 },  //6
	{ 2,75,77 },  //7
	{ 2,78,79 },  //8
	{ 3,137,208,138 },  //9
	{ 3,139,209,140 },  //10
	{ 3,141,210,142 },  //11
	{ 1,143 },  //12
	{ 5,213,134,214,135,136 },  //13
	{ 7,12,11,13,14,15,16,17 },  //14
	{ 7,18,19,20,21,22,23,24 },  //15
	{ 23,43,42,41,40,39,38,37,35,36,34,33,32,31,30,29,28,223,76,73,47,46,45,44 },  //16
	{ 2,25,26 },  //17
	{ 1,27 },  //18
	{ 2,48,49 },  //19
	{ 2,70,71 },  //20
	{ 2,50,51 },  //21
	{ 2,54,55 },  //22
	{ 2,56,57 },  //23
	{ 2,58,59 },  //24
	{ 2,60,61 },  //25
	{ 2,62,63 },  //26
	{ 2,64,65 },  //27
	{ 2,66,67},  //28
	{ 2,68,69 },  //29
	{ 2,52,53 },  //30
	{ 11,80,81,82,83,84,85,86,87,88,90,89 },  //31
	{ 2,91,93 },  //32
	{ 11,94,95,96,97,98,99,100,101,102,104,103 },  //33
	{ 3,105,215,106 },  //34
	{ 3,113,216,114 },  //35
	{ 3,104,217,108 },  //36
	{ 3,115,218,116 },  //37
	{ 3,109,219,110 },  //38
	{ 3,117,220,118 },  //39
	{ 3,111,221,112 },  //40
	{ 3,119,222,120 },  //41
	{ 1,121 },  //42
	{ 1,122 },  //43
	{ 11,123,124,125,126,127,128,129,130,131,133,132 },  //44
	{ 7,145,144,146,147,148,149,150 },  //45
	{ 7,151,152,153,154,155,156,157 },  //46
	{ 23,177,176,175,174,173,172,171,170,169,168,167,166,165,164,163,162,161,160,159,158,224,92,225 },  //47
	{ 2,178,179 },  //48
	{ 1,180 },  //49
	{ 2,181,182 },  //50
	{ 2,183,184 },  //51
	{ 2,189,190 },  //52
	{ 2,191,192 },  //53
	{ 2,193,194 },  //54
	{ 2,195,196 },  //55
	{ 2,185,186 },  //56
	{ 2,197,198 },  //57
	{ 2,199,200 },  //58
	{ 2,201,202 },  //59
	{ 2,203,204 },  //60
	{ 2,187,188 },  //61
	{ 16,12,17,151,157,24,18,25,26,27,178,179,180,78,79,80,81 },  //62
	{ 19,91,93,94,95,128,121,122,1,2,3,211,212,213,214,134,135,136,145,150 },  //63
};

BootAnimation::BootAnimation(QObject *parent)
	: QObject(parent), m_ftTask(nullptr)
{
	m_pTimer = new QTimer(this);
	m_pTimer->setSingleShot(true);
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(timeTask()));
}

BootAnimation::~BootAnimation()
{
    
}

void BootAnimation::start()
{
	m_actions.clear();
	generateActions();
	m_ftTask->clearBuff();
	timeTask();
}

void BootAnimation::timeTask()
{
	FlashAction action={0} ;	
	do{
		if (m_actions.size() == 0) {
			break;
		}
		action = m_actions.at(0);
		setButtonState(action.objectIndex, action);
		m_actions.removeAt(0);
	} while (action.holdTime == 0);
	m_ftTask->update();
	if (action.holdTime != 0) {
		m_pTimer->start(action.holdTime);
	}	
}

void BootAnimation::flashTask()
{
	FlashAction iaction = { 0 };
	iaction.holdTime = 0;
	iaction.ledIndex = 0;
	iaction.isAll = true;
	iaction.objectIndex = BUTTON21;
	iaction.action = LED_OFF;
	setButtonState(iaction.objectIndex,iaction);
	if (m_flashState == 1) {
		FlashAction iaction = { 0 };
		iaction.holdTime = 0;
		iaction.ledIndex = 0;
		iaction.isAll = true;
		iaction.objectIndex = BUTTON21;
		iaction.action = LED_OFF;
		setButtonState(iaction.objectIndex, iaction);
		iaction.objectIndex = BUTTON56;
		setButtonState(iaction.objectIndex, iaction);
		m_flashState = 0;
	}
	else {
		FlashAction iaction = { 0 };
		iaction.holdTime = 0;
		iaction.ledIndex = 0;
		iaction.isAll = true;
		iaction.objectIndex = BUTTON21;
		iaction.action = LED_ON;
		setButtonState(iaction.objectIndex, iaction);
		iaction.objectIndex = BUTTON56;
		setButtonState(iaction.objectIndex, iaction);
		m_flashState = 1;
	}
	m_ftTask->update();
	if (m_actions.size() == 0) {
		m_ftTask->clearBuff();
		flashOver();
		return;
	}
	QTimer::singleShot(500, this, SLOT(flashTask()));
}

void BootAnimation::stop()
{
	m_actions.clear();
}

void BootAnimation::addAction(int holdTime, int ledIndex, bool isAll, int objectIndex, int action)
{
	FlashAction iaction = { 0};
	iaction.holdTime = holdTime;
	iaction.ledIndex = ledIndex;
	iaction.isAll = isAll;
	iaction.objectIndex = objectIndex;
	iaction.action = action;
	m_actions.append(iaction);
}

void BootAnimation::generateActions()
{
	//testFlash();
	turnOnAlwaysBright();
	flashDropBtn();
	flashBtn31Btn33();
	flashBtn21Btn56();
	addAction(0, 0, true, 0, LED_ON);
}

void BootAnimation::setButtonState(int button, FlashAction action)
{
	if (button > MAX_BUTTON_INDEX) {
		return;
	}
	if (action.action == LED_ALLOFF) {
		m_ftTask->clearBuff();
		return;
	}
	else if (action.action == LED_ALLON) {
		for (int i = 0; i < 30; i++) {
			m_ftTask->setBuff(i, 0xff);
		}
		return;
	}
	if (button < 1) {
		return;
	}
	if (action.isAll) {
		for (int i = 1; i <= btn_map[button][0]; i++) {
			setLedState(btn_map[button][i], action.action);
		}
	}
	else {
		if ((action.ledIndex <= btn_map[button][0])&&(action.ledIndex > 0)) {
			setLedState(btn_map[button][action.ledIndex], action.action);
		}
	}
}

void BootAnimation::getBitAndIndexOfLED(int number, int* bit, int* index)
{
	int row_bit = 0;
	if ((number >= 1) && (number <= 225)){
		*index = led_map[number][1];
		*bit = led_map[number][0];
	}
}

void BootAnimation::setLedState(int number, int state)
{
	int bit = 0, index = 0;
	unsigned char buff = 0;
	getBitAndIndexOfLED(number, &bit, &index);
	buff = m_ftTask->getBuff(index);
	if (state == LED_ON) {
		buff |= 0x01 << bit;
	}
	else {
		buff = buff & (~(0x01 << bit));
	}	
	m_ftTask->setBuff(index, buff);
}

void BootAnimation::testFlash()
{
	addAction(3000, 0, true, BUTTON18, LED_ON);
	addAction(0, 0, true, BUTTON18, LED_OFF);
	addAction(3000, 0, true, BUTTON49, LED_ON);
	addAction(0, 0, true, BUTTON49, LED_OFF);
	for (int i = 1; i <= btn_map[18][0]; i++) {
		addAction(200, i, false, BUTTON18, LED_ON);
	}
	for (int i = 1; i <= btn_map[49][0]; i++) {
		addAction(200, i, false, BUTTON49, LED_ON);
	}
	for (int i = 1; i <= btn_map[18][0]; i++) {
		addAction(200, i, false, BUTTON18, LED_OFF);
	}
	for (int i = 1; i <= btn_map[49][0]; i++) {
		addAction(200, i, false, BUTTON49, LED_OFF);
	}
}

void BootAnimation::turnOnAlwaysBright()
{
	addAction(0, 0, true, BUTTON1, LED_ON);
	addAction(0, 0, true, BUTTON13, LED_ON);
	addAction(0, 7, false, BUTTON14, LED_ON);
	addAction(0, 7, false, BUTTON15, LED_ON);
	addAction(0, 7, false, BUTTON45, LED_ON);
	addAction(0, 7, false, BUTTON46, LED_ON);
}

void BootAnimation::flashDropBtn()
{
	addAction(2000, 0, true, 0, LED_ON);
	for (int i = 1; i <= btn_map[16][0]; i++) {
		addAction(0, i, false, BUTTON16, LED_ON);
		addAction(100, i, false, BUTTON47, LED_ON);
	}
	addAction(2000, 0, true, 0, LED_ON);
}

void BootAnimation::flashBtn31Btn33()
{
	addAction(200, 6, false, BUTTON31, LED_ON);
	for (int i = 1; i < 6; i++) {//31
		if (i < 7) {
			addAction(0, 6 - i, false, BUTTON31, LED_ON);
			addAction(200, 6 + i, false, BUTTON31, LED_ON);
		}
	}
	addAction(200, 6, false, BUTTON31, LED_OFF);
	for (int i = 1; i < 6; i++) {//31
		if (i < 7) {
			addAction(0, 6 - i, false, BUTTON31, LED_OFF);
			addAction(200, 6 + i, false, BUTTON31, LED_OFF);
		}
	}
	addAction(1000, 0, true, 0, LED_ON);
	addAction(1000, 0, true, BUTTON31, LED_OFF);
	
	addAction(0, 1, false, BUTTON31, LED_ON);
	addAction(0, 1, false, BUTTON33, LED_ON);
	addAction(200, 6, false, BUTTON44, LED_ON);
	for (int i = 2; i <= btn_map[31][0]; i++) {
		if (i < 7) {
			addAction(0, 7-i, false, BUTTON44, LED_ON);
			addAction(0, 5+i, false, BUTTON44, LED_ON);
		}
		addAction(0, i, false, BUTTON31, LED_ON);
		addAction(200, i, false, BUTTON33, LED_ON);
	}

	addAction(0, 1, false, BUTTON31, LED_OFF);
	addAction(0, 1, false, BUTTON33, LED_OFF);
	addAction(200, 6, false, BUTTON44, LED_OFF);
	for (int i = 2; i <= btn_map[31][0]; i++) {
		if (i < 7) {
			addAction(0, 7 - i, false, BUTTON44, LED_OFF);
			addAction(0, 5 + i, false, BUTTON44, LED_OFF);
		}
		addAction(0, i, false, BUTTON31, LED_OFF);
		addAction(200, i, false, BUTTON33, LED_OFF);
	}
	addAction(2000, 0, true, 0, LED_ON);
}

void BootAnimation::doAction(int ledIndex,bool isAll,int objectIndex,int action){
	FlashAction iaction = { 0};
	iaction.holdTime = 0;
	iaction.ledIndex = ledIndex;
	iaction.isAll = isAll;
	iaction.objectIndex = objectIndex;
	iaction.action = action;
	setButtonState(iaction.objectIndex, iaction);
}

void BootAnimation::flashOver()
{
	//doAction(1, false, BUTTON14, LED_ON);//D12
	doAction(0, true, BUTTON62, LED_ON);
	doAction(0, true, BUTTON63, LED_ON);
	m_ftTask->update();
}

void BootAnimation::flashBtn21Btn56()
{
	m_flashState = 0;
	QTimer::singleShot(500, this, SLOT(flashTask()));
}
