// #region ----- INIT -----
#include "stdarg.h"

uint16 *vgabuffer = (uint16 *)VGA_ADDRESS;
const int indentation = 1;
const int vIndentation = 1;
int charAt = indentation + 80 * vIndentation;

#define MAX_WINDOWS 10
int windowAttachments[MAX_WINDOWS];
int windowXpositions[MAX_WINDOWS];
int windowYpositions[MAX_WINDOWS];
int windowWidths[MAX_WINDOWS];
int windowHeights[MAX_WINDOWS];

int appPointers[100];
int nextOpenAppID = 0;

uint16 formatVGA(uint8 ch, uint8 FC, uint8 BC)
{
    uint16 ax = 0;
    uint8 ah = 0, al = 0;

    ah = BC;
    ah <<= 4;
    ah |= FC;
    ax = ah;
    ax <<= 8;
    al = ch;
    ax |= al;

    return ax; //backcolor,forecolore,ch
}

// #endregion

// #region ----- STOUT -----
int printAt(int printLOC, char type, ...)
{
    va_list ap;
    va_start(ap, type);
    if (type == 'c')
    {
        int pChar = va_arg(ap, int);
        vgabuffer[printLOC] = formatVGA(pChar, WHITE, BGcolor);
        return 1;
    }
    else if (type == 's')
    {
        char *pString = va_arg(ap, char *);

        int wordIndex = 0;
        int wordLen = len(pString);

        while (wordIndex < wordLen)
        {
            vgabuffer[printLOC] = formatVGA(pString[wordIndex], WHITE, BGcolor);
            printLOC++;
            wordIndex++;
        }
        return 1;
    }
    else if (type == 'i')
    {
        int pInt = va_arg(ap, int);
        int div;
        int oLen = 0;
        for (div = 1; div <= pInt; div *= 10)
            ;
        do
        {
            div /= 10;
            vgabuffer[printLOC] = formatVGA(pInt / div + 48, WHITE, BGcolor);
            pInt %= div;
            printLOC++;
            oLen++;
        } while (pInt);
        return oLen;
    }
    va_end(ap);
    return 0;
}

void printf(char type, ...)
{
    va_list ap;
    va_start(ap, type);

    if (type == 'c')
    {
        int pChar = va_arg(ap, int);
        printAt(charAt, type, pChar);
        charAt++;
    }
    else if (type == 's')
    {
        char *pString = va_arg(ap, char *);
        printAt(charAt, type, pString);
        charAt += len(pString);
    }
    else if (type == 'i')
    {
        int pInt = va_arg(ap, int);
        charAt += printAt(charAt, type, pInt);
    }
}

void printXY(int posX, int posY, char type, ...)
{
    va_list ap;
    va_start(ap, type);
    int compPos = (posX % 80) + (posY * 80);
    if (type == 'c')
    {
        int pChar = va_arg(ap, int);
        printAt(compPos, type, pChar);
        charAt++;
    }
    else if (type == 's')
    {
        char *pString = va_arg(ap, char *);
        printAt(compPos, type, pString);
        charAt += len(pString);
    }
    else if (type == 'i')
    {
        int pInt = va_arg(ap, int);
        charAt += printAt(compPos, type, pInt);
    }
}

void resetCharAt() {
    charAt = indentation + 80 * vIndentation;
}

void printNewLine()
{
    charAt = charAt + (80 - (charAt % 80));
    if(charAt > 2000) {
        onScreenFull();
    }
}

void printNewFormattedLine()
{
    printNewLine();
    charAt += indentation;
}

void println(char type, ...) {
    va_list ap;
    va_start(ap, type);

    if (type == 'c')
    {
        int pChar = va_arg(ap, int);
        printAt(charAt, type, pChar);
        charAt++;
    }
    else if (type == 's')
    {
        char *pString = va_arg(ap, char *);
        printAt(charAt, type, pString);
        charAt += len(pString);
    }
    else if (type == 'i')
    {
        int pInt = va_arg(ap, int);
        charAt += printAt(charAt, type, pInt);
    }
    printNewFormattedLine();
}

void fillScreen(uint8 color) {
    for(int i =0; i < 80 * 25; i++) {
        vgabuffer[i] = formatVGA(0,0,color);
    }
}
// #endregion

// #region ----- 2D GRAPHICS -----
void gfx_drawBox(int startX, int startY, int width, int height)
{
    int saveCharAt = charAt;

    //correction for taller chars
    height = (int)(height / 2);
    startY = (int)(startY / 2);

    int parsedStart = (startX % 80) + (startY * 80);
    charAt = parsedStart;

    printf('c', TOP_LEFT_ELBOW);

    int itIndex = 0;
    while (itIndex < width)
    {
        printf('c', HORIZONTAL_LINE);
        itIndex++;
    }
    printf('c', TOP_RIGHT_ELBOW);

    itIndex = 0;
    int itIndex2 = 0;
    while (itIndex < height)
    {
        printNewLine();
        charAt += startX % 80;
        printf('c', VERTICAL_LINE);
        itIndex2 = 0;
        while(itIndex2 < width) {
            vgabuffer[charAt] = formatVGA(0, 0, BLACK);
            itIndex2++;
            charAt++;
        }
        //charAt += width;
        printf('c', VERTICAL_LINE);
        itIndex++;
    }

    itIndex = 0;
    printNewLine();
    charAt += startX % 80;
    printf('c', BOTTOM_LEFT_ELBOW);
    while (itIndex < width)
    {
        printf('c', HORIZONTAL_LINE);
        itIndex++;
    }
    printf('c', BOTTOM_RIGHT_ELBOW);
    charAt = saveCharAt; //restore
}

void gfx_drawBox_uncompensated(int startX, int startY, int width, int height)
{
    gfx_drawBox(startX, startY * 2, width, height * 2);
}
// #endregion

// #region ----- WINDOW GFX -----
void window_renderWindow(char *title, int startX, int startY, int width, int height)
{
    gfx_drawBox(startX, startY, width, height);
    printXY(startX, startY / 2, 's', title);
}

void window_renderWindow_uncompensated(char *title, int startX, int startY, int width, int height)
{
    gfx_drawBox_uncompensated(startX, startY, width, height);
    printXY(startX, startY, 's', title);
}
// #endregion

// #region ----- WINDOW SERVER -----
int createWindow(int attachmentID, int startX, int startY, int width, int height)
{
    // each window is attached to an app
    windowAttachments[attachmentID] = attachmentID;
    windowXpositions[attachmentID] = startX;
    windowYpositions[attachmentID] = startY;
    windowWidths[attachmentID] = width;
    windowHeights[attachmentID] = height;
    return attachmentID;
}
// #endregion

// #region ----- STDIN -----
uint8 inb(uint16 port)
{
    uint8 ret;
    asm volatile("inb %1, %0"
                 : "=a"(ret)
                 : "d"(port));
    return ret;
}

int awaitKeyboard() {
    char ch = 0;
    while ((ch = inb(KEYBOARD_PORT)) != 0)
    {
        if (ch > 0)
            return ch;
    }
    return ch;
}
// #endregion