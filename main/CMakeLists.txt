idf_component_register(
    SRCS
    "main.c"
    "zigbee.c"
    "zigbee_task.c"
    "ds18b20_task.c"
    INCLUDE_DIRS "."
    REQUIRES "nvs_flash" "onewire_bus" "ds18b20"
)
