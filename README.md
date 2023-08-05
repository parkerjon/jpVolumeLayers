# Houdini Volume Layer Shaders
*Volume Shading Toolset for Side Effects Houdini's Karma CPU and Mantra renderers*

## Contents
- A new VOP struct type called `VolumeLayer`, with a VEX function library and VOP nodes to work with it
- Utility VOP nodes to make working with volume rendering a bit easier


### Why?
I created these tools to help me render planetary atmospheres for planetarium productions at The American Museum of Natural History.  

Atmospheres can be quite complex things to render, as there are multiple types of scattering, absorption and emission effects all in the same space.  Here are some examples of atmospheric effects that can all potentially overlap on earth:
- dust
- gas
- haze
- water vapor
- aurora
- smoke
- clouds

My initial planet renders used a volume object per effect, which was correct but could be extremely slow.  Then after some experimentation with the built in `ShadingLayer` struct-based VOPS I realized when compared to the multiple-volume object ground truth, the result was incorrect.  

Fortunately, BSDF types in VEX are combined in a *probabilistic* way, and so you can weight them by their relative densities, and then add them together.  The renderer will then sample each randomly based on their weight, thus emulating a proper mixing effect.

This method requires lots of of utility nodes wired together, so I combined them into a single struct type and codified the proper comination methods into a VEX library.

### How to Combine Volumes
In VEX, given two distinct volume phenomena that you want to combine, the formula to do so is fairly straightforward:

```
bsdf f1; bsdf f2;
vector d1; vector d2; // extinction (density)
vector em1; vector em2; // emission

vector density = d1 + d2;
bsdf f = (f1 * (d1 / density)) + (f2 * (d2 / density));
vector emission = em1 + em2;

```
## Examples

*NOTE: The images below are not production renders - the atmosphere thickness is exaggerated to show the effect more clearly, and the texture resolution is very low (2048x1024).*

#### Earth

The following simplified render of Earth uses two **`volumelayershader`** VOPs merged together in a material network.  One `volumelayershader` VOP models blue, isotropic Rayleigh scattering for the gas.  Another `volumelayershader` VOP models dust and haze, which scatters all wavelengths of light in a strongly forward direction, away from the light source.  These two are combined by a simple `volumeshadermerge` VOP before being converted to the necessary material outputs with a `volumelayercomputelighting` VOP.

<img src="resources/earth3.jpg" alt="Earth 3" width="512"/> <img src="resources/earth1.jpg" alt="Earth 1" width="512"/>
<img src="resources/earth2.jpg" alt="Earth 2" width="512"/> <img src="resources/earth4.jpg" alt="Earth 2" width="512"/>


#### Mars

The render of Mars below uses three `VolumeLayer` structs to model the atmosphere.  Like the Earth render above, one struct models blue Rayleigh scattering; however, the density of the atmosphere of Mars is only 5% that of Earth at sea level, so the blue scattering is nearly invisibly.  The second `VolumeLayer` struct models the effect of dust in the atmosphere.  The dust is the red of the Martian surface, which is forward scattering, mostly red, and slightly absorbing of green and blue colors. A third `VolumeLayer` is also combined; this also models dust, but is a thicker layer lying in the low areas, such as the large canyon Valles Marinaris seen towards the center of the sphere.

Again, the thickness has been exagerrated to show the effect.

Here we see the strong foward scattering of the dust as the crescent edge brightens considerably:
<img src="resources/mars1.jpg" alt="Mars 1" width="512"/> <img src="resources/mars3.jpg" alt="Mars 3" width="512"/>

As we move toward the sun's POV, the bright glow around the edge is diminished because most of the light travels away from us:
<img src="resources/mars4.jpg" alt="Mars Full" width="512"/> <img src="resources/mars2.jpg" alt="Mars 2" width="512"/>

### Limitations
Because the VOP nodes are VEX, they will not work in Karma's XPU rendering mode.

I previously thought MaterialX had fundamental limitations that made the above workflow impossible in that context.  However, it seems that MaterialX VOP nodes ***can*** be combined in a realistic way.  Tools to do this are TBD.

