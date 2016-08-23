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


#include "go/core/platform_info.h"


#include <comdef.h>


GO platform_info *GO the_platform_info = nullptr;


// ================================================================================ //
// go::platform_info member functions
// ================================================================================ //

go::platform_info::platform_info()
{
    if(initialize_wmi())
    {
        query_cpu_info();
        query_os_info(); 
        query_memory_info();
        query_video_info();
    }
}


bool go::platform_info::initialize_wmi()
{
    if(FAILED(CoCreateInstance(CLSID_WbemLocator, NULL, CLSCTX_ALL, IID_PPV_ARGS(&m_locator))))
    {  
        return false;  
    }

    if(FAILED(m_locator->ConnectServer(L"root\\CIMV2", NULL, NULL, NULL, WBEM_FLAG_CONNECT_USE_MAX_WAIT, NULL, NULL, &m_services)))  
    {
        return false;
    }

    return true;
}


std::string convert_bstr_to_string(const BSTR &bstr)
{
    std::string s;
    auto tmp = _com_util::ConvertBSTRToString(bstr);
    if(tmp)
    {
        s.assign(tmp);
        delete[] tmp;
    }
    return s;
}


void go::platform_info::query_cpu_info()
{
    Microsoft::WRL::ComPtr<IEnumWbemClassObject> eco;  
    if(FAILED(m_services->ExecQuery(L"WQL", L"SELECT * FROM Win32_Processor", WBEM_FLAG_FORWARD_ONLY, NULL, &eco)))
    {
        return;
    }

    Microsoft::WRL::ComPtr<IWbemClassObject> clsObj;
    int numElems;
    if(eco->Next(WBEM_INFINITE, 1, &clsObj, (ULONG *)&numElems) != WBEM_S_FALSE)  
    {
        VARIANT vRet;  
        VariantInit(&vRet);
        m_cpuInfo.currentClockSpeed = SUCCEEDED(clsObj->Get(L"CurrentClockSpeed", 0, &vRet, NULL, NULL)) ? vRet.uintVal : 0;
        m_cpuInfo.maxClockSpeed = SUCCEEDED(clsObj->Get(L"MaxClockSpeed", 0, &vRet, NULL, NULL)) ? vRet.uintVal : 0;
        m_cpuInfo.dataWidth = SUCCEEDED(clsObj->Get(L"DataWidth", 0, &vRet, NULL, NULL)) ? vRet.uiVal : 0;
        m_cpuInfo.numberOfCores = SUCCEEDED(clsObj->Get(L"NumberOfCores", 0, &vRet, NULL, NULL)) ? vRet.uintVal : 0;
        m_cpuInfo.numberOfLogicalProcessors = SUCCEEDED(clsObj->Get(L"NumberOfLogicalProcessors", 0, &vRet, NULL, NULL)) ? vRet.uintVal : 0;
        m_cpuInfo.caption = SUCCEEDED(clsObj->Get(L"Caption", 0, &vRet, NULL, NULL)) ? convert_bstr_to_string(vRet.bstrVal) : "";
        m_cpuInfo.l2CacheSize = SUCCEEDED(clsObj->Get(L"L2CacheSize", 0, &vRet, NULL, NULL)) ? vRet.uintVal : 0;
        m_cpuInfo.l2CacheSpeed = SUCCEEDED(clsObj->Get(L"L2CacheSpeed", 0, &vRet, NULL, NULL)) ? vRet.uintVal : 0;
        m_cpuInfo.l3CacheSize = SUCCEEDED(clsObj->Get(L"L3CacheSize", 0, &vRet, NULL, NULL)) ? vRet.uintVal : 0;
        m_cpuInfo.l3CacheSpeed = SUCCEEDED(clsObj->Get(L"L3CacheSpeed", 0, &vRet, NULL, NULL)) ? vRet.uintVal : 0;
        VariantClear(&vRet);
    }
}


void go::platform_info::query_os_info()
{
    Microsoft::WRL::ComPtr<IEnumWbemClassObject> eco;  
    if(FAILED(m_services->ExecQuery(L"WQL", L"SELECT * FROM Win32_OperatingSystem", WBEM_FLAG_FORWARD_ONLY, NULL, &eco)))
    {
        return;
    }

    Microsoft::WRL::ComPtr<IWbemClassObject> clsObj;
    int numElems;
    if(eco->Next(WBEM_INFINITE, 1, &clsObj, (ULONG *)&numElems) != WBEM_S_FALSE)  
    {
        VARIANT vRet;
        VariantInit(&vRet);
        m_osInfo.caption = SUCCEEDED(clsObj->Get(L"Caption", 0, &vRet, NULL, NULL)) ? convert_bstr_to_string(vRet.bstrVal) : "";
        m_osInfo.version = SUCCEEDED(clsObj->Get(L"Version", 0, &vRet, NULL, NULL)) ? convert_bstr_to_string(vRet.bstrVal) : "";
        m_osInfo.architecture = SUCCEEDED(clsObj->Get(L"OSArchitecture", 0, &vRet, NULL, NULL)) ? convert_bstr_to_string(vRet.bstrVal) : "";
        VariantClear(&vRet);
    }
}


void go::platform_info::query_memory_info()
{
    Microsoft::WRL::ComPtr<IEnumWbemClassObject> eco;  
    if(FAILED(m_services->ExecQuery(L"WQL", L"SELECT * FROM Win32_PhysicalMemory", WBEM_FLAG_FORWARD_ONLY, NULL, &eco)))
    {
        return;
    }

    Microsoft::WRL::ComPtr<IWbemClassObject> clsObj;
    int numElems;
    m_memoryInfo.capacity = 0;
    m_memoryInfo.numberOfModules = 0;
    while(eco->Next(WBEM_INFINITE, 1, &clsObj, (ULONG *)&numElems) != WBEM_S_FALSE)  
    {
        VARIANT vRet;
        VariantInit(&vRet);
        m_memoryInfo.capacity += SUCCEEDED(clsObj->Get(L"Capacity", 0, &vRet, NULL, NULL)) ? _atoi64(convert_bstr_to_string(vRet.bstrVal).c_str()) : 0;
        VariantClear(&vRet);
        ++(m_memoryInfo.numberOfModules);
    }
}


void go::platform_info::query_video_info()
{
    Microsoft::WRL::ComPtr<IEnumWbemClassObject> eco;  
    if(FAILED(m_services->ExecQuery(L"WQL", L"SELECT * FROM Win32_VideoController", WBEM_FLAG_FORWARD_ONLY, NULL, &eco)))
    {
        return;
    }

    Microsoft::WRL::ComPtr<IWbemClassObject> clsObj;
    int numElems;
    m_videoInfo.clear();
    while(eco->Next(WBEM_INFINITE, 1, &clsObj, (ULONG *)&numElems) != WBEM_S_FALSE)  
    {
        VARIANT vRet;
        VariantInit(&vRet);
        video_info info;
        info.currentBitsPerPixel = SUCCEEDED(clsObj->Get(L"CurrentBitsPerPixel", 0, &vRet, NULL, NULL)) ? vRet.uintVal : 0;
        info.caption = SUCCEEDED(clsObj->Get(L"Caption", 0, &vRet, NULL, NULL)) ? convert_bstr_to_string(vRet.bstrVal) : "";
        info.name = SUCCEEDED(clsObj->Get(L"Name", 0, &vRet, NULL, NULL)) ? convert_bstr_to_string(vRet.bstrVal) : "";
        info.currentHorizontalResolution = SUCCEEDED(clsObj->Get(L"CurrentHorizontalResolution", 0, &vRet, NULL, NULL)) ? vRet.uintVal : 0;
        info.currentRefreshRate = SUCCEEDED(clsObj->Get(L"CurrentRefreshRate", 0, &vRet, NULL, NULL)) ? vRet.intVal : 0;
        info.maxRefreshRate = SUCCEEDED(clsObj->Get(L"MaxRefreshRate", 0, &vRet, NULL, NULL)) ? vRet.intVal : 0;
        info.currentVerticalResolution = SUCCEEDED(clsObj->Get(L"CurrentVerticalResolution", 0, &vRet, NULL, NULL)) ? vRet.uintVal : 0;
        VariantClear(&vRet);

        m_videoInfo.push_back(std::move(info));
    }
}


const go::platform_info::cpu_info &go::platform_info::cpu() const noexcept
{
    return m_cpuInfo;
}


const go::platform_info::os_info &go::platform_info::os() const noexcept
{
    return m_osInfo;
}


const go::platform_info::memory_info &go::platform_info::memory() const noexcept
{
    return m_memoryInfo;
}


const go::platform_info::video_info_vector &go::platform_info::video() const noexcept
{
    return m_videoInfo;
}
