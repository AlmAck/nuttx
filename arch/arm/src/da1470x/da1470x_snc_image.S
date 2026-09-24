/****************************************************************************
 * arch/arm/src/da1470x/da1470x_snc_image.S
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

/* The sensor node controller's firmware, built from snc/ by the same build
 * and embedded here.  da1470x_snc.c copies it to the controller's RAM.
 */

	.section .rodata.snc_image, "a"
	.balign	4

	.global	g_da1470x_snc_image
	.global	g_da1470x_snc_image_end

g_da1470x_snc_image:
	.incbin	DA1470X_SNC_IMAGE
g_da1470x_snc_image_end:

	.end
