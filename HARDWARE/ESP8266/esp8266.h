#ifndef _ESP_8266_
#define _ESP_8266_

#define ESP8266_RST PBout(0)

void esp8266_init(void);
void esp8266_rst(void);

#endif
