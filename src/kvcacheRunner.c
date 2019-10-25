#include<stdio.h>
#include<stdlib.h>
#include"KVCache.h"
int main()
{
// int i,j;
// kvc=(struct data**)malloc(sets*(sizeof(struct data *)));
// for(i=0;i<sets;i++)
// {
//     *(kvc+i)=(struct data*)malloc(10*sizeof(struct data ));
//             //You can use this also. Meaning of both is same.
//             //arr[i]=(int*)malloc(sizeof(int)*col);
// }
// for(i=0;i<sets;i++)
// for(j=0;j<nway;j++)
// {
// 	lastVisited[i] = -1;
//     kvc[i][j].chance = 0;
//     kvc[i][j].isfree = 1;
// kvc[i][j].isReferenced = 0;
//     // kvc[i][j].isfree = 1;
// }

cacheInit();

toXML("ServerData/CacheXML.xml");

}
// for(i=9; i < 12 ; i++)
// {
// 	for(j=0;j<nway; j++){
// 		if(kvc[i][j].isfree == 0)
// 			printf( "%s\t" , kvc[i][j].key );
// 	}
// 	printf("\n");
// }


// if(delData("thisiske22"))
// 	printf("(del_success)\n" );
// else
// 	printf("del_failed\n");
// for(i=9; i < 12 ; i++)
// {
// 	for(j=0;j<nway; j++){
// 		if(kvc[i][j].isfree == 0)
// 		printf( "%s\t" , kvc[i][j].key );
// 	}
// 	printf("\n");
// }

// char *newValue = getData("thisiskey6");

// if(newValue)
// 	puts(newValue);
// else
// 	puts("Not Found");

// for(i=9; i < 12 ; i++)
// {
// 	for(j=0;j<nway; j++){
// 		if(kvc[i][j].isfree == 0)
// 		printf( "%s\t" , kvc[i][j].key );
// 	}
// 	printf("\n");
// }


// putData("thisiskey6" , "thisiskeyvalue444444");




// putData("thisiske22" , "thisiskeyvalue1111111111111111111");
// newValue = getData("thisiskey6");


// if(newValue)
	// puts(newValue);
// else
	// puts("Not Found");


//}
