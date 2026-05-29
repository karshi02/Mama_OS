#ifndef PIC_H
#define PIC_H

#include <stdint.h>

#define PIC1_CMD     0x20
#define PIC1_DATA    (PIC1_CMD+1)
#define PIC2_CMD     0xA0
#define PIC2_DATA    (PIC2_CMD+1)

#define ICW1_INIT    0x11
#define ICW4_8086    0x01

void remap_pic(void);
void enable_keyboard(void);
void disable_keyboard(void);

#endif
