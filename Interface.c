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
	
	Created on: 14.11.2012
*/

#ifndef __INTERFACE_C
#define __INTERFACE_C

#include "Interface.h"
#include <stdio.h>
#include "GUI/Color.h"
#include "Buzzer.h"
#include "Debug.h"
#include "System.h"

#define DEBUG_MODULE_THIS MODULE_GUI_TASK

xInterface *xInterfaceInstance;
xTouchEventType eTouchState = popTs;

xWindow * xActiveWindow = NULL;

xInterface * pxInterfaceCreate(bool (*pxOnCreateHandler)(xWidget *)){
  
  //gui_SetLanguage(Russian);
  //gui_SetLanguage(English);
  
  xInterfaceInstance = pxWidgetCreate(0, 0, LCD_SizeX, LCD_SizeY, NULL, TRUE);
  
  if(pxOnCreateHandler)
    pxOnCreateHandler(xInterfaceInstance);
  
  return xInterfaceInstance;
}

void vInterfaceDraw(){
  //� ���������� ��� ��������, ������� ���� ������ ���� ������� ������
  xInterfaceInstance->bInvalidateParent = FALSE;
  vWidgetDraw(xInterfaceInstance);
}

xInterface *pxInterfaceGet(){
  return xInterfaceInstance;
}

void vInterfaceInvalidate(){
  vWidgetInvalidate(xInterfaceInstance);
}

void vInterfaceBeep(){
  vBuzzerBeepNoLock(1000, 10);
}

void bInterfaceCheckTouchScreenEvent(){
  xTouchEvent xTouchRxedEvent;
  
  while (iTouchScreenGetEvent(&xTouchRxedEvent) == pdTRUE){
    if(!bSystemIsPowerFull())
      continue;
    vSystemResetTimers();
    MDEBUG("Touch Ev: %d; X: %d; Y: %d; xRaw: %d; yRaw: %d",
        xTouchRxedEvent.eventTouchScreen,
        xTouchRxedEvent.xTouchScreen,
        xTouchRxedEvent.yTouchScreen,
        xTouchRxedEvent.xRaw,
        xTouchRxedEvent.yRaw);
    //send only toggled events to widgets
    if(eTouchState != xTouchRxedEvent.eventTouchScreen){
      eTouchState = xTouchRxedEvent.eventTouchScreen;
      if(bWidgetCheckTouchScreenEvent(xInterfaceInstance, &xTouchRxedEvent))
        vInterfaceBeep();
    }
  }
}

void vInterfaceDebug(bool bDebug){
  vDebugSetModuleLogLevel(MODULE_GUI_TASK, LOG_DEBUG);
  vInterfaceInvalidate();
}

bool bInterfaceGetDebug(){
  return (iDebugGetModuleLogLevel(MODULE_GUI_TASK) >= LOG_DEBUG)?TRUE:FALSE;
}

xWindow * pxInterfaceIsWindowOpened(eWindow eWnd, xWindow ** pxNext){
  xWindowProps *xP;
  xWindow *xW = xActiveWindow;
  char cLimit = 100; //������ �� ������������

  *pxNext = NULL;

  do {
    if(!(xP = pxWidgetGetProps(xW, WidgetWindow)))
      return NULL;
    if ( xP->eId == eWnd)
        return xW;
    *pxNext = xW;
  }while((xW = pxWindowGetBack(xW)) && cLimit--);

  //TODO: ��������� cLimit

  return NULL;
}

xWindow * pxInterfaceIsWindowActive(eWindow eWnd){
  xWindowProps *xP;

  if(!(xP = pxWidgetGetProps(xActiveWindow, WidgetWindow)))
    return NULL;

  if(xP->eId == eWnd)
    return xActiveWindow;

  return NULL;
}

void inline prvDelWndFromStack(xWindow *pxN, xWindow *pxNext){

  xWidget * pxPrev; //����. ���� � �����
  xWindowProps *xP;

  if(!(xP = pxWidgetGetProps(pxN, WidgetWindow)))
    return; //������! TODO: ����������

  pxPrev = xP->xBackWindow;

  if(!(xP = pxWidgetGetProps(pxNext, WidgetWindow)))
    return; //������! TODO: ����������

  xP->xBackWindow = pxPrev;
}

void vInterfaceOpenWindow(eWindow eWnd){
  xWidget * pxN,    //������� ���� � �����
          * pxNext = NULL; //��������� ���� � �����
  xWindowProps *xP;

  //���� ���� ��� �������, �� ������ ��� ���������
  //TODO: ����� �� ������������ ������� � ���� ������??? openRequest???
  if(pxInterfaceIsWindowActive(eWnd)){
    vInterfaceUpdateWindow();
    return;
  }

  //���� ���� ���������, �� pxNext != NULL
  if((pxN = pxInterfaceIsWindowOpened(eWnd, &pxNext))){
    //������� ������ �� �����

    prvDelWndFromStack(pxN, pxNext);

    //������ ���� � ������� ����� (������ ��� Active)
    if(!(xP = pxWidgetGetProps(pxN, WidgetWindow)))
      return; //������! TODO: ����������

    xP->xBackWindow = xActiveWindow;

    vWidgetHide(xActiveWindow);
    vWidgetShow(pxN);
    
    xActiveWindow = pxN;
    vInterfaceUpdateWindow();
    return;
  }

  //���� ������� - ��������� ��� � ����
  //����� ���� ���� � ����������
  //��������� �� ������ ������-������� ����������
  pxN = xInterfaceInstance->pxChild;
  while( pxN ){

    if(!(xP = pxWidgetGetProps(pxN, WidgetWindow))){
      pxN = pxWidgetGetNext(pxN);
      continue;
    }

    if(xP->eId == eWnd){
      //�������� ��� �� ��������� ����, ������� �� ���. ��������
      if(pxN != xActiveWindow){
        if(xP->pxOnOpenRequest)
          if(! xP->pxOnOpenRequest(pxN))
            //�������� ��������, ���� ���������� ������ FALSE
            break;

        xP->xBackWindow = xActiveWindow;

        vWidgetHide(xActiveWindow);
        xActiveWindow = pxN;
        if(xP->pxOnOpen)
          xP->pxOnOpen(pxN);
        //�������� �� �������� �� ������ ��� ���� � ����������� onOpen
        if(xActiveWindow == pxN){
          vWidgetShow(xActiveWindow);
          vInterfaceUpdateWindow();
        }
      }
      break; //������ ���� ���� ����� ���� ������.
    }
    pxN = pxWidgetGetNext(pxN);
  }
}

void vInterfaceCloseActiveWindow(){
  xWindowProps *xP;
  
  //������������ ��������
  //TODO: ������� ���� �� ���������
  if(!xActiveWindow)
    return;

  if(!(xP = pxWidgetGetProps(xActiveWindow, WidgetWindow)))
    return;
  
  //��������� �������� �� ������� ���� � ������ ������
  if(xP->pxOnCloseRequest)
    if(!xP->pxOnCloseRequest(xActiveWindow))
      return;

  if(xP->xBackWindow){

    vWidgetHide(xActiveWindow);
    xActiveWindow = xP->xBackWindow;
    xP->xBackWindow = NULL;
    vWidgetShow(xActiveWindow);

    if(xP->pxOnClose)
      xP->pxOnClose(xActiveWindow);
    vInterfaceUpdateWindow();
  }
  /*
  else
    NVIC_SystemReset(); //��DO: ������ - �������������, low power state*/
}

void vInterfaceCloseWindow(eWindow eWnd){
  xWidget * pxN = xActiveWindow,
          * pxNext = NULL;
  xWindowProps *xP;

  if(pxInterfaceIsWindowActive(eWnd)){
    vInterfaceCloseActiveWindow();
    return;
  }

  if((pxN = pxInterfaceIsWindowOpened(eWnd, &pxNext))){
    if(!(xP = pxWidgetGetProps(pxN, WidgetWindow)))
      return;

    //��������� �������� �� ������� ���� � ������ ������
    if(xP->pxOnCloseRequest)
      if(!xP->pxOnCloseRequest(pxN)){
        //���� ����������, �� ���� ������ ����� �������� ��� ���������� �������
        vInterfaceOpenWindow(eWnd);
        return;
      }

    prvDelWndFromStack(pxN, pxNext);

    //��������� �������� �� ������� ���� � ������ ������
    if(xP->pxOnClose)
      if(!xP->pxOnClose(pxN))
        return;
  }
}

void vInterfaceUpdateWindow(){
#ifndef BOOTLOADER
  xWindowProps *xP;
  if(!(xP = pxWidgetGetProps(xActiveWindow, WidgetWindow)))
    return;

  if(xP->bFullScreen){
    vWidgetHide(pxStatusBarGet());
  }else{
    vWidgetShow(pxStatusBarGet());
  }
  vStatusBarSetWindowHeader(xP->strHeader);
#endif
}

#endif	//__INTERFACE_C
