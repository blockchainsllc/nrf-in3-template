#include "in3_mock.h"
#include "debug.h"


void prepare_response(d_token_t *response_array) {
    sb_t *sb = sb_new(NULL);
    str_range_t r = d_to_json(d_get_at(response_array, 0));
    sb_add_char(sb, '[');
    sb_add_range(sb, r.data, 0, r.len);
    sb_add_char(sb, ']');
    _tmp_response = _malloc(sizeof(bytes_t));
    _tmp_response->data = (uint8_t *) sb->data;
    _tmp_response->len = sb->len;
    _free(sb);

}

static int send_mock(char **urls, int urls_len, char *payload, in3_response_t *result) {
    dbg_log("payload: %s\n", payload);
    int i;
    for (i = 0; i < urls_len; i++)
        sb_add_range(&(result + i)->result, (char *) _tmp_response->data, 0, _tmp_response->len);

//    if (_tmp_response) {
//        free(_tmp_response->data);
//        free(_tmp_response);
//    }
    return 0;
}

int execRequest(in3_t *c, d_token_t *test) {
    d_token_t *request = d_get(test, key("request"));
    d_token_t *config = d_get(request, key("config"));
    d_token_t *t = NULL;
    char *method = NULL;
    char params[10000];

    // configure in3
    c->requestCount = (t = d_get(config, key("requestCount"))) ? d_int(t) : 1;
    method = d_get_string(request, "method");

    str_range_t s = d_to_json(d_get(request, key("params")));
    if (!method) {
        dbg_log("NO METHOD");
        return -1;
    }
    if (!s.data) {
        dbg_log("NO PARAMS");
        return -1;
    }
    strncpy(params, s.data, s.len);
    params[s.len] = 0;

    char *res = NULL, *err = NULL;
    int success = d_get_intkd(test, key("success"), 1);

    //  _tmp_response = response;
    int is_bin = d_get_int(test, "binaryFormat");

    in3_client_rpc(c, method, params, is_bin ? NULL : &res, &err);

    if (err && res) {
        dbg_log("Error and Result set");
        _free(err);
        _free(res);
        return -1;
    } else if (err) {
        if (success) {
            dbg_log("Failed");
            dbg_log(":%s", err);
            _free(err);
            return -1;
        }
        dbg_log("OK");
        _free(err);
        return 0;
    } else if (res || is_bin) {
        if (!success) {
            dbg_log("Should have Failed");
            if (!is_bin) _free(res);
            return -1;
        }
        dbg_log("OK");
        if (!is_bin) _free(res);
        return 0;
    } else {
        dbg_log("NO Error and no Result");
        return -1;
    }
}

int run_request(d_token_t *test, int counter, in3_proof_t proof) {
    dbg_log("running the request");
    char temp[300];
    char *descr = NULL;
    int i;
    in3_log_set_prefix("");

    if ((descr = d_get_string(test, "descr"))) {
        strcpy(temp, descr);
    }
    dbg_log("\n%2i : %-60s ", counter, temp);

    in3_t *c = in3_new();
    int j;
    c->max_attempts = 1;
    c->includeCode = 1;
    c->transport = send_mock;
    d_token_t *first_res = d_get(d_get_at(d_get(test, key("response")), 0), key("result"));
    d_token_t *registry_id = d_type(first_res) == T_OBJECT ? d_get(first_res, key("registryId")) : NULL;
    for (j = 0; j < c->chainsCount; j++) {
        c->chains[j].needsUpdate = false;
        if (registry_id) {
            c->chains[j].version = 2;
            memcpy(c->chains[j].registry_id, d_bytesl(registry_id, 32)->data, 32);
            memcpy(c->chains[j].contract->data, d_get_byteskl(first_res, key("contract"), 20)->data, 20);
        }
    }
    c->proof = proof;

    d_token_t *signatures = d_get(test, key("signatures"));
    c->chainId = d_get_longkd(test, key("chainId"), 1);
    if (signatures) {
        c->signatureCount = d_len(signatures);
        for (j = 0; j < c->chainsCount; j++) {
            if (c->chains[j].chainId == c->chainId) {
                for (i = 0; i < c->chains[j].nodeListLength; i++) {
                    if (i < c->signatureCount)
                        memcpy(c->chains[j].nodeList[i].address->data, d_get_bytes_at(signatures, i)->data, 20);
                    else
                        c->chains[j].weights[i].blacklistedUntil = 0xFFFFFFFFFFFFFF;
                }
            }
        }
    }
    int fail = execRequest(c, test);
    in3_free(c);
    dbg_log(" ( SUCCESS ?! %d) ", fail);

    d_token_t *response = d_get(test, key("response"));
    str_range_t res_size = d_to_json(response);
    bytes_builder_t *bb = bb_new();
    d_serialize_binary(bb, response);
    dbg_log(" (json: %lu bin: %i) ", res_size.len, bb->b.len);
    bb_free(bb);
    return fail;
}
