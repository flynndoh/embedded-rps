/*
 *  File:    game_utils.c
 *  Author:  Flynn Doherty
 *  Date:    9 Oct 2018
 *  Descr:   Contains some basic text display and button press utilities.
 */


#include <avr/io.h>
#include <system.h>
#include "pacer.h"
#include "tinygl.h"
#include "navswitch.h"


/*
 *  Detects if the lower left button on PIND has been pressed.
 *  @return a boolean.
 */
int restart_game_button_pressed(void)
{
    return PIND & (1<<7);
}


/*
 *  Displays a given character on the LED matrix.
 *  @param character to display.
 */
void display_character (char character)
{
    char buffer[2];
    buffer[0] = character;
    buffer[1] = '\0';
    tinygl_text (buffer);
}


/*
 *  Displays some given text on a scrolling banner on the LED matrix.
 *  @param result_text to display.
 */
void display_result_banner (char* result_text)
{
    // Prepare LED matrix for scrolling text.
    tinygl_clear();
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text (result_text);

    // Keep scrolling the result text until the user breaks out.
    int bool_text_scrolling = 1;
    while (bool_text_scrolling) {
        pacer_wait ();

        // If the user presses the restart game button.
        if (restart_game_button_pressed ()) {
            tinygl_clear ();
            bool_text_scrolling = 0;
        }

        tinygl_update ();
    }
}
