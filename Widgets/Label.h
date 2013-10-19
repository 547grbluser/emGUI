/*
	emGUI Library V1.0.0 - Copyright (C) 2013 
	Lemeshev K <konstantin.lemeshev@gmail.com>, 
	Roman Savrulin <romeo.deepmind@gmail.com>
	

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
	
	Created on: 24.12.2012
*/

/*
 *
 *      ����������� \n �� ������ � ����������� �� ����� ������ !!! ������ ������ �� �����!!!
 *   void vAdaptTextToMultiLine(char *pcText, u16 usLabelWidth, xFont xLabelFont);
 *          pcText - ��������� ��� ����� ��� ������
 *          usLabelWidth - ������ Label
 *          xLabelFont - �����
 *
 *      ������ �� Label MultiLine Label � ������� � �� ����� !!! ������ �� ����� �� ������!!!
 *   void vSetMultilineText(xWidget *pxW, char *pcText);
 *          pxW - Label-����������
 *          pcText -
 */

#ifndef __LABEL_H
#define __LABEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Widget.h"
#include "Font.h"
  
  typedef xWidget xLabel;
  
  typedef enum {
    LABEL_ALIGN_LEFT,
    LABEL_ALIGN_CENTER,
    LABEL_ALIGN_RIGHT,
  } eLabelTextAlign;
  
  typedef enum {
    LABEL_ALIGN_TOP,
    LABEL_ALIGN_MIDDLE,
    LABEL_ALIGN_BOTTOM,
  } eLabelVerticalAlign;
  
  typedef struct xLabelProps_t {
    eLabelTextAlign     eTextAlign;
    eLabelVerticalAlign eVerticalAlign;
    char *pcStr;
    int iMaxLength;
    xFont xFnt;
    u16 usColor;
    bool bIsMultiLine;
    bool bHaveCursor;
    char *pcPrvPage;
    char *pcNxtPage;
    char *pcCrntPage;
    void(*onEditHandler)(void);
  } xLabelProps;
  
  xLabel *    pxLabelCreate(u16 usX, u16 usY, u16 usW, u16 usH, char * cStr, xFont xFnt, int iMaxLength, xWidget *pxWidParent);
  char *      pcLabelSetText(xWidget *pxW, const char * pcStr);
  void        pcLabelSetTextAdaptWidth(xLabel *pxL, char * pcStr); // ������ ������ � �������������� ������ Label ��� ����� ������ ������, �������� ������ ��� 16 ������
  void        vLabelSetTextColor(xWidget *pxW, u16 usColor);
  void        vLabelSetTextAlign(xWidget *pxW, eLabelTextAlign eAlign);
  void        vLabelSetVerticalAlign(xWidget *pxW, eLabelVerticalAlign eAlign);
  void        vLabelSetTextExt(xWidget *pxW, char * pStr, int iMaxLength);
  char *      pcLabelGetText(xWidget *pxW);
  void        vLabelCheckCS(xWidget *pxW);
  bool        bLabelSetMultiline(xWidget *pxW, bool bMultiLine);
  bool        bLabelDrawNextPage(xLabel *pxL);
  bool        bLabelDrawPrevPage(xLabel *pxL);
  int         iLabelGetMaxLength(xLabel *pxL);
  bool        bLabelAppendChar(xWidget *pxW, char cChar, bool bSetInvalidate);
  bool        bLabelBackSpace(xWidget *pxW, bool bSetInvalidate);
  void        vLabelSetOnClickHandler(xWidget *pxW, bool (*callback)(xWidget*));
  void        vLabelSetOnEditHandler(xWidget *pxW, void (*callback)(void));
  void        vLabelSetHaveCursor(xWidget *pxW, bool bLabelHaveCursor);
  void        vLabelClear(xWidget *pxW, bool bSetInvalidate);
#ifdef __cplusplus
}
#endif

#endif	//__LABEL_H
