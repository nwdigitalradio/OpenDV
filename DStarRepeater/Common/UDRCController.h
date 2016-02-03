/*
 *      UDRCController Copyright (C) 2016 by John Hays, K7VE
 *      based on GPIOController which is
 *	Copyright (C) 2012,2015 by Jonathan Naylor, G4KLX
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

#ifndef	UDRCController_H
#define	UDRCController_H

#include "ExternalController.h"

#include <wx/wx.h>

class CUDRCController : public CExternalController {
public:
	CUDRCController(unsigned int config, bool pttInvert);
	virtual ~CUDRCController();

	virtual bool open();

	virtual bool getDisable() const;

	virtual void setRadioTransmit(bool value);
	virtual void setActive(bool value);

	virtual void close();

	virtual void* Entry();

private:
	enum m_repeaterMode {
		MODE_AA;
		MODE_DD;
		MODE_FF;
		MODE_AF;
	};
	void setMode(m_repeaterMode mode);
	unsigned int m_config;
};

#endif
