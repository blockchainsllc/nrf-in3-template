#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "app_uart.h"
#include "app_error.h"
#include "nrf.h"
#include "bsp.h"
#if defined (UART_PRESENT)
#include "nrf_uart.h"
#endif
#if defined (UARTE_PRESENT)
#include "nrf_uarte.h"
#endif

//IN3 Stuff
#include <debug.h>
#include <in3/client.h>

#include <in3_uart_transport.h>

#define RX_PIN_NUMBER NRF_GPIO_PIN_MAP(0,31)
#define TX_PIN_NUMBER NRF_GPIO_PIN_MAP(0,29)
#define RTS_PIN_NUMBER NRF_GPIO_PIN_MAP(1,13)
#define CTS_PIN_NUMBER NRF_GPIO_PIN_MAP(1,15)

#define MAX_RESPONSE_LEN 65536


//#define ENABLE_LOOPBACK_TEST  /**< if defined, then this example will be a loopback test, which means that TX should be connected to RX to get data loopback. */

#define MAX_TEST_DATA_BYTES     (15U)                /**< max number of test bytes to be used for tx and rx. */
#define UART_TX_BUF_SIZE 256                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 256                         /**< UART RX buffer size. */

void uart_error_handle(app_uart_evt_t * p_event)
{
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR)
    {
        dbg_log_raw("APP_UART_COMMUNICATION_ERROR\r\n");
        APP_ERROR_HANDLER(p_event->data.error_communication);
    }
    else if (p_event->evt_type == APP_UART_FIFO_ERROR)
    {
        dbg_log_raw("APP_UART_FIFO_ERROR\r\n");
        APP_ERROR_HANDLER(p_event->data.error_code);
    }
}


/* When UART is used for communication with the host do not use flow control.*/
#define UART_HWFC APP_UART_FLOW_CONTROL_DISABLED

void transport_uart_init() {
  uint32_t err_code;

  const app_uart_comm_params_t comm_params =
    {
        RX_PIN_NUMBER,
        TX_PIN_NUMBER,
        RTS_PIN_NUMBER,
        CTS_PIN_NUMBER,
        UART_HWFC,
        false,
#if defined (UART_PRESENT)
        NRF_UART_BAUDRATE_115200
#else
        NRF_UARTE_BAUDRATE_115200
#endif
    };

  APP_UART_FIFO_INIT(&comm_params,
                       UART_RX_BUF_SIZE,
                       UART_TX_BUF_SIZE,
                       uart_error_handle,
                       APP_IRQ_PRIORITY_LOWEST,
                       err_code);

  APP_ERROR_CHECK(err_code);
}

/**
 * @brief Function for main application entry.
 */
in3_ret_t local_transport_uart_func (char **urls, int urls_len, char *payload, in3_response_t *result) {

    uint8_t response[MAX_RESPONSE_LEN];
    int flag_response_complete = 0;

    for (int i=0; i<urls_len; i++) {

      int url_len = strlen(urls[i]);

      for (int j=0; j<url_len; j++) {
        while (app_uart_put(urls[i][j]) != NRF_SUCCESS);
      }

      while (app_uart_put(';') != NRF_SUCCESS);

      int payload_len = strlen(payload);

      for (int j=0; j<payload_len; j++) {
        while (app_uart_put(payload[j]) != NRF_SUCCESS);
      }

      while (app_uart_put('\0') != NRF_SUCCESS);

      //read the response
      int response_len = 0;

      while (!flag_response_complete) {
        while (app_uart_get(&response[response_len]) != NRF_SUCCESS);
        if (response[response_len] == '\0') {\
          flag_response_complete = 1;
        }
        else if (response_len > MAX_RESPONSE_LEN) {
          dbg_log("Exceeded the maximium length of expected response.");
          return IN3_ETRANS;
        }
        response_len++;
      }

      dbg_log("Response Length: %d\r\n", response_len);

      sb_add_range(&(result[i].result), response, 0, response_len);
    }

    return IN3_OK;
}

in3_ret_t transport_uart(in3_request_t* req)
{
	return local_transport_uart_func((const char**) req->urls, req->urls_len, req->payload, req->results);
}
