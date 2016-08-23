//! The source cubemap in the flattened layout.
Texture2D<float4> r_sourceCube : register(t0);
//! The destination cubemap.
RWTexture2DArray<float4> r_destCube : register(u0);

//! Number of threads in X.
#define NUM_THREADS_X 32
//! Number of threads in Y.
#define NUM_THREADS_Y 8


/*!
    Converts a 2D texture to a cubemap.

    \param sourcePixel The pixel in the source cubemap.
 */
[numthreads(NUM_THREADS_X, NUM_THREADS_Y, 1)]
void main(uint3 sourcePixel : SV_DispatchThreadID)
{
    // Read out the size of the texture
    uint3 sourceCubeDim;
    r_sourceCube.GetDimensions(0, sourceCubeDim.x, sourceCubeDim.y, sourceCubeDim.z);

    // Figure out which face this is
    uint faceSize = sourceCubeDim.x / 6;
    uint faceIndex = sourcePixel.x / faceSize;

    // Load the source cubemap
    float4 col = r_sourceCube.Load(uint3(sourcePixel.xy, 0));

    // Write the corresponding pixel in the destination cubemap
    uint3 targetPixel = uint3(sourcePixel.x - faceIndex * faceSize, sourcePixel.y, faceIndex);
    r_destCube[targetPixel] = col;
}