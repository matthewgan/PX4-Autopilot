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
 *
 * Declarations of parser for the Benewake TFmini laser rangefinder series
 */

#include "tfmini_parser.h"
#include <string.h>
#include <stdlib.h>

// #define TFMINI_DEBUG

#ifdef TFMINI_DEBUG
#include <stdio.h>

const char *parser_state[] = {
	"0_UNSYNC",
	"1_SYNC_1",
	"2_SYNC_2",
	"3_GOT_DIST_L",
	"4_GOT_DIST_H",
	"5_GOT_AMP_L",
	"6_GOT_AMP_H",
	"7_GOT_TEMP_L",
	"8_GOT_TEMP_H"
	"9_GOT_CHECKSUM"
};
#endif


unsigned int tf_bytes_to_uint(char c_low, char c_high)
{
	unsigned int t1 = c_low;
	unsigned int t2 = c_high;
	t2 <<= 8;
	t2 += t1;
	return t2;
}


int tfmini_parse(char c, char *parserbuf,
		unsigned *parserbuf_index,
		TFMINI_PARSE_STATE *state,
		float *dist,
		int *signal_strength,
		int *ctemp)
{
	int ret = -1;
	//char *end;

	switch (*state) {
	case TFMINI_PARSE_STATE::STATE6_GOT_CHECKSUM:
		if (c == 'Y') {
			*state = TFMINI_PARSE_STATE::STATE1_SYNC_1;
			parserbuf[*parserbuf_index] = c;
			(*parserbuf_index)++;

		} else {
			*state = TFMINI_PARSE_STATE::STATE0_UNSYNC;
		}

		break;

	case TFMINI_PARSE_STATE::STATE0_UNSYNC:
		if (c == 'Y') {
			*state = TFMINI_PARSE_STATE::STATE1_SYNC_1;
			parserbuf[*parserbuf_index] = c;
			(*parserbuf_index)++;
		}

		break;

	case TFMINI_PARSE_STATE::STATE1_SYNC_1:
		if (c == 'Y') {
			*state = TFMINI_PARSE_STATE::STATE1_SYNC_2;
			parserbuf[*parserbuf_index] = c;
			(*parserbuf_index)++;

		} else {
			*state = TFMINI_PARSE_STATE::STATE0_UNSYNC;
			*parserbuf_index = 0;
		}

		break;

	case TFMINI_PARSE_STATE::STATE1_SYNC_2:
		*state = TFMINI_PARSE_STATE::STATE2_GOT_DIST_L;
		parserbuf[*parserbuf_index] = c;
		(*parserbuf_index)++;

		break;

	case TFMINI_PARSE_STATE::STATE2_GOT_DIST_L:
		*state = TFMINI_PARSE_STATE::STATE2_GOT_DIST_H;
		parserbuf[*parserbuf_index] = c;
		(*parserbuf_index)++;

		break;

	case TFMINI_PARSE_STATE::STATE2_GOT_DIST_H:
		*state = TFMINI_PARSE_STATE::STATE3_GOT_AMP_L;
		parserbuf[*parserbuf_index] = c;
		(*parserbuf_index)++;

		break;

	case TFMINI_PARSE_STATE::STATE3_GOT_AMP_L:
		*state = TFMINI_PARSE_STATE::STATE3_GOT_AMP_H;
		parserbuf[*parserbuf_index] = c;
		(*parserbuf_index)++;

		break;

	case TFMINI_PARSE_STATE::STATE3_GOT_AMP_H:
		*state = TFMINI_PARSE_STATE::STATE4_GOT_TEMP_L;
		parserbuf[*parserbuf_index] = c;
		(*parserbuf_index)++;

		break;

	case TFMINI_PARSE_STATE::STATE4_GOT_TEMP_L:
		*state = TFMINI_PARSE_STATE::STATE5_GOT_TEMP_H;
		parserbuf[*parserbuf_index] = c;
		(*parserbuf_index)++;

		break;

	case TFMINI_PARSE_STATE::STATE5_GOT_TEMP_H:
		// Find the checksum
		unsigned char cksm = 0;

		for (int i = 0; i < 8; i++) {
			cksm += parserbuf[i];
		}

		if (c == cksm) {
			parserbuf[*parserbuf_index] = '\0';
			unsigned int distance = tf_bytes_to_uint(parserbuf[2],parserbuf[3]);
			*dist = ((float)distance) / 100;
			unsigned int amp = tf_bytes_to_uint(parserbuf[4],parserbuf[5]);
			*signal_strength = (int)amp;
			unsigned int temperature = tf_bytes_to_uint(parserbuf[6],parserbuf[7]);
			*ctemp = (int)temperature / 8 - 256;
			*state = TFMINI_PARSE_STATE::STATE6_GOT_CHECKSUM;
			*parserbuf_index = 0;
			ret = 0;

		} else {
			*state = TFMINI_PARSE_STATE::STATE0_UNSYNC;
			*parserbuf_index = 0;
		}

		break;
	}

#ifdef TFMINI_DEBUG
	printf("state: TFMINI_PARSE_STATE%s\n", parser_state[*state]);
#endif

	return ret;
}
