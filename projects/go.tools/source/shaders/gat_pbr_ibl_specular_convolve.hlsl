#include "gat_util.hlsli"

//! The cubemap to convolve with.
TextureCube<float4> r_sourceCube : register(t0);
//! The results of the previous iteration.
TextureCube<float4> r_previousColNorm : register(t1);

//! The result of this iteration.
RWTexture2DArray<float4> r_outCube : register(u0);

/*!
    Shader constants.
 */
cbuffer Constants : register(b0)
{
    //! The current MIP-level to process.
    uint g_mipLevel;
    //! The total number of MIPs in the cubemap.
    uint g_numMips;
    //! The size of a current MIP-face.
    uint g_mipSize;
    //! The current iteration.
    uint g_iteration;
    // >
    //! The total number of iterations.
    uint g_numIterations;
    //! Padding.
    uint _padding[3];
};


/*!
    Pre-filters an environment map.

    \param sourceCube The source cubemap.
    \param alpha The surface roughness.
    \param R The reflection vector.

    \return The filtered color (RGB) and normalization weight (A).
 */
float4 ibl_specular_convolve(TextureCube<float4> sourceCube, float alpha, float3 R)
{
    // Assume head-on view
    float3 N = R;
    float3 V = R;

    // The output color
    float3 prefilteredColor = 0;

    float totalWeight = 0.f;
    const uint kNumSamples = 128;

    uint begin = g_iteration * kNumSamples;
    uint end = begin + kNumSamples;
    uint totalSampleCount = g_numIterations * kNumSamples;

    // Filtered Importance Sampling precalc
    uint envWidth, envHeight;
    sourceCube.GetDimensions(envWidth, envHeight);
    float envMapPrecalc = 0.5 * log2((envWidth * envHeight) / float(kNumSamples));

    for(uint i = begin; i < end; i++)
    {
        // Grab a uniform rnd number
        float2 Xi = hammersley(i, totalSampleCount);

        // Importance sample the specular D term (e.g. GGX)
        float4 hpdf = importance_sample_specular_d(Xi, alpha, N);
        float3 H = hpdf.xyz;
        float pdf = hpdf.w;

        // Grab the microfacet reflection vector
        float3 L = 2 * dot(V, H) * H - V;

        // And compute the lighting
        float NoL = saturate(dot(N, L));
        if(NoL > 0)
        {
            // Compute the LoD
            float lod = 0; // compute_lod(H, pdf, kNumSamples, envMapPrecalc);

            // Li * cos(theta)
            prefilteredColor += sourceCube.SampleLevel(s_pointClampSampler, L, lod).rgb * NoL;
            totalWeight += NoL;
        }
    }

    return float4(prefilteredColor, totalWeight);
}


/*!
    Convolves the cubemap face with the specular D term and combines the outputs.
 */
[numthreads(1, 1, 1)]
void main(uint3 pixel : SV_DispatchThreadID)
{
    // Convert the current MIP level to a roughness value
    float roughness = roughness_from_mip(g_mipLevel, g_numMips);

    // Remap the current pixel to NDC space
    float2 zeroToOne = pixel.xy / float2(g_mipSize, g_mipSize);
    float2 ndc = (zeroToOne - float2(0.5f, 0.5f)) * float2(2.f, -2.f);
    
    // The z value encodes the face to process
    uint faceIndex = pixel.z;

    // Figure out what the reflection vector is
    // NOTE: this looks weird but it works, properly figure out the orientation
    // someday and make this nice!
    float3 R = 0;
    // +X
    if(faceIndex == 0) R = float3(1, ndc.yx * float2(1, -1));
    // -X
    else if(faceIndex == 1) R = float3(-1, ndc.yx);
    // +Y
    else if(faceIndex == 2) R = float3(ndc.x, 1, ndc.y * -1);
    // -Y
    else if(faceIndex == 3) R = float3(ndc.x, -1, ndc.y);
    // +Z
    else if(faceIndex == 4) R = float3(ndc.xy, 1);
    // -Z
    else if(faceIndex == 5) R = float3(ndc.xy, -1);

    R = normalize(R);

    // Remap roughness
    roughness *= roughness;

    // Prefilter the cubemap
    float4 colNorm = ibl_specular_convolve(r_sourceCube, roughness, R);

    // Combine with previous results
    uint3 targetPixel = uint3(pixel.xy, faceIndex);

    float4 prevColNorm = r_previousColNorm.SampleLevel(s_pointClampSampler, R, g_mipLevel);
    float4 outColNorm = colNorm + prevColNorm;

    // Apply the normalization weight if this is the last iteration
    if(g_iteration >= g_numIterations - 1)
    {
        outColNorm.rgb /= outColNorm.a;
        outColNorm.a = 1.f;
    }

    // Write out the result
    r_outCube[targetPixel] = outColNorm;
}