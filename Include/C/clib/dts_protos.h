/* Automatically generated header! Do not edit! */

#ifndef CLIB_DTS_PROTOS_H
#define CLIB_DTS_PROTOS_H


/*
**	$VER: dts_protos.h 1.0 (14.06.2025)
**
**	C prototypes. For use with 32 bit integers only.
**
**	Copyright (C) 2025 Aidan Holmes
**	All Rights Reserved
*/

#ifndef  EXEC_TYPES_H
#include <exec/types.h>
#endif

char* DTS_GetNodeName(APTR node);
char* DTS_GetNodeAddress(APTR node);
char* DTS_GetNodePath(APTR node);
APTR DTS_GetProperty(APTR node, char * name);
APTR DTS_GetFirstProperty(APTR node);
APTR DTS_GetNextProperty(APTR lastProperty);
ULONG DTS_GetRegAddress(APTR node, ULONG instance);
ULONG DTS_GetRegSize(APTR node, ULONG instance);
APTR DTS_GetFirstChildNode(char * path, char * compatible);
APTR DTS_GetNextSiblingNode(APTR node, char * compatible);
APTR DTS_GetNodeByAlias(char * aliasName);
APTR DTS_GetNodeByChosen(char * aliasName);
APTR DTS_GetNodeByLabel(char * labelName);
APTR DTS_GetNodeByPath(char * path);
APTR DTS_GetCompatibleNodeInstance(char * compatibleStr, ULONG instance);
APTR DTS_GetCompatibleNodeOKAY(char * compatibleStr);
char* DTS_GetPropertyName(APTR property);
char* DTS_GetPropertyStringValue(APTR property);
APTR DTS_GetFirstPropertyValue(APTR property);
APTR DTS_GetNextPropertyValue(APTR value);
ULONG DTS_GetValueSize(APTR value);
UWORD DTS_GetValueType(APTR value);
APTR DTS_GetValue(APTR value);

#endif	/*  CLIB_DTS_PROTOS_H  */
