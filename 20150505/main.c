
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

static char *spaceFirstZip(char *pstr);
static void spaceFirstUnzip(char **ppstr);

int main1(int argc, char *argv[])
{
    {
        //calc calc calc calc calc aaaa aaaa aaaa aaaa
        //压缩为： 5 calc 4 4a
        //意为: 数字[空格]字符串数字[空格]字符串
        //数字和字符之间有空格表示是字符串的重复； 数字和字符之间没有空格表示是字符的重复
        {
            //压缩之空间优先算法
            char str[] = "calc calc calc calc calc calc calc aaaa aaaa aaaa aaaa ";       //最后一个单词后必须有一个空格
            char *px = (char *)malloc((strlen(str) + 1) * sizeof(char));
            memset(px, 0, (strlen(str) + 1) * sizeof(char));
            memcpy(px, str, strlen(str) * sizeof(char));

            char *prect = spaceFirstZip(px);
            printf("压缩之空间优先算法: %s\n", px);
        }
    }

    {
//        // 5 calc 4 4a  ==> calc  calc  calc  clac  calc  4a 4a 4a 4a
//        // ==> calc calc calc calc calc aaaa aaaa aaaa aaaa
//        char srcStr[25] = "5 calc 4 aa";    //产量字符串
//        //因为会修改字符串的首地址和内容，所以不能用常量字符串，要使用堆上的字符串
//        char *px = (char *)malloc((strlen(srcStr) + 1) * sizeof(char));
//        memset(px, 0, (strlen(srcStr) + 1) * sizeof(char));
//        strcpy(px, srcStr);
//        {
//            //解压缩之空间优先算法
//            spaceFirstUnzip(&px);
//            printf("解压缩之空间优先算法: %s\n", px);
//        }
    }
    {
        //
    }
    return 0;
}

static int getNumberOfChar(char *pstr)
{
    int num = 0;

    while((*pstr < '0' || *pstr > '9') && *pstr != '\0')
    {
        num++;
        pstr++;
    }

    return num;
}

//解压缩空间优先算法
static void spaceFirstUnzip(char **ppstr)
{
    char *p1 = *ppstr;
    char *p2 = *ppstr;

    while((*p1 = *p2))
    {
        int number = 0;
        if(*p2 >= '0' && *p2 <= '9')
        {
            //如果指向的是数字字符
            number = *p2 - '0';
            printf("num = %d\n", number);
            p2++;   //p2指向下一个操作字母字符
            //获得要重复的字母字符的个数count和要扩容的空间大小totalSize
            int count = getNumberOfChar(p2);
            int totalSize = number * count - count; //减去count是因为已经有一个字符串了
            printf("count = %d, totalsize = %d\n", count, totalSize);
            //在分配内存之前，先记录当前p1和p2的相对位置
            int pos1 = p1 - *ppstr;
            int pos2 = p2 - *ppstr;
            //扩展内存
            *ppstr = (char *)realloc(*ppstr, (strlen(*ppstr) + totalSize + 1) * sizeof(char));

            //获得当前p1和p2的位置
            p1 = *ppstr + pos1;
            p2 = *ppstr + pos2;
            //从后向前移动元素
            char *pend = *ppstr + strlen(*ppstr);
            while(pend >= p2)
            {
                *(pend + totalSize) = *pend;
                pend--;
            }
            //拷贝数据
            char *p3 = p2;
            for(int i = 0; i < totalSize; i++)
            {
                p3[i] = p2[i % count];  //count是calc的长度
            }
        }
        else
        {
            //如果指向的不是数字字符
            *p1++ = *p2++;
        }
    }
}


//获得以空格结尾的单词
static char *getWord(char *pstr)
{
    char *p1 = pstr;
    char *p2 = pstr;
    while(*p2 != ' ')
    {
        p2++;
    }//此时p2指向空格

    //分配内存，容纳获取的单词
    char *pword = (char *)malloc((p2 - p1 + 1) * sizeof(char)); //p2-p1获得单词的长度， +1添加字符'\0'
    memset(pword, 0, (p2 - p1 + 1) * sizeof(char)); //初始化为0
    //拷贝单词到pword中
    strncpy(pword, p1, (p2 - p1) * sizeof(char));

    return pword;
}

//压缩之空间优先算法
static char *spaceFirstZip(char *px)
{
    char *p1 = px;
    char *p2 = px;

    while((*p1 = *p2))
    {
        char *word = getWord(p2);       //这里必须是p2， 因为p2始终指向新单词的首字母,如果用p1则会在一个单词内存重复的取单词
        int wordLen = strlen(word);     //标记单词的长度
        char *p3 = p2 + wordLen + 1;    //p3指向下一个单词的首字符  +1跳过空格
        int count = 1;                  //记录连续相同单词的个数

        if(*p3 != '\0' && strcmp(getWord(p2), getWord(p3)) == 0)
        {
            while(*p3 != '\0' && strcmp(getWord(p2), getWord(p3)) == 0)
            {
                count++;
                p2 = p3;
                p3 += wordLen + 1;
            }

            //
            *p1 = count + '0';
            *++p1 = ' ';
            p1++;
        }
        else
        {
            p1++;
            p2++;
        }
    }

    //打印
    printf("%s, %d\n", px, strlen(px));

    //3.开始压缩连续重复的字符
    p1 = px;
    p2 = px;

    while((*p1 = *p2))
    {
        if(*p2 == *(p2 + 1))
        {
            char *p3 = p2;
            int num = 0;
            while(*p2 == *p3)
            {
                num++;
                p3++;
            }//此时p1和p2都指向重复字符的第一个字符的位置
            *p1 = num + '0';
            *++p1 = *(p3 - 1);  //注意此时P2值已经改变了
            p1++;
            //
            p2 = p3;
        }
        else
        {
            p1++;
            p2++;
        }
    }



    //打印
    printf("%s, %d\n", px, strlen(px));
}
