#include<stdio.h>
#include"lexical_header.h"
#include<ctype.h>
#include<stdlib.h>

//skip preprocessor directives
void header(FILE *fp)
{
    char ch;
    while((ch = fgetc(fp)) != '\n');
}

//skip single line and multiline comments
void comments(char temp, FILE *fp)
{
    char ch;
    ch = fgetc(fp);
    if(ch == '/')
    {
        while((ch = fgetc(fp)) != '\n');
        return;
    }
    if(ch == '*')
    {
        while( (ch = fgetc(fp)) != EOF )
        {
            if(ch == '*')
            {
                if(fgetc(fp) == '/')
                {
                    break;
                }
            }
        }
        return ;
    }
    else
    {
        printf("Symbol  :  %c\n", temp);
        fseek(fp, -1, SEEK_CUR);
    }
}

//tokenize keywords and identifiers
void key_identifier(char temp, FILE *fp)
{
    char ch;
    char buffer1[1024];
    int count1 = 0;
    buffer1[count1++] = temp;
    while(isalnum(ch = fgetc(fp)))
    {
        buffer1[count1++] = ch;
    }
    buffer1[count1] = '\0';
    if(iskeyword(buffer1))
    {
        printf("Keyword  :  %s\n", buffer1);
    }
    else
    {
        printf("Identifier  :  %s\n", buffer1);
    }
    fseek(fp, -1, SEEK_CUR);
}

//tokenize numeric constants and find errors
void numeric_constant(char temp, FILE*fp)
{
    char buffer2[1024];
    int count2 = 0;
    buffer2[count2++] = temp;
    char ch;
    int flag = 0;
    int float_flag = 0;
    while(isalnum(ch = fgetc(fp)))
    {
        buffer2[count2] = ch;
        if(isalpha(ch)) 
        {
           if(flag == 0) flag = count2;
        }
        count2++;
    }
    buffer2[count2] = '\0';
    if(ch == '.')
    {
        buffer2[count2++] = ch;
        float_flag = 1;
        while(isalnum(ch = fgetc(fp)) || ch=='.')
        {
            buffer2[count2++] =ch;
        }
        buffer2[count2] = '\0';
        float_constant(buffer2);
        fseek(fp, -1, SEEK_CUR);
        return;

    }
    if(flag>0 && temp!='0')
    {
        printf("error : invalid suffix \"%s\" on integer constant\n", buffer2 + flag);
        exit(1);
    }
    if(temp == '0')
    {
        if(buffer2[1] == 'x' || buffer2[1] == 'X')
        {
            hex_constant(buffer2);
            fseek(fp, -1, SEEK_CUR);
            return;
        }
        else if(buffer2[1] == 'b')
        {
            binary_constant(buffer2);
            fseek(fp, -1, SEEK_CUR);
            return;
        }
        else if(isdigit(buffer2[1]))
        {
            octal_constant(buffer2);
            fseek(fp, -1, SEEK_CUR);
            return;
        }
        else
        {
            printf("Numeric constant : 0\n");
            fseek(fp, -1, SEEK_CUR);
        }
        return;
    }
    if(float_flag == 0)
       {
         printf("Numeric constant : %s\n", buffer2);
         fseek(fp, -1, SEEK_CUR);
        return;
    }
}

//tokenize char literal and find errors
void char_literal(char temp, FILE*fp)
{
    char char_buff[1024];
    int char_count = 0;
    char_buff[char_count++] = temp;
    char ch;
    int char_flag = 0;
    while((ch = fgetc(fp))!=EOF)
    {
        char_buff[char_count++] = ch;
        if(ch == '\'')
        {
           char_flag = 1;
           break;
        }
    }
    if(char_buff[1] == '\'')
    {
        printf("error: empty character constant\n");
        exit(1);
    } 
    char_buff[char_count] = '\0';
    if(char_flag == 0)  
    {
        printf("error : Missing terminating \' character\n");
        exit(1) ;
    }
    if(char_count>3) printf("warning : multi-character character constant\n");
    printf("Char literal : %s\n", char_buff);
}

//tokenize string literal and find errors
void string_literal(char temp, FILE*fp)
{
     int string_flag = 0;
     char string_buff[1024];
     int string_count = 0;
     string_buff[string_count++] = temp;
     char ch;
     while((ch = fgetc(fp))!= EOF)
     {
        string_buff[string_count++] = ch;
        if(ch == '\"') 
        {
            string_flag = 1;
            break;
        }
    }
    string_buff[string_count] = '\0';
    if(string_flag==0) 
    {
        printf("error : Missing terminating \" character\n");
        exit(1) ;
    }
    else
    {
        printf("String literal : %s\n", string_buff);
    }
}

//tokenize symbol
void symbols_print(char ch, char*open, char*close)
{
     int i = 0, j = 0;
     if(ch == '{') open[0]++;
     if(ch == '}') close[0]++;
     if(ch == '(') open[1]++;
     if(ch == ')') close[1]++;
     if(ch == '[') open[2]++;
     if(ch == ']') close[2]++;
    if(open[0] < close[0])
     {
        printf("error: expected identifier or '(' before '}' token\n");
        exit(1);
    }
    if(open[2] < close[2])
    {
        printf("error : error: expected '=', ',', ';', 'asm' or '__attribute__' before ']' token\n");
        exit(1);
    }
    if(open[1] < close[1])
    {
        printf("error : stray ')' in program\n");
        exit(1);
    }
    printf("Symbol   :  %c\n", ch);
}

//tokenize operators
void operators(char temp, FILE *fp)
{
    char ch;
    ch = fgetc(fp);
    char oper_buff[3] = {temp, ch, '\0'};
    if(double_operator(oper_buff))
    {
        printf("Operator  :  %s\n", oper_buff);
    }
    else
    {
        printf("Operator  :  %c\n", temp);
        fseek(fp, -1, SEEK_CUR);
    }
}

//errors by brackets
void brace_error(char*open, char*close)
{
    if(close[0] < open[0]) 
    {
        printf("error: expected declaration or statement at end of input\n'}'\n");
        exit(1);
    }
    if(close[1] < open[1]) 
    {
        printf("error: expected ')' before end of input\n");
        exit(1);
    }
    if(close[2] < open[2]) 
    {
        printf("error: expected ']' before end of input\n");
        exit(1);
    }
}