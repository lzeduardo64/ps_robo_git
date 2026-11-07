#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/vector.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

static volatile uint64_t ticks = 0;

static void delay_ms(uint64_t ms) {
  uint64_t start = ticks;
  while (ticks < ms + start) {
    __asm__("nop");
  }
}

void sys_tick_handler(void) { ticks++; }

int main(void) {

  rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);

  systick_set_frequency(1000, rcc_ahb_frequency);
  systick_counter_enable();
  systick_interrupt_enable();

  rcc_periph_clock_enable(RCC_GPIOC);
  gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                GPIO13);
  gpio_set(GPIOC, GPIO13);

  while (1) {
    gpio_toggle(GPIOC, GPIO13);
    delay_ms(1000);
  }
  return 0;
}
