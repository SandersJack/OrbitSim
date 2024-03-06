.PHONY: all clean

BUILDDIR=build

all: $(BUILDDIR)
	+@make -C $(BUILDDIR) install --no-print-directory $(MAKEFLAGS)


$(BUILDDIR): 
	@cmake -H. -B$(BUILDDIR) $(CMAKE_FLAGS) ${ARGS}


clean: $(BUILDDIR)
	rm -rf $(BUILDDIR)

