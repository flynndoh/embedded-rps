/*
 *  File:    game_utils.h
 *  Author:  Flynn Doherty
 *  Date:    9 Oct 2018
 *  Descr:   This is the header file for some basic text display and button press
 *           utilities.
 */


#ifndef GAME_UTILS_H
#define GAME_UTILS_H


/*
 *  Detects if the lower left button on PIND has been pressed.
 *  @return a boolean.
 */
int restart_game_button_pressed(void);


/*
 *  Displays a given character on the LED matrix.
 *  @param character to display.
 */
void display_character (char character);


/*
 *  Displays some given text on a scrolling banner on the LED matrix.
 *  @param result_text text to display.
 */
void display_result_banner (char* result_text);


#endif
