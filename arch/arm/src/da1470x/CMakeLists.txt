# ##############################################################################
# arch/arm/src/da1470x/CMakeLists.txt
#
# Licensed to the Apache Software Foundation (ASF) under one or more contributor
# license agreements.  See the NOTICE file distributed with this work for
# additional information regarding copyright ownership.  The ASF licenses this
# file to you under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License.  You may obtain a copy of
# the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
# License for the specific language governing permissions and limitations under
# the License.
#
# ##############################################################################

set(SRCS
    da1470x_allocateheap.c
    da1470x_clockconfig.c
    da1470x_gpio.c
    da1470x_irq.c
    da1470x_lowputc.c
    da1470x_start.c
    da1470x_pmu.c)

if(CONFIG_DA1470X_PM_EXTENDED_SLEEP)
  list(APPEND SRCS da1470x_deepsleep.S)
endif()

if(CONFIG_DA1470X_TICKLESS)
  list(APPEND SRCS da1470x_tickless.c)
else()
  list(APPEND SRCS da1470x_systick.c)
endif()

if(NOT CONFIG_ARCH_IDLE_CUSTOM)
  list(APPEND SRCS da1470x_idle.c)
endif()

if(CONFIG_DA1470X_UART)
  list(APPEND SRCS da1470x_serial.c)
endif()

if(CONFIG_DA1470X_DMA)
  list(APPEND SRCS da1470x_dma.c)
endif()

if(CONFIG_DA1470X_SPI)
  list(APPEND SRCS da1470x_spi.c)
endif()

if(CONFIG_DA1470X_I2C)
  list(APPEND SRCS da1470x_i2c.c)
endif()

if(CONFIG_DA1470X_RTC)
  list(APPEND SRCS da1470x_rtc.c da1470x_rtc_lowerhalf.c)
endif()

if(CONFIG_DA1470X_PDC)
  list(APPEND SRCS da1470x_pdc.c)
endif()

if(CONFIG_PM)
  list(APPEND SRCS da1470x_pm.c da1470x_pminitialize.c)
endif()

if(CONFIG_DA1470X_OQSPI_MTD)
  list(APPEND SRCS da1470x_oqspi.c)
endif()

if(CONFIG_DA1470X_LCDC)
  list(APPEND SRCS da1470x_lcdc.c)
  if(CONFIG_DA1470X_LCDC_AOD)
    list(APPEND SRCS da1470x_aodface.c)
  endif()
endif()

if(CONFIG_DA1470X_GPU)
  list(APPEND SRCS da1470x_gpu.c)
endif()

if(CONFIG_DA1470X_TCS)
  list(APPEND SRCS da1470x_otp.c da1470x_tcs.c)
endif()

if(CONFIG_DA1470X_BLE)
  list(APPEND SRCS da1470x_cmac.c da1470x_bt.c)
endif()

if(CONFIG_DA1470X_AUDIO)
  list(APPEND SRCS da1470x_audio.c)
endif()

if(CONFIG_DA1470X_VAD)
  list(APPEND SRCS da1470x_vad.c)
endif()

if(CONFIG_DA1470X_CHARGER)
  list(APPEND SRCS da1470x_charger.c)
endif()

if(CONFIG_DA1470X_GPADC)
  list(APPEND SRCS da1470x_gpadc.c)
endif()

if(CONFIG_DA1470X_PWM)
  list(APPEND SRCS da1470x_pwm.c)
endif()

if(CONFIG_DA1470X_PSRAM)
  list(APPEND SRCS da1470x_psram.c)
endif()

if(CONFIG_DA1470X_WDT)
  list(APPEND SRCS da1470x_wdt.c)
endif()

if(CONFIG_DA1470X_SNC)
  list(APPEND SRCS da1470x_snc.c da1470x_snc_image.S)

  # The sensor node controller's firmware is a separate Cortex-M0+ program,
  # built from snc/ and embedded in the image by da1470x_snc_image.S.

  set(SNC_DIR ${CMAKE_CURRENT_LIST_DIR}/snc)
  set(SNC_ELF ${CMAKE_CURRENT_BINARY_DIR}/snc_fw.elf)
  set(SNC_BIN ${CMAKE_CURRENT_BINARY_DIR}/snc_fw.bin)

  if(CONFIG_DA1470X_SNC_FW_DEMO)
    set(SNC_APP ${SNC_DIR}/app_demo.c)
  elseif(CONFIG_DA1470X_SNC_FW_INPUT)
    set(SNC_APP ${SNC_DIR}/app_input.c)
  endif()

  set(SNC_SRCS ${SNC_DIR}/snc_start.S ${SNC_DIR}/snc_rt.c ${SNC_APP})

  add_custom_command(
    OUTPUT ${SNC_BIN}
    COMMAND
      ${CMAKE_C_COMPILER} -mcpu=cortex-m0plus -mthumb -Os -g -std=gnu11
      -ffreestanding -fno-builtin -nostdlib -ffunction-sections -fdata-sections
      -Wall -Wextra -Wno-unused-parameter -idirafter ${CMAKE_BINARY_DIR}/include
      -idirafter ${NUTTX_DIR}/include -I${CMAKE_CURRENT_LIST_DIR} -T
      ${SNC_DIR}/snc.ld -Wl,--gc-sections -Wl,--no-warn-rwx-segments ${SNC_SRCS}
      -lgcc -o ${SNC_ELF}
    COMMAND ${CMAKE_OBJCOPY} -O binary ${SNC_ELF} ${SNC_BIN}
    DEPENDS ${SNC_SRCS} ${SNC_DIR}/snc.ld ${SNC_DIR}/snc_rt.h
            ${CMAKE_CURRENT_LIST_DIR}/da1470x_snc_ipc.h
    COMMENT "SNC firmware")

  add_custom_target(da1470x_snc_fw DEPENDS ${SNC_BIN})
  add_dependencies(arch da1470x_snc_fw)

  # The arch target is defined in another directory, and source properties are
  # directory-scoped, so they have to be set in the target's directory.

  set_source_files_properties(
    ${CMAKE_CURRENT_LIST_DIR}/da1470x_snc_image.S TARGET_DIRECTORY arch
    PROPERTIES COMPILE_DEFINITIONS "DA1470X_SNC_IMAGE=\"${SNC_BIN}\""
               OBJECT_DEPENDS ${SNC_BIN})
endif()

if(CONFIG_DA1470X_PWMLED)
  list(APPEND SRCS da1470x_pwmled.c)
endif()

target_sources(arch PRIVATE ${SRCS})
