idf_component_register(
    SRCS 
        "main.cpp"
        "./src/gps_parser.cpp"
        "./src/hardware.cpp"
        "./src/modem.cpp"  
        "./src/wifi_init_.cpp"      
    INCLUDE_DIRS      
        "./include"
        "."
    REQUIRES 
        driver
        freertos
        nvs_flash
        esp_event
        esp_wifi
        
)
