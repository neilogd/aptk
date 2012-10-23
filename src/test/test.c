/*
 * Copyright (c) 2012 Neil Richardson
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <aptk/aptk.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char* argv[])
{
	FILE* file = NULL;
	int filesize = 0;
	short* rawdata = NULL;
	float* indata = NULL;
	float* outdata = NULL;
	int samples = 0;
	int i;
	aptk_biquad_filter_buffer buffer;
	aptk_biquad_filter_coeff coeff;

	file = fopen(argv[1], "rb");
	if(file)
	{
		fseek(file, 0, SEEK_END);
		filesize = ftell(file);
		fseek(file, 0, SEEK_SET);
		rawdata = (short*)malloc(filesize);
		fread(rawdata, filesize, 1, file);
		fclose(file);

		samples = filesize / sizeof(short);
		indata = (float*)malloc(samples * sizeof(float));
		outdata = (float*)malloc(samples * sizeof(float));

		for(i = 0; i < samples; ++i)
		{
			indata[i] = (float)rawdata[i] / (float)SHRT_MAX;
		}

		aptk_biquad_filter_buffer_init(&buffer);
		//aptk_biquad_filter_coeff_passthrough(&coeff);
		aptk_biquad_filter_coeff_lowpass(&coeff, 44100.0f, 1000.0f, 0.0f);
		aptk_biquad_filter_process(&coeff, &buffer, indata, outdata, samples);

		for(i = 0; i < samples; ++i)
		{
			rawdata[i] = (short)(outdata[i] * (float)SHRT_MAX);
		}

		file = fopen(argv[2], "wb+");
		if(file)
		{
			fwrite(rawdata, samples * sizeof(short), 1, file);
			fclose(file);
		}
	}

	return 0;
}
