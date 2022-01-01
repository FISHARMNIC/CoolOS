void sleep(uint32 timer_count)
{
    while (1)
    {
        asm volatile("nop");
        timer_count--;
        if (timer_count <= 0)
            break;
    }
}

uint8 _oldColor;
void saveBGcolor() {
    _oldColor = BGcolor;
}

void restoreBGcolor() {
    BGcolor = _oldColor;
}

uint8 inb(uint16 port)
{
    uint8 ret;
    asm volatile("inb %1, %0"
                 : "=a"(ret)
                 : "d"(port));
    return ret;
}

uint16 inbWord(uint16 port)
{
    uint16 ret;
    asm volatile("inw %1, %0"
                 : "=a"(ret)
                 : "d"(port));
    return ret;
}

void outb(uint16 port, uint8 data)
{
    asm volatile("outb %0, %1"
                 : "=a"(data)
                 : "d"(port));
}

void outbLong(uint16 port, uint32 data)
{
    asm volatile("outl %0, %1"
                 : "=a"(data)
                 : "d"(port));
}