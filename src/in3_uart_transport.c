#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "app_uart.h"
#include "app_error.h"
#include <debug.h>
#include "nrf.h"
#include "bsp.h"
#if defined (UART_PRESENT)
#include "nrf_uart.h"
#endif
#if defined (UARTE_PRESENT)
#include "nrf_uarte.h"
#endif


//#define ENABLE_LOOPBACK_TEST  /**< if defined, then this example will be a loopback test, which means that TX should be connected to RX to get data loopback. */

#define MAX_TEST_DATA_BYTES     (15U)                /**< max number of test bytes to be used for tx and rx. */
#define UART_TX_BUF_SIZE 256                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 256                         /**< UART RX buffer size. */

void uart_error_handle(app_uart_evt_t * p_event)
{
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_communication);
    }
    else if (p_event->evt_type == APP_UART_FIFO_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_code);
    }
}


/* When UART is used for communication with the host do not use flow control.*/
#define UART_HWFC APP_UART_FLOW_CONTROL_DISABLED

/**
 * @brief Function for main application entry.
 */
in3_ret_t transport_uart (char **urls, int urls_len, char *payload, in3_response_t *result) {
    uint32_t err_code;

    bsp_board_init(BSP_INIT_LEDS);

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

    printf("\r\nSending IN3 payload over UART\r\n");

    uint32_t max_response_len = 65536;
    uint8_t response[max_response_len];
    int flag_response_complete = 0;
    char * p_url;

    for (int i=0; i<urls_len; i++) {
      dbg_log("url: %s\n", urls[i]);
      dbg_log("payload: %s\n", payload);

      p_url = urls[i];

      while(p_url != '\0') {
        while(app_uart_put(*p_url) != NRF_SUCCESS);
        p_url++;
      }

      while(app_uart_put(";") != NRF_SUCCESS);

      while(payload != '\0') {
        while(app_uart_put(*payload) != NRF_SUCCESS);
        payload++;
      }


      //read the response
      int response_len = 0;
      while(!flag_response_complete) {
        while(app_uart_get(&response[response_len]) != NRF_SUCCESS);
        if(response[i] == '\0') {
          flag_response_complete = 1;
        }
        response_len++;
      }
      response[response_len] = '\0';
      response_len++;

      dbg_log("Response: %s\n", response);

      sb_add_range(&(result[i].result), response, 0, response_len);
    }
}
