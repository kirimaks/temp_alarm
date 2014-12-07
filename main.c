/* This program checks CPU temperature and report if need. */
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include"temp_alarm.h"
#include<stdint.h>

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

int8_t check_temp(char *str_temp, char *cur_mode) 
{   /* To do a lot. */
    int cur_temp = atoi(str_temp);

    if (cur_temp >= MAX_THRESHOLD) {    /* Go to powersave mode. */
        /*printf("High temperature.\n");*/
        if (strncmp(cur_mode, "powersave", 9)) {
            /*printf("*** Write powersave mode ***\n");*/
            system("for c in $(ls -d /sys/devices/system/cpu/cpu[0-9]*); do echo powersave >$c/cpufreq/scaling_governor; done");
        }
    } else if (cur_temp <= MIN_THRESHOLD) {    /* Go to ondemand mode */
        /*printf("Low temperature.\n");*/
        if (strncmp(cur_mode, "ondemand", 8)) {
            /*printf("*** Write ondemand mode ***\n");*/
            system("for c in $(ls -d /sys/devices/system/cpu/cpu[0-9]*); do echo ondemand >$c/cpufreq/scaling_governor; done");
        }
    }

    /* Should return not zero for make notify send. */
    return (cur_temp >= MAX_THRESHOLD) ? cur_temp : 0;
}

int main(int argc, char **argv)
{
    FILE *f = NULL;
    /* Open file with current cpu mode. */
    FILE *mode_file = fopen("/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor", "r");
    char buff[BUFF_SIZE];
    char cmd[100] = "sudo -u ";

/* Create sting with arguments. (notify) */
    strcat(cmd, MY_USER);
    strcat(cmd, " DISPLAY=");
    strcat(cmd, DISP_NUM);
    strcat(cmd, " ");
    strcat(cmd, "notify-send ");
    strcat(cmd, APP_NAME);
    strcat(cmd, " "); strcat(cmd, SHOW_TEXT); 
    check_args(argc,argv,&f);

    int8_t cur_temp = 0;
    char cur_mode[BUFF_SIZE];


    for(;;) {
        /* Read from temp file. */
        fread(buff, sizeof(char), 2, f);    /* Read 2 symbols. */  
        fseek(f, 0L, SEEK_SET);
        fflush(f);

        /* Read from file wiht mode. */
        fread(cur_mode, sizeof(char), 9, mode_file);
        fseek(mode_file, 0L, SEEK_SET);
        fflush(mode_file);

        if((cur_temp = check_temp(buff, cur_mode))) { /* Create number of temperature in this call.   */
            strncat(cmd, buff,2);           /* Add string with current temperature.         */
            strcat(cmd, "\"\0");
            system(cmd);
            cmd[strlen(cmd)-3] = '\0';
        }
        sleep(INTERVALS);
    }

    return 0;
}
