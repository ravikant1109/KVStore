#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXKEYSIZE 260
#define MAXVALUESIZE 262150
#define MAXBUFSIZE 264000


int GetXmlTagValueSpecific(char *RespBuffer, char *TagValue, char FirstTag[100], char SecondTag[100])
{
	int len=0, pos = 0;
	int PosFirstTag=0, PosSecondTag=0;
	
	//check enter buffer
	len = strlen(RespBuffer);	
	if (len<=0)
	{
		return -1;
	}	
	
	//Get first tag position
	for (pos=0; pos<len; pos++)
	{
		if ( !memcmp(FirstTag, RespBuffer+pos, strlen(FirstTag)) )
		{
			PosFirstTag = pos;
			break;
		}
	}
	
	//Get second tag position
	for (pos=0; pos<len; pos++)
	{
		if ( !memcmp(SecondTag, RespBuffer+pos, strlen(SecondTag)) )
		{
			PosSecondTag = pos;
			break;
		}
	}
	
	
	if ( (PosFirstTag !=0) && (PosSecondTag !=0) )
	{
		if (PosSecondTag-PosFirstTag-strlen(FirstTag))
		{
			//Get tag value
			memcpy( TagValue, RespBuffer+PosFirstTag+strlen(FirstTag), PosSecondTag-PosFirstTag-strlen(FirstTag) );
			if (strlen(TagValue))
			{
				return 1;				
			}
		}
	}
	return -1;
}

int GetXmlTagValue(char *RespBuffer, char *Tag, char *TagValue)
{
	int len=0, pos = 0;
	char FirstTag[100] = {0}; //First Tag
	char SecondTag[100] = {0};//Second Tag
	int PosFirstTag=0, PosSecondTag=0;
	
	//check enter buffer
	len = strlen(RespBuffer);	
	if (len<=0)
	{
		return -1;
	}
	
	//Create first Tag
	memset(FirstTag, 0, sizeof(FirstTag));
	strcpy(FirstTag, "<");
	strcat(FirstTag, Tag);
	strcat(FirstTag, ">");
	
	//Create second tag
	memset(SecondTag, 0, sizeof(SecondTag));
	strcpy(SecondTag, "</");
	strcat(SecondTag, Tag);
	strcat(SecondTag, ">");
	
	
	//Get first tag position
	for (pos=0; pos<len; pos++)
	{
		if ( !memcmp(FirstTag, RespBuffer+pos, strlen(FirstTag)) )
		{
			PosFirstTag = pos;
			break;
		}
	}
	
	//Get second tag position
	for (pos=0; pos<len; pos++)
	{
		if ( !memcmp(SecondTag, RespBuffer+pos, strlen(SecondTag)) )
		{
			PosSecondTag = pos;
			break;
		}
	}
	
	
	if ( (PosFirstTag !=0) && (PosSecondTag !=0) )
	{
		if (PosSecondTag-PosFirstTag-strlen(FirstTag))
		{
			//Get tag value
			memcpy( TagValue, RespBuffer+PosFirstTag+strlen(FirstTag), PosSecondTag-PosFirstTag-strlen(FirstTag) );
			if (strlen(TagValue))
			{
				return 1;				
			}
		}
	}
	return -1;
}

void ParseXML(char msg[], char msgType[], char key[], char value[], char data[]){

	
	
	//Tag name which value you want to access
	char Tag[10] = {0};
	char FirstTag[] = "<KVMessage type=\"";
	char SecondTag[] = "\">";
	
	//Buffer to store tag value
	char TagValue[MAXBUFSIZE]= {0};
	
	//Function to get tag value
	GetXmlTagValueSpecific(data,TagValue, FirstTag, SecondTag);
	
	strcpy(msgType, TagValue);

	memset(Tag, 0, sizeof Tag);
	strcpy(Tag, "Key");
	memset(TagValue, 0, sizeof TagValue);
	GetXmlTagValue(data,Tag, TagValue);
	strcpy(key, TagValue);
	// printf("%s : %ld\n",Tag, strlen(TagValue));

	memset(Tag, 0, sizeof Tag);
	strcpy(Tag, "Value");
	memset(TagValue, 0, sizeof TagValue);
	GetXmlTagValue(data,Tag, TagValue);
	strcpy(value, TagValue);
	// printf("%s : %s\n",Tag, TagValue);

	memset(Tag, 0, sizeof Tag);
	strcpy(Tag, "Message");
	memset(TagValue, 0, sizeof TagValue);
	GetXmlTagValue(data,Tag, TagValue);
	strcpy(msg, TagValue);
	// printf("%s : %s\n",Tag, TagValue);
}

char *GenerateXML(char key[], char value[], char msg[], char msgType[]){

	char *response = (char *)malloc(MAXBUFSIZE * sizeof(char));
	strcat(response, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
	strcat(response, "<KVMessage type=\"");
	strcat(response, msgType);
	strcat(response, "\">");

	if(strcmp(key, "") && strcmp(value, ""))
	{
		strcat(response,"<Key>");
        strcat(response,key);
        strcat(response,"</Key>");

        strcat(response,"<Value>");
        strcat(response,value);
        strcat(response,"</Value>");
	}
	else if(strcmp(key, ""))
	{
		strcat(response,"<Key>");
        strcat(response,key);
        strcat(response,"</Key>");
	}
	else
	{
		strcat(response,"<Message>");
        strcat(response,msg);
        strcat(response,"</Message>");
    }
    strcat(response,"</KVMessage>");
    
	return response;
}