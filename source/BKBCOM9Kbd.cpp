#include <Windows.h>
#include "BKBCOM9Kbd.h"
#include "SerialComm.h"

// ������� ������������ 
static unsigned char MapScancode2Arduino[256]=
{
	// ������ 0x00
	//0,0,0,0,0,0,0,0,  VK_BACK, TAB, 0,0,0, RETURN, 0,0 
	0,0,0,0,0,0,0,0,   0xB2, 0xB3, 0,0,0, 0xB0, 0,0,

	// ������ 0x10
	// 0,0,0,0,0,0,0,0,   0,0,0, VK_ESCAPE, 0,0,0,0,
	0,0,0,0,0,0,0,0,   0,0,0, 0xB1, 0,0,0,0,

	// ������ 0x20
	// VK_SPACE, VK_PRIOR, VK_NEXT, VK_END, VK_HOME, VK_LEFT, VK_UP, VK_RIGHT,   VK_DOWN, 0,0,0,0, VK_INSERT,VK_DELETE, 0,
	' ', 0xD3, 0xD6, 0xD5, 0xD2, 0xD8, 0xDA, 0xD7,   0xD9, 0,0,0,0, 0xD1,0xD4, 0,

	// ������ 0x30
	'0','1','2','3','4','5','6','7',   '8','9', 0,0,0,0,0,0,

	// ������ 0x40
	0, 'a','b','c','d','e','f','g',    'h','i','j','k','l','m','n','o',

	// ������ 0x50
	// 0,0,0,0,0,0,0,0,   x,y,z, VK_LWIN, VK_RWIN, 0,0,0,
	'p','q','r','s','t','u','v','w',    'x','y','z', 0x83, 0x87, 0,0,0,

	// ������ 0x60
	// VK_NUMPAD0..VK_NUMPAD9, VK_MULTIPLY, VK_ADD, (VK_SEPARATOR), VK_SUBTRACT, (VK_DECIMAL), VK_DIVIDE,
	'0','1','2','3','4','5','6','7',   '8','9', '*','+', 0, '-', 0, '/',
	
	// ������ 0x70
	// VK_F1..VK_F16, (Arduino - �� F12)
	0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,   0xCA,0xCB,0xCC,0xCD, 0,0,0,0,

	// ������ 0x80
	// VK_F17..VK_F24, 0..0,
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,

	// ������ 0x90
	// VK_NUMLOCK, VK_SCROLLLOCK, (OEM)
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,

	// ������ 0xA0
	// Ctrl,Shift,Browser,Media,
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,

	// ������ 0xB0
	// Media,Launch, OEM,
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,

	// ������ 0xC0
	// OEM, reserved
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,

	// ������ 0xD0
	// ����� ������
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,

	// ������ 0xE0
	// ����� ������
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,

	// ������ 0xF0
	// ����� ������
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0

};

// ������� ������������ 
static unsigned char MapUnicode2Arduino[256]=
{
	// ������ 0x00
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,

	// ������ 0x10
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,

	// ������ 0x20
	' ','!','"','#','$','%','&','\'','(',')','*','+',',','-','.','/',

	// ������ 0x30
	'0','1','2','3','4','5','6','7','8','9',':',';','<','=','>','?',

	// ������ 0x40
	'@','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O',
	
	// ������ 0x50
	'P','Q','R','S','T','U','V','W','X','Y','Z','[','\\',']','^','_',
	
	// ������ 0x60
	'`','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o',
	
	// ������ 0x70
	'p','q','r','s','t','u','v','w','x','y','z','{','|','}','~',0,
	
	// ������ 0x80
	// 0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,

	// ������ 0x90
	// 0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,

	// ������ 0xA0
	// 0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,

	// ������ 0xB0
	// 0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,

	// ������ 0xC0
	// 0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,

	// ������ 0xD0
	// 0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,

	// ������ 0xE0
	// 0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,

	// ������ 0xF0
	// 0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0

};

//=======================================================================================================
// ���������� 0, ���� ������� �������� ����� �� ����������
// ���������� 1, ���� �� �������
//=======================================================================================================
int BKBCOM9Kbd::ScanCodeButton(WORD scancode, bool _shift_pressed, bool _ctrl_pressed, bool _alt_pressed)
{
	unsigned char c;

	if(scancode>255) return 1;

	// �������� ��� ��� Arduino �� �������� ����� ��������
	c=MapScancode2Arduino[scancode&0xFF];
	if(!c) return 1;

	return ArduinoButton(c, _shift_pressed, _ctrl_pressed, _alt_pressed);
}

//=======================================================================================================
// ���������� 0, ���� ������� �������� ����� �� ����������
// ���������� 1, ���� �� �������
//=======================================================================================================
int BKBCOM9Kbd::UnicodeButton(WORD unicode, bool _shift_pressed, bool _ctrl_pressed, bool _alt_pressed)
{
	unsigned char c;

	if(unicode>255) return 1;

	// �������� ��� ��� Arduino �� �������� ����� ��������
	c=MapUnicode2Arduino[unicode&0xFF];
	if(!c) return 1;

	return ArduinoButton(c, _shift_pressed, _ctrl_pressed, _alt_pressed);
}

//=======================================================================================================
// ���������� 0, ���� ������� �������� ����� �� ����������
// ���������� 1, ���� �� �������
//=======================================================================================================
int BKBCOM9Kbd::ArduinoButton(unsigned char c, bool _shift_pressed, bool _ctrl_pressed, bool _alt_pressed)
{
	unsigned char CRC=0xFF;
	unsigned char byte2=0, byte3;

	byte3=c;
	if(!byte3) return 1;

	// ��������� ������
	if(BKBSerial::SendByte(CRC, 1)) return 1;

	// �����-�����-��������
	if(_shift_pressed) byte2+=1;
	if(_ctrl_pressed) byte2+=2;
	if(_alt_pressed) byte2+=4;
	
	CRC+=byte2; // ������������� CRC

	// ��� ������ ����
	if(BKBSerial::SendByte(byte2, 1)) return 1;

	// ��� ������� ��� Arduino
	if(BKBSerial::SendByte(byte3, 1)) return 1;
	CRC+=byte3;
	
	// ������ ������� CRC
	if(BKBSerial::SendByte(CRC, 1)) return 1;
	
	return 0; // ������� ������������� ������� ����������� ����������!
}