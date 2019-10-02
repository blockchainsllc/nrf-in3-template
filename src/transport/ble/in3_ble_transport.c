/**
 * Copyright (c) 2016 - 2019, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "app_error.h"
#include "ble_m.h"
#include "pm_m.h"
#include "nfc_ble_pair_lib.h"
#include "app_timer.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_ble_lesc.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "in3_ble_transport.h"
#include "debug.h"

#include "ble_hci.h"
#include "ble_advertising.h"
#include "ble_nus.h"
#include "nrf_delay.h"
#include "app_util_platform.h"
#include "bsp_btn_ble.h"


#define DEAD_BEEF   0xDEADBEEF /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */
#define DEVICE_NAME "NRF_IN3"

#ifndef NFC_PAIRING_MODE
    #define NFC_PAIRING_MODE NFC_PAIRING_MODE_LESC_OOB
#endif

/**@brief Function for initializing nrf logger.
 */
static void log_init()
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}


/**@brief Function for initializing power management.
 */
static void power_management_init(void)
{
    ret_code_t err_code;
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for initializing application timer.
 */
static void timer_init()
{
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);
}

void idle_state_handle(void)
{
    ret_code_t err_code;

    err_code = nrf_ble_lesc_request_handler();
    if (err_code != NRF_ERROR_INVALID_STATE)
    {
        APP_ERROR_CHECK(err_code);
    }

    if (NRF_LOG_PROCESS() == false)
    {
        nrf_pwr_mgmt_run();
    }
}

/**@brief Function for initializing NFC BLE pairing module.
 */
static void nfc_pairing_init()
{
    ble_advertising_t * const p_advertising = ble_adv_instance_ptr_get();

    ret_code_t err_code = nfc_ble_pair_init(p_advertising, (nfc_pairing_mode_t)NFC_PAIRING_MODE);
    APP_ERROR_CHECK(err_code);
}


/**@brief Application main function.
 */
void transport_ble_init(void)
{
    bool erase_bonds;

    log_init();
    timer_init();
    buttons_leds_init(&erase_bonds);
    power_management_init();
    ble_stack_init();
    gap_params_init();

    // Set BLE device name.
    ble_set_device_name(DEVICE_NAME);

    gatt_init();
    services_init();
    advertising_init();
    conn_params_init();
    peer_manager_init(erase_bonds);
    if (erase_bonds)
    {
        NRF_LOG_INFO("Bonds erased!");
    }
    nfc_pairing_init();

    // Start execution.
    NRF_LOG_INFO("NFC Connection Handover BLE peripheral device example started.");
}

in3_ret_t transport_ble(char **urls, int urls_len, char *payload, in3_response_t *result) {
  return _transport_ble(urls, urls_len, payload, result);
}

int transport_connected() {
  return _transport_connected();
}

/**@brief Callback function for asserts in the SoftDevice.
 *
 * @details This function will be called in case of an assert in the SoftDevice.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyze
 *          how your product is supposed to react in case of Assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in] line_num   Line number of the failing ASSERT call.
 * @param[in] file_name  File name of the failing ASSERT call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}
