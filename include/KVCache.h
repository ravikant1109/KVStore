#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <pthread.h>
#include"kvstore.h"

#define MAXKEYSIZE 260
#define MAXVALUESIZE 262150

typedef struct data{
	char key[MAXKEYSIZE];
	char value[MAXVALUESIZE];
	int chance;
	int isReferenced;
	int isfree;
	// /struct data *next;

}cdata;

typedef struct SetLock{
	pthread_mutex_t  work_mutex;
    //pthread_cond_t   work_cond;
    pthread_cond_t   work_cond;
} SetLock;

SetLock *setLock;

struct data **kvc;

int *lastVisited;


int setValue( int len )
{
	return len%sets;
}



void cacheInit(){
	int i,j;
	kvc=(struct data**)malloc(sets*(sizeof(struct data *)));
	for(i=0;i<sets;i++)
	{
    *(kvc+i)=(struct data*)malloc(nway*sizeof(struct data ));
            //You can use this also. Meaning of both is same.
            //arr[i]=(int*)malloc(sizeof(int)*col);
	}
	setLock = (SetLock*) malloc(sets * sizeof(SetLock));
	lastVisited = (int*)malloc(sets * sizeof(int));

	for(i=0;i<sets;i++)
	{
		for(j=0;j<nway;j++)
		{
			lastVisited[i] = -1;
	    	kvc[i][j].chance = 0;
	    	kvc[i][j].isfree = 1;
	    	kvc[i][j].isReferenced = 0;

	    // kvc[i][j].isfree = 1;
		}
		pthread_mutex_init(&(setLock[i].work_mutex), NULL);
	}
}






int isPresent( int set , char key[]){
	int i;
	for( i =0 ; i<nway ; i++)
	{
		if(!strcmp(kvc[set][i].key , key) && kvc[set][i].chance != -1)
		{

			return i;
		}
	}
	return -1;
}


void findSlot(int set , char key[] , char value[])
{
	// printf("(%d)\n", lastVisited[set] );
	int i, j =0;
	// int counter = 10 ;
	for(i= ((lastVisited[set]+1)%nway); j<nway ; j++ )
	{

		i = ( i + j ) % nway;
		if( kvc[set][i].chance == 0 )
		{
			lastVisited[set] = i ;
			// kvc[set][i].chance = 0;
			strcpy (kvc[set][i].key , key);
			strcpy (kvc[set][i].value , value);
			kvc[set][i].isfree = 0;
			// printf("success %s \t %s\n", kvc[set][i].key , kvc[set][i].value );
//			printf("%d\t%dsuccess\n",  );
			return;
		}


	}
	


}






void updateCache(int set ,int pos ,char value[])
{
	memset(kvc[set][pos].value , 0 , sizeof(kvc[set][pos].value));
	strcpy (kvc[set][pos].value , value );
	kvc[set][pos].isReferenced = 0;
	//printf("updated %s \t %s\n", kvc[set][pos].key , kvc[set][pos].value );
}


int putData(char key[] , char value[])
{
	int set = setValue(strlen(key ));
	int pos = isPresent(set , key);
	pthread_mutex_lock(&(setLock[set].work_mutex));
	if(pos != -1)
	{
		updateCache( set , pos , value );
		
	}
	else{

		findSlot(set , key ,  value);
	}

	writeRecordToFile(key , value);

	pthread_mutex_unlock(&(setLock[set].work_mutex));
}


char * getData( char key[]){

	int set = setValue(strlen(key));
	int i;
		pthread_mutex_lock(&(setLock[set].work_mutex));

	for( i =0 ; i<nway ; i++)
	{
		if(!strcmp(kvc[set][i].key , key) && kvc[set][i].chance != -1 && kvc[set][i].isfree ==0)
		{
			// printf("\nCache Hit");
			kvc[set][i].chance = 1;
			kvc[set][i].isReferenced = 1;
			pthread_mutex_unlock(&(setLock[set].work_mutex));
			return kvc[set][i].value;

			
		}
	}
	// printf("\ncache miss");
	char* value = restoreFromFile(key);
	if(value)
	{
		findSlot(set , key , value);
	}
	pthread_mutex_unlock(&(setLock[set].work_mutex));
	return value;
}

int delData(char key[]){

	int set = setValue(strlen(key));
	pthread_mutex_lock(&(setLock[set].work_mutex));

	int i;
	for( i =0 ; i<nway ; i++)
	{
		if(!strcmp(kvc[set][i].key , key))
		{
			kvc[set][i].isfree = 1;
			kvc[set][i].isReferenced = 0;
			pthread_mutex_unlock(&(setLock[set].work_mutex));
			return delRecord(key);
			
		}
	}
	pthread_mutex_unlock(&(setLock[set].work_mutex));
	return delRecord(key);
}

void toXML(char filename[])
{
	
	FILE *outfile = fopen(filename, "w");

	fprintf(outfile,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\t<KVCache>\n");

	for(int i=0; i<sets; i++)
	{
		fprintf(outfile, "\t\t<Set Id=\"%d\">\n", i);
		for(int j=0; j<nway; j++)
		{
			fprintf(outfile, "\t\t\t<CacheEntry isReferenced=\"​");
			if(kvc[i][j].isReferenced == 1)
				fprintf(outfile, "​true\"");
			else
				fprintf(outfile, "​false\"");
			if(kvc[i][j].isfree == 1)
				 fprintf(outfile, "​isValid=\"​false\">\n");
			else
				fprintf(outfile, "​isValid=\"​true\">\n");

			fprintf(outfile, "\t\t\t\t<Key>%s</Key>\n", kvc[i][j].key);
			fprintf(outfile, "\t\t\t\t<Value>%s</Value>\n", kvc[i][j].value);
			fprintf(outfile, "\t\t\t</CacheEntry>\n");
		}
		
		fprintf(outfile, "\t\t</Set>\n");
	}

	fprintf(outfile,"\t</KVCache>");
	fclose(outfile);
}	


