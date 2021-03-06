﻿#include <Windows.h>
#include "BKBCOM9Kbd.h"
#include "SerialComm.h"

// Таблица соответствия 
static unsigned char MapScancode2Arduino[256]=
{
	// Строка 0x00
	//0,0,0,0,0,0,0,0,  VK_BACK, TAB, 0,0,0, RETURN, 0,0 
	0,0,0,0,0,0,0,0,   0xB2, 0xB3, 0,0,0, 0xB0, 0,0,

	// Строка 0x10
	// 0,0,0,0,0,0,0,0,   0,0,0, VK_ESCAPE, 0,0,0,0,
	0,0,0,0,0,0,0,0,   0,0,0, 0xB1, 0,0,0,0,

	// Строка 0x20
	// VK_SPACE, VK_PRIOR, VK_NEXT, VK_END, VK_HOME, VK_LEFT, VK_UP, VK_RIGHT,   VK_DOWN, 0,0,0,0, VK_INSERT,VK_DELETE, 0,
	' ', 0xD3, 0xD6, 0xD5, 0xD2, 0xD8, 0xDA, 0xD7,   0xD9, 0,0,0,0, 0xD1,0xD4, 0,

	// Строка 0x30
	'0','1','2','3','4','5','6','7',   '8','9', 0,0,0,0,0,0,

	// Строка 0x40
	0, 'a','b','c','d','e','f','g',    'h','i','j','k','l','m','n','o',

	// Строка 0x50
	// 0,0,0,0,0,0,0,0,   x,y,z, VK_LWIN, VK_RWIN, 0,0,0,
	'p','q','r','s','t','u','v','w',    'x','y','z', 0x83, 0x87, 0,0,0,

	// Строка 0x60
	// VK_NUMPAD0..VK_NUMPAD9, VK_MULTIPLY, VK_ADD, (VK_SEPARATOR), VK_SUBTRACT, (VK_DECIMAL), VK_DIVIDE,
	'0','1','2','3','4','5','6','7',   '8','9', '*','+', 0, '-', 0, '/',
	
	// Строка 0x70
	// VK_F1..VK_F16, (Arduino - до F12)
	0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,   0xCA,0xCB,0xCC,0xCD, 0,0,0,0,

	// Строка 0x80
	// VK_F17..VK_F24, 0..0,
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,

	// Строка 0x90
	// VK_NUMLOCK, VK_SCROLLLOCK, (OEM)
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,

	// Строка 0xA0
	// Ctrl,Shift,Browser,Media,
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,

	// Строка 0xB0
	// Media,Launch, OEM,
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,

	// Строка 0xC0
	// OEM, reserved
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,

	// Строка 0xD0
	// Фигня всякая
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,

	// Строка 0xE0
	// Фигня всякая
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,

	// Строка 0xF0
	// Фигня всякая
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0

};

// Таблица соответствия 
static unsigned char MapUnicode2Arduino[256]=
{
	// Строка 0x00
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,

	// Строка 0x10
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,

	// Строка 0x20
	' ','!','"','#','$','%','&','\'','(',')','*','+',',','-','.','/',

	// Строка 0x30
	'0','1','2','3','4','5','6','7','8','9',':',';','<','=','>','?',

	// Строка 0x40
	'@','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O',
	
	// Строка 0x50
	'P','Q','R','S','T','U','V','W','X','Y','Z','[','\\',']','^','_',
	
	// Строка 0x60
	'`','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o',
	
	// Строка 0x70
	'p','q','r','s','t','u','v','w','x','y','z','{','|','}','~',0,
	
	// Строка 0x80
	// 0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,

	// Строка 0x90
	// 0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,

	// Строка 0xA0
	// 0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,

	// Строка 0xB0
	// 0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,

	// Строка 0xC0
	// 0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,

	// Строка 0xD0
	// 0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,

	// Строка 0xE0
	// 0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,

	// Строка 0xF0
	// 0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0

};

//=======================================================================================================
// Возвращает 0, если удалось передать пакет на клавиатуру
// Возвращает 1, если не удалось
//=======================================================================================================
int BKBCOM9Kbd::ScanCodeButton(WORD scancode, bool _shift_pressed, bool _ctrl_pressed, bool _alt_pressed)
{
	unsigned char c;

	if(scancode>255) return 1;

	// получаем код для Arduino из младшего байта сканкода
	c=MapScancode2Arduino[scancode&0xFF];
	if(!c) return 1;

	return ArduinoButton(c, _shift_pressed, _ctrl_pressed, _alt_pressed);
}

//=======================================================================================================
// Возвращает 0, если удалось передать пакет на клавиатуру
// Возвращает 1, если не удалось
//=======================================================================================================
int BKBCOM9Kbd::UnicodeButton(WORD unicode, bool _shift_pressed, bool _ctrl_pressed, bool _alt_pressed)
{
	unsigned char c;

	if(unicode>255) return 1;

	// получаем код для Arduino из младшего байта сканкода
	c=MapUnicode2Arduino[unicode&0xFF];
	if(!c) return 1;

	return ArduinoButton(c, _shift_pressed, _ctrl_pressed, _alt_pressed);
}

//=======================================================================================================
// Возвращает 0, если удалось передать пакет на клавиатуру
// Возвращает 1, если не удалось
//=======================================================================================================
int BKBCOM9Kbd::ArduinoButton(unsigned char c, bool _shift_pressed, bool _ctrl_pressed, bool _alt_pressed)
{
	unsigned char CRC=0xFF;
	unsigned char byte2=0, byte3;

	byte3=c;
	if(!byte3) return 1;

	// Заголовок пакета
	if(BKBSerial::SendByte(CRC, 1)) return 1;

	// Шифты-альты-контролы
	if(_shift_pressed) byte2+=1;
	if(_ctrl_pressed) byte2+=2;
	if(_alt_pressed) byte2+=4;
	
	CRC+=byte2; // пересчитываем CRC

	// шлём второй байт
	if(BKBSerial::SendByte(byte2, 1)) return 1;

	// Шлём клавишу для Arduino
	if(BKBSerial::SendByte(byte3, 1)) return 1;
	CRC+=byte3;
	
	// Пришла очередь CRC
	if(BKBSerial::SendByte(CRC, 1)) return 1;
	
	return 0; // Удалось задействовать внешнюю управляемую клавиатуру!
}

//=======================================================================================================
// Возвращает 0, если удалось передать пакет на клавиатуру
// Возвращает 1, если не удалось
//=======================================================================================================
int BKBCOM9Kbd::Scroll(int scroll_value)
{
	unsigned char CRC=0xFF;
	unsigned char byte2=0, byte3;

	// Заголовок пакета
	if(BKBSerial::SendByte(CRC, 1)) return 1;

	// Здесь всместо шифтов-альтов-контролов выставляем бит скролла
	byte2=8;
	CRC+=byte2; // пересчитываем CRC

	// шлём второй байт
	if(BKBSerial::SendByte(byte2, 1)) return 1;

	// Шлём величину скролла
	if(scroll_value>127) scroll_value=127;
	if(scroll_value<-128) scroll_value=-128;

	// Упаковываем в беззнаковый char 
	if(scroll_value<0) byte3=127-scroll_value;
	else byte3=scroll_value;

	if(BKBSerial::SendByte(byte3, 1)) return 1;
	CRC+=byte3;
	
	// Пришла очередь CRC
	if(BKBSerial::SendByte(CRC, 1)) return 1;
	
	return 0; // Удалось задействовать внешнюю управляемую клавиатуру!
}
