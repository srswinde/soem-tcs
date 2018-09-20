#define VEL_MOD 0
#define POS_MOD 1
#define READ_MOD 2
#define COUNTS_PER_DEGREE 186413.511111111

#define OK 0
#define NOK 1

void init();
extern int configure();
int initEcat(char *, int);
char DEVID[20];

int ecat_getPosition(int);
int commandPos(int targPos);
int commandVel(int targVel);
