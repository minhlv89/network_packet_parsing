#pragma once
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>

typedef struct {
	uint8_t subsys_id;
	uint8_t compo_id;
	float temperature;
} CompoTlm_t;