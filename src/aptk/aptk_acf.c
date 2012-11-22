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

#include <aptk/aptk_acf.h>

#include <math.h>

/* 
 * aptk_acf_process
 */
int aptk_acf_process(const float* invalues, float* outvalues, int numvalues)
{
	register int i = 0;
	register int j = 0;
	register float sum = 0.0;

	for(i = 0; i < numvalues; ++i)
	{
		sum = 0.0;
		for(j = 0; j < numvalues - i; ++j)
		{
			sum += invalues[j] * invalues[j + i];
		}
		outvalues[i] = sum;
	}

	return 0;
}

/* 
 * aptk_acf_largest_peak_period
 */
int aptk_acf_largest_peak_period(const float* invalues, int numvalues, int* outperiod)
{
	register int i = 0;
	float prev = 0.0f;
	float curr = 0.0f;
	float prevgrad = 0.0f;
	float currgrad = 0.0f;

	float max0 = 0.0f;
	int max0idx = -1;
	float max1 = 0.0f;
	int max1idx = -1;

	prev = 0.0f;
	prevgrad = 0.0f;
	for(i = 0; i < numvalues; ++i)
	{
		curr = invalues[i];
		currgrad = prev - curr;

		if((currgrad > 0.0f && prevgrad < 0.0f) ||
		   (currgrad < 0.0f && prevgrad > 0.0f))
		{
			if(curr > max0)
			{
				max0 = curr;
				max0idx = i;
			}
			else if(curr > max1 && curr <= max0)
			{
				max1 = curr;
				max1idx = i;
			}
		}

		if(currgrad != 0.0f)
		{
			prevgrad = currgrad;
		}
		prev = curr;
	}

	*outperiod = max1idx - max0idx;
	return 0;
}