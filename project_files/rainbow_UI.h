//用户类型选择页面
enum UserType showMenu_Welcome(void);

//管理员页面
int showMenu_admin(void);
//用户页面
int showMenu_customer(void);

//用户 座位选择
void showSeatSelectMenu(void);
//用户 食物展示
void showFoodMenu(void);
//用户 点菜
void showOrderMenu(void);
//用户 结账
void showCheckMenu(void);

//管理员 系统管理员信息修改
void showUsersManagement_CURD_admin(void);
//管理员 增加、删除餐厅菜品.
void showFoodsManagement_CURD_admin(void);
//菜品、用户数据分析
void showStatisticsAnalysis_admin(void);
//管理员 查看餐厅实时座位占用情况
void showSeatMapInfo_admin(void);
//管理员 查看餐厅顾客
void showUsersInfo_admin(void);


//退出程序
void showExitProgram(void);

//清空屏幕缓冲区
void clearScreenBuffer(void);
