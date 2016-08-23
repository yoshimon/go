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
    \file core/platform_info.h
    \brief Contains a class for querying information about the current platform.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/platform/windows.h"


GO_BEGIN_NAMESPACE
 
//! A class, which provides information about the current platform.
class GO_API platform_info
{
    GO_DECLARE_NON_COPYABLE(platform_info)
public:
    //! A structure, which holds information about the display controller.
    struct video_info
    {
        //! Number of bits used to display each pixel.
        uint32_t currentBitsPerPixel;
        //! Current number of pixels in the horizontal direction (x-axis) of the display.
        uint32_t currentHorizontalResolution;
        //! Current number of pixels in the vertical direction (y-axis) of the display.
        uint32_t currentVerticalResolution;
        /*!
            Frequency at which the video controller refreshes the image for the monitor.
            A value of 0 (zero) indicates the default rate is being used, while 0xFFFFFFFF
            indicates the optimal rate is being used.
         */
        uint32_t currentRefreshRate;
        //! Maximum refresh rate of the video controller in hertz.
        uint32_t maxRefreshRate;
        //! Short description of the object - a one-line string.
        std::string caption;
        //! Label by which the object is known.
        std::string name;
    };
    //! A structure, which holds information about the operating systems.
    struct memory_info
    {
        //! Total capacity of the physical memory - in bytes.
        uint64_t capacity;
        //! Number of memory modules.
        uint32_t numberOfModules;
    };
    //! A structure, which holds information about the operating systems.
    struct os_info
    {
        //! Architecture of the operating system, as opposed to the processor.
        std::string architecture;
        //! Short description of the object - a one-line string.
        std::string caption;
        //! Version number of the operating system.
        std::string version;
    };
    //! A structure, which holds information about the CPU.
    struct cpu_info
    {
        //! The current clock speed of the CPU.
        uint32_t currentClockSpeed;
        //! The maximum clock speed of the CPU.
        uint32_t maxClockSpeed;
        //! On a 32-bit processor, the value is 32 and on a 64-bit processor it is 64.
        uint32_t dataWidth;
        //! Number of cores for the current instance of the processor.
        uint32_t numberOfCores;
        /*!
            Number of logical processors for the current instance of the processor. For processors
            capable of hyperthreading, this value includes only the processors which have
            hyperthreading enabled.
         */
        uint32_t numberOfLogicalProcessors;
        /*!
            Size of the Level 2 processor cache in kilobytes. A Level 2 cache is an external memory
            area that has a faster access time than the main RAM memory.
         */
        uint32_t l2CacheSize;
        /*!
            Clock speed of the Level 2 processor cache in MHz. A Level 2 cache is an external memory
            area that has a faster access time than the main RAM memory.
         */
        uint32_t l2CacheSpeed;
        /*!
            Size of the Level 3 processor cache in kilobytes. A Level 3 cache is an external memory
            area that has a faster access time than the main RAM memory.
         */
        uint32_t l3CacheSize;
        /*!
            Clock speed of the Level 3 processor cache in MHz. A Level 3 cache is an external memory
            area that has a faster access time than the main RAM memory.
         */
        uint32_t l3CacheSpeed;
        //! Short description of an object (a one-line string).
        std::string caption;
    };
    //! A list of video controller information structures.
    typedef std::vector<video_info> video_info_vector;
public:
    //! Constructor.
    platform_info();
public:
    /*!
        Returns the CPU information descriptor.

        \return The CPU information descriptor.
     */
    const cpu_info &cpu() const noexcept;
    /*!
        Returns the OS information descriptor.

        \return The OS information descriptor.
     */
    const os_info &os() const noexcept;
    /*!
        Returns the memory information descriptor.

        \return The memory information descriptor.
     */
    const memory_info &memory() const noexcept;
    /*!
        Returns the display information descriptor.

        \return The display information descriptor.
     */
    const video_info_vector &video() const noexcept;
private:
    /*!
        Initializes WMI.

        \return If WMI was successfully initialized, the return value is true. Otherwise, the return
        value is false.
     */
    bool initialize_wmi();
    //! Queries information about the CPU.
    void query_cpu_info();
    //! Queries information about the operating system.
    void query_os_info();
    //! Queries information about the physical memory.
    void query_memory_info();
    //! Queries information about the video controllers.
    void query_video_info();
private:
    //! WMI locator object.
    Microsoft::WRL::ComPtr<IWbemLocator> m_locator;
    //! WMI service object.
    Microsoft::WRL::ComPtr<IWbemServices> m_services;
    //! CPU information.
    cpu_info m_cpuInfo;
    //! OS information.
    os_info m_osInfo;
    //! Memory information.
    memory_info m_memoryInfo;
    //! Video information.
    video_info_vector m_videoInfo;
};


//! A global instance to access information about the platform.
extern GO_API platform_info *the_platform_info;

GO_END_NAMESPACE
