#include <exec/types.h>
#include <exec/resident.h>
#include <exec/exec.h>
#include <proto/exec.h>
#include "libdev.h"

/*
#define OFFSET(struct_name, struct_field) \
   ((ULONG)(&(((struct struct_name *)0)->struct_field)))
*/
/* Use the following macros in the structure definition */

#define INITBYTEDEF(name) \
   UBYTE name ## _c; UBYTE name ## _o1; UBYTE name ## _o2; \
   UBYTE name ## _o3; UBYTE name ## _v; UBYTE name ## _p
#define INITWORDDEF(name) \
   UBYTE name ## _c; UBYTE name ## _o1; UBYTE name ## _o2; \
   UWORD name ## _v
#define INITLONGDEF(name) \
   UBYTE name ## _c; UBYTE name ## _o1; UBYTE name ## _o2; \
   ULONG name ## _v
#define INITPINTDEF(name) \
   UBYTE name ## _c; UBYTE name ## _o1; UBYTE name ## _o2; \
   ULONG name ## _v

#define SMALLINITBYTEDEF(name) \
   UBYTE name ## _c; UBYTE name ## _o; UBYTE name ## _v; UBYTE name ## _p
#define SMALLINITWORDDEF(name) \
   UBYTE name ## _c; UBYTE name ## _o; UWORD name ## _v
#define SMALLINITLONGDEF(name) \
   UBYTE name ## _c; UBYTE name ## _o; ULONG name ## _v
#define SMALLINITPINTDEF(name) \
   UBYTE name ## _c; UBYTE name ## _o; ULONG name ## _v

#define INITENDDEF UBYTE the_end

/* Use the following macros to fill in a structure */

#define NEWINITBYTE(offset, value) \
   0xe0, (UBYTE)((offset) >> 16), (UBYTE)((offset) >> 8), (UBYTE)(offset), \
   (UBYTE)(value), 0
#define NEWINITWORD(offset, value) \
   0xd0, (UBYTE)((offset) >> 16), (UBYTE)((offset) >> 8), (UBYTE)(offset), \
   (UWORD)(value)
#define NEWINITLONG(offset, value) \
   0xc0, (UBYTE)((offset) >> 16), (UBYTE)((offset) >> 8), (UBYTE)(offset), \
   (ULONG)(value)
#define INITPINT(offset, value) \
   0xc0, (UBYTE)((offset) >> 16), (UBYTE)((offset) >> 8), (UBYTE)(offset), \
   (ULONG)(value)

#define SMALLINITBYTE(offset, value) \
   0xa0, (offset), (UBYTE)(value), 0
#define SMALLINITWORD(offset, value) \
   0x90, (offset), (UWORD)(value)
#define SMALLINITLONG(offset, value) \
   0x80, (offset), (ULONG)(value)
#define SMALLINITPINT(offset, value) \
   0x80, (offset), (ULONG)(value)

#define INITEND 0

/* Obsolete definitions */
/*
#define INITBYTE(offset, value) \
   (0xe000 | ((UWORD)(offset) >> 16)), \
   (UWORD)(offset), (UWORD)((value) << 8)
#define INITWORD(offset, value) \
   (0xd000 | ((UWORD)(offset) >> 16)), (UWORD)(offset), (UWORD)(value)
#define INITLONG(offset, value) \
   (0xc000 | ((UWORD)(offset) >> 16)), (UWORD)(offset), \
   (UWORD)((value) >> 16), (UWORD)(value)
#define INITAPTR(offset, value) \
   (0xc000 | ((UWORD)(offset) >> 16)), (UWORD)(offset), \
   (UWORD)((ULONG)(value) >> 16), (UWORD)(value)

#endif
*/
#ifndef LIBDEVNAME
#define LIBDEVNAME "none.device"
#endif

#define _STR(A) #A
#define STR(A) _STR(A)

#include "libdev.h"

#ifndef LIBDEVMAJOR
#define LIBDEVMAJOR 1
#endif

#ifndef LIBDEVMINOR
#define LIBDEVMINOR 0
#endif

#ifndef LIBDEVDATE
#define LIBDEVDATE "1.1.2024"
#endif

#ifdef AS_LIBRARY
static struct LibDevBase* __saveds __asm library_open(register __a6 struct LibDevBase *base);
extern struct LibDevBase* __saveds __asm libdev_library_open(register __a6 struct LibDevBase *base);
#else
static int __saveds __asm device_open(register __a6 struct LibDevBase *base, register __d0 ULONG unit, register __d1 ULONG flags, register __a1 struct IORequest *ior);
extern int __saveds __asm libdev_device_open(register __a6 struct LibDevBase *base, register __d0 ULONG unit, register __d1 ULONG flags, register __a1 struct IORequest *ior);
extern void __saveds __asm beginio(register __a6 struct LibDevBase *base, register __a1 struct IORequest *ior);
extern void __saveds __asm abortio(register __a6 struct LibDevBase *base, register __a1 struct IORequest *ior);
#endif

static void DeleteLibrary(struct LibDevBase *base);
static struct LibDevBase* __saveds __asm library_init(register __d0 struct LibDevBase *lib_base, register __a0 APTR seg_list, register __a6 struct LibDevBase *base);
static APTR __saveds __asm library_close(register __a6 struct LibDevBase *base);
static APTR __saveds __asm library_expunge(register __a6 struct LibDevBase *base);
static APTR __saveds __asm library_reserved(void){return NULL;}

extern struct LibDevBase* __saveds __asm libdev_initalise(register __a6 struct LibDevBase *base) ;
extern void __saveds __asm libdev_cleanup(register __a6 struct LibDevBase *base);

extern const APTR vectors[];
extern const APTR init_table[];

int main()
{
   return -1;
}

static const char library_name[] = LIBDEVNAME;
static const char version_string[] =
  LIBDEVNAME " " STR(LIBDEVMAJOR) "." STR(LIBDEVMINOR) " (" LIBDEVDATE ")\n";

static const struct Resident rom_tag =
{
   RTC_MATCHWORD,
   (struct Resident *)&rom_tag,
   (APTR)init_table,
   RTF_AUTOINIT,
   LIBDEVMAJOR,
#ifdef AS_LIBRARY
   NT_LIBRARY,
#else
   NT_DEVICE,
#endif
   0,
   (STRPTR)library_name,
   (STRPTR)version_string,
   (APTR)init_table
};

const APTR vectors[] =
{
#ifdef AS_LIBRARY
   (APTR)library_open,
#else
   (APTR)device_open,
#endif
   (APTR)library_close,
   (APTR)library_expunge,
   (APTR)library_reserved,
#ifndef AS_LIBRARY
   (APTR)beginio,
   (APTR)abortio,
#endif
/* Add all library functions here */
	(APTR)GetNodeName,
	(APTR)GetNodeAddress,
	(APTR)GetNodePath,
	(APTR)GetProperty,
	(APTR)GetFirstProperty,
	(APTR)GetNextProperty,
	(APTR)GetRegAddress,
	(APTR)GetRegSize,
	(APTR)GetFirstChildNode,
	(APTR)GetNextSiblingNode,
	(APTR)GetNodeByAlias,
	(APTR)GetNodeByChosen,
	(APTR)GetNodeByLabel,
	(APTR)GetNodeByPath,
	(APTR)GetCompatibleNodeInstance,
	(APTR)GetCompatibleNodeOKAY,
	(APTR)GetPropertyName,
	(APTR)GetPropertyStringValue,
	(APTR)GetFirstPropertyValue,
	(APTR)GetNextPropertyValue,
	(APTR)GetValueSize,
	(APTR)GetValueType,
	(APTR)GetValue,
/* End of custom library functions */
   (APTR)-1
};


static const struct _initdata
{
	SMALLINITBYTEDEF(type);
	SMALLINITPINTDEF(name);
	SMALLINITBYTEDEF(flags);
	SMALLINITWORDDEF(version);
	SMALLINITWORDDEF(revision);
	SMALLINITPINTDEF(id_string);
	INITENDDEF;
} init_data =
{
#ifdef AS_LIBRARY
	SMALLINITBYTE(OFFSET(Node, ln_Type), NT_LIBRARY),
#else
	SMALLINITBYTE(OFFSET(Node, ln_Type), NT_DEVICE),
#endif
	SMALLINITPINT(OFFSET(Node, ln_Name), library_name),
	SMALLINITBYTE(OFFSET(Library, lib_Flags), LIBF_SUMUSED|LIBF_CHANGED),
	SMALLINITWORD(OFFSET(Library, lib_Version), LIBDEVMAJOR),
	SMALLINITWORD(OFFSET(Library, lib_Revision), LIBDEVMINOR),
	SMALLINITPINT(OFFSET(Library, lib_IdString), version_string),
	INITEND
};


const APTR init_table[] =
{
	(APTR)sizeof(struct LibDevBase),
	(APTR)vectors,
	(APTR)&init_data,
	(APTR)library_init
};

static struct LibDevBase* __saveds __asm library_init(register __d0 struct LibDevBase *lib_base, register __a0 APTR seg_list, register __a6 struct LibDevBase *base)
{
	lib_base->sys_base = (APTR)base;
	base = lib_base;
	base->seg_list = seg_list;
	
	base = libdev_initalise(base);	// External function

	return base;
}
#ifdef AS_LIBRARY
static struct LibDevBase* __saveds __asm library_open(register __a6 struct LibDevBase *base)
{	
	if (!libdev_library_open(base)){
		return NULL;
	}
	
	base->device.dd_Library.lib_OpenCnt++;
	base->device.dd_Library.lib_Flags &= ~LIBF_DELEXP;

	return base;
}
#else
static int __saveds __asm device_open(register __a6 struct LibDevBase *base, register __d0 ULONG unit, register __d1 ULONG flags, register __a1 struct IORequest *ior)
{
	int ret = 0;
	
	ret = libdev_device_open(base, unit, flags, ior);
	if (ret > 0){
		return ret ;
	}
	base->device.dd_Library.lib_OpenCnt++;
	base->device.dd_Library.lib_Flags &= ~LIBF_DELEXP;

	return 0;
}
#endif

static APTR __saveds __asm library_close(register __a6 struct LibDevBase *base)
{
	APTR seg_list = NULL;

	/* Expunge the library if a delayed expunge is pending */

	if((--base->device.dd_Library.lib_OpenCnt) == 0)
	{
		if((base->device.dd_Library.lib_Flags & LIBF_DELEXP) != 0){
			seg_list = library_expunge(base);
		}
	}

	return seg_list;
}

static APTR __saveds __asm library_expunge(register __a6 struct LibDevBase *base)
{
	APTR seg_list;

	if(base->device.dd_Library.lib_OpenCnt == 0)
	{
		libdev_cleanup(base);
		seg_list = base->seg_list;
		Remove((APTR)base);
		DeleteLibrary(base);
	}
	else
	{
		base->device.dd_Library.lib_Flags |= LIBF_DELEXP;
		seg_list = NULL;
	}

	return seg_list;
}

static void DeleteLibrary(struct LibDevBase *base)
{
   UWORD neg_size, pos_size;

   /* Free library's memory */

   neg_size = base->device.dd_Library.lib_NegSize;
   pos_size = base->device.dd_Library.lib_PosSize;
   FreeMem((UBYTE *)base - neg_size, pos_size + neg_size);

   return;
}
