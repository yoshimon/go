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


// ================================================================================ //
// gfx_render_command_buffer member functions
// ================================================================================ //

template<class RenderCommand>
gfx_render_command_buffer<RenderCommand>::gfx_render_command_buffer(size_t bufferSizeHint)
{
    resize(bufferSizeHint);
}


template<class RenderCommand>
void gfx_render_command_buffer<RenderCommand>::resize(size_t bufferSizeHint)
{
    m_opaqueCommands.reserve(bufferSizeHint);
    m_opaqueCommandIndices.reserve(bufferSizeHint);

    m_transparentCommands.reserve(bufferSizeHint);
    m_transparentCommandIndices.reserve(bufferSizeHint);

    m_instanceCountBuffer.reserve(bufferSizeHint);
    //m_instanceCache.reserve(bufferSizeHint);
    m_nextInstanceID = 0;
}


template<class RenderCommand>
void gfx_render_command_buffer<RenderCommand>::enqueue(const typename render_command_t::enqueue_command_t &command)
{
    render_command_t data(command);
    auto viewZ = command.viewZ;

    for(auto &mesh : command.model->unskinnedMeshes)
    {
        enqueue_mesh(data, mesh, viewZ);
    }

    for(auto &mesh : command.model->skinnedMeshes)
    {
        enqueue_mesh(data, mesh, viewZ);
    }
}


template<class RenderCommand>
void gfx_render_command_buffer<RenderCommand>::enqueue_mesh(render_command_t &data, const gfx_mesh &mesh, float viewZ)
{
    if(!mesh.material)
    {
        return;
    }

    // Assume this is a new instance
    data.instanceID = ++m_nextInstanceID;

    // Use mesh address as hash key
    auto &&pib = m_instanceCache.insert(std::make_pair(reinterpret_cast<uintptr_t>(&mesh), data.instanceID));
    if(!pib.second)
    {
        // Already enqueued, grab existing ID
        data.instanceID = pib.first->second;
    }

    // Figure out which buffer to post this to
    auto blendMode = mesh.material->blendMode;
    command_vector_t *commandQueue;
    command_index_vector_t *commandIdxQueue;
    if(blendMode == gfx_blend_mode::opaque)
    {
        commandQueue = &m_opaqueCommands;
        commandIdxQueue = &m_opaqueCommandIndices;

        // NOTE: we reuse the blendMode field here to sort by "cutout"-value
        blendMode = (gfx_blend_mode)mesh.material->requiresAlphaTest;
    }
    else
    {
        commandQueue = &m_transparentCommands;
        commandIdxQueue = &m_transparentCommandIndices;
    }

    // Copy command data
    data.indexCount = mesh.indexCount;
    data.startIndex = mesh.startIndex;
    data.startVertex = mesh.startVertex;

    // Setup the command key
    gfx_render_command_key key(mesh.material->id, viewZ, blendMode);

    auto idx = commandQueue->emplace_back(key, data);
    commandIdxQueue->push_back(idx - commandQueue->begin());
}


template<class RenderCommand>
void gfx_render_command_buffer<RenderCommand>::clear()
{
    m_opaqueCommands.clear();
    m_opaqueCommandIndices.clear();
    m_transparentCommands.clear();
    m_transparentCommandIndices.clear();
    m_instanceCache.clear();
    m_nextInstanceID = 0;
}


template<class RenderCommand>
void gfx_render_command_buffer<RenderCommand>::sort()
{
    auto sortOpaquePred = [this](size_t ia, size_t ib)
    {
        auto &a = m_opaqueCommands[ia];
        auto &b = m_opaqueCommands[ib];
        auto &ak = std::get<0>(a);
        auto &bk = std::get<0>(b);

        {
            // Cutout
            // NOTE: we reuse the blendMode field here instead of adding a new "cutout"-field
            // The renderer knows that this buffer is not used for blending so it won't bind the corresponding blendstate
            auto i = ak.blend_mode();
            auto j = bk.blend_mode();
            if(i != j)
            {
                return i < j;
            }
        }

        {
            // InstanceID
            auto i = std::get<1>(a).instanceID;
            auto j = std::get<1>(b).instanceID;
            if(i != j)
            {
                return i < j;
            }
        }

        {
            // Material ID
            auto i = ak.material_id();
            auto j = bk.material_id();
            if(i != j)
            {
                return i < j;
            }
        }

        // NOTE: sorting by depth is most likely not going to buy us much since we are using unclustered instancing

        return false;
    };

    auto sortTransPred = [this](size_t ia, size_t ib)
    {
        auto &a = m_transparentCommands[ia];
        auto &b = m_transparentCommands[ib];
        auto &ak = std::get<0>(a);
        auto &bk = std::get<0>(b);

        // NOTE: doesn't matter what blend-mode this is, just sort for correct transparency
        {
            // Position
            auto i = ak.depth();
            auto j = bk.depth();
            if(i != j)
            {
                return i > j; // NOTE: back to front
            }
        }

        {
            // Translucency
            auto i = ak.blend_mode();
            auto j = bk.blend_mode();
            if(i != j)
            {
                return i < j;
            }
        }

        {
            // Material ID
            auto i = ak.material_id();
            auto j = bk.material_id();
            if(i != j)
            {
                return i < j;
            }
        }

        // NOTE: we don't support instancing for transparent geometry
        return false;
    };

    // Opaque sort
    {
        // Sort draw commands
        std::sort(m_opaqueCommandIndices.begin(), m_opaqueCommandIndices.end(), sortOpaquePred);

        // Build skip buffer
        // Find sequences: 000011111222223344444....
        m_instanceCountBuffer.clear();
        int i = 0;
        auto lastIndex = (int)m_opaqueCommands.size() - 1;
        if(lastIndex == 0)
        {
            // Edge case
            m_instanceCountBuffer.push_back(1);
        }
        else
        {
            while(i < lastIndex) // One before last
            {
                auto currCmdIndex = m_opaqueCommandIndices[i];
                auto &currCmd = m_opaqueCommands[currCmdIndex];
                auto currCmdInstanceID = std::get<1>(currCmd).instanceID;

                // j = index of next entry that differs from i
                int j = i + 1;
                do
                {
                    auto nextCmdIndex = m_opaqueCommandIndices[j];
                    auto nextCmdInstanceID = std::get<1>(m_opaqueCommands[nextCmdIndex]).instanceID;
                    if(currCmdInstanceID != nextCmdInstanceID)
                    {
                        // Next instance block
                        break;
                    }

                    ++j;
                } while(j <= lastIndex);

                // Add instance block
                m_instanceCountBuffer.push_back(j - i);

                // Advance to next instance block
                i = j;
            }

            if(i == lastIndex)
            {
                m_instanceCountBuffer.push_back(1);
            }
        }
    }

    // Transparency sort
    std::sort(m_transparentCommandIndices.begin(), m_transparentCommandIndices.end(), sortTransPred);
}


template<class RenderCommand>
auto gfx_render_command_buffer<RenderCommand>::opaque_commands() const noexcept -> const command_vector_t &
{
    return m_opaqueCommands;
}


template<class RenderCommand>
auto gfx_render_command_buffer<RenderCommand>::opaque_command_order() const noexcept -> const command_index_vector_t &
{
    return m_opaqueCommandIndices;
}


template<class RenderCommand>
auto gfx_render_command_buffer<RenderCommand>::transparent_commands() const noexcept -> const command_vector_t &
{
    return m_transparentCommands;
}


template<class RenderCommand>
auto gfx_render_command_buffer<RenderCommand>::transparent_command_order() const noexcept -> const command_index_vector_t &
{
    return m_transparentCommandIndices;
}


template<class RenderCommand>
auto gfx_render_command_buffer<RenderCommand>::instance_count_buffer() const noexcept -> const command_index_vector_t &
{
    return m_instanceCountBuffer;
}
