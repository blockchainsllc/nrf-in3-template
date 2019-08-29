#include <client.h>   // the core client
#include <eth_api.h>  // wrapper for easier use
#include <eth_full.h> // the full ethereum verifier containing the EVM
#include <log.h>
#include <inttypes.h>
#include <stdio.h>
#if defined(_WIN32) || defined(WIN32)
#include <windows.h>
#else
#include <unistd.h>
#endif

int main() {

  // register a chain-verifier for full Ethereum-Support
  in3_register_eth_full();

  // create new incubed client
  in3_t* in3_client = in3_new();

  // set your config
  in3_client->transport    = NULL; // use curl to handle the requests
  in3_client->requestCount = 1;         // number of requests to send
  in3_client->chainId      = 0x44d;     // use tobalaba

  in3_log_set_level(LOG_TRACE);

  // use a ethereum-api instead of pure JSON-RPC-Requests
  eth_block_t* block = eth_getBlockByNumber(in3_client, 11773341, true);

  if (!block)
    printf("Could not find the Block: %s\n", eth_last_error());
  else {
    printf("Number of verified transactions in block: %i\n", block->tx_count);
    free(block);
  }

  // clean up
  in3_free(in3_client);
}
