#define VEL_MOD 0
#define POS_MOD 1

void init();
extern int configure();

char DEVID[20];

int commandPos(int targPos);
int commandVel(int targVel);
