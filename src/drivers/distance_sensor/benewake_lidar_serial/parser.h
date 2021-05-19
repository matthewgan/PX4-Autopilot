/****************************************************************************
 *
 *   Copyright (c) 2014 PX4 Development Team. All rights reserved.
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
 * @file parser.cpp
 * @author Matthew Gan <matthewgan@126.com>
 *
 * Declarations of parser for the Benewake TF lidar rangefinder series
 */

#pragma once

enum TF_PARSE_STATE {
	TF_PARSE_STATE0_SEARCH_HEADER1 = 0,
	TF_PARSE_STATE1_SEARCH_HEADER2,
	TF_PARSE_STATE2_GOT_DIST_L,
	TF_PARSE_STATE3_GOT_DIST_H,
	TF_PARSE_STATE4_GOT_STRENGTH_L,
	TF_PARSE_STATE5_GOT_STRENGTH_H,
	TF_PARSE_STATE6_GOT_TEMP_L,
	TF_PARSE_STATE7_GOT_TEMP_H,
	TF_PARSE_STATE8_CHECKSUM,
};

int benewake_parser(char c, char *parserbuf, unsigned *parserbuf_index, enum TF_PARSE_STATE *state, float *dist);
