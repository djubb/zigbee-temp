#include "freertos/FreeRTOS.h"
#include "esp_zigbee_core.h"
#include "esp_log.h"

static const char *TAG = "zigbee";

// Updates the attribute's stored value and immediately unicasts an explicit
// Report Attributes command straight to the coordinator (0x0000). This is
// deliberate rather than relying on the stack's passive reporting engine
// (esp_zb_zcl_update_reporting_info): the min/max interval we configure
// there is only a *default* - ZHA's own Configure Reporting command sent
// during device interview can (and in practice does) override it with a
// much longer interval, so the only way to guarantee our own cadence is to
// push the report ourselves every cycle.
void reportAttribute(uint8_t endpoint, uint16_t clusterID, uint16_t attributeID, void *value)
{
    // The Zigbee stack runs in its own task - any call into esp-zigbee-lib
    // from another task (like the sensor task) must be wrapped in
    // esp_zb_lock_acquire/release.
    esp_zb_lock_acquire(portMAX_DELAY);
    esp_zb_zcl_status_t status = esp_zb_zcl_set_attribute_val(endpoint, clusterID, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE, attributeID, value, false);
    if (status == ESP_ZB_ZCL_STATUS_SUCCESS) {
        esp_zb_zcl_report_attr_cmd_t cmd = {
            .zcl_basic_cmd = {
                .dst_addr_u.addr_short = 0x0000,
                .dst_endpoint = endpoint,
                .src_endpoint = endpoint,
            },
            .address_mode = ESP_ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
            .clusterID = clusterID,
            .attributeID = attributeID,
        };
        esp_err_t err = esp_zb_zcl_report_attr_cmd_req(&cmd);
        if (err != ESP_OK) {
            ESP_LOGW(TAG, "report_attr_cmd_req cluster=0x%04x attr=0x%04x: %s", clusterID, attributeID, esp_err_to_name(err));
        }
    } else {
        ESP_LOGW(TAG, "set_attribute_val cluster=0x%04x attr=0x%04x: status 0x%02x", clusterID, attributeID, status);
    }
    esp_zb_lock_release();
}
