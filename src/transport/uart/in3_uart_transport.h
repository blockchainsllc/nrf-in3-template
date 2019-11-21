#ifndef IN3_UART_TRANSPORT_H_
  #define IN3_UART_TRANSPORT_H_
  #include <in3/client.h>   // the core client
  in3_ret_t transport_uart(char **urls, int urls_len, char *payload, in3_response_t *result);
  void transport_uart_init();
#endif
