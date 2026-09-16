# ##############################################################################
# boards/arm/da1470x/da14706-00hzdevkt-p/src/CMakeLists.txt
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

set(SRCS da1470x_boot.c da1470x_bringup.c)

if(CONFIG_BOARDCTL)
  list(APPEND SRCS da1470x_appinit.c)
endif()

if(CONFIG_ARCH_LEDS)
  list(APPEND SRCS da1470x_autoleds.c)
else()
  list(APPEND SRCS da1470x_userleds.c)
endif()

if(CONFIG_DEV_GPIO)
  list(APPEND SRCS da1470x_gpio.c)
endif()

if(CONFIG_DA1470X_SPI)
  list(APPEND SRCS da1470x_spi.c)
endif()

if(CONFIG_DA1470X_I2C)
  list(APPEND SRCS da1470x_i2c.c)
endif()

if(CONFIG_DA14706_LCD_E120A390QSR)
  list(APPEND SRCS da1470x_e120a390qsr.c)
endif()

if(CONFIG_DA14706_TOUCH_ZT2628)
  list(APPEND SRCS da1470x_zt2628.c)
endif()

if(CONFIG_DA1470X_PWMLED)
  list(APPEND SRCS da1470x_pwmled.c)
endif()

if(CONFIG_DA1470X_BLE)
  list(APPEND SRCS da1470x_cmac_fw.S)
  set_source_files_properties(
    da1470x_cmac_fw.S PROPERTIES COMPILE_DEFINITIONS
    "DA1470X_CMAC_FW_PATH=\"${NUTTX_DIR}/${CONFIG_DA1470X_CMAC_FW_PATH}\"")
endif()

target_sources(board PRIVATE ${SRCS})

set_property(GLOBAL PROPERTY LD_SCRIPT "${NUTTX_BOARD_DIR}/scripts/flash_app.ld")
