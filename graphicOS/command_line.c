char command[80];
char parameter_1[80];
char parameter_2[80];

char all_commands[4][40] = {
    "echo - repeat your command",
    "help - list all commands  ",
    "cls  - clear the screen   ",
    "writeDisk - test",
    };

int amount_of_commands = 4;

void runCommands();

// #region COMMANDS
void echo()
{
    BGcolor = GREEN;
    println('s', parameter_1);
}

void help()
{
    BGcolor = GREEN;
    for (int i = 0; i < amount_of_commands; i++)
    {
        println('s', all_commands + i);
    }
}

void cls() {
    onScreenFull();
}

void writeDisk() {
    cls();
    int i;
    i = 0;

    uint32 bwrite[512];
    for(i = 0; i < 512; i++)
    {
        bwrite[i] = 0x48;
        //printf('c', 'B');
    }
    write_sectors_ATA_PIO(0x0, 1, bwrite);

    read_sectors_ATA_PIO(0x0, 1);
    i = 0;
    while(i < 256)
    {
        printf('c', 14 + (target[i] & 0xFF));
        printf('c', 14 + ((target[i] >> 8) & 0xFF));
        i++;
    }
}

void unknownCommand()
{
    BGcolor = RED;
    printf('s', "Unknown command \"");
    printf('s', command);
    printf('c', '"');
    printNewFormattedLine();
}
// #endregion

void parseCommand(char *phrase)
{
    //clear
    charArrayFill(command, 80, 0);
    charArrayFill(parameter_1, 80, 0);
    charArrayFill(parameter_2, 80, 0);

    int i = 0;
    for (; phrase[i] != ' ' && phrase[i] != 0; i++)
    {
        command[i] = phrase[i];
    }
    i += 1;
    for (int j = 0; phrase[i] != ' ' && phrase[i] != 0; i++, j++)
    {
        parameter_1[j] = phrase[i];
    }
    i += 1;
    for (int j = 0; phrase[i] != ' ' && phrase[i] != 0; i++, j++)
    {
        parameter_2[j] = phrase[i];
    }

    runCommands();
}

void runCommands()
{
    saveBGcolor();
    if (strcmp(command, "ECHO"))
    {
        echo();
    }
    else if (strcmp(command, "HELP"))
    {
        help();
    }
    else if (strcmp(command, "CLS"))
    {
        cls();
    }
    else if (strcmp(command, "WRITEDISK"))
    {
        writeDisk();
    }
    else
    {
        unknownCommand();
    }
    restoreBGcolor();
}