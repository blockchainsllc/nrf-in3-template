#include <client.h>   // the core client
#include <eth_api.h>  // wrapper for easier use
#include <eth_full.h> // the full ethereum verifier containing the EVM
#include <log.h>
#include <inttypes.h>
#include <debug.h>
#include <in3_uart_transport.h>
#include <in3_ble_transport.h>
#include <stdio.h>
#if defined(_WIN32) || defined(WIN32)
#include <windows.h>
#else
#include <unistd.h>
#endif

int main() {

  dbg_log("================== NRF-IN3 ====================\n");

  // register a chain-verifier for full Ethereum-Support
  in3_register_eth_full();

  transport_ble_init();

  // create new incubed client
  in3_t* in3_client = in3_new();

  // set your config
  in3_client->transport    = transport_ble; // use uart to handle the requests
  in3_client->requestCount = 1;         // number of requests to send
  in3_client->chainId      = 0x1;
  in3_client->proof        = PROOF_FULL;

  in3_log_set_level(LOG_TRACE);

  // use a ethereum-api instead of pure JSON-RPC-Requests
  eth_block_t* block = eth_getBlockByNumber(in3_client, 6970454, true);

  if (!block)
    dbg_log("Could not find the Block: %s\n", eth_last_error());
  else {
    dbg_log("Number of verified transactions in block: %d\n", block->tx_count);
    free(block);
  }

  // define a address (20byte)
  address_t contract;

  // copy the hexcoded string into this address
  hex2byte_arr("0x2736D225f85740f42D17987100dc8d58e9e16252", -1, contract, 20);

  // ask for the number of servers registered
  json_ctx_t* response = eth_call_fn(in3_client, contract, "totalServers():uint256");
  if (!response) {
    dbg_log("Could not get the response: %s", eth_last_error());
    return -1;
  }

  // convert the response to a uint32_t,
  uint32_t number_of_servers = d_int(response->result);

  // clean up resources
  free_json(response);

  // output
  dbg_log("Found %u servers registered : \n", number_of_servers);

  // clean up
  in3_free(in3_client);
}
