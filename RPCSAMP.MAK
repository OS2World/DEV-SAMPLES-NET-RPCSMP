#===================================================================
#
#   rpcsamp Make file
#   Copyright 1992 IBM Corporation
#
#===================================================================

#===================================================================
#
#   Sample application makefile,common definitions for the IBM C
#   compiler environment
#===================================================================
.SUFFIXES:
.SUFFIXES: .rc .res .obj .lst .c .asm .hlp .itl .ipf
#===================================================================
# Default compilation macros for sample programs
#
# Compile switchs  that are enabled
# /c      compile don't link
# /Gm+    use the multi-threaded libraries
# /ss     allow  "//" for comment lines
# /Ms     use the system calling convention and not optilink as the default
# /Gd-    Disable optimization
# /Se     allow cset  extensions
#
#



AFLAGS  = /Mx -t -z
ASM     = ml /c /Zm
LFLAGS   = /NOE /NOD /ALIGN:16 /EXEPACK /M /BASE:0x10000
LINK    = LINK386  $(LFLAGS)
LIBS    = DDE4MBS + OS2386
STLIBS  = DDE4SBS + OS2386
MTLIBS  = DDE4MBS + os2386
DLLLIBS = DDE4NBS + os2386
VLIBS   = DDE4SBS + vdh + os2386

.c.lst:
    $(CC) -Fc$*.lst -Fo$*.obj $*.c

.c.obj:
    $(CC) -Fo$*.obj $*.c

.asm.obj:
    $(ASM)   $*.asm

.ipf.hlp:
        ipfc $*.ipf /W3

.itl.hlp:
        cc  -P $*.itl
        ipfc $*.i
        del $*.i

.rc.res:
        rc -r -p -x $*.rc

CC         = icc /c /Ge /Gd- /Se /Re /ss /Gm+ /Ti /W3 /Kb+ /Gt+
# note use of the /Gt+ flag above -- required as we are calling a 16 bit DLL

all: rgc.exe rgs.exe

rgs.obj: rgs.c rg.h

rgc.obj: rgc.c rg.h

rgus.obj: rgus.c rg.h

rguc.obj: rguc.c rg.h

rgs.exe: rgs.obj rgus.obj
  $(LINK) rgs rgus /NOI /CO /MAP,,,$(LIBS) rpcdll.lib tcpipdll.lib;

rgc.exe: rgc.obj rguc.obj
  $(LINK) rgc rguc /NOI /CO /MAP,,,$(LIBS) rpcdll.lib tcpipdll.lib;

