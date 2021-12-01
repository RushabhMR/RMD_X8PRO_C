#pragma once

#include <linux/can/raw.h>
#include <stdio.h>
#include <linux/can.h>
#include <unistd.h>
#include <math.h>

#define RD_PID 0x30					 // READ PID DATA COMMAND
#define WR_PID_RAM 0x31				 // WRITE PID TO RAM COMMAND
#define WR_PID_ROM 0X32				 // WRITE PID TO ROM COMMAND
#define RD_ACCN 0X33				 //READ ACCELERATION DATA COMMAND
#define WR_ACCN_RAM 0X34			 //WRITE ACCELERATION DATA TO RAM COMMAND
#define RD_ENC_DATA 0X90			 //READ ENCODER DATA COMMAND
#define WR_ENC_OFST 0X91			 //WRITE ENCODER OFFSET COMMAND
#define WR_MTR_ZERO 0X19			 //WRITE CURRENT POSITION TO ROM AS MOTOR ZERO COMMAND
#define RD_MULTITURN_ANG 0X92		 //READ MULTITURNS ANGLE COMMAND
#define RD_SINGLE_CIRCLE_ANG 0X94	 //READ SINGLE CIRCLE ANGLE COMMAND
#define RD_MTRSTATUS1_ERRORFLAG 0X9A //READ MOTOR STATUS ONE AND ERROR FLAG COMMAND
#define CLR_MOTOR_ERRORFLAG 0X9B	 //CLEAR MOTOR ERROR FLAG COMMAND
#define RD_MTR_STATUS_2 0X9C		 //READ MOTOR STATUS 2
#define RD_MTR_STATUS_3 0X9D		 //READ MOTOR STATUS 3
#define MTR_OFF 0X80				 //MOTOR OFF COMMAND
#define MTR_STOP 0X81				 //MOTOR STOP COMMAND
#define MTR_RUNNING_CMD 0X88		 //MOTOR RUNNING COMMAND
#define TORQ_CLOSEDLOOP 0XA1		 //TORQUE CLOSED-LOOP COMMAND
#define SPEED_CLOSEDLOOP 0XA2		 //SPEED CLOSED-LOOP COMMAND
#define POS_CLOSEDLOOP_1 0XA3		 //POSITION CLOSED-LOOP COMMAND1
#define POS_CLOSEDLOOP_2 0XA4		 //POSITION CLOSED-LOOP COMMAND2
#define POS_CLOSEDLOOP_3 0XA5		 //POSITION CLOSED-LOOP COMMAND3
#define POS_CLOSEDLOOP_4 0XA6		 //POSITION CLOSED-LOOP COMMAND4

#define FRAME_LEN 8

void read_multi_turn_angle_tx(int id, int s) ////////////////////// FUNCTION 21
{
	struct can_frame frame_tx1;

	/**
     * @brief 
     * Populate the frame and send
     */

	frame_tx1.can_id = id;
	frame_tx1.can_dlc = FRAME_LEN;
	frame_tx1.data[0] = RD_MULTITURN_ANG;
	frame_tx1.data[1] = 0x00;
	frame_tx1.data[2] = 0x00;
	frame_tx1.data[3] = 0x00;
	frame_tx1.data[4] = 0x00;
	frame_tx1.data[5] = 0x00;
	frame_tx1.data[6] = 0x00;
	frame_tx1.data[7] = 0x00;

	if (write(s, &frame_tx1, sizeof(struct can_frame)) != sizeof(struct can_frame))
	{
		perror("Write");
	}
}
////////////////////////////////////////
struct can_frame read_multi_turn_angle_rx(int id, int s) /////////////////////// FUNCTION 22
{
	struct can_frame frame_rx1;

	int nbytes = read(s, &frame_rx1, sizeof(struct can_frame));
	for (int i = 0; i < frame_rx1.can_dlc; i++)
	{
		printf("%02X ", frame_rx1.data[i]);
	}

	//printf("\nMotor Temperature: %X", frame_rx1.data[1]); //Temperature

	unsigned long lsbENC = frame_rx1.data[1]; //Torque Current
	unsigned long msbENC = frame_rx1.data[2];
	unsigned long combinedENC0 = (msbENC << 8) | lsbENC;
	//printf("\n%02lX \n", combinedENC0);

	unsigned long lsbENC1 = combinedENC0; //Speed
	unsigned long msbENC1 = frame_rx1.data[3];
	unsigned long combinedENC1 = (msbENC1 << 16) | lsbENC1;
	//printf("%02lX \n", combinedENC1);
	//printf("\nspeed %x", combinedENC1);

	unsigned long lsbENC2 = combinedENC1; //ENcoder Data
	unsigned long msbENC2 = frame_rx1.data[4];
	unsigned long combinedENC2 = (msbENC2 << 24) | lsbENC2;
	//printf("%02lX \n", combinedENC2);
	//printf("\nEncoder Position %X\n", combinedENC2);
	////////////////////////////////////////////////////////////////////

	unsigned long lsbENC3 = combinedENC2; //Torque Current
	unsigned long msbENC3 = frame_rx1.data[5];
	unsigned long combinedENC3 = (msbENC3 << 32) | lsbENC3;
	//printf("%02lX \n", combinedENC3);

	unsigned long lsbENC4 = combinedENC3; //Speed
	unsigned long msbENC4 = frame_rx1.data[6];
	unsigned long combinedENC4 = (msbENC4 << 40) | lsbENC4;
	//printf("%02lX \n", combinedENC4);
	//printf("\nspeed %x", combinedENC4);

	unsigned long lsbENC5 = combinedENC4; //ENcoder Data
	unsigned long msbENC5 = frame_rx1.data[7];
	int  combinedENC5 = (msbENC5 << 48) | lsbENC5;
	//printf("eombined enc5 %x \n", combinedENC5);
	//int inbtw = atoi();

	int Current_angle = (combinedENC5/600);
	printf("\nmulti turn data %d\n",Current_angle);
	//printf("\nMulti turn data %x \t %d\n", Current_angle, Current_angle);

	FILE *fptr;
	
	fptr = fopen("./Encoder.txt", "w+b+r");

	// if (fptr != NULL)
	// {
	// 	//printf("File created successfully!\n");
	// }
	// else
	// {
	// 	//printf("Failed to create the file.\n");
	// }

	if(Current_angle<=360){
		fprintf(fptr,"%d",Current_angle);
	
	//printf("angle is %d", Current_angle);
	printf("\n");
	//	}
	
	}

	else if(Current_angle>360)
	{
		int L;
		L= Current_angle/360;
		int M= L*360;
		Current_angle= Current_angle-M;
		fprintf(fptr," %d",Current_angle);


	}
	fclose(fptr);
	

	return frame_rx1;
}

struct can_frame read_multi_turn_angle_tx_rx(int s, int id) //////////////////////////////////function 23
{

	read_multi_turn_angle_tx(id, s);
	read_multi_turn_angle_rx(id, s);
	//////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void read_single_circle_angle_tx(int id, int s) ////////////////////// FUNCTION 21
{
	struct can_frame frame_Singlec_tx;

	/**
     * @brief 
     * Populate the frame and send
     */

	frame_Singlec_tx.can_id = id;
	frame_Singlec_tx.can_dlc = FRAME_LEN;
	frame_Singlec_tx.data[0] = RD_SINGLE_CIRCLE_ANG;
	frame_Singlec_tx.data[1] = 0x00;
	frame_Singlec_tx.data[2] = 0x00;
	frame_Singlec_tx.data[3] = 0x00;
	frame_Singlec_tx.data[4] = 0x00;
	frame_Singlec_tx.data[5] = 0x00;
	frame_Singlec_tx.data[6] = 0x00;
	frame_Singlec_tx.data[7] = 0x00;

	if (write(s, &frame_Singlec_tx, sizeof(struct can_frame)) != sizeof(struct can_frame))
	{
		perror("Write");
	}
}
struct can_frame read_single_circle_angle_rx(int id, int s) /////////////////////// FUNCTION 22
{
	struct can_frame frame_Singlec_rx1;

	int nbytes = read(s, &frame_Singlec_rx1, sizeof(struct can_frame));

	for (int i = 0; i < frame_Singlec_rx1.can_dlc; i++)
	{
		printf("%02X ", frame_Singlec_rx1.data[i]);
	}

	//printf("\nMotor Temperature: %X", frame_Singlec_rx1.data[1]); //Temperature

	unsigned long lsbcirc = frame_Singlec_rx1.data[6]; //Torque Current
	unsigned long msbcirc = frame_Singlec_rx1.data[7];
	unsigned int combinedcirc0 = (msbcirc << 8) | lsbcirc;
	//printf("\nSingle circle angle without /6 %d", combinedcirc0 / 100); 
	//printf("\nSingle circle angle %X", combinedcirc0 / 600); 
	printf("\nSingle circle angle %d\n", combinedcirc0 / 600);
	unsigned int Myval=(combinedcirc0 / 600);
	
	//Temperature

	FILE *fptr1;
	
	fptr1 = fopen("./SingleCircle.txt", "w+b+r");

	// if (fptr1 != NULL)
	// {
	// 	printf("File created successfully!\n");
	// }
	// else
	// {
	// 	printf("Failed to create the file.\n");
	// }

	
	
	fprintf(fptr1,"%d",Myval);
	
	printf("angle is %d", Myval);
	printf("\n");
	//	}
	fclose(fptr1);

	return frame_Singlec_rx1;
	//return a struct of type can_frame
}

struct can_frame read_single_circle_angle_tx_RX(int id, int s)
{
	read_single_circle_angle_tx(id, s);
	read_single_circle_angle_rx(id, s);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Motor_Zero_Position(int id, int s) ////////////////////// FUNCTION 21
{
	struct can_frame frame_zero_tx;

	///////////////////////////////////
	FILE *fptr;
	
	fptr = fopen("./Encoder.txt", "r");

	if (fptr != NULL)
	{
		printf("File opened successfully!\n");
	}
	else
	{
		printf("Failed to create the file.\n");
	}
	//read_single_circle_angle_tx( id,  s);
	int Q=0;
	//Q= Q- combinedcirc0;
//printf("ZZEro POsition %d",Q);
	
	
	fscanf(fptr,"%d",&Q);
	
	printf("angle is %d", Q);
	printf("\n");
	//	}
	fclose(fptr);

	////////////////////////////////

	//FILE *fptr;
	
	fptr = fopen("./SingleCircle.txt", "r");

	if (fptr != NULL)
	{
		//printf("File opened successfully!\n");
	}
	else
	{
		//printf("Failed to create the file.\n");
	}
	read_single_circle_angle_tx( id,  s);
	int W=0;
	//Q= Q- combinedcirc0;
	//printf("\nZZEro POsition %d",W);
	
	
	fscanf(fptr,"%d",&W);
	
	printf("angle is %d", W);
	printf("\n");

	fclose(fptr);
	int Z=W;
	
	if(Z>=30)
	{
		 Z= Q-W;
	printf("z is %d",Z);

	}
	else if(Z<30){
				 Z= Q;
	printf("z is %d",Z);

	}

	

	

	//myangle2 = (angle2 * 600);
	//myspeed2 = (speed_inpos2 * 6);

	//const int two2 = (myspeed2 & 0x00FF) >> 0;
	//const int three2 = (myspeed2 & 0xFF00) >> 8;

	const int store2 = (-Z*600);
	printf("the decoded data is %d\n",store2);
	printf("the decoded data is %x\n",store2);

	 int four22 = (store2 & 0x000000FF) >> 0;
	 int five22 = (store2 & 0x0000FF00) >> 8;
	 int six22 = (store2 & 0x00FF0000) >> 16;
	 int seven22 = (store2 & 0xFF000000) >> 24;
	frame_zero_tx.can_id = id;
	frame_zero_tx.can_dlc = FRAME_LEN;
	frame_zero_tx.data[0] = POS_CLOSEDLOOP_2;
	frame_zero_tx.data[1] = 0x00;
	frame_zero_tx.data[2] = 0x68;
	frame_zero_tx.data[3] = 0x01;
	frame_zero_tx.data[4] = four22;
	printf("the decoded data is %x\n",four22);
	frame_zero_tx.data[5] = five22;
	printf("the decoded data is %x\n",five22);
	frame_zero_tx.data[6] = six22;
	printf("the decoded data is %x\n",six22);
	frame_zero_tx.data[7] = seven22;
	printf("the decoded data is %x\n",seven22);
	//if(216000>=myangle2 & myangle2>=0)
	//{
		if (write(s, &frame_zero_tx, sizeof(struct can_frame)) != sizeof(struct can_frame))
		{
			perror("Write");
		}
	//}
	//else{
		//printf("invalid angle");
	//}




}