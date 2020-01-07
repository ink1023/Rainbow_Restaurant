#include "rainbow_CORE.h"
#include "rainbow_UI.h"
#include "rainbow_IO.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void clearScreenBuffer(void)
{
    char c;
    while ((c = getchar()) != EOF && c != '\n');

}

enum UserType showMenu_Welcome(void)
{
    enum UserType userType = undefine;
    int typeInput = 0;
    system("cls");
    while( !(userType==admin || userType==customer) )
    {
        printf("########## 河北大学 彩虹餐厅 #########\n\n");
        printf("\t请选择您的身份\n\n\n");
        printf("\t1) 我是一个顾客.\n\n");
        printf("\t2) 我是系统管理员.\n\n");
        printf("请输入你的身份对应的序号: [ ]\b\b");
        scanf("%d", &typeInput);
        clearScreenBuffer();
        system("cls");
        if( typeInput==1 )
        {
            userType = customer;
            G_myRainbowRestaurant.current_serve_user.userType = customer;
        }
        else if( typeInput==2 )
        {
            printf("########## 河北大学 彩虹餐厅 #########\n\n");
            printf("\t请输入管理员密码以便验证你的身份\n\n");
            printf("\t> ___________________________\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
            char password_input[ADMINPAS_LENGTH_MAX+1];
            //scanf("%s", password_input);
            fscanf(stdin, "%s", password_input);//更安全的输入函数！
            clearScreenBuffer();

            if( !(strcmp(password_input, G_myRainbowRestaurant.superAdmin_password)==0) )
            {
                //密码不匹配
                printf("密码不正确 !\n");
                rainbow_sleep(1000);
                userType = undefine;
                G_myRainbowRestaurant.current_serve_user.userType = undefine;
            }
            else
            {
                userType = admin;
                G_myRainbowRestaurant.current_serve_user.userType = admin;
            }
            system("cls");
        }
    }
    return userType;

}

int showMenu_admin(void)
{
    printf("########## 河北大学 彩虹餐厅 #########\n\n");
    printf("\t1) 增加、删除餐厅菜品.\n\n");
    printf("\t2) 查看、修改餐厅用户信息.\n\n");
    printf("\t3) 查看餐厅实时座位占用情况.\n\n");
    printf("\t4) 菜品、用户数据分析.\n\n");
    printf("\t5) 系统管理员信息修改.\n\n");
    printf("\t6) 退出系统.\n\n");
    int selection = 0;
    while( !(selection>=1 && selection<=6) )
    {
        printf("请输入操作对应的序号: [ ]\b\b");
        scanf("%d", &selection);
        clearScreenBuffer();
    }
    system("cls");
    return selection;

}

int showMenu_customer(void)
{
    //如果之前还没有定义过用户名（即第一次进入这个菜单还没有注册过信息）
    if( 0==strcmp("",G_myRainbowRestaurant.current_serve_user.userNick) )
    {
        struct User user;
        printf("########## 河北大学 彩虹餐厅 #########\n\n");
        printf("\t请输入的你名字（30字符以内）\n\n\t> __________\b\b\b\b\b\b\b\b\b\b");
        scanf("%s", user.userNick);

        if( isThisUserInUsersHistory(user.userNick) )//如果是之前来过的用户
        {
            loadUserDataToCurrentServeUserFromUsersHistory(user.userNick);
        }
        else//如果是新用户
        {
            printf("\n\t唔......\n\t看起来你是第一次来到本餐厅.\n");
            printf("\n\t请输入你要向账户里充值多少钱: \n\t(充值超过1000元自动成为餐厅VIP，全场8折哦)\n");
            printf("\t> __________\b\b\b\b\b\b\b\b\b\b");
            scanf("%d", &user.moneyInAccount);
            if(user.moneyInAccount >= 1000){
                user.IsVIP = 1;
            }else{
                user.IsVIP = 0;
            }
            user.order.foodCount = 0;
            user.order.ExpensesToBePaid = 0;
            user.userType = customer;
            user.seat.isSelected = 0;
            G_myRainbowRestaurant.current_serve_user = user;
        }
        updateUserInfoToHistoryAndFile(user);
        system("cls");
    }
    
    printf("########## 河北大学 彩虹餐厅 #########\n\n");
    printf("尊贵的用户 %s \n\n", G_myRainbowRestaurant.current_serve_user.userNick);
    if( G_myRainbowRestaurant.current_serve_user.IsVIP )
    {
        printf("你是本餐厅的VIP，所有菜品8折优惠\n");
    }
    printf("\n");
    printf("\t1) 先选一个座位坐好.\n\n");
    printf("\t2) 看看这里有什么菜品.\n\n");
    printf("\t3) 我要点一些吃的.\n\n");
    printf("\t4) 我要先去吃饭了.\n\n");
    printf("\t5) 吃饱了，我要结账.\n\n");
    printf("=======================================\n");
    if( G_myRainbowRestaurant.current_serve_user.order.foodCount > 0 )
    {//如果点了餐就展示点的餐的数量和费用
        printf("您目前共点了 %d 道菜.\n", G_myRainbowRestaurant.current_serve_user.order.foodCount);
        printf("需要花费金额: %d 元\n",G_myRainbowRestaurant.current_serve_user.order.ExpensesToBePaid);
    }
    printf("您当前账户中余额: %d\n", G_myRainbowRestaurant.current_serve_user.moneyInAccount);
    printf("=======================================\n\n");
    int selection = 0;
    while( !(selection>=1 && selection<=5) )
    {
        printf("请输入操作对应的序号: [ ]\b\b");
        scanf("%d", &selection);
        clearScreenBuffer();
    }
    system("cls");
    return selection;

}

//用户界面 开始###########################################################
void showSeatSelectMenu(void)
{
    printf("########## 河北大学 彩虹餐厅 #########\n\n");
    printf("这是餐厅的实时座位情况图.\n");
    printSeatsMap();
    printf("\n您可以按照 [排序号],[列序号] \n的输入格式来选择一个座位.\n\n");
    int row, col;
    while(1)
    {
        printf("我要选的位置: ");
        scanf("%d,%d", &row, &col);
        row -= 1;
        col -= 1;

        //判断座位是否可选择
        if( (col>=0 && col<SEATINFO_WIDTH) && (row>=0 && row<SEATINFO_HEIGHT) )
        {
            int isSeatAvailable = !(G_myRainbowRestaurant.seatsMap[row][col].isSelected);
            int self_row, self_col;
            self_row = G_myRainbowRestaurant.current_serve_user.seat.row;
            self_col = G_myRainbowRestaurant.current_serve_user.seat.col;
            int isSelfSeat = (row==self_row) && (col==self_col);
            
            if( isSelfSeat )//如果是自己之前选的座位
            {
                break;
            }

            if( isSeatAvailable )//如果是个空座位
            {
                break;
            }
            else//别人选过的座位
            {
                printf("这个位置已经有人了哦~\n");
                rainbow_sleep(1000);
            }
            
        }
        clearScreenBuffer();
    }
    if( G_myRainbowRestaurant.current_serve_user.seat.isSelected == 1 )
    {//如果原来选择过座位，即要更换座位
        int old_row, old_col;
        old_row = G_myRainbowRestaurant.current_serve_user.seat.row;
        old_col = G_myRainbowRestaurant.current_serve_user.seat.col;
        G_myRainbowRestaurant.seatsMap[old_row][old_col].isSelected = 0;//取消选择旧座位
    }
    G_myRainbowRestaurant.seatsMap[row][col].isSelected = 1;//选中用户输入的座位
    
    /*//下边这两行按理说是不用写的，因为生成database_seatMap文件的时候，
      //对应位置的seat结构体的row和col属性数据都应该是写好了的
      //但是也许实际上没有写好，但是我懒得检查了，就加上这两句修补一下?
      //G_myRainbowRestaurant.seatsMap[row][col].row = row;
      //G_myRainbowRestaurant.seatsMap[row][col].col = col;
    嘿嘿嘿解决了，上面的话当我没说*/
    G_myRainbowRestaurant.current_serve_user.seat = G_myRainbowRestaurant.seatsMap[row][col];
    writeSeatMapInfoToFile((struct Seat *)&G_myRainbowRestaurant.seatsMap);
    updateUserInfoToHistoryAndFile(G_myRainbowRestaurant.current_serve_user);
    printf("您选择了第 %d 排第 %d 列的座位.\n", row+1, col+1);
    rainbow_sleep(1000);
    system("cls");

}

void showFoodMenu(void)
{
    printf("########## 河北大学 彩虹餐厅 #########\n\n");
    printf("序号\t 菜名\t                单价\n");
    printf("=======================================\n\n");
    printLinkTable(G_myRainbowRestaurant.foods_we_have);
    printf("=======================================\n\n");
    printf("按Enter键返回用户上一级菜单...");
    getchar();
    system("cls");
    
}

void showOrderMenu(void)
{
    struct Order order;
    order.foodCount = 0;
    order.ExpensesToBePaid = 0;
    order.foods = NULL;
    NODE *pHead = NULL;
    
    while(1)
    {
        printf("########## 河北大学 彩虹餐厅 #########\n\n");
        int i;
        if( !(order.foodCount==0) )
        {
            printf("这是您已经点的菜品\n");
            printf("序号\t 名称\t                单价\n");
        }
        printf("===========================================\n\n");
        for(i=0; i<order.foodCount; i++)
        {
            printf("%d\t %-15s\t%-4dRMB\n",order.foods[i].id,order.foods[i].name,order.foods[i].price);
        }
        printf("\n===========================================\n");
        printf("总菜品数: %2d     应支付金额: %5d\n", order.foodCount, order.ExpensesToBePaid);
        printf("===========================================\n\n");
        printf("请输入您想点的菜对应的序号, \n输入 0 退出返回上一级菜单\n\n");
        static int addResult = -1;//初值反正不能是0
        if( !addResult )//如果上一次添加了非法foodID导致上一次添加失败，这里给与提示
        {
            printf("这个序号不存在，请输入正确的菜品序号.\n");
        }
        int foodID;
        printf("菜品序号: [ ]\b\b");
        scanf("%d", &foodID);
        clearScreenBuffer();
        system("cls");
        if( foodID==0 ) 
        {
            break;
        }
        else
        {
            addResult = addFoodToOrder(foodID, &order);
            /*if( !result )
            {
                printf("Wrong food id ! Please select the food exist.\n");
            }*/
        }
    }

    G_myRainbowRestaurant.current_serve_user.order = order;
    updateUserInfoToHistoryAndFile(G_myRainbowRestaurant.current_serve_user);

}

void showCheckMenu(void)
{
    printf("########## 河北大学 彩虹餐厅 #########\n\n");
    int fee;//本次应付费用
    int isVIP = G_myRainbowRestaurant.current_serve_user.IsVIP;
    if( isVIP )
    {
        fee = 0.8 * G_myRainbowRestaurant.current_serve_user.order.ExpensesToBePaid;
    }
    else
    {
        fee = G_myRainbowRestaurant.current_serve_user.order.ExpensesToBePaid;
    }
    printf("您总共需要支付 %5d 元 %s.\n", fee, isVIP?"(您是VIP，已打8折)":"");
    int moneyLeft = G_myRainbowRestaurant.current_serve_user.moneyInAccount-fee;
    printf("支付后您的帐户余额将变为 %d 元.\n", moneyLeft);

    if( moneyLeft>=0 )
    {
        printf("请扫描下边的二维码以确认本次支付\n\n");
        printf("\t   ■ ■■ ■■■ ■ ■ ■  \n");
        printf("\t   ■回■■ ■■■■ 回■   \n");
        printf("\t   ■ ■■ ■  ■■ ■■■  \n");
        printf("\t    ■■ ■■ ■■■■ ■■  \n");
        printf("\t   ■■■ ■■■■ ■ ■ ■  \n");
        printf("\t   ■回■■■ ■  ■回■   \n");
        printf("\t   ■■ ■■ ■■ ■ ■ ■  \n");
        //清空账单
        G_myRainbowRestaurant.current_serve_user.order.ExpensesToBePaid = 0;
        G_myRainbowRestaurant.current_serve_user.order.foodCount = 0;
        //扣钱！
        G_myRainbowRestaurant.current_serve_user.moneyInAccount -= fee;
        updateUserInfoToHistoryAndFile(G_myRainbowRestaurant.current_serve_user);
    }
    else
    {
        printf("\n然而您的账户中的余额不足以支付您本次用餐产生的费用.\n\n");
        printf("您现在有3个选择:\n\n");
        printf("\t1) 向账户中充值.\n\n");
        printf("\t2) 给餐厅洗盘子.\n\n");
        printf("\t3) 撒丫子快跑.\n\n");
        int selection = 0;
        while ( !(selection>=1 && selection<=3) )
        {
            printf("请输入操作对应的序号: [ ]\b\b");
            scanf("%d", &selection);
            clearScreenBuffer();
        }
        if( selection==1 )//充值
        {
            int moneyAdd = 0;
            while (1)
            {
                printf("请输入你要向账户里充值多少钱: \n");
                printf("> __________\b\b\b\b\b\b\b\b\b\b");
                scanf(" %d", &moneyAdd);
                if( moneyAdd>0 )
                {
                    G_myRainbowRestaurant.current_serve_user.moneyInAccount += moneyAdd;
                    updateUserInfoToHistoryAndFile(G_myRainbowRestaurant.current_serve_user);
                    printf("充值成功！\n");
                    printf("您的帐户当前余额: %d\n", G_myRainbowRestaurant.current_serve_user.moneyInAccount);
                    rainbow_sleep(1000);
                    system("cls");
                    return;//返回上一级菜单
                }
            }
        }
        else if( selection==2 )//洗盘子
        {
            int dishToWashNum = fee;//欠1块钱洗一个
            printf("\n您需要洗%d个盘子\n\n", dishToWashNum);
            printf("开始工作\n");
            int i;
            for(i=1; i<=dishToWashNum; i++)
            {
                printf("\t您正在清洗第 %d 个盘子...", i);
                rainbow_sleep(1);//每秒洗1000个盘子
                printf("\r");
            }
            G_myRainbowRestaurant.current_serve_user.order.ExpensesToBePaid = 0;
            G_myRainbowRestaurant.current_serve_user.order.foodCount = 0;
            printf("\n恭喜您已完成所有工作，本次餐费抵消\n");
            printf("\n\n按任意键返回上一级菜单.\n");
            getchar();
            system("cls");
            return;//返回上一级菜单
        }
        else if( selection==3 )//跑路
        {
            printf("\t系统已自动报警！\n");
            int i;
            for(i=3; i>=0; i--)
            {
                printf("\t警察还有 %d 秒到达现场...", i);
                rainbow_sleep(1000);
                printf("\r");
            }
            printf("\n\t你被警察抓走了 ！\n\n");
            printf("\n\n按任意键返回上一级菜单.\n");
            getchar();
            system("cls");
            return;
        }
        system("cls");
    }
    
    //取消座位选择
    int customer_row = G_myRainbowRestaurant.current_serve_user.seat.row;
    int customer_col = G_myRainbowRestaurant.current_serve_user.seat.col;
    G_myRainbowRestaurant.seatsMap[customer_row][customer_col].isSelected = 0;
    writeSeatMapInfoToFile((struct Seat *)&G_myRainbowRestaurant.seatsMap);

    printf("\n按任意键退出.\n");
    getchar();
    system("cls");
    printf("########## 河北大学 彩虹餐厅 #########\n\n");
    printf("\n\t祝您用餐愉快 !\n\n\t欢迎您下次光临\n\n\n");
    rainbow_sleep(1000);
    exit(EXIT_SUCCESS);

}
//用户界面 结束###########################################################

//管理员界面 开始###########################################################
void showUsersInfo_admin(void)
{
    while(1)
    {
        printf("########## 河北大学 彩虹餐厅 #########\n\n");
        printf("用户名\t账户余额\t是否VIP\t正在用餐\n\n");
        int i;
        struct User user;
        for(i=0; i<G_myRainbowRestaurant.historyUsersCount; i++)
        {
            user = G_myRainbowRestaurant.history_serve_users[i]; 
            printf("%-8s%-20d%-9s%-9s\n", user.userNick, user.moneyInAccount, user.IsVIP?"√":"×", user.seat.isSelected?"√":"×");
        }
        printf("\n--------------------------------------\n");
        printf("输入 \"set [用户名] [账户余额]\" 来设置顾客账户余额\n");
        printf("输入 \"del [用户名]\" 来删除顾客信息\n");
        printf("输入 \"quit\" to 返回上一级菜单\n\n");
        printf("@admin > ");
        {
            char cmd[30];
            scanf("%s", cmd);
            if( strcmp(cmd, "set")==0 )
            {
                char nickName[USERNICK_LENGTH_MAX+1];
                int money;
                scanf("%s", nickName);
                scanf(" %d", &money);
                setUserMoneyFromUsersHistory(nickName, money);
            }
            else if( strcmp(cmd, "del")==0 )
            {
                char nickName[USERNICK_LENGTH_MAX+1];
                scanf("%s", nickName);
                if( isThisUserInUsersHistory(nickName) )
                {
                    deleteUserFromUsersHistory(nickName);
                }
                else
                {
                    printf("这个用户不存在，请检查输入！\n");
                    rainbow_sleep(1000);
                }
                
            }
            else if( strcmp(cmd, "quit")==0 )
            {
                clearScreenBuffer();
                system("cls");
                break;
            }
            else
            {
                printf("无法识别的命令");
                rainbow_sleep(500);
            }
        }
        clearScreenBuffer();
        system("cls");
    }


}

void showUsersManagement_CURD_admin(void)
{
    printf("########## 河北大学 彩虹餐厅 #########\n\n");
    printf("系统中当前有 1 个管理员.\n\n");
    printf("\t1) 修改管理员密码.\n\n");
    printf("\t2) 返回上一级菜单.\n\n");
    int selection = 0;
    while ( !(selection>=1 && selection<=2) )
    {
        printf("请输入操作对应的序号: [ ]\b\b");
        scanf("%d", &selection);
        clearScreenBuffer();
    }
    if( selection==1 )
    {
        char password[ADMINPAS_LENGTH_MAX+1];
        printf("请输入新的密码.");
        scanf("%s", password);
        writeSuperAdminPasswordToFile(password);
        printf("修改成功！");
        rainbow_sleep(1000);
    }
    else if( selection==2 )
    {
        ;
    }
    system("cls");

}

void showFoodsManagement_CURD_admin(void)
{
    while (1)
    {
        printf("########## 河北大学 彩虹餐厅 #########\n\n");
        printf("序号\t 菜品名\t                单价\n");
        printf("=======================================\n\n");
        printLinkTable(G_myRainbowRestaurant.foods_we_have);
        printf("=======================================\n\n");
        printf("输入 \"add [菜品名] [单价]\" 来添加菜品\n");
        printf("输入 \"del [序号]\" 来删除菜品\n");
        printf("输入 \"quit\" to 返回上一级菜单\n\n");
        printf("@admin > ");
        {
            char cmd[30];
            scanf("%s", cmd);
            if( strcmp(cmd, "add")==0 )
            {
                //接受食物数据
                char name[30];
                int price;
                int id;
                scanf(" %s", name);
                scanf(" %d", &price);
                id = getAvailableFoodID();
                
                //构造食物结构体
                struct food newFood;
                newFood.id = id;
                strcpy(newFood.name, name);
                newFood.price = price;
                newFood.orderCount = 0;

                //更新食物数据到系统并且存盘
                addFoodToRestaurant(&newFood);
                G_myRainbowRestaurant.foodsCount++;
                saveFoodInfo();

            }
            else if( strcmp(cmd, "del")==0 )
            {
                if( !(0==G_myRainbowRestaurant.foodsCount) )//如果还有食物可删
                {
                    int id;
                    scanf(" %d", &id);
                    G_myRainbowRestaurant.foods_we_have = deleteRestaurantFoodByID(G_myRainbowRestaurant.foods_we_have, id);
                    G_myRainbowRestaurant.foodsCount--;
                    saveFoodInfo();
                }
            }
            else if( strcmp(cmd, "quit")==0 )
            {
                clearScreenBuffer();
                system("cls");
                break;
            }
            else
            {
                printf("无法识别的命令");
                rainbow_sleep(500);
            }
        }
        clearScreenBuffer();
        system("cls");
    }

}

void showSeatMapInfo_admin(void)
{
    printf("########## 河北大学 彩虹餐厅 #########\n\n");
    printf("这是餐厅实时座位图.\n");
    printSeatsMap();
    int customer_count = 0;
    int i, j;
    for(i=0; i<SEATINFO_HEIGHT; i++)
    {
        for(j=0; j<SEATINFO_WIDTH; j++)
        {
            if( G_myRainbowRestaurant.seatsMap[i][j].isSelected )
            {
                customer_count++;
            }
        }
    }
    printf("=======================================\n");
    printf("当前餐厅中有 %d 位顾客.\n", customer_count);
    printf("=======================================\n");
    printf("按Enter返回上一级菜单...");
    getchar();
    system("cls");

}


void showStatisticsAnalysis_admin(void)
{
    printf("########## 河北大学 彩虹餐厅 #########\n");
    printf("\n【这是当前最受顾客欢迎的 3 个菜品.】\n");
    printPopularFood(3);
    printf("\n【这是当前账户余额最多的 3 个顾客.】\n");
    printRichUser(3);

    //计算餐厅当前顾客个数、顾客账户中余额总和
    int i, j, MoneySum = 0;
    for(i=0; i<G_myRainbowRestaurant.historyUsersCount; i++)
    {
        MoneySum += G_myRainbowRestaurant.history_serve_users[i].moneyInAccount;
    }

    //计算餐厅座位上座率
    int current_customer_count = 0;
    for(i=0; i<SEATINFO_HEIGHT; i++)
    {
        for(j=0; j<SEATINFO_WIDTH; j++)
        {
            if( G_myRainbowRestaurant.seatsMap[i][j].isSelected )
            {
                current_customer_count++;
            }
        }
    }
    int totalSeatsNum = SEATINFO_HEIGHT*SEATINFO_WIDTH;
    float occupancyRate = (float)current_customer_count / totalSeatsNum;
    
    printf("\n【其他数据】\n");
    printf("---------------------------------------\n");
    printf("当前餐厅用户总数：%d 人\n", G_myRainbowRestaurant.historyUsersCount);
    printf("当前餐厅上座率：%.1f%% (%d/%d)\n", occupancyRate*100, current_customer_count, totalSeatsNum);
    printf("当前餐厅总资产：%d 元\n", MoneySum);
    printf("---------------------------------------\n\n\n");
    printf("按Enter返回上一级菜单...");
    getchar();
    system("cls");

}
//管理员界面 结束###########################################################

void showExitProgram(void)
{
    printf("########## 河北大学 彩虹餐厅 #########\n\n");
    if( G_myRainbowRestaurant.current_serve_user.userType == customer )
    {
        printf("尊敬的顾客，祝您用餐愉快！\n\n");
        printf("您可以稍后再来结账。\n\n");
    }
    else if( G_myRainbowRestaurant.current_serve_user.userType == admin )
    {
        printf("尊敬的系统管理员，您已安全退出系统。\n\n");
    }
    else
    {
        printf("\t您已退出系统。\n\n");
    }
    rainbow_sleep(2000);
    exit(EXIT_SUCCESS);
}
