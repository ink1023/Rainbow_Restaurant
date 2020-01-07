#include "rainbow_CORE.h"
#include "rainbow_IO.h"
#include "rainbow_UI.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    //数据文件读取、全局变量初始化
    rainbow_init(argc, argv);
    //让用户选择自己的身份
    enum UserType userType = showMenu_Welcome();
    if( userType==admin )//当前用户是管理员
    {
        while(1)
        {
            int selection = showMenu_admin();
            switch (selection)
            {
                case 1://增加、删除餐厅菜品.
                    showFoodsManagement_CURD_admin();
                    break;
                case 2://查看、修改餐厅用户信息.
                    showUsersInfo_admin();
                    break;
                case 3://查看餐厅实时座位占用情况.
                    showSeatMapInfo_admin();
                    break;
                case 4://菜品、用户数据分析.
                    showStatisticsAnalysis_admin();
                    break;
                case 5://系统管理员信息修改.
                    showUsersManagement_CURD_admin();
                    break;
                case 6://退出程序.
                    showExitProgram();
                    break;
            }
        }
    }
    else if( userType==customer )//当前用户是顾客
    {
        while(1)
        {
            int selection = showMenu_customer();
            switch (selection)
            {
                case 1://先选一个座位坐好.
                    showSeatSelectMenu();
                    break;
                case 2://看看这里有什么菜品.
                    showFoodMenu();
                    break;
                case 3://我要点一些吃的.
                    showOrderMenu();
                    break;
                case 4://我要先去吃饭了.
                    showExitProgram();
                    break;
                case 5://吃饱了，我要结账.
                    showCheckMenu();
                    break;
            }
        }
    }
    
    return 0;
}



