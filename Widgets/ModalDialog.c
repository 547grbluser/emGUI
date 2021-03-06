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


#ifdef __cplusplus
extern "C" {
#endif


#include "ModalDialog.h"
#include <stdlib.h>

#define MODAL_DIALOG_MAX_BUTTONS    3
#define MODAL_DIALOG_MAX_COUNT      10
#define MODAL_DIALOG_MAX_MSG_LENGTH 255
#define PB_BORDER                   20

	static xWindow *xThisWnd;

	static xLabel         *xMessage;
	//static xProgressBar   *xPBar;
	static xMenuButton    *xButtons[MODAL_DIALOG_MAX_BUTTONS]; // y(ok)/n/c Максимум в диалоге видно 4 кнопки.

	//автонумерация для автоматических диалогов
	uint16_t usDlgID = MODAL_AUTO + 1;

	typedef struct xModalDialog_t xModalDialog;

	struct xModalDialog_t {
		void(*pxClickHandlers[MODAL_DIALOG_MAX_BUTTONS])   ();

		void(*pxDefaultHandler)   ();

		//bool bActive
		char sDialogConfig[MODAL_DIALOG_MAX_BUTTONS + 1];
		uint16_t  usDlgID;
		char const* sHdr;
		char const* sMsg;
		signed char cProgress;
		bool bCanClose;
		xModalDialog * pxPrev;
	};

	typedef struct xModalDialogPictureSet_t {
		xPicture xPicMain;
		xPicture xPicMainPress;
		char* strLabel;
		//bool (*pxClickHandler)   (xWidget *);
	}xModalDialogPictureSet;

	xModalDialog *xMDActive = NULL;

	//char cDialogCount = 0;
#define MDCurrent (cDialogCount - 1)

	static void prvDlgShowActive();

	static inline xModalDialog *prvGetNextDlg(xModalDialog *xDlg) {
		if (!xDlg)
			return NULL;
		return xDlg->pxPrev;
	}

	static void prvResetDlgWnd() {
		for (int c = 0; c < MODAL_DIALOG_MAX_BUTTONS; c++) {
			vWidgetHide(xButtons[c]);
		}

		vWindowSetHeader(xThisWnd, "");
		pcLabelSetText(xMessage, "");
		//vWidgetHide(xPBar);
	}

	static bool prvOnOpenHandler(xWidget *pxW) {
		prvDlgShowActive();
		return true;
	}

	static bool prvOnOpenRequestHandler(xWidget *pxW) {
		if (xMDActive)
			return true;
		else
			return false;
	}

	static bool prvOnCloseHandler(xWidget *pxW) {
		return true;
	}

	static bool prvOnCloseRequestHandler(xWidget *pxW) {
		if (xMDActive)
			vModalDialogClose(xMDActive->usDlgID, true);

		if (xMDActive)
			return false;
		else
			return true;
	}

	static bool prvButtonHandler(xWidget *pxW) {
		//диалог в обработчике может открыть еще один диалог.
		//при этом ID актовного изменяется. Для сохранения используем
		//эту переменную.
		int usDlgId = xMDActive->usDlgID;
		if (!xMDActive)
			return false;
		for (int c = 0; c < MODAL_DIALOG_MAX_BUTTONS; c++) {
			if (xButtons[c] == pxW && xMDActive->pxClickHandlers[c]) {
				xMDActive->pxClickHandlers[c]();
				break;
			}
		}
		vModalDialogClose(usDlgId, false);
		return true;
	}

	xWidget * pxModalDialogWindowCreate() {

		// X0, Y0 - координаты расположения виджетов
		uint16_t usX, usY;

		xThisWnd = pxWindowCreate(WINDOW_MODAL);
		vWidgetSetBgColor(xThisWnd, ColorEcgBackground, false);
		vWindowSetOnOpenHandler(xThisWnd, prvOnOpenHandler);
		vWindowSetOnOpenRequestHandler(xThisWnd, prvOnOpenRequestHandler);
		vWindowSetOnCloseHandler(xThisWnd, prvOnCloseHandler);
		vWindowSetOnCloseRequestHandler(xThisWnd, prvOnCloseRequestHandler);

		usY = (usInterfaceGetWindowH() * 4) / 10 - usStatusBarGetH();

		xMessage = pxLabelCreate(0, 0, usWidgetGetW(xThisWnd), usY, "ModalDialogText", FONT_ASCII_16_X, MODAL_DIALOG_MAX_MSG_LENGTH, xThisWnd);
		bLabelSetMultiline(xMessage, true);
		vLabelSetTextAlign(xMessage, LABEL_ALIGN_CENTER);
		vLabelSetVerticalAlign(xMessage, LABEL_ALIGN_MIDDLE);

		usY = usWidgetGetY1(xMessage, false);

		//xPBar = pxProgressBarCreate(PB_BORDER, usY, usWidgetGetW(xThisWnd) - PB_BORDER * 2, 30, xThisWnd);
		//vProgressBarSetProcExec(xPBar, 55);

		usY = (usInterfaceGetWindowH() / 2 + pxDrawHDL()->usGetPictureH(EM_GUI_PIC_YES) / 3);
		usX = 0;

		for (int c = 0; c < MODAL_DIALOG_MAX_BUTTONS; c++) {
			xButtons[c] = pxMenuButtonCreate(usX, usY, EM_GUI_PIC_YES, "", prvButtonHandler, xThisWnd);
			usX += LCD_TsBtn_SIZE;
			vWidgetHide(xButtons[c]);
		}
		return xThisWnd;
	}

	xModalDialogPictureSet prvGetPicSet(char cType) {
		xModalDialogPictureSet xPicSet;
		switch (cType) {
			/*case 'y':
			  xPicSet.xPicMain = pxPictureGet(EM_GUI_PIC_YES);
			  xPicSet.xPicMainPress = pxPictureGet(Pic_ButtonOk_press);
			  xPicSet.xPicLabel = pxPictureGet(Pic_b2_yes);*/
		case 'n':
			xPicSet.xPicMain = (EM_GUI_PIC_NO);
			xPicSet.strLabel = "No";
			break;
		case 'c':
			xPicSet.xPicMain = (EM_GUI_PIC_RETURN);
			xPicSet.strLabel = "Cancel";
			break;
		case 'o':
			xPicSet.xPicMain = (EM_GUI_PIC_YES);
			xPicSet.strLabel = "OK";
			break;
		case 'e':
			xPicSet.xPicMain = (EM_GUI_PIC_NO);
			xPicSet.strLabel = "OK";
			break;
		default:
			xPicSet.xPicMain = (EM_GUI_PIC_YES);
			xPicSet.strLabel = "OK";
			break;
		}

		return xPicSet;
	}

	static inline void prvShowPB(xModalDialog * xDlg) {
		if (xDlg->cProgress >= 0) {
			//vWidgetShow(xPBar);
			//vProgressBarSetProcExec(xPBar, xDlg->cProgress);
		}
		else {
			//vWidgetHide(xPBar);
		}
	}

	static void prvDlgShowActive() {
		xModalDialog * xDlg = xMDActive;

		if (!xDlg) {
			//return;
			vInterfaceCloseWindow(WINDOW_MODAL);
			return;
			//TODO: выставить кол-во активных диалогов в 0
		}

		uint16_t cBtnCnt = (uint16_t)strlen(xDlg->sDialogConfig);
		xModalDialogPictureSet xPicSet;

		char * sBtns = xDlg->sDialogConfig;

		xMenuButton * xBtn;

		uint16_t betweenBtnsX,
			usX, usY;

		prvResetDlgWnd();

		betweenBtnsX = (usInterfaceGetW() - cBtnCnt * usWidgetGetW(xButtons[0])) / (cBtnCnt + 1);
		usY = usWidgetGetY0(xButtons[0], false);
		usX = betweenBtnsX;

		vWindowSetHeader(xThisWnd, xDlg->sHdr);
		pcLabelSetText(xMessage, xDlg->sMsg);

		prvShowPB(xDlg);

		for (int c = 0; c < cBtnCnt; c++) {
			xBtn = xButtons[c];
			xPicSet = prvGetPicSet(sBtns[c]);

			bWidgetMoveTo(xBtn, usX, usY);
			vWidgetShow(xBtn);

			pxMenuButtonSetMainPic(xBtn, xPicSet.xPicMain);
			//pxMenuButtonSetPushPic(xBtn, xPicSet.xPicMainPress);
			pxMenuButtonSetLabelText(xBtn, xPicSet.strLabel);

			usX += betweenBtnsX + usWidgetGetW(xBtn);
		}

	}

	void prvIncDlgId() {
		//TODO: thread protection???
		usDlgID++;
		if (usDlgID <= MODAL_AUTO)
			usDlgID = MODAL_AUTO + 1;
	}

	xModalDialog *prvDlgIsActive(int iDlgId) {
		if (!xMDActive)
			return NULL;

		if (xMDActive->usDlgID == iDlgId)
			return xMDActive;

		return NULL;
	}

	xModalDialog *prvDlgIsOpened(int iDlgId, xModalDialog ** pxNext) {
		xModalDialog * xDlg = xMDActive;

		*pxNext = NULL;

		while (xDlg) {
			if (xDlg->usDlgID == iDlgId)
				return xDlg;
			*pxNext = xDlg;
			xDlg = prvGetNextDlg(xDlg);
		}

		return NULL;
	}

	inline xModalDialog *prvDelDlgFromStack(xModalDialog *pxN, xModalDialog *pxNext) {

		xModalDialog * pxPrev; //пред. диалог в стеке
		pxPrev = pxN->pxPrev;
		pxNext->pxPrev = pxPrev;

		return pxN;
	}

	void prvDlgRefresh(xModalDialog * xDlg, char const* sBtns, char const* sHdr, char const* sMsg) {
		if (!xDlg)
			return;

		xDlg->sHdr = sHdr;
		xDlg->sMsg = sMsg;
		xDlg->cProgress = -1;
		xDlg->bCanClose = true;
		memcpy(xDlg->sDialogConfig, sBtns, MODAL_DIALOG_MAX_BUTTONS + 1);
		xDlg->sDialogConfig[MODAL_DIALOG_MAX_BUTTONS] = '\0';

		for (int c = 0; c < MODAL_DIALOG_MAX_BUTTONS; c++)
			xDlg->pxClickHandlers[c] = NULL;

		xDlg->pxDefaultHandler = NULL;
	}

	int iModalDialogOpen(int iDlgId, char const * sBtns, char const * sHdr, char const* sMsg) {
		xModalDialog * xDlg;
		xModalDialog * xDlgNext;

		/*/Проверка ограничения макс. количества открытых диалогов
		if(cDialogCount >= MODAL_DIALOG_MAX_COUNT){
		  vInterfaceOpenWindow(WINDOW_MODAL);
		  return -1;
		}*/

		//Если такой диалог уже активен - Сбросить его обработчики и конфигурацию.
		if ((xDlg = prvDlgIsActive(iDlgId))) {
			prvDlgRefresh(xDlg, sBtns, sHdr, sMsg);
			prvDlgShowActive();
			vInterfaceOpenWindow(WINDOW_MODAL);
			return -1;
		}

		//Диалог есть где-то в стеке, его надо поднять
		if ((xDlg = prvDlgIsOpened(iDlgId, &xDlgNext))) {
			prvDlgRefresh(xDlg, sBtns, sHdr, sMsg);
			prvDelDlgFromStack(xDlg, xDlgNext);
		}
		else {
			//Диалога в стеке нет, создаем новую структуру
			xDlg = malloc(sizeof(xModalDialog));
			//cDialogCount ++;
			memcpy(xDlg->sDialogConfig, sBtns, MODAL_DIALOG_MAX_BUTTONS + 1);
			xDlg->sDialogConfig[MODAL_DIALOG_MAX_BUTTONS] = '\0';
			xDlg->sHdr = sHdr;
			xDlg->sMsg = sMsg;
			xDlg->cProgress = -1;
			xDlg->bCanClose = true;
			xDlg->pxDefaultHandler = NULL;

			for (int c = 0; c < MODAL_DIALOG_MAX_BUTTONS; c++)
				xDlg->pxClickHandlers[c] = NULL;

			//Выставляем идентификатор диалога, по которому
			//будет возможность дальнейшей работы с этим диалогом.
			if (iDlgId != MODAL_AUTO)
				xDlg->usDlgID = iDlgId;
			else {
				xDlg->usDlgID = usDlgID;
				prvIncDlgId();
			}
		}

		//делаем диалог активным
		xDlg->pxPrev = xMDActive;
		xMDActive = xDlg;

		prvDlgShowActive();

		vInterfaceOpenWindow(WINDOW_MODAL);
		vWidgetInvalidate(xThisWnd);

		return xDlg->usDlgID;
	}

	void vModalDialogSetHandler(int iDlgID, char cHandler, void(*pxHandler)()) {
		xModalDialog * xDlg;
		xModalDialog * xDlgNext;

		if (!(xDlg = prvDlgIsOpened(iDlgID, &xDlgNext)))
			return;

		for (int c = 0; c < MODAL_DIALOG_MAX_BUTTONS; c++) {
			if (xDlg->sDialogConfig[c] == cHandler) {
				xDlg->pxClickHandlers[c] = pxHandler;
				return;
			}
		}
		xDlg->pxDefaultHandler = pxHandler;
	}

	void vModalDialogSetCloseable(int iDlgID, bool bCanClose) {
		xModalDialog * xDlg;
		xModalDialog * xDlgNext;

		if (!(xDlg = prvDlgIsOpened(iDlgID, &xDlgNext)))
			return;

		xDlg->bCanClose = bCanClose;
	}


	void vModalDialogSetProgress(int iDlgID, int iProgress) {
		xModalDialog * xDlg;
		xModalDialog * xDlgNext;

		if (!(xDlg = prvDlgIsOpened(iDlgID, &xDlgNext)))
			return;

		if (iProgress < 0)
			iProgress = -1;

		if (iProgress > 100)
			iProgress = 100;

		if (xDlg->cProgress == iProgress)
			return;

		xDlg->cProgress = iProgress;

		if (prvDlgIsActive(iDlgID)) {
			prvShowPB(xDlg);
		}
	}

	void vModalDialogClose(int iDlgID, bool bFireDefault) {
		xModalDialog * xDlg;
		xModalDialog * xDlgNext;

		xDlg = prvDlgIsOpened(iDlgID, &xDlgNext);

		//такой диалог не был в стеке
		if (!xDlg)
			return;

		//если диалог нельзя закрывать
		if (!xDlg->bCanClose)
			return;

		//если диалог активный, то ставим активным предыдущий.
		if (prvDlgIsActive(iDlgID)) {
			xMDActive = xMDActive->pxPrev;
		}
		else
			prvDelDlgFromStack(xDlg, xDlgNext);

		//генерируем событие по умолчанию, если надо и оно есть.
		if (bFireDefault) {
			if (xDlg->pxDefaultHandler)
				xDlg->pxDefaultHandler(NULL);
		}
		free(xDlg);
		prvDlgShowActive();
	}


#ifdef __cplusplus
}
#endif
