#ifndef __TTU2
#define __TTU2
/*
MIT License

Copyright (c) 2023 Jon Parker

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

//  two-term ultraspherical-2 phase function

/*Citation

A new analytical scattering phase function for interstellar dust
Maarten  Baes, Peter  Camps, Anand Utsav  Kapoor
A&A 659 A149 (2022)
DOI: 10.1051/0004-6361/202142437
*/

/*
From paper:
"Model parameters of the best-fitting TTU2 phase functions for the BARE-GR-S dust model
at the central wavelengths of a number of standard broadband filters."

Band 		λ 		g1 		g2 		γ
			[μm] 			
FUV 		0.154 	0.764 	−0.016 	0.950
NUV 		0.230 	0.670 	−0.010 	0.792
u 			0.356 	0.545 	−0.048 	0.841
g 			0.470 	0.449 	−0.109 	0.882
r 			0.618 	0.361 	−0.308 	0.952
i 			0.749 	0.320 	−0.370 	0.967
z 			0.895 	0.288 	−0.327 	0.960
J 			1.239 	0.262 	−0.274 	0.846
H 			1.649 	0.259 	−0.240 	0.643
Ks 			2.164 	0.254 	−0.241 	0.583
W1 			3.390 	0.248 	−0.245 	0.549
W2 			4.641 	0.247 	−0.246 	0.526
*/


bsdf ttu2_bsdf(const float g1; const float g2; const float weight; const string label)
{
    return cvex_bsdf("ttu2_eval", "ttu2_sample", "label", label, "phase1", g1, "phase2", g2, "weight", weight);
}

bsdf ttu2_bsdf(const float g1; const float g2; const float weight)
{
    return cvex_bsdf("ttu2_eval", "ttu2_sample", "label", "volume", "phase1", g1, "phase2", g2, "weight", weight);
}


#endif