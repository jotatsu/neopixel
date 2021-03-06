/*
 * Copyright (c) 2014-2018 Cesanta Software Limited
 * All rights reserved
 */

#include <stdlib.h>
#include <stdint.h>

#include "mgos_neopixel.h"

#include "common/cs_dbg.h"
#include "mgos_bitbang.h"
#include "mgos_gpio.h"
#include "mgos_system.h"

#define NUM_CHANNELS 4 

struct mgos_neopixel {
  int pin;
  int num_pixels;
  enum mgos_neopixel_order order;
  uint8_t *data;
};

struct mgos_neopixel *mgos_neopixel_create(int pin, int num_pixels,enum mgos_neopixel_order order) {
  mgos_gpio_set_mode(pin, MGOS_GPIO_MODE_OUTPUT);
  /* Keep in reset */
  mgos_gpio_write(pin, 0);

  struct mgos_neopixel *np = calloc(1, sizeof(*np));
  np->pin = pin;
  np->num_pixels = num_pixels;
  np->order = order;
  np->data = malloc(num_pixels * NUM_CHANNELS);
  mgos_neopixel_clear(np);
  return np;
}

void mgos_neopixel_set(struct mgos_neopixel *np, int i, int r, int g, int b) {
  uint8_t *p = np->data + i * NUM_CHANNELS;
  switch (np->order) {
    case MGOS_NEOPIXEL_ORDER_RGB:
      p[0] = r;
      p[1] = g;
      p[2] = b;
      break;

    case MGOS_NEOPIXEL_ORDER_GRB:
      p[0] = g;
      p[1] = r;
      p[2] = b;
      break;

    case MGOS_NEOPIXEL_ORDER_BGR:
      p[0] = b;
      p[1] = g;
      p[2] = r;
      break;


    default:
      LOG(LL_ERROR, ("Wrong order: %d", np->order));
      break;
  }
}

void mgos_neopixel_set_rgbw(struct mgos_neopixel *np, int i, int r, int g, int b, int w) {
  LOG(LL_ERROR, ("I got here"));
  uint8_t *p = np->data + i * NUM_CHANNELS;
  p[0] = g;
  p[1] = r;
  p[2] = b;
  p[3] = w;
}

void mgos_neopixel_clear(struct mgos_neopixel *np) {
  memset(np->data, 0, np->num_pixels * NUM_CHANNELS);
}

//Added for writing sk6812RGBW leds
void mgos_neopixel_show(struct mgos_neopixel *np) {
  mgos_gpio_write(np->pin, 0);
  mgos_usleep(60);
  mgos_bitbang_write_bits(np->pin, MGOS_DELAY_100NSEC, 3, 8, 8, 6, np->data,
                          np->num_pixels * NUM_CHANNELS);
  mgos_gpio_write(np->pin, 0);
  mgos_usleep(60);
  mgos_gpio_write(np->pin, 1);
}

void mgos_neopixel_free(struct mgos_neopixel *np) {
  free(np->data);
  free(np);
}

bool mgos_neopixelw_init(void) {
  return true;
}