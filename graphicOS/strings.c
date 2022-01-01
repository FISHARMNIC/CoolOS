#define FALSE 0
#define TRUE 1

int len(char *S)
{
    // Variable for traversal
    int i = 0;

    // Traverse till null is reached
    while (S[i])
        i++;

    return i;
}

int strcmp(char S1[], char S2[])
{
    // If lengths of the two
    // strings are different
    if (len(S1) != len(S2))
        return FALSE;

    // Variable for traversal
    int i = 0;

    // Traverse till null is reached
    while (S1[i])
    {

        if (S1[i] != S2[i])
            return FALSE;

        // Increment i
        i++;
    }
    return TRUE;
}

void charArrayFill(char *array, int length, char source) {
    for(int i =0; i<length; i++) {
        array[i] = source;
    }
}

void strcpy(char *destination, char *source)
{
    int index = 0;

    while (destination[index] != '\0')
    {
        source[index] = destination[index];
        index++;
    }
    for (index = 0; index < 10; index++)
    {
        source[index] = '\0';
    }
}