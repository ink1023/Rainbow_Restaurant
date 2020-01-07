//从文件读取食物信息
void readFoodInfoFromFile(struct food *(*foods), int *num);
//将食物信息写入文件
void writeFoodInfoToFile(struct food foods[], int num);

//从文件读取用户信息
void readUserInfoFromFile(struct User *(*users), int *num);
//向文件写入食物信息
void writeUserInfoToFile(struct User user[], int num);

//从文件读取管理员密码
char *readSuperAdminPasswordFromFile(void);
//向文件写入管理员密码
void writeSuperAdminPasswordToFile(char password[]);

//从文件读取座位图信息
void readSeatMapInfoFromFile(struct Seat *pSeatsMap);
//向文件写入座位图信息
void writeSeatMapInfoToFile(struct Seat *pSeatsMap);

//创建全新的数据库文件，原先的数据将被清空
void createFreshDataBaseFiles(void);
