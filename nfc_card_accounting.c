#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <pthread.h>
#include <linux/input.h>

#define KEYDevice "/dev/input/event4"

#define UARTDevice "/dev/ttyS2"
#define ACCINFOPATH "card_amount.txt"
#define HISTORYFILE "history.txt"

int cur_time;

unsigned char tube[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0x7f, 0xff};

unsigned char addr[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

unsigned char wakeup[] =
    {0x55, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x03, 0xfd,
     0xd4, 0x14, 0x01, 0x17, 0x00};    //唤醒指令

unsigned char getUID[] =
    {0x00, 0x00, 0xFF, 0x04, 0xFC, 0xD4, 0x4A, 0x01, 0x00, 0xE1, 0x00};                                //获取UID


struct typFNT_GB16                        // 汉字字模数据结构
{
  char Index[2];                    // 汉字内码索引
  char Msk[32];                       // 点阵码数据
};

int card_number(unsigned long uid) {
    switch (uid) {
        case 0x526d13d5:
            return 1;

        case 0x82c70cd5:
            return 2;

        case 0xc579860f:
            return 3;

        case 0xb26426d5:
            return 4;

        case 0xf516900f:
            return 5;

        case 0xf2171fd5:
            return 6;

        case 0x453e890f:
            return 7;

        case 0x153f8c0f:
            return 8;

        case 0xcb273de6:
            return 9;

        default:
            return 0;
    }
}

struct typFNT_GB16 led_mat_num[] =                // 数据表
    {

        "", 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

        "0", 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x0F, 0xE0, 0x10, 0x10, 0x20, 0x08, 0x20, 0x08, 0x10, 0x10, 0x0F, 0xE0, 0x00, 0x00,       //"0"的字模
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

        "1", 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x20, 0x10, 0x20, 0x10, 0x3F, 0xF8, 0x20, 0x00, 0x20, 0x00, 0x00, 0x00,      //"1"的字模
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

        "2", 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x30, 0x70, 0x28, 0x08, 0x24, 0x08, 0x22, 0x08, 0x21, 0x08, 0x30, 0xF0, 0x00, 0x00,      //"2"的字模
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

        "3", 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x18, 0x30, 0x20, 0x08, 0x21, 0x08, 0x21, 0x08, 0x22, 0x88, 0x1C, 0x70, 0x00, 0x00,      //"3"的字模
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

        "4", 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x06, 0x00, 0x05, 0x80, 0x24, 0x40, 0x24, 0x30, 0x3F, 0xF8, 0x24, 0x00, 0x24, 0x00,      //"4"的字模
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

        "5", 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x19, 0xF8, 0x20, 0x88, 0x20, 0x88, 0x20, 0x88, 0x11, 0x08, 0x0E, 0x08, 0x00, 0x00,      //"5"的字模
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

        "6", 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x0F, 0xE0, 0x11, 0x10, 0x20, 0x88, 0x20, 0x88, 0x20, 0x90, 0x1F, 0x00, 0x00, 0x00,      //"6"的字模
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

        "7", 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x18, 0x00, 0x08, 0x3E, 0x08, 0x01, 0x88, 0x00, 0x68, 0x00, 0x18, 0x00, 0x00,      //"7"的字模
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

        "8", 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x1C, 0x70, 0x22, 0x88, 0x21, 0x08, 0x21, 0x08, 0x22, 0x88, 0x1C, 0x70, 0x00, 0x00,      //"8"的字模
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

        "9", 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x01, 0xF0, 0x12, 0x08, 0x22, 0x08, 0x22, 0x08, 0x11, 0x10, 0x0F, 0xE0, 0x00, 0x00,      //"9"的字模
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

int keyboardMap(int code) {
    switch (code) {
        case 2:
            return 1;

        case 3:
            return 2;

        case 4:
            return 3;

        case 5:
            return 4;

        case 6:
            return 5;

        case 7:
            return 6;

        case 8:
            return 7;

        case 9:
            return 8;

        case 10:
            return 9;

        case 1:
            return 10;            // *

        case 115:
            return 0;

        case 114:
            return 11;            // #

        default:
            return 12;
    }
}

void success_audio() {
    int fd, version, i;
    struct input_event event;

    if ((fd = open("/dev/input/event1", O_RDWR)) < 0) {
        perror("success beep func FAILED");
        return;
    }

    event.type = EV_SND;
    event.code = SND_BELL;
    event.value = 1;

    for (i = 100; i > 0; i--) {
        write(fd, &event, sizeof(struct input_event));
        usleep(1000000);
    }

    close(fd);
}

void fault_audio() {
    int fd, version, i;
    struct input_event event;

    if ((fd = open("/dev/input/event1", O_RDWR)) < 0) {
        perror("fault beep func FAILED");
        return;
    }

    event.type = EV_SND;
    event.code = SND_BELL;
    for (i = 200; i > 0; i--) {
        if (i%2 == 0)
            event.value = 1;
        else
            event.value = 0;
        write(fd, &event, sizeof(struct input_event));
        usleep(1000000);
    }
    close(fd);
}

/*

- 1 获取卡中余额的函数(文件读)
- 2 从卡中扣除消费金额的函数(文件写)
- 3 向卡中充值的函数(文件写)
- 4 获取历史消费记录的函数(文件读)
- 5 LED点阵显示函数
- 6 蜂鸣器发声函数(扣除金额成功)
- 7 蜂鸣器发声函数(扣除金额不成功)
8 状态转换函数
9 根据状态，执行不同任务的函数

*/
int get_pressed_keys() {
    int keys_fd;
    keys_fd = open(KEYDevice, O_RDONLY);
    struct input_event t;

    while (1) {
        if (read(keys_fd, &t, sizeof(t)) == sizeof(t)) {
            if (t.type == EV_KEY)
                if (t.value == 0) {
                    return keyboardMap(t.code);
                }
        }
    }
}

int get_amount(int cardnumber) {
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int i = 0;
    int amount = 0;

    fp = fopen(ACCINFOPATH, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
//    char c_num ='0';
//    sprintf(&c_num,"%d", cardnumber);

    while ((read = getline(&line, &len, fp)) != -1) {

        if (atoi(&line[0]) == cardnumber)
            for (i = 0; i < 4; ++i) {
                // starting from the last digit
                // last char is '\n'
                amount += line[len - i - 1]*pow(10, i);
            }
    }

    return amount;
}

void set_amount(int cardnumber, int new_amount) {
    FILE *fp;
    int i = 0, j = 0;
    char buf_backup[1000];
    char ch;

    fp = fopen(ACCINFOPATH, "r");

    if (fp == NULL) {
        fault_audio();
        exit(EXIT_FAILURE);
    }
    while ((ch = getc(fp)) != EOF) {
        // write character ch in file
        // updating only amount of a needed card
        if (atoi(ch) == cardnumber && (i == 0 || buf_backup[i - 1] == '\n')) {
            i++;
            for (j = 0; j < 4; ++j) {
                sprintf(&buf_backup[j + 1], "%d", (int) (new_amount/pow(10, j))%10);
//                buf_backup[j + i] = itoa((int) (new_amount/pow(10, j))%10);
            }
        }
            // don't change the rest
        else
            buf_backup[i++] = ch;
    }

    success_audio();
    fclose(fp); // closing file

    fp = fopen(ACCINFOPATH, "w+");

    // rewriting file
    if (fp)
        fwrite(buf_backup, 1, sizeof(buf_backup), fp);

    fclose(fp);
}

void put_on_matrix(const int z) {
    int mem_fd, j, k;
    unsigned short *cpld;
    mem_fd = open("/dev/mem", O_RDWR);

    cpld = (unsigned short *) mmap(NULL,
                                   (size_t) 0x20,
                                   PROT_READ | PROT_WRITE | PROT_EXEC,
                                   MAP_SHARED,
                                   mem_fd,
                                   (off_t) (0x8000000));

    if (cpld == MAP_FAILED)
        return;

    usleep(120000);

    for (j = 0; j < 16; j++) {
        for (k = 0; k < 16; k++) {
            if (k < 16 - j)
                // 数码管
                *(cpld + ((0xc0 + k) << 1)) =
                    ((led_mat_num[z].Msk[(k + j)*2] << 8) + (led_mat_num[z].Msk[(k + j)*2 + 1]));
            else
                // 数码管
                *(cpld + ((0xc0 + k) << 1)) =
                    ((led_mat_num[z + 1].Msk[(k - (16 - j))*2] << 8) + (led_mat_num[z + 1].Msk[(k - (16 - j))*2 + 1]));
        }
    }
}

// time: hhmm (4 chars without delimiter)
// amount only has 4 chars
void put_on_board(const int time, const int amount) {
    // printing to the led board
    int mem_fd;
    int idx = 0, i;
    unsigned char *cpld;

    mem_fd = open("/dev/mem", O_RDWR);

    cpld = (unsigned char *) mmap(NULL,
                                  (size_t) 0x10,
                                  PROT_READ | PROT_WRITE | PROT_EXEC,
                                  MAP_SHARED,
                                  mem_fd,
                                  (off_t) (0x8000000));

    if (cpld == MAP_FAILED) {
        printf("Open error!\n");
        return;
    }

    // setting time
    for (i = 0; i < 4; ++i) {
        if (time > 0)
            // from left to right
            idx = (int) (time/pow(10, 3 - i))%10;
        else
            idx = 11;

        *(cpld + (0xe6 << 1)) = addr[i];
        *(cpld + (0xe4 << 1)) = tube[idx];
    }

    // setting amount
    for (i = 0; i < 4; ++i) {
        if (amount > 0)
            // from left to right
            idx = (int) (amount/pow(10, 3 - i))%10;
        else
            idx = 10;

        *(cpld + (0xe6 << 1)) = addr[3 + i];
        *(cpld + (0xe4 << 1)) = tube[idx];
    }
}

void read_history(const int cardnumber) {
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int i = 0;
    int amount = 0;
    int time = 0;

    fp = fopen(HISTORYFILE, "r");

    if (fp == NULL)
        exit(EXIT_FAILURE);

    // history file structure:
    // cardnumber(any) time(4 chars) amount-left(4 chars)
    while ((read = getline(&line, &len, fp)) != -1) {
        if (atoi(line[0]) == cardnumber)
            for (i = 0; i < 4; ++i) {
                // starting from the last digit
                // last char is '\n'
                time += line[len - i - 5]*pow(10, i);
                amount += line[len - i - 1]*pow(10, i);
            }
    }

    // print to led-board
    put_on_board(time, amount);

}

void record_history(const int cardnumber, const int time, const int amount) {
    char buf[] = "1 1452 0103\n";
    FILE *fp;
    int i = 0;
    // creating a string to be written to a history file
    sprintf(&buf[0], "%d", cardnumber);
    for (i = 2; i < 6; ++i) {
        sprintf(&buf[i], "%d", (int) (time/pow(10, i - 2))%10);
        sprintf(&buf[5 + i], "%d", (int) (amount/pow(10, i - 2))%10);
//        buf[i] = itoa((int) (time/pow(10, i - 2))%10);
//        buf[5 + i] = itoa((int) (amount/pow(10, i - 2))%10);
    }

    fp = fopen(HISTORYFILE, "a");

    // rewriting file
    if (fp)
        fwrite(buf, 1, sizeof(buf), fp);

    fclose(fp);
}
void process_card(int cardnumber) {
    //getting keys -> print to the board -> when is * -> log to the file
    int i = 0;
    int key = get_pressed_keys();
    int amount = 0;
    while (key != 10 || i < 4) {
        if (key == 11) {
            put_on_board(-1, -1);    // clear board
            amount = 0;
            key = 0;
            i = 0;
        } else {
            ++i;

            amount += key;

            put_on_board(0, amount);

        }
        key = get_pressed_keys();
    }
}

void card_deduct(int cardnumber) {
    int amount_left = get_amount(cardnumber) - 1;
    set_amount(cardnumber, amount_left);
    record_history(cardnumber, cur_time, amount_left);
}

int com_init(speed_t speed) {
    struct termios options;
    int COMDevice;
    COMDevice = open(UARTDevice, O_RDWR | O_NOCTTY | O_NDELAY);// | O_NONBLOCK
    if (COMDevice < 0) {
        printf("open com device failure");
    }
    tcgetattr(COMDevice, &options);
    cfsetispeed(&options, speed);//波特率
    cfsetospeed(&options, speed);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag |= CS8;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_oflag &= ~OPOST;
    options.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    options.c_cc[VTIME] = 0;//设置超时
    options.c_cc[VMIN] = 1;//最少读1个数据
    tcsetattr(COMDevice, TCSANOW, &options);

    return COMDevice;
}

void read_from_nfc() {
    unsigned char uartdata[25], c;
    unsigned long uid;
    int re, i, COMDevice;
    int cardnumber;
    int mem_fd;
    unsigned short *cpld;

    mem_fd = open("/dev/mem", O_RDWR);

    cpld = (unsigned short *) mmap(NULL,
                                   (size_t) 0x20,
                                   PROT_READ | PROT_WRITE | PROT_EXEC,
                                   MAP_SHARED,
                                   mem_fd,
                                   (off_t) (0x8000000));

    if (cpld == MAP_FAILED)
        return;

    printf("\n");

    while (1) {
        COMDevice = com_init(B115200);

        // waking up by sending request
        re = write(COMDevice, wakeup, sizeof(wakeup));

        for (;;) {
            //reading chars one by one
            re = read(COMDevice, &c, 1);

            if (re == 1) {
                for (i = 0; i < 24; i++) {
                    uartdata[i] = uartdata[i + 1];
                }

                uartdata[24] = c;

                if (uartdata[24 - 3] == 0xd5
                    && uartdata[24 - 2] == 0x15)                        // 唤醒指令返回值 D5  15表示正确执行并完毕
                {
//					printf("uartdata = ");

//					for(i=0;i<15;i++)
//						printf("%x  ",uartdata[10+i]);

//					printf("\n\n唤醒nfc指令返回值正确!\n\n");

                    // 发出获取NFC的UID的指令
                    // make NFC-reader return a card UID
                    write(COMDevice, getUID, sizeof(getUID));
                } else if (uartdata[0] == 0x00 && uartdata[1] == 0x00
                    && uartdata[2] == 0xFF && uartdata[3] == 0x00
                    && uartdata[4] == 0xff && uartdata[5] == 0x00
                    && uartdata[6] == 0x00 && uartdata[7] == 0x00
                    && uartdata[8] == 0xFF && uartdata[12] == 0x4b
                    && uartdata[18] == 0x04 && uartdata[24] == 0x00)    //UID返回值
                {
//					printf("uartdata = ");

//					for(i=0;i<25;i++)
//						printf("%x  ",uartdata[i]);

//					printf("\n\n返回得到的UID值正确!\n\n");

                    // reading from memory
                    uid = uartdata[19] << 24 | uartdata[20] << 16 | uartdata[21] << 8 | uartdata[22];

//					printf("uid = 0x%x\n\n",uid);

                    // converting UID to int
                    cardnumber = card_number(uid);

                    printf("Card ID = %d\n\n", cardnumber);

                    /* *** CARD NUMBER HAS BEEN RECEIVED *** */

                    // START PROCESSING CARD FROM NOW ON


                    break;
                }
            }
        }

        close(COMDevice);

        usleep(1000000);
    }

    munmap(cpld, 0x20);

    close(mem_fd);
}

int main(int argc, char *argv[]) {

    return 0;
}
