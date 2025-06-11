#ifndef _INCLUDE_PRAGMA_DTS_LIB_H
#define _INCLUDE_PRAGMA_DTS_LIB_H

#ifndef CLIB_DTS_PROTOS_H
#include <clib/dts_protos.h>
#endif

#if defined(AZTEC_C) || defined(__MAXON__) || defined(__STORM__)
#pragma amicall(DtsParserBase,0x01e,DTS_GetNodeName(a0))
#pragma amicall(DtsParserBase,0x024,DTS_GetNodeAddress(a0))
#pragma amicall(DtsParserBase,0x02a,DTS_GetNodePath(a0))
#pragma amicall(DtsParserBase,0x030,DTS_GetProperty(a0,a1))
#pragma amicall(DtsParserBase,0x036,DTS_GetFirstProperty(a0))
#pragma amicall(DtsParserBase,0x03c,DTS_GetNextProperty(a0))
#pragma amicall(DtsParserBase,0x042,DTS_GetFirstChildNode(a0,a1))
#pragma amicall(DtsParserBase,0x048,DTS_GetNextSiblingNode(a0,a1))
#pragma amicall(DtsParserBase,0x04e,DTS_GetNodeByAlias(a0))
#pragma amicall(DtsParserBase,0x054,DTS_GetNodeByChosen(a0))
#pragma amicall(DtsParserBase,0x05a,DTS_GetNodeByLabel(a0))
#pragma amicall(DtsParserBase,0x060,DTS_GetNodeByPath(a0))
#pragma amicall(DtsParserBase,0x066,DTS_GetCompatibleNodeInstance(a0,d1))
#pragma amicall(DtsParserBase,0x06c,DTS_GetCompatibleNodeOKAY(a0))
#pragma amicall(DtsParserBase,0x072,DTS_GetPropertyName(a0))
#pragma amicall(DtsParserBase,0x078,DTS_GetPropertyStringValue(a0))
#pragma amicall(DtsParserBase,0x07e,DTS_GetFirstPropertyValue(a0))
#pragma amicall(DtsParserBase,0x084,DTS_GetNextPropertyValue(a0))
#pragma amicall(DtsParserBase,0x08a,DTS_GetValueSize(a0))
#pragma amicall(DtsParserBase,0x090,DTS_GetValueType(a0))
#pragma amicall(DtsParserBase,0x096,DTS_GetValue(a0))
#endif
#if defined(_DCC) || defined(__SASC)
#pragma  libcall DtsParserBase DTS_GetNodeName        01e 801
#pragma  libcall DtsParserBase DTS_GetNodeAddress     024 801
#pragma  libcall DtsParserBase DTS_GetNodePath        02a 801
#pragma  libcall DtsParserBase DTS_GetProperty        030 9802
#pragma  libcall DtsParserBase DTS_GetFirstProperty   036 801
#pragma  libcall DtsParserBase DTS_GetNextProperty    03c 801
#pragma  libcall DtsParserBase DTS_GetFirstChildNode  042 9802
#pragma  libcall DtsParserBase DTS_GetNextSiblingNode 048 9802
#pragma  libcall DtsParserBase DTS_GetNodeByAlias     04e 801
#pragma  libcall DtsParserBase DTS_GetNodeByChosen    054 801
#pragma  libcall DtsParserBase DTS_GetNodeByLabel     05a 801
#pragma  libcall DtsParserBase DTS_GetNodeByPath      060 801
#pragma  libcall DtsParserBase DTS_GetCompatibleNodeInstance 066 1802
#pragma  libcall DtsParserBase DTS_GetCompatibleNodeOKAY 06c 801
#pragma  libcall DtsParserBase DTS_GetPropertyName    072 801
#pragma  libcall DtsParserBase DTS_GetPropertyStringValue 078 801
#pragma  libcall DtsParserBase DTS_GetFirstPropertyValue 07e 801
#pragma  libcall DtsParserBase DTS_GetNextPropertyValue 084 801
#pragma  libcall DtsParserBase DTS_GetValueSize       08a 801
#pragma  libcall DtsParserBase DTS_GetValueType       090 801
#pragma  libcall DtsParserBase DTS_GetValue           096 801
#endif

#endif	/*  _INCLUDE_PRAGMA_DTS_LIB_H  */
