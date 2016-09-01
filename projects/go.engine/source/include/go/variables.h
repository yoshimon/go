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

/*! 
    \file variables.h
    \brief Initialization functions and  for environment variables.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/environment/variable.h"


GO_BEGIN_NAMESPACE

//! A variable for the application window handle.
GO_DECLARE_CVAR(application_window)

//! The current display mode.
GO_DECLARE_CVAR(display_mode)
//! The current display gamma.
GO_DECLARE_CVAR(display_gamma)
//! The size of the windowed viewport.
GO_DECLARE_CVAR(windowed_viewport)
//! The size of the fullscreen viewport.
GO_DECLARE_CVAR(fullscreen_viewport)

//! Horizontal mouse speed scale.
GO_DECLARE_CVAR(mouse_speed_scale_x)
//! Horizontal mouse speed scale.
GO_DECLARE_CVAR(mouse_speed_scale_y)

//! Texture filter for surfaces.
GO_DECLARE_CVAR(texture_filter)

//! The strength of the real-time ambient occlusion.
GO_DECLARE_CVAR(ssao_strength)

//! The quality of the volumetric lights.
GO_DECLARE_CVAR(volumetric_lighting_quality)

//! The name of the default material.
GO_DECLARE_CVAR(default_material_name)

//! Enables the GPU profiler.
GO_DECLARE_CVAR(gpu_profiling)


//! The path_assets base path.
GO_DECLARE_CVAR(path_assets)
//! The shader base path.
GO_DECLARE_CVAR(path_shaders)
//! The UI base path.
GO_DECLARE_CVAR(path_ui_base)
//! The world base path.
GO_DECLARE_CVAR(path_world_base)

GO_END_NAMESPACE


GO_BEGIN_INTERNAL_NAMESPACE

//! Initializes all environment variables by adding them to the environment manager.
void initialize_all_environment_variables();

GO_END_INTERNAL_NAMESPACE
