PROJECT_NAME     := ecc_pca10059
TARGETS          := nrf52840_xxaa
OUTPUT_DIRECTORY := build

TOOLCHAIN_PATH := /Users/chirag-parmar/arm-toolchain/bin

PROJ_DIR := .
SRC_DIR := $(PROJ_DIR)/src
INC_DIR := $(PROJ_DIR)/include
LIB_DIR := $(PROJ_DIR)/lib

$(OUTPUT_DIRECTORY)/nrf52840_xxaa.out: \
  LINKER_SCRIPT  := ./src/ecc_gcc_nrf52.ld

	# Source files common to all targets
	SRC_FILES += \
		$(wildcard $(SRC_DIR)/*.c) \
		$(wildcard $(SRC_DIR)/in3/api/eth1/*.c) \
		$(wildcard $(SRC_DIR)/in3/core/client/*.c) \
		$(wildcard $(SRC_DIR)/in3/core/util/*.c) \
		$(wildcard $(SRC_DIR)/in3/third-party/crypto/*.c) \
		$(wildcard $(SRC_DIR)/in3/third-party/tommath/*.c) \
		$(wildcard $(SRC_DIR)/in3/verifier/eth1/**/*.c) \
	  $(SDK_ROOT)/modules/nrfx/mdk/gcc_startup_nrf52840.S \
	  $(SDK_ROOT)/components/libraries/log/src/nrf_log_backend_rtt.c \
	  $(SDK_ROOT)/components/libraries/log/src/nrf_log_backend_serial.c \
	  $(SDK_ROOT)/components/libraries/log/src/nrf_log_backend_uart.c \
	  $(SDK_ROOT)/components/libraries/log/src/nrf_log_default_backends.c \
	  $(SDK_ROOT)/components/libraries/log/src/nrf_log_frontend.c \
	  $(SDK_ROOT)/components/libraries/log/src/nrf_log_str_formatter.c \
	  $(SDK_ROOT)/components/boards/boards.c \
	  $(SDK_ROOT)/components/libraries/util/app_error.c \
	  $(SDK_ROOT)/components/libraries/util/app_error_handler_gcc.c \
	  $(SDK_ROOT)/components/libraries/util/app_error_weak.c \
	  $(SDK_ROOT)/components/libraries/util/app_util_platform.c \
	  $(SDK_ROOT)/components/libraries/util/nrf_assert.c \
	  $(SDK_ROOT)/components/libraries/atomic/nrf_atomic.c \
	  $(SDK_ROOT)/components/libraries/balloc/nrf_balloc.c \
	  $(SDK_ROOT)/external/fprintf/nrf_fprintf.c \
	  $(SDK_ROOT)/external/fprintf/nrf_fprintf_format.c \
	  $(SDK_ROOT)/components/libraries/memobj/nrf_memobj.c \
	  $(SDK_ROOT)/components/libraries/ringbuf/nrf_ringbuf.c \
	  $(SDK_ROOT)/components/libraries/strerror/nrf_strerror.c \
	  $(SDK_ROOT)/integration/nrfx/legacy/nrf_drv_uart.c \
	  $(SDK_ROOT)/modules/nrfx/soc/nrfx_atomic.c \
	  $(SDK_ROOT)/modules/nrfx/drivers/src/prs/nrfx_prs.c \
	  $(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_uart.c \
	  $(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_uarte.c \
	  $(SDK_ROOT)/modules/nrfx/mdk/system_nrf52840.c \

# Include folders common to all targets
INC_FOLDERS += \
  $(SDK_ROOT)/external/fprintf \
  $(SRC_DIR) \
	$(SRC_DIR)/in3/api/eth1 \
	$(SRC_DIR)/in3/core/client \
	$(SRC_DIR)/in3/core/util \
	$(SRC_DIR)/in3/third-party/crypto \
	$(SRC_DIR)/in3/third-party/tommath \
	$(SRC_DIR)/in3/verifier/eth1/basic \
	$(SRC_DIR)/in3/verifier/eth1/evm \
	$(SRC_DIR)/in3/verifier/eth1/full \
	$(SRC_DIR)/in3/verifier/eth1/nano \
  $(SDK_ROOT)/components/libraries/experimental_section_vars \
  $(SDK_ROOT)/external/nrf_cc310/include \
  $(SDK_ROOT)/components/libraries/atomic \
  $(SDK_ROOT)/components/libraries/delay \
  ./config \
  $(SDK_ROOT)/components/toolchain/cmsis/include \
  $(SDK_ROOT)/components/libraries/balloc \
  $(SDK_ROOT)/components/libraries/log \
  $(SDK_ROOT)/components/libraries/memobj \
  $(SDK_ROOT)/components \
  $(SDK_ROOT)/modules/nrfx/mdk \
  $(SDK_ROOT)/components/libraries/strerror \
  $(SDK_ROOT)/integration/nrfx \
  $(SDK_ROOT)/modules/nrfx/drivers/include \
  $(SDK_ROOT)/components/libraries/ringbuf \
  $(SDK_ROOT)/modules/nrfx \
  $(SDK_ROOT)/components/drivers_nrf/nrf_soc_nosd \
  $(SDK_ROOT)/components/libraries/log/src \
  $(SDK_ROOT)/modules/nrfx/hal \
  $(SDK_ROOT)/components/libraries/bsp \
  $(SDK_ROOT)/components/boards \
  $(SDK_ROOT)/integration/nrfx/legacy \
  $(SDK_ROOT)/components/libraries/util \
  $(SDK_ROOT)/external/nrf_cc310/common \

# Libraries common to all targets
LIB_FILES += \
  $(SDK_ROOT)/external/nrf_cc310/lib/cortex-m4/hard-float/libnrf_cc310_0.9.12.a \

# Optimization flags
OPT = -O3 -g3
# Uncomment the line below to enable link time optimization
#OPT += -flto

# C flags common to all targets
CFLAGS += $(OPT)
CFLAGS += -DBOARD_PCA10059
CFLAGS += -DBSP_DEFINES_ONLY
CFLAGS += -DCONFIG_GPIO_AS_PINRESET
CFLAGS += -DDEBUG
CFLAGS += -DDEBUG_NRF
CFLAGS += -DDX_CC_TEE
CFLAGS += -DECC_INTEGTEST
CFLAGS += -DFLOAT_ABI_HARD
CFLAGS += -DNRF52840_XXAA
CFLAGS += -DNRF_SDK_PRESENT
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb -mabi=aapcs
CFLAGS += -Wall
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# keep every function in a separate section, this allows linker to discard unused ones
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin -fshort-enums

# C++ flags common to all targets
CXXFLAGS += $(OPT)

# Assembler flags common to all targets
ASMFLAGS += -g3
ASMFLAGS += -mcpu=cortex-m4
ASMFLAGS += -mthumb -mabi=aapcs
ASMFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
ASMFLAGS += -DBOARD_PCA10059
ASMFLAGS += -DBSP_DEFINES_ONLY
ASMFLAGS += -DCONFIG_GPIO_AS_PINRESET
ASMFLAGS += -DDEBUG
ASMFLAGS += -DDEBUG_NRF
ASMFLAGS += -DDX_CC_TEE
ASMFLAGS += -DECC_INTEGTEST
ASMFLAGS += -DFLOAT_ABI_HARD
ASMFLAGS += -DNRF52840_XXAA
ASMFLAGS += -DNRF_SDK_PRESENT

# Linker flags
LDFLAGS += $(OPT)
LDFLAGS += -mthumb -mabi=aapcs -L$(SDK_ROOT)/modules/nrfx/mdk -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m4
LDFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# let linker dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs

nrf52840_xxaa: CFLAGS += -D__HEAP_SIZE=0
nrf52840_xxaa: CFLAGS += -D__STACK_SIZE=16384
nrf52840_xxaa: ASMFLAGS += -D__HEAP_SIZE=0
nrf52840_xxaa: ASMFLAGS += -D__STACK_SIZE=16384

# Add standard libraries at the very end of the linker input, after all objects
# that may need symbols provided by these libraries.
LIB_FILES += -lc -lnosys -lm


.PHONY: default help

# Default target - first one defined
default: check-env nrf52840_xxaa
# Print all targets that can be built
help:
	@echo following targets are available:
	@echo		nrf52840_xxaa
	@echo		sdk_config - starting external tool for editing sdk_config.h
	@echo		flash      - flashing binary

TEMPLATE_PATH := $(SDK_ROOT)/components/toolchain/gcc


include $(TEMPLATE_PATH)/Makefile.common

$(foreach target, $(TARGETS), $(call define_target, $(target)))

.PHONY: flash erase

# Flash the program
flash: default
	@echo Flashing: $(OUTPUT_DIRECTORY)/nrf52840_xxaa.hex
	nrfjprog -f nrf52 --program $(OUTPUT_DIRECTORY)/nrf52840_xxaa.hex --sectorerase
	nrfjprog -f nrf52 --reset

erase:
	nrfjprog -f nrf52 --eraseall

debug-server:
	JLinkGDBServerCL -device nrf52840_xxaa -if swd -port 2331

debug:
	$(TOOLCHAIN_PATH)/arm-none-eabi-gdb $(OUTPUT_DIRECTORY)/$(TARGETS).out -x debug_cmds.txt

SDK_CONFIG_FILE := ../config/sdk_config.h
CMSIS_CONFIG_TOOL := $(SDK_ROOT)/external_tools/cmsisconfig/CMSIS_Configuration_Wizard.jar
sdk_config:
	java -jar $(CMSIS_CONFIG_TOOL) $(SDK_CONFIG_FILE)

check-env:
ifndef SDK_ROOT
$(error Set environment variable 'SDK_ROOT' conataining the NRF5 SDK folder path)
endif
