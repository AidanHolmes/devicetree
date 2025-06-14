#include "devtreelib.h"
#include "devicetreeconfig.h"
#include <dos/dos.h>
#include <proto/dos.h>

struct LibDevBase* __saveds __asm libdev_library_open(register __a6 struct LibDevBase *base)
{
	return base;
}

struct LibDevBase* __saveds __asm libdev_initalise(register __a6 struct LibDevBase *base)
{
	BPTR dtsFile;
	struct Library *DOSBase = NULL;
	struct devicetreeConfig *config = NULL ;
	
	base->libData = config = (struct devicetreeConfig*)AllocVec(sizeof(struct devicetreeConfig), MEMF_ANY | MEMF_CLEAR);
	if (!base->libData){
		return NULL;
	}
	
	if (dtInitialise(config) != DT_RETURN_NOERROR){
		base = NULL ;
		goto end;
	}
	
	DOSBase = config->dos;
	
	if (!(dtsFile = Open("ENVARC:amiga.dts", MODE_OLDFILE))){
		base = NULL ;
		goto end;
	}

	if (dtOpenFile(config, dtsFile, 0) != DT_RETURN_NOERROR){
		base = NULL ;
		goto end;
	}
	
	if (dtParseConfig(config, NULL) != DT_RETURN_NOERROR){
		base = NULL ;
		goto end;
	}
	
end:
	if (!base){
		dtClose(config);
		FreeVec(base->libData);
		base->libData = NULL;
	}
	Close(dtsFile);
	
	return base;
}

void __saveds __asm libdev_cleanup(register __a6 struct LibDevBase *base)
{
	if (base->libData){
		dtClose((struct devicetreeConfig*)base->libData);
		FreeVec(base->libData);
		base->libData = NULL;
	}
}

void* __saveds __asm GetNodeByPath(register __a0 char *path, register __a6 struct LibDevBase *base)
{
	return getNode((struct devicetreeConfig*)base->libData, path);
}

char* __saveds __asm GetNodePath(register __a0 APTR node, register __a6 struct LibDevBase *base)
{
	return getNodePath((struct devicetreeConfig*)base->libData, (struct devicetreeNode*)node);
}

char* __saveds __asm GetNodeName(register __a0 void *node, register __a6 struct LibDevBase *base)
{
	struct devicetreeNode *n = (struct devicetreeNode*)node ;
	
	if (!n){
		return NULL;
	}
	
	return n->name;
}

char* __saveds __asm GetNodeAddress(register __a0 APTR node, register __a6 struct LibDevBase *base)
{
	struct devicetreeNode *n = (struct devicetreeNode*)node ;
	
	if (!n){
		return NULL;
	}
	
	return n->unitAddress;
}

APTR __saveds __asm GetFirstChildNode(register __a0 char *path, register __a1 char *compatible, register __a6 struct LibDevBase *base)
{
	return iterateChildNodes((struct devicetreeConfig*)base->libData, NULL, path, compatible);
}

APTR __saveds __asm GetNextSiblingNode(register __a0 APTR node, register __a1 char *compatible, register __a6 struct LibDevBase *base)
{
	return iterateChildNodes((struct devicetreeConfig*)base->libData, (struct devicetreeNode *)node, NULL, compatible);
}

APTR __saveds __asm GetProperty(register __a0 APTR node, register __a1 char *name, register __a6 struct LibDevBase *base)
{
	return getProperty((struct devicetreeConfig*)base->libData, (struct devicetreeNode*)node, name);
}

APTR __saveds __asm GetFirstProperty(register __a0 APTR node, register __a6 struct LibDevBase *base)
{
	return iterateProperty((struct devicetreeConfig*)base->libData, (struct devicetreeNode*)node, NULL);
}

APTR __saveds __asm GetNextProperty(register __a0 APTR lastProperty, register __a6 struct LibDevBase *base)
{
	return iterateProperty((struct devicetreeConfig*)base->libData, NULL, (struct devicetreeProperty*)lastProperty);
}

char* __saveds __asm GetPropertyName(register __a0 APTR property, register __a6 struct LibDevBase *base)
{
	if (!property){
		return NULL;
	}
	return ((struct devicetreeProperty*)property)->name ;
}

char* __saveds __asm GetPropertyStringValue(register __a0 APTR property, register __a6 struct LibDevBase *base)
{
	struct devicetreeValue *v=NULL;
	if (!property){
		return NULL;
	}

	for (v = ((struct devicetreeProperty*)property)->values; v; v=v->next){
		if (v->type == DT_VALUE_STRING){
			return (char*)v->value;
		}
	}
	return NULL;
}

APTR __saveds __asm GetFirstPropertyValue(register __a0 APTR property, register __a6 struct LibDevBase *base)
{
	if (!property){
		return NULL;
	}
	
	return ((struct devicetreeProperty*)property)->values;
}

APTR __saveds __asm GetNextPropertyValue(register __a0 APTR value, register __a6 struct LibDevBase *base)
{
	if (!value){
		return NULL;
	}
	
	return ((struct devicetreeValue*)value)->next;
}

ULONG __saveds __asm GetValueSize(register __a0 APTR value, register __a6 struct LibDevBase *base)
{
	if (!value){
		return NULL;
	}
	
	return ((struct devicetreeValue*)value)->size;
}

UWORD __saveds __asm GetValueType(register __a0 APTR value, register __a6 struct LibDevBase *base)
{
	if (!value){
		return NULL;
	}
	
	return ((struct devicetreeValue*)value)->type;
}

APTR __saveds __asm GetValue(register __a0 APTR value, register __a6 struct LibDevBase *base)
{
	if (!value){
		return NULL;
	}
	
	return ((struct devicetreeValue*)value)->value;
}

APTR __saveds __asm GetNodeByAlias(register __a0 char *aliasName, register __a6 struct LibDevBase *base)
{
	return getReferenceNodePropertyByPath((struct devicetreeConfig*)base->libData, "/aliases", aliasName);
}

APTR __saveds __asm GetNodeByChosen(register __a0 char *chosenName, register __a6 struct LibDevBase *base)
{
	return getReferenceNodePropertyByPath((struct devicetreeConfig*)base->libData, "/chosen", chosenName);
}

APTR __saveds __asm GetNodeByLabel(register __a0 char *labelName, register __a6 struct LibDevBase *base)
{
	struct devicetreeNode *dtsnode = NULL, *dtsnew = NULL ;
	
	if ((dtsnode = &((struct devicetreeConfig*)base->libData)->topNode)){
		// Walk all nodes
		while ( (dtsnew=dtsnode->child) || (dtsnew=dtsnode->next) || (dtsnode->parent && (dtsnew = dtsnode->parent->next)) ){
			dtsnode = dtsnew ;
			if (dtStriCmp(dtsnode->label, labelName, DT_MAX_NODE_LABEL)){
				return dtsnode;
			}
		}
	}
	return NULL;
}

struct compatibleNodeInstanceParam{
	char *compatible;
	UWORD instanceCount;
	UWORD instance;
	struct devicetreeNode *found;
};

UWORD callbackCompatibleNodeInstance(struct devicetreeConfig *config, struct devicetreeNode *node, struct compatibleNodeInstanceParam *context)
{
	struct devicetreeProperty *p;
	struct devicetreeValue *v ;
	if (p=getProperty(config, node, "compatible")){
		for (v=p->values; v; v=v->next){
			if (v->type == DT_VALUE_STRING){
				if (dtStriCmp((char*)v->value, context->compatible, DT_MAX_TEMP_STR)){
					if (context->instanceCount == context->instance){
						context->found = node;
						return DT_RETURN_STOP;
					}
					context->instanceCount += 1;
				}
			}
		}
	}
	
	return DT_RETURN_NOERROR;
}

APTR __saveds __asm GetCompatibleNodeInstance(register __a0 char *compatibleStr, register __d1 UWORD instance, register __a6 struct LibDevBase *base)
{
	struct compatibleNodeInstanceParam param ;

	param.instanceCount = 0;
	param.compatible = compatibleStr;
	param.instance = instance ;
	param.found = NULL ;
	walkAllNodes((struct devicetreeConfig*)base->libData, &(((struct devicetreeConfig*)base->libData)->topNode), callbackCompatibleNodeInstance, &param);

	return param.found;
}

struct compatibleNodeOKAYParam{
	char *compatible;
	struct devicetreeNode *found;
};

UWORD callbackCompatibleNodeOKAY(struct devicetreeConfig *config, struct devicetreeNode *node, struct compatibleNodeInstanceParam *context)
{
	struct devicetreeProperty *p;
	struct devicetreeValue *v ;
	BOOL matchingNode = FALSE;
			
	if (p=getProperty(config, node, "compatible")){
		for (v=p->values; v; v=v->next){
			if (v->type == DT_VALUE_STRING){
				if (dtStriCmp((char*)v->value, context->compatible, DT_MAX_TEMP_STR)){
					matchingNode = TRUE ;
				}
			}
		}
	}
	if (matchingNode){
		if (p=getProperty(config, node, "status")){
			for (v=p->values; v; v=v->next){
				if (v->type == DT_VALUE_STRING){
					if (dtStriCmp((char*)v->value, "okay", DT_MAX_TEMP_STR)){
						context->found = node;
						return DT_RETURN_STOP;
					}
				}
			}
		}
	}
	return DT_RETURN_NOERROR;
}

APTR __saveds __asm GetCompatibleNodeOKAY(register __a0 char *compatibleStr, register __a6 struct LibDevBase *base)
{
	struct compatibleNodeOKAYParam param ;

	param.compatible = compatibleStr;
	param.found = NULL ;
	walkAllNodes((struct devicetreeConfig*)base->libData, &(((struct devicetreeConfig*)base->libData)->topNode), callbackCompatibleNodeOKAY, &param);

	return param.found;
}

ULONG __saveds __asm GetRegAddress(register __a0 APTR node, register __d1 UWORD instance, register __a6 struct LibDevBase *base)
{
	struct TagItem tags[5], *pti=NULL;
	UWORD size = 0;
	ULONG ret = 0xFFFFFFFF, size_cell, address_cell;
	
	getSizeAddressCells((struct devicetreeConfig*)base->libData, (struct devicetreeNode*)node, &size_cell, &address_cell);
	if (address_cell == 0){
		return ret;
	}
	
	size = getRegActual((struct devicetreeConfig*)base->libData, (struct devicetreeNode*)node, tags, 5);
	if (((instance+1)*(size_cell+address_cell)) > size){ // no entry for this instance
		return ret;
	}
	
	if (size > 4 && ((instance+1)*(size_cell+address_cell)) > 4){
		if (!(pti = AllocVec(sizeof(struct TagItem) * (size+1), MEMF_ANY | MEMF_CLEAR))){
			return ret;
		}
		getRegActual((struct devicetreeConfig*)base->libData, (struct devicetreeNode*)node, pti, size+1);
	}
	
	if (pti){
		ret = pti[(instance*(size_cell+address_cell))+(address_cell-1)].ti_Data;
		FreeVec(pti);
	}else{
		ret = tags[(instance*(size_cell+address_cell))+(address_cell-1)].ti_Data;
	}
	
	return ret;
}

ULONG __saveds __asm GetRegSize(register __a0 APTR node, register __d1 UWORD instance, register __a6 struct LibDevBase *base)
{
	struct TagItem tags[5], *pti=NULL;
	UWORD size = 0;
	ULONG ret = 0xFFFFFFFF, size_cell, address_cell;
	
	getSizeAddressCells((struct devicetreeConfig*)base->libData, (struct devicetreeNode*)node, &size_cell, &address_cell);
	if (address_cell == 0){
		return ret;
	}
	
	size = getRegRelative((struct devicetreeConfig*)base->libData, (struct devicetreeNode*)node, tags, 5);
	if (((instance+1)*(size_cell+address_cell)) > size){ // no entry for this instance
		return ret;
	}
	
	if (size > 4 && ((instance+1)*(size_cell+address_cell)) > 4){
		if (!(pti = AllocVec(sizeof(struct TagItem) * (size+1), MEMF_ANY | MEMF_CLEAR))){
			return ret;
		}
		getRegRelative((struct devicetreeConfig*)base->libData, (struct devicetreeNode*)node, pti, size+1);
	}
	
	if (pti){
		ret = pti[(instance*(size_cell+address_cell))+(address_cell)+(size_cell-1)].ti_Data;
		FreeVec(pti);
	}else{
		ret = tags[(instance*(size_cell+address_cell))+(address_cell)+(size_cell-1)].ti_Data;
	}
	
	return ret;
}