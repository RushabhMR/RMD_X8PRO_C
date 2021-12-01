#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// #include "in.hpp"
//#include <conio.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>
#include "Start_stop.c"
#include "Control_CMD.h"
#include "PID.c"
#include "Read_Motor_Status.c"
#include "Reading_encoder.c"
#include "acceleration.c"

int main(int argc, char **argv)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int i, j = 0;
	int s;

	struct sockaddr_can addr;
	struct ifreq ifr;
	struct can_frame frame;
	struct can_frame frame2;
	int RPS;

	printf("CAN Sockets Demo\r\n");

	if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
	{
		perror("Socket");
		return 1;
	}

	strcpy(ifr.ifr_name, "can1");
	ioctl(s, SIOCGIFINDEX, &ifr);

	memset(&addr, 0, sizeof(addr));
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		perror("Bind");
		return 1;
	}

	int id = 0x142;

	// int spin_dir4 = 01;
	// printf("1");
	// int angle4 = 00;
	// printf("2");
	// int speed_inpos4 = 90;
	// printf("3");

	// position_control_cmd_4_tx_rx(id, s, angle4, spin_dir4, speed_inpos4);
	// printf("4\n");

	// {
	// 	int Start;
	// 	Scanf("")

	// 	if()
	// 	{

	// 	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	while (1)
	{
		// read_multi_turn_angle_tx_rx(s, id);

		// printf("Select The Mode\n");
		// printf("1.Read PID Data Command\n2.Write PID To RAM Command\n3.Write PID to ROM Command\n4.Read Acceleration Data Command\n");
		// printf("5.Write Acceleraton Data To Ram Command\n6.Read Encoder Data Command\n7.Write Encoder Offest Command\n8.Cant Be Done\n");
		// printf("9.Read Multi Turns Angle Command\n10.Read Single Circle Angle Command\n11.Read Motor Status 1 and Error Flag Command\n");
		// printf("12.Clear Motor Error Flag Command\n13.Read Motor Status 2\n14.Read Motor Status 3\n15.Motor Off Command\n16.Motor Stop Command\n");
		// printf("17.Motor Running Command\n18.Torque Closed-Loop Command 2\n19.Speed Closed-Loop Command\n20.Position Closed-Loop Command1\n");
		// printf("21.Position Closed-Loop Command2\n22.Position Closed-Loop Command3\n23.Position Closed-Loop Command4\n\n\n");

		int Mode;
		printf("Mode=");

		scanf("%d", &Mode);

		switch (Mode)
		{
		case 1:
			printf("\nRead PID Data Command");

			read_PID_Data_tx_rx(s, id);
			break;

		case 2:
			printf("\nWrite PID To RAM Command");

			int Pos_loop_kp;
			printf("\nEnter The Position Loop Kp ");
			scanf("%d", &Pos_loop_kp);
			int Pos_loop_ki;
			printf("\nEnter The Position Loop Ki");
			scanf("%d", &Pos_loop_ki);
			int speed_loop_kp;
			printf("\nEnter The Speed Loop Kp");
			scanf("%d", &speed_loop_kp);
			int speed_loop_ki;
			printf("\nEnter The Speed Loop Kp");
			scanf("%d", &speed_loop_ki);
			int Torque_loop_kp;
			printf("\nEnter The Torque Loop Kp");
			scanf("%d", &Torque_loop_kp);
			int Torque_loop_ki;
			printf("\nEnter The Torque Loop Kp");
			scanf("%d", &Torque_loop_ki);

			write_PID_to_RAM_tx_rx(id, s, Pos_loop_kp, Pos_loop_ki, speed_loop_kp, speed_loop_ki, Torque_loop_kp, Torque_loop_ki);

			break;

		case 3:
			printf("\nWrite PID to ROM Command");

			int Pos_loop_kp_ROM;
			printf("\nEnter The Position Loop Kp");
			scanf("%d", &Pos_loop_kp_ROM);

			int Pos_loop_ki_ROM;
			printf("\nEnter The Position Loop Ki");
			scanf("%d", &Pos_loop_ki_ROM);

			int speed_loop_kp_ROM;
			printf("\nEnter The Speed Loop Kp");
			scanf("%d", &speed_loop_kp_ROM);

			int speed_loop_ki_ROM;
			printf("\nEnter The Speed Loop Ki");
			scanf("%d", &speed_loop_ki_ROM);

			int Torque_loop_kp_ROM;
			printf("\nEnter The Torque Loop Kp");
			scanf("%d", &Torque_loop_kp_ROM);

			int Torque_loop_ki_ROM;
			printf("\nEnter The Torque Loop Ki");
			scanf("%d", &Torque_loop_ki_ROM);

			write_PID_to_ROM_tx_rx(id, s, Pos_loop_kp_ROM, Pos_loop_ki_ROM, speed_loop_kp_ROM, speed_loop_ki_ROM, Torque_loop_kp_ROM, Torque_loop_ki_ROM);

			break;

		case 4:
			printf("\nRead Acceleration Data Command");
			read_Accn_Data_tx_rx(s, id);

			break;

		case 5:
			printf("\nWrite Acceleraton Data To Ram Command");

			int Accn_input;
			printf("\nEnter The Acceleration to RAM");
			scanf("%d", &Accn_input);

			RDWR_Accn_Data_tx_rx(s, id, Accn_input);

			break;

		case 6:
			printf("\nRead Encoder Data Command");

			read_Encoder_Data_tx_rx(s, id);

			break;

		case 7:
			printf("\nWrite Encoder Offest Command");
			int Enc_Offest;
			printf("\nEnter The Encoder Offest");

			scanf("%d", &Enc_Offest);

			Write_Encoder_Offset_tx_rx(s, id, Enc_Offest);

			break;

		case 8:
			printf("\nCant Be Done");

			break;

		case 9:

			printf("\nRead Multi Turns Angle Command");

			// Motor_Zero_Position(id, s);

			break;

		case 10:
			printf("\nRead Single Circle Angle Command");
			read_single_circle_angle_tx_RX(id, s);

			break;

		case 11:
			printf("\nRead Motor Status 1 and Error Flag Command");
			read_Motor_Status1_AND_Error_Flag_tx_rx(id, s);

			break;

		case 12:
			printf("\nClear Motor Error Flag Command");
			Clear_Motor_Error_Flag_tx_rx(id, s);

			break;

		case 13:
			printf("\nRead Motor Status 2");
			Read_Motor_Status_2_tx_rx(id, s);

			break;

		case 14:
			printf("\nRead Motor Status 3");
			Read_Motor_Status_3_tx_rx(id, s);

			break;

		case 15:
			printf("\nMotor Off Command");
			Motor_Off(s, id);

			break;

		case 16:
			printf("\nMotor Stop Command");
			Motor_stop(s, id);

			break;

		case 17:
			printf("\nMotor Running Command");
			Motor_start(s, id);

			break;

		case 18:
			printf("\nTorque Closed-Loop Command");
			int Torque_C;
			printf("\nEnter The Torque Current");

			scanf("%d", &Torque_C);

			Torque_current_control_tx_rx(id, s, Torque_C);

			break;

		case 19:
			printf("\nSpeed Closed-Loop Command\n");

			printf("\nEnter the RPS\n");

			scanf(" %d", &RPS);

			uint speed = (RPS * 360);

			speed_closed_loop_tx_rx(s, id, speed);

			break;
		case 20:
			printf("\nPosition Closed-Loop Command1");
			int angle1;
			printf("\nEnter The Angle in Degree(Multi Circle)");
			scanf("%d", &angle1);

			position_control_cmd_1_tx_rx(s, id, angle1);

			break;

		case 21:
			printf("\nPosition Closed-Loop Command2\n");
			int angle2;
			printf("\nEnter The Angle in Degree(Multi Circle)\n");
			scanf(" %d", &angle2);
			int speed_inpos2;
			printf("\nEnter the Speed\n");
			scanf("%d", &speed_inpos2);
			printf("executed1");
			position_control_cmd_2_tx_rx(s, id, angle2, speed_inpos2);
			printf("\nexecuted2\n");
			break;

		case 22:
			printf("\nPosition Closed-Loop Command3");

			int angle3;
			printf("\nEnter The Angle in Degree(Single Circle)");
			scanf("%d", &angle3);
			int spin_dir3;
			printf("\nEnter The Spin Directionn\n 0 for Clockwise\n 1 For Counter Clockwise");

			scanf("%d", &spin_dir3);

			position_control_cmd_3_tx_rx(s, id, angle3, spin_dir3);

			break;

		case 23:
			printf("\nPosition Closed-Loop Command4");
			int spin_dir4;
			int speed_inpos4;
			int angle4;
			printf("\nSpin Direction");
			scanf("%d", &spin_dir4);
			printf("\nEnter The Speed in degree per Second\n");
			scanf(" %d ", &speed_inpos4); /////////CLOSED LOOP POSITION 4 COMMAND
			printf("Enter The Angle in Degree(Single Circle)\n");
			scanf(" %d ", &angle4);
			printf("\ndone"); /////////CLOSED LOOP POSITION 4 COMMAND
			position_control_cmd_4_tx_rx(id, s, spin_dir4, speed_inpos4, angle4);
			printf("\nBreak");

			break;

		case 24:
			usleep(1000);
			read_multi_turn_angle_tx_rx(s, id);
			usleep(1000);
			read_single_circle_angle_tx_RX(id, s);

			break;
		case 25:
			// read_single_circle_angle_tx_RX( id,  s);
			Motor_Zero_Position(id, s);
			usleep(1000);
			// Motor_Off( s,  id);
			usleep(1000);
			// Motor_start( s, id);

			break;
		case 26:
			Motor_Off(s, id);
			//usleep(5000);
			Motor_start(s, id);

			break;

		default:
			printf("\nDefault");
		}
	}
	return 0;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// RDWR_Accn_Data_tx_rx(s, id, Accn_input);
	// speed_closed_loop_tx_rx( s, id,  speed);
	// position_control_cmd_2_tx_rx(s, id, angle2, speed_inpos2);
}
