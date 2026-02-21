#include<stdio.h>
#include<string.h>
#include "lexical_header.h"

int iskeyword(char *buffer)
{
    static char* keywords[] ={"auto","break","case","char","const","continue","default","do","double","else","enum",
                             "extern","float","for","goto","if","int","long","register","return","short","signed",
                             "sizeof","static","struct","switch","typedef","union","unsigned","void","volatile",
                             "while"};
    
    int i;
    for(i = 0; i < 32; i++)
    {
        if(strcmp(keywords[i], buffer) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int issymbols(char type)
{
    static char symbol[] = "{}()[];:";
    int i;
    for(i = 0; symbol[i] != '\0'; i++ )
    {
        if(symbol[i] == type)
        {
            return 1;
        }
    }
    return 0;
    
}

int single_operator(char op)
{
    char single_operators[] = "+-*/%=<>&|^!~,";
    int i;
    for(i = 0; single_operators[i] != '\0'; i++ )
    {
        if(single_operators[i] == op)
        {
            return 1;
        }
    }
    return 0;

}
int double_operator(char *buffer)
{
    static char *double_operator[] =   {"++", "--", "+=", "-=", "*=", "/=", "%=", "==", 
                                 "!=", "<=", ">=", "&&", "||", "&=", "|=", "^="};
    int size = sizeof(double_operator) / sizeof(double_operator[0]);
    int i;
    for(i = 0; i<size ; i++)
    {
        if(strcmp(double_operator[i], buffer) == 0)
        {
            return 1;
        }
    }
    return 0;
}
