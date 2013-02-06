# Include common prelude make file
include $(ROOT_DIR)base.mak

source: source_cpp source_c source_h
	echo "Copy platform specific kernel source"
	
source_cpp: ./$(VARIANT)/$(TOOLCHAIN)/$(wildcard *.cpp)
	$(COPYCMD) ./$(VARIANT)/$(TOOLCHAIN)/*.cpp $(SRC_DIR)

source_c: ./$(VARIANT)/$(TOOLCHAIN)/$(wildcard *.c)
	$(COPYCMD) ./$(VARIANT)/$(TOOLCHAIN)/*.c $(SRC_DIR)

source_h: ./$(VARIANT)/$(TOOLCHAIN)/$(wildcard *.h)
	$(COPYCMD) ./$(VARIANT)/$(TOOLCHAIN)/*.h $(SRC_DIR)

headers: ./$(VARIANT)/$(TOOLCHAIN)/public/$(wildcard *.h)
	$(COPYCMD) ./$(VARIANT)/$(TOOLCHAIN)/public/*.h $(SRC_DIR)
	$(COPYCMD) ./$(VARIANT)/$(TOOLCHAIN)/public/*.h $(INC_DIR)
	