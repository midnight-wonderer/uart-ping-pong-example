// Copyright (c) 2020 Sarun Rattanasiri
// under the 3-Clause BSD License
// https://opensource.org/licenses/BSD-3-Clause

#include "stm8s.h"

#include "haystack_command_encoder.h"
#include "stm8s_uart1_transceiver.h"
#define OUTPUT_COMMAND_PING (0)
#define OUTPUT_COMMAND_PONG (1)
static const HaystackCommandEncoder_Config command_encoder_config = {
    // 128-bit key generated with cryptographically-secured method
    // to ensure collision resistance
    .key = {0xe0, 0x9b, 0x9f, 0xa4, 0x4c, 0xc1, 0xaa, 0x99, 0xdc, 0xa8, 0x7e,
            0xfe, 0xc1, 0xb4, 0x88, 0x0c},
    .port_ready = &is_uart_ready,
    .port_flush = &uart_send,
};
static HaystackCommandEncoder_State command_encoder_state;
// These are things needed to be implemented for each hardware
// For this example, I used a firmware that also created by Slime Systems
// included from uart1_transceiver.h (more on that later)
static bool is_uart_ready() {
  return stm8s_uart1__is_ready();
}
static void uart_send(uint8_t command) {
  stm8s_uart1__send(command);
}

#include "haystack_command_decoder.h"
#define INPUT_COMMAND_PING (0)
#define INPUT_COMMAND_PONG (1)
static const HaystackCommandDecoder_Config command_decoder_config = {
    // Should be the same key as the encoder.
    // You can use a different key but AFAIK there is no apparent reason to do
    // so.
    .key = {0xe0, 0x9b, 0x9f, 0xa4, 0x4c, 0xc1, 0xaa, 0x99, 0xdc, 0xa8, 0x7e,
            0xfe, 0xc1, 0xb4, 0x88, 0x0c},
    .command_received = &command_handler,
};
static HaystackCommandDecoder_State command_decoder_state;
static void command_handler(uint8_t command) {
  switch (command) {
    case INPUT_COMMAND_PING:
      // when we receive a PING we response with a PONG
      haystack_command_encoder__queue(&command_encoder_state,
                                      OUTPUT_COMMAND_PONG);
      break;
  }
}

#include "stm8s_uart1_transceiver.h"
static const stm8sUart1_ReceiverConfig receiver_config = {
    .data_received = &data_handler,
    .overrun_detected = &uart_error_handler,
    .data_error_detected = &uart_error_handler,
};
static void data_handler(uint8_t data) {
  haystack_command_decoder__push(&command_decoder_config,
                                 &command_decoder_state, data);
}
static void uart_error_handler(void) {
  haystack_command_decoder__discard(&command_decoder_state);
}

int main() {
  disableInterrupts();
  haystack_command_encoder__init(&command_encoder_state);
  haystack_command_decoder__init(&command_decoder_state);
  stm8s_uart1__setup_clock(0x1a, 0x01);
  stm8s_uart1__setup_transmitter();
  stm8s_uart1__setup_receiver(&receiver_config);
  enableInterrupts();
  for (;;) {
    haystack_command_decoder__execute_pending(&command_decoder_config,
                                              &command_decoder_state);
    haystack_command_encoder__poll(&command_encoder_config,
                                   &command_encoder_state);
  }
}
