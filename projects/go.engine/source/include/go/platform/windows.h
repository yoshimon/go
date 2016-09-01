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
    \file platform/windows.h
    \brief Windows include file.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


// This should always be the first #include!
#include "go/platform/targetver.h"

#include "go/defines.h"
#include "go/core/exception.h"

// Exclude rarely used stuff.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <tchar.h>

#ifdef GO_USE_DIRECT3D11
// Windows 8
#include <dxgidebug.h>

// DirectX stuff
#include <d3d11.h>
#include <dxgi.h>
#include <d3d11shader.h>
#include <d3dcompiler.h>
#include <d3d9.h> // D3DPERF API
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d9.lib")
#endif // GO_USE_DIRECT3D11


#include <wincodec.h>
#include <DirectXMath.h>


// XInput
#include <XInput.h>


// WRT
#include <wrl/client.h>

// WMI
#include <WbemCli.h>


#pragma comment(lib, "Shlwapi.lib")
#include "Shlwapi.h"


GO_BEGIN_NAMESPACE

//! A exception class, which is thrown when an HRESULT indicates failure.
class GO_API hresult_error : public GO exception
{
public:
    //! The base class.
    typedef GO exception base_class_type;
public:
    /*!
        Constructs the object and initializes it.

        \param whatArg Explanatory string.
        \param errorCode The HRESULT error-code.
     */
    hresult_error(const std::string &whatArg, HRESULT errorCode);
    //! \overload
    hresult_error(const char *whatArg, HRESULT errorCode);
public:
    /*!
        Returns the HRESULT error-code.

        \return The HRESULT error-code.
     */
    HRESULT error_code() const noexcept;
private:
    //! The HRESULT error code.
    HRESULT m_errorCode;
};


//! A structure, which represents a Windows file handle.
struct windows_file_handle
{
    /*!
        Constructor.

        \param value The handle value.
     */
    windows_file_handle(HANDLE value) noexcept : handle(value) {}
    //! \overload
    windows_file_handle() = default;
    //! Destructor.
     ~windows_file_handle() { if(handle != INVALID_HANDLE_VALUE) { CloseHandle(handle); }  }
    //! Returns the handle.
    operator HANDLE() const noexcept{ return handle; }
    //! Releases the internal handle value without closing it.
    void yield() { handle = INVALID_HANDLE_VALUE; }
    //! The file handle.
    HANDLE handle = INVALID_HANDLE_VALUE;
};


/*!
    Throws an exception, if the HRESULT value indicates failure.

    \param fileName The filename to log.
    \param line The line to log.
    \param hr The HRESULT.
    \param logMsg The message to write to the log file.
 */
GO_API void throw_if_hresult_failed(const char *fileName, int32_t line, HRESULT hr, std::string logMsg);


/*!
    Stores a matrix into a float4x3 (row-major) array.

    DirectX-Math extension.

    \param pDestination The destination array.
    \param M The matrix to store.
 */
inline void XM_CALLCONV XMStoreFloat3x4(DirectX::XMFLOAT4X3 *pDestination, DirectX::FXMMATRIX M)
{
    DirectX::XMStoreFloat4((DirectX::XMFLOAT4 *)&pDestination->_11, M.r[0]);
    DirectX::XMStoreFloat4((DirectX::XMFLOAT4 *)&pDestination->_22, M.r[1]);
    DirectX::XMStoreFloat4((DirectX::XMFLOAT4 *)&pDestination->_33, M.r[2]);
}

GO_END_NAMESPACE
