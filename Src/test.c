#include "devicetreeconfig.h"
#include <stdio.h>
#include <string.h>
#include <proto/exec.h>
#include <exec/exec.h>
#include <dos/dos.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/cardres.h>
#include <resources/card.h>
#include <clib/expansion_protos.h>
#include <libraries/configvars.h>

#define DOSBase deviceTree.dos

struct devicetreeConfig deviceTree;

void doIndent(UWORD indent)
{
	UWORD i;
	for (i=0;i<indent;i++){
		putc(' ', stdout);
	}
}
void printNode(struct devicetreeNode *n1, UWORD indent)
{
	struct devicetreeObject *o;
	struct devicetreeCommand *cmd;
	struct devicetreeProperty *prop;
	struct devicetreeValue *val;
	struct TagItem *ulArray, tiReg[10], tiRegRelative[10];
	UBYTE *bArray;
	UWORD i=0;
	struct devicetreeReference *ref;
	ULONG size_cell, address_cell;
	
	memset(tiReg, 0, sizeof(struct TagItem)*10);
	memset(tiRegRelative, 0, sizeof(struct TagItem)*10);
	
	getSizeAddressCells(&deviceTree, n1, &size_cell, &address_cell);
	getRegActual(&deviceTree, n1, tiReg, 10);
	getRegRelative(&deviceTree, n1, tiRegRelative, 10);
	
	doIndent(indent);printf("[Node label: \"%s\", name: \"%s\", address: \"%s\" BusAddress: 0x%04X, RegSize: %u]\n", n1->label, n1->name, n1->unitAddress,
																													tiReg[address_cell-1].ti_Data,
																													tiRegRelative[address_cell+(size_cell-1)].ti_Data);
	for(o = n1->firstObject; o; o=o->next){
		if (o->type == DT_OBJECT_PROPERTY){
			prop = (struct devicetreeProperty*)o;
			doIndent(indent);printf("-Property label: \"%s\", name: \"%s\", values: ", prop->label, prop->name);
			if (prop->values){
				for(val=prop->values; val; val=val->next){
					switch(val->type){
						case DT_VALUE_ULONG_ARRAY:
							printf("(tag array) ");
							ulArray = (struct TagItem *)val->value;
							for(i=0; i<val->size; i++){
								if (ulArray[i].ti_Tag == DT_ENCODED_VALUE_REFERENCE){
									ref = (struct devicetreeReference*)ulArray[i].ti_Data;
									printf("<phandle:%d node:0x%p> ", ref->phandleRef, ref->node);
								}else if (ulArray[i].ti_Tag == DT_ENCODED_VALUE_U32){	
									printf("<u32:0x%04X> ", ulArray[i].ti_Data);
								}
							}
							break;
						case DT_VALUE_STRING:
							printf("(string) %s ", (char*)val->value);
							break;
						case DT_VALUE_BYTE_STRING:
							printf("(byte string) ") ;
							bArray = (UBYTE*)val->value;
							for(i=0; i<val->size; i++){
								printf("%02X ", bArray[i]);
							}
							break;
						case DT_VALUE_LOGIC:
							printf("(logic) %s ", (char*)val->value);
							break;
						default:
							printf("(unknown) ");
					}
				}
				printf("\n");
			}else{
				printf("(no value)\n");
			}
		}else if (o->type == DT_OBJECT_COMMAND){
			cmd = (struct devicetreeCommand*)o;
			doIndent(indent);printf("-Command %s, value %s\n", cmd->name, cmd->value);
		}
	}
}

void printCardResource(void)
{
	struct Library *CardResource;
	struct CardMemoryMap *cmm;
	
	CardResource = (struct Library *)OpenResource(CARDRESNAME);
	
	cmm = GetCardMap() ;
	printf("Card resources:\nCommon Memory %p\nAttriute Memory %p\nIO Memory %p\n", cmm->cmm_CommonMemory, cmm->cmm_AttributeMemory, cmm->cmm_IOMemory);
	
}

void printExpansionDevices(void)
{
	struct Library *ExpansionBase = NULL;
	struct ConfigDev *cd=NULL;
	
	ExpansionBase = (struct Library*)OpenLibrary("expansion.library", 0L);
	if (!ExpansionBase){
		printf("Cannot open the expansion.library\n");
		return ;
	}
	
	for (cd = FindConfigDev(cd,-1,-1); cd; cd = FindConfigDev(cd,-1,-1)){
		printf("Autoconfig board: Manufacturer ID %d, Product ID %d, Address %p, Size %d kb %s\n", 
			cd->cd_Rom.er_Manufacturer, 
			cd->cd_Rom.er_Product, 
			cd->cd_BoardAddr, 
			cd->cd_BoardSize / 1024,
			(cd->cd_Rom.er_Type & ERTF_MEMLIST)?"Type is memory":"");
	}
	
	CloseLibrary(ExpansionBase);
}

struct walkParams{
	UWORD count ;
};

UWORD treeCallback(struct devicetreeConfig *config, struct devicetreeNode *node, void *context)
{
	UWORD depth = 0;
	struct walkParams *p = context ;
	struct devicetreeNode *n;
	
	p->count++;
	for (depth=0, n=node; n; n=n->parent, depth++);
	printNode(node, depth);
	
	return DT_RETURN_NOERROR;
}

int main(int argc, char **argv)
{
	BPTR f;
	UWORD ret;
	struct devicetreeNode *n;
	struct walkParams params;
	
	if (argc < 2){
		printf("Usage: <filename> [path]\n");
		goto quit;
	}
	dtInitialise(&deviceTree);
	
	f = Open(argv[1], MODE_OLDFILE);
	if (!f){
		printf("Cannot open file %s\n", argv[1]);
		goto quit;
	}
	
	if ((ret=dtOpenFile(&deviceTree, f, 0)) != DT_RETURN_NOERROR){
		printf("dtOpenFile failed with error 0x%04X\n", ret);
		goto quit;
	}
	
	if ((ret=dtParseConfig(&deviceTree, NULL)) != DT_RETURN_NOERROR){
		printf("dtParseConfig failed with error 0x%04X, at line %d\n", ret, lastLineNumber(&deviceTree));
		goto quit;
	}
	
	params.count = 0;
	walkAllNodes(&deviceTree, deviceTree.topNode.child, treeCallback, &params);
	printf("There are %d nodes in the DTS\n", params.count);
	
	printCardResource();
	printExpansionDevices();
	
	if (argc >= 3){
		if (!(n=getNode(&deviceTree, argv[2]))){
			printf("Cannot find %s node path\n", argv[2]);
		}else{
			printf("%s node is:\n", argv[2]);
			printNode(n, 2);
		}
		
		printf("All child nodes of %s:\n", argv[2]);
		n=NULL;
		while ((n = iterateChildNodes(&deviceTree, n, argv[2], NULL))){
			printf("  Child node: %s@%s\n", n->name, n->unitAddress);
		}
	}	
quit:
	if (f){Close(f);};
	dtClose(&deviceTree);
	return 0;
}