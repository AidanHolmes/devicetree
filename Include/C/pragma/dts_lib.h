#ifndef _INCLUDE_PRAGMA_DTS_LIB_H
#define _INCLUDE_PRAGMA_DTS_LIB_H

#ifndef CLIB_DTS_PROTOS_H
#include <clib/dts_protos.h>
#endif

#if defined(AZTEC_C) || defined(__MAXON__) || defined(__STORM__)
#pragma amicall(DtsParserBase,0x01e,DTS_GetNode(a0))
#pragma amicall(DtsParserBase,0x024,DTS_GetNodeName(a0))
#pragma amicall(DtsParserBase,0x02a,DTS_GetNodeAddress(a0))
#pragma amicall(DtsParserBase,0x030,DTS_GetFirstChildNode(a0,a1))
#pragma amicall(DtsParserBase,0x036,DTS_GetNextSiblingNode(a0,a1))
#pragma amicall(DtsParserBase,0x03c,DTS_GetProperty(a0,a1))
#pragma amicall(DtsParserBase,0x042,DTS_GetFirstProperty(a0))
#pragma amicall(DtsParserBase,0x048,DTS_GetNextProperty(a0))
#pragma amicall(DtsParserBase,0x04e,DTS_GetPropertyName(a0))
#pragma amicall(DtsParserBase,0x054,DTS_GetPropertyStringValue(a0))
#pragma amicall(DtsParserBase,0x05a,DTS_GetFirstPropertyValue(a0))
#pragma amicall(DtsParserBase,0x060,DTS_GetNextPropertyValue(a0))
#pragma amicall(DtsParserBase,0x066,DTS_GetValueSize(a0))
#pragma amicall(DtsParserBase,0x06c,DTS_GetValueType(a0))
#pragma amicall(DtsParserBase,0x072,DTS_GetValue(a0))
#endif
#if defined(_DCC) || defined(__SASC)
#pragma  libcall DtsParserBase DTS_GetNode            01e 801
#pragma  libcall DtsParserBase DTS_GetNodeName        024 801
#pragma  libcall DtsParserBase DTS_GetNodeAddress     02a 801
#pragma  libcall DtsParserBase DTS_GetFirstChildNode  030 9802
#pragma  libcall DtsParserBase DTS_GetNextSiblingNode 036 9802
#pragma  libcall DtsParserBase DTS_GetProperty        03c 9802
#pragma  libcall DtsParserBase DTS_GetFirstProperty   042 801
#pragma  libcall DtsParserBase DTS_GetNextProperty    048 801
#pragma  libcall DtsParserBase DTS_GetPropertyName    04e 801
#pragma  libcall DtsParserBase DTS_GetPropertyStringValue 054 801
#pragma  libcall DtsParserBase DTS_GetFirstPropertyValue 05a 801
#pragma  libcall DtsParserBase DTS_GetNextPropertyValue 060 801
#pragma  libcall DtsParserBase DTS_GetValueSize       066 801
#pragma  libcall DtsParserBase DTS_GetValueType       06c 801
#pragma  libcall DtsParserBase DTS_GetValue           072 801
#endif

#endif	/*  _INCLUDE_PRAGMA_DTS_LIB_H  */
