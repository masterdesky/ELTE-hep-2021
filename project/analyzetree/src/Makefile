#all codes ending on .cc will be attempted to compile with this Makefile
PROGRAMS = $(basename $(shell ls *.cc))

#At zeroth order, the user does not have to below this line.
WrkDir := $(shell pwd)
SrcDir = .
BinDir = ../bin
ObjDir = $(BinDir)/object
DepDir = $(ObjDir)/dependencies

CXX     = clang++
LD      = clang++
ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs) -lMinuit
CFLAGS  = -O3 -Wall -fPIC -fno-inline $(ROOTCFLAGS) -I$(WrkDir)/$(RdrDir)
LDFLAGS = -O3 #-m32 # beware -m32 may have to be deleted/added

COMMON_SOURCES = particle_tree.C
COMMON_OBJECTS = $(addprefix $(ObjDir)/, $(addsuffix .o,$(notdir $(basename $(COMMON_SOURCES)))))
SOURCES = $(addprefix $(SrcDir)/,$(addsuffix .cc,$(PROGRAMS))) $(COMMON_SOURCES)
ALL_SOURCES = $(sort $(SOURCES))

define COMPILE_TEMPLATE
-include $(DepDir)/$(notdir $(basename $(1))).d
$(ObjDir)/$(notdir $(basename $(1))).o:
	@echo ""
	$(CXX) $(CFLAGS) -c -MD -MP -MF $(DepDir)/$(notdir $(basename $(1))).d $(1) -o $$@
endef
$(foreach source, $(ALL_SOURCES), $(eval $(call COMPILE_TEMPLATE,$(source))))

define FINAL_LINK_TEMPLATE
$(BinDir)/$(1): $(2) $(ObjDir)/$(1).o
	@echo ""
	$(LD) $(LDFLAGS) $$^ $(ROOTLIBS) $(SYSLIBS) -o $$@
endef
$(foreach program, $(PROGRAMS), $(eval $(call FINAL_LINK_TEMPLATE,$(program),$(COMMON_OBJECTS))))

#all: $(addprefix $(ExeDir)/,$(addsuffix .exe, $(PROGRAMS)))

clean:
	@rm -f $(BinDir)/* $(ObjDir)/*.o $(DepDir)/*.d
