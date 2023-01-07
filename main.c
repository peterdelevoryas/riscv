#include <stdint.h>
#include <stddef.h>

#define MASK8(shift, length)                                                \
    ((0xff >> (8 - length)) << shift)

#define REG(reg, addr)                                                      \
    enum {                                                                  \
        reg ## _ADDR = (addr),                                              \
    }

#define FIELD(reg, field, shift, length)                                    \
    enum {                                                                  \
        reg ## _ ## field ## _SHIFT = (shift),                              \
        reg ## _ ## field ## _LENGTH = (length),                            \
        reg ## _ ## field ## _MASK8 = MASK8(shift, length),                 \
    }

REG(UART0_RHR, 0x10000000);
REG(UART0_THR, 0x10000000);
REG(UART0_ISR, 0x10000002);
REG(UART0_FCR, 0x10000002);
REG(UART0_LSR, 0x10000005);
FIELD(UART0_RHR, RX_CHAR, 0, 8);
FIELD(UART0_THR, TX_CHAR, 0, 8);
FIELD(UART0_FCR, FIFO_ENABLE, 0, 1);
FIELD(UART0_LSR, DATA_READY, 0, 1);
FIELD(UART0_LSR, THR_EMPTY, 5, 1);
FIELD(UART0_LSR, TRANSMITTER_EMPTY, 6, 1);

#define lb(reg, field) ({                                                   \
    uint8_t r = *((volatile uint8_t *)reg ## _ADDR);                        \
    uint8_t mask = reg ## _ ## field ## _MASK8;                             \
    uint8_t shift = reg ## _ ## field ## _SHIFT;                            \
    r &= mask;                                                              \
    r >>= shift;                                                            \
    r;                                                                      \
})

#define sb(reg, field, data) ({                                             \
    volatile uint8_t *ptr = (volatile uint8_t *)reg ## _ADDR;               \
    uint8_t mask = reg ## _ ## field ## _MASK8;                             \
    uint8_t shift = reg ## _ ## field ## _SHIFT;                            \
    uint8_t r = *ptr;                                                       \
    r &= ~mask;                                                             \
    r |= data << shift;                                                     \
    *ptr = r;                                                               \
})

void putchar(char c)
{
    while (!lb(UART0_LSR, THR_EMPTY)) {
        // wait
    }
    sb(UART0_THR, TX_CHAR, c);

    if (c == '\r') {
        putchar('\n');
    }
}

char getchar(void)
{
    while (!lb(UART0_LSR, DATA_READY)) {
        // wait
    }
    return lb(UART0_RHR, RX_CHAR);
}

void print(const char *s)
{
    while (*s) {
        putchar(*s);
        s++;
    }
}

void main(void)
{
    sb(UART0_FCR, FIFO_ENABLE, 1);

    print("hello world\n");

    for (;;) {
        putchar(getchar());
    }
}
