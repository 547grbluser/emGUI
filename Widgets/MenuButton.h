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

#include "Widgets/Widget.h"
#include "Widgets/Button.h"
#include "Widgets/Label.h"


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

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
xMenuButton * pxMenuButtonCreate(uint16_t usX, uint16_t usY, unsigned short const*  pusPic, char* strLabel, bool(*pvClickHanlder) (xWidget *), xWidget *pxWidParent);

/**
* @brief ������������� �������� �� ������
*
* @param pxW - ������
* @param xPic - ��������� �� ��������
*
*/
void pxMenuButtonSetMainPic(xWidget * pxW, unsigned short const*  pusPic);

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
#ifdef __cplusplus
}
#endif // __cplusplus
#endif /* MENUBUTTON_H_ */
