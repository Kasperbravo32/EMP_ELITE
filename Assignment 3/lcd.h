/*
 * setup.h
 *
 *  Created on: 7. mar. 2017
 *      Author: Frederik
 */

#ifndef LCD_H_
#define LCD_H_

    void lcd_init(void);
    void lcd_print(INT8U string_type[]);
    void lcd_shift(INT8U value);
    void lcd_setcursor(INT8U value);
    void lcd_clear(void);

#endif /* LCD_H_ */
