#include "FtUnitl.h"
#include "ftd2xx.h"
#include <QThread>

#define FT_In_Buffer_Size  0x8000
#define FT_Out_Buffer_Size 0x8000

FtUnitl::FtUnitl(QObject *parent)
	: QObject(parent), OutIndex(0), PortAIsOpen(false), InIndex(0)
{
	FT_Device_String_Buffer[0] = 0;
	FT_In_Buffer = new unsigned char[FT_In_Buffer_Size];
	FT_Out_Buffer = new unsigned char[FT_Out_Buffer_Size];
}

FtUnitl::~FtUnitl()
{
	delete[] FT_Out_Buffer;
	if (PortAIsOpen) {
		Close_USB_Device();
	}
}

QString FtUnitl::checkDevice()
{
	QStringList devices = listDeivce();
	if (devices.size() > 0) {
		if (Open_USB_Device_By_Device_Description(devices.at(0)) == FT_OK) {
			qDebug("open device success!");
			if (Get_USB_Device_QueueStatus() == FT_OK) {
				PortAIsOpen = true;
				return devices.at(0);
			}
		}
	}
	return NULL;
}

bool FtUnitl::isDeviceOnline()
{
	if (PortAIsOpen) {
		FT_STATUS rst = FT_GetQueueStatus(FT_Handle, &FT_Q_Bytes);
		if (rst != FT_OK) {
			PortAIsOpen = false;
		}
	}
	return PortAIsOpen;
}

QByteArray FtUnitl::readReceivedDatas()
{
	QByteArray datas;
	for (int i = 0; i < InIndex; i++) {
		datas.append(FT_In_Buffer[i]);
	}
	return datas;
}

void FtUnitl::readAll()
{
	FT_STATUS rst = Get_USB_Device_QueueStatus();
	if (FT_Q_Bytes > 0) {
		InIndex = Read_USB_Device_Buffer(FT_Q_Bytes);
	}
}

QStringList FtUnitl::listDeivce() {
	QStringList devices;
	int number = 0;
	FT_Device_Names.clear();
	FT_ListDevices(&number,0, FT_LIST_NUMBER_ONLY);
	if (number > 0) {
		for (int i = 0; i < number; i++) {
			GetFTDeviceDescription(i);
			qDebug("device %s",qPrintable(FT_Device_String));
			FT_Device_Names.append(FT_Device_String);
			if (Check_For_Dev_A(FT_Device_String)) {
				devices.append(FT_Device_String);
				qDebug("find A %s ", qPrintable(FT_Device_String));
				
			}
		}
	}
	qDebug("device number %d", number);
	return devices;
}


int FtUnitl::GetFTDeviceDescription(int device_index) {
	FT_STATUS rst = FT_ListDevices((PVOID)device_index, FT_Device_String_Buffer,(FT_OPEN_BY_DESCRIPTION | FT_LIST_BY_INDEX));
	if (rst == FT_OK) {
		FT_Device_String = GetDeviceString();		
	}
	return rst;
}

QString FtUnitl::GetDeviceString()
{
	FT_Device_String_Buffer[49] = 0;
	QString str(FT_Device_String_Buffer);
	return str;
}

bool FtUnitl::Check_For_Dev_A(QString name) {
	if (!name.isEmpty()) {
		if (name.length() > 2) {
			if (name.right(2) == " A") {
				return true;
			}
		}
	}
	return false;
}

FT_STATUS FtUnitl::Open_USB_Device_By_Device_Description(QString Device_Description) {
	SetDeviceString(Device_Description);
	FT_STATUS rst = FT_OpenEx(FT_Device_String_Buffer, FT_OPEN_BY_DESCRIPTION, &FT_Handle);
	return rst;
}

void FtUnitl::SetDeviceString(QString Device_Description) {
	QByteArray ba= Device_Description.toLatin1();
	char* p = ba.data();
	int length = Device_Description.length();
	if (length > 50)length = 50;
	memcpy(FT_Device_String_Buffer, p, length);
}

FT_STATUS FtUnitl::Get_USB_Device_QueueStatus() {
	FT_STATUS rst = FT_GetQueueStatus(FT_Handle, &FT_Q_Bytes);
	if (rst != FT_OK) {
		FT_Error_Report("FT_GetQueueStatus", rst);
	}
	return rst;
}

void FtUnitl::FT_Error_Report(QString ErrStr,int PortStatus) {
	qDebug("error %s , status %d", qPrintable(ErrStr), PortStatus);
	isDeviceOnline();
}

FT_STATUS FtUnitl::Close_USB_Device() {
	FT_STATUS rst = FT_Close(FT_Handle);
	if (rst != FT_OK) {
		FT_Error_Report("FT_Close", rst);
	}
	PortAIsOpen = false;
	return rst;
}

void FtUnitl::AddToBuffer(unsigned char d) {
	FT_Out_Buffer[OutIndex++] = d;
}

void FtUnitl::SendBytes(int number) {
	int count = Write_USB_Device_Buffer(number);
	OutIndex -= count;
}

int FtUnitl::Write_USB_Device_Buffer(int count) {
	DWORD Write_Result;
	FT_STATUS FT_IO_Status = FT_Write(FT_Handle, FT_Out_Buffer, count, &Write_Result);
	if (FT_IO_Status != FT_OK) {
		FT_Error_Report("FT_Write", FT_IO_Status);
	}
	return Write_Result;
}

void FtUnitl::Write_Short_Add(unsigned char addr, unsigned char data) {
	AddToBuffer(0x92); // write short
	AddToBuffer(addr); // Address low
	AddToBuffer(data); // Data
	SendBytes(OutIndex); // send off the command
}

void FtUnitl::Write_Ext_Add(unsigned char addrH, unsigned char addrL, unsigned char data)
{
	AddToBuffer(0x93); // write short
	AddToBuffer(addrH); // Address high
	AddToBuffer(addrL); // Address low
	AddToBuffer(data); // Data
	SendBytes(OutIndex); // send off the command
}

void FtUnitl::Read_Short_Add(unsigned char addr) {
	AddToBuffer(0x90); // read short
	AddToBuffer(addr); // Address low
	AddToBuffer(0x87); // Send Immediate
	SendBytes(OutIndex); // send off the command
}

void FtUnitl::Read_Ext_Add(unsigned char addrH, unsigned char addrL)
{
	AddToBuffer(0x91); // read short
	AddToBuffer(addrH); // Address high
	AddToBuffer(addrL); // Address low
	AddToBuffer(0x87); // Send Immediate
	SendBytes(OutIndex); // send off the command
}

int FtUnitl::Read_Short_Add_Wait(unsigned char addr, int timeout)
{
	readAll();
	AddToBuffer(0x90); // read short
	AddToBuffer(addr); // Address low
	AddToBuffer(0x87); // Send Immediate
	SendBytes(OutIndex); // send off the command
	int cnt = 0;
	InIndex = 0;
	
	while (cnt < timeout) {
		FT_STATUS rst = Get_USB_Device_QueueStatus();
		if (rst != FT_OK) {
			break;
		}
		if (FT_Q_Bytes > 0) {
			DWORD Read_Result = 0;
			FT_STATUS rst = FT_Read(FT_Handle, FT_In_Buffer, 1, &Read_Result);
			if (rst != FT_OK) {
				FT_Error_Report("FT_Read", rst);
			}
			else {
				return FT_In_Buffer[0];
			}
			break;
		}
		else {
			QThread::currentThread()->msleep(1);
			cnt++;
		}
	}
	return -1;
}

int FtUnitl::GetData() {
	int cnt = 0;
	InIndex = 0;
	while(cnt < 100){
		FT_STATUS rst = Get_USB_Device_QueueStatus();
		if (FT_Q_Bytes > 0) {
			InIndex = Read_USB_Device_Buffer(FT_Q_Bytes);
			break;
		}
		else {
			QThread::currentThread()->msleep(10);
			cnt++;
		}
	}
	return InIndex;
}

int FtUnitl::Read_USB_Device_Buffer(int count) {
	DWORD Read_Result = 0;
	FT_STATUS rst = FT_Read(FT_Handle,FT_In_Buffer,count,&Read_Result);
	if (rst != FT_OK) {
		FT_Error_Report("FT_Read", rst);
	}
	return Read_Result;
}

bool FtUnitl::InitDev()
{
	bool passed = false;
	FT_STATUS rst;
	rst = FT_SetLatencyTimer(FT_Handle, 16);
	rst = FT_SetBitMode(FT_Handle, 0, 0);// reset controller
	rst = FT_SetBitMode(FT_Handle, 0, 8);// enable Host Bus Emulation
	return Sync_To_MPSSE();
}

bool FtUnitl::Sync_To_MPSSE()
{
	FT_STATUS rst;
	bool Done = false;
	int i = 0, j = 0;
	rst = Get_USB_Device_QueueStatus();
	if (rst != FT_OK) {
		return false;
	}
	if (FT_Q_Bytes > 0) {
		i = Read_USB_Device_Buffer(FT_Q_Bytes);
	}
	do{
		OutIndex = 0;
		AddToBuffer(0xaa);
		SendBytes(OutIndex);
		rst = Get_USB_Device_QueueStatus();
		if (rst != FT_OK) {
			return false;
		}
		if (FT_Q_Bytes == 0) {
			QThread::currentThread()->msleep(1);
		}
	} while (FT_Q_Bytes == 0);
	i = Read_USB_Device_Buffer(FT_Q_Bytes);
	j = 0;
	do {
		if (FT_In_Buffer[j] == 0xfa) {
			if (j < (i - 2)) {
				if (FT_In_Buffer[j + 1] = 0xAA) {
					Done = true;
					break;
				}
			}

		}
		j++;
	} while (j<i);
	OutIndex = 0;
	AddToBuffer(0xab);
	SendBytes(OutIndex);
	do {
		rst = Get_USB_Device_QueueStatus();
		if (rst != FT_OK) {
			return false;
		}
	} while (FT_Q_Bytes == 0);

	i = Read_USB_Device_Buffer(FT_Q_Bytes);
	j = 0;
	do {
		if (FT_In_Buffer[j] == 0xfa) {
			if (j < (i - 2)) {
				if (FT_In_Buffer[j + 1] = 0xAB) {
					Done = true;
					break;
				}
			}

		}
		j++;
	} while (j < i);
	return Done;
}
