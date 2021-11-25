#ifndef main_h
#define main_h

#include <WiFi.h>

#ifdef __cplusplus
extern "C"{
#endif

#define WORK            (1)
#define HOME            (0)

#if WORK == 1
    // Имя и пароль точки доступа
    const char* ssid     = "QUALITET 2.4G";
    const char* password = "291309856";
    // Задаем статический IP-адрес: Qualitet
    IPAddress local_IP(192, 168, 0, 50);
    // Задаем IP-адрес сетевого шлюза:
    IPAddress gateway(192, 168, 0, 1);
#endif
#if HOME == 1
    const char* ssid     = "domik";
    const char* password = "2363423634";
    IPAddress local_IP(192, 168, 1, 50);
    IPAddress gateway(192, 168, 1, 1);
#endif
// Задаем номер порта для веб-сервера («80»):
WiFiServer server(80);
// Переменная для хранения HTTP-запроса:
String header;
IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   
IPAddress secondaryDNS(8, 8, 4, 4); 


}
#endif