/*******************************************************************************
 * This file is part of the Incubed project.
 * Sources: https://github.com/slockit/in3-c
 * 
 * Copyright (C) 2018-2019 slock.it GmbH, Blockchains LLC
 * 
 * 
 * COMMERCIAL LICENSE USAGE
 * 
 * Licensees holding a valid commercial license may use this file in accordance 
 * with the commercial license agreement provided with the Software or, alternatively, 
 * in accordance with the terms contained in a written agreement between you and 
 * slock.it GmbH/Blockchains LLC. For licensing terms and conditions or further 
 * information please contact slock.it at in3@slock.it.
 * 	
 * Alternatively, this file may be used under the AGPL license as follows:
 *    
 * AGPL LICENSE USAGE
 * 
 * This program is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Affero General Public License as published by the Free Software 
 * Foundation, either version 3 of the License, or (at your option) any later version.
 *  
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
 * PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
 * [Permissions of this strong copyleft license are conditioned on making available 
 * complete source code of licensed works and modifications, which include larger 
 * works using a licensed work, under the same license. Copyright and license notices 
 * must be preserved. Contributors provide an express grant of patent rights.]
 * You should have received a copy of the GNU Affero General Public License along 
 * with this program. If not, see <https://www.gnu.org/licenses/>.
 *******************************************************************************/

#include "client.h"
#include "../util/data.h"
#include "../util/mem.h"
#include "context.h"
#include "keys.h"
#include "send.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

in3_ctx_t* in3_client_rpc_ctx(in3_t* c, char* method, char* params) {
  // generate the rpc-request
  char req[strlen(method) + strlen(params) + 200];
  snprintX(req, sizeof(req), "{\"method\":\"%s\",\"jsonrpc\":\"2.0\",\"id\":1,\"params\":%s}", method, params);

  // create a new context by parsing the request
  in3_ctx_t* ctx = new_ctx(c, req);

  // this happens if the request is not parseable (JSON-error in params)
  if (ctx->error) return ctx;

  // execute it
  if (in3_send_ctx(ctx) == IN3_OK) {
    // the request was succesfull, so we delete interim errors (which can happen in case in3 had to retry)
    if (ctx->error) _free(ctx->error);
    ctx->error = NULL;
  }

  // return context and hope the calle will clean it.
  return ctx;
}

in3_ret_t in3_client_rpc(in3_t* c, char* method, char* params, char** result, char** error) {
  if (!error) return IN3_EINVAL;
  in3_ret_t res = IN3_OK;
  // prepare request
  char req[strlen(method) + strlen(params) + 200];
  snprintX(req, sizeof(req), "{\"method\":\"%s\",\"jsonrpc\":\"2.0\",\"id\":1,\"params\":%s}", method, params);

  // parse it
  in3_ctx_t*  ctx = new_ctx(c, req);
  str_range_t s;

  // make sure result & error are clean
  if (result) result[0] = 0;
  if (error) *error = NULL;

  // check parse-errors
  if (ctx->error) {
    if (error != NULL) {
      *error = _malloc(strlen(ctx->error) + 1);
      strcpy(*error, ctx->error);
    }
    res = IN3_EUNKNOWN;
  } else {
    // so far everything is good, so we send the request
    res = in3_send_ctx(ctx);
    if (res >= 0) {

      // looks good, so we get the result
      d_token_t* r = d_get(ctx->responses[0], K_RESULT);
      if (r) {
        // we have a result and copy it
        if (result) *result = d_create_json(r);
      } else if ((r = d_get(ctx->responses[0], K_ERROR))) {
        // the response was correct but contains a error-object, which we convert into a string
        if (d_type(r) == T_OBJECT) {
          s = d_to_json(r);
          if (error != NULL) {
            *error = _malloc(s.len + 1);
            strncpy(*error, s.data, s.len);
            (*error)[s.len] = '\0';
          }
        } else {
          if (error != NULL) {
            *error = _malloc(d_len(r) + 1);
            strncpy(*error, d_string(r), d_len(r));
            (*error)[d_len(r)] = '\0';
          }
        }
      } else if (ctx->error) {
        // we don't have a result, but an error, so we copy this
        if (error != NULL) {
          *error = _malloc(strlen(ctx->error) + 1);
          strcpy(*error, ctx->error);
        }
      } else {
        // should not happen
        if (error != NULL) {
          *error = _malloc(50);
          strcpy(*error, "No Result and also no error");
        }
      }

    } else if (ctx->error) {
      // there was an error, copy it
      if (error != NULL) {
        *error = _malloc(strlen(ctx->error) + 1);
        strcpy(*error, ctx->error);
      }
    } else {
      // something went wrong, but no error
      if (error != NULL) {
        *error = _malloc(50);
        strcpy(*error, "Error sending the request");
      }
    }
  }
  free_ctx(ctx);

  // if we have an error, we always return IN3_EUNKNOWN
  return *error ? IN3_EUNKNOWN : res;
}
