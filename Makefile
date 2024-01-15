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

# Install directories
ifeq ( $(shell id -u), 0 )
	PREFIX ?= /usr/local
else
	PREFIX ?= $(HOME)/.local
endif
BINDIR = $(PREFIX)/bin

# Configuration-dependant variables
CFLAGS_DEBUG = -Wall -Wextra -Wundef -Wformat-security -g
CFLAGS_RELEASE = -Wall -Wextra -Werror -Wundef -Wformat-security -O2

.PHONY: all clean install uninstall run debug release

# Default to release
all: release

# Build the target
$(BUILD_DIR)/release/$(BIN_NAME) $(BUILD_DIR)/debug/$(BIN_NAME): $(SOURCE)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $(SOURCE) $(LDFLAGS)
	ln -s -f $(notdir $@) $(dir $@)$(SYMLINK_NAME)

release: CFLAGS += $(CFLAGS_RELEASE)
release: BUILD_DIR := $(BUILD_DIR)/release
release: $(BUILD_DIR)/release/$(BIN_NAME)

debug: CFLAGS += $(CFLAGS_DEBUG)
debug: BUILD_DIR := $(BUILD_DIR)/debug
debug: $(BUILD_DIR)/debug/$(BIN_NAME)

# Launch the debug binary
run:
	$(BUILD_DIR)/debug/$(SYMLINK_NAME)

# Install the release binary (does not build)
install:
	mkdir -p $(DESTDIR)$(BINDIR)
	install -m 755 $(BUILD_DIR)/release/$(BIN_NAME) $(DESTDIR)$(BINDIR)
	ln -s -f $(DESTDIR)$(BINDIR)/$(BIN_NAME) $(DESTDIR)$(BINDIR)/$(SYMLINK_NAME)

# Uninstall the release binary
uninstall:
	rm -f $(DESTDIR)$(BINDIR)/$(BIN_NAME)
	rm -f $(DESTDIR)$(BINDIR)/$(SYMLINK_NAME)

# Clean-up
clean:
	rm -r $(BUILD_DIR)
