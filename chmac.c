#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>

#define ORIG_MAC "00:90:4C:0E:81:23"
#define BIN_FILE "/lib/firmware/brcm/brcmfmac43143.bin"

// Seek through a file until either the end of the file or
// the key is found.
int seekUntil(int fd, const char *key) {
    int blen = strlen(key) + 1;
    char buf[blen];

    memset(buf, 0, blen);

    char inch = 0;
    while (read(fd, &inch, 1) == 1) {
        int i;
        for (i = 0; i < blen-2; i++) {
            buf[i] = buf[i+1];
        }
        buf[blen-2] = inch;
        if (!strcmp(buf, key)) {
            return 1;
        }
    }
    return 0;
}

int main(int argc, char **argv) {

    // Open the firmware file
    uint8_t newmac[6];
    int force = 0;

    char *binfile = BIN_FILE;
    srandom(time(NULL));
    newmac[0] = 0x00;
    newmac[1] = 0x90;
    newmac[2] = 0x4c;
    newmac[3] = random();
    newmac[4] = random();
    newmac[5] = random();

    int c;
    while ((c = getopt(argc, argv, "hfm:b:")) != -1) {
        switch (c) {
            case 'f':
                force = 1;
                break;
            case 'm':
                if (strlen(optarg) != 17) {
                    fprintf(stderr, "%s: Format of MAC address must be xx:xx:xx:xx:xx:xx\n", argv[0]);
                    return 10;
                }
                if (
                    (optarg[2] != ':') ||
                    (optarg[5] != ':') ||
                    (optarg[8] != ':') ||
                    (optarg[11] != ':') ||
                    (optarg[14] != ':')
                ) {
                    fprintf(stderr, "%s: Format of MAC address must be xx:xx:xx:xx:xx:xx\n", argv[0]);
                    return 10;
                }

                optarg[2] = 0;
                optarg[5] = 0;
                optarg[8] = 0;
                optarg[11] = 0;
                optarg[14] = 0;

                newmac[0] = strtol(&optarg[0], NULL, 16);
                newmac[1] = strtol(&optarg[3], NULL, 16);
                newmac[2] = strtol(&optarg[6], NULL, 16);
                newmac[3] = strtol(&optarg[9], NULL, 16);
                newmac[4] = strtol(&optarg[12], NULL, 16);
                newmac[5] = strtol(&optarg[15], NULL, 16);

                optarg[2] = ':';
                optarg[5] = ':';
                optarg[8] = ':';
                optarg[11] = ':';
                optarg[14] = ':';
                break;
            case 'b':
                binfile = optarg;
                break;
            case 'h':
                printf("\n");
                printf("BCM43143 Firmware MAC Address Tweaker\n");
                printf("\n");
                printf("Usage: %s [options]\n", argv[0]);
                printf("  Options:\n");
                printf("    -h          This help text\n");
                printf("    -f          Force a MAC change\n");
                printf("    -m <mac>    New MAC address to use\n");
                printf("    -b <file>   Firmare .bin file to modify\n");
                printf("\n");
                printf("This utility will automatically change the hard-coded\n");
                printf("MAC address in the BCM43143 WiFi adapter firmware file\n");
                printf("if it is currently the factory default.  A new random\n");
                printf("MAC address is selected and automatically inserted into\n");
                printf("the file.  You can re-program it as many times as you like\n");
                printf("by specifying the -f flag to force a new change, and you can\n");
                printf("provide your own MAC address to use with the -m flag.  MAC\n");
                printf("addresses must be provided in the format xx:xx:xx:xx:xx:xx\n");
                printf("where 'xx' is a two-digit hexadecimal value between 00 and ff.\n");
                printf("\n");
                return 0;
        }
    }

    int fd = open(binfile, O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "%s: Cannot find firmware file %s\n",
            argv[0], binfile);
        return 10;
    }

    // Look for the phrase "macaddr="

    char mac[18];
    memset(mac, 0, 18);

    if (seekUntil(fd, "macaddr=")) {
        if (seekUntil(fd, "macaddr=")) {
            if (seekUntil(fd, "macaddr=")) {
                off_t here = lseek(fd, 0, SEEK_CUR);
                read(fd, mac, 17);
                printf("Current MAC address: %s\n", mac);
                printf("Offset is: %d\n", here);
                if ((!strcmp(mac, "00:90:4c:0e:81:23")) || (force == 1)) {
                    printf("MAC is the factory default. Changing.\n");
                    sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x", newmac[0], newmac[1], newmac[2],
                        newmac[3], newmac[4], newmac[5]);
                    printf("New MAC: %s\n", mac);
                    if (lseek(fd, here, SEEK_SET) == here) {
                        write(fd, mac, 17);
                    }
                } else {
                    printf("The MAC address has already been changed.\nUse -f to force a new change.\n");
                }
            }
        }
    }

    close(fd);
    return 0;

}

