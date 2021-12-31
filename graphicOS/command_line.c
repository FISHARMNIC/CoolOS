char command[80];
char parameter_1[80];
char parameter_2[80];

char all_commands[2][40] = {
    "echo - repeat your command",
    "help - list all commands  "};

int amount_of_commands = 2;

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
    else
    {
        unknownCommand();
    }
    restoreBGcolor();
}