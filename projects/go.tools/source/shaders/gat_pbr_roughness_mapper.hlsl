Texture2D<float> roughnessTex : register(t0);
Texture2D<float2> normalTex : register(t1);

RWTexture2D<float> output : register(u0);

cbuffer Constants : register(b0)
{
    uint g_mipLevel;
    uint3 _padding;
}

inline float3 unpack_bc5(in float2 packedNormal)
{
   float3 n = float3(packedNormal.x * 2.f - 1.f, packedNormal.y * 2.f - 1.f, 0);
   n.z = sqrt(1.f - n.x * n.x - n.y * n.y);
   return n;
}


// NEUBELT & PATTINEO ACTION IN HERE! (The Order: 1886)

// ===============================================================================
// Computes a new roughness value for a single mipmap texel given a normal map
// and an existing roughness value
// ===============================================================================
float compute_roughness(in float2 texelPos, in uint mipLevel, in float roughness,
    in uint2 normalTexSize)
{
    if(mipLevel == 0)
    {
        return roughness;
    }
    else
    {
        float3 avgNormal = 0.0f;
        // Sample all normal map texels from the base mip level that are within
        // the footprint of the current mipmap texel
        const uint texelFootprint = (1 << mipLevel);
        const float2 topLeft = (-float(texelFootprint) / 2.0f) + 0.5f;
        for(uint y = 0; y < texelFootprint; ++y)
        {
            for(uint x = 0; x < texelFootprint; ++x)
            {
                float2 offset = topLeft + float2(x, y);
                float2 samplePos = (floor(texelPos + offset) % normalTexSize) + 0.5f;
                float3 sampleNormal = unpack_bc5(normalTex[samplePos]);
                avgNormal += sampleNormal;
            }
        }

        // Fit a vMF lobe to NDF for this mip texel
        avgNormal /= (texelFootprint * texelFootprint);
        float r = length(avgNormal);
        float kappa = 10000.0f;

        if(r < 1.0f)
        {
            kappa = (3 * r - r * r * r) / (1 - r * r);
        }

        // Compute the new roughness value
        return sqrt(roughness * roughness + (1.0f / kappa));
    }
}

[numthreads(1, 1, 1)]
void main(uint3 pixel : SV_DispatchThreadID)
{
    uint mipLevel = g_mipLevel;

    uint3 normalTexSize;
    normalTex.GetDimensions(0, normalTexSize.x, normalTexSize.y, normalTexSize.z);
    uint3 roughnessTexSize;
    roughnessTex.GetDimensions(mipLevel, roughnessTexSize.x, roughnessTexSize.y, roughnessTexSize.z);

    float rIn = roughnessTex.Load(uint3(pixel.xy, mipLevel));
    uint3 toNormalMap = normalTexSize / roughnessTexSize;
    float rOut = compute_roughness(pixel.xy * toNormalMap.xy, mipLevel, rIn, normalTexSize.xy);
    output[pixel.xy] = rOut;
}