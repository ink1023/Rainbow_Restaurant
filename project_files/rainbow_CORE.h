//用户昵称 最大长度
#define USERNICK_LENGTH_MAX 30
//食物名称 最大长度
#define FOODNAME_LENGTH_MAX 30
//管理员密码 最大长度
#define ADMINPAS_LENGTH_MAX 30
//座位矩阵 列数
#define SEATINFO_WIDTH 9
//座位矩阵 行数
#define SEATINFO_HEIGHT 7

//日期
struct Date
{
    int year;
    int month;
    int day;
};
//时间
struct Time
{
    int hour;
    int minute;
    int second;
};
//菜品
struct food
{
    int id;//食物ID(唯一性)
    char name[FOODNAME_LENGTH_MAX];//食物名称
    int price;//食物价格
    int orderCount;//这个菜从添加到系统里以来一共被点的次数
};
//订单
struct Order
{
    //struct Date date;
    //struct Time time;
    //long orderID;
    int foodCount;//当前订单的食物数
    struct food *foods;
    int ExpensesToBePaid;//当前订单应付费用
};
//用户类型
enum UserType
{
    undefine,//还不知道（未登录）
    customer,//顾客
    admin//管理员
};
//座位
struct Seat
{
    int isSelected;//这个座位是否有人
    int row;//座位行号
    int col;//座位列号
};
//用户
struct User
{
    //long userID;//TODO:
    char userNick[USERNICK_LENGTH_MAX];
    //struct Date registerDate;//TODO:
    int IsVIP;//是否VIP
    int moneyInAccount;//用户账户余额
    //先只弄一个订单
    struct Order order;//当前订单
    enum UserType userType;//用户类型
    struct Seat seat;//用户座位
};

//餐厅日志
struct RestaurantDailyLog
{
    struct Date date;
    int orderCount;
    struct Order *orders;
};

//链表节点(食物)
struct node
{
    struct food data;
    struct node *next;
};
typedef struct node NODE;

//彩虹餐厅公共数据
struct RainbowRestaurant
{
    NODE *foods_we_have;//指针 指向当前餐厅拥有的所有菜品的结构体数组的空间
    int foodsCount;//当前餐厅拥有的菜品总数
    struct User *history_serve_users;//指针 指向所有来过本餐厅的用户的用户信息结构体数组的空间
    int historyUsersCount;
    //先考虑只服务于一个顾客，不然做不完了
    struct User current_serve_user;//当前服务的用户
    char superAdmin_password[ADMINPAS_LENGTH_MAX + 1];//系统管理员密码
    struct Seat seatsMap[SEATINFO_HEIGHT][SEATINFO_WIDTH];//餐厅座位图信息

} G_myRainbowRestaurant;


//添加一个食物数据到一个链表尾部
NODE *addNodeToTail(NODE *pHead, struct food data);
//添加一个食物数据到一个链表头部
NODE *addNodeToHead(NODE *pHead, struct food data);
//添加一个食物数据到一个有序链表 根据食物ID递增排序
NODE *addNodeByOrder(NODE *pHead, struct food data);
//删除传入链表的一个节点 根据传入的食物数据
NODE *deleteNodeByValue(NODE *pHead, struct food data);
//删除传入链表的一个节点 根据传入的食物数据的ID
NODE *deleteRestaurantFoodByID(NODE *pHead, int id);
//判断传入的两个食物结构体是否相等
int isTwoFoodEqual(struct food food1, struct food food2);
//转换食物数据数组到链表(食物)
NODE *loadFoodInfoToLinkTable(struct food *foods, int num);
//转换链表(食物)到食物数据数组
struct food *convertLinkTableToFoodInfo(NODE *pHead, int num);
//将当前餐厅的食物数据信息存盘
void saveFoodInfo(void);
//打印指定链表
void printLinkTable(NODE *pHead);


//初始化函数 用来读取数据文件、全局变量初始化
int rainbow_init(int argc, char const *argv[]);
//把一个食物数据添加到指定订单
int addFoodToOrder(int foodID, struct Order *order);
//把当前用户加入历史用户列表
void addUserToHistory(struct User user);
//把一个食物数据添加到餐厅的食物数据链表
void addFoodToRestaurant(struct food *newFood);
//获取一个新的可用foodID
int getAvailableFoodID(void);

//打印当前餐厅的座位信息图
void printSeatsMap(void);

//根据昵称判断餐厅之前是否服务过这个顾客（即判断这个用户是否在历史数据中）
int isThisUserInUsersHistory(char userNick[]);

//根据用户名设定指定用户的账户余额
void setUserMoneyFromUsersHistory(char userNick[], int Money);

//根据用户名从历史用户数据中删除用户
void deleteUserFromUsersHistory(char userNick[]);

//从历史用户数据中查找老用户并设置为当前正在的服务用户（请确保此用户存在于历史数据中！）
void loadUserDataToCurrentServeUserFromUsersHistory(char userNick[]);

//把输入的struct User更新到对应用户名的历史用户数据（请确保此用户存在于历史数据中！）
void updateUserInfoToUsersHistory(struct User user);

//把输入的struct User更新或添加到对应用户名的历史用户数据并存盘
void updateUserInfoToHistoryAndFile(struct User user);

//打印前n个最受欢迎的食物
void printPopularFood(int maxOutNum);
//打印前n个最有钱的用户?
void printRichUser(int maxOutNum);

//程序暂停指定毫秒时间
void rainbow_sleep(unsigned long dwMilliseconds);

//如果传入的指针是NULL那么就报错并结束程序
void if_Is_Null_Then_End_Program(void *ptr);

