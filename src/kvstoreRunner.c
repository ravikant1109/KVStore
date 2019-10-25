
#include "kvstore.h"
#include <stdio.h>
#include <string.h>

DataNode *head;
// extern int sets;
// extern int nway;

// sets = 256;
// nway = 10;

void main()
{
	// head = createNode("thisisnewkey0", "thisisnewvalue");
	// head->next = createNode("thisisnewkey00", "thisisnewvalue");
	// head->next->next = createNode("thisisnewkey000", "thisisnewvalue");

	// writeRecordToFile("thisisnewkey0", "thisisnewvalue");
	// writeRecordToFile("thisisnewkey0", "thisisnewvalueupdated");
	// writeRecordToFile("thisisnewkey0", "thisisndsfffffffffffffffffffffffewvalueupdated");
	// writeRecordToFile("thisisnewkey22", "thisisnewvalue");
	// writeRecordToFile("thisisnewkey333", "thisisnewvalue");
	// writeRecordToFile("thisisnewkey334", "thisisnewvalue");
	// writeRecordToFile("thisisnewkey7", "thisisnewvalue77676767676");

	// char *node;
	// node = restoreFromFile("thisisnewkey7");

	// if(node)
	// 	printf("%s\n", node);
	// else
	// 	puts("Not Found");

	dumpToFile("ServerData/StoreXML.xml");
		
	
	// delRecord("thisisnewkey333");

	// node = restoreFromFile("thisisnewkey333");

	// if(node)
	// 	printf("%s\n", node);
	// else
	// 	puts("Not Found");
	// FILE *fp = fopen ("ServerData/kvstore_13.dat", "r+b");
	// printFile(fp);
	// fclose(fp);
}