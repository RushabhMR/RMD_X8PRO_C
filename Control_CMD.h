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

void speed_closed_loop_tx(int s, int id, int speed) /////////////////////// FUNCTION 11
{
	struct can_frame frame;
	
	/**
     * @brief 
     * Populate the frame and send
     */

	int decvalue;
	decvalue = (speed*600) ;

	const int store = decvalue;
	const int four = (store & 0x000000FF) >> 0;
	const int five = (store & 0x0000FF00) >> 8;
	const int six = (store & 0x00FF0000) >> 16;
	const int seven = (store & 0xFF000000) >> 24;

	frame.can_id = id;
	frame.can_dlc = FRAME_LEN;
	frame.data[0] = SPEED_CLOSEDLOOP;
	frame.data[1] = 0x00;
	frame.data[2] = 0x00;
	frame.data[3] = 0x00;
	frame.data[4] = four;
	frame.data[5] = five;
	frame.data[6] = six;
	frame.data[7] = seven;

	if (write(s, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame))
	{
		perror("Write");
	}
}

struct can_frame speed_closed_loop_rx(int id, int s) /////////////////////// FUNCTION 12
{
	struct can_frame frame_rx;

	int nbytes = read(s, &frame_rx, sizeof(struct can_frame));

	//for (int i = 0; i < frame_rx.can_dlc; i++)
	//{
		//printf("%02X ", frame_rx.data[i]);
	//}

	// printf("\nMotor Temperature: %d", frame_rx.data[1]); //Temperature

	unsigned short lsb0 = frame_rx.data[2]; //Torque Current
	unsigned short msb0 = frame_rx.data[3];
	unsigned int combined0 = (msb0 << 8) | lsb0;
	//printf("\nTorque Current %d", combined0);

	unsigned short lsb1 = frame_rx.data[4]; //Speed
	unsigned short msb1 = frame_rx.data[5];
	short combined1 = (msb1 << 8) | lsb1;
	///printf("\nspeed %d", combined1 / 6);

	unsigned short lsb2 = frame_rx.data[6]; //ENcoder Data
	unsigned short msb2 = frame_rx.data[7];
	int combined2 = (msb2 << 8) | lsb2;
	//printf("\nEncoder Position %d\n", combined2);
	return frame_rx;
	//return a struct of type can_frame
}

// // return_type name_fn(args)
struct can_frame speed_closed_loop_tx_rx(int s, int id, int speed) //////////////////////////////////function 13
{																  
	// struct can_frame frame_tx;
	//struct can_frame frame_tx;
	/**
     * @brief
     * Populate the frame and send
     */
	printf("Doner");
	speed_closed_loop_tx(s, id, speed);
	speed_closed_loop_rx(id, s);
	/**
     * @brief
     * Receive the frame and send
     *
     */
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Torque_current_control_tx(int id, int s, int Torque_C) ////////////////////// FUNCTION 31
{
	struct can_frame frame_Torque_tx;

	int myTorque;


	myTorque = ((2048* Torque_C)/33);
	

	const int four1 = (myTorque & 0x00FF) >> 0;
	const int five1 = (myTorque & 0xFF00) >> 8;

	
	frame_Torque_tx.can_id = id;
	frame_Torque_tx.can_dlc = FRAME_LEN;
	frame_Torque_tx.data[0] = TORQ_CLOSEDLOOP;
	frame_Torque_tx.data[1] = 0x00;
	frame_Torque_tx.data[2] = 0x00;
	frame_Torque_tx.data[3] = 0x00;
	frame_Torque_tx.data[4] = four1;
	frame_Torque_tx.data[5] = five1;
	frame_Torque_tx.data[6] = 0x00;
	frame_Torque_tx.data[7] = 0x00;

	if (write(s, &frame_Torque_tx, sizeof(struct can_frame)) != sizeof(struct can_frame))
	{
		perror("Write");
	}
}

struct can_frame Torque_current_control_rx(int id, int s) /////////////////////// FUNCTION 32
{
	struct can_frame frame_Torque_rx;

	int nbytes = read(s, &frame_Torque_rx, sizeof(struct can_frame));

	for (int i = 0; i < frame_Torque_rx.can_dlc; i++)
	{
		//printf("%02X ", frame_Torque_rx.data[i]);
	}

	printf("\nMotor Temperature: %d", frame_Torque_rx.data[1]); //Temperature

	unsigned short lsbtorq0 = frame_Torque_rx.data[2]; //Torque Current
	unsigned short msbtorq0 = frame_Torque_rx.data[3];
	unsigned int combinedtorq0 = (msbtorq0 << 8) | lsbtorq0;
	printf("\nTorque Current %d", combinedtorq0);

	unsigned short lsbtorq1 = frame_Torque_rx.data[4]; //Speed
	unsigned short msbtorq1 = frame_Torque_rx.data[5];
	short combinedtorq1 = (msbtorq1 << 8) | lsbtorq1;
	printf("\nspeed %d", combinedtorq1 / 6);

	unsigned short lsbtorq2 = frame_Torque_rx.data[6]; //ENcoder Data
	unsigned short msbtorq2 = frame_Torque_rx.data[7];
	int combinedtorq2 = (msbtorq2 << 8) | lsbtorq2;
	printf("\nencoder position %d", combinedtorq2 / 6);
	return frame_Torque_rx;
	//return a struct of type can_frame
}

struct can_frame Torque_current_control_tx_rx(int id, int s, int Torque_C) //////////////////////////////////function 33
{

	Torque_current_control_tx( id, s, Torque_C);
	Torque_current_control_rx( id,  s);
}

//////////////////////////////////////////////////////////////////////////POSITION LOOP 1///////////////////////////////////////////////////////////////////////////


void position_control_cmd_1_tx(int id, int s, int angle1) ////////////////////// FUNCTION 31
{
	struct can_frame frame_Pos1_tx;

	int myangle1;
	

	myangle1 = (angle1 * 600);
	const int store1 = myangle1;

	const int four1 = (store1 & 0x000000FF) >> 0;
	const int five1 = (store1 & 0x0000FF00) >> 8;
	const int six1 = (store1 & 0x00FF0000) >> 16;
	const int seven1 = (store1 & 0xFF000000) >> 24;
	frame_Pos1_tx.can_id = id;
	frame_Pos1_tx.can_dlc = FRAME_LEN;
	frame_Pos1_tx.data[0] = POS_CLOSEDLOOP_1;
	frame_Pos1_tx.data[1] = 0x00;
	frame_Pos1_tx.data[2] = 0x00;
	frame_Pos1_tx.data[3] = 0x00;
	frame_Pos1_tx.data[4] = four1;
	frame_Pos1_tx.data[5] = five1;
	frame_Pos1_tx.data[6] = six1;
	frame_Pos1_tx.data[7] = seven1;

	if (write(s, &frame_Pos1_tx, sizeof(struct can_frame)) != sizeof(struct can_frame))
	{
		perror("Write");
	}
}

struct can_frame position_control_cmd_1_rx(int id, int s) /////////////////////// FUNCTION 32
{
	struct can_frame frame_Pos1_rx;

	int nbytes = read(s, &frame_Pos1_rx, sizeof(struct can_frame));

	for (int i = 0; i < frame_Pos1_rx.can_dlc; i++)
	{
		//printf("%02X ", frame_Pos1_rx.data[i]);
	}

	printf("\nMotor Temperature: %d", frame_Pos1_rx.data[1]); //Temperature

	unsigned short lsbpos10 = frame_Pos1_rx.data[2]; //Torque Current
	unsigned short msbpos10 = frame_Pos1_rx.data[3];
	unsigned int combinedposs10 = (msbpos10 << 8) | lsbpos10;
	printf("\nTorque Current %d", combinedposs10);

	unsigned short lsbpos11 = frame_Pos1_rx.data[4]; //Speed
	unsigned short msbpos11 = frame_Pos1_rx.data[5];
	short combinedposa11 = (msbpos11 << 8) | lsbpos11;
	printf("\nspeed %d", combinedposa11 / 6);

	unsigned short lsbpos12 = frame_Pos1_rx.data[6]; //ENcoder Data
	unsigned short msbpos12 = frame_Pos1_rx.data[7];
	int combinedposs12 = (msbpos12 << 8) | lsbpos12;
	printf("\nencoder position %d", combinedposs12 / 6);
	return frame_Pos1_rx;
	//return a struct of type can_frame
}

struct can_frame position_control_cmd_1_tx_rx(int s, int id, int angle1) //////////////////////////////////function 33
{

	position_control_cmd_1_tx(id, s, angle1);
	position_control_cmd_1_rx(id, s);
}




///////////////////////////////////////////////////////////////////////////////POSITION LOOP 2///////////////////////////////////////////////////////////////////////////////////////////
void position_control_cmd_2_tx(int id, int s, int angle2, int speed_inpos2) ////////////////////// FUNCTION 31
{
	struct can_frame frame_Pos2_tx;
	printf("position loop 2 called");

	int myangle2;
	int myspeed2;

	myangle2 = (angle2 * 600);
	myspeed2 = (speed_inpos2 * 6);

	const int two2 = (myspeed2 & 0x00FF) >> 0;
	const int three2 = (myspeed2 & 0xFF00) >> 8;

	const int store2 = myangle2;

	 int four2 = (store2 & 0x000000FF) >> 0;
	 int five2 = (store2 & 0x0000FF00) >> 8;
	 int six2 = (store2 & 0x00FF0000) >> 16;
	 int seven2 = (store2 & 0xFF000000) >> 24;
	frame_Pos2_tx.can_id = id;
	frame_Pos2_tx.can_dlc = FRAME_LEN;
	frame_Pos2_tx.data[0] = POS_CLOSEDLOOP_2;
	frame_Pos2_tx.data[1] = 0x00;
	frame_Pos2_tx.data[2] = two2;
	frame_Pos2_tx.data[3] = three2;
	frame_Pos2_tx.data[4] = four2;
	frame_Pos2_tx.data[5] = five2;
	frame_Pos2_tx.data[6] = six2;
	frame_Pos2_tx.data[7] = seven2;
	//if(216000>=myangle2 & myangle2>=0)
	//{
		if (write(s, &frame_Pos2_tx, sizeof(struct can_frame)) != sizeof(struct can_frame))
		{
			perror("Write");
		}
	//}
	//else{
//		printf("invalid angle");
	//}
}
////////////////////////////////////////
struct can_frame position_control_cmd_2_rx(int id, int s) /////////////////////// FUNCTION 32
{
	struct can_frame frame_Pos2_rx;

	int nbytes = read(s, &frame_Pos2_rx, sizeof(struct can_frame));

	for (int i = 0; i < frame_Pos2_rx.can_dlc; i++)
	{
		//printf("%02X ", frame_Pos2_rx.data[i]);
	}

//	printf("\nMotor Temperature: %d", frame_Pos2_rx.data[1]); //Temperature

	unsigned short lsbpos20 = frame_Pos2_rx.data[2]; //Torque Current
	unsigned short msbpos20 = frame_Pos2_rx.data[3];
	unsigned int combinedposS20 = (msbpos20 << 8) | lsbpos20;
	//printf("\nTorque Current %d", combinedposS20);

	unsigned short lsbpos21 = frame_Pos2_rx.data[4]; //Speed
	unsigned short msbpos21 = frame_Pos2_rx.data[5];
	short combinedposs21 = (msbpos21 << 8) | lsbpos21;
	//printf("\nspeed %d", combinedposs21 / 6);

	unsigned short lsbpos22 = frame_Pos2_rx.data[6]; //ENcoder Data
	unsigned short msbpos22 = frame_Pos2_rx.data[7];
	int combinedposs22 = (msbpos22 << 8) | lsbpos22;
	//printf("\nencoder position %d", combinedposs22 / 6);
	return frame_Pos2_rx;
	//return a struct of type can_frame
}

struct can_frame position_control_cmd_2_tx_rx(int s, int id, int angle, int speed_inpos) //////////////////////////////////function 33
{

	position_control_cmd_2_tx(id, s, angle, speed_inpos);
	position_control_cmd_2_rx(id, s);
}

//////////////////////////////////////////////////////////////////////////POSITION LOOP 3///////////////////////////////////////////////////////////////////////

void position_control_cmd_3_tx(int id, int s, int angle3, int spin_dir3) ////////////////////// FUNCTION 31
{
	struct can_frame frame_Pos3_tx;

	int myangle3;
	int Rotate= spin_dir3;
	

	myangle3 = (angle3 * 100);
	

	const int four3 = (myangle3 & 0x00FF) >> 0;
	const int five3 = (myangle3 & 0xFF00) >> 8;


	frame_Pos3_tx.can_id = id;
	frame_Pos3_tx.can_dlc = FRAME_LEN;
	frame_Pos3_tx.data[0] = POS_CLOSEDLOOP_3;
	frame_Pos3_tx.data[1] = Rotate;
	frame_Pos3_tx.data[2] = 0x00;
	frame_Pos3_tx.data[3] = 0x00;
	frame_Pos3_tx.data[4] = four3;
	frame_Pos3_tx.data[5] = five3;
	frame_Pos3_tx.data[6] = 0x00;
	frame_Pos3_tx.data[7] = 0x00;

	if (write(s, &frame_Pos3_tx, sizeof(struct can_frame)) != sizeof(struct can_frame))
	{
		perror("Write");
	}
}

struct can_frame position_control_cmd_3_rx(int id, int s) /////////////////////// FUNCTION 32
{
	struct can_frame frame_Pos3_rx;

	int nbytes = read(s, &frame_Pos3_rx, sizeof(struct can_frame));

	for (int i = 0; i < frame_Pos3_rx.can_dlc; i++)
	{
		printf("%02X ", frame_Pos3_rx.data[i]);
	}

	printf("\nMotor Temperature: %d", frame_Pos3_rx.data[1]); //Temperature

	unsigned short lsbpos30 = frame_Pos3_rx.data[2]; //Torque Current
	unsigned short msbpos30 = frame_Pos3_rx.data[3];
	unsigned int combinedposs30 = (msbpos30 << 8) | lsbpos30;
	printf("\nTorque Current %d", combinedposs30);

	unsigned short lsbpos31 = frame_Pos3_rx.data[4]; //Speed
	unsigned short msbpos31 = frame_Pos3_rx.data[5];
	short combinedposs31 = (msbpos31 << 8) | lsbpos31;
	printf("\nspeed %d", combinedposs31 / 6);

	unsigned short lsbpos32 = frame_Pos3_rx.data[6]; //ENcoder Data
	unsigned short msbpos32 = frame_Pos3_rx.data[7];
	int combinedposs32 = (msbpos32 << 8) | lsbpos32;
	printf("\nencoder position %d", combinedposs32 / 6);
	return frame_Pos3_rx;
	//return a struct of type can_frame
}

struct can_frame position_control_cmd_3_tx_rx(int s, int id, int angle, int spin_dir) //////////////////////////////////function 33
{

	position_control_cmd_3_tx(id, s, angle , spin_dir);
	position_control_cmd_3_rx(id, s);
}

////////////////////////////////////////////////////////////////////////////POSITION LOOP 4/////////////////////////////////////////////////////////////////////////////////////////////




void position_control_cmd_4_tx(int id, int s, int angle4, int spin_dir4, int speed_inpos4) ////////////////////// FUNCTION 31
{
	struct can_frame frame_Pos4_tx;

	int myangle4; 
	int Rotate4= spin_dir4;
	int Speeed;
	Speeed= (speed_inpos4*6);
	

	myangle4 = (angle4*100);
	

	const int four4 = (myangle4 & 0x00FF) >> 0;
	const int five4 = (myangle4 & 0xFF00) >> 8;
	
	const int Two4 = (Speeed & 0x00FF) >> 0;
	const int Three4 = (Speeed & 0xFF00) >> 8;


	frame_Pos4_tx.can_id = id;
	frame_Pos4_tx.can_dlc = FRAME_LEN;
	frame_Pos4_tx.data[0] = POS_CLOSEDLOOP_4;
	frame_Pos4_tx.data[1] = Rotate4;
	frame_Pos4_tx.data[2] = Two4;
	frame_Pos4_tx.data[3] = Three4;
	frame_Pos4_tx.data[4] = four4;
	frame_Pos4_tx.data[5] = five4;
	frame_Pos4_tx.data[6] = 0x00;
	frame_Pos4_tx.data[7] = 0x00;
	printf("Rotate4%x",Rotate4);
	printf("Rotate4%x",Two4);
	printf("Rotate4%x",Three4);
	printf("Rotate4%x",four4);
	printf("Rotate4%x",five4);

	if (write(s, &frame_Pos4_tx, sizeof(struct can_frame)) != sizeof(struct can_frame))
	{
		perror("Write");
	}
}

struct can_frame position_control_cmd_4_rx(int id, int s) /////////////////////// FUNCTION 32
{
	struct can_frame frame_Pos4_rx;

	int nbytes = read(s, &frame_Pos4_rx, sizeof(struct can_frame));

	for (int i = 0; i < frame_Pos4_rx.can_dlc; i++)
	{
		//printf("%02X ", frame_Pos4_rx.data[i]);
	}

	printf("\nMotor Temperature: %d", frame_Pos4_rx.data[1]); //Temperature

	unsigned short lsbpos40 = frame_Pos4_rx.data[2]; //Torque Current
	unsigned short msbpos40 = frame_Pos4_rx.data[3];
	unsigned int combinedposs40 = (msbpos40 << 8) | lsbpos40;
	printf("\nTorque Current %d", combinedposs40);

	unsigned short lsbpos41 = frame_Pos4_rx.data[4]; //Speed
	unsigned short msbpos41 = frame_Pos4_rx.data[5];
	short combinedposs41 = (msbpos41 << 8) | lsbpos41;
	printf("\nspeed %d", combinedposs41 / 6);

	unsigned short lsbpos42 = frame_Pos4_rx.data[6]; //ENcoder Data
	unsigned short msbpos42 = frame_Pos4_rx.data[7];
	int combinedposs42 = (msbpos42 << 8) | lsbpos42;
	printf("\nencoder position %d", combinedposs42 / 6);
	return frame_Pos4_rx;
	//return a struct of type can_frame
}

struct can_frame position_control_cmd_4_tx_rx(int id, int s, int angle4, int spin_dir4, int speed_inpos4) //////////////////////////////////function 33
{

	position_control_cmd_4_tx(id, s, angle4,  spin_dir4, speed_inpos4);
	position_control_cmd_4_rx(id, s);
}
