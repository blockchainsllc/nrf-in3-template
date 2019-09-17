#include <client.h>   // the core client
#include <eth_api.h>  // wrapper for easier use
#include <eth_basic.h> // the full ethereum verifier containing the EVM
#include <log.h>
#include <inttypes.h>
#include <debug.h>
#include <in3_mock_transport.h>
#include <stdio.h>
#if defined(_WIN32) || defined(WIN32)
#include <windows.h>
#else
#include <unistd.h>
#endif

int main() {

  dbg_log("================== NRF-IN3 ====================\n");

  // register a chain-verifier for full Ethereum-Support
  in3_register_eth_basic();

  // create new incubed client
  in3_t* in3_client = in3_new();

  // set your config
  in3_client->transport    = transport_mock; // use curl to handle the requests
  in3_client->requestCount = 1;         // number of requests to send
  in3_client->chainId      = 0x1;     // use tobalaba

  in3_log_set_level(LOG_TRACE);

  // use a ethereum-api instead of pure JSON-RPC-Requests
  eth_block_t* block = eth_getBlockByNumber(in3_client, 6970454, true);

  if (!block)
    printf("Could not find the Block: %s\n", eth_last_error());
  else {
    printf("Number of verified transactions in block: %i\n", block->tx_count);
    free(block);
  }

  // clean up
  in3_free(in3_client);
}
