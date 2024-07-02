/*
 * Copyright (c) 2023 Qoda, engineering
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms and conditions of the GNU General Public License,
 * version 3 or later, as published by the Free Software Foundation.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.

 * You should have received copies of the GNU General Public License and
 * the GNU Lesser General Public License along with this program.  If
 * not, see https://www.gnu.org/licenses/
 */

#include "interrupt.h"

#include "ax_syscall.h"
#include "irq_arch.h"
#include "printk.h"
#include "test.h"
#include "timer.h"

#define TIMER_PERIOD_IN_S 0.3

#define TIMER_LO_LIMIT_IN_US (TIMER_PERIOD_IN_S * ARCH_TIMER_RATE * 0.8 / 10UL)
#define TIMER_HI_LIMIT_IN_US (TIMER_PERIOD_IN_S * ARCH_TIMER_RATE * 1.2 / 10UL)

/*******************************************************************************
 * Definitions
 ******************************************************************************/
stack_t interrupt_thread_stack;

static uint8_t test_step = 0;

/******************************************************************************
 * @brief initalize interrupt handler and timer
 * @param None
 * @return None
 ******************************************************************************/
void interrupt_thread(void) {
  time_t raw_timer_value  = 0;
  time_t start_date_in_us = 0;
  time_t stop_date_in_us  = 0;
  time_t duration_in_us   = 0;

  // STEP 1
  test_step += 1;
  TEST_ASSERT(test_step >= 1)

  // get the current time
  start_date_in_us = reg_read_double_word(TIMER_MTIME_ADDR) / 10UL;

  // set the timer to wait 1 s
  raw_timer_value = reg_read_double_word(TIMER_MTIME_ADDR);
  raw_timer_value += (time_t)(TIMER_PERIOD_IN_S * ARCH_TIMER_RATE);
  reg_write_double_word(TIMER_MTIMECMP_ADDR, raw_timer_value);

  // enable the interrupt in csr register
  ax_interrupt_request(TIMER_INTERRUPT);

  // go to sleep, the interrupt will wake up the task in 1s
  ax_task_sleep();

  // STEP 2
  test_step += 1;
  TEST_ASSERT(test_step >= 2)

  // get the current time
  stop_date_in_us = reg_read_double_word(TIMER_MTIME_ADDR) / 10UL;

  duration_in_us = stop_date_in_us - start_date_in_us;
  printk("Duration : %d\r\n", duration_in_us);

  if ((duration_in_us > (time_t)TIMER_LO_LIMIT_IN_US) &&
      (duration_in_us < (time_t)TIMER_HI_LIMIT_IN_US)) {
    test_step += 1;
    printk("TEST OK");
  }

  // STEP 3
  TEST_ASSERT(test_step >= 3)
}

REGISTER_TEST("interrupt_test", interrupt_thread, interrupt_thread_stack, 5)