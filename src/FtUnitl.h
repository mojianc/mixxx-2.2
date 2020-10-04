#pragma once

#include <QObject>
#include "ftd2xx.h"

class FtUnitl : public QObject
{
	Q_OBJECT

public:
	FtUnitl(QObject *parent=NULL);
	~FtUnitl();

	bool isPortAOpen() { return PortAIsOpen; }
	QString checkDevice();
	bool isDeviceOnline();
	QByteArray readReceivedDatas();
	void readAll();

	QStringList listDeivce();

	int GetFTDeviceDescription(int device_index);

	QString GetDeviceString();

	bool Check_For_Dev_A(QString name);
	FT_STATUS Open_USB_Device_By_Device_Description(QString Device_Description);
	void SetDeviceString(QString Device_Description);
	FT_STATUS Get_USB_Device_QueueStatus();
	void FT_Error_Report(QString ErrStr, int PortStatus);
	FT_STATUS Close_USB_Device();
	void AddToBuffer(unsigned char d);
	void SendBytes(int number);
	int Write_USB_Device_Buffer(int count);
	void Write_Short_Add(unsigned char addr, unsigned char data);
	void Write_Ext_Add(unsigned char addrH, unsigned char addrL, unsigned char data);
	void Read_Short_Add(unsigned char addr);
	void Read_Ext_Add(unsigned char addrH, unsigned char addrL);
	int Read_Short_Add_Wait(unsigned char addr, int timeout = 100);
	int GetData();
	int Read_USB_Device_Buffer(int count);
	bool InitDev();
	bool Sync_To_MPSSE();
private:

	char FT_Device_String_Buffer[50];
	int  FT_Device_Count;
	QString FT_Device_String;
	QStringList FT_Device_Names;

	FT_HANDLE FT_Handle;
	DWORD FT_Q_Bytes;
	unsigned char *FT_In_Buffer;
	unsigned char *FT_Out_Buffer;
	int OutIndex;
	int InIndex;
	bool PortAIsOpen;
};
