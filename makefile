
all: includes install

clean:
   delete Include/C/pragma/\#? Include/C/inline/\#? Include/C/proto/\#? Include/C/inline/\#? Include/Assembly/lvo/\#? FD/\#?
   
includes: SFD/dts_lib.sfd
	fd2pragma "SFD/dts_lib.sfd" 111 TO "Include/C/clib" AUTOHEADER
	fd2pragma "SFD/dts_lib.sfd" 110 TO "FD"
	fd2pragma "SFD/dts_lib.sfd" 6 TO "Include/C/pragma"
	fd2pragma "SFD/dts_lib.sfd" 40 TO "Include/C/inline"
	fd2pragma "SFD/dts_lib.sfd" 38 TO "Include/C/proto"
	fd2pragma "SFD/dts_lib.sfd" 70 TO "Include/C/inline"
	fd2pragma "SFD/dts_lib.sfd" 23 TO "Include/Assembly/lvo"
	
install:
	copy DTS/\#? ENVARC: