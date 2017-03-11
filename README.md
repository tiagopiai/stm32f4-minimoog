# Sintetizador Subtrativo Monofônico de Áudio Baseado no MiniMoog.

Implementação em STM32F4-Discovery de um sintetizador baseado no Minimoog D,
utilizou-se também para realizar os controles das características e variáveis do sistema, uma interface implementada em MatLab em conjunto com uma comunicação via USART (RS232) para a tranferência de dados entre a aplicação de controle e o microcontrolador.

# Organização:

# Componentes:

## Diagrama de Blocos:
Diagrama de blocos similar ao apresentado nos Minimoogs clássicos. O diagrama foi levantado com o auxílio do professor Décio Gazzoni Filho.

## Osciladores:
Foram utilizados valores tabelados gerados através do MatLab para criar DDCs para a implementação dos osciladores. Utilizou-se também, uma interpolação linear para obter valores intermediários de frequências.

- **Limite (Range):**
Determina as notas de uma até cinco oitavas acima da tocada.
- **Amplitude (Volume):**
Controle da intensidade do som produzido pelo oscilador (0%-100%).
- **Forma de Onda (Waveform):**
Quadrada, triangular, dente de serra, senoidal e distosine.
- **Frequência (Frequency):**
Afinada até 5 tons acima ou abaixo da nota tocada.

## LFO (Amplitude):
- **Princípio:**
Opera uma modulação do volume do sinal de saída, utilizando um sinal modulante de baixa frequência.
- **Controles:**
Frequência do sinal modulante e intensidade do LFO.

## LFO (Frequência):
* **LFO na Frequência:**
É possível escolher uma faixa de frequência de atuação do LFO, a qual vai de 0,05Hz até 200Hz. Além disso, se o botão estiver no mínimo, a onda utilizada é triangular, e se estiver próximo ao máximo, a onda é quadrada.

## Geradores de Envelope (Amplitude):
- **Attack:**
Tempo de disparo da nota após  a tecla ser pressionada (1ms - 10s)
- **Decay:**
Tempo de passagem da amplitude máxima do attack o sustain (4ms - 35s)
- **Sustain:** 
Nível de amplitude em que a nota é mantida enquanto a tecla mantém-se pressionada (0% - 100%)

## Geradores de Envelope (Frequência):
- **Attack:**
Determina o tempo necessário para aumentar a frequência de corte do ajuste manual até o seu máximo (definida pelo AMOUNT OF CONTOUR)
- **Decay:**
Define o tempo necessário para que abaixe a frequência de corte do nível alcançado pelo estágio de ATTACK até o definido pelo SUSTAIN LEVEL
- **Sustain:**
Após os estágios de ATTACK e DECAY estiverem sido concluídos, a frequência de corte se manterá no nível determinado pelo SUSTAIN LEVEL enquanto uma nota for mantida

## Gerador de Ruído:
Utilizou-se o periférico RNG que possui semeação 99% analógica para criar o gerador de ruído.

## Filtro Passa Baixa (Biquad):
Foi implementado um filtro biquadrático de caráter passa baixa, utilizou-se 2 "buffers" com três elementos de memória cada, para a implementação de sua função de transferência em equação de diferenças. Onde são guardados as duas últimas amostras de sáidas do filtro e as duas últimas amostras entradas do filtro.

- **Fator Q (Emphasis):**
Controla a ressonância do sinal filtrado, acrescentando distorções
- **Frequência de Corte:**
Sempre uma oitava acima da nota tocada, podendo ser ajustada para cinco tons abaixo ou acima


## Interface Gráfica para Controle:




