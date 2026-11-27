#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "w25q64.h"
#include "wiringPiSPI.h"

#define SPI_SPEED 2000000

int parse_spidev(const char *dev, int *channel, int *port) {
    if (strncmp(dev, "/dev/spidev", 11) != 0) return -1;

    int ch = -1, pt = -1;
    if (sscanf(dev, "/dev/spidev%d.%d", &ch, &pt) == 2) {
        *channel = ch;
        *port = pt;
        return 0;
    }
    return -1;
}

void dump(uint8_t *dt, uint32_t n) {
  uint32_t sz;
  char buf[64];
  uint16_t clm = 0;
  uint8_t data;
  uint8_t sum;
  uint8_t vsum[16];
  uint8_t total =0;
  uint32_t saddr =0;
  uint32_t eaddr =n-1;
  sz = eaddr -saddr;
  
  printf("----------------------------------------------------------\n");
  uint16_t i;
  for (i=0;i<16;i++) vsum[i]=0;  
  uint32_t addr;
  for (addr = saddr; addr <= eaddr; addr++) {
    data = dt[addr];
    if (clm == 0) {
      sum =0;
      printf("%05x: ",addr);
    }

    sum+=data;
    vsum[addr % 16]+=data;
    
    printf("%02x ",data);
    clm++;
    if (clm == 16) {
      printf("|%02x \n",sum);
      clm = 0;
    }
  }
  printf("----------------------------------------------------------\n");
  printf("       ");
  for (i=0; i<16;i++) {
    total+=vsum[i];
    printf("%02x ",vsum[i]);
  }
  printf("|%02x \n\n",total);
}

int main(int argc, char *argv[]) {
    const char *spi_dev = "/dev/spidev0.0";
    int channel = 0, port = 0;
    uint8_t buf[256];    
    uint8_t wdata[26];
    uint8_t i;
    uint16_t n;          
    int opt;

    while ((opt = getopt(argc, argv, "D:")) != -1) {
        switch (opt) {
            case 'D':
                spi_dev = optarg;
                break;
            default:
                fprintf(stderr, "Usage: %s [-D /dev/spidevX.Y]\n", argv[0]);
                return 1;
        }
    }

    if (parse_spidev(spi_dev, &channel, &port) != 0) {
        fprintf(stderr, "Invalid SPI device format: %s\n", spi_dev);
        return 1;
    }

    if (wiringPiSPISetupMode(channel, port, SPI_SPEED, 0) < 0) {
        perror("SPI setup failed");
        return 1;
    }

    // Start Flush Memory
    W25Q64_begin(channel);
    
    // JEDEC ID Get
    W25Q64_readManufacturer(buf);
    printf("JEDEC ID : ");
    for (i=0; i< 3; i++) {
      printf("%x ",buf[i]);
    }
    printf("\n");
    
    // Unique ID Get
    W25Q64_readUniqieID(buf);
    printf("Unique ID : ");
    for (i=0; i< 7; i++) {
      printf("%x ",buf[i]);
    }
    printf("\n");
    
    // Read 256 byte data from Address=0
    memset(buf,0,256);
    n =  W25Q64_read(0, buf, 256);
    printf("Read Data: n=%d\n",n);
    dump(buf,256);

    // First read 256 byte data from Address=0
    memset(buf,0,256);
    n =  W25Q64_fastread(0, buf, 256);
    printf("Fast Read Data: n=%d\n",n);
    dump(buf,256);

    // Erase data by Sector
    n = W25Q64_eraseSector(0,true);
    printf("Erase Sector(0): n=%d\n",n);
    memset(buf,0,256);
    n =  W25Q64_read (0, buf, 256);
    dump(buf,256);
 
    // Write data to Sector=0 Address=10
    for (i=0; i < 26;i++) {
      wdata[i]='A'+i; // A-Z     
    }  
    n =  W25Q64_pageWrite(0, 10, wdata, 26);
    printf("page_write(0,10,d,26): n=%d\n",n);

    // Read 256 byte data from Address=0
    memset(buf,0,256);
    n =  W25Q64_read(0, buf, 256);
    printf("Read Data: n=%d\n",n);
    dump(buf,256);

    // Write data to Sector=0 Address=0
    for (i=0; i < 10;i++) {
      wdata[i]='0'+i; // 0-9     
    }  
    n =  W25Q64_pageWrite(0, 0, wdata, 10);
    printf("page_write(0,0,d,10): n=%d\n",n);

    // First read 256 byte data from Address=0
    memset(buf,0,256);
    n =  W25Q64_fastread(0,buf, 256);
    printf("Fast Read Data: n=%d\n",n);
    dump(buf,256);

    // Get fron Status Register1
    buf[0] = W25Q64_readStatusReg1();
    printf("Status Register-1: %x\n",buf[0]);

    // Get fron Status Register2
    buf[0] = W25Q64_readStatusReg2();
    printf("Status Register-2: %x\n",buf[0]);
    return 0;
}
