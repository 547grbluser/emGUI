/**
 * @file   StatusBar.c
 * @author Roman Savrulin (romeo.deepmind@gmail.com)
 * @date   10.11.2012
 * @brief  Реализация виджета статус-бара
 *
 * Это - виджет синглтон
 *
 */

#include <stdint.h>

#include "Widgets/StatusBar.h"
 /** @weakgroup prop-widget-statusbar
 *  @{
 */



typedef struct xStatusBarProps_struct {
	uint16_t usColor;          ///< цвет статус-бара
} xStatusBarProps;

static xWidget             *xStatusBarInstance; ///< дескриптор виджета
static xButton             *xCloseButton;       ///< дескриптор крестика
static xLabel              *xWndHeader;         ///< дескриптор заголовка окна

 /**
  * @brief обработчик щелчка на кнопке крестика
  *
  * @returns true - событие обработано
  */
static bool prvCloseClickHandler(xWidget* pxW) {
	vInterfaceCloseActiveWindow();
	return true;
}


bool bStatusBarCreate(uint16_t usColor) {

	xStatusBarInstance = pxWidgetCreate(0, 0, usInterfaceGetW(), LCD_STATUS_BAR_HEIGHT, pxInterfaceGet(), true);
	xStatusBarInstance->eType = WidgetStatusBar;
	vWidgetSetBgColor(xStatusBarInstance, usColor, false);

	uint16_t usX, usY, usW;

	usY = (LCD_STATUS_BAR_HEIGHT - pxDrawHDL()->usGetPictureH(EM_GUI_PIC_CROSS)) / 2;
	usX = LCD_SizeX - pxDrawHDL()->usGetPictureW(EM_GUI_PIC_CROSS) - usY;

	xCloseButton = pxButtonCreate(usX, usY, EM_GUI_PIC_CROSS, xStatusBarInstance);
	vWidgetSetOnClickHandler(xCloseButton, prvCloseClickHandler);

	usY = (usStatusBarGetH() - usFontGetH(FONT_ASCII_16_X)) / 2;
	usW = usFontGetStrW("Default Title", FONT_ASCII_16_X) + 10;
	usX = usStatusBarGetW() / 2 - usW / 2;

	xWndHeader = pxLabelCreate(usX, usY, usW, 0, "Default Title", (xFont)FONT_ASCII_16_X, 100, xStatusBarInstance);
	vWidgetSetBgColor(xWndHeader, usColor, false);
	vLabelSetTextColor(xWndHeader, ColorMenuHeaderText);
	vLabelSetTextAlign(xWndHeader, LABEL_ALIGN_CENTER);


	//vGuiSendEvent(GuiEventRefreshPb, 0);

	return true;
}

void  vStatusBarSetWindowHeader(char * strH) {
	pcLabelSetText(xWndHeader, strH);
}

xWidget *pxStatusBarGet() {
	return xStatusBarInstance;
}



/**
 *  @}
 */
