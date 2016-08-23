function shader_class()
	return 'go::gfx_shader'
end

function cbuffer_class()
	return 'go::gfx_constant_buffer';
end

function resource_class(hlslType)
	if string.find(hlslType, 'Texture') ~= nil then
		return 'go::gfx_texture'
	elseif string.find(hlslType, 'Sampler') ~= nil then
		return 'go::gfx_sampler_state'
	else
		return 'go::gfx_structured_buffer'
	end
end

function precompiled_header()
	return 'go/stdafx.h'
end

function compiled_shader_filename(shaderName, profile)
	return shaderName .. '_' .. string.sub(profile, 1, 2) .. '.cso'
end

function cpp_compiled_shader_filename(shaderName, profile)
	return 'GO_PATH_SYMBOL_DATA "' .. shaderName .. '_' .. string.sub(profile, 1, 2) .. '.cso"'
end

function forward_declare_type(typeName)
	return 'class ' .. typeName .. ';'
end

function include_user_types()
	return
	[[
#include "go/graphics/d3d11/device.h"
#include "go/graphics/d3d11/renderer.h"
#include "go/graphics/d3d11/buffer.h"
#include "go/graphics/d3d11/texture.h"
	]]
end

function load_shader(dstObj, fileName, profile)
	-- return dstObj .. ' = go::the_gfx_device->shader_mgr().get(' .. fileName .. ');'
    return dstObj .. ' = nullptr;';
end

function unload_shader(obj)
	return '' -- Destructor cleans up object when shaderMgr gets destroyed
end

function bind_shader(shader, obj)
	return 'go::the_gfx_renderer->bind_' .. shader .. '(' .. obj .. ');'
end

function bind_resource(shader, obj, slot, resourceIndex)
	return 'go::the_gfx_renderer->bind_' .. shader .. '_resource(' .. slot .. ', ' .. obj .. ', ' .. resourceIndex .. ');'
end

function bind_sampler(shader, obj, slot)
	return 'go::the_gfx_renderer->bind_' .. shader .. '_sampler(' .. slot .. ', ' .. obj .. ');'
end

function bind_constants(shader, obj, slot)
	return 'go::the_gfx_renderer->bind_' .. shader .. '_constants(' .. slot .. ', ' .. obj .. ');'
end

function bind_uav(shader, obj, slot, resourceIndex, initialCount)
	return 'go::the_gfx_renderer->bind_' .. shader .. '_uav(' .. slot .. ', ' .. obj .. ', ' .. resourceIndex .. ', ' .. initialCount .. ');'
end