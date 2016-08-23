#ifndef GAT_UTIL_HLSLI
#define GAT_UTIL_HLSLI

//! A point-clamp sampler.
SamplerState s_pointClampSampler : register(s0);

//! Pi.
#define DFX_PI 3.14159265359f
//! Smallest reasonable float value for comparison.
#define DFX_FLOAT_EPSILON 0.001f

/*!
    Squares a value.

    \param x The value to square.

    \return The squared value.
 */
float sqr(float x)
{
    return x * x;
}


/*!
    Converts a MIP-level to a roughness value.

    \param mip The MIP-level.
    \param maxMipCount The maximum number of MIP-levels.

    \return The roughness value.
 */
float roughness_from_mip(float mip, float maxMipCount)
{
    // TODO:
    return lerp(0.01, 1, mip / maxMipCount);
}


/*!
    Computes Hammersley-distributed random variables.

    \param i The sample index.
    \param N The total number of samples.

    \return The Hammersley-distributed random variables.
 */
float2 hammersley(uint i, uint N)
{
    float e1 = float(i) / N;
    float e2 = (float)((double)reversebits(i) * (double)2.3283064365386963e-10); // div 0x100000000
    return float2(e1, e2);
}


/*!
    Importance samples the GGX specular D term.

    \param random Uniform random variables.
    \param alpha The surface roughness.
    \param n The surface normal.

    \return The world-space microfacet normal.
 */
float4 importance_sample_ggx(float2 random, float alpha, float3 n)
{
    // Disney roughness
    float alpha2 = alpha * alpha;

    // Importance sample
    float phi = 2 * DFX_PI * random.x;
    float cosTheta = sqrt((1 - random.y) / (1 + (alpha2 - 1) * random.y));
    float cosTheta2 = cosTheta * cosTheta;
    float sinTheta = sqrt(1 - cosTheta2);

    // Spherical coords
    float3 h;
    h.x = sinTheta * cos(phi);
    h.y = sinTheta * sin(phi);
    h.z = cosTheta;

    // Tangent to world space
    float3 upVector = abs(n.z) < 0.999 ? float3(0, 0, 1) : float3(1, 0, 0);
    float3 tangentX = normalize(cross(upVector, n));
    float3 tangentY = cross(n, tangentX);

    // Calculate the PDF for the direction
    float d = (cosTheta * alpha2 - cosTheta) * cosTheta + 1.0;
    float D = alpha2 / (DFX_PI * d * d);
    float pdf = D * cosTheta * sinTheta;

    return float4(tangentX * h.x + tangentY * h.y + n * h.z, pdf);
}


/*!
    Importance samples the specular D term.

    \param random Uniform random variables.
    \param roughness The surface roughness.
    \param n The surface normal.

    \return The world-space microfacet normal and PDF value for the sampled direction.
 */
float4 importance_sample_specular_d(float2 random, float roughness, float3 n)
{
    return importance_sample_ggx(random, roughness, n);
}

/*!
    Computes the MIP-LoD for filtered importance sampling.

    \param H The half-angle vector (the importance-sampled direction).
    \param pdf The PDF value for the sampled direction.
    \param numSamples The total number of samples used.
    \param halfLog2SizeDivN Half of the log2 value of the size of the environment map in pixels divided by the number of samples numSamples.

    \return The LoD.
 */
float compute_lod(float3 H, float pdf, float numSamples, float halfLog2SizeDivN)
{
    return max(0.0, halfLog2SizeDivN - 0.5 * log2(pdf));
}

/*!
    Computes the latlong coordinates for a vector.

    \param v The vector to compute the latlong coordinates for.
 */
float2 latlong(in float3 v)
{
    v = normalize(v);
    float theta = acos(v.y) + DFX_PI;
    float phi = atan2(v.z, v.x) + DFX_PI * 0.5;
    return float2(phi, theta) * float2(0.1591549, 0.6366198 * 0.5);
}

#endif