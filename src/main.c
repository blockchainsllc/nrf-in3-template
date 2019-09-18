#include <client.h>   // the core client
#include <eth_api.h>  // wrapper for easier use
#include <eth_full.h> // the full ethereum verifier containing the EVM
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
  in3_register_eth_full();

  // create new incubed client
  in3_t* in3_client = in3_new();

  // set your config
  in3_client->transport    = transport_mock; // use curl to handle the requests
  in3_client->requestCount = 1;         // number of requests to send
  in3_client->chainId      = 0x1;
  in3_client->proof        = PROOF_STANDARD;

  in3_log_set_level(LOG_TRACE);

  // use a ethereum-api instead of pure JSON-RPC-Requests
  eth_block_t* block = eth_getBlockByNumber(in3_client, 6970454, true);

  if (!block)
    dbg_log("Could not find the Block: %s\n", eth_last_error());
  else {
    dbg_log("Number of verified transactions in block: %d\n", block->tx_count);
    free(block);
  }

  // clean up
  in3_free(in3_client);
}
