#ifndef IN3_BLE_TRANSPORT_H_
  #define IN3_BLE_TRANSPORT_H_
  #include <client.h>   // the core client
  in3_ret_t transport_ble(char **urls, int urls_len, char *payload, in3_response_t *result);
  int transport_connected();
  void transport_ble_init();
#endif
