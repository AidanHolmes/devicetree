#ifndef _PROTO_DTS_H
#define _PROTO_DTS_H

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#if !defined(CLIB_DTS_PROTOS_H) && !defined(__GNUC__)
#include <clib/dts_protos.h>
#endif

#ifndef __NOLIBBASE__
extern struct Library *DtsParserBase;
#endif

#ifdef __GNUC__
#include <inline/dts.h>
#elif defined(__VBCC__)
#if defined(__MORPHOS__) || !defined(__PPC__)
#include <inline/dts_protos.h>
#endif
#else
#include <pragma/dts_lib.h>
#endif

#endif	/*  _PROTO_DTS_H  */
