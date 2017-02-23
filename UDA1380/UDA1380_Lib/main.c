#include <math.h>
#include "uda1380.h"
#include "arm_math.h"
#include "fdacoefs.h"
#include "fdacoefs2.h"

#include "wavetable.h"

// Os parametros abaixo estao nos valores minimos para reduzir latência e gasto de memória
#define QUEUE_LEN 2048
#define BLOCK_SIZE 64
#define SAMPLERATE 48000

#define WT_LENGHT 1024 			// Tamanho da wavetable (wavetable.h)
// Tipos de onda dos osciladores
typedef enum {
	square = 0, triangle, sawtooth, distosine, sine, cosine
} Wave_form;

/*Variáveis Globais*/

int16_t buf_in[QUEUE_LEN], buf_out[QUEUE_LEN];

float32_t buf_mono[BLOCK_SIZE];

Wave_form wave_form; // Tipos de onda

//teste
float32_t signal = 0;
uint16_t pWavetable_osc1 = 0;
float incr_osc1 = WT_LENGHT / SAMPLERATE;

float32_t signal2 = 0;
uint16_t pWavetable_osc2 = 0;

uint8_t mult_osc1 = 1, mult_osc2 = 1;

float32_t osc_1(uint8_t forma_onda) {

	//float_t signal = 0;
	//static uint16_t pWavetable_osc1;

	switch (forma_onda) {
	case square:

		signal = squarewave[pWavetable_osc1];
		break;

	case triangle:
		signal = trianglewave[pWavetable_osc1];
		break;

	case sawtooth:
		signal = sawtoothwave[pWavetable_osc1];
		break;

	case distosine:
		signal = distosinewave[pWavetable_osc1];
		break;

	case sine:
		signal = sinewave[pWavetable_osc1];
		break;

	case cosine:
		signal = cosinewave[pWavetable_osc1];
		break;

	}

		pWavetable_osc1 = (pWavetable_osc1 + mult_osc1) % WT_LENGHT;

	// atualização ponteiros
	/*
	 pWavetable_osc1 = pWavetable_osc1 + incr_osc1;

	if (pWavetable_osc1 > WT_LENGHT) {
		pWavetable_osc1 = pWavetable_osc1 - WT_LENGHT;
	}
	*/

	return (signal);
}

float32_t osc_2(uint8_t forma_onda) {

	//float_t signal = 0;
	//static uint16_t pWavetable_osc1;

	switch (forma_onda) {
	case square:
		signal2 = squarewave[pWavetable_osc2];
		break;

	case triangle:
		signal2 = trianglewave[pWavetable_osc2];
		break;

	case sawtooth:
		signal2 = sawtoothwave[pWavetable_osc2];
		break;

	case distosine:
		signal2 = distosinewave[pWavetable_osc2];
		break;

	case sine:
		signal2 = sinewave[pWavetable_osc2];
		break;

	case cosine:
		signal2 = cosinewave[pWavetable_osc2];
		break;

	}

	pWavetable_osc2 = (pWavetable_osc2 + mult_osc2) % WT_LENGHT;

	return (signal2);
}

uint16_t a, b = 0;
float_t da,db;

volatile uint8_t iBuff_mono = 0;
void processamento(void) {

	while (1) {

		// osciladores
		mult_osc1 = 4;
		mult_osc2 = 1;
//----------------------------- interpolação precisa arrumar
		a = (int) pWavetable_osc1;
		da = pWavetable_osc1 - a;
		b = a + 1;
		db = b - pWavetable_osc1;

		if(b==WT_LENGHT) b=0;
//------------------------------

		if (iBuff_mono < BLOCK_SIZE) {

			buf_mono[iBuff_mono] = osc_1(sine);

			iBuff_mono++;

		} else {
			iBuff_mono = 0;

			UDA1380_SendSamplesFloatMono(buf_mono, BLOCK_SIZE);
		}
	}
}

int main(void) {
	UDA1380_InitTypeDef UDA1380_InitStructure;

	UDA1380_StructInit(&UDA1380_InitStructure);
	UDA1380_InitStructure.UDA1380_Queue_Length = QUEUE_LEN;
	UDA1380_InitStructure.UDA1380_Buffer_In = buf_in;
	UDA1380_InitStructure.UDA1380_Buffer_Out = buf_out;
	UDA1380_InitStructure.UDA1380_Callback = processamento;
	UDA1380_InitStructure.UDA1380_DAC_Enable = ENABLE;
	UDA1380_InitStructure.UDA1380_ADC_Enable = DISABLE;
	UDA1380_Init(&UDA1380_InitStructure);

	while (1)
		;
}
