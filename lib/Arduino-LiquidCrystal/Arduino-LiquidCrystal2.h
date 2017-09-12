#define CLEAR_DISPLAY  0b00000001
#define RETURN_HOME    0b00000010
#define ENTRY_MODE     0b00000100
#define DISPLAY_CTRL   0b00001000
#define CUR_DISP_SHIFT 0b00010000
#define FUNCTION_SET   0b00100000
#define SET_CGRAM_ADDR 0b01000000
#define SET_DDRAM_ADDR 0b10000000

#define INCREMENT 0x02
#define DECREMENT 0x00
#define ACCOMPANY_DISP_SHIFT 0x01

#define DISPLAY_ON 0x04
#define DISPLAY_OFF 0x00
#define CURSOR_ON 0x02
#define CURSOR_OFF 0x00
#define BLINK_CURSOR_ON 0x01
#define BLINK_CURSOR_OFF 0x00

#define DISPLAY_SHIFT 0x08
#define CURSOR_MOVE 0x00

#define SHIFT_RIGHT 0x04
#define SHIFT_LEFT 0x00

#define MODE_8BIT 0x10
#define MODE_4BIT 0x00

#define MODE_2LINE 0x08
#define MODE_1LINE 0x00

#define MODE_5X10_FONT 0x04
#define MODE_5X8_FONT 0x00

#include <Arduino.h>

class LiquidCrystal: public Print {
  public:
    LiquidCrystal(uint8_t _rs, uint8_t _en, uint8_t _d4, uint8_t _d5, uint8_t _d6, uint8_t _d7);
    void begin(uint8_t _cols, uint8_t _rows);
    void clear();
    void home();
    void setCursor(uint8_t _col, uint8_t _row);
    void cursor(uint8_t enabled);
    void blink_cursor_pos(uint8_t enabled);
    void display(uint8_t enabled);
  private:
    uint8_t rs = 255,
    en = 255;

    uint8_t data_pins[8] = {255};

    uint8_t _display_ctrl = 0;

    uint8_t cols = 0, rows = 0;

    void write_byte(uint8_t _rs, uint8_t data);
    void write_nibble(uint8_t _rs, uint8_t data);

    virtual size_t write(uint8_t data);
};
