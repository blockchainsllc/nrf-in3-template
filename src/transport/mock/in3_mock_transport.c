#include "in3_mock_transport.h"
#include <in3/client.h>   // the core client
#include <debug.h>

#ifdef IN3_PAYLOAD_JSON
#include <in3_responses/block_response_json.h>
#include <in3_responses/eth_call_response_json.h>
#include <in3_responses/eth_getCode_response_json.h>
#include <in3_responses/in3_nodeList_response_json.h>
#elif IN3_PAYLOAD_BINARY
#include <in3_responses/block_response_binary.h>
#include <in3_responses/eth_call_response_binary.h>
#include <in3_responses/eth_getCode_response_binary.h>
#include <in3_responses/in3_nodeList_response_binary.h>
#endif

in3_ret_t local_transport_func(char **urls, int urls_len, char *payload, in3_response_t *result) {
  for (int i=0; i<urls_len; i++) {
    if (strstr(payload, "nodeList") != NULL) {
      dbg_log("Returning Node List ...\n");
      sb_add_range(&(result[i].result), mock_in3_nodeList_response, 0, mock_in3_nodeList_response_len);
    }
    else if (strstr(payload, "BlockByNumber") != NULL) {
      dbg_log("Returning Block Response ...\n");
      sb_add_range(&(result[i].result), mock_block_response, 0, mock_block_response_len);
		}
    else if (strstr(payload, "eth_call") != NULL) {
      dbg_log("Returning Call Response ...\n");
      sb_add_range(&(result[i].result), mock_eth_call_response, 0, mock_eth_call_response_len);
		}
    else if (strstr(payload, "eth_getCode") != NULL) {
      dbg_log_raw("Payload: %s\n", payload);
      dbg_log("Returning getCode Response ...\n");
      sb_add_range(&(result[i].result), mock_eth_getCode_response, 0, mock_eth_getCode_response_len);
		}
    else {
			dbg_log("Not supported for this mock\n");
		}
	}

  return IN3_OK;
}

in3_ret_t transport_mock(in3_request_t* req)
{
	return local_transport_func((const char**) req->urls, req->urls_len, req->payload, req->results);
}
