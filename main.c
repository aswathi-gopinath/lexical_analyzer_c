#include<stdio.h>
#include"lexical_header.h"
#include<ctype.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{
   if(argc < 2)
   {
      printf("Invalid arguments\n");
      return 0;
   }
   FILE *fp;
   fp = fopen(argv[1], "r");
   if(fp == NULL)
   {
      perror("ERROR : ");
      return -1;
   }
   char ch;
   char open[3] = {0};
   char close[3] = {0};
   while( (ch = fgetc(fp)) != EOF )
   {
     if(ch == '#')
     {
        header(fp);
        continue;
     } 
     if(ch == '/' )
     {
        comments(ch,fp);
        continue;
     }
     if(isalpha(ch))
     {
        key_identifier(ch,fp);
        continue;
     }
     if(isdigit(ch))
     {
        numeric_constant(ch,fp);
        continue;
     }
     if(ch == '\'')
     {
        char_literal(ch, fp);
        continue;
     }
     if(ch == '\"')
     {
        string_literal(ch, fp);
        continue;
     }
     if(issymbols(ch))
     {
      symbols_print(ch, open, close);
      continue;
    }
     if(single_operator(ch))
     {
        operators(ch, fp);
        continue;
     }
   }
   brace_error(open, close);
   fclose(fp);
   return 0;
}