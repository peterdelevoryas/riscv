#include <stdint.h>
#include <stddef.h>

struct Uart {
    union {
        uint8_t rhr;
        uint8_t thr;
    };
    uint8_t ier;
    union {
        uint8_t isr;
        struct {
            uint8_t fifo_enable : 1;
            uint8_t rx_fifo_reset : 1;
            uint8_t tx_fifo_reset : 1;
            uint8_t dma_mode : 1;
            uint8_t enable_dma_end : 1;
            uint8_t _0 : 1;
            uint8_t rx_fifo_trigger_level : 2;
        } fcr;
    };
    uint8_t lcr;
    uint8_t mcr;
    struct {
        uint8_t data_ready : 1;
        uint8_t overrun_error : 1;
        uint8_t parity_error : 1;
        uint8_t framing_error : 1;
        uint8_t break_interrupt : 1;
        uint8_t thr_empty : 1;
        uint8_t transmitter_empty : 1;
        uint8_t fifo_data_error : 1;
    } lsr;
    uint8_t msr;
    uint8_t spr;
};

volatile struct Uart *UART = (volatile struct Uart *)0x10000000;

void putchar(char c)
{
    while (!UART[0].lsr.thr_empty) {
        // wait
    }
    UART[0].thr = c;

    if (c == '\r') {
        putchar('\n');
    }
}

char getchar(void)
{
    while (!UART[0].lsr.data_ready) {
        // wait
    }
    return UART[0].rhr;
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
    UART[0].fcr.fifo_enable = 1;

    print("hello world\n");

    for (;;) {
        putchar(getchar());
    }
}
