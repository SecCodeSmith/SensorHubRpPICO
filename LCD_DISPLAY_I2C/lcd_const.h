#ifndef PROJECTPMC_LCD_CONST_H
#define PROJECTPMC_LCD_CONST_H

// commands
#define LCD_CLEAR_DISPLAY 0x01
#define LCD_RETURN_HOME 0x02
#define LCD_ENTRY_MODE_SET 0x04
#define LCD_DISPLAY_CONTROL 0x08
#define LCD_CURSOR_SHIFT 0x10
#define LCD_FUNCTION_SET 0x20
#define LCD_SET_CGRAM_ADDR 0x40
#define LCD_SET_DDRAM_ADDR 0x80

// flags for display entry mode
#define LCD_ENTRY_SHIFT_INCREMENT 0x01
#define LCD_ENTRY_LEFT 0x02

// flags for display on/off control
#define LCD_BLINK_ON 0x01
#define LCD_CURSOR_ON 0x02
#define LCD_DISPLAY_ON 0x04

// flags for display and cursor control
#define LCD_BLINK_OFF 0x00
#define LCD_BLIK_ON 0x01
#define LCD_CURSOR_ON 0x02
#define LCD_DISPLAY_ON 0x04

// flags for display and cursor shift
#define LCD_MOVE_CURSOR_RIGHT 0x04
#define LCD_DISPLAY_MOVE 0x08

// flags for function set
#define LCD_5x10DOTS 0x04
#define LCD_2LINE 0x08
#define LCD_8BIT_MODE 0x10

// flags for backlight control
#define LCD_BACKLIGHT 0x08

#define LCD_ENABLE_BIT 0x04

#define DELAY_US 1000

// Modes for send_byte function
#define LCD_COMMAND 0
#define LCD_CHARACTER 1

#endif //PROJECTPMC_LCD_CONST_H