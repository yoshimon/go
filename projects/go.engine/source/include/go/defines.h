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
    \file defines.h
    \brief Preprocessor macros and global symbols.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


// Verify global macro switches
#ifndef GO_USE_DIRECT3D11
#error No renderer specified. You can specify GO_USE_DIRECT3D11 to use the Direct3D11 renderer.
#endif // GO_USE_DIRECT3D11


//! \name Temporary C++11 Macros
//! \{
#if _MSC_VER <= 1800
//! C++11: noexcept-keyword.
#define noexcept throw()
//! C++11: constexpr-keyword macro.
#define constexpr const
#endif
//! \}


//! \name 64-bit portability macros.
//! \{
#if defined(WIN32)
//! Size-type macro.
#define GO_PRINTF_SIZET ""
#else
//! Size-type macro.
#define GO_PRINTF_SIZET "I"
#endif
/*!
    Emits a string for use with printf.

    \param Placeholder The format placeholder.
 */
#define GO_PPH(Placeholder) "%" GO_PRINTF_SIZET GO_TOSTRING(Placeholder)
//! \}


//! \name Dll Macros
//! \{
#ifdef GO_EXPORT
//! Sharing macro.
#define GO_API __declspec(dllexport)
#else
//! Sharing macro.
#define GO_API __declspec(dllimport)
#endif
//! \}


//! \name Profiler Macros
//! Begins a new profiler event.
#if defined(DEBUG) || defined(_DEBUG)
#define GO_PERF_BEGIN_EVENT(Name) D3DPERF_BeginEvent(0, L#Name);
//! Ends the current profiler event.
#define GO_PERF_END_EVENT D3DPERF_EndEvent();
#else
//! Begins a new profiler event.
#define GO_PERF_BEGIN_EVENT(Name)
//! Ends the current profiler event.
#define GO_PERF_END_EVENT
#endif


//! \name File I/O Macros
//! \{
//! Maximum number of concurrent file I/O requests
#define GO_MAX_FILE_IO_REQUESTS 8192
//! \}


//! \name Exception Macros
//! \{
//! Throws an exception and writes a log entry.
#define GO_THROW(Exception, Message, ...) GO throw_exception<Exception>(__FILE__, __LINE__, Message, __VA_ARGS__)
//! Throws an exception if an HRESULT indicates failure.
#define GO_THROW_HRESULT(HR, Message, ...) { HRESULT __myTempHR = HR; if(FAILED(__myTempHR)) { GO throw_if_hresult_failed(__FILE__, __LINE__, __myTempHR, Message, __VA_ARGS__); } }
//! \}


//! \name Global State Macros
//! \{
//! The number of game-state updates per second (in hertz).
#define GO_UPDATE_FREQUENCY 60
//! The game-state update period (in seconds).
#define GO_UPDATE_PERIOD (1.0f / GO_UPDATE_FREQUENCY)
//! Maximum time for a frame (in seconds).
#define GO_MAX_FRAME_TIME 0.1f
//! \}


/*!
    \name Namespace Macros
    These macros should be used whenever declaring Go or internal Go (library) functionality.
 */
//! \{
//! Default library namespace macro.
#define GO_BEGIN_NAMESPACE namespace go {
//! Default library namespace macro.
#define GO_END_NAMESPACE }
//! Internal library namespace macro.
#define GO_BEGIN_INTERNAL_NAMESPACE GO_BEGIN_NAMESPACE namespace internal {
//! Internal library namespace macro.
#define GO_END_INTERNAL_NAMESPACE } GO_END_NAMESPACE
//! Namespace-prefix macro.
#define GO ::go::
//! Namespace-prefix Macro.
#define GO_INTERNAL ::go::internal::
//! \}


/*!
    \name Architecture Macros
    These are hardware-architecture-specific macros.
 */
//! \{
#ifdef _WIN64
//! The build target.
#define GO_BUILD_TARGET "x64"
#else
//! The build target.
#define GO_BUILD_TARGET "Win32"
#endif // WIN64
//! Cache line size in bytes.
#define GO_CACHE_LINE_SIZE 64
#if defined(WIN32)
//! CPU alignment size.
#define GO_CPU_ALIGN_SIZE 4
#else
//! CPU alignment size.
#define GO_CPU_ALIGN_SIZE 8
#endif // WIN32
//! CPU alignment macro.
#define GO_CPU_ALIGN alignas(GO_CPU_ALIGN_SIZE)
//! GPU alignment size.
#define GO_GPU_ALIGN_SIZE 16
//! GPU alignment macro.
#define GO_GPU_ALIGN alignas(GO_GPU_ALIGN_SIZE)
//! SSE alignment size.
#define GO_SSE_ALIGN_SIZE 16
//! SSE alignment macro.
#define GO_SSE_ALIGN alignas(GO_SSE_ALIGN_SIZE)
//! \}


//! \name Application-Directory Macros
//! \{
//! The name of the application directory.
#define GO_APPLICATION_DIRECTORY_NAME "Go"
 //! The name of the data directory.
#define GO_DATA_DIRECTORY_PATH "..\\..\\..\\assets\\"
//! \}


//! \name Path Symbols
//! \{
//! The application directory.
#define GO_PATH_SYMBOL_NAME_APP "AppDir"
//! The application directory.
#define GO_PATH_SYMBOL_APP "$(" GO_PATH_SYMBOL_NAME_APP ")"
 //! The data directory.
#define GO_PATH_SYMBOL_NAME_DATA "DataDir"
//! The data directory.
#define GO_PATH_SYMBOL_DATA "$(" GO_PATH_SYMBOL_NAME_DATA ")"
 //! The world-data directory.
#define GO_PATH_SYMBOL_NAME_WORLD_BASE "WorldBaseDir"
//! The world-data directory.
#define GO_PATH_SYMBOL_WORLD_BASE "$(" GO_PATH_SYMBOL_NAME_WORLD_BASE ")"
//! The local (Windows) directory.
#define GO_PATH_SYMBOL_NAME_LOCAL "LocalDir"
//! The local (Windows) directory.
#define GO_PATH_SYMBOL_LOCAL "$(" GO_PATH_SYMBOL_NAME_LOCAL ")"
//! The roaming (Windows) directory.
#define GO_PATH_SYMBOL_NAME_ROAMING "RoamingDir"
//! The roaming (Windows) directory.
#define GO_PATH_SYMBOL_ROAMING "$(" GO_PATH_SYMBOL_NAME_ROAMING ")"
//! The shader directory.
#define GO_PATH_SYMBOL_NAME_SHADER "ShaderDir"
//! The shader directory.
#define GO_PATH_SYMBOL_SHADER "$(" GO_PATH_SYMBOL_NAME_SHADER ")"
//! The UI base directory.
#define GO_PATH_SYMBOL_NAME_UI_BASE "UIBaseDir"
//! The UI base directory.
#define GO_PATH_SYMBOL_UI_BASE "$(" GO_PATH_SYMBOL_NAME_UI_BASE ")"
//! The assets directory.
#define GO_PATH_SYMBOL_NAME_ASSETS "AssetsDir"
//! The assets directory.
#define GO_PATH_SYMBOL_ASSETS "$(" GO_PATH_SYMBOL_NAME_ASSETS ")"
//! \}


//! \name Filename Macros
//! \{
//! The console variable configuration file.
#define GO_VARIABLES_CONFIG_FILE GO_PATH_SYMBOL_ROAMING "variables.cfg"
//! The user configuration file for console variable.
#define GO_VARIABLES_USER_CONFIG_FILE GO_PATH_SYMBOL_ROAMING "user.cfg"
//! The input scheme configuration file.
#define GO_INPUT_CONFIG_FILE GO_PATH_SYMBOL_ROAMING "input.cfg"
//! The standard log file.
#define GO_LOG_FILE GO_PATH_SYMBOL_LOCAL "log.txt"
//! \}


//! \name Misc. Constants
//! \{
/*!
    Maximum length of a formatted string supported by the Logger.
    This does NOT include the null-terminator.
 */
#define GO_MAX_LOGGER_FORMAT_STRING_LENGTH 1023
/*!
    Stringifying-macro.
    \param X The value to stringify.
 */
#define GO_TOSTRING(X) GO_TOSTRING_HELPER(X)
/*!
    Stringifying-macro.
    \param X The value to stringify.
 */
#define GO_TOSTRING_HELPER(X) #X
//! Returns the number of arguments in a variadic macro.
#define GO_NUM_ARGS(...) std::tuple_size<decltype(std::make_tuple(__VA_ARGS__))>::value
//! \}


//! \name Display Macros
//! \{
//! The default horizontal DPI.
#define GO_DISPLAY_DEFAULT_DPI_X 96
//! The default vertical DPI.
#define GO_DISPLAY_DEFAULT_DPI_Y 96
//! \}


//! \name Assertion Macros
//! \{
#if defined(DEBUG) || defined(_DEBUG)
/*!
    General assertion.
    \param Expr The expression to check.
 */
#define GO_ASSERT(Expr) (void)((!!(Expr)) || (_wassert(_CRT_WIDE(#Expr), _CRT_WIDE(__FILE__), __LINE__), 0) || (__debugbreak(), 0))
/*!
    General assertion.
    \param Expr The expression to check.
    \param Msg The message to print, if the assertion fails.
 */
#define GO_MSGASSERT(Expr, Msg) (void)((!!(Expr)) || (_wassert(_CRT_WIDE(#Expr) L": " _CRT_WIDE(Msg), _CRT_WIDE(__FILE__), __LINE__), 0) || (__debugbreak(), 0))
#else
/*!
    General assertion.
    \param Expr The expression to check.
 */
#define GO_ASSERT(Expr) {} 
/*!
    General assertion.
    \param Expr The expression to check.
    \param Msg The message to print, if the assertion fails.
 */
#define GO_MSGASSERT(Expr, Msg) {}
#endif
//! \}


//! \name Logging Macros
//! \{
//! A string containing a tuple of the current file and line.
#define GO_FILE_LINE_STRING "(" __FILE__ " @ line " GO_TOSTRING(__LINE__) ") "
/*!
    Logging macro.
    \param TxtFmt The format string.
 */
#define GO_LOGFILE_ERROR(TxtFmt, ...) { if(go::the_logger) { go::the_logger->reportf(GO_FILE_LINE_STRING TxtFmt, go::log::file | go::log::error, __VA_ARGS__); } }
/*!
    Logging macro.
    \param TxtFmt The format string.
 */
#define GO_LOGFILE_PLAIN(TxtFmt, ...) { if(go::the_logger) { go::the_logger->reportf(TxtFmt, go::log::file | go::log::no_date, __VA_ARGS__); } }
/*!
    Logging macro.
    \param TxtFmt The format string.
 */
#define GO_LOGFILE_INFO(TxtFmt, ...) { if(go::the_logger) { go::the_logger->reportf(TxtFmt, go::log::file | go::log::info, __VA_ARGS__); } }
/*!
    Logging macro.
    \param TxtFmt The format string.
 */
#define GO_LOGFILE_WARNING(TxtFmt, ...) { if(go::the_logger) { go::the_logger->reportf(GO_FILE_LINE_STRING TxtFmt, go::log::file | go::log::warning, __VA_ARGS__); } }
/*!
    Logging macro.
    \param HResult The HResult to print.
 */
#define GO_LOGFILE_HRESULT_ERROR(HResult) GO_LOGFILE_ERROR("HRESULT indicates failure: 0x%X.\n", HResult);
//! \}


//! \name Relation Macros
//! \{
/*!
    Greater-than relation.
    \param LHS The left-hand-side argument.
    \param RHS The right-hand-side argument.
 */
#define GO_RELATION_GREATER(LHS, RHS) (LHS > RHS)
//! \}


//! \name Graphics Macros
//! \{
//! The maximum number of draw calls per pass.
#define GO_MAX_NUM_DRAW_CALLS_PER_PASS (1024 * 16)

//! Maximum number of visible lights per light type.
#define GO_MAX_NUM_VISIBLE_LIGHTS_PER_TYPE 1024

 //! The maximum number of textures per material.
#define GO_MATERIAL_MAX_TEXTURE_COUNT 6
//! \}

//! \name Functional-programming Macros
//! \{
/*!
    Maps a failed HRESULT return value to a new return value using a constant.
    \param HResult The HRESULT to test.
    \param FailureReturn The value to return, if HResult indicates failure.
 */
#define GO_MAPC_RETURN_HRESULT(HResult, FailureReturn) { HRESULT __myHR = HResult; if(FAILED(__myHR)) { GO_LOGFILE_HRESULT_ERROR(__myHR); return FailureReturn; } }
/*!
    Maps a failed HRESULT return value to a new return value using a function.
    \param HResult The HRESULT to test.
    \param ReturnValue The value to return, if HResult indicates failure.
 */
#define GO_MAPF_RETURN_HRESULT(HResult, MapFunc) { HRESULT __myHR = HResult; if(FAILED(__myHR)) { GO_LOGFILE_HRESULT_ERROR(__myHR); return MapFunc(__myHR); } }
/*!
    Maps a failed return value to a new return value using a constant.
    \param Result The Result to test.
    \param Success The success constant.
    \param FailureReturn The value to return, if Result indicates failure.
 */
#define GO_MAPC_RETURN(Result, Success, FailureReturn) { if((Result) != (Success)) { return FailureReturn; } }
/*!
    Compares two arguments using a binary relation and returns a specific value, incase of failure.
    \param LHS The left-hand side parameter.
    \param Relation The binary relation.
    \param RHS The right-hand side parameter.
    \param FailureReturn The value to return, if (LHS, RHS) do not satify the relation.
 */
#define GO_CMP_MAPR_RETURN(LHS, Relation, RHS, FailureReturn) { if(!(Relation(LHS, RHS))) { auto __myFailureReturn = FailureReturn; GO_LOGFILE_ERROR("Compare-Map failed with code %d.\n", __myFailureReturn); return __myFailureReturn; } }
/*!
    Tests whether a return value matches an expected value and returns the actual result if the comparison evaluates to false.
    \param Result The result to test.
    \param Expected The value to test against.
 */
#define GO_ERROR_RETURN(Result, Expected) { auto __myRet = Result; auto __myExp = Expected; if(__myRet != __myExp) { GO_LOGFILE_ERROR("Expected return value was %d, but we got %d.\n", (int32_t)__myExp, (int32_t)__myRet); return __myRet; } }
/*!
    Tests whether an expression evaluates to nullptr and returns a user-defined value.
    \param Expr The expression to compare to nullptr.
    \param FailReturn The result to return, if Expr evaluates to nullptr.
 */
#define GO_PTRERROR_RETURN(Expr, FailReturn) { if((Expr) == nullptr) { GO_LOGFILE_ERROR("Unexpected null-pointer encountered.\n"); return FailReturn; } }
//! \}


/*!
    \name Member Function Macros
    These macros can be used to declare common member functions.
 */
//! \{
/*!
    Declares a class as being non-copyable.
    \param ClassName The class name (without template signature).
 */
#define GO_DECLARE_NON_COPYABLE(ClassName) public: ClassName(const ClassName &) = delete; \
    ClassName &operator=(const ClassName&) = delete;
/*!
    Declares a static member function, which returns the component class name.

    \param ClassName The component class name.
 */
#define GO_DECLARE_ENTITY_COMPONENT(ClassName) public: static const std::string &component_class_name() { static const std::string s = GO_TOSTRING(ClassName); return s; }
/*!
    Declares a static member function, which returns the component class name.

    \param ClassName The component class name.
 */
#define GO_DECLARE_ENTITY_COMPONENT_MANAGER_BASE public: virtual size_t component_instance_class_id() const noexcept = 0;
/*!
    Declares a static member function, which returns the component class name.

    \param ClassName The component class name.
 */
#define GO_DECLARE_ENTITY_COMPONENT_MANAGER(ClassName) public: static size_t component_class_id() { static const auto result = std::hash<std::string>()(GO_TOSTRING(ClassName)); return result; } \
    size_t component_instance_class_id() const noexcept override { return component_class_id(); }
//! \}


//! \name Scene Macros
//! \{
//! Maximum number of components per entity.
#define GO_ENTITY_MAX_COMPONENT_COUNT 8
//! Maximum number of entities in a scene (hint).
#define GO_ENTITY_MAX_COUNT_HINT 100000
//! Number of component classes (hint).
#define GO_ENTITY_MAX_COMPONENT_MANAGERS_COUNT_HINT 1024
//! \}


//! \name CVar Macros
//! \{
//! Declares a new console variable.
#define GO_DECLARE_CVAR(Name) extern GO_API variable v_##Name;
//! Defines a console variable.
#define GO_DEFINE_CVAR(Name, ...) GO variable go::v_##Name(GO_TOSTRING(Name), __VA_ARGS__);
//! Adds a new console variable to the environment.
#define GO_ADD_CVAR(Name) GO the_environment->add_variable(&GO v_##Name);
//! Returns the CVar behind a variable name.
#define GO_CVAR(Name) GO v_##Name
//! Returns the name behind a variable.
#define GO_CVAR_NAME(Name) GO_TOSTRING(Name)
//! Adds a new console variable and associates its value with a path symbol.
#define GO_ADD_CVAR_WITH_PATH_SYMBOL(Name, Symbol) { GO_ADD_CVAR(Name) define_path_symbol(Symbol, GO_CVAR(Name).value()); }
//! \}


//! \name Helper Macros
//! \{
/*!
    For-each macro with built-in step count.

    \param Current The loop iterator.
    \param C The container to loop over.
 */
#define GO_FOR_EACH_INDEX(Current, C) for( \
    auto Current = foreach_iterator<decltype(std::begin(C))>{ 0, std::begin(C) }, \
    _LoopEnd = foreach_iterator<decltype(std::begin(C))>{ 0, std::end(C) }; \
    Current.iterator != _LoopEnd.iterator; ++Current.index, ++Current.iterator)
/*!
    Safe-delete macro.
    \param Ptr The pointer to delete.
 */
#define GO_SAFE_DELETE(Ptr) { delete Ptr; Ptr = nullptr; }
/*!
    COM-Macro.
    \param Ptr The interface-pointer.
 */
#define GO_SAFE_RELEASE(Ptr) { if(Ptr) { Ptr->Release(); Ptr = nullptr; } }
/*!
    Declares a new helper class.
    \param BaseClass The base class.
 */
#define GO_BEGIN_DECLARE_HELPER_CLASS(BaseClass) \
    struct GO_API c##BaseClass : public BaseClass { \
        c##BaseClass() = default; \
        explicit c##BaseClass(const BaseClass &o) : BaseClass(o) {} \
        explicit c##BaseClass(const cdefault_value &); \
        c##BaseClass(BaseClass &&o) { *this = std::move(o); } \
        ~c##BaseClass() {}
/*!
    Declares a new helper class.
    \param BaseClass The base class.
 */
#define GO_BEGIN_DECLARE_HELPER_CLASS_EX(BaseClass, NewClassName) \
    struct GO_API NewClassName : public BaseClass { \
        NewClassName() = default; \
        explicit NewClassName(const NewClassName &o) : BaseClass(o) {} \
        explicit NewClassName(const cdefault_value &); \
        NewClassName(NewClassName &&o) { *this = std::move(o); } \
        ~NewClassName() {} \
        NewClassName &NewClassName::operator=(const NewClassName &o) = default;

//! Finishes the declaration of a helper class.
#define GO_END_DECLARE_HELPER_CLASS };
/*!
    Declares a new helper class.
    \param BaseClass The base class.
 */
#define GO_DECLARE_HELPER_CLASS(BaseClass) \
    GO_BEGIN_DECLARE_HELPER_CLASS(BaseClass) \
    GO_END_DECLARE_HELPER_CLASS
/*!
    Declares a new helper class.
    \param BaseClass The base class.
    \param NewClass The new, extended class.
 */
#define GO_DECLARE_HELPER_CLASS_EX(BaseClass, NewClass) \
    GO_BEGIN_DECLARE_HELPER_CLASS_EX(BaseClass, NewClass) \
    GO_END_DECLARE_HELPER_CLASS
/*!
    Declares a new helper class with an environment initialization function.
    \param BaseClass The base class.
 */
#define GO_DECLARE_ENVIRONMENT_HELPER_CLASS(BaseClass) \
    GO_BEGIN_DECLARE_HELPER_CLASS(BaseClass) \
    explicit c##BaseClass(const cenvironment_value &); \
    GO_END_DECLARE_HELPER_CLASS
/*!
    Declares a new helper class with an environment initialization function.
    \param BaseClass The base class.
    \param NewClass The new, extended class.
 */
#define GO_DECLARE_ENVIRONMENT_HELPER_CLASS_EX(BaseClass, NewClass) \
    GO_BEGIN_DECLARE_HELPER_CLASS_EX(BaseClass, NewClass) \
    explicit NewClass(const cenvironment_value &); \
    GO_END_DECLARE_HELPER_CLASS
/*!
    Declares member functions for iterating a class using the C++ iterator syntax on an STL-based container.
 
    \param Container The type of the container.
    \param ContainerName The name of the container instance to iterate over.
 */
#define GO_DECLARE_STL_ITERABLE(ContainerType, ContainerName) \
    public: \
    ContainerType::const_iterator begin() const { return ContainerName.begin(); } \
    ContainerType::iterator begin() { return ContainerName.begin(); } \
    ContainerType::const_iterator end() const { return ContainerName.end(); } \
    ContainerType::iterator end() { return ContainerName.end(); } \
/*!
    Declares member functions for iterating a class using the C++ iterator syntax on an STL-based container.
 
    \param Container The type of the container.
    \param ContainerName The name of the container instance to iterate over.
 */
#define GO_DECLARE_STL_CONST_ITERABLE(ContainerType, ContainerName) \
    public: \
    ContainerType::const_iterator begin() const { return ContainerName.begin(); } \
    ContainerType::const_iterator end() const { return ContainerName.end(); } \
/*!
    Declares member functions for iterating a class using the C++ iterator syntax on an STL-based container.
 
    \param Container The type of the container.
    \param ContainerName The name of the container instance to iterate over.
    \param EndIndex The end-of-iteration index (offset to begin()).
 */
#define GO_DECLARE_STL_ITERABLE_CUSTOM_END(ContainerType, ContainerName, EndIndex) \
    public: \
    ContainerType::const_iterator begin() const { return ContainerName.begin(); } \
    ContainerType::iterator begin() { return ContainerName.begin(); } \
    ContainerType::const_iterator end() const { return ContainerName.begin() + EndIndex; } \
    ContainerType::iterator end() { return ContainerName.begin() + EndIndex; } \
/*!
    Declares member functions for iterating a class using the C++ iterator syntax on an STL-based container.
 
    \param Container The type of the container.
    \param ContainerName The name of the container instance to iterate over.
    \param EndIndex The end-of-iteration index (offset to begin()).
 */
#define GO_DECLARE_STL_CONST_ITERABLE_CUSTOM_END(ContainerType, ContainerName, EndIndex) \
    public: \
    ContainerType::const_iterator begin() const { return ContainerName.begin(); } \
    ContainerType::const_iterator end() const { return ContainerName.begin() + EndIndex; } \
//! \}
