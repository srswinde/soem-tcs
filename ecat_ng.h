#define VEL_MOD 0
#define POS_MOD 1

void init();
extern int configure();
int initEcat(char *, int);
char DEVID[20];

int ecat_getPosition(int);
int commandPos(int targPos);
int commandVel(int targVel);
