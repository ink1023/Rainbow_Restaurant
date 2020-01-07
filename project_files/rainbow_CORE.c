#include "rainbow_CORE.h"
#include "rainbow_IO.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>


int rainbow_init(int argc, char const *argv[])
{
    int i;
    for(i=1; i<argc; i++)
    {
        if( 0==strcmp("/init",argv[i]) )//生成全新的初始数据文件（旧数据将丢失！）
        {
            createFreshDataBaseFiles();
            printf("已经生成了新的数据文件");
            exit(EXIT_SUCCESS);
        }
        else if( 0==strcmp("/author",argv[i]) )//显示作者信息
        {
            printf("河北大学 网计学院 2019级 信息安全1班 陈墨楠");
            exit(EXIT_SUCCESS);
        }
    }
    //设置控制台前/背景色
    //system("COLOR F0");
    //设置控制台大小
    system("mode con cols=50 lines=30");
    //载入食物数据
    int num;
    struct food *foods_read;
    readFoodInfoFromFile(&foods_read, &num); 
    NODE *pHead = loadFoodInfoToLinkTable(foods_read, num);
    G_myRainbowRestaurant.foods_we_have = pHead;
    G_myRainbowRestaurant.foodsCount = num;

    //载入用户数据
    struct User *users_read;
    readUserInfoFromFile(&users_read, &num);
    G_myRainbowRestaurant.history_serve_users = users_read;
    G_myRainbowRestaurant.historyUsersCount = num;

    //读取管理员密码
    char *pPassword = readSuperAdminPasswordFromFile();
    strcpy(G_myRainbowRestaurant.superAdmin_password, pPassword);

    //初始化当前服务用户结构体必要信息
    strcpy(G_myRainbowRestaurant.current_serve_user.userNick, "");
    G_myRainbowRestaurant.current_serve_user.seat.isSelected = 0;
    G_myRainbowRestaurant.current_serve_user.seat.row = -1;
    G_myRainbowRestaurant.current_serve_user.seat.col = -1;
    G_myRainbowRestaurant.current_serve_user.userType = undefine;

    //载入座位图信息
    readSeatMapInfoFromFile((struct Seat *)&G_myRainbowRestaurant.seatsMap);
    return 0;
}


int isTwoFoodEqual(struct food food1, struct food food2)
{
    return (food1.id==food2.id) && 
            (strcmp(food1.name, food2.name)==0) && 
            (food1.price==food2.price);
}

//链表相关操作 开始##############################################################
NODE *addNodeByOrder(NODE *pHead, struct food data)
{
    //根据传入数据创建一个新的链表节点
    NODE *pNode = (NODE *)malloc(sizeof(NODE));
    if_Is_Null_Then_End_Program(pNode);
    pNode->data = data;
    pNode->next = NULL;

    if( pHead==NULL )//如果要添加到一个 空链表 中
    {
        pNode->next = pHead;
        pHead = pNode;
    }
    else//如果要添加到一个 非空链表 中
    {
        if( pNode->data.id<pHead->data.id )//如果应当添加到非空链表的 第一位 上
        {
            pNode->next = pHead;
            pHead = pNode;
        }
        else//如果应当添加到非空链表的 非第一位 上
        {
            NODE *pr = pHead;
            while ( !(pr==NULL) )//试图遍历链表
            {
                if( pr->data.id<pNode->data.id )//“有可能”迭代到这个新结点该到的位置上了
                {   
                    if( pr->next==NULL )//如果当前位置 是 链表最后一位
                    {
                        //直接无条件添加到链表尾部
                        pNode->next = pr->next;
                        pr->next = pNode;
                        break;
                    }
                    else//如果当前位置 不是 链表最后一位
                    {
                        if(pr->next->data.id > pNode->data.id)//真的迭代到这个新结点该到的位置上了
                        {
                            //添加新结点到这个位置上
                            pNode->next = pr->next;
                            pr->next = pNode;
                            break;
                        }
                    }
                }
                pr = pr->next;//迭代链表节点
            }
        }
    }
    //一定要返回新的链表头地址，因为有可能链表头地址在上边代码中被修改了
    return pHead;//返回新的链表头
}

NODE *addNodeToTail(NODE *pHead, struct food data)
{
    //根据传入数据创建一个新的链表节点
    NODE *pNode = (NODE *)malloc(sizeof(NODE));
    if_Is_Null_Then_End_Program(pNode);
    pNode->data = data;
    pNode->next = NULL;

    if( pHead==NULL )//如果要添加到一个 空链表 中
    {
        pNode->next = pHead;
        pHead = pNode;
    }
    else//如果要添加到一个 非空链表 中
    {
        NODE *pr=NULL;
        pr = pHead;
        while ( !( pr->next==NULL ) )//迭代到链表尾部
        {
            pr = pr->next;
        }
        pr->next = pNode;//添加新结点到链表尾部
    }
    return pHead;
}

NODE *addNodeToHead(NODE *pHead, struct food data)
{
    //根据传入数据创建一个新的链表节点
    NODE *pNode = (NODE *)malloc(sizeof(NODE));
    if_Is_Null_Then_End_Program(pNode);
    pNode->data = data;
    pNode->next = NULL;

    //添加新结点到链表头部
    pNode->next = pHead;
    pHead = pNode;
    return pHead;
}

NODE *deleteNodeByValue(NODE *pHead, struct food data)
{
    if( pHead==NULL ) //空表则直接退出函数
    {
        return NULL;
    }

    //if( pHead->data == data ) //对于在表头的数据特殊处理
    if( isTwoFoodEqual(pHead->data, data) )
    {
        NODE *pDelete = pHead;
        pHead = pHead->next;
        free(pDelete);
    }
    else // 不是在表头的数据
    {
        NODE *pLast = pHead;
        NODE *pNext = pHead->next;
        NODE *pDelete = NULL;
        while ( !(pNext->next==NULL) )//试图遍历链表
        {
            pLast = pLast->next;
            pNext = pNext->next;
            //if( pNext->data==data )
            if( isTwoFoodEqual(pHead->data, data) )//如果找到
            {
                //删除
                pDelete = pNext;
                pLast->next = pNext->next;
                free(pDelete);
                //这里直接break，只会删除第一个匹配的节点
                break;
            }
        }
        
        
    }
    return pHead;
}

NODE *deleteRestaurantFoodByID(NODE *pHead, int id)
{
    if( pHead==NULL ) //空表
    {
        return NULL;
    }
    
    //if( pHead->data == data ) //对于在表头的数据特殊处理
    if( id==pHead->data.id )
    {
        NODE *pDelete = pHead;
        pHead = pHead->next;
        free(pDelete);
    }
    else // 不是在表头的数据
    {
        NODE *pLast = pHead;
        NODE *pNext = pHead->next;
        NODE *pDelete = NULL;
        while (1)
        {
            //if( pNext->data==data )
            if( id==pNext->data.id )//如果找到
            {
                pDelete = pNext;
                pLast->next = pNext->next;
            }

            if(pNext->next==NULL)//如果到了链表结尾
            {
                break;
            }
            else
            {
                pLast = pLast->next;
                pNext = pNext->next;
            }
            /* 这里一定要先判断找到数据，再将pLast、pNext往下移动，
            顺序不能反，否则链表第二个数据就会被忽略过去，别问我为什么非得写这段注释(￣_￣|||) */
        }
        free(pDelete);
    }
    return pHead;
}

void printLinkTable(NODE *pHead)
{
    NODE *pr=NULL;
    for(pr=pHead; !(pr==NULL); pr=pr->next)//遍历链表
    {
        //printf("%d ", pr->data);
        printf("%d\t %-15s\t%-4d 元\n", pr->data.id, pr->data.name, pr->data.price);
    }
    printf("\n");
}

NODE *loadFoodInfoToLinkTable(struct food *foods, int num)
{
    NODE *pHead = NULL;
    int i;
    for(i=0; i<num; i++)
    {
        pHead = addNodeToTail(pHead, foods[i]);
    }
    return pHead;
    
}

struct food *convertLinkTableToFoodInfo(NODE *pHead, int num)
{
    struct food *foods = NULL;
    foods = (struct food *)malloc(num*sizeof(struct food));
    if_Is_Null_Then_End_Program(foods);
    NODE *pr=NULL;
    int i=0;
    for(pr=pHead; !(pr==NULL); pr=pr->next)//遍历链表
    {
        foods[i] = pr->data;
        i++;
    }
    return foods;
}
//链表相关操作 结束##############################################################


int addFoodToOrder(int foodID, struct Order *order)
{
    NODE *pHead = G_myRainbowRestaurant.foods_we_have;
    NODE *pr;

    pr=pHead;
    int i;
    for(i=0; i<G_myRainbowRestaurant.foodsCount; i++)//遍历餐厅中现在有的所有食物
    {
        if( foodID==pr->data.id )//没错！要的就是这个食物
        {
            //这个食物又被多点了一次，记录下来
            pr->data.orderCount++;
            saveFoodInfo();

            //为这个订单再开辟一块空间以便容纳新的这一个food结构体
            int newSize = (order->foodCount+1) * sizeof(struct food);
            order->foods = (struct food *)realloc(order->foods, newSize);
            if_Is_Null_Then_End_Program(order->foods);

            //更新这个订单
            order->foods[(order->foodCount+1)-1] = pr->data;
            order->foodCount++;
            order->ExpensesToBePaid += pr->data.price;
            return 1;
        }
        pr = pr->next;
    }
    /*while(1)
    {
        if( !(pr==NULL) )
        {
            printf("%d\n", pr->data.id);
            if( foodID==pr->data.id )
            {
                order->foods[order->foodCount] = pr->data;
                order->foodCount++;
                order->ExpensesToBePaid += pr->data.price;
                return 1;
            }
            pr = pr->next;
        }
        else
        {
            break;
        }
        
    }*/
    //for(pr=pHead; !(pr==NULL); pr=pr->next)
    //{
    //    printf("%d\n", pr->data.id);
    //    if( foodID==pr->data.id )
    //    {
    //        order->foods[order->foodCount] = pr->data;
    //        order->foodCount++;
    //        order->ExpensesToBePaid += pr->data.price;
    //        return 1;
    //    }
    //    //printf("%d\t %-15s\t%-4dRMB\n", pr->data.id, pr->data.name, pr->data.price);
    //}
    return 0;
}

void addUserToHistory(struct User user)
{
    struct User *users = G_myRainbowRestaurant.history_serve_users;
    int num = 1 + G_myRainbowRestaurant.historyUsersCount;

    int newSize = (num) * sizeof(struct User);
    users = (struct User *)realloc(users, newSize);
    if_Is_Null_Then_End_Program(users);
    users[num-1] = user;

    G_myRainbowRestaurant.history_serve_users = users;
    G_myRainbowRestaurant.historyUsersCount = num;

    writeUserInfoToFile(G_myRainbowRestaurant.history_serve_users, num);

}

void addFoodToRestaurant(struct food *newFood)
{
    //G_myRainbowRestaurant.foods_we_have = addNodeToTail(G_myRainbowRestaurant.foods_we_have, *newFood);
    G_myRainbowRestaurant.foods_we_have = addNodeByOrder(G_myRainbowRestaurant.foods_we_have, *newFood);
} 

void saveFoodInfo(void)
{
    struct food *foods;
    //把链表转换成数组
    foods = convertLinkTableToFoodInfo(G_myRainbowRestaurant.foods_we_have, G_myRainbowRestaurant.foodsCount);
    //保存这个数组
    writeFoodInfoToFile(foods, G_myRainbowRestaurant.foodsCount);
}

int getAvailableFoodID(void)
{
    int id;
    if( G_myRainbowRestaurant.foodsCount==0 )
    {
        id = 1;
    }
    else
    {   if( G_myRainbowRestaurant.foods_we_have->data.id > 1 )//如果第一个元素id不是1
        {
            id = 1;//生成一个1
        }
        else
        {
            NODE *pr=NULL;
            for(pr=G_myRainbowRestaurant.foods_we_have; !(pr==NULL); pr=pr->next)
            {
                if(pr->next==NULL)//如果到了最后一个元素
                {
                    id = 1 + pr->data.id;//生成比最后一个元素id大1的id
                    break;
                }
                else//还没到最后一个元素
                {
                    if( ((pr->next)->data.id - pr->data.id)>1 )//如果某两个元素的id不是连续的
                    {
                        id = 1 + pr->data.id;//生成比前一个元素id大1的id
                        break;
                    }
                }
            }
        }
    }
    return id;
}

void printSeatsMap(void)
{
    int row,col;
    printf("   ");
    for(row=0; row<SEATINFO_WIDTH; row++)
    {
        printf("%3d", row+1);
    }
    printf("\n");
    for(row=0; row<SEATINFO_HEIGHT; row++)
    {
        printf("%3d ", row+1);
        for(col=0; col<SEATINFO_WIDTH; col++)
        {
            char ch;
            if( !G_myRainbowRestaurant.seatsMap[row][col].isSelected )
            {
                ch = ' ';
            }
            else
            {
                int currentUser_row = G_myRainbowRestaurant.current_serve_user.seat.row;
                int currentUser_col = G_myRainbowRestaurant.current_serve_user.seat.col;
                int isCurrentUserPos = (currentUser_row==G_myRainbowRestaurant.seatsMap[row][col].row) && 
                                        (currentUser_col==G_myRainbowRestaurant.seatsMap[row][col].col);
                if( isCurrentUserPos )//如果是 当前顾客 之前选的座位
                {
                    ch = '@';
                }
                else//如果是 其他顾客 之前选的座位
                {
                    ch = '*';
                }
                
            }
            printf("[%c]", ch);
        }
        printf("\n");
    }
}

int isThisUserInUsersHistory(char userNick[])
{
    int i;
    for(i=0; i<G_myRainbowRestaurant.historyUsersCount; i++)//遍历历史用户数据
    {
        if( 0 == strcmp(userNick, G_myRainbowRestaurant.history_serve_users[i].userNick) )
        {
            return 1;
        }
    }
    return 0;
}

void setUserMoneyFromUsersHistory(char userNick[], int money)
{
    int i;
    for(i=0; i<G_myRainbowRestaurant.historyUsersCount; i++)//遍历历史用户数据
    {
        if( 0 == strcmp(userNick, G_myRainbowRestaurant.history_serve_users[i].userNick) )
        {
            G_myRainbowRestaurant.history_serve_users[i].moneyInAccount = money;
            G_myRainbowRestaurant.history_serve_users[i].IsVIP = money>=1000?1:0;

            //存盘
            writeUserInfoToFile(G_myRainbowRestaurant.history_serve_users, G_myRainbowRestaurant.historyUsersCount);
            return;
        }
    }
    printf("不该执行这行代码的，我在尝试设置一个不存在于history_serve_users的user的账户余额");
    rainbow_sleep(1000);
    exit(EXIT_FAILURE);

}

void deleteUserFromUsersHistory(char userNick[])//强迫症专用删除法
{
    //找到应该删除的元素的位置   
    int i=0;
    while( 0!=strcmp(userNick, G_myRainbowRestaurant.history_serve_users[i].userNick) )
    {
        i++;
        if( (i+1)>G_myRainbowRestaurant.historyUsersCount )
        {
            printf("不该执行这行代码的，我在尝试删除一个不存在于history_serve_users的user");
            rainbow_sleep(1000);
            exit(EXIT_FAILURE);
        }
    }
    //把后边的内存往前整体移动一个元素的长度（然而现在最后一个位置就空出来了）
    struct User *dest=NULL, *src=NULL;
    dest = &G_myRainbowRestaurant.history_serve_users[i];
    src = &G_myRainbowRestaurant.history_serve_users[i+1];
    int arrLen = G_myRainbowRestaurant.historyUsersCount;
    size_t size = sizeof(struct User) * ( arrLen - (i+1) );
    memcpy(dest, src, size);

    //申请一块大小正合适的新空间，把旧的数据复制进去，然后free掉旧的空间
    struct User *oldMemory = G_myRainbowRestaurant.history_serve_users;
    size_t lessSize = sizeof(struct User) * (arrLen - 1);
    struct User *newMemory = (struct User *)malloc(lessSize);
    if_Is_Null_Then_End_Program(newMemory);
    memcpy(newMemory, oldMemory, lessSize);
    free(oldMemory);

    //更新数据到彩虹餐厅公共数据并存盘
    G_myRainbowRestaurant.history_serve_users = newMemory;
    G_myRainbowRestaurant.historyUsersCount--;
    writeUserInfoToFile(G_myRainbowRestaurant.history_serve_users, G_myRainbowRestaurant.historyUsersCount);
    
}

void loadUserDataToCurrentServeUserFromUsersHistory(char userNick[])
{
    int i;
    struct User *aHistoryUser;
    for(i=0; i<G_myRainbowRestaurant.historyUsersCount; i++)//遍历历史用户
    {
        aHistoryUser = &G_myRainbowRestaurant.history_serve_users[i];
        if( 0 == strcmp(userNick, aHistoryUser->userNick) )
        {
            G_myRainbowRestaurant.current_serve_user = *aHistoryUser;
            break;
        }
    }
}

void updateUserInfoToUsersHistory(struct User user)
{
    int i;
    for(i=0; i<G_myRainbowRestaurant.historyUsersCount; i++)//遍历历史用户
    {
        if( 0==strcmp(user.userNick, G_myRainbowRestaurant.history_serve_users[i].userNick) )
        {
            G_myRainbowRestaurant.history_serve_users[i] = user;
            return;
        }
    }
    //不应该运行到下边这些代码的，本来可以主动限制的，但是我不想改了
    printf("Error: 传入的 struct User 不在 history_serve_users 里!\n");
    printf("快去 Debug !.");//
    exit(EXIT_FAILURE);
}

void updateUserInfoToHistoryAndFile(struct User user)
{
    if( isThisUserInUsersHistory(user.userNick) )//如果这个用户是老用户
    {
        //更新该用户历史用户数据
        updateUserInfoToUsersHistory(G_myRainbowRestaurant.current_serve_user);
    }
    else
    {
        //添加该用户到历史用户数据
        addUserToHistory(user);
    }
    writeUserInfoToFile(G_myRainbowRestaurant.history_serve_users, G_myRainbowRestaurant.historyUsersCount);
}

void printPopularFood(int maxOutNum)
{
    int num = G_myRainbowRestaurant.foodsCount;
    maxOutNum = maxOutNum<num?maxOutNum:num;//取小的那个，不然数组越界了
    struct food *foods;
    //foods = (struct food *)malloc(num*sizeof(struct food));
    //获得保存了餐厅所有食物信息的一个结构体数组
    foods = convertLinkTableToFoodInfo(G_myRainbowRestaurant.foods_we_have, num);
    struct food tempFood;
    int i, j;
    //按照食物被点次数从大到小排序
    for(i=0; i<num-1; i++)
    {
        for(j=i+1; j<num; j++)
        {
            if( !(foods[i].orderCount >= foods[j].orderCount) )
            {
                tempFood = foods[i];
                foods[i] = foods[j];
                foods[j] = tempFood;
            }
        }
    }
    printf("序号\t菜名\t\t被点次数\n");
    printf("---------------------------------------\n");
    for(i=0; i<maxOutNum; i++)//输出前maxOutNum个数据
    {
        printf("%d\t%-12s\t%d\n", foods[i].id, foods[i].name, foods[i].orderCount);
    }
    printf("---------------------------------------\n");
}

void printRichUser(int maxOutNum)
{
    int num = G_myRainbowRestaurant.historyUsersCount;
    maxOutNum = maxOutNum<num?maxOutNum:num;//取小的那个，不然数组越界了

    //获得保存了餐厅所有用户信息的一个新的结构体数组（不能改原来的哦）
    struct User *users;
    users = (struct User *)malloc(num*sizeof(struct User));
    if_Is_Null_Then_End_Program(users);
    memcpy(users, G_myRainbowRestaurant.history_serve_users, num*sizeof(struct User));
    
    struct User tempUser;
    int i, j;
    //按照用户账户余额从大到小排序
    for(i=0; i<num-1; i++)
    {
        for(j=i+1; j<num; j++)
        {
            if( !(users[i].moneyInAccount > users[j].moneyInAccount) )
            {
                tempUser = users[i];
                users[i] = users[j];
                users[j] = tempUser;
            }
        }
    }
    printf("---------------------------------------\n");
    for(i=0; i<maxOutNum; i++)//输出前maxOutNum个数据
    {
        printf("用户名: %s\t账户余额: %d\n", users[i].userNick, users[i].moneyInAccount);
    }
    printf("---------------------------------------\n");
}

void rainbow_sleep(unsigned long dwMilliseconds)
{
    //window.h里 typedef unsigned long DWORD 了
    //它们两个实际上是一个东西
    Sleep((DWORD)dwMilliseconds);
}

/*
int rainbow_printf(const char *__restrict__ _Format, ...)
{
    //do some other things
    return printf(_Format, __VA_ARGS__);
}
*/

void if_Is_Null_Then_End_Program(void *ptr)
{
    if(  ptr==NULL )
    {
        printf("程序发生错误 !\n堆内存分配失败\n这可能是您系统内存不足导致的\n即将退出程序...\n");
        rainbow_sleep(1000);
        exit(EXIT_FAILURE);
    }
}