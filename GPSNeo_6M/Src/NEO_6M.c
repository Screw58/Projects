 /******************************************************************************
  * @file    NEO_6M.c
  * @author  Jacek
  * @date	 05.08.2022
  * @brief   This file contains code for the handling the GPS neo6-m module and
  * 		 processing the data that uC collects from him
  ******************************************************************************/

#include <string.h>
#include <stdlib.h>
#include "NEO_6M.h"
#include "main.h"

/*
 * strings used by parsing_buf function
 */
const char * gprmc = "$GPRMC" ;
const char * gpvtg = "$GPVTG" ;
const char * gpgga = "$GPGGA" ;
const char * gpgsa = "$GPGSA" ;




 /*
  * @brief  parsing a single line that is in the working buffer and writing data
  * 		to the structure
  * @param  address of the working buffer
  * @param  a pointer to a structure variable to which this function writes data
  * @retval 0 if structure variable used in @param, doesn't contain all the
  * 		required information
  * 		1 if this structure variable contains all the necessary information
  */
uint8_t  parsing_buf(char * xyz, NEO_state * gps_data)
{
	char * ptr = xyz ;
	char * ptr2;
	char tab[20] = {0};

	ptr2 = Extract_data(ptr, tab);

	if( strcmp(tab, gprmc) == 0)
	{
		for(uint8_t count = 1; count <10; count++)
		{
			ptr2 = Extract_data(ptr2, tab);

			switch (count)
			{
				case 1: calculate_time(atoi(tab), gps_data);
						break;
				case 3: gps_data->Latitude = atof(tab);
						break;
				case 4: gps_data->LatitudeDirection = *tab;
						break;
				case 5: gps_data->Longitude = atof(tab) ;
						break;
				case 6: gps_data->LongitudeDirection = *tab;
						break;
				case 9: calculate_date(atoi(tab), gps_data);
						break;
				default: continue;
			}
		}
		ptr = xyz;
	}

	else if( strcmp(tab, gpvtg)== 0 )
	{
		for(uint8_t count = 1; count <8; count++)
		{
			ptr2 =Extract_data(ptr2, tab);

			if(count == 5)				gps_data->SpeedKnots = atof(tab);
			else if (count == 7)		gps_data->SpeedKilometers = atof(tab);
			else						continue;
		}
		ptr = xyz;
	}

	else if(strcmp(tab, gpgga)== 0)
	{
		for(uint8_t count = 1; count <10; count++)
		{
			ptr2 = Extract_data(ptr2, tab);

			if(count == 6)				gps_data->Quality = atoi(tab);
			else if (count == 7)		gps_data->SatelitesNumber = atoi(tab);
			else if(count == 9)			gps_data->Altitude = atof(tab) ;
			else						continue;
		}
		ptr = xyz;
	}

	else if(strcmp(tab, gpgsa)== 0)
	{
		for(uint8_t count = 1; count < 18; count++)
		{
			ptr2 = Extract_data(ptr2, tab);

			if(count == 15)				gps_data->Dop = atof(tab);
			else if (count == 16)		gps_data->Hdop = atof(tab);
			else if(count == 17)		gps_data->Vdop = atof(tab) ;
			else						continue;
		}
		ptr = xyz;
		return COMPLETE_INFO;
	}

	return INCOMPLETE_INFO;
}



 /*
  * @brief  At the beginning of the string in the working buffer,this function
  * 		extracts line identifier, then splits rest of string  into tokens which
  * 		are between ','
  * @param  address from which the function starts looking for a token
  * @param  address of the array where the function writes the token
  * @retval point in which is end of token or identifier
  */
static char *  Extract_data(char * ptr , char * tab)
{
	memset(tab, 0 ,20);

	for(uint8_t i =0 ; i < 20 && (*ptr != ',' ) && (*ptr != '*'); i++, ptr++)
	{
		tab[i] = *ptr ;
	}
	ptr++;

	return ptr;
}



 /*
  * @brief  Writes character from 2nd @param into circular buffer in 1st @param
  */

void load_into_buff(circ_buff_t * circ_buf, char character)
{

	uint8_t temp = circ_buf->head + 1;

	if ( temp == CIRC_BUFF_SIZE)
		temp =0;

	if(temp == circ_buf->tail)
	{
		HAL_GPIO_WritePin(LED_Port, LED_Pin, GPIO_PIN_SET)	;   		//buffer is full
	}
	circ_buf->buffer[circ_buf->head] = character ;
	circ_buf->head = temp;
}



/*
 * @brief  Reads value of 1st @param and writes to the address in 2nd @param
 */
void fetch_from_buff(circ_buff_t * circ_buf, char * data)
{
	if(circ_buf->tail == CIRC_BUFF_SIZE)
		circ_buf->tail = 0;

	if(circ_buf->head == circ_buf->tail)
	{
		HAL_GPIO_WritePin(LED_Port, LED_Pin, GPIO_PIN_SET)	;		//buffer is empty
	}
	else
	{
		*data = circ_buf->buffer[circ_buf->tail];
		circ_buf->tail += 1;
	}
}




/*
 * @brief  These functions calculate the time and date from int number
 * 			and write data into the structure
 */
static void calculate_time(int num, NEO_state * gps_data)
{
	uint16_t temp;

	gps_data->Second = (uint8_t)(num%100) ;

	temp = num%10000;

	gps_data->Minute = (uint8_t)(temp / 100);

	gps_data->Hour = (uint8_t)(num/10000);
}


static void calculate_date(int num, NEO_state * gps_data)
{
	uint16_t temp;

	gps_data->Year = (uint16_t) 2000 + (num%100) ;

	temp = num%10000;

	gps_data->Month = (uint8_t)(temp / 100);

	gps_data->Day = (uint8_t)(num/10000);
}


