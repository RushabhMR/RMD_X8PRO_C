
#pragma once
#include <linux/can/raw.h>
#include <stdio.h>
#include <linux/can.h>
#include <unistd.h>
#define RD_PID 0x30 // READ PID DATA COMMAND
#define FRAME_LEN 8
#define WR_PID_RAM 0x31 // WRITE PID TO RAM COMMAND

void read_PID_Data_tx(int id, int s) ////////////////////// FUNCTION 11
{
    struct can_frame frame_PIDTX;

    frame_PIDTX.can_id = id;
    frame_PIDTX.can_dlc = FRAME_LEN;
    frame_PIDTX.data[0] = RD_PID;
    frame_PIDTX.data[1] = 0x00;
    frame_PIDTX.data[2] = 0x00;
    frame_PIDTX.data[3] = 0x00;
    frame_PIDTX.data[4] = 0x00;
    frame_PIDTX.data[5] = 0x00;
    frame_PIDTX.data[6] = 0x00;
    frame_PIDTX.data[7] = 0x00;

    if (write(s, &frame_PIDTX, sizeof(struct can_frame)) != sizeof(struct can_frame))
    {
        perror("Write");
    }
}

struct can_frame read_PID_Data_rx(int id, int s) /////////////////////////////////////// FUNCTION 12
{
    struct can_frame frame_PIDRX;

    int nbytes = read(s, &frame_PIDRX, sizeof(struct can_frame));

    for (int i = 0; i < frame_PIDRX.can_dlc; i++)
    {
        //printf("%02X ", frame_PIDRX.data[i]);
    }
    unsigned short POS_KP = frame_PIDRX.data[2]; //Torque Current
    unsigned short POS_KI = frame_PIDRX.data[3];
    unsigned short SPEED_KP = frame_PIDRX.data[4]; //Speed
    unsigned short SPEED_KI = frame_PIDRX.data[5];
    unsigned short TORQUE_KP = frame_PIDRX.data[6]; //ENcoder Data
    unsigned short TORQUE_KI = frame_PIDRX.data[7];

    printf("\nPOSITION LOOP KP %d\n", POS_KP);
    printf("\nPOSITION LOOP KI %d\n", POS_KI);
    printf("\nSPEED LOOP KP %d\n", SPEED_KP);
    printf("\nSPEED LOOP KI %d\n", SPEED_KI);
    printf("\nTORQUE LOOP KP %d\n", TORQUE_KP);
    printf("\nTORQUE LOOP KI %d\n", TORQUE_KI);
    return frame_PIDRX;
    //return a struct of type can_frame
}

struct can_frame read_PID_Data_tx_rx(int s, int id) //////////////////////////////////function 13
{

    read_PID_Data_tx(id, s);
    read_PID_Data_rx(id, s);
}

/////////////////////////////////////////////////////////////////////////////////////////////

void write_PID_to_RAM_tx(int id, int s, int Pos_loop_kp, int Pos_loop_ki, int speed_loop_kp, int speed_loop_ki, int Torque_loop_kp, int Torque_loop_ki)

////////////////////// FUNCTION 21
{
    struct can_frame frame_PID_RAM_tx;

    frame_PID_RAM_tx.can_id = id;
    frame_PID_RAM_tx.can_dlc = FRAME_LEN;
    frame_PID_RAM_tx.data[0] = WR_PID_RAM;
    frame_PID_RAM_tx.data[1] = 0x00;
    frame_PID_RAM_tx.data[2] = Pos_loop_kp;
    frame_PID_RAM_tx.data[3] = Pos_loop_ki;
    frame_PID_RAM_tx.data[4] = speed_loop_ki;
    frame_PID_RAM_tx.data[5] = speed_loop_ki;
    frame_PID_RAM_tx.data[6] = Torque_loop_ki;
    frame_PID_RAM_tx.data[7] = Torque_loop_ki;

    if (write(s, &frame_PID_RAM_tx, sizeof(struct can_frame)) != sizeof(struct can_frame))
    {
        perror("Write");
    }
}

struct can_frame write_PID_to_RAM_rx(int id, int s) /////////////////////////////////////// FUNCTION 22
{
    struct can_frame frame_PID_RAM_RX;

    int nbytes = read(s, &frame_PID_RAM_RX, sizeof(struct can_frame));

    for (int i = 0; i < frame_PID_RAM_RX.can_dlc; i++)
    {
        //printf("%02X ", frame_PID_RAM_RX.data[i]);
    }
    unsigned short POS_KP = frame_PID_RAM_RX.data[2]; //Torque Current
    unsigned short POS_KI = frame_PID_RAM_RX.data[3];
    unsigned short SPEED_KP = frame_PID_RAM_RX.data[4]; //Speed
    unsigned short SPEED_KI = frame_PID_RAM_RX.data[5];
    unsigned short TORQUE_KP = frame_PID_RAM_RX.data[6]; //ENcoder Data
    unsigned short TORQUE_KI = frame_PID_RAM_RX.data[7];

    printf("\nPOSITION LOOP KP %d\n", POS_KP);
    printf("\nPOSITION LOOP KI %d\n", POS_KI);
    printf("\nSPEED LOOP KP %d\n", SPEED_KP);
    printf("\nSPEED LOOP KI %d\n", SPEED_KI);
    printf("\nTORQUE LOOP KP %d\n", TORQUE_KP);
    printf("\nTORQUE LOOP KI %d\n", TORQUE_KI);
    return frame_PID_RAM_RX;
    //return a struct of type can_frame
}

struct can_frame write_PID_to_RAM_tx_rx(int id, int s, int Pos_loop_kp, int Pos_loop_ki, int speed_loop_kp, int speed_loop_ki, int Torque_loop_kp, int Torque_loop_ki) 
{

     write_PID_to_RAM_tx(id,s,Pos_loop_kp, Pos_loop_ki, speed_loop_kp,speed_loop_ki, Torque_loop_kp, Torque_loop_ki);  //////////////////////////////////function 23
    write_PID_to_RAM_rx(id, s);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////



void write_PID_to_ROM_tx(int id, int s, int Pos_loop_kp_ROM, int Pos_loop_ki_ROM, int speed_loop_kp_ROM, int speed_loop_ki_ROM, int Torque_loop_kp_ROM, int Torque_loop_ki_ROM)

                                                ////////////////////// FUNCTION 21
{
    struct can_frame frame_PID_ROM_tx;

    frame_PID_ROM_tx.can_id = id;
    frame_PID_ROM_tx.can_dlc = FRAME_LEN;
    frame_PID_ROM_tx.data[0] = WR_PID_RAM;
    frame_PID_ROM_tx.data[1] = 0x00;
    frame_PID_ROM_tx.data[2] = Pos_loop_kp_ROM;
    frame_PID_ROM_tx.data[3] = Pos_loop_ki_ROM;
    frame_PID_ROM_tx.data[4] = speed_loop_ki_ROM;
    frame_PID_ROM_tx.data[5] = speed_loop_ki_ROM;
    frame_PID_ROM_tx.data[6] = Torque_loop_ki_ROM;
    frame_PID_ROM_tx.data[7] = Torque_loop_ki_ROM;

    if (write(s, &frame_PID_ROM_tx, sizeof(struct can_frame)) != sizeof(struct can_frame))
    {
        perror("Write");
    }
}

struct can_frame write_PID_to_ROM_rx(int id, int s) /////////////////////////////////////// FUNCTION 22
{
    struct can_frame frame_PID_ROM_RX;

    int nbytes = read(s, &frame_PID_ROM_RX, sizeof(struct can_frame));

    for (int i = 0; i < frame_PID_ROM_RX.can_dlc; i++)
    {
        //printf("%02X ", frame_PID_ROM_RX.data[i]);
    }
    unsigned short POS_KP_ROM = frame_PID_ROM_RX.data[2]; //Torque Current
    unsigned short POS_KI_ROM = frame_PID_ROM_RX.data[3];
    unsigned short SPEED_KP_ROM = frame_PID_ROM_RX.data[4]; //Speed
    unsigned short SPEED_KI_ROM = frame_PID_ROM_RX.data[5];
    unsigned short TORQUE_KP_ROM = frame_PID_ROM_RX.data[6]; //ENcoder Data
    unsigned short TORQUE_KI_ROM = frame_PID_ROM_RX.data[7];

    printf("\nPOSITION LOOP KP %d\n", POS_KP_ROM);
    printf("\nPOSITION LOOP KI %d\n", POS_KI_ROM);
    printf("\nSPEED LOOP KP %d\n", SPEED_KP_ROM);
    printf("\nSPEED LOOP KI %d\n", SPEED_KI_ROM);
    printf("\nTORQUE LOOP KP %d\n", TORQUE_KP_ROM);
    printf("\nTORQUE LOOP KI %d\n", TORQUE_KI_ROM);
    return frame_PID_ROM_RX;
    //return a struct of type can_frame
}

struct can_frame write_PID_to_ROM_tx_rx(int id, int s, int Pos_loop_kp_ROM, int Pos_loop_ki_ROM, int speed_loop_kp_ROM, int speed_loop_ki_ROM, int Torque_loop_kp_ROM, int Torque_loop_ki_ROM) 
{

     write_PID_to_ROM_tx(id,s,Pos_loop_kp_ROM, Pos_loop_ki_ROM, speed_loop_kp_ROM,speed_loop_ki_ROM, Torque_loop_kp_ROM, Torque_loop_ki_ROM);  //////////////////////////////////function 23
    write_PID_to_ROM_rx(id, s);
}

