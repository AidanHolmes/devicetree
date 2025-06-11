#ifndef __H_LIBDEV_
#define __H_LIBDEV_
#include <exec/types.h>
#include <exec/resident.h>
#include <exec/exec.h>
#include <proto/exec.h>

/* Exported functions */

// Node functions
char* __saveds __asm GetNodeName(register __a0 APTR node, register __a6 struct LibDevBase *base);
char* __saveds __asm GetNodeAddress(register __a0 APTR node, register __a6 struct LibDevBase *base);
char* __saveds __asm GetNodePath(register __a0 APTR node, register __a6 struct LibDevBase *base);
APTR __saveds __asm GetProperty(register __a0 APTR node, register __a1 char *name, register __a6 struct LibDevBase *base);
APTR __saveds __asm GetFirstProperty(register __a0 APTR node, register __a6 struct LibDevBase *base);
APTR __saveds __asm GetNextProperty(register __a0 APTR lastProperty, register __a6 struct LibDevBase *base);

// Find node functions
APTR __saveds __asm GetFirstChildNode(register __a0 char *path, register __a1 char *compatible, register __a6 struct LibDevBase *base);
APTR __saveds __asm GetNextSiblingNode(register __a0 APTR node, register __a1 char *compatible, register __a6 struct LibDevBase *base);
APTR __saveds __asm GetNodeByAlias(register __a0 char *aliasName, register __a6 struct LibDevBase *base);
APTR __saveds __asm GetNodeByChosen(register __a0 char *chosenName, register __a6 struct LibDevBase *base);
APTR __saveds __asm GetNodeByLabel(register __a0 char *labelName, register __a6 struct LibDevBase *base);
APTR __saveds __asm GetNodeByPath(register __a0 char *path, register __a6 struct LibDevBase *base);
APTR __saveds __asm GetCompatibleNodeInstance(register __a0 char *compatibleStr, register __d1 UWORD instance, register __a6 struct LibDevBase *base);
APTR __saveds __asm GetCompatibleNodeOKAY(register __a0 char *compatibleStr, register __a6 struct LibDevBase *base);

// Property functions
char* __saveds __asm GetPropertyName(register __a0 APTR property, register __a6 struct LibDevBase *base);
char* __saveds __asm GetPropertyStringValue(register __a0 APTR property, register __a6 struct LibDevBase *base);
APTR __saveds __asm GetFirstPropertyValue(register __a0 APTR property, register __a6 struct LibDevBase *base);
APTR __saveds __asm GetNextPropertyValue(register __a0 APTR value, register __a6 struct LibDevBase *base);
ULONG __saveds __asm GetValueSize(register __a0 APTR value, register __a6 struct LibDevBase *base);
UWORD __saveds __asm GetValueType(register __a0 APTR value, register __a6 struct LibDevBase *base);
APTR __saveds __asm GetValue(register __a0 APTR value, register __a6 struct LibDevBase *base);


/* Customise base according to requirements */
struct LibDevBase
{
	struct Device device;
	APTR seg_list;
	struct ExecBase *sys_base;
	struct Process *drvProc;
	struct Library *dosbase;
	struct MsgPort *drvPort;
	BYTE sigTerm;
	void *libData; // generic pointer to any remaining library/device data
};

#endif