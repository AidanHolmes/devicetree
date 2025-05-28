/* Automatically generated header! Do not edit! */

#ifndef CLIB_DTS_PROTOS_H
#define CLIB_DTS_PROTOS_H


/*
**	$VER: dts_protos.h 1.0 (28.05.2025)
**
**	C prototypes. For use with 32 bit integers only.
**
**	Copyright (C) 2025 Aidan Holmes
**	All Rights Reserved
*/

#ifndef  EXEC_TYPES_H
#include <exec/types.h>
#endif

APTR DTS_GetNode(char * path);
char* DTS_GetNodeName(APTR node);
char* DTS_GetNodeAddress(APTR node);
APTR DTS_GetFirstChildNode(char * path, char * compatible);
APTR DTS_GetNextSiblingNode(APTR node, char * compatible);
APTR DTS_GetProperty(APTR node, char * name);
APTR DTS_GetFirstProperty(APTR node);
APTR DTS_GetNextProperty(APTR lastProperty);
char* DTS_GetPropertyName(APTR property);
char* DTS_GetPropertyStringValue(APTR property);
APTR DTS_GetFirstPropertyValue(APTR property);
APTR DTS_GetNextPropertyValue(APTR value);
ULONG DTS_GetValueSize(APTR value);
UWORD DTS_GetValueType(APTR value);
APTR DTS_GetValue(APTR value);

#endif	/*  CLIB_DTS_PROTOS_H  */
