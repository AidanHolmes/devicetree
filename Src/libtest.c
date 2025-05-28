#include "/Include/C/proto/devicetree.h"
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
	struct Library *DeviceTreeBase = NULL ;
	char *modelName, *nodeAddress;
	ULONG *address;
	
	DeviceTreeBase = OpenLibrary("devicetree.library", 0);
	
	if (!DeviceTreeBase){
		printf("Cannot open device tree library\n");
		return 0;
	}
	dtBus = DeviceTree_GetNode("/bus") ;
	if (!dtBus){
		printf("No /bus node in the DTS\n");
		goto exit;
	}
	
	if ((n = DeviceTree_GetFirstChildNode("/bus", "clockport-spi"))){
		printf("Found a compatible clockport device - ");
		if ((p=DeviceTree_GetProperty(n, "model"))){
			if ((modelName = DeviceTree_GetPropertyStringValue(p))){
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
		printf("    [Name: %s]", DeviceTree_GetNodeName(thisNode));
		if ((nodeAddress = DeviceTree_GetNodeAddress(thisNode)) && nodeAddress[0] != '\0'){
			printf("@[Address: %s]\n", nodeAddress);
		}

		if ((p=DeviceTree_GetProperty(n, "reg"))){
			for(v=DeviceTree_GetFirstPropertyValue(p); v; v=DeviceTree_GetNextPropertyValue(v)){
				if (DeviceTree_GetValueType(v) == DT_VALUE_ULONG_ARRAY && DeviceTree_GetValueSize(v) >= 1){
					address = (ULONG*)DeviceTree_GetValue(v);
					printf("    [Reg value specifies device at address 0x%04X]\n", address[0]);
				}
			}
		}
	}else{
		printf("Couldn't find config node needed to get address details\n");
	}
	
exit:
	CloseLibrary(DeviceTreeBase);
}	