/****************************************************************************
 * arch/arm/src/da1470x/snc/snc.ld
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/* The sensor node controller runs from RAM1 and RAM2, which it sees at its
 * own address zero.  The M33 copies the image there whole, so code and
 * initialised data are linked where they run and there is no load address.
 * RAM8, shared with the M33, is reached through a fixed pointer, not
 * through this script.
 */

MEMORY
{
  snc (rwx) : ORIGIN = 0x00000000, LENGTH = 64K
}

ENTRY(snc_reset)

SECTIONS
{
  .text :
  {
    KEEP(*(.vectors))
    . = 0xc0;                   /* SNC_IMAGE_HDR_OFFSET */
    KEEP(*(.snc_header))
    *(.text .text.*)
    *(.rodata .rodata.*)
  } > snc

  .data :
  {
    . = ALIGN(4);
    *(.data .data.*)
    . = ALIGN(4);
  } > snc

  _snc_image_end = .;

  .bss (NOLOAD) :
  {
    _snc_sbss = .;
    *(.bss .bss.*)
    *(COMMON)
    . = ALIGN(4);
    _snc_ebss = .;
  } > snc

  _snc_stack_top = ORIGIN(snc) + LENGTH(snc);

  /* Keep room for the stack */

  ASSERT(_snc_ebss + 0x800 <= _snc_stack_top, "SNC image leaves no stack")

  /DISCARD/ :
  {
    *(.ARM.exidx*)
    *(.comment)
  }
}
