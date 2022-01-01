// http://learnitonweb.com/2020/05/22/12-developing-an-operating-system-tutorial-episode-6-ata-pio-driver-osdev/


#define STATUS_BSY 0x80
#define STATUS_RDY 0x40
#define STATUS_DRQ 0x08
#define STATUS_DF 0x20
#define STATUS_ERR 0x01

//This is really specific to out OS now, assuming ATA bus 0 master
//Source - OsDev wiki
static void ATA_wait_BSY();
static void ATA_wait_DRQ();

void read_sectors_ATA_PIO(uint32 LBA, uint8 sector_count)
{

  ATA_wait_BSY();
  outb(0x1F6, 0xE0 | ((LBA >> 24) & 0xF));
  outb(0x1F2, sector_count);
  outb(0x1F3, (uint8)LBA);
  outb(0x1F4, (uint8)(LBA >> 8));
  outb(0x1F5, (uint8)(LBA >> 16));
  outb(0x1F7, 0x20); //Send the read command

  uint32 outerIndex = 0;
  // for (int j = 0; j < sector_count; j++)
  // {
  //   for (int i = 0; i < 256; i++)
  //   {
  //     target[i + outerIndex] = 48;
  //   }
  //   outerIndex += 256;
  // }

  //uint16 *target = (uint16 *)target_address;

  for (int j = 0; j < sector_count; j++)
  {
    ATA_wait_BSY();
    //printf('c', 'A');
    //ATA_wait_DRQ();
    //printf('c', 'B');
    for (int i = 0; i < 256; i++)
    {
      //printf('c', 'S');
      target[i + outerIndex] = inbWord(0x1F0);
    }
    outerIndex += 256;
    //printf('i', j);
  }
}

void write_sectors_ATA_PIO(uint32 LBA, uint8 sector_count, uint32 *bytes)
{
  ATA_wait_BSY();
  outb(0x1F6, 0xE0 | ((LBA >> 24) & 0xF));
  outb(0x1F2, sector_count);
  outb(0x1F3, (uint8)LBA);
  outb(0x1F4, (uint8)(LBA >> 8));
  outb(0x1F5, (uint8)(LBA >> 16));
  outb(0x1F7, 0x30); //Send the write command

  for (int j = 0; j < sector_count; j++)
  {
    //ATA_wait_BSY();
    //ATA_wait_DRQ();
    for (int i = 0; i < 256; i++)
    {
      outbLong(0x1F0, bytes[i]);
      printf('c', 'B');
    }
  }
}

static void ATA_wait_BSY() //Wait for bsy to be 0
{
  while (inb(0x1F7) & STATUS_BSY)
    ;
}
static void ATA_wait_DRQ() //Wait for drq to be 1
{
  while (!(inb(0x1F7) & STATUS_RDY))
    ;
}