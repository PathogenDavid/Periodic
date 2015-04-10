APP = periodic

include $(SDK_DIR)/Makefile.defs

OBJS = $(ASSETS).gen.o main.o coders_crux.gen.o number_font.o Element.o ElementCube.o periodic.o Reaction.o Reaction.Process.o Bond.o BondSolution.o Compound.o LinkedList.o
ASSETDEPS += *.png $(ASSETS).lua
CDEPS += coders_crux.gen.cpp

# build assets.html to proof stir-processed assets.
# comment out to disable.
ASSETS_BUILD_PROOF := yes

include $(SDK_DIR)/Makefile.rules

# FB32 Font Generation
MSBUILD=C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe
FONTGEN=../FontGen/bin/Debug/FontGen.exe

$(FONTGEN):
	@echo Building FontGen...
	@$(MSBUILD) ../FontGen/FontGen.csproj

coders_crux.gen.cpp: coders_crux.png $(FONTGEN)
	@echo Processing coders_crux font...
	@$(FONTGEN) coders_crux.png

GENERATED_FILES += coders_crux.gen.cpp coders_crux.gen.h ../FontGen/bin/Debug/* ../FontGen/obj/*
