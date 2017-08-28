/**
* @file   MenuButton.h
* @author Lemeshev K (konstantin.lemeshev@gmail.com)
* @date   21.02.2014
* @brief  ���������� ������� ������ ��� ���� � ��������
*
*/
#ifndef MENUBUTTON_H_
#define MENUBUTTON_H_

/** \weakgroup gui-prop-widgets
*  @{
*/

/** \addtogroup prop-widget-menu-button Menu Button
*  @{
*/

#include "Widget.h"
#include "Button.h"
#include "PictureStorage.h"

typedef xWidget xMenuButton; ///< ��� ��� ������ ����

							 /**
							 * @brief ������� ������ ������
							 *
							 * @param uX - ��������� �� ����������� ������������ ������������� ��������
							 * @param uY - ��������� �� ��������� ������������ ������������� ��������
							 * @param xPicFG - ��������� �� ��������
							 * @param strLabel - ������� ��� �������
							 * @param pvClickHanlder - ���������� �������
							 * @param pxWidParent - �������� �������
							 *
							 * @returns ��������� �� ��������� ������
							 */
xMenuButton * pxMenuButtonCreate(u16 usX, u16 usY, xPicture xPicFG, char* strLabel, bool(*pvClickHanlder) (xWidget *), xWidget *pxWidParent);

/**
* @brief ������������� �������� �� ������
*
* @param pxW - ������
* @param xPic - ��������� �� ��������
*
*/
void pxMenuButtonSetMainPic(xWidget * pxW, xPicture xPic);

/**
* @brief ������������� ������� ��� �������
*
* @param pxW - ������
* @param xPic - ��������� ������
*
*/
void pxMenuButtonSetLabelText(xWidget * pxW, char const* strL);

/**
*  @}
*/
/**
*  @}
*/

#endif /* MENUBUTTON_H_ */
