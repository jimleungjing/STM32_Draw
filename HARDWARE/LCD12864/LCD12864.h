#ifndef LCD12864_H_
#define LCD12864_H_


#include"sys.h"

#define LCD_CS_GPIO		GPIOC
#define LCD_SID_GPIO	GPIOC
#define LCD_SCLK_GPIO	GPIOC

#define LCD_CS_BIT		GPIO_Pin_13
#define LCD_SID_BIT		GPIO_Pin_14
#define LCD_SCLK_BIT	GPIO_Pin_15

#define	LCD_CS_H		GPIO_SetBits(LCD_CS_GPIO,LCD_CS_BIT)
#define	LCD_CS_L		GPIO_ResetBits(LCD_CS_GPIO,LCD_CS_BIT)
#define	LCD_SID_H		GPIO_SetBits(LCD_SID_GPIO,LCD_SID_BIT)
#define	LCD_SID_L		GPIO_ResetBits(LCD_SID_GPIO,LCD_SID_BIT)
#define	LCD_SCLK_H		GPIO_SetBits(LCD_SCLK_GPIO,LCD_SCLK_BIT)
#define	LCD_SCLK_L		GPIO_ResetBits(LCD_SCLK_GPIO,LCD_SCLK_BIT)



void LCD12864_PORT_init(void);



#define LCD_ROW                          4
#define LCD_COL_ENG                      16
#define LCD_COL                          8

#define LCD_GROW                         64
#define LCD_GCOL                         8
#define LCD_GCOL_CHAR                    16
#define LCD_GCOL_DOT                     128
#define LCD_GX                           LCD_GROW
#define LCD_GY                           LCD_GCOL_DOT

//Command
	//Config 8bits
#define LCD_RE_0                         (0x30) //基本指令集
#define LCD_RE_1                         (0x34) //拓展指令集

//基本指令集
#define LCD_TEXT_CLR                     (0x01)
#define LCD_AC_RST                       (0x02)

#define LCD_BIT                          (0x04)
#define LCD_BIT_RIGHT                    (0x02)
#define LCD_BIT_DISPLAYMOVEEN            (0x01)
#define LCD_BIT_MODIFY                   (LCD_BIT+LCD_BIT_RIGHT)

#define LCD_DISPLAY                      (0X08)
#define LCD_DISPLAY_ALL                  (0x04)
#define LCD_DISPLAY_CURSOR               (0x02)
#define LCD_DISPLAY_BCURSOR              (0x01)
#define LCD_DISPLAY_OFF                  (LCD_DISPLAY)
#define LCD_DISPLAY_ON                   (LCD_DISPLAY+LCD_DISPLAY_ALL)

#define LCD_DDRAM                        (0x80)
#define LCD_DDRAM_POS(row, col)          (LCD_DDRAM+((row)<<4)-(((row)>1)?0x18:0x00)+(col))

//拓展指令集
#define LCD_WAIT                         (0x01)

#define LCD_SCROLL_ON                    (0x03)
#define LCD_SCROLL_OFF                   (0x02)

#define LCD_SLEEP_ON                     (0x0C)
#define LCD_SLEEP_OFF                    (0x08)

#define LCD_GRAPHIC_ON                   (0x36)
#define LCD_GRAPHIC_OFF                  (0x34)

#define LCD_GRAM                         (0x80)
#define LCD_GRAM_AC(AC)                  (LCD_GRAM+(AC))
#define LCD_SET_IRAM                     (0x02)
#define LCD_SET_SR                       (0x03)

extern void LCD_init(void);

extern void LCD_TClr(void);
extern void LCD_TInt(int row, int col, unsigned char *data);
//由梁宇钦加上的函数
extern void LCD_TSentence(int row,int col,unsigned char *data);
extern void LCD_TData(int row,int col,double data);
extern void array_move_L(unsigned char *temp);

extern void LCD_TMultiLineLengthEnd(int row, int col, unsigned char *data, int len, unsigned char const end);
#define LCD_TMultiLineEndOnce(row, col, data, end) \
	LCD_TMultiLineLengthEnd(row, col, data, LCD_ROW*LCD_COL, end)
#define LCD_TMultiLineEndAll(row, col, data, end) \
	LCD_TMultiLineLengthEnd(row, col, data, -1, end)
#define LCD_TMultiLineEnd(row, col, data, end) \
	LCD_TMultiLineLengthEnd(row, col, data, LCD_ROW*LCD_COL-(row)*LCD_COL-(col), end)
#define LCD_TMultiLineOnce(row, col, data) \
	LCD_TMultiLineLengthEnd(row, col, data, LCD_ROW*LCD_COL, '\0')
#define LCD_TMultiLineAll(row, col, data) \
	LCD_TMultiLineLengthEnd(row, col, data, -1, '\0')
#define LCD_TMultiLine(row, col, data) \
	LCD_TMultiLineLengthEnd(row, col, data, LCD_ROW*LCD_COL-(row)*LCD_COL-(col), '\0')

extern void LCD_TLineLengthEnd(int row, int col, unsigned char *data, int len, unsigned char const end);
#define LCD_TLineEndOnce(row, col, data, end) \
	LCD_TLineLengthEnd(row, col, data, LCD_COL, end)
#define LCD_TLineEndAll(row, col, data, end) \
	LCD_TLineLengthEnd(row, col, data, -1, end)
#define LCD_TLineEnd(row, col, data, end) \
	LCD_TLineLengthEnd(row, col, data, LCD_COL-(col), end)
#define LCD_TLineOnce(row, col, data) \
	LCD_TLineLengthEnd(row, col, data, LCD_COL, '\0')
#define LCD_TLineAll(row, col, data) \
	LCD_TLineLengthEnd(row, col, data, -1, '\0')
#define LCD_TLine(row, col, data) \
	LCD_TLineLengthEnd(row, col, data, LCD_COL-(col), '\0')
#define LCD_TL(row, data) \
	LCD_TLineLengthEnd(row, 0, data, LCD_COL, '\0')


extern unsigned char LCDGD[LCD_GROW][LCD_GCOL_CHAR];

extern void LCD_Text(void);
extern void LCD_Graph(void);
extern void LCD_GPosSet(int row, int col);
extern void LCD_GClr(void);
extern void LCD_GInt(int row, int col);

extern unsigned char LCD_GGet(int x, int y);

extern void LCD_GSetDot(int x, int y);
extern void LCD_GResDot(int x, int y);
extern void LCD_GNotDot(int x, int y);
extern void LCD_GLine(int x1, int y1, int x2, int y2, void LCD_GDot(int x, int y));
extern void LCD_GSetLine(int x1, int y1, int x2, int y2);
extern void LCD_GResLine(int x1, int y1, int x2, int y2);
extern void LCD_GNotLine(int x1, int y1, int x2, int y2);
extern void LCD_GRect(int x1, int y1, int x2, int y2, void LCD_G(unsigned char*, unsigned char));

#endif

