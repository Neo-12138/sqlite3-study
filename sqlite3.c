#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <unistd.h>

// 数据库创建并打开
sqlite3 *creat_open_db(char *path) // path在哪个路径创建数据库
{
    // 打开创建数据库
    sqlite3 *ppDb;
    int ret = sqlite3_open(path, &ppDb);
    // SQLITE_OK表示成功打开
    if (ret != SQLITE_OK)
    {
        printf("sqlite3 %s\n", sqlite3_errmsg(ppDb)); // 打印错误码
        exit(1);
    }
    return ppDb;
}

// 在指定的数据库创建表，自定义表的内容
void *create_table(sqlite3 *ppDb, char *table_name, char *in_data) // in_data 表的内容  table_name 标的名字
{
    char sql[128] = {0};
    sprintf(sql, "create table if not exists %s(%s);", table_name, in_data);
    int ret = sqlite3_exec(ppDb, sql, NULL, NULL, NULL); // 第三个参数回调函数暂时不需要
    if (ret != SQLITE_OK)
    {
        printf("sqlite3_exec1 error: %s\n", sqlite3_errmsg(ppDb));
        exit(2);
    }
}

// 数据库选择界面
int menu(void)
{
    printf("1.插入数据\n");
    printf("2.删除数据\n");
    printf("3.修改数据\n");
    printf("4.查询数据\n");
    printf("0.退出\n");
    printf("请选择：\n");
    int n;
    scanf("%d\n", &n);
    while (getchar() != '\n')
        ;
    return n;
}

// 数据库加入数据  ..表的名字
void sqlite3_add(sqlite3 *ppDb, char *table_name)
{
    // 插入数据
    int id, age;
    char name[32] = {0};
    char sql[128] = {0};

    memset(name, 0, 32);
    printf("请输入id, 姓名， 年龄\n");
    scanf("%d%s%d", &id, name, &age);
    while (getchar() != '\n')
        ;
    memset(sql, 0, 128);
    sprintf(sql, "insert into %s values (%d,'%s',%d);", table_name, id, name, age);
    int ret = sqlite3_exec(ppDb, sql, NULL, NULL, NULL); // 第三个参数回调函数暂时不需要
    if (ret != SQLITE_OK)
    {
        printf("sqlite3_exec insert error: %s\n", sqlite3_errmsg(ppDb));
        exit(3);
    }
}

// 数据库删除数据
void sqlite3_delete(sqlite3 *ppDb, char *table_name)
{
    int id;
    char sql[128] = {0};
    printf("请输入你要删除的id\n");
    scanf("%d", &id);
    while (getchar() != '\n')
        ;
    memset(sql, 0, 128);
    sprintf(sql, "delete from %s where id = %d;", table_name, id);
    int ret = sqlite3_exec(ppDb, sql, NULL, NULL, NULL); // 第三个参数回调函数暂时不需要
    if (ret != SQLITE_OK)
    {
        printf("sqlite3_exec insert error: %s\n", sqlite3_errmsg(ppDb));
        exit(3);
    }
}

// 数据库修改数据
void sqlite3_change(sqlite3 *ppDb, char *table_name)
{
    int id, age, n;
    char name[32] = {0};
    char sql[128] = {0};
    printf("请输入你要修改的id\n");
    while (getchar() != '\n')
        ;
    scanf("%d", &id);
    printf("请选择你要修改的数据：1.姓名|2.年龄\n");
    scanf("%n", &n);
    while (getchar() != '\n')
        ;
    if (n == 1)
    {
        printf("请输入你要修改的名字\n");
        scanf("%s", name);
        while (getchar() != '\n')
            ;
        memset(sql, 0, 128);
        sprintf(sql, "update %s set name = %s wher id = %d;", table_name, name, id);
        sqlite3_exec(ppDb, sql, NULL, NULL, NULL);
    }
    else if (n == 2)
    {
        printf("请输入你要修改的年龄\n");
        scanf("%d", &age);
        while (getchar() != '\n')
            ;
        memset(sql, 0, 128);
        sprintf(sql, "update %s set age = %d wher id = %d;", table_name, age, id);
        sqlite3_exec(ppDb, sql, NULL, NULL, NULL);
    }
    else
    {
        printf("输入有误，请重新选择\n");
    }
    return;
}

// 数据库查看表总数据
void sqlite3_show(sqlite3 *ppDb, char *table_name)
{
    // char sql[128] = {0};
    // memset(sql, 0, 128);
    // sprintf(sql, ".header on");
    // sqlite3_exec(ppDb, sql, NULL, NULL, NULL);

    // memset(sql, 0, 128);
    // sprintf(sql, ".mode column");
    // sqlite3_exec(ppDb, sql, NULL, NULL, NULL);

    // memset(sql, 0, 128);
    // sprintf(sql, "select *from %s;", table_name);
    // sqlite3_exec(ppDb, sql, NULL, NULL, NULL); // 第三个参数回调函数暂时不需要
}

int main(int argc, char const *argv[])
{
    // 创建并打开数据库
    sqlite3 *ppDb = creat_open_db("./test.db");

    // 穿件一个储存学生信息的表
    char *table_name = "student";                                     // 表的名字
    char *in_data = "id int primary key not NULL,name text, age int"; // 表的内容
    create_table(ppDb, table_name, in_data);

    while (1)
    {
        int ret = menu();
        switch (ret)
        {
        case 1:
            sqlite3_add(ppDb, "student"); // 增
            break;
        case 2:
            sqlite3_delete(ppDb, "student"); // 以id为删除标志
            break;
        case 3:
            sqlite3_change(ppDb, "student"); // 修改
            break;
        case 4:
            sqlite3_show(ppDb, "student");
            break;
        case 0:
            break;
        }
    }

    return 0;
}
