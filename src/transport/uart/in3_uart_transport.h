#ifndef IN3_UART_TRANSPORT_H_
  #define IN3_UART_TRANSPORT_H_
  #include <in3/client.h>   // the core client
  in3_ret_t transport_uart(in3_request_t* req);
  void transport_uart_init();
#endif
