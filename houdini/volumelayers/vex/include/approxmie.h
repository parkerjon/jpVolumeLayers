#ifndef __APPROXMIE
#define __APPROXMIE

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

/*
Approximate Mie VEX shader bsdfs.  Original research here:
  https://research.nvidia.com/labs/rtr/approximate-mie/

Citation:
[Jendersie and d'Eon 2023]
  SIGGRAPH 2023 Talks
  https://doi.org/10.1145/3587421.3595409
*/

// From:
// https://research.nvidia.com/labs/rtr/approximate-mie/publications/approximate-mie-supplemental.pdf
void approxmie_remap_diameter(const float diameter; export float ghg; export float wd; export float gd; export float a)
{
    // model is only good to 50 micrometers
    float d = clamp(diameter, 0, 50);

    float logd = log(d);
    // small particles, diameter <= 0.1 μm
    if (d <= 0.1)
    {
        ghg = 13.8 * d * d;
        gd = 1.1456 * d * sin(9.29044 * d);
        a = 250;
        wd = 0.252977 - 312.983 * pow(d, 4.3);
    }
    // mid-range particles, diameter 0.1 μm < d < 1.5 μm
    else if (d < 1.5)
    {
        ghg = 0.862 - 0.143 * (logd * logd);
        gd = 0.379685 * cos(1.19692 * cos(((logd - 0.238604) * (logd + 1.00667)) / (0.507522 - 0.15677 * logd)) + 1.37932 * logd + 0.0625835) + 0.344213;
        a = 250;
        wd = 0.146209 * cos(3.38707 * logd + 2.11193) + 0.316072 + 0.0778917 * logd;
    }
    // mid-range particles, diameter 1.5 μm <= d < 5μm
    else if (d < 5)
    {
        ghg = 0.0604931 * log(logd) + 0.940256;
        gd = 0.500411 - (0.081287 / (-2 * logd + tan(logd) + 1.27551));
        a = 7.30354 * logd + 6.31675;
        wd = 0.026914 * (logd - cos(5.68947 * log(logd)) - 0.0292149) + 0.376475;
    }
    // larger particles, diameter 5μm <= d <= 50μm
    else
    {
        ghg = exp(-0.0990567 / (d - 1.67154));
        gd = exp(-(2.20679 / (d + 3.91029)) - 0.428934);
        a = exp(3.62489 - (8.29288 / (d + 5.5825)));
        wd = exp(-(.599085 / (d - 0.641583)) - 0.665888);
    }
}

bsdf draine_bsdf(const float g; const float a; const string label)
{
    return cvex_bsdf("draine_eval", "draine_sample", "label", label, "phase", g, "alpha", a);
}

bsdf draine_bsdf(const float g; const float a)
{
    return draine_bsdf(g, a, "volume");
}

bsdf cornetteshanks_bsdf(const float g; const string label)
{
    return cvex_bsdf("draine_eval", "draine_sample", "label", label, "phase", g, "alpha", 1);
}

bsdf cornetteshanks_bsdf(const float g)
{
    return cornetteshanks_bsdf(g, "volume");
}

// Fog bsdf referenced in paper
bsdf approxmie_bsdf(const float ghg; const float gd; const float wd; const float a; const string label)
{
    bsdf hg = henyeygreenstein(ghg, "label", label);
    bsdf draine = draine_bsdf(gd, a, label);

    return hg * (1 - wd) + draine * wd;
}

bsdf approxmie_bsdf(const float ghg; const float gd; const float wd; const float a)
{
    return approxmie_bsdf(ghg, gd, wd, a, "volume");
}

bsdf approxmie_bsdf(const float diameter; const string label)
{
    float ghg, gd, wd, a;
    approxmie_remap_diameter(diameter, ghg, wd, gd, a);
    return approxmie_bsdf(ghg, gd, wd, a, label);
}

bsdf approxmie_bsdf(const float diameter)
{
    return approxmie_bsdf(diameter, "volume");
}

// The supplemental paper describes two ways to realize Rayleigh scattering,
// using Draine directly with g=0 and a=1, or using the approx-mie method
// below with values of ghg=0, a=∞, gd=0 and wd=0.25.
bsdf rayleigh_bsdf(const string label)
{
    return cvex_bsdf("draine_eval", "draine_sample", "label", label, "phase", 0, "alpha", 1);
}

bsdf rayleigh_bsdf()
{
    return rayleigh_bsdf("volume");
}


#endif