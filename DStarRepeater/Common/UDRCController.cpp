/*
 *	UDRCController Copyright (C) 2016 by John Hays, K7VE
 *	based on GPIOController which is
 *	Copyright (C) 2012,2013,2015 by Jonathan Naylor, G4KLX
 *	Copyright (c) 2016 by Jeremy McDermond, NH6Z
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; version 2 of the License.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 */

#include "UDRCController.h"

CUDRCController::CUDRCController(unsigned int config, bool pttInvert) :
		CExternalController(NULL, pttInvert),
		m_config(config)
{
}

CUDRCController::~CUDRCController() 
{
}

#if !defined(GPIO)

bool CUDRCController::open() {
	return false;
}

void CUDRCController::close() {
}

#else

#include <wiringPi.h>

#define ARRAY_SIZE(array) \
	(sizeof(array) / sizeof(*array))
	
enum pins {
	PIN_EXT1,
	PIN_EXT2,
	PIN_PTT,
	PIN_EXT3,
	PIN_EXT4,
	PIN_BASE
};

static const int output_pins[] = { 
	[PIN_EXT1] = 2,
	[PIN_EXT2] = 0,
	[PIN_PTT] = 3, 
	[PIN_EXT3] = 21,
	[PIN_EXT4] = 22 ,
	[PIN_BASE] = 4;
};
static const int input_pins[] = { 5, 6 };

bool CUDRCController::open()
{
	bool ret = ::wiringPiSetup() != -1;
	if (!ret) {
		wxLogError(wxT("Unable to initialise wiringPi"));
		return false;
	}

	for(int i = 0; i < ARRAY_SIZE(input_pins); ++i) {
		::pinMode(input_pins[i], INPUT);
		::pullUpDnControl(input_pins[i], PUD_UP);
	}
	
	for(int i = 0; i < ARRAY_SIZE(output_pins); ++i) {
		::pinMode(output_pins[i], OUTPUT);
		::digitalWrite(output_pins[i], LOW);
	}

	return true;
}

void CUDRCController::setRadioTransmit(bool value) 
{
	value = m_pttInvert ? ~value : value;
	::digitalWrite(output_pins[PIN_PTT], value ? HIGH : LOW);
}

void CUDRCController::setMode(enum repeaterMode mode)
{
	switch(mode) {
		case MODE_AA:
			::digitalWrite(ouptutPins[PIN_EXT1], LOW);
			::digitalWrite(outputPins[PIN_EXT2], LOW);
			break;
		case MODE_AF:
			::digitalWrite(ouptutPins[PIN_EXT1], HIGH);
			::digitalWrite(outputPins[PIN_EXT2], HIGH);
			break;
		case MODE_DD:
			::digitalWrite(ouptutPins[PIN_EXT1], HIGH);
			::digitalWrite(outputPins[PIN_EXT2], LOW);
			break;
		case MODE_FF:
			::digitalWrite(ouptutPins[PIN_EXT1], LOW);
			::digitalWrite(outputPins[PIN_EXT2], HIGH);
			break;
	}
}

void CUDRCController::setActive(bool value)
{
	if(value) {
		::digitalWrite(outputPins[PIN_BASE], LOW);
		setMode(MODE_FF);
	} else {
		::digitalWrite(outputPins[PIN_BASE], HIGH);
		switch(m_config) {
			case 1:
				setMode(MODE_AF);
				break;
			case 3:
				setMode(MODE_DD);
				break;
			case 2:
			default:
				setMode(MODE_AA);
				break;
		}
	}
}

bool CUDRCController::getDisable() const
{
	return false;
}

void CUDRCController::close()
{
}

#endif
