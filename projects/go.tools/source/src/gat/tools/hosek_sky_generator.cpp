/*
    Go GPL Source Code
    Copyright (C) 2012 Gokhan Ozdogan.

    This file is part of the Go GPL Source Code ("Go Source Code").

    Go Source Code is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Go Source Code is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Go Source Code.  If not, see <http://www.gnu.org/licenses/>.

    In addition, the Go Source Code is also subject to certain additional terms.
    You should have received a copy of these additional terms immediately
    following the terms and conditions of the GNU General Public License which
    accompanied the Go Source Code.  If not, please request a copy in writing
    from mailto:goekhan.oezdogan@hotmail.de.
 */

#include "gat/tools/hosek_sky_generator.h"


#include "gat/files/asset_meta_file.h"
#include "gat/util/exception.h"
#include "gat/util/cmd_help.h"
#include "gat/util/converters.h"

#include <DirectXMath.h>

#include "gat/hosek_sky/ArHosekSkyModel.h"


using namespace DirectX;

GAT_DEFINE_TOOL(hosek_sky_generator, _T("hosek_sky"))


//===========================================================================
// gat::hosek_sky
//===========================================================================

void gat::hosek_sky_generator::run(int argc, const _TCHAR *argv[], const config_file &cfg)
{
    _tprintf(_T("Hosek-Wilkie Sky Generator loaded.\n"));

    auto &&turbidity = get_parameter_value(argc, argv, _T("-turbidity"), 1.0f);
    auto &&albedoR = get_parameter_value(argc, argv, _T("-albedoR"), 1.0f);
    auto &&albedoG = get_parameter_value(argc, argv, _T("-albedoG"), 1.0f);
    auto &&albedoB = get_parameter_value(argc, argv, _T("-albedoB"), 1.0f);
    auto &&sunDirX = get_parameter_value(argc, argv, _T("-x"), 0.0f);
    auto &&sunDirY = get_parameter_value(argc, argv, _T("-y"), 1.0f);
    auto &&sunDirZ = get_parameter_value(argc, argv, _T("-z"), 0.0f);
    auto &&size = get_parameter_value(argc, argv, _T("-size"), 0.0f);
    auto &&includeSun = get_parameter_value(argc, argv, _T("-includeSun"), 1);
    auto &&sunRadius = get_parameter_value(argc, argv, _T("-sunRadius"), 0.0f);
    auto &&sunColorR = get_parameter_value(argc, argv, _T("-sunColorR"), 0.0f);
    auto &&sunColorG = get_parameter_value(argc, argv, _T("-sunColorG"), 0.0f);
    auto &&sunColorB = get_parameter_value(argc, argv, _T("-sunColorB"), 0.0f);

    // Sun direction
    auto &&vSunDir = -XMVectorSet(sunDirX, sunDirY, sunDirZ, 0.0f);
    auto &&vTheta = XMVector3AngleBetweenVectors(vSunDir, g_XMIdentityR1);
    auto &&elevation = XMVectorGetX(g_XMTwoPi - vTheta);
    
    // Initialize the Hosek sky
    auto &&stateR = arhosek_rgb_skymodelstate_alloc_init(turbidity, albedoR, elevation);
    auto &&stateG = arhosek_rgb_skymodelstate_alloc_init(turbidity, albedoG, elevation);
    auto &&stateB = arhosek_rgb_skymodelstate_alloc_init(turbidity, albedoB, elevation);

    // Cosine of the sun angular radius
    auto &&cosSunAngularRadius = std::cos(sunRadius * DFX_PI / 180.0f);
    auto &&vSunColor = XMVectorSet(sunColorR, sunColorG, sunColorB, 0.0f);

    auto &&vMaxF16 = XMVectorReplicate(65000.0f);
    
    auto &&rad90 = DFX_PI * 0.5f;

    // Create an environment map from that
    for(int face = 0; face < 6; ++face)
    {
        for(int x = 0; x < size; ++x)
        {
            for(int y = 0; y < size; ++y)
            {
                // NDC in [-1,+1]
                auto &&u = float(x) / size;
                auto &&v = float(y) / size;
                auto &&ndcX = (u - 0.5f) * 2.f;
                auto &&ndcY = (v - 0.5f) * -2.f;

                XMVECTOR vSampleDir;
                switch(face)
                {
                case 0: vSampleDir = XMVectorSet(1.0f, ndcY, ndcX, 0.0f); break;
                case 1: vSampleDir = XMVectorSet(-1.0f, ndcY, -ndcX, 0.0f); break;
                case 2: vSampleDir = XMVectorSet(ndcX, 1.0f, -ndcY, 0.0f); break;
                case 3: vSampleDir = XMVectorSet(ndcX, -1.0f, ndcY, 0.0f); break;
                case 4: vSampleDir = XMVectorSet(ndcX, ndcY, 1.0f, 0.0f); break;
                case 5: vSampleDir = XMVectorSet(ndcX, ndcY, -1.0f, 0.0f); break;
                }

                vSampleDir = XMVector3Normalize(vSampleDir);

                auto &&gamma = XMVectorGetX(XMVector3AngleBetweenVectors(vSampleDir, vSunDir));
                auto &&theta = XMVectorGetX(XMVector3AngleBetweenVectors(vSampleDir, g_XMIdentityR1));

                auto &&radianceR = (float)arhosek_tristim_skymodel_radiance(stateR, theta, gamma, 0);
                auto &&radianceG = (float)arhosek_tristim_skymodel_radiance(stateG, theta, gamma, 1);
                auto &&radianceB = (float)arhosek_tristim_skymodel_radiance(stateB, theta, gamma, 2);
                auto &&vRadiance = XMVectorSet(radianceR, radianceG, radianceB, 0.0f);

                // Convert to photometric units (100% luminous efficacy at 683 lm/W)
                vRadiance *= 683.0f;

                if(includeSun)
                {
                    auto &&cosGamma = XMVectorGetX(XMVector3Dot(vSampleDir, vSunDir));
                    if(cosGamma >= cosSunAngularRadius)
                    {
                        vRadiance += vSunColor;
                    }
                }

                // Clamp to float16 to be used in-game
                vRadiance = XMVectorClamp(vRadiance, g_XMZero, vMaxF16);

                // TODO: Save out the radiance
            }
        }
    }

    // TODO: Create the texture

    // Write it out
    arhosekskymodelstate_free(stateR);
    arhosekskymodelstate_free(stateG);
    arhosekskymodelstate_free(stateB);
}