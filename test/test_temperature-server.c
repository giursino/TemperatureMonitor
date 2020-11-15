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
  msg = mock_type(uint8_t *);

  int msglen;
  msglen = mock_type(int);

  if (rxlen < msglen) return -1;

  memcpy(rxbuf, msg, msglen);


  return msglen;
}

int __wrap_write(int socket, void* buf, int len)
{
  check_expected(socket);

  char* out_track = ((SocketData_Type *) buf)->track;
  check_expected_ptr(out_track);

  float out_value = ((SocketData_Type *) buf)->value;
  check_expected(out_value);

  check_expected(len);

  // to exit from main loop
  toexit = mock_type(bool);

  return len;
}

const uint8_t in1[] = {0xBC, 0x11, 0x0F, 0x21, 0x77, 0xE2, 0x00, 0x80, 0x0C, 0x1A, 0xCC};
const SocketData_Type out1 = {.track = "Ta_giorno", .value = 21.0f};

const uint8_t in2[] = {0xBC, 0x11, 0x0F, 0x21, 0x77, 0xE2, 0x00, 0x80, 0x0C, 0x1A, 0xCC};
const SocketData_Type out2 = {.track = "Ta_giorno", .value = 21.0f};

typedef struct{const uint8_t (*in)[]; uint8_t in_len; const SocketData_Type* out;} test_data_t;
const test_data_t test_data[]={
  {&in1, sizeof(in1)/sizeof(uint8_t), &out1},
  {&in2, sizeof(in2)/sizeof(uint8_t), &out2},
};

static void test_rx(void **state)
{
  ThreadKnxArgs_Type arg={
    .pDevice = 0,
    .socket = 1
  };

  for (int i=0; i<sizeof(test_data)/sizeof(test_data[0]); i++) {

    // mock "LKU_ReceiveLBusmonMessage"
    expect_value(__wrap_LKU_ReceiveLBusmonMessage, device, arg.pDevice);
    will_return(__wrap_LKU_ReceiveLBusmonMessage, test_data[i].in);
    will_return(__wrap_LKU_ReceiveLBusmonMessage, test_data[i].in_len);


    // mock "write"
    expect_value(__wrap_write, socket, arg.socket);
    expect_string(__wrap_write, out_track, test_data[i].out->track);
    expect_value(__wrap_write, out_value, test_data[i].out->value);
    expect_value(__wrap_write, len, sizeof(SocketData_Type));


    // to exit from main loop
    bool toexit = false;
    if (i == (sizeof(test_data)/sizeof(test_data[0]) - 1)) {
      toexit = true;
    }
    will_return(__wrap_write, toexit);
  }

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
