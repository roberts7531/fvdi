#ifndef INCLUDE_V99X8_V99X8_H
#define INCLUDE_V99X8_V99X8_H

#include "util.h"
//#include <machine.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// These should be somewhere else
#define V99X8_VRAM_DATA_RW 0xC00001
#define V99X8_STATUS_REGISTER_R 0xC00003
#define V99X8_VRAM_ADDRESS_W 0xC00003
#define V99X8_REGISTER_SETUP_W 0xC00003
#define V99X8_PALETTE_REGISTERS_W 0xC00005
#define V99X8_REGISTER_INDIRECT_ADDRESSING_W 0xC00007

#define V99X8_R(reg) (*(volatile uint8_t *) (reg))

short colours[];
// Control Registers

enum v99x8_control_register {
    // Mode Registers
    V99X8_CONTROL_REGISTER_MODE_R0                            = 0,
    V99X8_CONTROL_REGISTER_MODE_R1                            = 1,
    V99X8_CONTROL_REGISTER_MODE_R8                            = 8,
    V99X8_CONTROL_REGISTER_MODE_R9                            = 9,

    // Table Base Address Registers
    V99X8_CONTROL_REGISTER_PATTERN_LAYOUT_TABLE               = 2,
    V99X8_CONTROL_REGISTER_COLOR_TABLE_LOW                    = 3,
    V99X8_CONTROL_REGISTER_COLOR_TABLE_HIGH                   = 10,
    V99X8_CONTROL_REGISTER_PATTERN_GENERATOR_TABLE            = 4,
    V99X8_CONTROL_REGISTER_SPRITE_ATTRIBUTE_TABLE_LOW         = 5,
    V99X8_CONTROL_REGISTER_SPRITE_ATTRIBUTE_TABLE_HIGH        = 11,
    V99X8_CONTROL_REGISTER_SPRITE_PATTERN_GENERATOR_TABLE     = 6,

    // Color Registers
    V99X8_CONTROL_REGISTER_TEXT_AND_SCREEN_MARGIN_COLOR       = 7,
    V99X8_CONTROL_REGISTER_TEXT_AND_BACKGROUND_BLINK_COLOR    = 12,

    V99X8_CONTROL_REGISTER_BLINKING_PERIOD_REGISTER           = 13,

    V99X8_CONTROL_REGISTER_COLOR_BURST_REGISTER_1             = 20,
    V99X8_CONTROL_REGISTER_COLOR_BURST_REGISTER_2             = 21,
    V99X8_CONTROL_REGISTER_COLOR_BURST_REGISTER_3             = 22,

    // Display Registers
    V99X8_CONTROL_REGISTER_DISPLAY_ADJUST_REGISTER            = 18,
    V99X8_CONTROL_REGISTER_VERTICAL_OFFSET_REGISTER           = 23,

    V99X8_CONTROL_REGISTER_INTERRUPT_LINE_REGISTER            = 19,

    // Address Registers
    V99X8_CONTROL_REGISTER_VRAM_ACCESS_BASE_REGISTER          = 14,
    V99X8_CONTROL_REGISTER_STATUS_REGISTER_POINTER            = 15,
    V99X8_CONTROL_REGISTER_COLOR_PALETTE_ADDRESS_REGISTER     = 16,
    V99X8_CONTROL_REGISTER_CONTROL_REGISTER_POINTER           = 17,

    // Command Registers
    V99X8_CONTROL_REGISTER_SOURCE_X_LOW_REGISTER              = 32,
    V99X8_CONTROL_REGISTER_SOURCE_X_HIGH_REGISTER             = 33,
    V99X8_CONTROL_REGISTER_SOURCE_Y_LOW_REGISTER              = 34,
    V99X8_CONTROL_REGISTER_SOURCE_Y_HIGH_REGISTER             = 35,

    V99X8_CONTROL_REGISTER_DESTINATION_X_LOW_REGISTER         = 36,
    V99X8_CONTROL_REGISTER_DESTINATION_X_HIGH_REGISTER        = 37,
    V99X8_CONTROL_REGISTER_DESTINATION_Y_LOW_REGISTER         = 38,
    V99X8_CONTROL_REGISTER_DESTINATION_Y_HIGH_REGISTER        = 39,

    V99X8_CONTROL_REGISTER_NUMBER_OF_DOTS_X_LOW_REGISTER      = 40,
    V99X8_CONTROL_REGISTER_NUMBER_OF_DOTS_X_HIGH_REGISTER     = 41,
    V99X8_CONTROL_REGISTER_NUMBER_OF_DOTS_Y_LOW_REGISTER      = 42,
    V99X8_CONTROL_REGISTER_NUMBER_OF_DOTS_Y_HIGH_REGISTER     = 43,

    V99X8_CONTROL_REGISTER_COLOR_REGISTER                     = 44,
    V99X8_CONTROL_REGISTER_ARGUMENT_REGISTER                  = 45,
    V99X8_CONTROL_REGISTER_COMMAND_REGISTER                   = 46,

    // V9958 Registers
    V99X8_CONTROL_REGISTER_MODE_R25                           = 25,
    V99X8_CONTROL_REGISTER_HORIZONTAL_SCROLL_HIGH             = 26,
    V99X8_CONTROL_REGISTER_HORIZONTAL_SCROLL_LOW              = 27,
    
    V99X8_CONTROL_REGISTER_REGISTER_MAX                       = 46,
    V99X8_CONTROL_REGISTER_REGISTER_MASK                      = 0x3F,

    V99X8_CONTROL_REGISTER_REGISTER_SETUP                     = 0x80,
    V99X8_CONTROL_REGISTER_AUTOINCREMENT                      = 0x80,

    V99X8_CONTROL_REGISTER_MASK                               = V99X8_CONTROL_REGISTER_REGISTER_SETUP | V99X8_CONTROL_REGISTER_REGISTER_MASK,
    V99X8_CONTROL_REGISTER_MAX                                = V99X8_CONTROL_REGISTER_REGISTER_SETUP + V99X8_CONTROL_REGISTER_REGISTER_MAX,
};

static inline void v99x8_control_register_write(enum v99x8_control_register cr, uint8_t value) {
    V99X8_R(V99X8_REGISTER_SETUP_W) = value;
    V99X8_R(V99X8_REGISTER_SETUP_W) = V99X8_CONTROL_REGISTER_REGISTER_SETUP | (cr & V99X8_CONTROL_REGISTER_REGISTER_MASK);
}

// Write Mode Registers

enum v99x8_mode_r0 {
    V99X8_MODE_R0_DG    = 1 << 6,
    V99X8_MODE_R0_IE2   = 1 << 5,
    V99X8_MODE_R0_IE1   = 1 << 4,
    V99X8_MODE_R0_M5    = 1 << 3,
    V99X8_MODE_R0_M4    = 1 << 2,
    V99X8_MODE_R0_M3    = 1 << 1,

    V99X8_MODE_R0_MASK  = 0x7E,
    V99X8_MODE_R0_MAX   = 0x7E,
};

enum v99x8_mode_r1 {
    V99X8_MODE_R1_BL    = 1 << 6,
    V99X8_MODE_R1_IE0   = 1 << 5,
    V99X8_MODE_R1_M1    = 1 << 4,
    V99X8_MODE_R1_M2    = 1 << 3,
    V99X8_MODE_R1_SI    = 1 << 1,
    V99X8_MODE_R1_MAG   = 1 << 0,

    V99X8_MODE_R1_MASK  = 0x7B,
    V99X8_MODE_R1_MAX   = 0x7B,
};

enum v99x8_mode_r8 {
    V99X8_MODE_R8_MS    = 1 << 7,
    V99X8_MODE_R8_LP    = 1 << 6,
    V99X8_MODE_R8_TP    = 1 << 5,
    V99X8_MODE_R8_CB    = 1 << 4,
    V99X8_MODE_R8_VR    = 1 << 3,
    V99X8_MODE_R8_SPD   = 1 << 1,
    V99X8_MODE_R8_BW    = 1 << 0,

    V99X8_MODE_R8_MASK  = 0xFB,
    V99X8_MODE_R8_MAX   = 0xFB,
};

enum v99x8_mode_r9 {
    V99X8_MODE_R9_LN    = 1 << 7,
    V99X8_MODE_R9_S1    = 1 << 5,
    V99X8_MODE_R9_S0    = 1 << 4,
    V99X8_MODE_R9_IL    = 1 << 3,
    V99X8_MODE_R9_EO    = 1 << 2,
    V99X8_MODE_R9_NT    = 1 << 1,
    V99X8_MODE_R9_DC    = 1 << 0,

    V99X8_MODE_R9_MASK  = 0xBF,
    V99X8_MODE_R9_MAX   = 0xBF,
};

static inline void v99x8_mode_r0_write(enum v99x8_mode_r0 mode_r0) {
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_MODE_R0, mode_r0);
}

static inline void v99x8_mode_r1_write(enum v99x8_mode_r1 mode_r1) {
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_MODE_R1, mode_r1);
}

static inline void v99x8_mode_r8_write(enum v99x8_mode_r8 mode_r8) {
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_MODE_R8, mode_r8);
}

static inline void v99x8_mode_r9_write(enum v99x8_mode_r9 mode_r9) {
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_MODE_R9, mode_r9);
}

// Write Table Base Address Registers

static inline void v99x8_pattern_layout_table_write(uint8_t a16_a10) {
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_PATTERN_LAYOUT_TABLE, (a16_a10 >> 0) & 0x7F);
}

static inline void v99x8_color_table_write(uint16_t a16_a6) {
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_COLOR_TABLE_LOW, (a16_a6 >> 0) & 0xFF);
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_COLOR_TABLE_HIGH, (a16_a6 >> 8) & 0x07);
}

static inline void v99x8_pattern_generator_table_write(uint8_t a16_a11) {
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_PATTERN_GENERATOR_TABLE, (a16_a11 >> 0) & 0x7F);
}

static inline void v99x8_sprite_attribute_table_write(uint16_t a16_a7) {
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_SPRITE_ATTRIBUTE_TABLE_LOW, (a16_a7 >> 0) & 0xFF);
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_SPRITE_ATTRIBUTE_TABLE_HIGH, (a16_a7 >> 8) & 0x03);
}

static inline void v99x8_sprite_pattern_generator_table_write(uint8_t a16_a11) {
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_SPRITE_PATTERN_GENERATOR_TABLE, (a16_a11 >> 0) & 0x7F);
}

// Write Color Registers

enum v99x8_color_burst {
    V99X8_COLOR_BURST_NONE      = 0x000000,
    V99X8_COLOR_BURST_COMPOSITE = 0x053B00,
    
    V99X8_COLOR_BURST_MASK      = 0xFFFFFF,
    V99X8_COLOR_BURST_MAX       = 0xFFFFFF,
};

static inline void v99x8_text_and_screen_margin_color_write(uint8_t color) {
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_TEXT_AND_SCREEN_MARGIN_COLOR, color);
}

static inline void v99x8_text_and_screen_margin_color_write_split(uint8_t text_color, uint8_t background_color) {
    v99x8_text_and_screen_margin_color_write(((text_color & 0xF) << 4) | ((background_color & 0xF) << 0));
}

static inline void v99x8_text_and_background_blink_color_write(uint8_t color) {
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_TEXT_AND_BACKGROUND_BLINK_COLOR, color);
}

static inline void v99x8_text_and_background_blink_color_write_split(uint8_t text_blink_color, uint8_t background_blink_color) {
    v99x8_text_and_background_blink_color_write(((text_blink_color & 0xF) << 4) | ((background_blink_color & 0xF) << 0));
}

static inline void v99x8_blinking_period_register_write(uint8_t on_even_time, uint8_t off_odd_time) {
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_BLINKING_PERIOD_REGISTER, ((on_even_time & 0xF) << 4) | ((off_odd_time & 0xF) << 0));
}

static inline void v99x8_color_burst_register_write(enum v99x8_color_burst color_burst) {
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_COLOR_BURST_REGISTER_1, (color_burst >> 0) & 0xFF);
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_COLOR_BURST_REGISTER_2, (color_burst >> 8) & 0xFF);
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_COLOR_BURST_REGISTER_3, (color_burst >> 16) & 0xFF);
}

// Write Display Registers

static inline void v99x8_display_adjust_register_write(uint8_t vertical, uint8_t horizontal) {
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_DISPLAY_ADJUST_REGISTER, ((vertical & 0xF) << 4) | ((horizontal & 0xF) << 0));
}

static inline void v99x8_vertical_offset_register_write(uint8_t display_offset) {
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_VERTICAL_OFFSET_REGISTER, display_offset);
}

static inline void v99x8_interrupt_line_register_write(uint8_t interrupt_line) {
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_INTERRUPT_LINE_REGISTER, interrupt_line);
}

// Write Access Registers

enum v99x8_status_register {
    V99X8_STATUS_REGISTER_0                             = 0,
    V99X8_STATUS_REGISTER_1                             = 1,
    V99X8_STATUS_REGISTER_2                             = 2,

    V99X8_STATUS_REGISTER_COLUMN_REGISTER_LOW           = 3,
    V99X8_STATUS_REGISTER_COLUMN_REGISTER_HIGH          = 4,
    V99X8_STATUS_REGISTER_ROW_REGISTER_LOW              = 5,
    V99X8_STATUS_REGISTER_ROW_REGISTER_HIGH             = 6,
    
    V99X8_STATUS_REGISTER_COLOR_REGISTER                = 7,
    
    V99X8_STATUS_REGISTER_CODED_COLOR_X_REGISTER_LOW    = 8,
    V99X8_STATUS_REGISTER_CODED_COLOR_X_REGISTER_HIGH   = 9,

    V99X8_STATUS_REGISTER_MASK                          = 0xF,
    V99X8_STATUS_REGISTER_MAX                           = 9,
};

static inline void v99x8_vram_access_base_register_write(uint8_t a16_a14) {
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_VRAM_ACCESS_BASE_REGISTER, a16_a14 & 0x07);
}

static inline void v99x8_status_register_pointer_write(enum v99x8_status_register status_register) {
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_STATUS_REGISTER_POINTER, status_register & V99X8_STATUS_REGISTER_MASK);
}

static inline void v99x8_color_palette_address_register_write(uint8_t color_palette) {
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_COLOR_PALETTE_ADDRESS_REGISTER, color_palette & 0x0F);
}

static inline void v99x8_control_register_pointer_write(enum v99x8_control_register control_register, bool auto_increment) {
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_CONTROL_REGISTER_POINTER, (auto_increment ? 0 : V99X8_CONTROL_REGISTER_AUTOINCREMENT) | (control_register & V99X8_CONTROL_REGISTER_REGISTER_MASK));
}

// Write Command Registers

enum v99x8_argument {
    V99X8_ARGUMENT_MXC_VRAM     = 0 << 6,
    V99X8_ARGUMENT_MXC_EXPRAM   = 1 << 6,
    V99X8_ARGUMENT_MXC_MASK     = 1 << 6,

    V99X8_ARGUMENT_MXD_VRAM     = 0 << 5,
    V99X8_ARGUMENT_MXD_EXPRAM   = 1 << 5,
    V99X8_ARGUMENT_MXD_MASK     = 1 << 5,

    V99X8_ARGUMENT_MXS_VRAM     = 0 << 4,
    V99X8_ARGUMENT_MXS_EXPRAM   = 1 << 4,
    V99X8_ARGUMENT_MXS_MASK     = 1 << 4,

    V99X8_ARGUMENT_DIY_DOWN     = 0 << 3,
    V99X8_ARGUMENT_DIY_UP       = 1 << 3,
    V99X8_ARGUMENT_DIY_MASK     = 1 << 3,

    V99X8_ARGUMENT_DIX_RIGHT    = 0 << 2,
    V99X8_ARGUMENT_DIX_LEFT     = 1 << 2,
    V99X8_ARGUMENT_DIX_MASK     = 1 << 2,

    V99X8_ARGUMENT_EQ_NEQSTOP   = 0 << 1,
    V99X8_ARGUMENT_EQ_EQSTOP    = 1 << 1,
    V99X8_ARGUMENT_EQ_MASK      = 1 << 1,

    V99X8_ARGUMENT_MAJ_LONG_X   = 0 << 0,
    V99X8_ARGUMENT_MAJ_LONG_Y   = 1 << 0,
    V99X8_ARGUMENT_MAJ_MASK     = 1 << 0,

    V99X8_ARGUMENT_MASK         = 0x7F,
    V99X8_ARGUMENT_MAX          = 0x7F,
};

enum v99x8_command {
    V99X8_COMMAND_HMMC  = 0xF,  // High speed move CPU to VRAM 
    V99X8_COMMAND_YMMM  = 0xE,  // High speed move VRAM to VRAM, Y coordinate only
    V99X8_COMMAND_HMMM  = 0xD,  // High speed move VRAM to VRAM
    V99X8_COMMAND_HMMV  = 0xC,  // High speed move VDP to VRAM
    V99X8_COMMAND_LMMC  = 0xB,  // Logical move CPU to VRAM
    V99X8_COMMAND_LMCM  = 0xA,  // Logical move VRAM to CPU 
    V99X8_COMMAND_LMMM  = 0x9,  // Logical move VRAM to VRAM
    V99X8_COMMAND_LMMV  = 0x8,  // Logical move VDP to VRAM
    V99X8_COMMAND_LINE  = 0x7,
    V99X8_COMMAND_SRCH  = 0x6,
    V99X8_COMMAND_PSET  = 0x5,
    V99X8_COMMAND_POINT = 0x4,
    V99X8_COMMAND_STOP  = 0x0,

    V99X8_COMMAND_MASK    = 0xF,
    V99X8_COMMAND_MAX     = 0xF,
};

enum v99x8_logical_operation {
    V99X8_LOGICAL_OPERATION_IMP     = 0x0,
    V99X8_LOGICAL_OPERATION_AND     = 0x1,
    V99X8_LOGICAL_OPERATION_OR      = 0x2,
    V99X8_LOGICAL_OPERATION_XOR     = 0x3,
    V99X8_LOGICAL_OPERATION_NOT     = 0x4,
    V99X8_LOGICAL_OPERATION_TIMP    = 0x8,
    V99X8_LOGICAL_OPERATION_TAND    = 0x9,
    V99X8_LOGICAL_OPERATION_TOR     = 0xA,
    V99X8_LOGICAL_OPERATION_TXOR    = 0xB,
    V99X8_LOGICAL_OPERATION_TNOT    = 0xD,

    V99X8_LOGICAL_OPERATION_MASK    = 0xF,
    V99X8_LOGICAL_OPERATION_MAX     = 0xD,
};

static inline void v99x8_source_x_register_write(uint16_t source_x) {
    volatile unsigned long tmp = 0;
    tmp += 1;
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_SOURCE_X_LOW_REGISTER, (source_x >> 0) & 0xFF);
    tmp += 1;
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_SOURCE_X_HIGH_REGISTER, (source_x >> 8) & 0x01);
}

static inline void v99x8_source_y_register_write(uint16_t source_y) {
    volatile unsigned long tmp = 0;
    tmp += 1;
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_SOURCE_Y_LOW_REGISTER, (source_y >> 0) & 0xFF);
    tmp += 1;
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_SOURCE_Y_HIGH_REGISTER, (source_y >> 8) & 0x03);
}

static inline void v99x8_destination_x_register_write(uint16_t destination_x) {
    volatile unsigned long tmp = 0;
    tmp += 1;
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_DESTINATION_X_LOW_REGISTER, (destination_x >> 0) & 0xFF);
    tmp += 1;
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_DESTINATION_X_HIGH_REGISTER, (destination_x >> 8) & 0x01);
}

static inline void v99x8_destination_y_register_write(uint16_t destination_y) {
    volatile unsigned long tmp = 0;
    tmp += 1;
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_DESTINATION_Y_LOW_REGISTER, (destination_y >> 0) & 0xFF);
    tmp += 1;
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_DESTINATION_Y_HIGH_REGISTER, (destination_y >> 8) & 0x03);
}

static inline void v99x8_number_of_dots_x_register_write(uint16_t number_of_dots_x) {
    volatile unsigned long tmp = 0;
    tmp += 1;
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_NUMBER_OF_DOTS_X_LOW_REGISTER, (number_of_dots_x >> 0) & 0xFF);
    tmp += 1;
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_NUMBER_OF_DOTS_X_HIGH_REGISTER, (number_of_dots_x >> 8) & 0x01);
}

static inline void v99x8_number_of_dots_y_register_write(uint16_t number_of_dots_y) {
    volatile unsigned long tmp = 0;
    tmp += 1;
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_NUMBER_OF_DOTS_Y_LOW_REGISTER, (number_of_dots_y >> 0) & 0xFF);
    tmp += 1;
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_NUMBER_OF_DOTS_Y_HIGH_REGISTER, (number_of_dots_y >> 8) & 0x03);
}

static inline void v99x8_number_of_dots_long_register_write(uint16_t maj) {
    v99x8_number_of_dots_x_register_write(maj);
}

static inline void v99x8_number_of_dots_short_register_write(uint16_t min) {
    v99x8_number_of_dots_y_register_write(min);
}

static inline void v99x8_color_register_write(uint8_t color) {
    volatile unsigned long tmp = 0;
    tmp += 1;
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_COLOR_REGISTER, color);
}

static inline void v99x8_argument_register_write(enum v99x8_argument argument) {
    volatile unsigned long tmp = 0;
    tmp += 1;
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_ARGUMENT_REGISTER, argument & 0x7F);
}

static inline void v99x8_command_register_write(enum v99x8_command command, enum v99x8_logical_operation logical_operation) {
    volatile unsigned long tmp = 0;
    tmp += 1;
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_COMMAND_REGISTER, ((command & V99X8_COMMAND_MASK) << 4) | ((logical_operation & V99X8_LOGICAL_OPERATION_MASK) << 0));
}

// Write V9958 Registers

enum v99x8_mode_r25 {
    V99X8_MODE_R25_CMD  = 1 << 6,
    V99X8_MODE_R25_VDS  = 1 << 5,
    V99X8_MODE_R25_YAE  = 1 << 4,
    V99X8_MODE_R25_YJK  = 1 << 3,
    V99X8_MODE_R25_WTE  = 1 << 2,
    V99X8_MODE_R25_MSK  = 1 << 1,
    V99X8_MODE_R25_SP2  = 1 << 0,

    V99X8_MODE_R25_MASK = 0x7F,
    V99X8_MODE_R25_MAX  = 0x7F,
};

static inline void v99x8_mode_r25_write(enum v99x8_mode_r25 mode_r25) {
    volatile unsigned long tmp = 0;
    tmp += 1;
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_MODE_R25, mode_r25);
}

static inline void v99x8_horizontal_scroll_high_write(uint8_t chars) {
    volatile unsigned long tmp = 0;
    tmp += 1;
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_HORIZONTAL_SCROLL_HIGH, chars);
}

static inline void v99x8_horizontal_scroll_low_write(uint8_t dots) {
    volatile unsigned long tmp = 0;
    tmp += 1;
    v99x8_control_register_write(V99X8_CONTROL_REGISTER_HORIZONTAL_SCROLL_LOW, dots);
}
/*
static inline void v99x8_horizontal_scroll_write(uint16_t h_scroll) {
    uint16_t rounded_h_scroll = round_up(h_scroll, 8);
    uint8_t chars = rounded_h_scroll / 8;
    uint8_t dots = rounded_h_scroll - h_scroll;
    chars &= 0x3F;
    dots &= 0x07;
    v99x8_horizontal_scroll_high_write(chars);
    v99x8_horizontal_scroll_low_write(dots);
}*/

// Indirect Access to Registers

static inline void v99x8_control_registers_indirect_write_nosetup(size_t values_count, const uint8_t values[values_count]) {
    size_t i;
    for (i = 0; i < values_count; ++i) {
        V99X8_R(V99X8_REGISTER_INDIRECT_ADDRESSING_W) = values[i];
    }
}

static inline void v99x8_control_registers_indirect_write(enum v99x8_control_register cr, bool auto_increment, size_t values_count, const uint8_t values[values_count]) {
    v99x8_control_register_pointer_write(cr, auto_increment);
    v99x8_control_registers_indirect_write_nosetup(values_count, values);
}

// Accessing the Palette Registers

static inline void v99x8_palette_register_write(uint8_t palette, uint16_t data) {
    volatile unsigned long tmp = 0;
    tmp += 1;
    v99x8_color_palette_address_register_write(palette);
    tmp += 1;
    V99X8_R(V99X8_PALETTE_REGISTERS_W) = (data >> 0) & 0x77;
    tmp += 1;
    V99X8_R(V99X8_PALETTE_REGISTERS_W) = (data >> 8) & 0x07;
}

// Accessing the Status Registers

enum v99x8_status_register_0 {
    V99X8_STATUS_REGISTER_0_F           = 1 << 7,
    V99X8_STATUS_REGISTER_0_5S          = 1 << 6,
    V99X8_STATUS_REGISTER_0_C           = 1 << 5,

    V99X8_STATUS_REGISTER_0_5SN_MASK    = 0x1F << 0,
    
    V99X8_STATUS_REGISTER_0_MASK        = 0xFF,
    V99X8_STATUS_REGISTER_0_MAX         = 0xFF,
};

enum v99x8_id {
    V99X8_ID_V9938  = 0,
    V99X8_ID_V9958  = 2,
    
    V99X8_ID_MASK   = 0x1F,
    V99X8_ID_MAX    = 0x1F,
};
#define V99X8_STATUS_REGISTER_1_ID_SHIFT 1
enum v99x8_status_register_1 {
    V99X8_STATUS_REGISTER_1_FL          = 1 << 7,
    V99X8_STATUS_REGISTER_1_LPS         = 1 << 6,

    V99X8_STATUS_REGISTER_1_ID_MASK     = V99X8_ID_MASK << V99X8_STATUS_REGISTER_1_ID_SHIFT,
    
    V99X8_STATUS_REGISTER_1_FH          = 1 << 0,
    
    V99X8_STATUS_REGISTER_1_MASK        = 0xFF,
    V99X8_STATUS_REGISTER_1_MAX         = 0xFF,
};

enum v99x8_status_register_2 {
    V99X8_STATUS_REGISTER_2_TR          = 1 << 7,
    V99X8_STATUS_REGISTER_2_VR          = 1 << 6,
    V99X8_STATUS_REGISTER_2_HR          = 1 << 5,
    V99X8_STATUS_REGISTER_2_BD          = 1 << 4,
    V99X8_STATUS_REGISTER_2_EO          = 1 << 1,
    V99X8_STATUS_REGISTER_2_CE          = 1 << 0,
    
    V99X8_STATUS_REGISTER_2_MASK        = 0xF3,
    V99X8_STATUS_REGISTER_2_MAX         = 0xFF,
};

static inline uint8_t v99x8_status_register_read(enum v99x8_status_register stat_reg) {
    v99x8_status_register_pointer_write(stat_reg);
    return V99X8_R(V99X8_STATUS_REGISTER_R);
}

static inline enum v99x8_status_register_0 v99x8_status_register_0_read(void) {
    return v99x8_status_register_read(V99X8_STATUS_REGISTER_0);
}

static inline enum v99x8_status_register_1 v99x8_status_register_1_read(void) {
    return v99x8_status_register_read(V99X8_STATUS_REGISTER_1);
}

static inline enum v99x8_status_register_2 v99x8_status_register_2_read(void) {
    return v99x8_status_register_read(V99X8_STATUS_REGISTER_2);
}

static inline uint16_t v99x8_column_register_read(void) {
    uint8_t low = v99x8_status_register_read(V99X8_STATUS_REGISTER_COLUMN_REGISTER_LOW) & 0xFF;
    uint8_t high = v99x8_status_register_read(V99X8_STATUS_REGISTER_COLUMN_REGISTER_HIGH) & 0x01;
    return (high << 8) | (low << 0);
}

static inline uint16_t v99x8_row_register_read(void) {
    uint8_t low = v99x8_status_register_read(V99X8_STATUS_REGISTER_ROW_REGISTER_LOW) & 0xFF;
    uint8_t high = v99x8_status_register_read(V99X8_STATUS_REGISTER_ROW_REGISTER_HIGH) & 0x03;
    return (high << 8) | (low << 0);
}

static inline uint8_t v99x8_color_register_read(void) {
    return v99x8_status_register_read(V99X8_STATUS_REGISTER_COLOR_REGISTER) & 0xFF;
}

static inline uint16_t v99x8_coded_color_x_register_read(void) {
    uint8_t low = v99x8_status_register_read(V99X8_STATUS_REGISTER_CODED_COLOR_X_REGISTER_LOW) & 0xFF;
    uint8_t high = v99x8_status_register_read(V99X8_STATUS_REGISTER_CODED_COLOR_X_REGISTER_HIGH) & 0x01;
    return (high << 8) | (low << 0);
}

// Accessing the Video RAM

enum v99x8_vram_operation_mode {
    V99X8_VRAM_OPERATION_MODE_DATA_READ     = 0x00,
    V99X8_VRAM_OPERATION_MODE_DATA_WRITE    = 0x40,
    
    V99X8_VRAM_OPERATION_MODE_MASK          = 0x40,
    V99X8_VRAM_OPERATION_MODE_MAX           = 0x40,
};

static inline void v99x8_set_ram_bank(enum v99x8_argument bank) {
    v99x8_argument_register_write(bank & V99X8_ARGUMENT_MXC_MASK);
}

static inline void v99x8_set_address_counter_a13_to_a0_and_operation_mode(uint16_t a13_a0, enum v99x8_vram_operation_mode operation_mode) {
    uint16_t a7_a0 = (a13_a0 >> 0) & 0xFF;
    uint16_t a13_a8 = (a13_a0 >> 8) & 0x3F;

    V99X8_R(V99X8_VRAM_ADDRESS_W) = a7_a0;
    V99X8_R(V99X8_VRAM_ADDRESS_W) = (operation_mode & V99X8_VRAM_OPERATION_MODE_MASK) | (a13_a8 & 0x3F);
}

static inline void v99x8_set_address_counter_and_operation_mode(bool use_expram, uint32_t address, enum v99x8_vram_operation_mode operation_mode) {
    v99x8_set_ram_bank(use_expram ? V99X8_ARGUMENT_MXC_EXPRAM : V99X8_ARGUMENT_MXC_VRAM);
    v99x8_vram_access_base_register_write((address >> 14) & 0x07);
    v99x8_set_address_counter_a13_to_a0_and_operation_mode((address >> 0) & 0x3FFF, operation_mode);
}

static inline void v99x8_vram_wait(void) {
    volatile unsigned long long tmp = 0;
    tmp += 1;
    tmp += 1;
}

static inline void v99x8_vram_read_nosetup(size_t data_count, uint8_t data[data_count]) {
    size_t i;
    for (i = 0; i < data_count; ++i) {
        v99x8_vram_wait();
        data[i] = V99X8_R(V99X8_VRAM_DATA_RW);
    }
}

static inline void v99x8_vram_write_nosetup(size_t data_count, const uint8_t data[data_count]) {
    size_t i;
    for (i = 0; i < data_count; ++i) {
        v99x8_vram_wait();
        V99X8_R(V99X8_VRAM_DATA_RW) = data[i];
    }
}

static inline void v99x8_vram_read(bool use_expram, uint32_t address, size_t data_count, uint8_t data[data_count]) {
    v99x8_set_address_counter_and_operation_mode(use_expram, address, V99X8_VRAM_OPERATION_MODE_DATA_READ);
    v99x8_vram_read_nosetup(data_count, data);
}

static inline void v99x8_vram_write(bool use_expram, uint32_t address, size_t data_count, const uint8_t data[data_count]) {
    v99x8_set_address_counter_and_operation_mode(use_expram, address, V99X8_VRAM_OPERATION_MODE_DATA_WRITE);
    v99x8_vram_write_nosetup(data_count, data);
}

static inline void v99x8_wait_ce(void) {
    while (v99x8_status_register_2_read() & V99X8_STATUS_REGISTER_2_CE) {
        // Keep checking
    }
}

void v99x8_hmmc(uint16_t dx, uint16_t dy, uint16_t nx, uint16_t ny, enum v99x8_argument arg, const uint8_t *data);
void v99x8_ymmm(uint16_t sx, uint16_t dx, uint16_t dy, uint16_t ny, enum v99x8_argument arg);
void v99x8_hmmm(uint16_t sx, uint16_t sy, uint16_t dx, uint16_t dy, uint16_t nx, uint16_t ny, enum v99x8_argument arg);
void v99x8_hmmv(uint16_t dx, uint16_t dy, uint16_t nx, uint16_t ny, uint8_t color, enum v99x8_argument arg);
void v99x8_lmmc(uint16_t dx, uint16_t dy, uint16_t nx, uint16_t ny, enum v99x8_argument arg, enum v99x8_logical_operation lo, const uint8_t *data);
void v99x8_lmcm(uint16_t sx, uint16_t sy, uint16_t nx, uint16_t ny, enum v99x8_argument arg, uint8_t *data);
void v99x8_lmmm(uint16_t sx, uint16_t sy, uint16_t dx, uint16_t dy, uint16_t nx, uint16_t ny, enum v99x8_argument arg, enum v99x8_logical_operation lo);
void v99x8_lmmv(uint16_t dx, uint16_t dy, uint16_t nx, uint16_t ny, uint8_t color, enum v99x8_argument arg, enum v99x8_logical_operation lo);
void v99x8_line(uint16_t dx, uint16_t dy, uint16_t maj, uint16_t min, uint8_t color, enum v99x8_argument arg, enum v99x8_logical_operation lo);
int16_t v99x8_status_srch(uint16_t sx, uint16_t sy, uint8_t color, enum v99x8_argument arg, enum v99x8_logical_operation lo);   // Returns -1 on failure to find color
void v99x8_pset(uint16_t dx, uint16_t dy, uint8_t color, enum v99x8_argument arg, enum v99x8_logical_operation lo);
uint8_t v99x8_point(uint16_t sx, uint16_t sy, enum v99x8_argument arg, enum v99x8_logical_operation lo);
void v99x8_stop(void);

static inline void v99x8_hmmc_ext(uint32_t d, uint32_t n, enum v99x8_argument arg, const uint8_t *data) {
    v99x8_hmmc(
        d % 256, d / 256,
        n % 256, n / 256,
        arg, data
    );
}

static inline void v99x8_ymmm_ext(uint16_t sx, uint32_t d, uint32_t ny, enum v99x8_argument arg) {
    v99x8_ymmm(
        sx,
        d % 256, d / 256,
        ny,
        arg
    );
}

static inline void v99x8_hmmm_ext(uint32_t s, uint32_t d, uint32_t n, enum v99x8_argument arg) {
    v99x8_hmmm(
        s % 256, s / 256,
        d % 256, d / 256,
        n % 256, n / 256,
        arg
    );
}

static inline void v99x8_hmmv_ext(uint32_t d, uint32_t n, uint8_t color, enum v99x8_argument arg) {
    v99x8_hmmv(
        d % 256, d / 256,
        n % 256, n / 256,
        color, arg
    );
}

static inline void v99x8_lmmc_ext(uint32_t d, uint32_t n, enum v99x8_argument arg, enum v99x8_logical_operation lo, const uint8_t *data) {
    v99x8_lmmc(
        d % 256, d / 256,
        n % 256, n / 256,
        arg, lo, data
    );
}

static inline void v99x8_lmcm_ext(uint32_t s, uint32_t n, enum v99x8_argument arg, uint8_t *data) {
    v99x8_lmcm(
        s % 256, s / 256,
        n % 256, n / 256,
        arg, data
    );
}

static inline void v99x8_lmmm_ext(uint32_t s, uint32_t d, uint32_t n, enum v99x8_argument arg, enum v99x8_logical_operation lo) {
    v99x8_lmmm(
        s % 256, s / 256,
        d % 256, d / 256,
        n % 256, n / 256,
        arg, lo
    );
}

static inline void v99x8_lmmv_ext(uint32_t d, uint32_t n, uint8_t color, enum v99x8_argument arg, enum v99x8_logical_operation lo) {
    v99x8_lmmv(
        d % 256, d / 256,
        n % 256, n / 256,
        color, arg, lo
    );
}

static inline int16_t v99x8_status_srch_ext(uint32_t s, uint8_t color, enum v99x8_argument arg, enum v99x8_logical_operation lo) {
    return v99x8_status_srch(
        s % 256, s / 256,
        color, arg, lo
    );
}

static inline void v99x8_pset_ext(uint32_t d, uint8_t color, enum v99x8_argument arg, enum v99x8_logical_operation lo) {
    v99x8_pset(
        d % 256, d / 256,
        color, arg, lo
    );
}

static inline uint8_t v99x8_point_ex(uint32_t s, enum v99x8_argument arg, enum v99x8_logical_operation lo) {
    return v99x8_point(
        s % 256, s / 256,
        arg, lo
    );
}

// Sprite Manipulation

static inline void v99x8_sm2_sprite_attribute_write(uint32_t sprite_attribute_table, uint8_t sprite, uint8_t y_coordinate, uint8_t x_coordinate, uint8_t pattern_number) {
    uint8_t buffer[3] = {
        y_coordinate,
        x_coordinate,
        pattern_number
    };
    uint32_t vram_address = sprite_attribute_table + (uint32_t) sprite * 4;
    v99x8_vram_write(false, vram_address, 3, buffer);
}

static inline void v99x8_sm2_sprite_pattern_generator_write(uint32_t sprite_pattern_generator_table, uint8_t pattern_number, uint8_t bitmap[8]) {
    uint32_t vram_address = sprite_pattern_generator_table + (uint32_t) pattern_number * 8;
    v99x8_vram_write(false, vram_address, 8, bitmap);
}

static inline void v99x8_sm2_sprite_pattern_generator_write_4(uint32_t sprite_pattern_generator_table, uint8_t pattern_number, uint8_t bitmap[4][8]) {
    pattern_number &= ~0x03;
    uint32_t vram_address = sprite_pattern_generator_table + (uint32_t) pattern_number * 8;
    v99x8_vram_write(false, vram_address, 32, (uint8_t *) bitmap);
}

static inline void v99x8_sm2_sprite_color_write(uint32_t sprite_attribute_table, uint8_t sprite, bool size_16, uint8_t colors[size_16 ? 16 : 8]) {
    uint32_t vram_address = sprite_attribute_table - 0x200 + (uint32_t) sprite * 16;
    v99x8_vram_write(false, vram_address, size_16 ? 16 : 8, colors);
}

// Color conversion

// TODO: Add YJK
/*
static inline uint8_t v99x8_rgb3x8_to_grb8(uint8_t r, uint8_t g, uint8_t b) {
    uint8_t r_3 = round_down(r, 0x20) >> 5 & 0x7;
    uint8_t g_3 = round_down(g, 0x20) >> 5 & 0x7;
    uint8_t b_2 = round_down(b, 0x40) >> 6 & 0x3;

    return g_3 << 5 | r_3 << 2 | b_2 << 0;
}

static inline uint16_t v99x8_rgb3x8_to_grb16(uint8_t r, uint8_t g, uint8_t b) {
    uint8_t r_3 = round_down(r, 0x20) >> 5 & 0x7;
    uint8_t g_3 = round_down(g, 0x20) >> 5 & 0x7;
    uint8_t b_3 = round_down(b, 0x20) >> 5 & 0x7;

    return g_3 << 8 | r_3 << 4 | b_3 << 0;
}
*/
static inline uint8_t v99x8_grb16_to_r8(uint16_t a) {
    uint8_t r_3 = (a >> 4) & 0x7;

    return r_3 << 5 | r_3 << 2 | r_3 >> 1;
}

static inline uint8_t v99x8_grb16_to_g8(uint16_t a) {
    uint8_t g_3 = (a >> 8) & 0x7;

    return g_3 << 5 | g_3 << 2 | g_3 >> 1;
}

static inline uint8_t v99x8_grb16_to_b8(uint16_t a) {
    uint8_t b_3 = (a >> 0) & 0x7;

    return b_3 << 5 | b_3 << 2 | b_3 >> 1;
}

static inline uint8_t v99x8_grb8_to_r8(uint8_t a) {
    uint8_t r_3 = (a >> 2) & 0x7;

    return r_3 << 5 | r_3 << 2 | r_3 >> 1;
}

static inline uint8_t v99x8_grb8_to_g8(uint8_t a) {
    uint8_t g_3 = (a >> 5) & 0x7;

    return g_3 << 5 | g_3 << 2 | g_3 >> 1;
}

static inline uint8_t v99x8_grb8_to_b8(uint8_t a) {
    uint8_t b_2 = (a >> 0) & 0x3;

    return b_2 << 6 | b_2 << 4 | b_2 << 2 | b_2 << 0;
}

static inline uint8_t v99x8_grb16_to_grb8(uint16_t a) {
    uint8_t r_3 = (a >> 4) & 0x7;
    uint8_t g_3 = (a >> 8) & 0x7;
    uint8_t b_3 = (a >> 0) & 0x7;
    uint8_t b_2 = (b_3 + 0x1) >> 1 & 0x3;

    return g_3 << 5 | r_3 << 2 | b_2 << 0;
}

static inline uint16_t v99x8_grb8_to_grb16(uint8_t a) {
    uint8_t r_3 = (a >> 2) & 0x7;
    uint8_t g_3 = (a >> 5) & 0x7;
    uint8_t b_2 = (a >> 0) & 0x3;
    uint8_t b_3 = (b_2 << 1) | (b_2 >> 1);

    return g_3 << 8 | r_3 << 4 | b_3 << 0;
}

static inline uint32_t v99x8_rgb3x8_distance_squared(uint8_t a_r, uint8_t a_g, uint8_t a_b, uint8_t b_r, uint8_t b_g, uint8_t b_b) {
    int16_t d_r = b_r - a_r;
    int16_t d_g = b_g - a_g;
    int16_t d_b = b_b - a_b;

    uint32_t d_r_2 = d_r * d_r;
    uint32_t d_g_2 = d_g * d_g;
    uint32_t d_b_2 = d_b * d_b;

    return d_r_2 + d_g_2 + d_b_2;
}

static inline uint32_t v99x8_grb8_distance_squared(uint8_t a, uint8_t b) {
    uint8_t a_r = v99x8_grb8_to_r8(a);
    uint8_t a_g = v99x8_grb8_to_g8(a);
    uint8_t a_b = v99x8_grb8_to_b8(a);
    uint8_t b_r = v99x8_grb8_to_r8(b);
    uint8_t b_g = v99x8_grb8_to_g8(b);
    uint8_t b_b = v99x8_grb8_to_b8(b);

    return v99x8_rgb3x8_distance_squared(a_r, a_g, a_b, b_r, b_g, b_b);
}

static inline uint32_t v99x8_grb16_distance_squared(uint16_t a, uint16_t b) {
    uint8_t a_r = v99x8_grb16_to_r8(a);
    uint8_t a_g = v99x8_grb16_to_g8(a);
    uint8_t a_b = v99x8_grb16_to_b8(a);
    uint8_t b_r = v99x8_grb16_to_r8(b);
    uint8_t b_g = v99x8_grb16_to_g8(b);
    uint8_t b_b = v99x8_grb16_to_b8(b);

    return v99x8_rgb3x8_distance_squared(a_r, a_g, a_b, b_r, b_g, b_b);
}

static inline uint16_t v99x8_enc4_to_grb16(uint8_t c) {
    uint8_t r_3;
    uint8_t g_3;
    uint8_t b_3;

    c &= 0xF;

    if (c == 0x8) {
        r_3 = 0x7;
        g_3 = 0x4;
        b_3 = 0x2;
    } else {
        bool c_i = c & 0x8;
        bool c_g = c & 0x4;
        bool c_r = c & 0x2;
        bool c_b = c & 0x1;

        r_3 = (c_i & c_r) << 2 | (c_r) << 1 | (c_r) << 0;
        g_3 = (c_i & c_g) << 2 | (c_g) << 1 | (c_g) << 0;
        b_3 = (c_i & c_b) << 2 | (c_b) << 1 | (c_i & c_b) << 0;
    }

    return g_3 << 8 | r_3 << 4 | b_3 << 0;
}

static inline uint8_t v99x8_enc4_to_grb8(uint8_t c) {
    return v99x8_grb16_to_grb8(v99x8_enc4_to_grb16(c));
}

#endif
