#include "freertos/FreeRTOS.h"
#include "esp_zigbee_core.h"
#include "esp_log.h"

static const char *TAG = "zigbee";

void reportAttribute(uint8_t endpoint, uint16_t clusterID, uint16_t attributeID, void *value)
{
    // The Zigbee stack runs in its own task - any call into esp-zigbee-lib
    // from another task (like the sensor task) must be wrapped in
    // esp_zb_lock_acquire/release.
    esp_zb_lock_acquire(portMAX_DELAY);
    esp_zb_zcl_status_t status = esp_zb_zcl_set_attribute_val(endpoint, clusterID, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE, attributeID, value, false);
    esp_zb_lock_release();
    if (status != ESP_ZB_ZCL_STATUS_SUCCESS) {
        ESP_LOGW(TAG, "set_attribute_val cluster=0x%04x attr=0x%04x: status 0x%02x", clusterID, attributeID, status);
    }
}
