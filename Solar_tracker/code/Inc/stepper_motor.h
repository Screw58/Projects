/**
  ******************************************************************************
  * @file    stepper_motor.h
  * @author  Jacek
  * @brief   This file provides function prototypes, data structures and important
  * 		 defined values for stepper_motor handling
  *
  ******************************************************************************/

#ifndef INC_STEPPER_MOTOR_H_
#define INC_STEPPER_MOTOR_H_


#define ANGLE_PER_STEP		1.8f
#define MICRO_STEPS_1_2		2
#define MICRO_STEPS_1_4		4
#define MICRO_STEPS_1_8		8
#define MICRO_STEPS_1_16	16

#define MAX_SPEED			1000
#define MIN_SPEED			1

#define DIR_PIN		GPIO_PIN_0
#define DIR_PORT	GPIOC

//#define EN_PIN								//!<TODO
//#define EN_PORT       						//!<TODO


typedef enum mode {
	off ,
	angle ,
	continous
}stepper_mode_t;


typedef enum{
	right,
	left
}dir_t;


typedef struct
{
	TIM_HandleTypeDef * timer;
	stepper_mode_t 	mode;
	uint16_t 	speed;
	dir_t 		direction;
	uint32_t 	channel;
	uint32_t	angle_counter;
	uint16_t	angle;
	uint8_t		micro_steps;
}stepper_t;

void Stepper_start(stepper_t * motor);
void Stepper_stop(stepper_t * motor);
void Stepper_set_mode(stepper_t * motor, stepper_mode_t mode );
void Stepper_set_speed(stepper_t * motor, uint16_t speed);
void Stepper_set_dir(stepper_t * motor, dir_t direction );
void Stepper_init(stepper_t * motor, TIM_HandleTypeDef * htim ,uint32_t channel);
void Stepper_interrupt(TIM_HandleTypeDef * htim, stepper_t * motor);
void Stepper_set_angle(stepper_t * motor, uint16_t angle);


#endif /* INC_STEPPER_MOTOR_H_ */
