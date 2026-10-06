#pragma once

// globally tracked task handles
extern TaskHandle_t xDs18b20Task;
extern TaskHandle_t xZigbeeTask;

void ds18b20_scan_bus(void);
void ds18b20_task(void *pvParameters);
void esp_zb_task(void *pvParameters);
