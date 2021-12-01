#pragma once
#include <linux/can/raw.h>
#include <stdio.h>
#include <linux/can.h>
#include <unistd.h>
#define RD_PID 0x30  // READ PID DATA COMMAND
#define RD_ACCN 0X33 //READ ACCELERATION DATA COMMAND
#define WR_ACCN_RAM 0X34			 //WRITE ACCELERATION DATA TO RAM COMMAND
#define FRAME_LEN 8
#define WR_PID_RAM 0x31
// WRITE PID TO RAM COMMAND

void read_Accn_Data_tx(int id, int s) ////////////////////// FUNCTION 11
{
    struct can_frame frame_Accn_tx;

    frame_Accn_tx.can_id = id;
    frame_Accn_tx.can_dlc = FRAME_LEN;
    frame_Accn_tx.data[0] = RD_ACCN;
    frame_Accn_tx.data[1] = 0x00;
    frame_Accn_tx.data[2] = 0x00;
    frame_Accn_tx.data[3] = 0x00;
    frame_Accn_tx.data[4] = 0x00;
    frame_Accn_tx.data[5] = 0x00;
    frame_Accn_tx.data[6] = 0x00;
    frame_Accn_tx.data[7] = 0x00;

    if (write(s, &frame_Accn_tx, sizeof(struct can_frame)) != sizeof(struct can_frame))
    {
        perror("Write");
    }
}

struct can_frame read_Accn_Data_rx(int id, int s) /////////////////////////////////////// FUNCTION 12
{
    struct can_frame frame_Accn_rx;

    int nbytes = read(s, &frame_Accn_rx, sizeof(struct can_frame));

    for (int i = 0; i < frame_Accn_rx.can_dlc; i++)
    {
        //printf("%02X ", frame_Accn_rx.data[i]);
    }

    unsigned short lsbaccn0 = frame_Accn_rx.data[4];
    unsigned short msbaccn0 = frame_Accn_rx.data[5];
    unsigned int combinedaccn0 = (msbaccn0 << 8) | lsbaccn0;

    unsigned short lsbaccn1 = combinedaccn0;
    unsigned short msbaccn1 = frame_Accn_rx.data[6];
    short combinedaccn1 = (msbaccn1 << 8) | lsbaccn1;

    unsigned short lsbaccn2 = combinedaccn1;
    unsigned short msbaccn2 = frame_Accn_rx.data[7];
    int combinedaccn2 = (msbaccn2 << 8) | lsbaccn2;

    // unsigned short POS_P = frame_Accn_rx.data[2];
    // unsigned short POS_I = frame_Accn_rx.data[3];
    // unsigned short SPEED_P = frame_Accn_rx.data[4];
    // unsigned short SPEED_I = frame_Accn_rx.data[5];
    // unsigned short TORQUE_P = frame_Accn_rx.data[6];
    // unsigned short TORQUE_I = frame_Accn_rx.data[7];

    printf("\nReading accn Data %d\n", combinedaccn2);
    // printf("\nPOSITION LOOP I %d\n", POS_I);
    // printf("\nSPEED LOOP P %d\n", SPEED_P);
    // printf("\nSPEED LOOP I %d\n", SPEED_I);
    // printf("\nTORQUE LOOP P %d\n", TORQUE_P);
    // printf("\nTORQUE LOOP I %d\n", TORQUE_I);
    return frame_Accn_rx;
    //return a struct of type can_frame
}

struct can_frame read_Accn_Data_tx_rx(int s, int id) //////////////////////////////////function 13
{

    read_Accn_Data_tx(id, s);
    read_Accn_Data_rx(id, s);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void write_Accn_to_RAM_tx(int id, int s, int Accn_input ) ////////////////////// FUNCTION 11
{
    struct can_frame frame_Accn_RAM_tx;
    const int stored = (Accn_input*6);
	const int fourr = (stored & 0x000000FF) >> 0;
	const int fivee = (stored & 0x0000FF00) >> 8;
	const int sixx = (stored & 0x00FF0000) >> 16;
	const int sevenn = (stored & 0xFF000000) >> 24;

    frame_Accn_RAM_tx.can_id = id;
    frame_Accn_RAM_tx.can_dlc = FRAME_LEN;
    frame_Accn_RAM_tx.data[0] = WR_ACCN_RAM;
    frame_Accn_RAM_tx.data[1] = 0x00;
    frame_Accn_RAM_tx.data[2] = 0x00;
    frame_Accn_RAM_tx.data[3] = 0x00;
    frame_Accn_RAM_tx.data[4] = fourr;
    frame_Accn_RAM_tx.data[5] = fivee;
    frame_Accn_RAM_tx.data[6] = sixx;
    frame_Accn_RAM_tx.data[7] = sevenn;

    if (write(s, &frame_Accn_RAM_tx, sizeof(struct can_frame)) != sizeof(struct can_frame))
    {
        perror("Write");
    }
}


struct can_frame read_Accn_from_RAM_rx(int id, int s) /////////////////////////////////////// FUNCTION 12
{
    struct can_frame frame_Accn_RAM_rx;

    int nbytes = read(s, &frame_Accn_RAM_rx, sizeof(struct can_frame));

    for (int i = 0; i < frame_Accn_RAM_rx.can_dlc; i++)
    {
        //printf("%02X ", frame_Accn_RAM_rx.data[i]);
    }

    unsigned short lsbaccn0 = frame_Accn_RAM_rx.data[4];
    unsigned short msbaccn0 = frame_Accn_RAM_rx.data[5];
    unsigned int combinedaccn0 = (msbaccn0 << 8) | lsbaccn0;

    unsigned short lsbaccn1 = combinedaccn0;
    unsigned short msbaccn1 = frame_Accn_RAM_rx.data[6];
    short combinedaccn1 = (msbaccn1 << 8) | lsbaccn1;

    unsigned short lsbaccn2 = combinedaccn1;
    unsigned short msbaccn2 = frame_Accn_RAM_rx.data[7];
    int combinedaccn2 = (msbaccn2 << 8) | lsbaccn2;

    printf("\nReading accn Data %d\n", combinedaccn2);
   
    return frame_Accn_RAM_rx;
    //return a struct of type can_frame
}

struct can_frame RDWR_Accn_Data_tx_rx(int s, int id, int Accn_input) //////////////////////////////////function 13
{

    write_Accn_to_RAM_tx( id, s, Accn_input );
    read_Accn_from_RAM_rx( id,  s);
}