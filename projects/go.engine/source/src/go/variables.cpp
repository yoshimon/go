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


#include "go/stdafx.h"


#include "go/variables.h"


#include "go/environment/manager.h"


// ============================================================================== //
// Global definitions
// ============================================================================== //

GO_DEFINE_CVAR(application_window, "0", "The application window handle.", go::variable::nonpersistent | go::variable::permanent)

GO_DEFINE_CVAR(display_mode, "0", "The display mode.", go::variable::permanent)
GO_DEFINE_CVAR(display_gamma, "2.2", "The display gamma.", go::variable::permanent)

GO_DEFINE_CVAR(fullscreen_viewport, "1920x1200", "The size of the display viewport in fullscreen-mode.", go::variable::permanent)
GO_DEFINE_CVAR(windowed_viewport, "1280x720", "The size of the display viewport in windowed-mode.", go::variable::permanent)

GO_DEFINE_CVAR(mouse_speed_scale_x, "1", "The horizontal scaling factor for the mouse cursor speed.", go::variable::permanent)
GO_DEFINE_CVAR(mouse_speed_scale_y, "1", "The vertical scaling factor for the mouse cursor speed.", go::variable::permanent)

GO_DEFINE_CVAR(texture_filter, "1", "The texture filter to use. (0 = linear, 1 = anisotropic)", go::variable::permanent)

GO_DEFINE_CVAR(ssao_strength, "3", "The strength of the real-time screen-space ambient occlusion.", go::variable::permanent)

GO_DEFINE_CVAR(volumetric_lighting_quality, "1", "The quality of the volumetric lighting.", go::variable::permanent)

GO_DEFINE_CVAR(default_material_name, "engine\\default", "The name of the default material.", go::variable::permanent)

GO_DEFINE_CVAR(gpu_profiling, "1", "State flag, indicating whether GPU profiling is enabled.", go::variable::permanent)

GO_DEFINE_CVAR(path_assets, GO_PATH_SYMBOL_WORLD_BASE, "The asset base path.", go::variable::nonpersistent | go::variable::permanent)
GO_DEFINE_CVAR(path_shaders, GO_PATH_SYMBOL_DATA "shaders\\", "The shader base path.", go::variable::nonpersistent | go::variable::permanent)
GO_DEFINE_CVAR(path_ui_base, GO_PATH_SYMBOL_DATA "ui\\", "The UI base path.", go::variable::nonpersistent | go::variable::permanent)
GO_DEFINE_CVAR(path_world_base, GO_PATH_SYMBOL_DATA "world\\", "The world base path.", go::variable::nonpersistent | go::variable::permanent)


// ============================================================================== //
// Global functions
// ============================================================================== //

void go::internal::initialize_all_environment_variables()
{
    GO_ADD_CVAR(application_window);
    GO_ADD_CVAR(default_material_name);
    GO_ADD_CVAR(display_gamma);
    GO_ADD_CVAR(display_mode);
    GO_ADD_CVAR(gpu_profiling);
    GO_ADD_CVAR(fullscreen_viewport);
    GO_ADD_CVAR(mouse_speed_scale_x);
    GO_ADD_CVAR(mouse_speed_scale_y);
    GO_ADD_CVAR_WITH_PATH_SYMBOL(path_assets, GO_PATH_SYMBOL_NAME_ASSETS);
    GO_ADD_CVAR_WITH_PATH_SYMBOL(path_shaders, GO_PATH_SYMBOL_NAME_SHADER);
    GO_ADD_CVAR_WITH_PATH_SYMBOL(path_ui_base, GO_PATH_SYMBOL_NAME_UI_BASE);
    GO_ADD_CVAR_WITH_PATH_SYMBOL(path_world_base, GO_PATH_SYMBOL_NAME_WORLD_BASE);
    GO_ADD_CVAR(ssao_strength);
    GO_ADD_CVAR(texture_filter);
    GO_ADD_CVAR(volumetric_lighting_quality);
    GO_ADD_CVAR(windowed_viewport);
}