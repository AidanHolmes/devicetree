#include "devicetreeconfig.h"
#include <stdio.h>
#include <string.h>
#include <proto/exec.h>
#include <exec/exec.h>
#include <dos/dos.h>
#include <proto/exec.h>
#include <proto/dos.h>

#define DOSBase deviceTree.dos

void doIndent(UWORD indent)
{
	UWORD i;
	for (i=0;i<indent;i++){
		putc(' ', stdout);
		//printf(" ");
	}
}
void printNode(struct devicetreeNode *n1, UWORD indent)
{
	struct devicetreeObject *o;
	struct devicetreeCommand *cmd;
	struct devicetreeProperty *prop;
	struct devicetreeValue *val;
	struct TagItem *ulArray;
	UBYTE *bArray;
	UWORD i=0;
	struct devicetreeReference *ref;
	
	doIndent(indent);printf("[Node name: \"%s\", label: \"%s\", address: \"%s\"]\n", n1->name, n1->label, n1->unitAddress);
	//if(n1->firstObject){
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
	//}
}

void printDTS(struct devicetreeConfig *config)
{
	struct devicetreeNode *n1;
	
	UWORD level = 0;
	
		
	n1=&config->topNode;
	
	do{
		printNode(n1, level);
		
		if (n1->child){
			n1 = n1->child;
			level += 1;
		}else{
			while(!n1->next && n1 != &config->topNode){
				level -= 1;
				n1 = n1->parent;
			}

			n1 = n1->next;
		}
	}while(n1);
}

int main(int argc, char **argv)
{
	BPTR f;
	UWORD ret;
	struct devicetreeConfig deviceTree;
	struct devicetreeNode *n;
	
	if (argc < 2){
		printf("Usage: <filename>\n");
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
	
	printDTS(&deviceTree);
	
	if (!(n=getNode(&deviceTree, "/"))){
		printf("Cannot find root node\n");
	}else{
		printf("Root node is:\n");
		printNode(n, 2);
	}
	
	if (!(n=getNode(&deviceTree, "/bus"))){
		printf("Cannot find bus node\n");
	}else{
		printf("Bus node is:\n");
		printNode(n, 2);
	}
	
	printf("All child nodes of bus:\n");
	n=NULL;
	while ((n = iterateChildNodes(&deviceTree, n, "/bus", NULL))){
		printf("  Child node: %s@%s\n", n->name, n->unitAddress);
	}
	
quit:
	if (f){Close(f);};
	dtClose(&deviceTree);
	return 0;
}