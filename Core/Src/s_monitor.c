/*
 * s_monitor.c
 *
 *  Created on: Jan 24, 2022
 *      Author: hugin
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "circular_queue.h"
#include "main.h"
#include "stm32f1xx_hal.h"
#include "s_monitor.h"
#include "cwlibx.h"



extern UART_HandleTypeDef huart1;
#define USART_PORT huart1.Instance

uint8_t cmd[CMD_BUF_SIZE];
uint8_t ans[ANS_BUF_SIZE];
queue * Queue;
uint32_t size;
bool new_cmd;

void monitor_send_string(uint8_t *data) {

	HAL_UART_Transmit(&huart1, (uint8_t*) data, strlen(data), 100);

}

void monitor_check_cmd(char *cmd, uint32_t size) {
	if (strncmp(cmd, "help", 4) == 0) {

		snprintf((char*) ans, CMD_BUF_SIZE,
				"aqui eu deveria imprimir toda a tabela de comandos\r\n");
		monitor_send_string(ans);
	}
	//é um comando?
	else if (cmd[0] == 254) {

		if (cmd[1] == LCD_FIRMWARE_NUMBER) {
			snprintf((char*) ans, CMD_BUF_SIZE, "Firmware Version: %s\r\n",
			model_version);
			monitor_send_string(ans);
		} else if (cmd[1] == LCD_MODEL_NUMBER) {
			snprintf((char*) ans, CMD_BUF_SIZE, "Model Version: %s\r\n",
			fw_version);
			monitor_send_string(ans);
		}

		else if (cmd[1] == LCD_SOFT_RESET) {
			lcd_soft_reset();
		}

		else if (cmd[1] == LCD_BACKLIGHT_ON) {
			enable_backlight(true);
		}

		else if (cmd[1] == LCD_BACKLIGHT_OFF) {
			enable_backlight(false);
		}

		else if (cmd[1] == LCD_INVERSE_TEXT_ON) {
			inverse_text(true);
		}

		else if (cmd[1] == LCD_INVERSE_TEXT_OFF) {
			inverse_text(false);
		}

		else if (cmd[1] == LCD_CLEAR) {
			clear_display();
		}
//		para os 2 comandos a seguir é necessário inverter as coordenadas pois o display tem o eixo de coordenadas com o hardware em posição retrato
		else if (cmd[1] == LCD_PUT_PIXEL) {
			put_pixel(cmd[3], cmd[2]);
		}

		else if (cmd[1] == LCD_CLEAR_PIXEL) {
			erase_pixel(cmd[3], cmd[2]);
		}

		else if (cmd[1] == LCD_WRAP_ENABLE) {
			//não achei comando definido talvez eu tenha que contar char na string e mudar o cursor eu mesmo...getStrWidth pode ajudar
//			snprintf((char*) ans, CMD_BUF_SIZE, "n sei ainda.");
//			monitor_send_string(ans);
				auto_line_wrap(true);

		}

		else if (cmd[1] == LCD_WRAP_DISABLE) {
			//não achei comando definido talvez eu tenha que contar char na string e mudar o cursor eu mesmo...getStrWidth pode ajudar
//			snprintf((char*) ans, CMD_BUF_SIZE, "n sei ainda.");
//			monitor_send_string(ans);

				auto_line_wrap(false);

		}

		else if (cmd[1] == LCD_SET_INSERT) {
			text_insertion_point(cmd[2], cmd[3]);
		}

		else if (cmd[1] == LCD_HOME_INSERT) {
			text_insertion_point(0, 0);
		}

		else if (cmd[1] == LCD_READ_GPI) {
			if(cmd[2] == '0'){
				snprintf((char*) ans, CMD_BUF_SIZE, "%d.%d V\r\n",ddp[0],ddp[1]);
				monitor_send_string(ans);
			}
		}

		else if (cmd[1] == LCD_DISPLAY_SPLASH) {
				disp_splash();
		}

		else if (cmd[1] == LCD_DEF_THICK_VERTICAL_BAR) {
			 def_thick_v_bar();
		}

		else if (cmd[1] == LCD_DEF_THIN_VERTICAL_BAR) {
			 def_thin_v_bar();

		}

		else if (cmd[1] == LCD_DRAW_VERTICAL_BAR_GRAPH) {
			 draw_v_bar_graph(cmd[2], cmd[3]);
		}

		else if (cmd[1] == LCD_DRAW_VERTICAL_BAR_GRAPH) {

			erase_v_bar_graph(cmd[2], cmd[3]);
		}

		else if (cmd[1] == LCD_DRAW_HORIZONTAL_BAR_GRAPH ) {
			 draw_h_bar_graph(cmd[2], cmd[3] ,cmd[4]);
		}

	}
	//se não era comando, tenta escrever na tela
	else {
		lcd_print(cmd);
	}
}

void monitor_irq_handler(void) {
	uint8_t c;
	uint32_t sr;
	USART_TypeDef *h = USART_PORT;

	sr = h->SR;
	while (sr & (UART_FLAG_ORE | UART_FLAG_PE | UART_FLAG_FE | UART_FLAG_NE)) {
		sr = h->SR;
		c = h->DR;
	}

	if (sr & UART_FLAG_RXNE) {
		c = h->DR;

		if (c == 253) {
			cmd[size] = 253;
			enQueue(Queue, c);
			new_cmd = true;
		}

		else if (!new_cmd) {
			cmd[size] = c;
			enQueue(Queue, c);
			size++;

			if (size >= CMD_BUF_SIZE)
				size = 0;
		}
	}
	monitor_interrupt();
}

monitor_interrupt(){
	if (new_cmd) {
		monitor_check_cmd((char*) cmd, size);
		size = 0;
		new_cmd = false;
	}
}

void serial_begin(void) {
	size = 0;
	new_cmd = false;
	Queue =  createQueue(CMD_BUF_SIZE);

	clear_display();
	test_font();
	clear_display();
	put_cursor();

	// enabling interrupts for errors
	//   (Frame error, noise error, overrun error)
	huart1.Instance->CR3 |= USART_CR3_EIE;
	// enabling interrupt for parity errors and rx
	huart1.Instance->CR1 |= USART_CR1_PEIE | USART_CR1_RXNEIE;

	HAL_NVIC_SetPriority(USART1_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
	HAL_NVIC_ClearPendingIRQ(USART1_IRQn);

	//mensagem de inicialização
	snprintf((char*) ans, CMD_BUF_SIZE, "hora do show!\r\n");
	monitor_send_string(ans);

	monitor_interrupt();
}
