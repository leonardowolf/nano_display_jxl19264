/*
 * s_monitor.h
 *
 *  Created on: Jan 24, 2022
 *      Author: hugin
 */

#ifndef INC_S_MONITOR_H_
#define INC_S_MONITOR_H_

void monitor_send_string(uint8_t *data);
void monitor_check_cmd(char *cmd, uint32_t size);
void monitor_irq_handler(void);
void monitor_interrupt(void);
void serial_begin(void);

#define CMD_BUF_SIZE 100
#define ANS_BUF_SIZE 100

#endif /* INC_S_MONITOR_H_ */
