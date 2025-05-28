#ifndef _INCLUDE_PRAGMA_DEVICETREE_LIB_H
#define _INCLUDE_PRAGMA_DEVICETREE_LIB_H

#ifndef CLIB_DEVICETREE_PROTOS_H
#include <clib/devicetree_protos.h>
#endif

#if defined(AZTEC_C) || defined(__MAXON__) || defined(__STORM__)
#pragma amicall(DeviceTreeBase,0x01e,DeviceTree_GetNode(a0))
#pragma amicall(DeviceTreeBase,0x024,DeviceTree_GetNodeName(a0))
#pragma amicall(DeviceTreeBase,0x02a,DeviceTree_GetNodeAddress(a0))
#pragma amicall(DeviceTreeBase,0x030,DeviceTree_GetFirstChildNode(a0,a1))
#pragma amicall(DeviceTreeBase,0x036,DeviceTree_GetNextSiblingNode(a0,a1))
#pragma amicall(DeviceTreeBase,0x03c,DeviceTree_GetProperty(a0,a1))
#pragma amicall(DeviceTreeBase,0x042,DeviceTree_GetFirstProperty(a0))
#pragma amicall(DeviceTreeBase,0x048,DeviceTree_GetNextProperty(a0))
#pragma amicall(DeviceTreeBase,0x04e,DeviceTree_GetPropertyName(a0))
#pragma amicall(DeviceTreeBase,0x054,DeviceTree_GetPropertyStringValue(a0))
#pragma amicall(DeviceTreeBase,0x05a,DeviceTree_GetFirstPropertyValue(a0))
#pragma amicall(DeviceTreeBase,0x060,DeviceTree_GetNextPropertyValue(a0))
#pragma amicall(DeviceTreeBase,0x066,DeviceTree_GetValueSize(a0))
#pragma amicall(DeviceTreeBase,0x06c,DeviceTree_GetValueType(a0))
#pragma amicall(DeviceTreeBase,0x072,DeviceTree_GetValue(a0))
#endif
#if defined(_DCC) || defined(__SASC)
#pragma  libcall DeviceTreeBase DeviceTree_GetNode     01e 801
#pragma  libcall DeviceTreeBase DeviceTree_GetNodeName 024 801
#pragma  libcall DeviceTreeBase DeviceTree_GetNodeAddress 02a 801
#pragma  libcall DeviceTreeBase DeviceTree_GetFirstChildNode 030 9802
#pragma  libcall DeviceTreeBase DeviceTree_GetNextSiblingNode 036 9802
#pragma  libcall DeviceTreeBase DeviceTree_GetProperty 03c 9802
#pragma  libcall DeviceTreeBase DeviceTree_GetFirstProperty 042 801
#pragma  libcall DeviceTreeBase DeviceTree_GetNextProperty 048 801
#pragma  libcall DeviceTreeBase DeviceTree_GetPropertyName 04e 801
#pragma  libcall DeviceTreeBase DeviceTree_GetPropertyStringValue 054 801
#pragma  libcall DeviceTreeBase DeviceTree_GetFirstPropertyValue 05a 801
#pragma  libcall DeviceTreeBase DeviceTree_GetNextPropertyValue 060 801
#pragma  libcall DeviceTreeBase DeviceTree_GetValueSize 066 801
#pragma  libcall DeviceTreeBase DeviceTree_GetValueType 06c 801
#pragma  libcall DeviceTreeBase DeviceTree_GetValue    072 801
#endif

#endif	/*  _INCLUDE_PRAGMA_DEVICETREE_LIB_H  */
