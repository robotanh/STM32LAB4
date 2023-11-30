#include "fsm.h"
#include "main.h"
#include <inttypes.h>

enum Command_state {
		Begin,
		Body
};
static enum Command_state command_state = Begin;
char strr[30];
uint32_t ADC_value;
enum Uart_state {
		Init,
		Wait,
		Nothing
};
static enum Uart_state uart_state = Nothing;
int flag =0;
uint8_t index_command = 0;
void command_parser_fsm (){
	switch (command_state){
	case Begin:
		if(buffer[curr_index_buffer]=='!') {
			curr_index_buffer++;
			index_command = 0;
			command_data[0] = '\0';
			command_flag = 0;
			command_state = Body;
			if(curr_index_buffer==30) curr_index_buffer=0;
		}
		break;
	case Body:
		if(buffer[curr_index_buffer]=='!'){
			//command_flag=0;
			//uart_state = Init;
			curr_index_buffer++;
			index_command = 0;
			command_data[0] = '\0';
			command_flag = 0;
			if(curr_index_buffer==30) curr_index_buffer=0;
			break;
		}
		if(buffer[curr_index_buffer]=='#') {

			command_data[index_command] = '\0';
			if(strcmp(command_data, "RST") == 0) {
				command_flag = 1;
				uart_state = Init;
				flag =1;
			}
			else if(strcmp(command_data,"OK")  == 0) {
				command_flag = 0;
				flag=0;
				uart_state = Nothing;
			}
			else{
				if(flag){
					uart_state = Wait;
				}
			}

			command_state = Begin;
		}
		else {

			command_data[index_command++] = buffer[curr_index_buffer];
		}
		curr_index_buffer++;
		if(curr_index_buffer==30) curr_index_buffer=0;
		break;
	}
}

void uart_communiation_fsm () {
	switch (uart_state){
	case Init:
		if(command_flag==1 && strcmp(command_data, "RST") == 0) {
			HAL_GPIO_TogglePin ( GPIOA , GPIO_PIN_5 ) ;
			//HAL_ADC_PollForConversion(&hadc1, 20);
			ADC_value = HAL_ADC_GetValue (&hadc1);
			sprintf( strr , "\r\n!ADC=%lu#\r\n", ADC_value );
			HAL_UART_Transmit (&huart2 , (uint8_t *) strr, strlen( strr), 20);
			command_flag = 0;
			uart_state = Wait;
			set_Timer1(300);
		}
		break;
	case Wait:
//		if(command_flag==1 && strcmp(command_data, "OK") == 0) {
//			uart_state = Init;
//			command_flag = 0;
//		}
		if (timer1_flag == 1){
			//sprintf ( strr , "\r\n!ADC=%lu#\r\n", ADC_value );
			HAL_UART_Transmit (&huart2 , (uint8_t *) strr, strlen( strr), 20);
			set_Timer1(300);
		}
		break;
	case Nothing:
		break;
	}


}
