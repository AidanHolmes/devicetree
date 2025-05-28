#ifndef _VBCCINLINE_DEVICETREE_H
#define _VBCCINLINE_DEVICETREE_H

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

APTR __DeviceTree_GetNode(__reg("a6") struct Library *, __reg("a0") char * path)="\tjsr\t-30(a6)";
#define DeviceTree_GetNode(path) __DeviceTree_GetNode(DeviceTreeBase, (path))

char* __DeviceTree_GetNodeName(__reg("a6") struct Library *, __reg("a0") APTR node)="\tjsr\t-36(a6)";
#define DeviceTree_GetNodeName(node) __DeviceTree_GetNodeName(DeviceTreeBase, (node))

char* __DeviceTree_GetNodeAddress(__reg("a6") struct Library *, __reg("a0") APTR node)="\tjsr\t-42(a6)";
#define DeviceTree_GetNodeAddress(node) __DeviceTree_GetNodeAddress(DeviceTreeBase, (node))

APTR __DeviceTree_GetFirstChildNode(__reg("a6") struct Library *, __reg("a0") char * path, __reg("a1") char * compatible)="\tjsr\t-48(a6)";
#define DeviceTree_GetFirstChildNode(path, compatible) __DeviceTree_GetFirstChildNode(DeviceTreeBase, (path), (compatible))

APTR __DeviceTree_GetNextSiblingNode(__reg("a6") struct Library *, __reg("a0") APTR node, __reg("a1") char * compatible)="\tjsr\t-54(a6)";
#define DeviceTree_GetNextSiblingNode(node, compatible) __DeviceTree_GetNextSiblingNode(DeviceTreeBase, (node), (compatible))

APTR __DeviceTree_GetProperty(__reg("a6") struct Library *, __reg("a0") APTR node, __reg("a1") char * name)="\tjsr\t-60(a6)";
#define DeviceTree_GetProperty(node, name) __DeviceTree_GetProperty(DeviceTreeBase, (node), (name))

APTR __DeviceTree_GetFirstProperty(__reg("a6") struct Library *, __reg("a0") APTR node)="\tjsr\t-66(a6)";
#define DeviceTree_GetFirstProperty(node) __DeviceTree_GetFirstProperty(DeviceTreeBase, (node))

APTR __DeviceTree_GetNextProperty(__reg("a6") struct Library *, __reg("a0") APTR lastProperty)="\tjsr\t-72(a6)";
#define DeviceTree_GetNextProperty(lastProperty) __DeviceTree_GetNextProperty(DeviceTreeBase, (lastProperty))

char* __DeviceTree_GetPropertyName(__reg("a6") struct Library *, __reg("a0") APTR property)="\tjsr\t-78(a6)";
#define DeviceTree_GetPropertyName(property) __DeviceTree_GetPropertyName(DeviceTreeBase, (property))

char* __DeviceTree_GetPropertyStringValue(__reg("a6") struct Library *, __reg("a0") APTR property)="\tjsr\t-84(a6)";
#define DeviceTree_GetPropertyStringValue(property) __DeviceTree_GetPropertyStringValue(DeviceTreeBase, (property))

APTR __DeviceTree_GetFirstPropertyValue(__reg("a6") struct Library *, __reg("a0") APTR property)="\tjsr\t-90(a6)";
#define DeviceTree_GetFirstPropertyValue(property) __DeviceTree_GetFirstPropertyValue(DeviceTreeBase, (property))

APTR __DeviceTree_GetNextPropertyValue(__reg("a6") struct Library *, __reg("a0") APTR value)="\tjsr\t-96(a6)";
#define DeviceTree_GetNextPropertyValue(value) __DeviceTree_GetNextPropertyValue(DeviceTreeBase, (value))

ULONG __DeviceTree_GetValueSize(__reg("a6") struct Library *, __reg("a0") APTR value)="\tjsr\t-102(a6)";
#define DeviceTree_GetValueSize(value) __DeviceTree_GetValueSize(DeviceTreeBase, (value))

UWORD __DeviceTree_GetValueType(__reg("a6") struct Library *, __reg("a0") APTR value)="\tjsr\t-108(a6)";
#define DeviceTree_GetValueType(value) __DeviceTree_GetValueType(DeviceTreeBase, (value))

APTR __DeviceTree_GetValue(__reg("a6") struct Library *, __reg("a0") APTR value)="\tjsr\t-114(a6)";
#define DeviceTree_GetValue(value) __DeviceTree_GetValue(DeviceTreeBase, (value))

#endif /*  _VBCCINLINE_DEVICETREE_H  */
