###############################################################################
# Default target as it is the very first in the makefile.
# Note: assure to have no target specified before including THIS file.
###############################################################################
all: $(LIB) $(APP)
	@echo "Compiling $< ..."

###############################################################################
# Rule how to convert *.c files into object files
###############################################################################
%.o: %.c
	@echo "Compiling $< ..."
ifeq ($(VERBOSE),1)
	@echo $(CC) -Wall -Wextra -std=gnu99 $(CFLAGS) -I$(TOPDIR)/include -c $< -o $@
endif
	@$(CC) -Wall -Wextra -std=gnu99 $(CFLAGS) -I$(TOPDIR)/include -c $< -o $@

###############################################################################
# Rule how to convert objects into library
###############################################################################
$(LIB): $(OBJECTS)
	@echo "Creating  $@ ..."
ifeq ($(VERBOSE),1)
	@echo $(AR) cr $(TOPDIR)/$@ $^
endif
	@$(AR) cr $(TOPDIR)/$@ $^

###############################################################################
# Rule how to convert objects into library
###############################################################################
$(APP): $(OBJECTS)
	@echo "Creating  $@ ..."
ifeq ($(VERBOSE),1)
	@echo $(CC) -o $(TOPDIR)/$@ $(OBJECTS) -L$(TOPDIR) $(addprefix -l,$(LIBS_DIR)) -lm
endif
	@$(CC) -o $(TOPDIR)/$@ $(OBJECTS) -L$(TOPDIR) $(addprefix -l,$(LIBS_DIR)) -lm

###############################################################################
# Clean target
###############################################################################
clean:
	@echo "Clean $(MODULNAME) in $@ ..."
	@$(RM) -rf $(OBJECTS) depends.mk

###############################################################################
# Dependency tracking
###############################################################################
-include depends.mk
depends.mk:
	@$(CC) $(CFLAGS) -MM -I$(TOPDIR)/include $(SOURCES) >> $@

###############################################################################
# Phony targets
###############################################################################
.PHONY: $(LIB) clean all

