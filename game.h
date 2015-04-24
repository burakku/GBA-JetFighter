//Name: Wenqi Hu

#define PLAYER_W 22
#define PLAYER_H 23
#define TARGET_W 8
#define TARGET_H 10
#define TG_NUM 5
#define BL_NUM 15

#define START 0
#define GAME 1
#define LOSE 2

extern int state;

void reset();
void start();
void game();
void destroy(int i, int j);
void lose();