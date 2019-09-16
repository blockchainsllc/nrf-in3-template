#include "client.h"   // the core client
#include <eth_api.h>  // wrapper for easier use
#include <eth_basic.h> // the full ethereum verifier containing the EVM
#include <log.h>
#include <inttypes.h>
#include <stdio.h>
#include "context.h"
#include "mem.h"

static bytes_t* _tmp_response;
//mock for in3_basic






void prepare_response(d_token_t* response_array) ;

int execRequest(in3_t* c, d_token_t* test) ;

int run_request(d_token_t* test, int counter, in3_proof_t proof) ;
