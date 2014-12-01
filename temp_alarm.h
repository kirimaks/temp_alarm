#define BUFF_SIZE 100
#define LENGTH_TO_READ 2    /* The length to read from file.    */
#define INTERVALS 3         /* Waiting seconds.                 */
#define THRESHOLD 80

#define MY_USER "kirimaks"
#define DISP_NUM ":0.0"
#define APP_NAME "\"ACPI Event\""
#define SHOW_TEXT "\"Warning Warning Warning...CPU Temperature is:"

void check_args(int, char**, FILE**);
int8_t check_temp(char*);
