APP = periodic-tests

CFLAGS += -I../periodic

include $(SDK_DIR)/Makefile.defs

OBJS = $(ASSETS).gen.o ../periodic/Element.o ../periodic/periodic.o main.o Test.o
ASSETDEPS += *.png $(ASSETS).lua
ASSETS_BUILD_PROOF := yes

# Test steps
OBJS += TestStep_ElementBasic.o
OBJS += TestStep_Strcmp.o
OBJS += TestStep_Bonds.o
OBJS += TestStep_ObjectPool.o

include $(SDK_DIR)/Makefile.rules
