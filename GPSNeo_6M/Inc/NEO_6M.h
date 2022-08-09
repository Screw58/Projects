/*
 * NEO_6M.h
 *
 *  Created on: Aug 7, 2022
 *      Author: Jacek
 */

#ifndef INC_NEO_6M_H_
#define INC_NEO_6M_H_

#include "main.h"

typedef struct
{
	// Time and Date
	uint8_t Hour;
	uint8_t Minute;
	uint8_t Second;
	uint8_t Day;
	uint8_t Month;
	uint16_t Year;


	// Position
	double Latitude;
	char LatitudeDirection;
	double Longitude;
	char LongitudeDirection;
	double Altitude;

	// Speed
	double SpeedKnots;
	double SpeedKilometers;

	// Satelites parameters
	uint8_t SatelitesNumber;
	uint8_t Quality; 		// 0 - no Fix, 1 - Fix, 2 - Dif. Fix
	uint8_t FixMode; 		// 1 - no Fiz, 2 - 2D, 3 - 3D
	double Dop; 			// Dilution of precision
	double Hdop; 			// Dilution of precision for flat coords
	double Vdop; 			// Dilution of precision for height

}NEO_state;

uint8_t parsing_buf(char * xyz, NEO_state * gps_data) ;
static char *  Extract_data(char * ptr , char * tab);

void load_into_buff(circ_buff_t * circ_buf, char character);
void fetch_from_buff(circ_buff_t * circ_buf, char * data);
static void calculate_time(int num, NEO_state * gps_data);
static void calculate_date(int num, NEO_state * gps_data);






#endif /* INC_NEO_6M_H_ */
