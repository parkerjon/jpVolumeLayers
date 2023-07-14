#ifndef __GUARD__VOLUME_LAYER__
#define __GUARD__VOLUME_LAYER__

/*
Volume layer struct.  
Layering volume shaders is NOT like layering surface shaders and the struct below is a fix for that.
*/

struct VolumeLayer
{
    bsdf        F       ;
    vector      density ;
    vector      Ce      ;
}

// Construct a volume layer.  f is assumed to have an unmodified albedo...
VolumeLayer buildVolumeLayer(
    bsdf f;
    vector absorb;
    vector scatter;
    vector emit;
    )
{
    vector extinction = absorb + scatter;
    vector albedo = scatter / extinction;
    
    return VolumeLayer(f * albedo, extinction, emit);
}


VolumeLayer buildVolumeLayer(
    float phase;
    vector absorb;
    vector scatter;
    vector emit;
    string label;
)
{
    bsdf f;
    if (phase == 0)
        f = isotropic("label", label);
    else 
        f = henyeygreenstein(phase, "label", label);
    
    return buildVolumeLayer(f, absorb, scatter, emit);
}


VolumeLayer volumeLayerMerge(
    VolumeLayer a;
    VolumeLayer b;
    )
{
    vector density = a.density + b.density;
    bsdf f = (a.F * (a.density / density)) + (b.F * (b.density / density));
    vector emit = a.Ce + b.Ce;
    return VolumeLayer(f, density, emit);
}


VolumeLayer volumeLayerMix(
    VolumeLayer a;
    VolumeLayer b;
    float density_mix;
    float F_mix;
    float Ce_mix;
    )
{
    vector density = a.density * (1. - density_mix) + b.density * density_mix;
    bsdf f = (a.F * (a.density / density) * (1. - F_mix)) + (b.F * (b.density / density) * F_mix);
    vector emit  = a.Ce * (1. - Ce_mix) + b.Ce * Ce_mix;
    return VolumeLayer(f, density, emit);
}

VolumeLayer volumeLayerMix(
    VolumeLayer a;
    VolumeLayer b;
    float mix;
    )
{
    return volumeLayerMix(a, b, mix, mix, mix);
}

/*
struct VolumeLayerArray
{
    bsdf        F[]     ;
    vector      density[];
    vector      Ce[]    ;
}

VolumeLayer volumeLayerMerge(
    VolumeLayerArray layers;
    )
{
    VolumeLayer out = VolumeLayer(bsdf(), vector(0.), vector(0.));
    foreach(int i=0; i<len(layers.F); i++)
    {
        vector density = out.density + layers[i].density;
        out.F = (out.F * (out.density / density)) + (layers[i].F * (layers[i].density / density));
        out.density = density;
        out.Ce += layers[i].Ce;
    }
    return out;
}
*/

#endif
