#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

typedef struct Data
{
	char key[260];
	char value[256*1024];
	struct Data *next;
} DataNode;

DataNode *createNode(char key[], char value[]){
    DataNode *temp; // declare a node
    temp = (struct Data*)malloc(sizeof(struct Data)); // allocate memory using malloc()
    temp->next = NULL;// make next point to NULL
    strcpy(temp->key, key);
    strcpy(temp->value, value);
    return temp;//return the new node
}

void getFileName(char key[], char filename[])
{
	char len[10];
	snprintf(len, 10, "%ld", (strlen(key)%sets));
    // len = len % sets
	strcat(filename, len);
	strcat(filename, ".csv");
}

void printFile(FILE *fp)
{
	DataNode input;
	while(fread(&input, sizeof(struct Data), 1, fp))
    {
    	printf("%s - %s\n", input.key, input.value);
    	
    }
}

int delRecord(char key[])
{
	FILE *infile;
	FILE *outfile;

	char infileName[50] = "ServerData/kvstore_setid_";
	char outfileName[50] = "ServerData/temp_";

	getFileName(key, infileName);
	getFileName(key, outfileName);

	infile = fopen (infileName, "r+"); 
    if (infile == NULL) 
    { 
        //fprintf(stderr, "\nError opend file\n"); 
        return -1; 
    }     

    outfile = fopen (outfileName, "w+"); 
    if (outfile == NULL) 
    { 
        //fprintf(stderr, "\nError opend file\n"); 
        return -1; 
    }
    

    DataNode myrecord;
    int found = 0;
 //    while (fread(&myrecord,sizeof(struct Data),1,infile)) {
		// if (!strcmp (key, myrecord.key)) {
		// 	// printf("A record with requested key found and deleted.\n\n");
		// 	found=1;
		// } else {
		// 	fwrite(&myrecord, sizeof(struct Data), 1, outfile);
		// }
	// }
    char buf[256*1025];
    while(fgets(buf, 256*1025, infile))
    {
        char *keyTemp = (char *)strtok(buf, ",");
        char *valueTemp = (char *)strtok(NULL, "\n");

        if (!strcmp (key, keyTemp)) {
            // printf("A record with requested key found and deleted.\n\n");
            found=1;
        } else {
            // fwrite(&myrecord, sizeof(struct Data), 1, outfile);
            fprintf(outfile,"%s,%s\n",keyTemp,valueTemp);
        }
    }

	fclose(infile);
	fclose(outfile);

	remove(infileName);
	rename(outfileName, infileName);

	if (! found) {
		// printf("No record(s) found with the requested key for Delete: %s\n\n", key);
		return -1;
	}
	return 1;

}


int writeRecordToFile(char key[], char value[])
{
	FILE *outfile; 	
    // DataNode input;
    // long num_rec = 1;

    char filename[200] = "ServerData/kvstore_setid_";		
	getFileName(key, filename);

	delRecord(key);

	outfile = fopen (filename, "a+");
    if (outfile == NULL) 
    { 
        fprintf(stderr, "\nError opend file\n"); 
        return -1; 
    }

	// strcpy(input.key, key);
	// strcpy(input.value, value);
	// fwrite (&input, sizeof(struct Data), 1, outfile);
    fprintf(outfile,"%s,%s\n",key,value);

	fclose(outfile);

	//puts("Success in writing");
	return 1;
}

char *restoreFromFile(char key[])
{
	FILE *infile; 
    char filename[200] = "ServerData/kvstore_setid_";
	getFileName(key, filename);

	infile = fopen (filename, "r+"); 
    if (infile == NULL) 
    { 
        //fprintf(stderr, "\nError opend file\n"); 
        return NULL; 
    }

    static DataNode input;
    char buf[256*1025];
    while(fgets(buf, 256*1025, infile))
    {
        char *keyTemp = (char *)strtok(buf, ",");
        char *valueTemp = (char *)strtok(NULL, "\n");

        if (!strcmp (key, keyTemp)) {
            // printf("A record with requested key found and deleted.\n\n");
            return valueTemp;
        } 
    }
    fclose(infile);
    return NULL;	
}






void dumpToFile(char filename[])
{
    
    FILE *infile;
    FILE *outfile = fopen(filename, "w");
    if (outfile == NULL) 
    { 
        //fprintf(stderr, "\nError opend file\n"); 
        return; 
    }
    fprintf(outfile,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\t<KVStore>\n");
    for(int i=0; i<sets; i++)
    {
        char infileName[50];
        sprintf(infileName, "ServerData/kvstore_setid_%d.csv", i);
        infile = fopen(infileName, "r+");
        if (infile == NULL) 
        { 
            //fprintf(stderr, "\nError opend file\n"); 
            continue; 
        }

        char buf[256*1025];
        while(fgets(buf, 256*1025, infile))
        {
            char *keyTemp = (char *)strtok(buf, ",");
            char *valueTemp = (char *)strtok(NULL, "\n");

            
            
            fprintf(outfile,"\t\t<KVPair>\n\t\t\t<Key>​%s</Key>\n\t\t\t<Value>​%s</Value>\n\t\t</KVPair>\n",keyTemp,valueTemp);
            
        }

        fclose(infile);
    }

    fprintf(outfile,"\t</KVStore>");
    fclose(outfile);
}
