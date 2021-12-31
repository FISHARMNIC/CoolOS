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