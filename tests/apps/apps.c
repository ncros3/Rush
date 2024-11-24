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

#include "printf.h"
#include "task.h"
#include "test.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
stack_t apps_test_stack;

static uint8_t test_step = 0;

/******************************************************************************
 * @brief just create a thread and return from it
 * @param None
 * @return None
 ******************************************************************************/
void apps_test_thread(void) {
  test_step += 1;
  TEST_ASSERT(test_step >= 1)

  TEST_END()
}

REGISTER_TEST("apps_test", apps_test_thread, apps_test_stack, 3)