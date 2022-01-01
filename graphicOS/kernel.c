void onScreenFull();

#include "types.h"

uint8 BGcolor = BLUE;

#include "strings.c"
#include "sys.c"
#include "stdio.c"
#include "command_line.c"
#include "filesystem.c"

char *current_directory = "ROOT";
char typed_phrase[80];
int typed_phrase_index = 0;
int keyPressed = 'A';

int correspondingChar;

void kernel_entry()
{
    disableCursor();
    onScreenFull();
    println('s', "CoolOS - Version 1.1");
    println('s', "Type \"help\" for a list of commands");
    printNewFormattedLine();

    // TEST READ SECTION

    // int i;
    // i = 0;

    // uint32 bwrite[512];
    // for(i = 0; i < 512; i++)
    // {
    //     bwrite[i] = 0x48;
    //     //printf('c', 'B');
    // }
    // write_sectors_ATA_PIO(0x0, 1, bwrite);

    // read_sectors_ATA_PIO(0x0, 1);
    // while(i < 256)
    // {
    //     printf('c', 14 + (target[i] & 0xFF));
    //     printf('c', 14 + ((target[i] >> 8) & 0xFF));
    //     i++;
    // }
    //END TEST READ SECTION



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
        sleep(67000000);
    }
}

void onScreenFull()
{
    //saveBGcolor();
    BGcolor = BLUE;
    fillScreen(BGcolor);
    window_renderWindow_uncompensated(current_directory, 0, 0, 77, 23);
    resetCharAt();
    //restoreBGcolor();
}
