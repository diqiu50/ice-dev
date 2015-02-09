
#//
#// Copyright (C) 2005
#// Packet Engineering, Inc. All rights reserved.
#//
#// Redistribution and use in source and binary forms, with or without
#// modification is not permitted unless authorized in writing by a duly
#// appointed officer of Packet Engineering, Inc. or its derivatives
#//
#// Description:
#//
#// Modification History:
#// 12/4/2007: Created by Ice
#//
#////////////////////////////////////////////////////////////////////////////

TARGET = Tester
TARGETEXE = $(TARGET).exe
TARGETLIB = lib$(TARGET).a
TARGETDLL = $(TARGET).dll

.SUFFIXES: .cpp .h
CC = g++
FLAGS = -g $(Optimize) -std=c++11
INCDIRS = 
LIBDIR = 
LIBS = 
DLIBS = 


OUTPUTDIR = ..\Debug
OBJDIR = $(OUTPUTDIR)\src

CREATEOBJDIR = $(shell if not exist $(OBJDIR)  mkdir $(OBJDIR))

#
# objects and targets
#

OBJECTS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(wildcard *.cpp))

#
# Rules for normal comile and link 
#
all:: $(TARGETEXE)

$(TARGETEXE): $(OBJECTS) 
	$(CC) -o $(OUTPUTDIR)/$@ $^ $(DLIBS) $(LIBS) 

$(OBJECTS): $(OBJDIR)/%.o: %.cpp
	$(CREATEOBJDIR)
	$(CC) -c $(FLAGS) $< $(INCDIRS) -o $@

lib:: $(TARGETLIB)
$(TARGETLIB): $(OBJECTS) 
	ar rvu $(OUTPUTDIR)/$(TARGETLIB) $^

dll:: $(TARGETDLL)
$(TARGETDLL): $(OBJECTS) 
	$(CC) -shared -o $(OUTPUTDIR)/$@ $^ $(DLIBS) $(LIBS) 

clean:
	del /S /Q $(OBJDIR)\*.o $(OUTPUTDIR)\*$(TARGET)*
#	rmdir $(OBJDIR)
