# Version 
MAJOR = 1
MINOR = 0
DATE = "14.06.2025"
LIBNAME = dts.library

DEFSTD = DEFINE=AS_LIBRARY DEFINE=LIBDEVMAJOR=$(MAJOR) DEFINE=LIBDEVMINOR=$(MINOR) DEFINE=LIBDEVDATE=$(DATE) DEFINE=LIBDEVNAME="$(LIBNAME)"

# Test config
#SCOPTS = IncludeDirectory=/Include/C NOSTACKCHECK DEFINE=_DEBUG DEFINE=DEBUG_SERIAL debug=full $(DEFSTD) 
# DEBUG=SYMBOLFLUSH

# Release config with debug
#SCOPTS = IncludeDirectory=/Include/C NOSTACKCHECK OPTIMIZE Optimizerinline OptimizerComplexity=10 OptimizerGlobal OptimizerDepth=1 OptimizerTime OptimizerSchedule OptimizerPeephole PARAMETERS=stack DEFINE=DEBUG_SERIAL DEFINE=_DEBUG $(DEFSTD)

# optimised and release version
SCOPTS = IncludeDirectory=/Include/C NOSTACKCHECK OPTIMIZE Optimizerinline OptimizerComplexity=10 OptimizerGlobal OptimizerDepth=1 OptimizerTime OptimizerSchedule OptimizerPeephole PARAMETERS=stack $(DEFSTD)


all: dtstest dtslibrary $(LIBNAME)

clean:
   delete \#?.o \#?.lnk \#?.map \#?.gst \#?.asm \#?.info dtstest dtslibrary $(LIBNAME)

$(LIBNAME): libdev.o devicetreeconfig.o devtreelib.o
	sc link to $(LIBNAME) libdev.o devicetreeconfig.o devtreelib.o
	
dtstest: devicetreeconfig.o test.o
	sc link to dtstest devicetreeconfig.o test.o
	
dtslibrary: devicetreeconfig.o libtest.o
	sc link to dtslibrary libtest.o devicetreeconfig.o
	
libtest.o: libtest.c
	sc $(SCOPTS) libtest.c

libdev.o: libdev.c libdev.h
    sc $(SCOPTS) libdev.c

devtreelib.o: devtreelib.c devtreelib.h
    sc $(SCOPTS) devtreelib.c

devicetreeconfig.o: devicetreeconfig.c devicetreeconfig.h
   sc $(SCOPTS) devicetreeconfig.c
   
test.o: test.c 
   sc $(SCOPTS) test.c
