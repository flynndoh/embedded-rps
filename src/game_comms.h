/*
 *  File:    game_comms.h
 *  Author:  Flynn Doherty
 *  Date:    9 Oct 2018
 *  Descr:   This is the header file for some the game's IR communication module.
 */


#ifndef GAME_COMMS_H
#define GAME_COMMS_H


/*
 *  Checks if it's okay to write and if so, sends the player's hand.
 *  @param hand to send over IR.
 */
void ir_send_hand(char player_hand);


/*
 *  Checks if the board has IR information to be read and if so, determines the
 *  the opposition's hand.
 *  @param opposition_hand a pass-through variable.
 *  @return the opposition's hand.
 */
char ir_recieve_hand(char opposition_hand);


#endif
