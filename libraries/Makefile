###############################################################################
# generall config
###############################################################################
APPL_NAME = testcases.exe

OS ?= LINUX

CONFIG ?= debug
export CONFIG

###############################################################################
# root folder
###############################################################################
TOPDIR = $(shell $(PWD))
export TOPDIR

###############################################################################
# toolchain to use
###############################################################################
include $(wildcard build_configs/$(OS)/toolchain.mk)

###############################################################################
# operating system configuration
###############################################################################
include $(wildcard build_configs/$(OS)/os.mk)

###############################################################################
# common 
###############################################################################
include $(wildcard build_configs/configs.mk)

###############################################################################
# units to build
###############################################################################
include $(wildcard build_configs/units.mk)

###############################################################################
# all (default target)
###############################################################################
all: build_info $(patsubst %,call_make_%,$(LIBS_DIR)) $(patsubst %,call_make_%,$(APP_DIR))

###############################################################################
# build_info (dump a build environment info)
###############################################################################
build_info:
	@echo "------------------------------------------------------------"
	@echo "build-environment"
	@echo "operating system : $(OS)"
	@echo "compiler         : $(CC)"
	@echo "linker           : $(LD)"
	@echo "remove command   : $(RM)"
	@echo "path command     : $(PWD)"
	@echo "------------------------------------------------------------"
	@echo "CFLAGS           : $(CFLAGS)"
	@echo "------------------------------------------------------------"

###############################################################################
# clean
###############################################################################
clean: $(patsubst %,call_make_clean_%,$(LIBS_DIR)) $(patsubst %,call_make_clean_%,$(APP_DIR))
	@$(RM) -rf $(addsuffix .exe,$(APP_DIR))
	@$(RM) -rf $(addprefix lib,$(addsuffix .a,$(LIBS_DIR)))

distclean: clean
	@$(RM) -rf cscope.*

###############################################################################
# index
###############################################################################
index:
	@find $(TOPDIR) -name "*.[chS]" -type f > cscope.files
	@cscope -b

###############################################################################
# NOTE: we do a nice trick here to iterate through all lib directories
###############################################################################
define call_make_target
call_make_$(1):
	@echo "Build $(1)"
	@$(MAKE) -C $(1) all
endef

define call_make_target_clean
call_make_clean_$(1):
	@$(MAKE) -C $(1) clean
endef

define call_make_target_lint
call_make_lint_$(1):
	$(MAKE) -C $(1) lint
endef

$(foreach lib,$(LIBS_DIR),$(eval $(call call_make_target,$(lib))))
$(foreach lib,$(LIBS_DIR),$(eval $(call call_make_target_clean,$(lib))))
$(foreach lib,$(LIBS_DIR),$(eval $(call call_make_target_lint,$(lib))))

$(foreach app,$(APP_DIR),$(eval $(call call_make_target,$(app))))
$(foreach app,$(APP_DIR),$(eval $(call call_make_target_clean,$(app))))
$(foreach app,$(APP_DIR),$(eval $(call call_make_target_lint,$(app))))

.PHONY: all index clean

