#include "v99x8.h"
short colours[] = {
1000,1000,1000,     0,   0,   0,  1000,   1,   0,     0,1000,   0,  
   0,   0,1000,     0,1000,1000,  1000,1000,   0,  1000,   0,1000,  
 733, 733, 733,   533, 533, 533,   667,   0,   0,     0, 667,   0,  
   0,   0, 667,     0, 667, 667,   667, 667,   0,   667,   0, 667,  
1000,1000,1000,   933, 933, 933,   867, 867, 867,   800, 800, 800,  
 733, 733, 733,   667, 667, 667,   600, 600, 600,   533, 533, 533,  
 467, 467, 467,   400, 400, 400,   333, 333, 333,   267, 267, 267,  
 200, 200, 200,   133, 133, 133,    67,  67,  67,     0,   0,   0,  
1000,   0,   0,  1000,   0,  67,  1000,   0, 133,  1000,   0, 200,  
1000,   0, 267,  1000,   0, 333,  1000,   0, 400,  1000,   0, 467,  
1000,   0, 533,  1000,   0, 600,  1000,   0, 667,  1000,   0, 733,  
1000,   0, 800,  1000,   0, 867,  1000,   0, 933,  1000,   0,1000,  
 933,   0,1000,   867,   0,1000,   800,   0,1000,   733,   0,1000,  
 667,   0,1000,   600,   0,1000,   533,   0,1000,   467,   0,1000,  
 400,   0,1000,   333,   0,1000,   267,   0,1000,   200,   0,1000,  
 133,   0,1000,    67,   0,1000,     0,   0,1000,     0,  67,1000,  
   0, 133,1000,     0, 200,1000,     0, 267,1000,     0, 333,1000,  
   0, 400,1000,     0, 467,1000,     0, 533,1000,     0, 600,1000,  
   0, 667,1000,     0, 733,1000,     0, 800,1000,     0, 867,1000,  
   0, 933,1000,     0,1000,1000,     0,1000, 933,     0,1000, 867,  
   0,1000, 800,     0,1000, 733,     0,1000, 667,     0,1000, 600,  
   0,1000, 533,     0,1000, 467,     0,1000, 400,     0,1000, 333,  
   0,1000, 267,     0,1000, 200,     0,1000, 133,     0,1000,  67,  
   0,1000,   0,    67,1000,   0,   133,1000,   0,   200,1000,   0,  
 267,1000,   0,   333,1000,   0,   400,1000,   0,   467,1000,   0,  
 533,1000,   0,   600,1000,   0,   667,1000,   0,   733,1000,   0,  
 800,1000,   0,   867,1000,   0,   933,1000,   0,  1000,1000,   0,  
1000, 933,   0,  1000, 867,   0,  1000, 800,   0,  1000, 733,   0,  
1000, 667,   0,  1000, 600,   0,  1000, 533,   0,  1000, 467,   0,  
1000, 400,   0,  1000, 333,   0,  1000, 267,   0,  1000, 200,   0,  
1000, 133,   0,  1000,  67,   0,   733,   0,   0,   733,   0,  67,  
 733,   0, 133,   733,   0, 200,   733,   0, 267,   733,   0, 333,  
 733,   0, 400,   733,   0, 467,   733,   0, 533,   733,   0, 600,  
 733,   0, 667,   733,   0, 733,   667,   0, 733,   600,   0, 733,  
 533,   0, 733,   467,   0, 733,   400,   0, 733,   333,   0, 733,  
 267,   0, 733,   200,   0, 733,   133,   0, 733,    67,   0, 733,  
   0,   0, 733,     0,  67, 733,     0, 133, 733,     0, 200, 733,  
   0, 267, 733,     0, 333, 733,     0, 400, 733,     0, 467, 733,  
   0, 533, 733,     0, 600, 733,     0, 667, 733,     0, 733, 733,  
   0, 733, 667,     0, 733, 600,     0, 733, 533,     0, 733, 467,  
   0, 733, 400,     0, 733, 333,     0, 733, 267,     0, 733, 200,  
   0, 733, 133,     0, 733,  67,     0, 733,   0,    67, 733,   0,  
 133, 733,   0,   200, 733,   0,   267, 733,   0,   333, 733,   0,  
 400, 733,   0,   467, 733,   0,   533, 733,   0,   600, 733,   0,  
 667, 733,   0,   733, 733,   0,   733, 667,   0,   733, 600,   0,  
 733, 533,   0,   733, 467,   0,   733, 400,   0,   733, 333,   0,  
 733, 267,   0,   733, 200,   0,   733, 133,   0,   733,  67,   0,  
 467,   0,   0,   467,   0,  67,   467,   0, 133,   467,   0, 200,  
 467,   0, 267,   467,   0, 333,   467,   0, 400,   467,   0, 467,  
 400,   0, 467,   333,   0, 467,   267,   0, 467,   200,   0, 467,  
 133,   0, 467,    67,   0, 467,     0,   0, 467,     0,  67, 467,  
   0, 133, 467,     0, 200, 467,     0, 267, 467,     0, 333, 467,  
   0, 400, 467,     0, 467, 467,     0, 467, 400,     0, 467, 333,  
   0, 467, 267,     0, 467, 200,     0, 467, 133,     0, 467,  67,  
   0, 467,   0,    67, 467,   0,   133, 467,   0,   200, 467,   0,  
 267, 467,   0,   333, 467,   0,   400, 467,   0,   467, 467,   0,  
 467, 400,   0,   467, 333,   0,   467, 267,   0,   467, 200,   0,  
 467, 133,   0,   467,  67,   0,   267,   0,   0,   267,   0,  67,  
 267,   0, 133,   267,   0, 200,   267,   0, 267,   200,   0, 267,  
 133,   0, 267,    67,   0, 267,     0,   0, 267,     0,  67, 267,  
   0, 133, 267,     0, 200, 267,     0, 267, 267,     0, 267, 200,  
   0, 267, 133,     0, 267,  67,     0, 267,   0,    67, 267,   0,  
 133, 267,   0,   200, 267,   0,   267, 267,   0,   267, 200,   0,  
 267, 133,   0,   267,  67,   0,  1000,1000,1000,     0,   0,   0};
void v99x8_hmmc(uint16_t dx, uint16_t dy, uint16_t nx, uint16_t ny, enum v99x8_argument arg, const uint8_t *data) {
    v99x8_wait_ce();

    v99x8_destination_x_register_write(dx);
    v99x8_destination_y_register_write(dy);
    v99x8_number_of_dots_x_register_write(nx);
    v99x8_number_of_dots_y_register_write(ny);

    v99x8_color_register_write(*(data++));

    v99x8_argument_register_write(arg);

    v99x8_command_register_write(V99X8_COMMAND_HMMC, 0);

    uint8_t reg_2;
    while ((reg_2 = v99x8_status_register_2_read()) & V99X8_STATUS_REGISTER_2_CE) {
        if (reg_2 & V99X8_STATUS_REGISTER_2_TR) {
            v99x8_color_register_write(*(data++));
        }
    }
}

void v99x8_ymmm(uint16_t sx, uint16_t dx, uint16_t dy, uint16_t ny, enum v99x8_argument arg) {
    v99x8_wait_ce();

    v99x8_source_x_register_write(sx);
    v99x8_destination_x_register_write(dx);
    v99x8_destination_y_register_write(dy);
    v99x8_number_of_dots_y_register_write(ny);

    v99x8_argument_register_write(arg);

    v99x8_command_register_write(V99X8_COMMAND_YMMM, 0);
}

void v99x8_hmmm(uint16_t sx, uint16_t sy, uint16_t dx, uint16_t dy, uint16_t nx, uint16_t ny, enum v99x8_argument arg) {
    v99x8_wait_ce();

    v99x8_source_x_register_write(sx);
    v99x8_source_y_register_write(sy);
    v99x8_destination_x_register_write(dx);
    v99x8_destination_y_register_write(dy);
    v99x8_number_of_dots_x_register_write(nx);
    v99x8_number_of_dots_y_register_write(ny);

    v99x8_argument_register_write(arg);

    v99x8_command_register_write(V99X8_COMMAND_HMMM, 0);
}

void v99x8_hmmv(uint16_t dx, uint16_t dy, uint16_t nx, uint16_t ny, uint8_t color, enum v99x8_argument arg) {
    v99x8_wait_ce();

    v99x8_destination_x_register_write(dx);
    v99x8_destination_y_register_write(dy);
    v99x8_number_of_dots_x_register_write(nx);
    v99x8_number_of_dots_y_register_write(ny);

    v99x8_color_register_write(color);

    v99x8_argument_register_write(arg);

    v99x8_command_register_write(V99X8_COMMAND_HMMV, 0);
}

void v99x8_lmmc(uint16_t dx, uint16_t dy, uint16_t nx, uint16_t ny, enum v99x8_argument arg, enum v99x8_logical_operation lo, const uint8_t *data) {
    v99x8_wait_ce();

    v99x8_destination_x_register_write(dx);
    v99x8_destination_y_register_write(dy);
    v99x8_number_of_dots_x_register_write(nx);
    v99x8_number_of_dots_y_register_write(ny);

    v99x8_color_register_write(*(data++));

    v99x8_argument_register_write(arg);

    v99x8_command_register_write(V99X8_COMMAND_LMMC, lo);

    uint8_t reg_2;
    while ((reg_2 = v99x8_status_register_2_read()) & V99X8_STATUS_REGISTER_2_CE) {
        if (reg_2 & V99X8_STATUS_REGISTER_2_TR) {
            v99x8_color_register_write(*(data++));
        }
    }
}


void v99x8_lmcm(uint16_t sx, uint16_t sy, uint16_t nx, uint16_t ny, enum v99x8_argument arg, uint8_t *data) {
    v99x8_wait_ce();

    v99x8_source_x_register_write(sx);
    v99x8_source_y_register_write(sy);
    v99x8_number_of_dots_x_register_write(nx);
    v99x8_number_of_dots_y_register_write(ny);

    v99x8_argument_register_write(arg);

    v99x8_command_register_write(V99X8_COMMAND_LMCM, 0);

    uint8_t reg_2;
    do {
        reg_2 = v99x8_status_register_2_read();
        if (reg_2 & V99X8_STATUS_REGISTER_2_TR) {
            *(data++) = v99x8_color_register_read();
        }
    } while (reg_2 & V99X8_STATUS_REGISTER_2_CE);
}

void v99x8_lmmm(uint16_t sx, uint16_t sy, uint16_t dx, uint16_t dy, uint16_t nx, uint16_t ny, enum v99x8_argument arg, enum v99x8_logical_operation lo) {
    v99x8_wait_ce();

    v99x8_source_x_register_write(sx);
    v99x8_source_y_register_write(sy);
    v99x8_destination_x_register_write(dx);
    v99x8_destination_y_register_write(dy);
    v99x8_number_of_dots_x_register_write(nx);
    v99x8_number_of_dots_y_register_write(ny);

    v99x8_argument_register_write(arg);

    v99x8_command_register_write(V99X8_COMMAND_LMMM, lo);
}

void v99x8_lmmv(uint16_t dx, uint16_t dy, uint16_t nx, uint16_t ny, uint8_t color, enum v99x8_argument arg, enum v99x8_logical_operation lo) {
    v99x8_wait_ce();

    v99x8_destination_x_register_write(dx);
    v99x8_destination_y_register_write(dy);
    v99x8_number_of_dots_x_register_write(nx);
    v99x8_number_of_dots_y_register_write(ny);

    v99x8_color_register_write(color);

    v99x8_argument_register_write(arg);

    v99x8_command_register_write(V99X8_COMMAND_LMMV, lo);
}

void v99x8_line(uint16_t dx, uint16_t dy, uint16_t maj, uint16_t min, uint8_t color, enum v99x8_argument arg, enum v99x8_logical_operation lo) {
    v99x8_wait_ce();

    v99x8_destination_x_register_write(dx);
    v99x8_destination_y_register_write(dy);
    v99x8_number_of_dots_long_register_write(maj);
    v99x8_number_of_dots_short_register_write(min);

    v99x8_color_register_write(color);

    v99x8_argument_register_write(arg);

    v99x8_command_register_write(V99X8_COMMAND_LINE, lo);
}

// Returns -1 on failure to find color
int16_t v99x8_status_srch(uint16_t sx, uint16_t sy, uint8_t color, enum v99x8_argument arg, enum v99x8_logical_operation lo) {
    v99x8_wait_ce();

    v99x8_source_x_register_write(sx);
    v99x8_source_y_register_write(sy);

    v99x8_color_register_write(color);

    v99x8_argument_register_write(arg);

    v99x8_command_register_write(V99X8_COMMAND_SRCH, lo);

    v99x8_wait_ce();

    if (v99x8_status_register_2_read() & V99X8_STATUS_REGISTER_2_BD) {
        return v99x8_coded_color_x_register_read();
    } else {
        return -1;
    }
}

void v99x8_pset(uint16_t dx, uint16_t dy, uint8_t color, enum v99x8_argument arg, enum v99x8_logical_operation lo) {
    v99x8_wait_ce();

    v99x8_destination_x_register_write(dx);
    v99x8_destination_y_register_write(dy);

    v99x8_color_register_write(color);

    v99x8_argument_register_write(arg);

    v99x8_command_register_write(V99X8_COMMAND_PSET, lo);
}

uint8_t v99x8_point(uint16_t sx, uint16_t sy, enum v99x8_argument arg, enum v99x8_logical_operation lo) {
    v99x8_wait_ce();

    v99x8_source_x_register_write(sx);
    v99x8_source_y_register_write(sy);

    v99x8_argument_register_write(arg);

    v99x8_command_register_write(V99X8_COMMAND_POINT, lo);

    v99x8_wait_ce();

    return v99x8_color_register_read();
}

void v99x8_stop(void) {
    v99x8_command_register_write(V99X8_COMMAND_STOP, 0);
}
