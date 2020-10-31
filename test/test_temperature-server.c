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
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>

#define main __real_main
#include "temperature-server.c"
#undef main

int __wrap_LKU_ReceiveLBusmonMessage(const hid_device* device, uint8_t* rxbuf, int rxlen)
{
  check_expected_ptr(device);

  uint8_t* msg;
  msg = mock_ptr_type(uint8_t *);

  int msglen;
  msglen = mock_type(int);

  if (rxlen < msglen) return -1;

  memcpy(rxbuf, msg, msglen);

  return msglen;
}

int __wrap_write(int socket, void* buf, int len)
{
  check_expected(socket);
  check_expected_ptr(buf);
  check_expected(len);

  // to exit from main loop
  return -1;
}

size_t __wrap_strftime(char *s, size_t max, const char *format, const struct tm *tm)
{
  char* time;
  time = mock_ptr_type(char *);
  memcpy(s, time, strlen(time));
  s[strlen(time)]=0;


  return strlen(s);
}




static void test_rx(void **state)
{
  ThreadKnxArgs_Type arg={
    .pDevice = 0,
    .socket = 1
  };

  // mock "LKU_ReceiveLBusmonMessage"
  uint8_t knxmsg[11]={0xBC, 0x11, 0x0F, 0x21, 0x77, 0xE2, 0x00, 0x80, 0x0C, 0x1A, 0xCC};

  expect_value(__wrap_LKU_ReceiveLBusmonMessage, device, arg.pDevice);
  will_return(__wrap_LKU_ReceiveLBusmonMessage, cast_to_largest_integral_type(knxmsg));
  will_return(__wrap_LKU_ReceiveLBusmonMessage, (sizeof(knxmsg)/sizeof(knxmsg[0])));


  // mock "write"
  SocketData_Type out={
    .time = "time",
    .track = "Ta_giorno",
    .value = 20.0f,
  };

  expect_value(__wrap_write, socket, arg.socket);
  expect_memory(__wrap_write, buf, &out, sizeof(out));
  expect_value(__wrap_write, len, sizeof(out));

  // mock "strftime"
  will_return(__wrap_strftime, cast_to_largest_integral_type(out.time));
  will_return(__wrap_strftime, cast_to_largest_integral_type(out.time));


  // RUN
  ThreadKnxRx(&arg);
}

int main()
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_rx),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
