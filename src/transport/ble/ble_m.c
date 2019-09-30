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

#include "ble_m.h"
#include "nordic_common.h"
#include "nrf_sdh.h"
#include "nrf_sdh_soc.h"
#include "nrf_sdh_ble.h"
#include "ble_conn_params.h"
#include "ble_conn_state.h"
#include "peer_manager.h"
#include "ble_advdata.h"
#include "app_timer.h"
#include "bsp.h"
#include "sdk_errors.h"
#include "bsp_btn_ble.h"
#include "fds.h"
#include "nfc_ble_pair_lib.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_qwr.h"

#include "in3_ble_transport.h"
#include "debug.h"
#include "nrf_delay.h"

#include "ble_hci.h"
#include "ble_nus.h"
#include "app_util_platform.h"
#include "bsp_btn_ble.h"

#define NRF_LOG_MODULE_NAME BLE_M
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();

#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(100, UNIT_1_25_MS)            /**< Minimum acceptable connection interval (0.1 seconds). */
#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(200, UNIT_1_25_MS)            /**< Maximum acceptable connection interval (0.2 second). */
#define SLAVE_LATENCY                   0                                           /**< Slave latency. */
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(4000, UNIT_10_MS)             /**< Connection supervisory timeout (4 seconds). */

#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(5000)                       /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(30000)                      /**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT    3                                           /**< Number of attempts before giving up the connection parameter negotiation. */

#define MANUFACTURER_NAME               "NordicSemiconductor"                       /**< Manufacturer. Will be passed to Device Information Service. */

#define APP_BLE_OBSERVER_PRIO           3                                           /**< Application's BLE observer priority. You shouldn't need to modify this value. */
#define APP_BLE_CONN_CFG_TAG            1                                           /**< A tag for a BLE stack configuration. */

#define APP_ADV_INTERVAL                300                                         /**< The advertising interval (in units of 0.625 ms. This value corresponds to 187.5 ms). */
#define APP_ADV_DURATION                18000                                       /**< The advertising duration (180 seconds) in units of 10 milliseconds. */

#define NUS_SERVICE_UUID_TYPE           BLE_UUID_TYPE_VENDOR_BEGIN                  /**< UUID type for the Nordic UART Service (vendor specific). */

//define UART pins for PCA100059
#define RX_PIN_NUMBER NRF_GPIO_PIN_MAP(0,31)
#define TX_PIN_NUMBER NRF_GPIO_PIN_MAP(0,29)
#define RTS_PIN_NUMBER NRF_GPIO_PIN_MAP(1,13)
#define CTS_PIN_NUMBER NRF_GPIO_PIN_MAP(1,15)

#define MAX_RESPONSE_LEN 65536
#define MAX_REQUEST_LEN 2048
// 50*100 milliseconds
#define BLE_IN3_TIMEOUT 600

BLE_NUS_DEF(m_nus, NRF_SDH_BLE_TOTAL_LINK_COUNT);                                   /**< BLE NUS service instance. */
NRF_BLE_GATT_DEF(m_gatt);                                                           /**< GATT module instance. */
NRF_BLE_QWR_DEF(m_qwr);                                                             /**< Context for the Queued Write module.*/
BLE_ADVERTISING_DEF(m_advertising);                                                 /**< Advertising module instance. */

static bool ble_connected = false;
static uint8_t response[MAX_RESPONSE_LEN];
static int response_len = 0;
static bool response_received = false;
static bool start_storing_response = false;
static int tx_received_length = 0;

static uint16_t                m_conn_handle  = BLE_CONN_HANDLE_INVALID;            /**< Handle of the current connection. */
static uint16_t   m_ble_nus_max_data_len = BLE_GATT_ATT_MTU_DEFAULT - 3;            /**< Maximum length of data (in bytes) that can be transmitted to the peer by the Nordic UART service module. */
static ble_gap_conn_sec_mode_t m_sec_mode;                                          /**< Holds GAP security requirements. */
static ble_advdata_t           m_advdata;                                           /**< Variable holding advertised data. */

static ble_uuid_t m_adv_uuids[] =                                                   /**< Universally unique service identifiers. */
{
    {BLE_UUID_DEVICE_INFORMATION_SERVICE, BLE_UUID_TYPE_BLE}
};


/**@brief Function for handling BLE events.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 * @param[in]   p_context   Unused.
 */
static void ble_evt_handler(ble_evt_t const * p_ble_evt, void * p_context)
{
    ret_code_t err_code = NRF_SUCCESS;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_DISCONNECTED:
            NRF_LOG_INFO("Disonnected");
            err_code = bsp_indication_set(BSP_INDICATE_IDLE);
            ble_connected = false;
            APP_ERROR_CHECK(err_code);
            m_conn_handle = BLE_CONN_HANDLE_INVALID;
            break;

        case BLE_GAP_EVT_CONNECTED:
            NRF_LOG_INFO("Connected.");
            err_code = bsp_indication_set(BSP_INDICATE_CONNECTED);
            ble_connected = true;
            APP_ERROR_CHECK(err_code);
            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            err_code = nrf_ble_qwr_conn_handle_assign(&m_qwr, m_conn_handle);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GAP_EVT_PHY_UPDATE_REQUEST:
        {
            NRF_LOG_DEBUG("PHY update request.");
            ble_gap_phys_t const phys =
            {
                .rx_phys = BLE_GAP_PHY_AUTO,
                .tx_phys = BLE_GAP_PHY_AUTO,
            };
            err_code = sd_ble_gap_phy_update(p_ble_evt->evt.gap_evt.conn_handle, &phys);
            APP_ERROR_CHECK(err_code);
        } break;

        case BLE_GATTC_EVT_TIMEOUT:
            // Disconnect on GATT Client timeout event.
            NRF_LOG_DEBUG("GATT Client Timeout.");
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gattc_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GATTS_EVT_TIMEOUT:
            // Disconnect on GATT Server timeout event.
            NRF_LOG_DEBUG("GATT Server Timeout.");
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gatts_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GAP_EVT_AUTH_STATUS:
            NRF_LOG_INFO("BLE_GAP_EVT_AUTH_STATUS");
            if (p_ble_evt->evt.gap_evt.params.auth_status.auth_status ==
                BLE_GAP_SEC_STATUS_SUCCESS)
            {
                NRF_LOG_INFO("Authorization succeeded!");
            }
            else
            {
                NRF_LOG_INFO("Authorization failed with code: %u!",
                             p_ble_evt->evt.gap_evt.params.auth_status.auth_status);
            }
            break;

        case BLE_GAP_EVT_CONN_SEC_UPDATE:
            NRF_LOG_INFO("BLE_GAP_EVT_CONN_SEC_UPDATE");
            NRF_LOG_INFO("Security mode: %u",
                         p_ble_evt->evt.gap_evt.params.conn_sec_update.conn_sec.sec_mode.lv);

            break;

        default:
            // No implementation needed.
            break;
    }

    UNUSED_VARIABLE(err_code);
}


/**@brief   Function for handling the Connection Parameters Module.
 *
 * @details This function will be called for all events in the Connection Parameters Module which
 *          are passed to the application.
 *          @note All this function does is to disconnect. This could have been done by simply
 *                setting the disconnect_on_fail config parameter, but instead we use the event
 *                handler mechanism to demonstrate its use.
 *
 * @param[in] p_evt  Event received from the Connection Parameters Module.
 */
static void on_conn_params_evt(ble_conn_params_evt_t * p_evt)
{
    ret_code_t err_code;

    if (p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
    {
        err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
        APP_ERROR_CHECK(err_code);
    }
}


/**@brief Function for handling a Connection Parameters error.
 *
 * @param[in] nrf_error  Error code containing information about what went wrong.
 */
static void conn_params_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}


/**@brief Function for handling Queued Write Module errors.
 *
 * @details A pointer to this function will be passed to each service which may need to inform the
 *          application about an error.
 *
 * @param[in]   nrf_error   Error code containing information about what went wrong.
 */
static void nrf_qwr_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}

/**@brief Function for handling the data from the Nordic UART Service.
 *
 * @details This function will process the data received from the Nordic UART BLE Service and send
 *          it to the UART module.
 *
 * @param[in] p_evt       Nordic UART Service event.
 */
/**@snippet [Handling the data received over BLE] */
static void nus_data_handler(ble_nus_evt_t * p_evt)
{

    if (p_evt->type == BLE_NUS_EVT_RX_DATA)
    {
        NRF_LOG_DEBUG("Received data from BLE NUS. Writing data on UART.");
        // NRF_LOG_HEXDUMP_DEBUG(p_evt->params.rx_data.p_data, p_evt->params.rx_data.length);

        if (start_storing_response) {
          memcpy(response + response_len, p_evt->params.rx_data.p_data, p_evt->params.rx_data.length);
          response_len += p_evt->params.rx_data.length;
          dbg_log("Got a chunk of response, total response length: %d\n", response_len);
        }
        else if (!start_storing_response && (strstr(p_evt->params.rx_data.p_data, "res:") != NULL)) {
          //do this in a better way
          char length_buffer[20];
          memcpy(length_buffer, &(p_evt->params.rx_data.p_data[4]), p_evt->params.rx_data.length - 4);
          tx_received_length = atoi(length_buffer);
          start_storing_response = true;
          dbg_log("Incoming response of length: %d\n", tx_received_length);
        }

        if (response_len == tx_received_length) {
          start_storing_response = false;
          tx_received_length = 0;
          response_received = true;
          dbg_log("Response Received\n");
        }
        // do
        // {
        //     err_code = app_uart_put(p_evt->params.rx_data.p_data[i]);
        //     if ((err_code != NRF_SUCCESS) && (err_code != NRF_ERROR_BUSY))
        //     {
        //         NRF_LOG_ERROR("Failed receiving NUS message. Error 0x%x. ", err_code);
        //         APP_ERROR_CHECK(err_code);
        //     }
        // } while (err_code == NRF_ERROR_BUSY);
        // if (p_evt->params.rx_data.p_data[p_evt->params.rx_data.length - 1] == '\r')
        // {
        //     while (app_uart_put('\n') == NRF_ERROR_BUSY);
        // }
    }

}


/**@brief   Function for handling advertising events.
 *
 * @details This function will be called for advertising events which are passed to the application.
 *
 * @param[in] ble_adv_evt  Advertising event.
 */
static void on_adv_evt(ble_adv_evt_t ble_adv_evt)
{
    ret_code_t err_code;

    switch (ble_adv_evt)
    {
        case BLE_ADV_EVT_FAST:
            NRF_LOG_INFO("Fast advertising.");
            err_code = bsp_indication_set(BSP_INDICATE_ADVERTISING);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_ADV_EVT_IDLE:
            NRF_LOG_INFO("Advertising stopped.");
            err_code = bsp_indication_set(BSP_INDICATE_IDLE);
            APP_ERROR_CHECK(err_code);
            break;

        default:
            break;
    }
}

ble_advertising_t * ble_adv_instance_ptr_get(void)
{
    return &m_advertising;
}

/**@brief Function for initializing the BLE stack.
 *
 * @details Initializes the SoftDevice and the BLE event interrupt.
 */
void ble_stack_init(void)
{
    ret_code_t err_code;

    err_code = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err_code);

    // Configure the BLE stack using the default settings.
    // Fetch the start address of the application RAM.
    uint32_t ram_start = 0;
    err_code = nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &ram_start);
    APP_ERROR_CHECK(err_code);

    // Enable BLE stack.
    err_code = nrf_sdh_ble_enable(&ram_start);
    APP_ERROR_CHECK(err_code);

    // Register a handler for BLE events.
    NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO, ble_evt_handler, NULL);
}


void advertising_init(void)
{
    ret_code_t err_code;
    ble_advertising_init_t init;

    memset(&init, 0, sizeof(init));

    // Only set up adv_data. Options will be set depending on if advertising will be enabled or not.
    m_advdata.name_type               = BLE_ADVDATA_FULL_NAME;
    m_advdata.include_appearance      = true;
    m_advdata.flags                   = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
    m_advdata.uuids_complete.uuid_cnt = sizeof(m_adv_uuids) / sizeof(m_adv_uuids[0]);
    m_advdata.uuids_complete.p_uuids  = m_adv_uuids;

    init.advdata = m_advdata;

    init.config.ble_adv_fast_enabled           = true;
    init.config.ble_adv_fast_interval          = APP_ADV_INTERVAL;
    init.config.ble_adv_fast_timeout           = APP_ADV_DURATION;
    init.config.ble_adv_whitelist_enabled      = false;
    init.config.ble_adv_on_disconnect_disabled = true;

    init.evt_handler = on_adv_evt;

    err_code = ble_advertising_init(&m_advertising, &init);
    APP_ERROR_CHECK(err_code);

    ble_advertising_conn_cfg_tag_set(&m_advertising, APP_BLE_CONN_CFG_TAG);
}


void ble_disconnect(void)
{
    ret_code_t err_code;

    if (m_conn_handle != BLE_CONN_HANDLE_INVALID)
    {
        err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
        APP_ERROR_CHECK(err_code);
    }
}


void ble_set_device_name(char const * device_name)
{
    ret_code_t err_code;

    err_code = sd_ble_gap_device_name_set(&m_sec_mode,
                                          (uint8_t const *)device_name,
                                          strlen(device_name));
    APP_ERROR_CHECK(err_code);
}


void gap_params_init(void)
{
    ret_code_t            err_code;
    ble_gap_conn_params_t gap_conn_params;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&m_sec_mode);

    err_code = sd_ble_gap_appearance_set(BLE_APPEARANCE_UNKNOWN);
    APP_ERROR_CHECK(err_code);

    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for handling events from the GATT library. */
void gatt_evt_handler(nrf_ble_gatt_t * p_gatt, nrf_ble_gatt_evt_t const * p_evt)
{
    if ((m_conn_handle == p_evt->conn_handle) && (p_evt->evt_id == NRF_BLE_GATT_EVT_ATT_MTU_UPDATED))
    {
        m_ble_nus_max_data_len = p_evt->params.att_mtu_effective - OPCODE_LENGTH - HANDLE_LENGTH;
        NRF_LOG_INFO("Data len is set to 0x%X(%d)", m_ble_nus_max_data_len, m_ble_nus_max_data_len);
    }
    NRF_LOG_DEBUG("ATT MTU exchange completed. central 0x%x peripheral 0x%x",
                  p_gatt->att_mtu_desired_central,
                  p_gatt->att_mtu_desired_periph);
}


void gatt_init()
{
    ret_code_t err_code = nrf_ble_gatt_init(&m_gatt, NULL);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_ble_gatt_att_mtu_periph_set(&m_gatt, NRF_SDH_BLE_GATT_MAX_MTU_SIZE);
    APP_ERROR_CHECK(err_code);
}

void services_init(void)
{
    uint32_t           err_code;
    ble_nus_init_t     nus_init;
    nrf_ble_qwr_init_t qwr_init = {0};

    // Initialize Queued Write Module.
    qwr_init.error_handler = nrf_qwr_error_handler;

    err_code = nrf_ble_qwr_init(&m_qwr, &qwr_init);
    APP_ERROR_CHECK(err_code);

    // Initialize NUS.
    memset(&nus_init, 0, sizeof(nus_init));

    nus_init.data_handler = nus_data_handler;

    err_code = ble_nus_init(&m_nus, &nus_init);
    APP_ERROR_CHECK(err_code);
}


void conn_params_init(void)
{
    ret_code_t             err_code;
    ble_conn_params_init_t cp_init;

    memset(&cp_init, 0, sizeof(cp_init));

    cp_init.p_conn_params                  = NULL;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail             = false;
    cp_init.evt_handler                    = on_conn_params_evt;
    cp_init.error_handler                  = conn_params_error_handler;

    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}

int _transport_connected() {
  if (ble_connected) {
    return 1;
  }

  return 0;
}

in3_ret_t _transport_ble(char **urls, int urls_len, char *payload, in3_response_t *result) {
  uint32_t ble_ret;
  int timeout = 0;

  if (!ble_connected) {
    dbg_log ("Not connected to any device!\n");
    return IN3_ETRANS;
  }

  uint8_t ble_payload[MAX_REQUEST_LEN];
  uint16_t url_len = 0;
  uint16_t payload_len = 0;
  uint16_t total_len = 0;
  uint16_t transmit_message_len = m_ble_nus_max_data_len;

  //TODO: Asynchronize the requests if possible, because bluetooth supports it.
  for (int i=0; i<urls_len; i++) {

    url_len = strlen(urls[i]);
    payload_len = strlen(payload);
    total_len = url_len + payload_len + 1;

    memcpy(ble_payload, urls[i], url_len);
    memcpy(ble_payload + url_len, ";", 1);
    memcpy(ble_payload + url_len + 1, payload, payload_len);

    response_received = false;
    response_len = 0;
    transmit_message_len = m_ble_nus_max_data_len;

    for(int j=0; j<total_len; j+=m_ble_nus_max_data_len) {
      if ((total_len - j) < m_ble_nus_max_data_len) {
        transmit_message_len = total_len - j;
      }
      ble_ret = ble_nus_data_send(&m_nus, ble_payload + j, &(transmit_message_len), m_conn_handle);
    }

    if (ble_ret != NRF_SUCCESS) {
      dbg_log("Could not relay the payload via bluetooth, Error Code: 0x%x\n", ble_ret);
      memset(ble_payload, 0, MAX_REQUEST_LEN);
      return IN3_ETRANS;
    }

    memset(ble_payload, 0, MAX_REQUEST_LEN);

    while(!response_received) {

      if (timeout >= BLE_IN3_TIMEOUT) {
        dbg_log("REQUEST TIMED OUT\n");
        start_storing_response = false;
        tx_received_length = 0;
        memset(response, 0, response_len);
        return IN3_ETRANS;
      }

      timeout = timeout + 1;
      nrf_delay_ms(100);
    }

    if(response_received) {
      dbg_log("Response Length: %d\n", response_len);
      response_received = false;
      sb_add_range(&(result[i].result), response, 0, response_len);
      response_len = 0;
      memset(response, 0, response_len);
    }
  }
  dbg_log("Sending OK\n");
  return IN3_OK;
}

/**@brief Function for putting the chip into sleep mode.
 *
 * @note This function will not return.
 */
static void sleep_mode_enter(void)
{
    uint32_t err_code = bsp_indication_set(BSP_INDICATE_IDLE);
    APP_ERROR_CHECK(err_code);

    // Prepare wakeup buttons.
    err_code = bsp_btn_ble_sleep_mode_prepare();
    APP_ERROR_CHECK(err_code);

    // Go to system-off mode (this function will not return; wakeup will cause a reset).
    err_code = sd_power_system_off();
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling events from the BSP module.
 *
 * @param[in]   event   Event generated by button press.
 */
static void bsp_event_handler(bsp_event_t event)
{
    uint32_t err_code;

    switch (event)
    {
        case BSP_EVENT_SLEEP:
            sleep_mode_enter();
            break;

        case BSP_EVENT_DISCONNECT:
            err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            if (err_code != NRF_ERROR_INVALID_STATE)
            {
                APP_ERROR_CHECK(err_code);
            }
            break;

        case BSP_EVENT_WHITELIST_OFF:
            if (m_conn_handle == BLE_CONN_HANDLE_INVALID)
            {
                err_code = ble_advertising_restart_without_whitelist(&m_advertising);
                if (err_code != NRF_ERROR_INVALID_STATE)
                {
                    APP_ERROR_CHECK(err_code);
                }
            }
            break;
        default:
            break;
    }
}

/**@brief Function for initializing buttons and leds.
 *
 * @param[out] p_erase_bonds  Will be true if the clear bonding button was pressed to wake the application up.
 */
void buttons_leds_init(bool * p_erase_bonds)
{
    bsp_event_t startup_event;

    uint32_t err_code = bsp_init(BSP_INIT_LEDS | BSP_INIT_BUTTONS, bsp_event_handler);
    APP_ERROR_CHECK(err_code);

    err_code = bsp_btn_ble_init(NULL, &startup_event);
    APP_ERROR_CHECK(err_code);

    *p_erase_bonds = (startup_event == BSP_EVENT_CLEAR_BONDING_DATA);
}
