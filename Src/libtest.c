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
	APTR dtBus=NULL, n=NULL, p=NULL ;
	struct Library *DtsParserBase = NULL ;
	char *modelName;
	ULONG address, size;
	UWORD i = 0;
	
	if (!(DtsParserBase = OpenLibrary("dts.library", 0))){
		printf("Cannot open device tree library\n");
		return 0;
	}
	dtBus = DTS_GetNodeByPath("/bus") ;
	if (!dtBus){
		printf("No /bus node in the DTS\n");
		goto exit;
	}
	
	printf("Available compatible devices in DTS:\n");
	while((n=DTS_GetCompatibleNodeInstance("niklas,genericspi",i))){
		address = DTS_GetRegAddress(n, 0); // Get instance 0 of a register address for the node at bus level
		size = DTS_GetRegSize(n, 0); // Get instance 0 of a register size for the node device
		printf("  Node: %s, Address: 0x%04X, Size: %u\n", DTS_GetNodeName(n), address, size);
		if ((p=DTS_GetProperty(n, "model"))){
			if ((modelName=DTS_GetPropertyStringValue(p))){
				printf("  -> Model: %s\n", modelName);
			}
		}
		i += 1;
	}
	
exit:
	CloseLibrary(DtsParserBase);
}	