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
void menu(void)
{
    printf("");
}

int main(int argc, char const *argv[])
{
    // 创建并打开数据库
    sqlite3 *ppDb = creat_open_db("./test.db");

    // 穿件一个储存学生信息的表
    char *table_name = "student";                                     // 表的名字
    char *in_data = "id int primary key not NULL,text name, age int"; // 表的内容
    create_table(ppDb, table_name, in_data);

    menu();
    // 插入数据
    int id,
        age, i;
    char name[32] = {0};
    char sql[128] = {0};

    for (i = 0; i < 2; i++)
    {
        memset(name, 0, 32);
        printf("请输入id, 姓名， 年龄\n");
        scanf("%d%s%d", &id, name, &age);
        memset(sql, 0, 128);
        sprintf(sql, "insert into student values (%d,'%s',%d);", id, name, age);
        int ret = sqlite3_exec(ppDb, sql, NULL, NULL, NULL); // 第三个参数回调函数暂时不需要
        if (ret != SQLITE_OK)
        {
            printf("sqlite3_exec insert error: %s\n", sqlite3_errmsg(ppDb));
            exit(3);
        }
    }

    return 0;
}
