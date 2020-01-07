#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rainbow_CORE.h"

void readFoodInfoFromFile(struct food *(*foods), int *num)
{
    FILE *fp;
    if( fp = fopen("dataBase_food.rainbow", "rb") )
    {
        (*num) = 0;
        int i = 0;
        fread(num, sizeof(int), 1, fp);
        (*foods) = (struct food *)calloc((*num), sizeof(struct food));
        if_Is_Null_Then_End_Program(*foods);
        fread(*foods, sizeof(struct food), (*num), fp);
        //下边这个方法也被弃用了，因为我想直接把有多少个food的值存储在文件的开头
        /*while(1)
        {
            struct food foodData;
            fread(&foodData, sizeof(struct food), 1, fp);
            if( !feof(fp) )
            {
                foods[i] = foodData;
                i++;
                (*num)++;
            }
            else
            {
                break;
            }
        }*/
        //下边是一个踩的坑
        // feof()函数当fp指向文件尾的时候依然无法返回true，需要再次read一下之后才能feof()==true (虽然最后这一次read读不到有意义的数据)
        // 下边的代码会多写入一次food数据，最后一次是占用了非法空间的非法数据
        //for(i=0 ; !feof(fp); i++)
        //{
        //    fread(&foods[i], sizeof(struct food), 1, fp);
        //    (*num)++;
        //}
        fclose(fp);
        
    }
    else
    {
        printf("\"dataBase_food\" open failed while try to read the file.\n");
        exit(EXIT_FAILURE);
    }
}

void writeFoodInfoToFile(struct food foods[], int num)
{
    FILE *fp;
    if( fp = fopen("dataBase_food.rainbow", "wb") )
    {
        fwrite(&num, sizeof(int), 1, fp);
        fwrite(foods, sizeof(struct food), num, fp);
        fclose(fp);
    }
    else
    {
        printf("\"dataBase_food\" open failed while try to write to file.\n");
        exit(EXIT_FAILURE);
    }
    
}

void writeUserInfoToFile(struct User user[], int num)
{
    FILE *fp;
    if( fp = fopen("dataBase_user.rainbow", "wb") )
    {
        fwrite(&num, sizeof(int), 1, fp);
        fwrite(user, sizeof(struct User), num, fp);
        fclose(fp);
    }
    else
    {
        printf("\"dataBase_user\" open failed while try to write to file.\n");
        exit(EXIT_FAILURE);
    }
    
}

void readUserInfoFromFile(struct User *(*users), int *num)
{
    FILE *fp;
    if( fp = fopen("dataBase_user.rainbow", "rb") )
    {
        (*num) = 0;
        int i = 0;
        fread(num, sizeof(int), 1, fp);
        if( !(num==0) )
        {
            (*users) = (struct User *)calloc((*num), sizeof(struct User));
            if_Is_Null_Then_End_Program(*users);
        }
        else
        {
            (*users) = NULL;
        }
        fread(*users, sizeof(struct User), (*num), fp);
        fclose(fp);
        
    }
    else
    {
        printf("\"dataBase_user\" open failed while try to read the file.\n");
        exit(EXIT_FAILURE);
    }
}

char* readSuperAdminPasswordFromFile(void)
{
    FILE *fp;
    fp = fopen("dataBase_superAdminPassword.rainbow", "r");
    char *pPassword = (char *)malloc(30 * sizeof(char));
    fgets(pPassword, 30+1, fp);
    fclose(fp);
    return pPassword;
}

void writeSuperAdminPasswordToFile(char password[])
{
    if( strlen(password) > ADMINPAS_LENGTH_MAX )//先这么限制一下，回头再搞不定长字符串
    {
        printf("密码太长了，只有前 %d 个字符被储存.\n", ADMINPAS_LENGTH_MAX);
        rainbow_sleep(1000);
    }
    FILE *fp;
    fp = fopen("dataBase_superAdminPassword.rainbow", "w");
    fputs(password, fp);
    fclose(fp);
}

void writeSeatMapInfoToFile(struct Seat *pSeatsMap)
{
    FILE *fp;
    if( fp = fopen("dataBase_seatsMap.rainbow", "wb") )
    {
        fwrite(pSeatsMap, sizeof(struct Seat), SEATINFO_HEIGHT*SEATINFO_WIDTH, fp);
        fclose(fp);
    }
    else
    {
        printf("\"dataBase_seatsMap\" open failed while try to write to file.\n");
        exit(EXIT_FAILURE);
    }
}

void readSeatMapInfoFromFile(struct Seat *pSeatsMap)
{
    FILE *fp;
    if( fp = fopen("dataBase_seatsMap.rainbow", "rb") )
    {
        //(*pSeatsMap) = (struct Seat *)calloc(SEATINFO_HEIGHT*SEATINFO_WIDTH, sizeof(struct Seat));
        fread(pSeatsMap, sizeof(struct Seat), SEATINFO_HEIGHT*SEATINFO_WIDTH, fp);
        fclose(fp);
    }
    else
    {
        printf("\"dataBase_seatsMap\" open failed while try to read the file.\n");
        exit(EXIT_FAILURE);
    }
}

void createFreshDataBaseFiles(void)
{
    //这段代码用来生成一个全新的dataBase_seatMap
    int row,col;
    for(row=0; row<SEATINFO_HEIGHT; row++)
    {
        for(col=0; col<SEATINFO_WIDTH; col++)
        { 
            G_myRainbowRestaurant.seatsMap[row][col].isSelected = 0;
            G_myRainbowRestaurant.seatsMap[row][col].row = row;
            G_myRainbowRestaurant.seatsMap[row][col].col = col;
        }
    }
    writeSeatMapInfoToFile((struct Seat *)&G_myRainbowRestaurant.seatsMap);
    
    //生成空的dataBase_food
    writeFoodInfoToFile((struct food *)0, 0);

    //生成空的dataBase_user
    writeUserInfoToFile((struct User *)0,0);

    //写入初始密码为12345
    writeSuperAdminPasswordToFile("12345");
    
}