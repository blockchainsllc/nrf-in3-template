#include <in3/client.h>   // the core client
#include <in3/eth_api.h>  // wrapper for easier use
#ifdef IN3_VERSION_NANO
#include <in3/eth_nano.h>
#elif IN3_VERSION_BASIC
#include <in3/eth_basic.h>
#elif IN3_VERSION_FULL
#include <in3/eth_full.h>
#endif
#include <in3/log.h>
// #include <in3/inttypes.h>
#ifdef IN3_TRANSPORT_BLE
#include "nfc_ble_pair_lib.h"
#include <in3_ble_transport.h>
#elif IN3_TRANSPORT_UART
#include <in3_uart_transport.h>
#elif IN3_TRANSPORT_MOCK
#include <in3_mock_transport.h>
#endif
#include "nrf_delay.h"
#include <stdio.h>

#if defined(_WIN32) || defined(WIN32)
#include <windows.h>
#else

#include <unistd.h>

#endif

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "debug.h"

int main() {

    dbg_log("-----IN3 PROJECT TEMPLATE FOR NRF52840-----\n");
    // INCUBED VERIFICATION LEVEL SELECTOR SET ON MAKEFILE
#ifdef IN3_VERSION_NANO
    in3_register_eth_nano();
#elif IN3_VERSION_BASIC
    in3_register_eth_basic();
#elif IN3_VERSION_FULL
    in3_register_eth_full();
#endif
    // TRANSPORT MODULE SELECTOR SET ON MAKEFILE
#ifdef IN3_TRANSPORT_BLE
    transport_ble_init();
#elif IN3_TRANSPORT_UART
    transport_uart_init();
#endif

    // Instantiate new incubed client
    in3_t* in3_client = in3_new();
    // INCUBED CLIENT CONFIGURATION
    in3_client->requestCount = 1; // number of requests to send
    in3_log_set_level(LOG_TRACE);
#ifdef IN3_CHAIN_ID
    in3_client->chainId      = IN3_CHAIN_ID;
#else
    in3_client->chainId      = 0x1;
#endif
#ifdef IN3_TRANSPORT_BLE
    in3_client->transport    = transport_ble;
#elif IN3_TRANSPORT_UART
    in3_client->transport    = transport_uart;
#elif IN3_TRANSPORT_MOCK
    in3_client->transport    = transport_mock;
#endif
#ifdef IN3_VERSION_NANO
    in3_client->proof        = PROOF_NONE; // NANO
#elif IN3_VERSION_BASIC
    in3_client->proof        = PROOF_STANDARD; // BASIC
#elif IN3_VERSION_FULL
    in3_client->proof        = PROOF_FULL;
#endif
#ifdef IN3_PAYLOAD_BINARY
    in3_client->use_binary   = 1;
#endif

#ifdef IN3_TRANSPORT_BLE
    while(!transport_connected());
#endif

    // use a ethereum-api instead of pure JSON-RPC-Requests
    eth_block_t* block = eth_getBlockByNumber(in3_client, BLKNUM(6970454), true);
    if (!block) {
      dbg_log("Could not find the Block: %s\n", eth_last_error());
    }
    else {
      dbg_log("Number of verified transactions in block: %d\n", block->tx_count);
      free(block);
    }

#if defined(IN3_VERSION_FULL) || defined(IN3_VERSION_BASIC)
    // define a address (20byte)
    address_t contract;
    // copy the hexcoded string into this address
    hex2byte_arr("0x2736D225f85740f42D17987100dc8d58e9e16252", -1, contract, 20);
    // ask for the number of servers registered
    json_ctx_t* response = eth_call_fn(in3_client, contract, BLKNUM_LATEST(), "totalServers():uint256");
    if (!response) {
      dbg_log("Could not get the response: %s\n", eth_last_error());
      return -1;
    }
    // convert the response to a uint32_t,
    uint32_t number_of_servers = d_int(response->result);
    // clean up resources
    free_json(response);
    // output
    dbg_log("Found %u servers registered.\n", number_of_servers);

#elif IN3_VERSION_NANO
    dbg_log("Nano version, no `eth_call` allowed ;)\n");
#endif

    // clean up
    in3_free(in3_client);
  }
