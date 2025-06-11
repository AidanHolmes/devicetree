#include "/Include/C/proto/dts.h"
#include <exec/types.h>
#include <exec/exec.h>
#include <proto/exec.h>
#include <stdio.h>
#include <string.h>

#define DT_VALUE_UNDEFINED		0
#define DT_VALUE_ULONG_ARRAY	1
#define DT_VALUE_STRING			2
#define DT_VALUE_BYTE_STRING	3
#define DT_VALUE_LOGIC			4

int main (int argc, char **argv)
{
	APTR dtBus=NULL, n=NULL, p=NULL, thisNode=NULL, v=NULL ;
	struct Library *DtsParserBase = NULL ;
	char *modelName, *nodeAddress;
	ULONG *address;
	
	DtsParserBase = OpenLibrary("dts.library", 0);
	
	if (!DtsParserBase){
		printf("Cannot open device tree library\n");
		return 0;
	}
	dtBus = DTS_GetNodeByPath("/bus") ;
	if (!dtBus){
		printf("No /bus node in the DTS\n");
		goto exit;
	}
	
	if ((n = DTS_GetFirstChildNode("/bus", "clockport-spi"))){
		printf("Found a compatible clockport device - ");
		if ((p=DTS_GetProperty(n, "model"))){
			if ((modelName = DTS_GetPropertyStringValue(p))){
				printf("%s\n", modelName);
				if (strcmp(modelName, "niklas,spider-v1") == 0){
					// This is the model and compatability we were looking for
					thisNode = n;
				}
			}else{
				printf("(Error obtaining model name)\n");
			}
		}else{
			printf("(No model info)\n");
		}
	}else{
		printf("Couldn't find any child nodes in the /bus node\n");
		goto exit;
	}
	
	printf("\n");
	
	if (thisNode){
		printf("Found the config node required:\n");
		printf("    [Name: %s]", DTS_GetNodeName(thisNode));
		if ((nodeAddress = DTS_GetNodeAddress(thisNode)) && nodeAddress[0] != '\0'){
			printf("@[Address: %s]\n", nodeAddress);
		}

		if ((p=DTS_GetProperty(n, "reg"))){
			for(v=DTS_GetFirstPropertyValue(p); v; v=DTS_GetNextPropertyValue(v)){
				if (DTS_GetValueType(v) == DT_VALUE_ULONG_ARRAY && DTS_GetValueSize(v) >= 1){
					address = (ULONG*)DTS_GetValue(v);
					printf("    [Reg value specifies device at address 0x%04X]\n", address[0]);
				}
			}
		}
	}else{
		printf("Couldn't find config node needed to get address details\n");
	}
	
exit:
	CloseLibrary(DtsParserBase);
}	