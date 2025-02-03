# Controle de Números em Matriz de LEDs WS2812

Este projeto implementa um sistema de controle para exibição de números em uma matriz de LEDs WS2812 (5x5) utilizando um Raspberry Pi Pico W. O código foi desenvolvido em C e faz uso de interrupções para incrementar e decrementar os números exibidos. A implementação foi realizada na plataforma educacional BitDogLab.

## 🚀 Funcionalidades
- Exibição de números de 0 a 9 na matriz de LEDs.
- Controle do número exibido por meio de dois botões físicos.
- Indicação de funcionamento por meio de um LED vermelho piscante.
- É válido ressaltar a utilização de deboucing via software.

## 📌 Hardware Necessário
- Raspberry Pi Pico W RP2040
- Matriz de LEDs WS2812 (5x5)
- 2 botões (para controle de números)
- 1 LED vermelho (indicação de funcionamento)
- Resistores pull-up para os botões

## 🛠️ Configuração do Projeto
1. Clone este repositório:
   ```sh
   git clone https://github.com/VicGBiel/tarefa_interrupcao
   ```
2. Compile o código utilizando o SDK do Raspberry Pi Pico.
3. Faça o upload do código para a plataforma BitDogLab.

## 📝 Código
O código principal está no arquivo `tarefa_interrupção.c`. Ele inicializa os pinos GPIO, configura a comunicação com os LEDs e gerencia as interrupções para mudança de números na matriz de LEDs.

## 🎮 Controles
- **Botão A (Pino 5):** Incrementa o número exibido.
- **Botão B (Pino 6):** Decrementa o número exibido.

## 📺 Demonstração
Assista ao funcionamento do projeto no vídeo abaixo:

[![Demonstração do Projeto]](https://youtu.be/AXcYIkYAAho)

---
Criado por Victor Gabriel Guimarães Lopes 