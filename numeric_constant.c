#include<stdio.h>
#include"lexical_header.h"
#include<ctype.h>
#include<stdlib.h>

void float_constant(char float_buff[])
{
    int i;
    int flag1 = 0;
    int flag2 = 0;
    for(i = 0; float_buff[i]!='\0'; i++)
    {
        if(isalpha(float_buff[i]) && float_buff[i]!='f')
        {
            flag1++;
            printf("error: invalid suffix \" %s \" on floating constant\n", float_buff + i);
            exit(1) ;
        }
        if(float_buff[i] == '.')
        {
            flag2++;
        }
    }
    if(flag2 > 1)
    {
        printf("error: too many decimal points in number\n");
        exit(1);
    }

    if(flag1 == 0 && flag2 ==1)
    {
        printf("Float constant : %s\n", float_buff);
    }
}
void hex_constant(char hex_buff[])
{
    if(!(isxdigit(hex_buff[2])))
    {
        printf("error : invalid suffix \"%s\" on integer constant\n",hex_buff+1);
        exit(1); 
    }
    int j ;
    for(int j = 3; hex_buff[j]!='\0';j++)
    {
        if(!(isxdigit(hex_buff[j])))
        {
            printf("error : invalid suffix \"%s\" on integer constant\n",hex_buff + j);
            exit(1);
        }
    }
    printf("Hex constant : %s\n", hex_buff);
}

void binary_constant(char bin_buff[])
{
    if(bin_buff[2]!='0' && bin_buff[2]!='1')
    {
        printf("error : invalid suffix \"%s\" on integer constant\n",bin_buff+1);
        exit(1); 
    }
    int j ;
    for(int j = 3; bin_buff[j]!='\0';j++)
    {
        if(bin_buff[j]!='0' && bin_buff[j]!='1')
        {
            printf("error : invalid suffix \"%s\" on integer constant\n",bin_buff + j);
            exit(1);
        }
    }
    printf("Binary constant : %s\n", bin_buff);
    return;
}

void octal_constant(char oct_buff[])
{
    for(int j = 0; oct_buff[j]!='\0';j++)
    {
        if(!(oct_buff[j]>='0' && oct_buff[j]<='7'))
        {
            printf("error : invalid suffix \"%c\" on integer constant\n",oct_buff[j]);
            exit(1);
        }
    }
    printf("Octal constant : %s\n", oct_buff);
}