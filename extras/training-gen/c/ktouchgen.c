/***************************************************************************
                          main.c  -  description
                             -------------------
    begin                : Wed Mar 21 21:39:53 EST 2001
    copyright            : (C) 2001 by Haavard Froeiland 2264228
    email                : havard@student.unsw.edu.au

 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

#define max 50

typedef struct WordNode *WordList;

struct WordNode
{
  char *word;
  struct WordNode *next;
};

void *addWord(WordList first, char w[])
{
  struct WordNode *ptr;

  ptr=malloc(sizeof(struct WordNode));
  ptr->word=strdup(w);
  ptr->next=first;
  return ptr;
}

void printWordList(WordList l)
{
  struct WordNode *ptr=l;
  /* loop throug the linked list */
  while(ptr!=NULL)
  {
    printf("%s\n",ptr->word);
    ptr=ptr->next;
  }
}

void printLevel(FILE *f,char *l[])
{
  int pos=0;
  int line=0;
  int arrayMax=0;
  int arrayPos=0;

  while(l[arrayMax]!=NULL)
  {
    arrayMax++;
  }

  if (arrayMax == 0) return;
  /* loop throug the linked list */
  while(line<30)
  {
    arrayPos=((float)rand()/RAND_MAX)*arrayMax;
    pos=pos+strlen(l[arrayPos]);
    fprintf(f,"%s",l[arrayPos]);
    if(pos>40)
    {
      fprintf(f,"\n");
      pos=0;
      line++;
    }
    else
    {
      pos++;
      fprintf(f," ");
    }
  }
}


void creatLevelList(WordList l,char *levelList[],char s_or[], char s_and[])
{
  struct WordNode *ptr=l;
  int i;
  int j;
  int pos=0;
  bool or_failed;
  bool and_failed;

  /* loop throug the linked list */
  while(ptr!=NULL)
  {
    /* loop through the word */
    /* printf("%s\n",ptr->word); */
    i=strlen(ptr->word)-1;
    for(;i>=0;i--)
    {

      or_failed=true;
      j=strlen(s_or)-1;
      for(;j>=0;j--)
      {
        if(ptr->word[i]==s_or[j])
        {
          or_failed=false;
          j=-1;
        }
      }

      and_failed=true;
      j=strlen(s_and)-1;
      for(;j>=0;j--)
      {
        if(ptr->word[i]==s_and[j])
        {
          and_failed=false;
          j=-1;
        }
      }

      if(or_failed==true)
      {
        i=-1;
      }
    }
    if(or_failed==false && and_failed==false)
    {
      levelList[pos]=ptr->word;
      pos++;
      /* printf("%d\n",pos); */
    }

    ptr=ptr->next;
  }
  levelList[pos]=NULL;
}


int main(int argc, char *argv[])
{
  time_t lt;
  FILE *file;
  WordList list=NULL;
  char *levelList[100000];
  char word[max];
  char test[100];
  int i=0;
  char *s[50];

  if(argc<4)
  {
    printf("\nUsage: ktouchgen ConfigFile WordFile TrainingFile\n");
    exit(0);
  }

  /**
   * Read in the configFile
   *
   */
  if((file = fopen(argv[1],"r"))==NULL)
  {
    printf("can't open config_file:%s for reading",argv[2]);
  }
  i=0;
  while(!feof(file))
  {
    fscanf(file,"%s",word);

    s[i]=strdup(word);
    printf("%s\n",s[i]);
    i++;
  }
  s[i]=NULL;
  fclose(file);



  /**
   * Read in the wordFile and add each word to the list
   *
   */
  printf("Starting reading words");
  if((file = fopen(argv[2],"r"))==NULL)
  {
    printf("can't open word_file:%s for reading",argv[2]);
  }
  while(!feof(file))
  {
    fscanf(file,"%s",word);
    list=addWord(list, word);
  }
  fclose(file);


  if((file = fopen(argv[3],"w"))==NULL)
  {
    printf("Error when writing to file:%s",argv[3]);
  }

  lt = time(NULL);
  fprintf(file,"#############################################################\n");
  fprintf(file,"# Rrainingfile genereated %s",ctime(&lt));
  fprintf(file,"# Program written by Håvard Frøiland\n");
  fprintf(file,"#############################################################\n\n");

  strcpy(test,"");

  i=0;
  while(s[i]!=NULL)
  {
    if (strlen(test) + strlen(s[i]) + 1 > sizeof(test))
    {
       printf("Buffer overflow.\n");
       exit(1);
    }
    strcat(test,s[i]);
    fprintf(file,"# Level %d\n",i+1);
    fprintf(file,"%s\n", s[i]);
    creatLevelList(list,levelList,test,s[i]);
    printLevel(file,levelList);
    fprintf(file,"\n");
    i++;
  }
  fclose(file);

  return EXIT_SUCCESS;
}

