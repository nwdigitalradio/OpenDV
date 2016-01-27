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

CUDRCController::CUDRCController(unsigned int config) :
		m_config(config), m_outp1(false), m_outp2(false),
                m_outp3(false), m_outp4(false), m_outp5(false),
                m_outp6(false), m_outp7(false), m_outp8(false) {
}

CUDRCController::~CUDRCController() {
}

#if !defined(GPIO)

bool CUDRCController::open() {
	return false;
}

void CUDRCController::getDigitalInputs(bool&, bool&, bool&, bool&, bool&) {
}

void CUDRCController::setDigitalOutputs(bool, bool, bool, bool, bool, bool,
                                        bool, bool) {
}

void CUDRCController::close() {
}

#else

#include <wiringPi.h>

bool CUDRCController::open()
{
	bool ret = ::wiringPiSetup() != -1;
	if (!ret) {
		wxLogError(wxT("Unable to initialise wiringPi"));
		return false;
	}

	::pinMode(5, INPUT);		// Tone Squelch
	::pinMode(6, INPUT);		// Noise Squelch

	// Set pull ups on the input pins
	::pullUpDnControl(5, PUD_UP);
	::pullUpDnControl(6, PUD_UP);

	// Take control as external repeater controller
	::pinMode(4, OUTPUT);

	::pinMode(3, OUTPUT);		// PTT
	::pinMode(2, OUTPUT);		// EXT 1
	::pinMode(0, OUTPUT);		// EXT 2
	::pinMode(22, OUTPUT);		// EXT 3
	::pinMode(21, OUTPUT);		// EXT 4

	//  Start in FM/FM Fixed
	setDigitalOutputs(false, false, false, false, false, false, false, 
                          false);

	return true;
}

void CUDRCController::getDigitalInputs(bool& inp1, bool& inp2, bool& inp3, bool& inp4, bool& inp5)
{
	inp1 = ::digitalRead(5) == LOW;
	inp2 = ::digitalRead(6) == LOW;
	inp3 = LOW;
	inp4 = LOW;
	inp5 = LOW;
}

void CUDRCController::setDigitalOutputs(bool outp1, bool outp2, bool outp3, bool outp4, bool outp5, bool outp6, bool outp7, bool outp8)
{

	if (outp1 != m_outp1) { 		// PTT
		::digitalWrite(3, outp1 ? HIGH : LOW);
		m_outp1 = outp1;
	}

	if (outp2 != m_outp2) {
	//	::digitalWrite(3, outp2 ? HIGH : LOW);
		m_outp2 = outp2;
	}

	if (outp3 != m_outp3) { 		// Heartbeat
	//	::digitalWrite(2, outp3 ? HIGH : LOW);
		m_outp3 = outp3;
	}

	if (outp4 != m_outp4) { // Active (Repeater Takeover)
		//	::digitalWrite(3, outp4 ? HIGH : LOW);
		if (outp4 == HIGH) { 		// FM/FM Fixed
			::digitalWrite(4,LOW);
			::digitalWrite(2,LOW);
			::digitalWrite(0,HIGH);
		} else {
			::digitalWrite(4,HIGH); // Release Control
			switch (m_config) {
				case 1:		// AUTO/FM
					::digitalWrite(2,HIGH);
					::digitalWrite(0,HIGH);
					break;

				case 2:		// AUTO/AUTO
					::digitalWrite(2,LOW);
					::digitalWrite(0,LOW);
					break;

				case 3:		// Digital/Digital
					::digitalWrite(2,HIGH);
					::digitalWrite(0,LOW);
					break;

				default:	// AUTO/AUTO
					::digitalWrite(2,LOW);
					::digitalWrite(0,LOW);
					break;
			}
		}
		m_outp4 = outp4;
	}

	if (outp5 != m_outp5) { // EXT1
	//	::digitalWrite(2, outp5 ? HIGH : LOW);
		m_outp5 = outp5;
	}

	if (outp6 != m_outp6) { // EXT2
	//	::digitalWrite(0, outp6 ? HIGH : LOW);
		m_outp6 = outp6;
	}

	if (outp7 != m_outp7) {
	//	::digitalWrite(22, outp7 ? HIGH : LOW);
		m_outp7 = outp7;
	}

	if (outp8 != m_outp8) {
	//	::digitalWrite(21, outp8 ? HIGH : LOW);
		m_outp8 = outp8;
	}

}

void CUDRCController::close()
{
}

#endif
