/*
 *  File:    game_utils.c
 *  Author:  Flynn Doherty
 *  Date:    9 Oct 2018
 *  Descr:   Contains the game's IR communication functions.
 */


#include <avr/io.h>
#include <system.h>
#include "ir_uart.h"


/*
 *  Checks if it's okay to write and if so, sends the player's hand.
 *  @param player_hand to send over IR.
 */
void ir_send_hand(char player_hand)
{
    // If it's okay to write.
    if (ir_uart_write_ready_p ()) {
        ir_uart_putc (player_hand);
    }
}


/*
 *  Checks if the board has IR information to be read and if so, determines the
 *  the opposition's hand.
 *  @param opposition_hand a pass-through variable.
 *  @return the opposition's hand.
 */
char ir_recieve_hand(char opposition_hand)
{
    // If there is information ready to be read.
    if (ir_uart_read_ready_p ()) {
        char character;
        character = ir_uart_getc ();

        // If the recieved IR is a valid selection.
        if (character == 'R' || character == 'P' || character == 'S') {
            opposition_hand = character;
        }
    }

    return opposition_hand;
}
