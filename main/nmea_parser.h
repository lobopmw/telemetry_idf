// nmea_parser.h
#ifndef NMEA_PARSER_H
#define NMEA_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

// Estrutura para armazenar dados GPS
typedef struct {
    char latitude[16];
    char longitude[16];
    char date[16];
    char time[16];
} gps_data_t;

// Função para analisar a sentença NMEA
void parse_gps_data(const char *nmea_sentence, gps_data_t *gps_data);

// Função para obter dados GPS do UART
void get_gps_data(gps_data_t *gps_data);

#ifdef __cplusplus
}
#endif

#endif // NMEA_PARSER_H