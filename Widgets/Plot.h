/**
 * @file   Plot.h
 * @author Roman Savrulin (romeo.deepmind@gmail.com)
 * @date   20.11.2012
 * @brief  Интерфейс виджета, выводящего график
 *
 * Виджет отрисовывает график в соответствии с
 * * установленным отведением
 * * установленным масштабом
 * * текущей позицией регистрации сигнала
 *
 * При отрисовке происходит сжатие графика в бар-диаграмму и масштабирование в границы виджета.
 *
 *
 */

#ifndef __PLOT_H
#define __PLOT_H
#include "Widgets/Widget.h"


/**
* @brief масштаб сигнала
*/
typedef enum {
	PLOTScale1mV = 1, ///< 1mV в клетке
	PLOTScale2mV = 2, ///< 2mV в клетке
	PLOTScale4mV = 4, ///< 4mV в клетке
	PLOTScale8mV = 8  ///< 8mV в клетке
} ePLOTScale;

/**
* @brief дескриптор набора данных
*/
typedef struct {
	char const  *sName;           ///< текстовый идентификатор данных

	bool        bWriteEnabled;    ///< данные могут записываться в буферы
	bool        bDataFilled;      ///< флажок, показывающий, что данные заполнены (сняты хотя бы в 1 проход)

	uint32_t    ulWritePos;       ///< позиция указателя записи в буферах xData
	uint32_t    ulElemCount;      ///< количество отсчётов в текущей сессии записи (для отведений контура значения равны)

	short   *psData;          ///< указатель на буфер
	short    sMedian;         ///< среднее значение
	/*short    sMinValue;       ///< минимальное значение
	short    sMaxValue;       ///< максимальное значение
	long     lRange;          ///< размах
	unsigned char cRangeIdx;  ///< индекс масштаба*/
}xLead_t;


#define AFE_DATA_RATE 500

#define PLOTRate (1575) ///< количество отсчетов в 1мв

#define PLOT_1MV   PLOTRate                 ///< количество отсчетов в 1мв
#define PLOT_2MV   (PLOTRate * PLOTScale2mV) ///< количество отсчетов в 2мв
#define PLOT_4MV   (PLOTRate * PLOTScale4mV) ///< количество отсчетов в 4мв
#define PLOT_8MV   (PLOTRate * PLOTScale8mV) ///< количество отсчетов в 8мв

#define PLOT_LOW_LIMIT_COEF  0.82f   ///< нижняя граница переключения масштабной сетки
#define PLOT_HI_LIMIT_COEF   0.95f   ///< верхняя граница переключения масштабной сетки

/** \weakgroup gui-prop-widgets
 *  @{
 */

 /** \addtogroup prop-widget-PLOT-plot PLOT Plot
  *  @{
  */

typedef xWidget xPlot; ///< тип виджета

/**
 * @brief создает виджет
 *
 * @param uX0 - положение по горизонтали левого верхнего угла относительно родительского элемента
 * @param uY0 - положение по веркикали левого верхнего угла относительно родительского элемента
 * @param uX0 - положение по горизонтали правого нижнего угла относительно родительского элемента
 * @param uY0 - положение по веркикали правого нижнего угла относительно родительского элемента
 * @param pxWidParent - родитель виджета
 * @param eLead - отведение для отрисовки
 *
 * @returns указатель на созданный виджет
 */
xPlot * pxPlotCreate(uint16_t usX0, uint16_t usY0, uint16_t usX1, uint16_t usY1, xWidget *pxWidParent, xLead_t * pxL);

/**
 * @brief устанавливает масштаб виджета
 *
 * @param pxW - виджет
 * @param eScale - новый масштаб графика
 *
 */
void vPlotSetScale(xPlot * pxW, ePLOTScale eScale);

/**
 * @brief сбрасывает позицию курсора в виджете
 *
 * @param pxW - виджет
 * @param bInvalidate - перерисовать весь график
 *
 */
void vPlotReset(xPlot *pxW, bool bInvalidate); //TODO(Andrew): рефакторинг на основе позиции при отрисовке проверить использование валидации и инвалидации.
//По возможости избавится от функции, но если bInvalidate важен, то не трогать. Спросить у Романа.
// xPLOTPlotProps_struct - э

/**
 * @brief выставляет цвет графика в виджете
 *
 * @param pxW - виджет
 * @param usColor - цвет
 * @param bInvalidate - перерисовать весь график
 *
 */
void vPlotSetColor(xPlot *pxW, uint16_t usColor, bool bInvalidate);


/**
 *  @}
 */
 /**
  *  @}
  */

#endif	//__PLOT_PLOT_H