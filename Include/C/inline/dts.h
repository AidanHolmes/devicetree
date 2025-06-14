#ifndef _INLINE_DTS_H
#define _INLINE_DTS_H

#ifndef CLIB_DTS_PROTOS_H
#define CLIB_DTS_PROTOS_H
#endif

#ifndef __INLINE_MACROS_H
#include <inline/macros.h>
#endif

#ifndef  EXEC_TYPES_H
#include <exec/types.h>
#endif

#ifndef DTS_BASE_NAME
#define DTS_BASE_NAME DtsParserBase
#endif

#define DTS_GetNodeName(node) \
	LP1(0x1e, char*, DTS_GetNodeName, APTR, node, a0, \
	, DTS_BASE_NAME)

#define DTS_GetNodeAddress(node) \
	LP1(0x24, char*, DTS_GetNodeAddress, APTR, node, a0, \
	, DTS_BASE_NAME)

#define DTS_GetNodePath(node) \
	LP1(0x2a, char*, DTS_GetNodePath, APTR, node, a0, \
	, DTS_BASE_NAME)

#define DTS_GetProperty(node, name) \
	LP2(0x30, APTR, DTS_GetProperty, APTR, node, a0, char *, name, a1, \
	, DTS_BASE_NAME)

#define DTS_GetFirstProperty(node) \
	LP1(0x36, APTR, DTS_GetFirstProperty, APTR, node, a0, \
	, DTS_BASE_NAME)

#define DTS_GetNextProperty(lastProperty) \
	LP1(0x3c, APTR, DTS_GetNextProperty, APTR, lastProperty, a0, \
	, DTS_BASE_NAME)

#define DTS_GetRegAddress(node, instance) \
	LP2(0x42, ULONG, DTS_GetRegAddress, APTR, node, a0, ULONG, instance, d1, \
	, DTS_BASE_NAME)

#define DTS_GetRegSize(node, instance) \
	LP2(0x48, ULONG, DTS_GetRegSize, APTR, node, a0, ULONG, instance, d1, \
	, DTS_BASE_NAME)

#define DTS_GetFirstChildNode(path, compatible) \
	LP2(0x4e, APTR, DTS_GetFirstChildNode, char *, path, a0, char *, compatible, a1, \
	, DTS_BASE_NAME)

#define DTS_GetNextSiblingNode(node, compatible) \
	LP2(0x54, APTR, DTS_GetNextSiblingNode, APTR, node, a0, char *, compatible, a1, \
	, DTS_BASE_NAME)

#define DTS_GetNodeByAlias(aliasName) \
	LP1(0x5a, APTR, DTS_GetNodeByAlias, char *, aliasName, a0, \
	, DTS_BASE_NAME)

#define DTS_GetNodeByChosen(aliasName) \
	LP1(0x60, APTR, DTS_GetNodeByChosen, char *, aliasName, a0, \
	, DTS_BASE_NAME)

#define DTS_GetNodeByLabel(labelName) \
	LP1(0x66, APTR, DTS_GetNodeByLabel, char *, labelName, a0, \
	, DTS_BASE_NAME)

#define DTS_GetNodeByPath(path) \
	LP1(0x6c, APTR, DTS_GetNodeByPath, char *, path, a0, \
	, DTS_BASE_NAME)

#define DTS_GetCompatibleNodeInstance(compatibleStr, instance) \
	LP2(0x72, APTR, DTS_GetCompatibleNodeInstance, char *, compatibleStr, a0, ULONG, instance, d1, \
	, DTS_BASE_NAME)

#define DTS_GetCompatibleNodeOKAY(compatibleStr) \
	LP1(0x78, APTR, DTS_GetCompatibleNodeOKAY, char *, compatibleStr, a0, \
	, DTS_BASE_NAME)

#define DTS_GetPropertyName(property) \
	LP1(0x7e, char*, DTS_GetPropertyName, APTR, property, a0, \
	, DTS_BASE_NAME)

#define DTS_GetPropertyStringValue(property) \
	LP1(0x84, char*, DTS_GetPropertyStringValue, APTR, property, a0, \
	, DTS_BASE_NAME)

#define DTS_GetFirstPropertyValue(property) \
	LP1(0x8a, APTR, DTS_GetFirstPropertyValue, APTR, property, a0, \
	, DTS_BASE_NAME)

#define DTS_GetNextPropertyValue(value) \
	LP1(0x90, APTR, DTS_GetNextPropertyValue, APTR, value, a0, \
	, DTS_BASE_NAME)

#define DTS_GetValueSize(value) \
	LP1(0x96, ULONG, DTS_GetValueSize, APTR, value, a0, \
	, DTS_BASE_NAME)

#define DTS_GetValueType(value) \
	LP1(0x9c, UWORD, DTS_GetValueType, APTR, value, a0, \
	, DTS_BASE_NAME)

#define DTS_GetValue(value) \
	LP1(0xa2, APTR, DTS_GetValue, APTR, value, a0, \
	, DTS_BASE_NAME)

#endif /*  _INLINE_DTS_H  */
