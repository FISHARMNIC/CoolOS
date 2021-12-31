void onScreenFull();

#include "types.h"

uint8 BGcolor = BLUE;

#include "strings.c"
#include "sys.c"
#include "stdio.c"
#include "command_line.c"

char *current_directory = "ROOT";
char typed_phrase[80];
int typed_phrase_index = 0;
int keyPressed = 'A';

int correspondingChar;

void kernel_entry()
{
    onScreenFull();
    println('s', "CoolOS - Version 1.1");
    println('s', "Type \"help\" for a list of commands");
    printNewFormattedLine();

    while (TRUE)
    {

        keyPressed = awaitKeyboard();
        if (keyPressed == KEY_ENTER)
        {
            printNewFormattedLine();
            parseCommand(typed_phrase);
            typed_phrase_index = 0;
            charArrayFill(typed_phrase, 80, 0 );
        }
        else if (keyPressed == KEY_BACKSPACE)
        {
            // delete on data side
            typed_phrase_index--;
            typed_phrase[typed_phrase_index] = 0;
            // delete on visual side
            charAt--;
            vgabuffer[charAt] = formatVGA(0, BLACK, BLACK);
        }
        else
        {
            correspondingChar = get_ascii_char(keyPressed);
            printf('c', correspondingChar);
            typed_phrase[typed_phrase_index] = (char) correspondingChar;
            typed_phrase_index++;
        }
        sleep(67108863);
    }
}

void onScreenFull()
{
    saveBGcolor();
    BGcolor = BLUE;
    fillScreen(BGcolor);
    window_renderWindow_uncompensated(current_directory, 0, 0, 77, 23);
    resetCharAt();
    restoreBGcolor();
}
