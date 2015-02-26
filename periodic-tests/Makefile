APP = periodic-tests

CFLAGS += -I../periodic

include $(SDK_DIR)/Makefile.defs

OBJS = $(ASSETS).gen.o main.o Test.o
ASSETDEPS += *.png $(ASSETS).lua
ASSETS_BUILD_PROOF := yes

# Files needed for testing
OBJS += ../periodic/Element.o
OBJS += ../periodic/periodic.o
OBJS += ../periodic/Bond.o
OBJS += ../periodic/BondSolution.o

# Test steps
OBJS += TestStep_ElementBasic.o
OBJS += TestStep_Strcmp.o
OBJS += TestStep_Bonds.o
OBJS += TestStep_ObjectPool.o

include $(SDK_DIR)/Makefile.rules
