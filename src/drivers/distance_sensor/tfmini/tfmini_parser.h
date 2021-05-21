/****************************************************************************
 *
 *   Copyright (c) 2017-2019 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
 * @file modified from sf0x_parser.cpp
 * @author Lorenz Meier <lm@inf.ethz.ch>
 * @author Chuong Nguyen <chnguye7@asu.edu>
 * @author Ayush Gaud <ayush.gaud@gmail.com>
 * @author Matthew Gan <matthewgan@126.com>
 *
 * Declarations of parser for the Benewake TFmini laser rangefinder series
 */

#pragma once

// Data Format for Benewake TFmini
// ===============================
// 9 bytes total per message:
// 1) 0x59
// 2) 0x59
// 3) Dist_L (low 8bit)
// 4) Dist_H (high 8bit)
// 5) Strength_L (low 8bit)
// 6) Strength_H (high 8bit)
// 7) Reserved bytes
// 8) Original signal quality degree
// 9) Checksum parity bit (low 8bit), Checksum = Byte1 + Byte2 +...+Byte8. This is only a low 8bit though
// Update data fromat for Benewake TF series Lidar
// Interface are unified for TFmini, TFmini S, TFmini Plus, TF luna
// 9-BYTE/cm default output format
// 5) Strength_L (low 8bit)		-> Amp_L (low 8bit)
// 6) Strength_H (high 8bit)		-> Amp_H (High 8bit)
// 7) Reserved bytes 			-> Temp_L (low 8bit)
// 8) Original signal quality degree 	-> Temp_H (High 8bit)
// Amp: Signal strength indicator. Dist value is unreliable when Amp < 100 or Amp = 65535 (Overexposure)
// Temp: Celsius temperature = Temp / 8 - 256℃


enum class TFMINI_PARSE_STATE {
	STATE0_UNSYNC = 0,
	STATE1_SYNC_1,
	STATE1_SYNC_2,
	STATE2_GOT_DIST_L,
	STATE2_GOT_DIST_H,
	STATE3_GOT_AMP_L,
	STATE3_GOT_AMP_H,
	STATE4_GOT_TEMP_L,
	STATE5_GOT_TEMP_H,
	STATE6_GOT_CHECKSUM
};

int tfmini_parse(char c, char *parserbuf, unsigned *parserbuf_index, TFMINI_PARSE_STATE *state, float *dist, int *amp, int *temp);
