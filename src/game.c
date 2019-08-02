/*
 *  File:    game.c
 *  Author:  Flynn Doherty
 *  Date:    8 Oct 2018
 *  Descr:   The main game module. Uses the game_utils and game_comms modules.
 *           The game of choice is rock, paper, scissors. This game is two player
 *           and requires two UCFK4s to play. Refer to the README for more info.
 */


#include <avr/io.h>
#include <system.h>
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"
#include "game_utils.h"
#include "game_comms.h"


// Magic numbers
#define PACER_RATE 500
#define MESSAGE_RATE 15


/*
 *  Initialises the LED matrix, navswitch, onboard buttons IR communications,
 *  and the blue status LED.
 */
void init (void)
{
    // Call to various initialisation functions.
    system_init ();
    tinygl_init (PACER_RATE);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    pacer_init (PACER_RATE);
    navswitch_init ();
    ir_uart_init ();

    // Initialise the game restart button.
    DDRD &=~ (1<<7);

    // Initialise the blue led.
    DDRC |= (1 << 2);
}


/*
 *  Displays a scrollable selecion UI that enables the player to choose their
 *  hand to play.
 *  @return the player's chosen hand.
 */
char choose_hand (void)
{
    // Declare local variables.
    int index = 0;
    int bool_waiting_for_choice = 1;
    const char options[3] = {'P', 'R', 'S'};

    // While the user hasn't selected their hand.
    while (bool_waiting_for_choice) {
        pacer_wait ();
        navswitch_update ();

        // If the navswitch is pressed in any direction.
        if (navswitch_push_event_p (NAVSWITCH_NORTH) ||
            navswitch_push_event_p (NAVSWITCH_SOUTH) ||
            navswitch_push_event_p (NAVSWITCH_EAST)  ||
            navswitch_push_event_p (NAVSWITCH_WEST)) {

            // Update the selection index in mod 3, as there are only 3 options.
            index = (index + 1) % 3;
        }

        // Display the character on the LED matrix.
        display_character (options[index]);

        // If the user has made a selection by pressing the navswitch down.
        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            tinygl_clear ();
            bool_waiting_for_choice = 0;
        }

        tinygl_update ();
    }

    return options[index];
}


/*
 *  Gets the opposition's hand and sends the player's hand. This performs a
 *  handshake of sorts. The blue LED will be active while waiting to receive the
 *  opposition's hand.
 *  @return the opposition's chosen hand.
 */
char get_opposition_hand (char player_hand)
{
    // Set the opposition's hand to N (null).
    char opposition_hand = 'N';

    // While the opposition's hand hasn't been set.
    while (opposition_hand == 'N') {
        pacer_wait ();

        // Turn on blue LED.
        PORTC |= (1 << 2);

        // Attempt to perform the handshake.
        opposition_hand = ir_recieve_hand(opposition_hand);
        ir_send_hand (player_hand);
    }

    // Turn off blue LED.
    PORTC &= ~(1 << 2);

    return opposition_hand;
}


/*
 *  Determine the winner of two given hands and prompt the user to play again.
 *  @param hand the player's hand.
 *  @param opposition_hand the opposition's hand.
 *  @return the result string.
 */
char* check_winner (char hand, char opposition_hand)
{
    char* result_text;

    // If both hands are the same.
    if (hand == opposition_hand) {
        result_text = "Tie! Press to play again.";

    // Otherwise, check determine who has won.
    } else if ((hand == 'R' && opposition_hand == 'S') ||
               (hand == 'P' && opposition_hand == 'R') ||
               (hand == 'S' && opposition_hand == 'P')) {

        result_text = "Winner! Press to play again.";

    // Otherwise, the player has lost.
    } else {
        result_text = "Loser! Press to play again.";
    }

    return result_text;
}


/*
 *  The heartbeat of the game, calls to all other funtions.
 *  @return 0 as per style guidelines.
 */
int main (void)
{
    // Declare local variables.
    char player_hand;
    char opposition_hand;
    char* result_text;

    // Initialise the required UCFK components.
    init ();

    // The main game loop, runs until the board loses power.
    while (1) {
        // Select a hand.
        player_hand = choose_hand ();

        // Get the opposition's hand.
        opposition_hand = get_opposition_hand (player_hand);

        // Determine the winner.
        result_text = check_winner (player_hand, opposition_hand);

        // Display the appropriate win/loss/tie text.
        display_result_banner (result_text);
    }

    return 0;
}
