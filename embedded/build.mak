#----------------------------------------------------------------------------
# Mark3 Makefile system
# Copyright (c) 2012 Funkenstein Software Consulting
#----------------------------------------------------------------------------

# The user specifies objects in the OBJ variable
OBJ_DIR_FINAL=$(OBJ_DIR)$(ARCH)/$(VARIANT)/$(TOOLCHAIN)/
APP_DIR_FINAL=$(APP_DIR)$(ARCH)/$(VARIANT)/$(TOOLCHAIN)/
LIB_DIR_FINAL=$(LIB_DIR)$(ARCH)/$(VARIANT)/$(TOOLCHAIN)/
DBG_DIR_FINAL=$(DBG_DIR)$(ARCH)/$(VARIANT)/$(TOOLCHAIN)/

C_OBJ=$(addprefix $(OBJ_DIR_FINAL), $(C_SOURCE:%.c=%.c.o))
CPP_OBJ=$(addprefix $(OBJ_DIR_FINAL), $(CPP_SOURCE:%.cpp=%.cpp.o))

PORT_DIR=./cpu/$(ARCH)/$(VARIANT)/$(TOOLCHAIN)/
PORT_OBJ_DIR=$(PORT_DIR)obj/

PORT_C_SOURCE_ADJ=$(addprefix $(PORT_DIR), $(PORT_C_SOURCE))
PORT_CPP_SOURCE_ADJ=$(addprefix $(PORT_DIR), $(PORT_CPP_SOURCE))

PORT_C_OBJ=$(addprefix $(PORT_OBJ_DIR), $(PORT_C_SOURCE:%.c=%.c.o))
PORT_CPP_OBJ=$(addprefix $(PORT_OBJ_DIR), $(PORT_CPP_SOURCE:%.cpp=%.cpp.o))

USR_OBJS=$(C_OBJ) $(CPP_OBJ) $(PORT_C_OBJ) $(PORT_CPP_OBJ)

CPU_SPEC_HEADERS=$(PORT_DIR)/public/

ifeq ($(IS_APP), 1)
	LFLAGS+=-Wl,-Map="$(APPNAME).map"
endif

# the target is specified in the user makefile
all : banner binary 

# Display what we're building
clean_banner : $(realpath .)
	@echo
	@echo ------------------------------------------------------------
	@echo [ Cleaning Build Artifacts $^ ]
	@echo ------------------------------------------------------------
	
# Display what we're building
banner : $(realpath .)
	@echo
	@echo ------------------------------------------------------------
	@echo [ Building Objects $^ ]
	@echo ------------------------------------------------------------

# Display what we're building
headers_banner : $(realpath .)
	@echo
	@echo ------------------------------------------------------------
	@echo [ Copying Public Headers $^ ]
	@echo ------------------------------------------------------------

# Display what we're building
source_banner : $(realpath .)
	@echo
	@echo ------------------------------------------------------------
	@echo [ Copying Source Files $^ ]
	@echo ------------------------------------------------------------
	
# Get rid of all of the binary objects created previous... 
clean : clean_banner
	 @$(RMCMD) $(USR_OBJS)
ifeq ($(IS_LIB),1)
	 @$(RMCMD) $(OBJ_DIR_FINAL)lib$(LIBNAME).a
endif
ifeq ($(IS_APP),1)
	 @$(RMCMD) $(OBJ_DIR_FINAL)$(APPNAME).elf
	 @$(RMCMD) $(OBJ_DIR_FINAL)$(APPNAME).hex
endif
	@echo "[DONE]"
# Recursive cleaning...
	@ for i in $(SUBDIRS); \
	do \
		if test -f $$i/makefile; then \
			cd $$i ;\
			$(MAKE) --no-print-directory clean ;\
			cd .. ;\
		fi ;\
	done

#----------------------------------------------------------------------------
# Copy public header files to the staging directory
#----------------------------------------------------------------------------
headers : headers_banner
	@if test -d $(PUBLIC_DIR); then \
		if test -d $(INC_DIR); then \
			if test -f $(wildcard *.h); then \
				$(COPYCMD) $(PUBLIC_DIR)*.h $(INC_DIR); \
			fi;	\
		fi; \
	fi;
	@if test -d $(CPU_SPEC_HEADERS); then \
		if test -d $(INC_DIR); then \
			if test -f $(wildcard *.h); then \
			    $(COPYCMD) $(CPU_SPEC_HEADERS)*.h $(INC_DIR); \
			fi;	\
		fi; \
	fi;	
	@echo "[DONE]"
	@for i in $(SUBDIRS); \
	do \
		if test -f $$i/makefile; then \
			cd $$i ;\
			$(MAKE) --no-print-directory headers ;\
			cd .. ;\
		fi; \
	done

#----------------------------------------------------------------------------
# Copy source files to the staging directory
#----------------------------------------------------------------------------
source: source_banner
ifneq ($(wildcard *.cpp), )
		@$(COPYCMD) *.cpp $(SRC_DIR)
endif
ifneq ($(wildcard *.c), )
		@$(COPYCMD) *.c $(SRC_DIR)
endif
ifneq ($(wildcard *.h), )
		@$(COPYCMD) *.h $(SRC_DIR)
endif
	@echo "[DONE]"
	@if test -d $(PUBLIC_DIR); then \
		if test -d $(INC_DIR); then \
			if test -f $(wildcard *.h); then \
				$(COPYCMD) $(PUBLIC_DIR)*.h $(SRC_DIR); \
			fi;	\
		fi; \
	fi;
	@for i in $(SUBDIRS); \
	do \
		if test -f $$i/makefile; then \
			cd $$i ;\
			$(MAKE) --no-print-directory source ;\
			cd .. ;\
		fi; \
	done
	
	
#----------------------------------------------------------------------------
# Build libraries from objects
#----------------------------------------------------------------------------
ifeq ($(IS_LIB), 1)
library : banner directories objects $(USR_OBJS)
else
library : banner
endif
ifeq ($(IS_LIB), 1)
	@echo
	@echo =====[ Creating Library: $(LIBNAME) ] =====
	@echo
	@$(AR) $(ARFLAGS) $(OBJ_DIR_FINAL)lib$(LIBNAME).a $(USR_OBJS)
 ifneq ($(wildcard $(STAGE)lib), "")
	@$(COPYCMD) $(OBJ_DIR_FINAL)lib$(LIBNAME).a $(LIB_DIR_FINAL)
 endif 
	@echo "[DONE]"
endif
	@for i in $(SUBDIRS); \
	do \
		if test -f $$i/makefile; then \
			cd $$i ;\
			$(MAKE) --no-print-directory library ;\
			cd .. ;\
		fi; \
	done

#----------------------------------------------------------------------------
# Build executables from objects
#----------------------------------------------------------------------------
ifeq ($(IS_APP), 1)
binary : banner directories objects $(USR_OBJS)
else
binary : banner
endif
ifeq ($(IS_APP), 1)
	@echo
	@echo =====[ Building Executable: $(APPNAME) ]=====
	@echo
	@$(LINK) $(LFLAGS) -o $(OBJ_DIR_FINAL)$(APPNAME).elf $(USR_OBJS) $(addsuffix .a, $(addprefix $(LIB_DIR_FINAL), $(addprefix lib, $(LIBS))))
	@$(LINK) $(LFLAGS_DBG) -o $(DBG_DIR_FINAL)$(APPNAME).elf $(USR_OBJS) $(addsuffix .a, $(addprefix $(LIB_DIR_FINAL), $(addprefix lib, $(LIBS))))
	@$(OBJCOPY) $(OBJCOPY_FLAGS) "$(OBJ_DIR_FINAL)$(APPNAME).elf" "$(OBJ_DIR_FINAL)$(APPNAME).hex" > /dev/null 2>&1
	@$(OBJCOPY) $(OBJCOPY_DBG_FLAGS) "$(DBG_DIR_FINAL)$(APPNAME).elf" "$(DBG_DIR_FINAL)$(APPNAME).dbg" > /dev/null 2>&1
	@$(RMCMD) "$(DBG_DIR_FINAL)$(APPNAME).elf"
 ifneq ($(wildcard $(STAGE)app), "")	
	@$(COPYCMD) $(OBJ_DIR_FINAL)$(APPNAME).elf $(APP_DIR_FINAL)
	@$(COPYCMD) $(OBJ_DIR_FINAL)$(APPNAME).hex $(APP_DIR_FINAL)
 endif
	@echo "[DONE]"
endif
	@for i in $(SUBDIRS); \
	do \
		if test -f $$i/makefile; then \
			cd $$i ;\
			$(MAKE) --no-print-directory binary ;\
			cd .. ;\
		fi; \
	done

#----------------------------------------------------------------------------
# Code to create object directories (only for folders with lib/app targets
#----------------------------------------------------------------------------
directories : $(OBJ_DIR_FINAL) $(APP_DIR_FINAL) $(LIB_DIR_FINAL) $(DBG_DIR_FINAL)
$(OBJ_DIR) :
	@if test ! -d $(OBJ_DIR); then \
		mkdir $(OBJ_DIR)	;\
	fi;

$(OBJ_DIR)$(ARCH) : $(OBJ_DIR)
	@if test ! -d $(OBJ_DIR)$(ARCH); then \
		mkdir $(OBJ_DIR)$(ARCH)    ;\
	fi;

$(OBJ_DIR)$(ARCH)/$(VARIANT) : $(OBJ_DIR)$(ARCH)
	@if test ! -d $(OBJ_DIR)$(ARCH)/$(VARIANT); then \
		mkdir $(OBJ_DIR)$(ARCH)/$(VARIANT)    ;\
	fi;

$(OBJ_DIR_FINAL) : $(OBJ_DIR)$(ARCH)/$(VARIANT)
	@if test ! -d $(OBJ_DIR)$(ARCH)/$(VARIANT)/$(TOOLCHAIN); then \
		mkdir $(OBJ_DIR)$(ARCH)/$(VARIANT)/$(TOOLCHAIN)    ;\
	fi;

$(APP_DIR) :
	@if test ! -d $(APP_DIR); then \
		mkdir $(APP_DIR)	;\
	fi;

$(APP_DIR)$(ARCH) : $(APP_DIR)
	@if test ! -d $(APP_DIR)$(ARCH); then \
		mkdir $(APP_DIR)$(ARCH)    ;\
	fi;

$(APP_DIR)$(ARCH)/$(VARIANT) : $(APP_DIR)$(ARCH)
	@if test ! -d $(APP_DIR)$(ARCH)/$(VARIANT); then \
		mkdir $(APP_DIR)$(ARCH)/$(VARIANT)    ;\
	fi;

$(APP_DIR_FINAL) : $(APP_DIR)$(ARCH)/$(VARIANT)
	@if test ! -d $(APP_DIR)$(ARCH)/$(VARIANT)/$(TOOLCHAIN); then \
		mkdir $(APP_DIR)$(ARCH)/$(VARIANT)/$(TOOLCHAIN)    ;\
	fi;

$(LIB_DIR) :
	@if test ! -d $(LIB_DIR); then \
		mkdir $(LIB_DIR)	;\
	fi;

$(LIB_DIR)$(ARCH) : $(LIB_DIR)
	@if test ! -d $(LIB_DIR)$(ARCH); then \
		mkdir $(LIB_DIR)$(ARCH)    ;\
	fi;

$(LIB_DIR)$(ARCH)/$(VARIANT) : $(LIB_DIR)$(ARCH)
	@if test ! -d $(LIB_DIR)$(ARCH)/$(VARIANT); then \
		mkdir $(LIB_DIR)$(ARCH)/$(VARIANT)    ;\
	fi;

$(LIB_DIR_FINAL) : $(LIB_DIR)$(ARCH)/$(VARIANT)
	@if test ! -d $(LIB_DIR)$(ARCH)/$(VARIANT)/$(TOOLCHAIN); then \
		mkdir $(LIB_DIR)$(ARCH)/$(VARIANT)/$(TOOLCHAIN)    ;\
	fi;

$(DBG_DIR) :
	@if test ! -d $(DBG_DIR); then \
		mkdir $(DBG_DIR)	;\
	fi;

$(DBG_DIR)$(ARCH) : $(DBG_DIR)
	@if test ! -d $(DBG_DIR)$(ARCH); then \
		mkdir $(DBG_DIR)$(ARCH)    ;\
	fi;

$(DBG_DIR)$(ARCH)/$(VARIANT) : $(DBG_DIR)$(ARCH)
	@if test ! -d $(DBG_DIR)$(ARCH)/$(VARIANT); then \
		mkdir $(DBG_DIR)$(ARCH)/$(VARIANT)    ;\
	fi;

$(DBG_DIR_FINAL) : $(DBG_DIR)$(ARCH)/$(VARIANT)
	@if test ! -d $(DBG_DIR)$(ARCH)/$(VARIANT)/$(TOOLCHAIN); then \
		mkdir $(DBG_DIR)$(ARCH)/$(VARIANT)/$(TOOLCHAIN)    ;\
	fi;


#----------------------------------------------------------------------------
# Rule to build C code into object files
#----------------------------------------------------------------------------
objects : directories $(C_OBJ) $(CPP_OBJ) $(PORT_C_OBJ) $(PORT_CPP_OBJ)
$(OBJ_DIR_FINAL)%.c.o : %.c
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_DIR_FINAL)%.cpp.o : %.cpp
	$(CPP) $(CPPFLAGS) $< -o $@

$(PORT_OBJ_DIR)%.c.o : $(PORT_DIR)%.c $(PORT_OBJ_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(PORT_OBJ_DIR)%.cpp.o : $(PORT_DIR)%.cpp $(PORT_OBJ_DIR)
	$(CPP) $(CPPFLAGS) $< -o $@

$(PORT_OBJ_DIR) : $(PORT_DIR)
	@if test ! -d $(PORT_OBJ_DIR); then \
		mkdir $(PORT_OBJ_DIR)	; \
	fi;

