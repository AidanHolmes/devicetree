#ifndef _PROTO_DEVICETREE_H
#define _PROTO_DEVICETREE_H

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#if !defined(CLIB_DEVICETREE_PROTOS_H) && !defined(__GNUC__)
#include <clib/devicetree_protos.h>
#endif

#ifndef __NOLIBBASE__
extern struct Library *DeviceTreeBase;
#endif

#ifdef __GNUC__
#include <inline/devicetree.h>
#elif defined(__VBCC__)
#if defined(__MORPHOS__) || !defined(__PPC__)
#include <inline/devicetree_protos.h>
#endif
#else
#include <pragma/devicetree_lib.h>
#endif

#endif	/*  _PROTO_DEVICETREE_H  */
