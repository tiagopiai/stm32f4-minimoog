#include <math.h>
#include "uda1380.h"
#include "arm_math.h"
#include "fdacoefs.h"
#include "fdacoefs2.h"

#include "wavetable.h"

// Os parametros abaixo estao nos valores tipicos para otimizar
// tempo de processamento e tolerar processamentos mais longos
#define QUEUE_LEN 2048
#define BLOCK_SIZE 64

// Os parametros abaixo estao nos valores minimos para reduzir latência e gasto de memória
//#define QUEUE_LEN 16
//#define BLOCK_SIZE 4

// Tipos de onda dos osciladores
typedef enum {
	square = 0, triangle, sawtooth, distosine, sine, cosine
} Wave_form;


#define WT_LENGHT 1024 			// Tamanho da wavetable (wavetable.h)




/*Variáveis Globais*/

int16_t buf_in[QUEUE_LEN], buf_out[QUEUE_LEN];

float32_t buf_mono[BLOCK_SIZE];

Wave_form wave_form; // Tipos de onda


//teste
float32_t signal = 0;
uint16_t pWavetable_osc1 = 0;


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

	pWavetable_osc1 = (pWavetable_osc1 + 1) % WT_LENGHT;

	return signal+1;
}

uint8_t iBuff_mono = 0;
void processamento(void) {

	//uint8_t iBuff_mono = 0;

	while (1) {
		//	UDA1380_ReceiveSamplesMono(buf_mono, BLOCK_SIZE);

		if (iBuff_mono < BLOCK_SIZE) {

			buf_mono[iBuff_mono] = osc_1(square);

			iBuff_mono ++;

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
	UDA1380_Init(&UDA1380_InitStructure);

	while (1)
		;
}
