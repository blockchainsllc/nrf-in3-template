#include <client.h>   // the core client
#include <eth_api.h>  // wrapper for easier use
#include <eth_full.h> // the full ethereum verifier containing the EVM
#include <log.h>
#include <inttypes.h>
#include <debug.h>
#include <in3_uart_transport.h>
#include "nfc_ble_pair_lib.h"
#include <in3_ble_transport.h>
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

int main() {

  NRF_LOG_INFO("================== NRF-IN3 ====================\n");

  // INCUBED VERIFICATION LEVEL SELECTOR
  // register a chain-verifier for full Ethereum-Support
  //in3_register_eth_nano();
  //in3_register_eth_basic();
  in3_register_eth_full();

  // TRANSPORT MODULE SELECTOR
  //transport_ble_init();
  transport_uart_init();

  // Instantiate new incubed client
  in3_t* in3_client = in3_new();

  // INCUBED CLIENT CONFIGURATION
  // REPEAT TRANSPORT AND PROOF SELECTION
  // in3_client->transport    = transport_ble;
  in3_client->transport    = transport_uart;
  in3_client->requestCount = 1;         // number of requests to send
  in3_client->chainId      = 0x1;
  //in3_client->proof        = PROOF_NONE; // NANO
  //in3_client->proof        = PROOF_STANDARD; // BASIC
  in3_client->proof        = PROOF_FULL; // FULL

  in3_log_set_level(LOG_TRACE);

// BLUETOOTH ONLY
//  while(!transport_connected());

  // use a ethereum-api instead of pure JSON-RPC-Requests
  eth_block_t* block = eth_getBlockByNumber(in3_client, 6970454, true);

  if (!block) {
    NRF_LOG_INFO("Could not find the Block: %s\n", eth_last_error());
  }
  else {
    NRF_LOG_INFO("Number of verified transactions in block: %d\n", block->tx_count);
    free(block);
  }


  // define a address (20byte)
  address_t contract;

  // copy the hexcoded string into this address
  hex2byte_arr("0x2736D225f85740f42D17987100dc8d58e9e16252", -1, contract, 20);

  // ask for the number of servers registered
  json_ctx_t* response = eth_call_fn(in3_client, contract, "totalServers():uint256");
  if (!response) {
    NRF_LOG_INFO("Could not get the response: %s\n", eth_last_error());
    return -1;
  }

  // convert the response to a uint32_t,
  uint32_t number_of_servers = d_int(response->result);

  // clean up resources
  free_json(response);

  // output
  NRF_LOG_INFO("Found %u servers registered : \n", number_of_servers);

  // clean up
  in3_free(in3_client);
}
