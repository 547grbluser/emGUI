/*
	emGUI Library V1.0.0 - Copyright (C) 2013 Roman Savrulin <romeo.deepmind@gmail.com>

	This file is part of the emGUI Library distribution.

	emGUI Library is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License (version 2) as published by the
	Roman Savrulin AND MODIFIED BY the emGUI Library exception.

	>>>NOTE<<< The modification to the GPL is included to allow you to
	distribute a combined work that includes emGUI Library without being obliged to
	provide the source code for proprietary components outside of the emGUI Library.
	emGUI Library is distributed in the hope that it will be useful, but
	WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
	or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
	more details. You should have received a copy of the GNU General Public
	License and the emGUI Library license exception along with emGUI Library; if not it
	can be obtained by writing to Roman Savrulin <romeo.deepmind@gmail.com>.

	Created on: 06.03.2013
*/

#ifndef MODALDIALOG_H_
#define MODALDIALOG_H_


#include "Widgets/Label.h"
#include "Widgets/Button.h"
#include "Widgets/StatusBar.h"
#include "Widgets/Window.h"
#include "Widgets/MenuButton.h"

//#include "GuiInterface.h"
//#include "GsmInterface.h"
//#include "Memory.h"
#include <string.h>
//#include "Widgets/ProgressBar.h"



#ifdef __cplusplus
extern "C" {
#endif

	typedef enum {
		MODAL_AUTO,
	}eModalDialog;

	xWidget * pxModalDialogWindowCreate();
	int iModalDialogOpen(int iDlgId, char const * sBtns, char const * sHdr, char const* sMsg);
	void vModalDialogClose(int iDlgID, bool bFireDefault);
	void vModalDialogSetHandler(int iDlgID, char cHandler, void(*pxHandler)());
	void vModalDialogSetProgress(int iDlgID, int iProgress);
	void vModalDialogSetCloseable(int iDlgID, bool bCanClose);


#ifdef __cplusplus
}
#endif

#endif /* MODALDIALOG_H_ */
