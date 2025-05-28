#ifndef _INLINE_DEVICETREE_H
#define _INLINE_DEVICETREE_H

#ifndef CLIB_DEVICETREE_PROTOS_H
#define CLIB_DEVICETREE_PROTOS_H
#endif

#ifndef __INLINE_MACROS_H
#include <inline/macros.h>
#endif

#ifndef  EXEC_TYPES_H
#include <exec/types.h>
#endif

#ifndef DEVICETREE_BASE_NAME
#define DEVICETREE_BASE_NAME DeviceTreeBase
#endif

#define DeviceTree_GetNode(path) \
	LP1(0x1e, APTR, DeviceTree_GetNode, char *, path, a0, \
	, DEVICETREE_BASE_NAME)

#define DeviceTree_GetNodeName(node) \
	LP1(0x24, char*, DeviceTree_GetNodeName, APTR, node, a0, \
	, DEVICETREE_BASE_NAME)

#define DeviceTree_GetNodeAddress(node) \
	LP1(0x2a, char*, DeviceTree_GetNodeAddress, APTR, node, a0, \
	, DEVICETREE_BASE_NAME)

#define DeviceTree_GetFirstChildNode(path, compatible) \
	LP2(0x30, APTR, DeviceTree_GetFirstChildNode, char *, path, a0, char *, compatible, a1, \
	, DEVICETREE_BASE_NAME)

#define DeviceTree_GetNextSiblingNode(node, compatible) \
	LP2(0x36, APTR, DeviceTree_GetNextSiblingNode, APTR, node, a0, char *, compatible, a1, \
	, DEVICETREE_BASE_NAME)

#define DeviceTree_GetProperty(node, name) \
	LP2(0x3c, APTR, DeviceTree_GetProperty, APTR, node, a0, char *, name, a1, \
	, DEVICETREE_BASE_NAME)

#define DeviceTree_GetFirstProperty(node) \
	LP1(0x42, APTR, DeviceTree_GetFirstProperty, APTR, node, a0, \
	, DEVICETREE_BASE_NAME)

#define DeviceTree_GetNextProperty(lastProperty) \
	LP1(0x48, APTR, DeviceTree_GetNextProperty, APTR, lastProperty, a0, \
	, DEVICETREE_BASE_NAME)

#define DeviceTree_GetPropertyName(property) \
	LP1(0x4e, char*, DeviceTree_GetPropertyName, APTR, property, a0, \
	, DEVICETREE_BASE_NAME)

#define DeviceTree_GetPropertyStringValue(property) \
	LP1(0x54, char*, DeviceTree_GetPropertyStringValue, APTR, property, a0, \
	, DEVICETREE_BASE_NAME)

#define DeviceTree_GetFirstPropertyValue(property) \
	LP1(0x5a, APTR, DeviceTree_GetFirstPropertyValue, APTR, property, a0, \
	, DEVICETREE_BASE_NAME)

#define DeviceTree_GetNextPropertyValue(value) \
	LP1(0x60, APTR, DeviceTree_GetNextPropertyValue, APTR, value, a0, \
	, DEVICETREE_BASE_NAME)

#define DeviceTree_GetValueSize(value) \
	LP1(0x66, ULONG, DeviceTree_GetValueSize, APTR, value, a0, \
	, DEVICETREE_BASE_NAME)

#define DeviceTree_GetValueType(value) \
	LP1(0x6c, UWORD, DeviceTree_GetValueType, APTR, value, a0, \
	, DEVICETREE_BASE_NAME)

#define DeviceTree_GetValue(value) \
	LP1(0x72, APTR, DeviceTree_GetValue, APTR, value, a0, \
	, DEVICETREE_BASE_NAME)

#endif /*  _INLINE_DEVICETREE_H  */
