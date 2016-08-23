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
    \file golib.h
    \brief Go interface header file.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


// Root
#include "go/variables.h"

// Core
#include "go/core/allocator.h"
#include "go/core/byte_step_view.h"
#include "go/core/common.h"
#include "go/core/exception.h"
#include "go/core/interpolator.h"
#include "go/core/log.h"
#include "go/core/noise_filter.h"
#include "go/core/optional.h"
#include "go/core/path_utility.h"
#include "go/core/performance_counter.h"
#include "go/core/platform_info.h"
#include "go/core/pool_allocator.h"
#include "go/core/runtime_configuration.h"
#include "go/core/semaphore.h"
#include "go/core/stack_allocator.h"
#include "go/core/string_utility.h"
#include "go/core/timer.h"
#include "go/core/timer_pool.h"

// Design
#include "go/design/tree.h"
#include "go/design/observer.h"
#include "go/design/singleton.h"

// Environment
#include "go/environment/manager.h"
#include "go/environment/variable.h"

// File-system
#include "go/filesystem/views/native/gmd_file.h"
#include "go/filesystem/async.h"
#include "go/filesystem/file.h"
#include "go/filesystem/system.h"

// Framework
#include "go/framework/application.h"

// Graphics
#include "go/graphics/api/buffer.h"
#include "go/graphics/api/blend_state.h"
#include "go/graphics/api/display.h"
#include "go/graphics/api/shader.h"
#include "go/graphics/api/texture.h"
#include "go/graphics/api/ui/system.h"

#include "go/graphics/entity.h"
#include "go/graphics/entity_component_manager.h"
#include "go/graphics/scene.h"
#include "go/graphics/scene_manager.h"
#include "go/graphics/intersection_tests.h"

// HID
#include "go/hid/input_action_map.h"
#include "go/hid/input_context.h"
#include "go/hid/input_dispatcher.h"
#include "go/hid/input_event.h"
#include "go/hid/system_input_driver.h"

// Util
#include "go/util/rapidxml_helper.h"


GO_BEGIN_NAMESPACE

/*!
    Initializes the Go API.

    Call this function once before accessing any of the services provided by Go.
    The function only succeeds if all systems were initialized properly.
    
    \param runtimeConfig Runtime configuration.
 */
GO_API void initialize(runtime_configuration *runtimeConfig);
/*!
    Initializes the Go API.

    Call this function once before accessing any of the services provided by Go.
    The function only succeeds if all systems were initialized properly.
    The function will load the runtime configuration from the current environment settings.
 */
GO_API void initialize();
//! Shuts the Go library down.
GO_API void shutdown();

GO_END_NAMESPACE
