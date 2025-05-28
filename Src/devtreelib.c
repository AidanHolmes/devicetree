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

void* __saveds __asm GetNode(register __a0 char *path, register __a6 struct LibDevBase *base)
{
	return getNode((struct devicetreeConfig*)base->libData, path);
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