#pragma once
#include <linux/can/raw.h>
#include <stdio.h>
#include <linux/can.h>
#include <unistd.h>

#define FRAME_LEN 8
#define RD_ENC_DATA 0X90			 //READ ENCODER DATA COMMAND
#define WR_ENC_OFST 0X91			 //WRITE ENCODER OFFSET COMMAND
#define RD_MTRSTATUS1_ERRORFLAG 0X9A //READ MOTOR STATUS ONE AND ERROR FLAG COMMAND
#define CLR_MOTOR_ERRORFLAG 0X9B	 //CLEAR MOTOR ERROR FLAG COMMAND
#define RD_MTR_STATUS_2 0X9C		 //READ MOTOR STATUS 2
#define RD_MTR_STATUS_3 0X9D		 //READ MOTOR STATUS 3


void read_Encoder_Data_tx(int id, int s) ////////////////////// FUNCTION 11
{
    struct can_frame frame_ENC_data_tx;

    frame_ENC_data_tx.can_id = id;
    frame_ENC_data_tx.can_dlc = FRAME_LEN;
    frame_ENC_data_tx.data[0] = RD_ENC_DATA;
    frame_ENC_data_tx.data[1] = 0x00;
    frame_ENC_data_tx.data[2] = 0x00;
    frame_ENC_data_tx.data[3] = 0x00;
    frame_ENC_data_tx.data[4] = 0x00;
    frame_ENC_data_tx.data[5] = 0x00;
    frame_ENC_data_tx.data[6] = 0x00;
    frame_ENC_data_tx.data[7] = 0x00;

    if (write(s, &frame_ENC_data_tx, sizeof(struct can_frame)) != sizeof(struct can_frame))
    {
        perror("Write");
    }
}



struct can_frame read_Encoder_Data_rx(int id, int s) /////////////////////// FUNCTION 12
{
	struct can_frame frame_ENC_data_rx;

	int nbytes = read(s, &frame_ENC_data_rx, sizeof(struct can_frame));

    if(frame_ENC_data_rx.data[0]==0x90)
    {

	for (int i = 0; i < frame_ENC_data_rx.can_dlc; i++)
	{
		printf("%02X ", frame_ENC_data_rx.data[i]);
	}

	//printf("\nMotor Temperature: %d", frame_ENC_data_rx.data[1]); //Temperature

	unsigned short lsbb0 = frame_ENC_data_rx.data[2]; //Torque Current
	unsigned short msbb0 = frame_ENC_data_rx.data[3];
	unsigned int combinedd0 = (msbb0 << 8) | lsbb0;
	printf("\nEncoder Position      %d", combinedd0);
   // usleep(1000);

	unsigned short lsbb1 = frame_ENC_data_rx.data[4]; //Speed
	unsigned short msbb1 = frame_ENC_data_rx.data[5];
	int combinedd1 = (msbb1 << 8) | lsbb1;
	printf("\nEncoder Original Position      %d", combinedd1 );

	unsigned short lsbb2 = frame_ENC_data_rx.data[6]; //ENcoder Data
	unsigned short msbb2 = frame_ENC_data_rx.data[7];
	int combinedd2 = (msbb2 << 8) | lsbb2;
	printf("\nEncoder Offset Position       %d\n", combinedd2);
	return frame_ENC_data_rx;
	//return a struct of type can_frame
    }
}
struct can_frame read_Encoder_Data_tx_rx(int s, int id) //////////////////////////////////function 13
{

   read_Encoder_Data_tx( id,s);
   read_Encoder_Data_rx( id,s);
    
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Write_Encoder_Offset_tx(int s, int id, int Enc_Offest) /////////////////////// FUNCTION 1
{
	struct can_frame frame_offset_tx;
	
	frame_offset_tx.can_id = id;
	frame_offset_tx.can_dlc = FRAME_LEN;
	frame_offset_tx.data[0] = WR_ENC_OFST;
	
	if (write(s, &frame_offset_tx, sizeof(struct can_frame)) != sizeof(struct can_frame))
	{
		perror("Write");
	}
}

struct can_frame Write_Encoder_Offset_rx(int id, int s) /////////////////////////////////////// FUNCTION 12
{
    struct can_frame frame_offset_rx;

    int nbytes = read(s, &frame_offset_rx, sizeof(struct can_frame));

    for (int i = 0; i < frame_offset_rx.can_dlc; i++)
    {
        //printf("%02X ", frame_offset_rx.data[i]);
    }

 
    unsigned short lsboffst2 = frame_offset_rx.data[6];
    unsigned short msboffst2 = frame_offset_rx.data[7];
    int combinedoffst2 = (msboffst2 << 8) | lsboffst2;

    printf("\nEncoder off set command set to: %d\n", combinedoffst2);
   
    return frame_offset_rx;	 //CLEAR MOTOR ERROR FLAG COMMAND


}
struct can_frame Write_Encoder_Offset_tx_rx(int s, int id, int Enc_Offest) //////////////////////////////////function 13
{

  Write_Encoder_Offset_tx( s, id,  Enc_Offest);
  Write_Encoder_Offset_rx( id,  s);
}






//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void read_Motor_Status1_AND_Error_Flag_tx(int id, int s) ////////////////////// FUNCTION 11
{
    struct can_frame frame_MTRStatus1_tx;

    frame_MTRStatus1_tx.can_id = id;
    frame_MTRStatus1_tx.can_dlc = FRAME_LEN;
    frame_MTRStatus1_tx.data[0] = RD_MTRSTATUS1_ERRORFLAG;
    frame_MTRStatus1_tx.data[1] = 0x00;
    frame_MTRStatus1_tx.data[2] = 0x00;
    frame_MTRStatus1_tx.data[3] = 0x00;
    frame_MTRStatus1_tx.data[4] = 0x00;
    frame_MTRStatus1_tx.data[5] = 0x00;
    frame_MTRStatus1_tx.data[6] = 0x00;
    frame_MTRStatus1_tx.data[7] = 0x00;

    if (write(s, &frame_MTRStatus1_tx, sizeof(struct can_frame)) != sizeof(struct can_frame))
    {
        perror("Write");
    }
}

struct can_frame read_Motor_Status1_AND_Error_Flag_rx(int id, int s) /////////////////////////////////////// FUNCTION 12
{
    struct can_frame frame_MTRStatus1_rx;

    int nbytes = read(s, &frame_MTRStatus1_rx, sizeof(struct can_frame));

    for (int i = 0; i < frame_MTRStatus1_rx.can_dlc; i++)
    {
        //printf("%02X ", frame_MTRStatus1_rx.data[i]);
    }
    unsigned short combinedMTRST0 = frame_MTRStatus1_rx.data[1];
    printf("\nMotor Temperature %d\n", combinedMTRST0); 
 
    unsigned short lsbMTRST1 = frame_MTRStatus1_rx.data[3];
    unsigned short msbMTRST1 = frame_MTRStatus1_rx.data[4];
    int combinedMTRST1 = (msbMTRST1 << 8) | lsbMTRST1;
    printf("\nVoltage %d\n", combinedMTRST1); 

    unsigned short combinedMTRST2 = frame_MTRStatus1_rx.data[7];   //////to be tested
    printf("\nError State Byte %d\n", combinedMTRST2); 
    return frame_MTRStatus1_rx;
    //return a struct of type can_frame
}




struct can_frame read_Motor_Status1_AND_Error_Flag_tx_rx(int id, int s) //////////////////////////////////function 13
{

  read_Motor_Status1_AND_Error_Flag_tx( id,  s);
  read_Motor_Status1_AND_Error_Flag_rx( id, s);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




void Clear_Motor_Error_Flag_tx(int id, int s) ////////////////////// FUNCTION 11
{
    struct can_frame frame_CLRMTR_tx;
    

    frame_CLRMTR_tx.can_id = id;
    frame_CLRMTR_tx.can_dlc = FRAME_LEN;
    frame_CLRMTR_tx.data[0] = CLR_MOTOR_ERRORFLAG;
    frame_CLRMTR_tx.data[1] = 0x00;
    frame_CLRMTR_tx.data[2] = 0x00;
    frame_CLRMTR_tx.data[3] = 0x00;
    frame_CLRMTR_tx.data[4] = 0x00;
    frame_CLRMTR_tx.data[5] = 0x00;
    frame_CLRMTR_tx.data[6] = 0x00;
    frame_CLRMTR_tx.data[7] = 0x00;

    if (write(s, &frame_CLRMTR_tx, sizeof(struct can_frame)) != sizeof(struct can_frame))
    {
        perror("Write");
    }
}

struct can_frame Clear_Motor_Error_Flag_rx(int id, int s) /////////////////////////////////////// FUNCTION 12
{
    struct can_frame frame_CLRMTR_rx;

    int nbytes = read(s, &frame_CLRMTR_rx, sizeof(struct can_frame));

    for (int i = 0; i < frame_CLRMTR_rx.can_dlc; i++)
    {
        //printf("%02X ", frame_CLRMTR_rx.data[i]);
    }
    unsigned short combinedCLRMTR = frame_CLRMTR_rx.data[1];
    printf("\nMotor Temperature %d\n", combinedCLRMTR); 
 
    unsigned short lsbCLRMTR = frame_CLRMTR_rx.data[3];
    unsigned short msbCLRMTR = frame_CLRMTR_rx.data[4];
    int combinedCLRMTR1 = (msbCLRMTR << 8) | lsbCLRMTR;
    printf("\nVoltage %d\n", combinedCLRMTR1); 

    unsigned short combinedCLRMTR2 = frame_CLRMTR_rx.data[7];   //////to be tested
    printf("\nError State Byte %d\n", combinedCLRMTR2); 
    return frame_CLRMTR_rx;
    //return a struct of type can_frame
}



struct can_frame Clear_Motor_Error_Flag_tx_rx(int id, int s) //////////////////////////////////function 13
{

  Clear_Motor_Error_Flag_tx( id,  s);
  Clear_Motor_Error_Flag_rx( id,  s);
}







//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Read_Motor_Status_2_tx(int id, int s) ////////////////////// FUNCTION 11
{
    struct can_frame frame_MTRStatus2_tx;
    

    frame_MTRStatus2_tx.can_id = id;
    frame_MTRStatus2_tx.can_dlc = FRAME_LEN;
    frame_MTRStatus2_tx.data[0] = RD_MTR_STATUS_2;
    frame_MTRStatus2_tx.data[1] = 0x00;
    frame_MTRStatus2_tx.data[2] = 0x00;
    frame_MTRStatus2_tx.data[3] = 0x00;
    frame_MTRStatus2_tx.data[4] = 0x00;
    frame_MTRStatus2_tx.data[5] = 0x00;
    frame_MTRStatus2_tx.data[6] = 0x00;
    frame_MTRStatus2_tx.data[7] = 0x00;

    if (write(s, &frame_MTRStatus2_tx, sizeof(struct can_frame)) != sizeof(struct can_frame))
    {
        perror("Write");
    }
}

struct can_frame Read_Motor_Status_2_rx(int id, int s) /////////////////////////////////////// FUNCTION 12
{
    struct can_frame frame_MTRStatus2_rx;

    int nbytes = read(s, &frame_MTRStatus2_rx, sizeof(struct can_frame));

    for (int i = 0; i < frame_MTRStatus2_rx.can_dlc; i++)
    {
        //printf("%02X ", frame_MTRStatus2_rx.data[i]);
    }
    unsigned short combinedMTRST3= frame_MTRStatus2_rx.data[1];
    printf("\nMotor Temperature %d\n", combinedMTRST3); 
 
    unsigned short lsbMTRSTT0 = frame_MTRStatus2_rx.data[2];
    unsigned short msbMTRSTT0 = frame_MTRStatus2_rx.data[3];
    int combinedMTRST4 = (msbMTRSTT0 << 8) | lsbMTRSTT0;
    printf("\nTorque Current %d\n", combinedMTRST4); 

    unsigned short lsbMTRSTT1 = frame_MTRStatus2_rx.data[4];
    unsigned short msbMTRSTT1 = frame_MTRStatus2_rx.data[5];
    int combinedMTRST5 = (msbMTRSTT1 << 8) | lsbMTRSTT1;
    printf("\nSpeed %d\n", combinedMTRST5);

    unsigned short lsbMTRSTT2 = frame_MTRStatus2_rx.data[6];
    unsigned short msbMTRSTT2 = frame_MTRStatus2_rx.data[7];
    int combinedMTRST6 = (msbMTRSTT2 << 8) | lsbMTRSTT2;
    printf("\nEncoder Position %d\n", combinedMTRST6);
    
    
     return frame_MTRStatus2_rx;
    //return a struct of type can_frame
}

struct can_frame Read_Motor_Status_2_tx_rx(int id, int s) //////////////////////////////////function 13
{

  Read_Motor_Status_2_tx( id,  s);
  Read_Motor_Status_2_rx( id,  s);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////

void Read_Motor_Status_3_tx(int id, int s) ////////////////////// FUNCTION 11
{
    struct can_frame frame_MTRStatus3_tx;
    

    frame_MTRStatus3_tx.can_id = id;
    frame_MTRStatus3_tx.can_dlc = FRAME_LEN;
    frame_MTRStatus3_tx.data[0] = RD_MTR_STATUS_3 ;
    frame_MTRStatus3_tx.data[1] = 0x00;
    frame_MTRStatus3_tx.data[2] = 0x00;
    frame_MTRStatus3_tx.data[3] = 0x00;
    frame_MTRStatus3_tx.data[4] = 0x00;
    frame_MTRStatus3_tx.data[5] = 0x00;
    frame_MTRStatus3_tx.data[6] = 0x00;
    frame_MTRStatus3_tx.data[7] = 0x00;

    if (write(s, &frame_MTRStatus3_tx, sizeof(struct can_frame)) != sizeof(struct can_frame))
    {
        perror("Write");
    }
}

struct can_frame Read_Motor_Status_3_rx(int id, int s) /////////////////////////////////////// FUNCTION 12
{
    struct can_frame frame_MTRStatus3_rx;

    int nbytes = read(s, &frame_MTRStatus3_rx, sizeof(struct can_frame));

    for (int i = 0; i < frame_MTRStatus3_rx.can_dlc; i++)
    {
        //printf("%02X ", frame_MTRStatus3_rx.data[i]);
    }
    unsigned short combinedMTRST7= frame_MTRStatus3_rx.data[1];
    printf("\nMotor Temperature %d\n", combinedMTRST7); 
 
    unsigned short lsbMTRST3 = frame_MTRStatus3_rx.data[2];
    unsigned short msbMTRST3 = frame_MTRStatus3_rx.data[3];
    int combinedMTRST8 = (msbMTRST3 << 8) | lsbMTRST3;
    printf("\nPhase A Current %d\n", combinedMTRST8); 

    unsigned short lsbMTRST4 = frame_MTRStatus3_rx.data[4];
    unsigned short msbMTRST4 = frame_MTRStatus3_rx.data[5];
    int combinedMTRST9 = (msbMTRST4 << 8) | lsbMTRST4;
    printf("\nPhase B Current %d\n", combinedMTRST9);

    unsigned short lsbMTRST5 = frame_MTRStatus3_rx.data[6];
    unsigned short msbMTRST5 = frame_MTRStatus3_rx.data[7];
    int combinedMTRST10 = (msbMTRST5 << 8) | lsbMTRST5;
    printf("\nPhase C Current %d\n", combinedMTRST10);
    
    
     return frame_MTRStatus3_rx;
    //return a struct of type can_frame
}

struct can_frame Read_Motor_Status_3_tx_rx(int id, int s) //////////////////////////////////function 13
{

  Read_Motor_Status_3_tx( id,  s);
  Read_Motor_Status_3_rx( id,  s);
}