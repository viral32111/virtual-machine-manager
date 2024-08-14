# Metadata
VMM_MAJOR_VERSION = 0
VMM_MINOR_VERSION = 3
VMM_PATCH_VERSION = 0

# Compiler & linker options
CC = gcc
CFLAGS = -DVMM_MAJOR_VERSION=$(VMM_MAJOR_VERSION) -DVMM_MINOR_VERSION=$(VMM_MINOR_VERSION) -DVMM_PATCH_VERSION=$(VMM_PATCH_VERSION)
LDFLAGS =
SOURCE = source/main.c

# File & directory names
VERSION = $(VMM_MAJOR_VERSION).$(VMM_MINOR_VERSION).$(VMM_PATCH_VERSION)
BIN_NAME = vmm-$(VERSION)
SYMLINK_NAME = vmm
BUILD_DIR = build

# Install in /usr/local if root, otherwise $HOME/.local
ifeq ($(shell id -u), 0)
	PREFIX ?= /usr/local
else
	PREFIX ?= $(HOME)/.local
endif
BINDIR = $(PREFIX)/bin

# Configuration-dependant variables
CFLAGS_DEBUG = -nodefaultlibs -lc -Wall -Wextra -Wundef -Wformat-security -g -DDEBUG
CFLAGS_RELEASE = -nodefaultlibs -lc -Wall -Wextra -Werror -Wundef -Wformat-security -O2

# Targets
.PHONY: all clean install uninstall run debug release watch strip dump

# Default to release
all: release

# Build the target
$(BUILD_DIR)/release/$(BIN_NAME) $(BUILD_DIR)/debug/$(BIN_NAME): $(SOURCE)
	mkdir -v -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $(SOURCE) $(LDFLAGS)
	ln -v -s -f $(notdir $@) $(dir $@)$(SYMLINK_NAME)
	@echo

# Release configuration
release: CFLAGS += $(CFLAGS_RELEASE)
release: BUILD_DIR := $(BUILD_DIR)/release
release: strip
release: BIN_PATH := $(BUILD_DIR)/$(BIN_NAME)
release: dump

# Remove symbols & debug information
strip: $(BUILD_DIR)/release/$(BIN_NAME)
	strip $<
	@echo

# Debug configuration
debug: CFLAGS += $(CFLAGS_DEBUG)
debug: BUILD_DIR := $(BUILD_DIR)/debug
debug: $(BUILD_DIR)/debug/$(BIN_NAME)
debug: BIN_PATH := $(BUILD_DIR)/$(BIN_NAME)
debug: dump

# Print information about the binary
dump: $(BIN_PATH)
	ldd $(BIN_PATH)
	du -h $(BIN_PATH)
	stat $(BIN_PATH)
	file $(BIN_PATH)
	md5sum $(BIN_PATH)
	sha1sum $(BIN_PATH)
	sha256sum $(BIN_PATH)
	sha512sum $(BIN_PATH)
	@echo

# Watch for changes in the source directory
watch:
	@while inotifywait -r -e modify,create,delete,move source/; do \
		make clean; \
		make debug; \
		make run; \
	done

# Launch the debug binary
run:
	$(BUILD_DIR)/debug/$(BIN_NAME) $(filter-out $@,$(MAKECMDGOALS)) $(ARGS)

# Install the release binary (does not build)
install:
	mkdir -v -p $(DESTDIR)$(BINDIR)
	install -m 755 $(BUILD_DIR)/release/$(BIN_NAME) $(DESTDIR)$(BINDIR)
	ln -v -s -f $(DESTDIR)$(BINDIR)/$(BIN_NAME) $(DESTDIR)$(BINDIR)/$(SYMLINK_NAME)
	@echo

# Uninstall the release binary
uninstall:
	rm -v -f $(DESTDIR)$(BINDIR)/$(BIN_NAME)
	rm -v -f $(DESTDIR)$(BINDIR)/$(SYMLINK_NAME)
	@echo

# Clean-up
clean:
	rm -v -r -f $(BUILD_DIR)
	@echo

# Prevent interpreting arguments destined for run as targets
%:
	@:
