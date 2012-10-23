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

#include <aptk/aptk_biquad_filter.h>

#include <math.h>

#define PI						( 3.14159265358979323846f )
#define SQRT_2					( 1.41421356237309504880f )

// see this! http://dspwiki.com/index.php?title=Cookbook_Formulae_for_audio_EQ_biquad_filter_coefficients
// APF: remember, it's useful mate: http://en.wikipedia.org/wiki/All-pass_filter

/*
 * aptk_biquad_filter_init
 */
int aptk_biquad_filter_buffer_init(aptk_biquad_filter_buffer* buffer)
{
	buffer->in[0] = 0.0f;
	buffer->in[1] = 0.0f;
	buffer->out[0] = 0.0f;
	buffer->out[1] = 0.0f;

	return 0;
}

/*
 * internal_aptk_biquad_filter_process_single
 */
static float internal_aptk_biquad_filter_process_single(const aptk_biquad_filter_coeff* coeff, aptk_biquad_filter_buffer* buffer, float value)
{
	float newvalue;

	newvalue = coeff->in[0] * value +
	           coeff->in[1] * buffer->in[0] +
	           coeff->in[2] * buffer->in[1] -
	           coeff->out[0] * buffer->out[0] -
	           coeff->out[1] * buffer->out[1];
	
	buffer->in[1] = buffer->in[0];
	buffer->in[0] = value;
	buffer->out[1] = buffer->out[0];
	buffer->out[0] = newvalue;
		
	return newvalue;
}

/*
 * aptk_biquad_filter_process
 */
int aptk_biquad_filter_process(const aptk_biquad_filter_coeff* coeff, aptk_biquad_filter_buffer* buffer, const float* invalues, float* outvalues, int numvalues)
{
	while(numvalues--)
	{
		*outvalues++ = internal_aptk_biquad_filter_process_single(coeff, buffer, *invalues++);
	}

	return 0;
}

/*
 * aptk_biquad_filter_coeff_passthrough
 */
int aptk_biquad_filter_coeff_passthrough(aptk_biquad_filter_coeff* coeff)
{
	coeff->in[0] = 1.0f;
	coeff->in[1] = 0.0f;
	coeff->in[2] = 0.0f;
	coeff->out[0] = 0.0f;
	coeff->out[1] = 0.0f;

	return 0;
}

/*
 * aptk_biquad_filter_coeff_lowpass
 */
int aptk_biquad_filter_coeff_lowpass(aptk_biquad_filter_coeff* coeff, float rate, float cutoff, float resonance)
{
	// http://musicdsp.org
	// Type : biquad, tweaked butterworth
    // References : Posted by Patrice Tarrabia

	register float initcoef = 1.0f / tanf(PI * cutoff / rate);
	register float initcoefpw2 = initcoef * initcoef;
	register float resrange = ((SQRT_2 - 0.1f) * (1.0f - resonance)) + 0.1f;

	coeff->in[0] = 1.0f / (1.0f + (resrange * initcoef) + initcoefpw2);
	coeff->in[1] = 2.0f * coeff->in[0];
	coeff->in[2] = coeff->in[0];

	coeff->out[0] = 2.0f * coeff->in[0] * (1.0f - initcoefpw2);
	coeff->out[1] = coeff->in[0] * (1.0f - resrange * initcoef + initcoefpw2);

	return 0;
}

/*
 * aptk_biquad_filter_coeff_highpass
 */
int aptk_biquad_filter_coeff_highpass(aptk_biquad_filter_coeff* coeff, float rate, float cutoff, float resonance)
{
	// http://musicdsp.org
	// Type : biquad, tweaked butterworth
    // References : Posted by Patrice Tarrabia

	register float initcoef = 1.0f / tanf(PI * cutoff / rate);
	register float initcoefpw2 = initcoef * initcoef;
	register float resrange = ((SQRT_2 - 0.1f) * (1.0f - resonance)) + 0.1f;

	coeff->in[0] = 1.0f / (1.0f + (resrange * initcoef) + initcoefpw2);
	coeff->in[1] = -2.0f * coeff->in[0];
	coeff->in[2] = coeff->in[0];

	coeff->out[0] = 2.0f * coeff->in[0] * (initcoefpw2 - 1.0f);
	coeff->out[1] = coeff->in[0] * (1.0f - resrange * initcoef + initcoefpw2);

	return 0;
}
