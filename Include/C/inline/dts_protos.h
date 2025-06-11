#ifndef _VBCCINLINE_DTS_H
#define _VBCCINLINE_DTS_H

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

char* __DTS_GetNodeName(__reg("a6") struct Library *, __reg("a0") APTR node)="\tjsr\t-30(a6)";
#define DTS_GetNodeName(node) __DTS_GetNodeName(DtsParserBase, (node))

char* __DTS_GetNodeAddress(__reg("a6") struct Library *, __reg("a0") APTR node)="\tjsr\t-36(a6)";
#define DTS_GetNodeAddress(node) __DTS_GetNodeAddress(DtsParserBase, (node))

char* __DTS_GetNodePath(__reg("a6") struct Library *, __reg("a0") APTR node)="\tjsr\t-42(a6)";
#define DTS_GetNodePath(node) __DTS_GetNodePath(DtsParserBase, (node))

APTR __DTS_GetProperty(__reg("a6") struct Library *, __reg("a0") APTR node, __reg("a1") char * name)="\tjsr\t-48(a6)";
#define DTS_GetProperty(node, name) __DTS_GetProperty(DtsParserBase, (node), (name))

APTR __DTS_GetFirstProperty(__reg("a6") struct Library *, __reg("a0") APTR node)="\tjsr\t-54(a6)";
#define DTS_GetFirstProperty(node) __DTS_GetFirstProperty(DtsParserBase, (node))

APTR __DTS_GetNextProperty(__reg("a6") struct Library *, __reg("a0") APTR lastProperty)="\tjsr\t-60(a6)";
#define DTS_GetNextProperty(lastProperty) __DTS_GetNextProperty(DtsParserBase, (lastProperty))

APTR __DTS_GetFirstChildNode(__reg("a6") struct Library *, __reg("a0") char * path, __reg("a1") char * compatible)="\tjsr\t-66(a6)";
#define DTS_GetFirstChildNode(path, compatible) __DTS_GetFirstChildNode(DtsParserBase, (path), (compatible))

APTR __DTS_GetNextSiblingNode(__reg("a6") struct Library *, __reg("a0") APTR node, __reg("a1") char * compatible)="\tjsr\t-72(a6)";
#define DTS_GetNextSiblingNode(node, compatible) __DTS_GetNextSiblingNode(DtsParserBase, (node), (compatible))

APTR __DTS_GetNodeByAlias(__reg("a6") struct Library *, __reg("a0") char * aliasName)="\tjsr\t-78(a6)";
#define DTS_GetNodeByAlias(aliasName) __DTS_GetNodeByAlias(DtsParserBase, (aliasName))

APTR __DTS_GetNodeByChosen(__reg("a6") struct Library *, __reg("a0") char * aliasName)="\tjsr\t-84(a6)";
#define DTS_GetNodeByChosen(aliasName) __DTS_GetNodeByChosen(DtsParserBase, (aliasName))

APTR __DTS_GetNodeByLabel(__reg("a6") struct Library *, __reg("a0") char * labelName)="\tjsr\t-90(a6)";
#define DTS_GetNodeByLabel(labelName) __DTS_GetNodeByLabel(DtsParserBase, (labelName))

APTR __DTS_GetNodeByPath(__reg("a6") struct Library *, __reg("a0") char * path)="\tjsr\t-96(a6)";
#define DTS_GetNodeByPath(path) __DTS_GetNodeByPath(DtsParserBase, (path))

APTR __DTS_GetCompatibleNodeInstance(__reg("a6") struct Library *, __reg("a0") char * compatibleStr, __reg("d1") ULONG instance)="\tjsr\t-102(a6)";
#define DTS_GetCompatibleNodeInstance(compatibleStr, instance) __DTS_GetCompatibleNodeInstance(DtsParserBase, (compatibleStr), (instance))

APTR __DTS_GetCompatibleNodeOKAY(__reg("a6") struct Library *, __reg("a0") char * compatibleStr)="\tjsr\t-108(a6)";
#define DTS_GetCompatibleNodeOKAY(compatibleStr) __DTS_GetCompatibleNodeOKAY(DtsParserBase, (compatibleStr))

char* __DTS_GetPropertyName(__reg("a6") struct Library *, __reg("a0") APTR property)="\tjsr\t-114(a6)";
#define DTS_GetPropertyName(property) __DTS_GetPropertyName(DtsParserBase, (property))

char* __DTS_GetPropertyStringValue(__reg("a6") struct Library *, __reg("a0") APTR property)="\tjsr\t-120(a6)";
#define DTS_GetPropertyStringValue(property) __DTS_GetPropertyStringValue(DtsParserBase, (property))

APTR __DTS_GetFirstPropertyValue(__reg("a6") struct Library *, __reg("a0") APTR property)="\tjsr\t-126(a6)";
#define DTS_GetFirstPropertyValue(property) __DTS_GetFirstPropertyValue(DtsParserBase, (property))

APTR __DTS_GetNextPropertyValue(__reg("a6") struct Library *, __reg("a0") APTR value)="\tjsr\t-132(a6)";
#define DTS_GetNextPropertyValue(value) __DTS_GetNextPropertyValue(DtsParserBase, (value))

ULONG __DTS_GetValueSize(__reg("a6") struct Library *, __reg("a0") APTR value)="\tjsr\t-138(a6)";
#define DTS_GetValueSize(value) __DTS_GetValueSize(DtsParserBase, (value))

UWORD __DTS_GetValueType(__reg("a6") struct Library *, __reg("a0") APTR value)="\tjsr\t-144(a6)";
#define DTS_GetValueType(value) __DTS_GetValueType(DtsParserBase, (value))

APTR __DTS_GetValue(__reg("a6") struct Library *, __reg("a0") APTR value)="\tjsr\t-150(a6)";
#define DTS_GetValue(value) __DTS_GetValue(DtsParserBase, (value))

#endif /*  _VBCCINLINE_DTS_H  */
