/* Automatically generated header! Do not edit! */

#ifndef CLIB_DEVICETREE_PROTOS_H
#define CLIB_DEVICETREE_PROTOS_H


/*
**	$VER: devicetree_protos.h devicetree_lib.sfd (28.05.2025)
**
**	C prototypes. For use with 32 bit integers only.
**
**	Copyright (C) 2025 Aidan Holmes
**	All Rights Reserved
*/

#ifndef  EXEC_TYPES_H
#include <exec/types.h>
#endif

APTR DeviceTree_GetNode(char * path);
char* DeviceTree_GetNodeName(APTR node);
char* DeviceTree_GetNodeAddress(APTR node);
APTR DeviceTree_GetFirstChildNode(char * path, char * compatible);
APTR DeviceTree_GetNextSiblingNode(APTR node, char * compatible);
APTR DeviceTree_GetProperty(APTR node, char * name);
APTR DeviceTree_GetFirstProperty(APTR node);
APTR DeviceTree_GetNextProperty(APTR lastProperty);
char* DeviceTree_GetPropertyName(APTR property);
char* DeviceTree_GetPropertyStringValue(APTR property);
APTR DeviceTree_GetFirstPropertyValue(APTR property);
APTR DeviceTree_GetNextPropertyValue(APTR value);
ULONG DeviceTree_GetValueSize(APTR value);
UWORD DeviceTree_GetValueType(APTR value);
APTR DeviceTree_GetValue(APTR value);

#endif	/*  CLIB_DEVICETREE_PROTOS_H  */
