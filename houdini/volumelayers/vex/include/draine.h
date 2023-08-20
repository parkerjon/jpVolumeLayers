#ifndef __GUARD__DRAINE__
#define __GUARD__DRAINE__


float sample_draine_cos(float xi; float g; float a)
{
    float g2 = g * g;
	float g3 = g * g2;
	float g4 = g2 * g2;
	float g6 = g2 * g4;
	float pgp1_2 = (1 + g2) * (1 + g2);
	float T1 = (-1 + g2) * (4 * g2 + a * pgp1_2);
	float T1a = -a + a * g4;
	float T1a3 = T1a * T1a * T1a;
	float T2 = -1296 * (-1 + g2) * (a - a * g2) * (T1a) * (4 * g2 + a * pgp1_2);
	float T3 = 3 * g2 * (1 + g * (-1 + 2 * xi)) + a * (2 + g2 + g3 * (1 + 2 * g2) * (-1 + 2 * xi));
	float T4a = 432 * T1a3 + T2 + 432 * (a - a * g2) * T3 * T3;
	float T4b = -144 * a * g2 + 288 * a * g4 - 144 * a * g6;
	float T4b3 = T4b * T4b * T4b;
	float T4 = T4a + sqrt(-4 * T4b3 + T4a * T4a);
	float T4p3 = pow(T4, 1.0 / 3.0);
	float T6 = (2 * T1a + (48 * pow(2, 1.0 / 3.0) *
		(-(a * g2) + 2 * a * g4 - a * g6)) / T4p3 + T4p3 / (3. * pow(2, 1.0 / 3.0))) / (a - a * g2);
	float T5 = 6 * (1 + g2) + T6;
	return (1 + g2 - pow(-0.5 * sqrt(T5) + sqrt(6 * (1 + g2) - (8 * T3) / (a * (-1 + g2) * sqrt(T5)) - T6) / 2., 2)) / (2. * g);
}

float sample_draine_cos_paper(float xi; float g; float a)
{
    float g2 = g*g;
    float g3 = g * g2;
    float g4 = g2 * g2;

    float t0 = a - a*g2;
    float t1 = a * g4 - a;
    float t2 = -3 * (4 * (g4 - g2) + t1 * (1 + g2));
    float t3 = g * (2*xi - 1);
    float t4 = 3 * g2 * (1 + t3) + a * (2 + g2 * (1 + (1 + 2 * g2) * t3));
    float t5 = t0 * (t1 * t2 + (t4*t4)) + (t1*t1*t1);
    float t6 = t0 * 4 * (g4 - g2);
    float t7 = pow(t5 + sqrt((t5*t5) - (t6*t6*t6)), 1./3);
    float t8 = 2 * ((t1 + t6 / t7 + t7) / t0);
    float t9 = sqrt(6 * (1 + g2) + t8);
    return 9./2 + (1/(2*g) - 1/(8*g) * pow(sqrt(6 * (1 + g2) - t8 + (8*t4) / (t0*t9)), 2));
}

float eval_draine_pdf(float u; float g; float a)
{
	return ((1 - g*g) / pow(1 + g*g - 2*g*u, 1.5)) * ((1 + a * (u*u)) / (1 + a * (1 + 2*(g*g)) / 3));
}

/*
    """
    This model provides a meaningful and intuitive single parameter for the Henyey-Greenstein-Drain model. 
    
    Calculates anisotropy, secondary mix, secondary anisotropy, and secondary shape values for Henyey-Greenstein-Drain
    function in this given order; using a droplet size input (µm).

    The logic for this function is taken from An Approximate Mie Scattering Function for Fog and Cloud Rendering by NVIDIA
    https://research.nvidia.com/labs/rtr/approximate-mie/publications/approximate-mie-supplemental.pdf

    Original paper:
    https://research.nvidia.com/labs/rtr/approximate-mie/publications/approximate-mie.pdf
    """
    

    if d <= 0.1:
        # small particles
        return 13.8 * d * d, 0.252977 - 312.983 * pow(d, 4.3), 1.1456 * d * math.sin(9.29044 * d), 250.0
    elif d < 1.5:
        # mid-range below 1.5 µm
        logd = math.log(d)
        anisotropy = 0.862 - 0.143 * logd * logd
        secondarymix = 0.146209 * math.cos(3.38707 * logd + 2.11193) + 0.316072 + 0.0778917 * logd
        secondaryanisotropy = 0.379685 * math.cos(1.19692 * math.cos(((logd - 0.238604) * (logd + 1.00667)) /
                              (0.507522 - 0.15677 * logd)) + 1.37932 * logd + 0.0625835) + 0.344213
        secondaryshape = 250.0
        return anisotropy, secondarymix, secondaryanisotropy, secondaryshape
    elif d < 5:
        # mid-range below 5µm
        logd = math.log(d)
        anisotropy = 0.0604931 * math.log(logd) + 0.940256;
        secondarymix = 0.026914 * (logd - math.cos(5.68947 * math.log(logd) - 0.0292149)) + 0.376475
        secondaryanisotropy = 0.500411 - (0.081287 / (-2.0 * logd + math.tan(logd) + 1.27551))
        secondaryshape = 7.30354 * logd + 6.31675
        return anisotropy, secondarymix, secondaryanisotropy, secondaryshape
    else:
        # large particles
        anisotropy = math.exp(-0.0990567 / (d - 1.67154))
        secondarymix = math.exp(-0.599085 / (d - 0.641583) - 0.665888)
        secondaryanisotropy = math.exp(-2.20679 / (d + 3.91029) - 0.428934)
        secondaryshape = math.exp(3.62489 - (8.29288 / (d + 5.52825)))
        return anisotropy, secondarymix, secondaryanisotropy, secondaryshape

*/
void draine_remap_parameters(float d; export float g1; export float mix; export float g2; export float shape)
{
    if (d <= 0.1)
    {
    }
}

#endif

