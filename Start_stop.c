
#pragma once
#include <linux/can/raw.h>
#include <stdio.h>
#include <linux/can.h>
#include <unistd.h>
#define MTR_OFF 0X80				 //MOTOR OFF COMMAND
#define MTR_STOP 0X81				 //MOTOR STOP COMMAND
#define MTR_RUNNING_CMD 0X88
#define MTR_OFF 0X80				 //MOTOR OFF COMMAND
#define DLC 8


void Motor_stop(int s, int id) /////////////////////// FUNCTION 1
{
	struct can_frame frame_stop;
	
	frame_stop.can_id = id;
	frame_stop.can_dlc = DLC;
	frame_stop.data[0] = MTR_STOP;
	
	if (write(s, &frame_stop, sizeof(struct can_frame)) != sizeof(struct can_frame))
	{
		perror("Write");
	}
}


void Motor_start(int s, int id) /////////////////////// FUNCTION 1
{
	struct can_frame frame_start;
	
	frame_start.can_id = id;
	frame_start.can_dlc = DLC;
	frame_start.data[0] = MTR_RUNNING_CMD;
	frame_start.data[1] = 0x00;
	frame_start.data[2] = 0x00;
	frame_start.data[3] = 0x00;
	frame_start.data[4] = 0x00;
	frame_start.data[5] = 0x00;
	frame_start.data[6] = 0x00;
	frame_start.data[7] = 0x00;
	

	
	
	
	
	if (write(s, &frame_start, sizeof(struct can_frame)) != sizeof(struct can_frame))
	{
		perror("Write");
	}
}

void Motor_Off( int s, int id) /////////////////////// FUNCTION 1
{
	struct can_frame frame_off;
	
	frame_off.can_id = id;
	frame_off.can_dlc = DLC;
	frame_off.data[0] =MTR_OFF;
	frame_off.data[1] =0x00;
	frame_off.data[2] =0x00;
	frame_off.data[3] =0x00;
	frame_off.data[4] =0x00;
	frame_off.data[5] =0x00;
	frame_off.data[6] =0x00;
	frame_off.data[7] =0x00;
	
	
	if (write(s, &frame_off, sizeof(struct can_frame)) != sizeof(struct can_frame))
	{
		perror("Write");
	}
}