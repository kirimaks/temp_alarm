/* This program checks CPU temperature and report if need. */
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include"temp_alarm.h"
#include<stdint.h>


int main(int argc, char **argv)
{
    FILE *f = NULL;
    char buff[BUFF_SIZE];
    char cmd[100] = "sudo -u ";

    strcat(cmd, MY_USER);
    strcat(cmd, " DISPLAY=");
    strcat(cmd, DISP_NUM);
    strcat(cmd, " ");
    strcat(cmd, "notify-send ");
    strcat(cmd, APP_NAME);
    strcat(cmd, " ");
    strcat(cmd, SHOW_TEXT);

    check_args(argc,argv,&f);

    int8_t cur_temp = 0;

    for(;;) {
        fread(buff, sizeof(char), LENGTH_TO_READ, f);
        fseek(f,0,SEEK_SET);
        fflush(f);

        if((cur_temp = check_temp(buff))) {
            strncat(cmd, buff,2);
            strcat(cmd, "\"\0");
            system(cmd);
            cmd[strlen(cmd)-3] = '\0';
        }
        sleep(INTERVALS);
    }

    return 0;
}

void check_args(int argc, char **argv, FILE **f) 
{   /* To do a lot. */
    if(argc == 1) {
        printf("Usage: %s [file_to_monitor]\n", argv[0]);
        exit(EXIT_FAILURE);
    } else if(!(*f = fopen(argv[1], "r"))) {
        printf("Can't open %s, %s\n", argv[1], strerror(errno));
        exit(EXIT_FAILURE);
    }
}

int8_t check_temp(char *str_temp) 
{   /* To do a lot. */
    int temp = atoi(str_temp);
    /*printf("{%d}\n", temp);*/

    /* Maybe set cpu to powersave mode from this function if the temperature is too high. */
    return (temp >= THRESHOLD) ? temp : 0;
}
