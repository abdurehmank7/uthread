# run `make init` to initialize project
# run `make` to build the project, using default mode specified in the makefile. can either be release or debug. by default it's debug. Make sure you have run `make init` before running this command.
# run `make debug` to build the project in debug mode.
# run `make release` to build the project in release mode.
# run `make run` to run the executable

# Set the Default build. Set to `release` to get the release build (optimized binary without debug statements)
BUILD_DEFAULT = release

# Directories
BUILD_DIR=build
SRC_DIR=src
INCLUDE_DIR=include
TEST_DIR=test
SEMAPHORE_DIR=$(TEST_DIR)/semaphore
MUTEX_DIR=$(TEST_DIR)/mutex
THREAD_DIR=$(TEST_DIR)/thread
DOCTOR_DIR=$(TEST_DIR)/doctor
LAUNDRY_DIR=$(TEST_DIR)/laundry
APP_DIR=app

TEST_PROGRAMS := $(wildcard $(TEST_PROGRAMS_DIR)/*.c)
TEST_PROGRAMS_BINS := $(patsubst $(TEST_PROGRAMS_DIR)/%.c, $(BUILD_DIR)/%.test.out, $(TEST_PROGRAMS))

# Target executable
TARGET_NAME=libthread.a
TARGET=$(BUILD_DIR)/$(TARGET_NAME)

# Shell Commands
CC=gcc
AR=ar rcs
MKDIR=mkdir -p
RM=rm -rf
CP=cp

# useful utility to convert lowercase to uppercase. 
UPPERCASE_CMD = tr '[:lower:][\-/]' '[:upper:][__]'

# Flags for compiler and other programs
# CFLAGS=-Wall -Wextra -Werror
VALG_FLAGS = --leak-check=full --track-origins=yes
DEBUG_FLAGS = -g -DDEBUG
RELEASE_FLAGS = -march=native 
LINKER_FLAGS =

# Color codes for print statements
GREEN = \033[1;32m
CYAN = \033[1;36m
RED = \033[1;31m
RESET = \033[0m

# Verbosity control. Inspired from the Contiki-NG build system. A few hacks here and there, will probably improve later.
ifeq ($(V),1)
  TRACE_CC =
  TRACE_LD =
  TRACE_MKDIR =
  TRACE_CP =
	TRACE_AR =
	TRACE_RUN =
  Q ?=
  
  BUILD_SUCCESS=
  BUILD_FAILURE=:
  LINK_FAILURE=:
  INIT_SUCCESS=
  INIT_MAIN=
  RUN=
  VALGRIND_RUN=
  
  CLEAN=
  MK_INIT_ERROR=
else

  TRACE_CC       = @echo "$(CYAN)   CC     $(RESET)" $<
  TRACE_LD       = @echo "$(CYAN)   LD     $(RESET)" $@
	TRACE_AR 		   = @echo "$(CYAN)   AR     $(RESET)" $@
  TRACE_MKDIR    = @echo "$(CYAN)   MKDIR  $(RESET)" $@
  TRACE_CP       = @echo "$(CYAN)   CP     $(RESET)" $< "-->" $@
	TRACE_RUN      = @echo "$(GREEN)   RUN    $(RESET)" $<
  Q ?= @
  
  BUILD_SUCCESS  =@echo "   --      $(GREEN)Build successful in $(BUILD_DEFAULT) mode.$(RESET)"
  BUILD_FAILURE  =echo  "   --      $(RED)Build failed.$(RESET)"; exit 1
  LINK_FAILURE   =echo  "   --      $(RED)Linking failed.$(RESET)"; exit 1
  INIT_MAIN      =@echo "   --      $(CYAN)Creating main.c$(RESET)"
  INIT_SUCCESS   =@echo "   --      $(GREEN)Initialized the project structure$(RESET)"
  RUN            =@echo "   --      $(CYAN)Executing$(RESET): $(TARGET_NAME)"
  VALGRIND_RUN   =@echo "   --      $(CYAN)Running Valgrind on$(RESET): $(TARGET_NAME)"
  CLEAN          =@echo "   --      $(GREEN)Cleaned$(RESET): $(BUILD_DIR)/*"
  
  MK_INIT_ERROR  =@echo "$(RED)Error: $(SRC_DIR) directory doesn't exist. Please run make init to initialize the project.$(RESET)"
endif

# Sets flags based on the build mode.
ifeq ($(BUILD_DEFAULT), release)
  CFLAGS += $(RELEASE_FLAGS)
else
  CFLAGS += $(DEBUG_FLAGS)
endif

# Find all the source files and corresponding objects
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

SRC_DIR_EXISTS := $(shell if [ -d "$(SRC_DIR)" ]; then echo 1; else echo 0; fi)

# Checks if src directory exists. If it doesn't, probably they haven't run `make init` yet.
ifeq ($(SRC_DIR_EXISTS), 0)
all: 
	$(MK_INIT_ERROR)
else
all: $(TARGET)
endif

# The TARGET target depends on the generated object files. 
$(TARGET): $(OBJS)
	$(TRACE_AR)
	$(Q) $(AR) $@ $^ || ($(LINK_FAILURE))
	$(BUILD_SUCCESS)

# The object files' targets, depend on their corresponding source files.
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(TRACE_CC)
	$(Q) $(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@ || ($(BUILD_FAILURE))

# Create the build, src and include directories if they don't exist.
$(BUILD_DIR) $(SRC_DIR) $(INCLUDE_DIR):
	$(TRACE_MKDIR)
	$(Q) $(MKDIR) $@

# Initializes the project directories, and creates a main.c file in the src directory.
init: $(BUILD_DIR) $(SRC_DIR) $(INCLUDE_DIR)
	$(INIT_SUCCESS)


ARGS=
DRIVER_C_FLAGS= -march=native 

driver: $(BUILD_DIR)/driver.out
	$(Q) $(TRACE_RUN)
	$(Q) $(BUILD_DIR)/driver.out $(ARGS)

$(BUILD_DIR)/driver.out: $(TEST_DIR)/driver.c $(TARGET)
	$(TRACE_CC)
	$(Q) $(CC) $(DRIVER_C_FLAGS) -I$(INCLUDE_DIR) $< -o $@ -L$(BUILD_DIR) -lthread

laundry: $(LAUNDRY_DIR)/build/laundry.out

$(LAUNDRY_DIR)/build/laundry.out: $(APP_DIR)/laundry.c $(TARGET)
	$(TRACE_CC)
	$(Q) $(CC) $(DRIVER_C_FLAGS) -I$(INCLUDE_DIR) $< -o $@ -L$(BUILD_DIR) -lthread

doctor: $(DOCTOR_DIR)/build/doctor.out

$(DOCTOR_DIR)/build/doctor.out: $(APP_DIR)/doctor.c $(TARGET)
	$(TRACE_CC)
	$(Q) $(CC) $(DRIVER_C_FLAGS) -I$(INCLUDE_DIR) $< -o $@ -L$(BUILD_DIR) -lthread

MUTEX_PROGRAMS := $(wildcard $(MUTEX_DIR)/*.c)
MUTEX_PROGRAMS_BINS := $(patsubst $(MUTEX_DIR)/%.c, $(MUTEX_DIR)/build/%.out, $(MUTEX_PROGRAMS))

mutex: $(TARGET) $(MUTEX_PROGRAMS_BINS)

$(MUTEX_DIR)/build/%.out: $(MUTEX_DIR)/%.c
	$(TRACE_CC)
	$(Q) $(CC) $(CFLAGS) -I$(INCLUDE_DIR) $< -o $@ -L$(BUILD_DIR) -lthread || ($(BUILD_FAILURE))


SEMAPHORE_PROGRAMS := $(wildcard $(SEMAPHORE_DIR)/*.c)
SEMAPHORE_PROGRAMS_BINS := $(patsubst $(SEMAPHORE_DIR)/%.c, $(SEMAPHORE_DIR)/build/%.out, $(SEMAPHORE_PROGRAMS))

semaphore: $(TARGET) $(SEMAPHORE_PROGRAMS_BINS)

$(SEMAPHORE_DIR)/build/%.out: $(SEMAPHORE_DIR)/%.c
	$(TRACE_CC)
	$(Q) $(CC) $(CFLAGS) -I$(INCLUDE_DIR) $< -o $@ -L$(BUILD_DIR) -lthread || ($(BUILD_FAILURE))

THREAD_PROGRAMS := $(wildcard $(THREAD_DIR)/*.c)
THREAD_PROGRAMS_BINS := $(patsubst $(THREAD_DIR)/%.c, $(THREAD_DIR)/build/%.out, $(THREAD_PROGRAMS))

# build an executable for each C file in the thread directory
thread: $(TARGET) $(THREAD_PROGRAMS_BINS)

$(THREAD_DIR)/build/%.out: $(THREAD_DIR)/%.c
	$(TRACE_CC)
	$(Q) $(CC) $(CFLAGS) -I$(INCLUDE_DIR) $< -o $@ -L$(BUILD_DIR) -lthread || ($(BUILD_FAILURE))


# Cleans the build directory.
clean:
	$(Q) $(RM) $(BUILD_DIR)/*
	$(Q) $(RM) $(MUTEX_DIR)/build/*
	$(Q) $(RM) $(SEMAPHORE_DIR)/build/*
	$(Q) $(RM) $(THREAD_DIR)/build/*
	$(Q) $(RM) $(DOCTOR_DIR)/build/*
	$(Q) $(RM) $(LAUNDRY_DIR)/build/*
	$(CLEAN)

# phony targets
.PHONY: all init run debug release valgrind clean