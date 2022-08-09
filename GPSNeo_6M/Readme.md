# GPS

## General info
Project is based on the nucleo f446 board and the u-blox GY-NEO6M module. 
The goal of this project is to convert NMEA messages transmitted by Neo6 
into human understandable information.

## Results

--------------------------------------------------------------------------- Before -----------------------------------------------------------------------

In this case the module is connected to USB-UART converter. Messages are printed 
on terminal directly from NEO_6

![](Images/gps_without_program.jpg)





--------------------------------------------------------------------------- After -----------------------------------------------------------------------

Here, module is connected to Nucleo board and program displays trough virtual COM port all info. 

![](Images/gps_done.jpg)


## Software
