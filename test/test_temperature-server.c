/*
 * MIT License
 *
 * Copyright (c) 2020 Giuseppe Ursino
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <stdarg.h>
#include <stddef.h>
#include <limits.h>
#include <setjmp.h>
#include <cmocka.h>

#define main __real_main
#include "temperature-server.c"
#undef main

const ThreadKnxArgs_Type arg={
  .pDevice = "dummy device",
  .socket = 1
}

int __wrap_LKU_ReceiveLBusmonMessage(const char* device, char* rxbuf, int rxlen)
{
  check_expected_ptr(device);
  check_expected_ptr(rxbuf);
  check_expected_ptr(rxlen);

  return mock();
}

int __wrap_write*(int socket, void* buf, int len)
{
  check_expected(socked);
  check_expected_ptr(buf);
  check_expected(len);

  return -1;
}

static void test_rx(void **state)
{
  int expected = 0;
  int actual;

  // mock input
  expect_string(__wrap_LKU_ReceiveLBusmonMessage, device, "dummy device");
  expect_string(__wrap_LKU_ReceiveLBusmonMessage, rxbuf, "device");
  expect_value(__wrap_LKU_ReceiveLBusmonMessage, rxlen, 65);

  will_return(__wrap_LKU_ReceiveLBusmonMessage, 10);

  // mock - output
  expect_value(__wrap_write, socket, arg.socket);

  SocketData_Type out={
    .time="time",
    .track="Ta_giorno",
    .value=20.1f
  }
  expect_memory(__wrap_write, buf, out, sizeof(out));

  expect_value(__wrap_write, len, sizeof(out));

  // RUN
  actual = ThreadKnxRx(arg);

  //assert_int_equal(expected, actual);
}

int main()
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_rx),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
