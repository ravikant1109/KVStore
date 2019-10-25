#include "xmlParser.h"

void main()
{
	char key[100], value[100], msg[100], msgType[100];
	char *resp = GenerateXML("", "", "thisismsg", "thisismsgtype");
	printf("%s\n", resp);

	ParseXML(msg, msgType, key, value, resp);

	printf("\nKey: %s\n", key);
	printf("Value: %s\n", value);
	printf("Msg: %s\n", msg);
	printf("MsgType: %s\n", msgType);
}