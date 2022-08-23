/**
  ******************************************************************************
  * @file    serwo.h
  * @author  Jacek
  * @brief   This file provides function prototypes, data structures and defines
  * 		 for serwo handling
  ******************************************************************************/

#ifndef INC_SERWO_H_
#define INC_SERWO_H_


#define PULSE_ON_DEGREE		444

#define SERWO_MAX_POS		180
#define SERWO_MIN_POS		0

/*value of the speed	degrees per second */
#define SERWO_SPEED_1		1
#define SERWO_SPEED_2		2
#define SERWO_SPEED_3		5
#define SERWO_SPEED_4		10
#define SERWO_SPEED_5		25
#define SERWO_SPEED_6		50

 typedef enum{
	 directly,
	 indirectly
 }serwo_mode_t;

typedef struct
{
	TIM_HandleTypeDef * timer;
	uint32_t 	 channel;
	serwo_mode_t mode;					//direct or degree_per_second
	uint8_t speed;
	uint8_t actual_position;
	uint8_t given_position;
	uint8_t counter;
	uint8_t counter_given;

}serwo_t;

void Serwo_start(serwo_t *serwo);
void Serwo_stop(serwo_t *serwo);
void Serwo_Init(serwo_t *serwo, TIM_HandleTypeDef * timer, uint32_t channel );
void Serwo_set_mode(serwo_t *serwo, serwo_mode_t mode);
void Serwo_set_pos(serwo_t *serwo, uint8_t position);
void Serwo_set_speed(serwo_t *serwo, uint8_t speed);
void Serwo_interrupt(serwo_t *serwo);



#endif /* INC_SERWO_H_ */
