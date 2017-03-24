#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

//得到以空格结尾的字符串//5 calc 4 aaaa , 14
static char *getStrOfSpaceEnding(char *pstr)
{
    //pstr指向要重复的字符串的首个字符
    char *px = pstr;
    while(*px != ' ')
    {
        px++;
    }
    //此时，指针px指向字符串的空格位置

    int strlen = px - pstr + 1;         //px-pstr是重复的字符串的长度，+1给空格字符留位置

    //申请内存
    char *pword = (char *)malloc((strlen + 1) * sizeof(char));      //+1表示给'\0留位置
    memset(pword, 0, (strlen + 1) * sizeof(char));

    //给新分配的内存赋值
    int i = 0;
    for(; i < px - pstr; i++)
    {
        pword[i] = pstr[i];
    }
    pword[i] = ' ';                     //字符串是以空格字符结尾的

    return pword;
}


int main(int argc, char *argv[])
{
    char str[] = "5 calc 4 4a ";
    //char str[] = "5 calc 4 aaaa ";
    char *px = (char *)malloc((strlen(str) + 1) * sizeof(char));
    memset(px, 0, (strlen(str) + 1) * sizeof(char));
    strcpy(px, str);
    printf("%s\n", px);


    //1.首先解压数字后直接是字符，而不是空格的字符串(表示字符出现多少次)
    char *p1 = px;
    char *p2 = px;

    while((*p1 = *p2))
    {
        if((*p2 >= '0' && *p2 <= '9') && (*(p2 + 1) != ' '))
        {
            //当前指针p2指向的是数字字符，并且下一个字符不是空格字符
            char chnext = *(p2 + 1);    //指针p2指向的字符的下一个字符
            int num = *p2 - '0';        //字符要重复的个数
            int posLen = num - 2;       //字符串要扩大的个数

            int pos1 = p1 - px;         //记录指针p1相对于字符串px首地址的偏移量
            int pos2 = p2 - px;         //记录指针p2相对于字符串px首地址的偏移量

            px = (char *)realloc(px, (strlen(px) + posLen) * sizeof(char)); //扩大字符串px的内存    ??? ??? ???+1

            p1 = px + pos1;             //恢复指针p1的现在的位置
            p2 = px + pos2;             //恢复指针p2的现在的位置

            //从末尾向前移动字符串
            char *pend = px + strlen(px);//realloc不会初始化字符串px，所以这里要把字符'\0'也往后移动
            while(pend >= p2)
            {
                *(pend + posLen) = *pend;
                pend--;
            }

            //赋值源字符串px，从指针p2指向的地址开始的num个字符，为*(p2 + 1)
            for(int i = 0; i < num; i++)
            {
                p2[i] = chnext;
            }

        }
        else
        {
            p1++;
            p2++;
        }
    }

    printf("%s, %d\n", px, strlen(px));         //5 calc 4 aaaa , 14

    //2.再解压字符串（数字+空格+字符串： 表示字符串重复数字个)
    p1 = px;
    p2 = px;

    while((*p1 = *p2))
    {
        if((*p2 >= '0' && *p2 <= '9') && (*(p2 + 1) == ' '))
        {
            //当前指针p2指向的是数字字符，而下一个字符是空格字符
            int count = *p2 - '0';      //字符串要重复的个数
            p2 += 2;                    //p2后移两位字符，指向要重复的字符串的首个字符
            char *pword = getStrOfSpaceEnding(p2);  //pword是以空格结尾的字符串

            //计算要增加的字节数
            int wordLen = strlen(pword);    //5
            int capacity = (count - 1) * wordLen - 2;     //-1是因为源字符串px中已经有1个要重复的字符串的长度了;
                                                          //-2是因为原字符串px中还包含数字字符和其后的空格字符，这是解压缩后的字符串中是不需要的

            int pos1 = p1 - px;         //记录指针p1相对于字符串px首地址的偏移量
            int pos2 = p2 - px;         //记录指针p2相对于字符串px首地址的偏移量

            px = (char *)realloc(px, (strlen(px) + capacity + 1) * sizeof(char));   //+1表示字符'\0’的位置

            p1 = px + pos1;             //恢复指针p1的位置
            p2 = px + pos2;             //恢复指针p2的位置

            //从后向前移动字符串
            char *pend = px + strlen(px);   //pend指向字符串px的结尾字符'\0'
            while(pend >= p2)
            {
                *(pend + capacity) = *pend;
                pend--;
            }

            //将相应的地方填充上字符pword
            for(int i = 0; i < capacity + 2; i++)     //+2 对于
            {
                p1[i] = pword[i % wordLen];
                //printf("%c\n", p1[i] = pword[i % wordLen]);
            }
            printf("\n");
            p2 = p1;
        }
        else
        {
            p1++;
            p2++;
        }
    }

    printf("%s, %d\n", px, strlen(px));

    return 0;
}

int main1(int argc, char *argv[])
{
    {
        char *prect = getStrOfSpaceEnding("calc 4 aaaa");
        printf("%s, %d\n", prect, strlen(prect));
    }
}
