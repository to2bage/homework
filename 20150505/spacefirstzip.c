/*
    先压缩连续重复的字符串， 再压缩连续重复的字符

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

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

int main_spacefirstzip(int argc, char *argv[])
{
    //压缩calc calc calc calc calc ==> 5 calc
    char str[] = "calc calc calc calc calc calc calc aaaa aaaa aaaa aaaa ";       //最后一个单词后必须有一个空格
    char *px = (char *)malloc((strlen(str) + 1) * sizeof(char));
    memset(px, 0, (strlen(str) + 1) * sizeof(char));
    memcpy(px, str, strlen(str) * sizeof(char));
    //printf("%s\n", px);

    //压缩单词，单词都是以空格结束的
    //1.获得当前指针指向的单词
    //char *pword = getWord(px);
    //printf("%s, %d\n", pword, strlen(pword));       //calc, 4 用于测试

    //2.开始压缩连续重复的单词
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

    return 0;
}
