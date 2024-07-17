#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "mqtt_client.h"
#include "cJSON.h"
#include "driver/uart.h"
#include "nmea_parser.h"

#define WIFI_SSID "BATERA"
#define WIFI_PASS "drummer123!"
#define MQTT_BROKER "mqtt://192.168.10.23"
#define MQTT_USERNAME "trash_truck"
#define MQTT_PASSWORD "trash_truck_porto"
#define MQTT_CLIENT_ID "trash_truck"
#define MQTT_TOPIC "v1/devices/me/telemetry"

#define GPS_UART_PORT UART_NUM_1
#define GPS_UART_RX_PIN 16
#define GPS_UART_TX_PIN 17
#define GPS_UART_BAUD_RATE 9600

static const char *TAG = "GPS_MQTT";
esp_mqtt_client_handle_t mqtt_client;

static const int RX_BUF_SIZE = 2024;

void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        esp_wifi_connect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ESP_LOGI(TAG, "Got IP");
    }
}

void wifi_init() {
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, &instance_any_id);
    esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, &instance_got_ip);

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS
        }
    };
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
    esp_wifi_start();
}

esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event) {
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            esp_mqtt_client_subscribe(mqtt_client, MQTT_TOPIC, 0);
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED");
            break;
        default:
            break;
    }
    return ESP_OK;
}

void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%ld", base, (long)event_id);
    mqtt_event_handler_cb(event_data);
}

void mqtt_app_start() {
    const esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = MQTT_BROKER,
        .broker.address.port = 1883,
        .credentials.username = MQTT_USERNAME,
        .credentials.client_id = MQTT_CLIENT_ID,
        .credentials.authentication.password = MQTT_PASSWORD, 
    };

    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, mqtt_event_handler, mqtt_client);
    esp_mqtt_client_start(mqtt_client);
}

void init_gps() {
    const uart_config_t uart_config = {
        .baud_rate = GPS_UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    uart_driver_install(GPS_UART_PORT, 2048, 0, 0, NULL, 0);
    uart_param_config(GPS_UART_PORT, &uart_config);
    uart_set_pin(GPS_UART_PORT, GPS_UART_TX_PIN, GPS_UART_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

// void parse_gps_data(const char *nmea_sentence, char *latitude, char *longitude, char *date, char *time) {
//     nmea_parser_config_t config = NMEA_PARSER_CONFIG_DEFAULT();
//     nmea_parser_handle_t nmea_hdl = nmea_parser_init(&config);
//     nmea_event_data_t event = {0};

//     if (nmea_parser_parse(nmea_hdl, nmea_sentence, &event) == ESP_OK) {
//         sprintf(latitude, "%f", event.gps.latitude);
//         sprintf(longitude, "%f", event.gps.longitude);
//         sprintf(date, "%02d%02d%02d", event.date.day, event.date.month, event.date.year);
//         sprintf(time, "%02d%02d%02d", event.time.hour, event.time.minute, event.time.second);
//     } else {
//         ESP_LOGE(TAG, "Failed to parse NMEA sentence");
//     }

//     nmea_parser_deinit(nmea_hdl);
// }

// void get_gps_data(char *latitude, char *longitude, char *date, char *time) {
//     const int buffer_size = 1024;
//     char buffer[buffer_size];
//     int length = uart_read_bytes(GPS_UART_PORT, (uint8_t*)buffer, buffer_size, 100 / portTICK_PERIOD_MS);
//     if (length > 0) {
//         buffer[length] = '\0';
//         printf("\nLatitude %s\n", latitude);
//         parse_gps_data(buffer, latitude, longitude, date, time);
//     }
// }

void parse_gps_data(const char *nmea_sentence, gps_data_t *gps_data) {
    // Simulando análise da sentença NMEA para obter latitude, longitude, data e hora
    // Isso deve ser substituído por uma análise real da sentença NMEA
    printf("\nLatitude: %s\n", gps_data->latitude);
    sscanf(nmea_sentence, "Location: %[^,], %[^ ] Data/Time: %[^ ] %[^.]", 
           gps_data->latitude, gps_data->longitude, gps_data->date, gps_data->time);
}

// void get_gps_data(gps_data_t *gps_data) {
//     const int buffer_size = 1024;
//     char buffer[buffer_size];
//     int length = uart_read_bytes(GPS_UART_PORT, (uint8_t*)buffer, buffer_size, 100 / portTICK_PERIOD_MS);
//     printf("\nLength: %i\n", length);
//     if (length > 0) {
//         buffer[length] = '\0';
//         parse_gps_data(buffer, gps_data);
//     }
// }

static void get_gps(void *gps_data) {
     
    uint8_t* data = (uint8_t*)malloc(RX_BUF_SIZE+1);
    
    while(1){
            const int rxbytes = uart_read_bytes(GPS_UART_PORT, data, RX_BUF_SIZE, 1000 / portTICK_PERIOD_MS);
        
            if (rxbytes > 0) {
                data[rxbytes] = 0;
                uart_write_bytes(GPS_UART_PORT, (const char*)data, rxbytes);
            }
    vTaskDelay(100/portTICK_PERIOD_MS);
    }
    free(data);
}


void send_gps_data(void *pvParameters) {
    gps_data_t gps_data;

    while(1) {
        // get_gps_data(&gps_data);

        cJSON *root = cJSON_CreateObject();
        
        cJSON_AddStringToObject(root, "latitude", gps_data.latitude);
        cJSON_AddStringToObject(root, "longitude", gps_data.longitude);
        cJSON_AddStringToObject(root, "date", gps_data.date);
        cJSON_AddStringToObject(root, "time", gps_data.time);
        char *json_string = cJSON_Print(root);
        
        esp_mqtt_client_publish(mqtt_client, MQTT_TOPIC, json_string, 0, 1, 0);

        cJSON_Delete(root);
        free(json_string);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main() {
    esp_log_level_set("*", ESP_LOG_INFO);
    nvs_flash_init();
    wifi_init();
    mqtt_app_start();
    init_gps();

    // xTaskCreate(&send_gps_data, "send_gps_data", 4096, NULL, 5, NULL);
    xTaskCreate(&get_gps, "get_gps", 4096, NULL, 5, NULL);
}

