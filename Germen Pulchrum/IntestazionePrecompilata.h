#pragma once

#define _CRT_SECURE_NO_WARNINGS

// C++ standard library

#include <algorithm>
#include <array>
#include <chrono>
#include <clocale>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

// Vulkan

#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan.h>

// vk-bootstrap

#include <VkBootstrap.h>

// SLD3

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

// ImGui

#include <imgui.h>
#include <imgui_freetype.h>
#include <imgui_internal.h>

// ImGui back-end

#include <imgui_impl_sdl3.h>
#include <imgui_impl_vulkan.h>

// ImPlot

#define IMPLOT_DISABLE_OBSOLETE_FUNCTIONS

#include <implot.h>

// Boost

#include <boost/locale.hpp>

// nlohmann JSON

// #include <nlohmann/json.hpp>

// O.S. API ed altro

#ifdef WIN32

#    define NOMINMAX // - Macros min(a,b) and max(a,b)

#    define NOGDICAPMASKS     // - CC_*, LC_*, PC_*, CP_*, TC_*, RC_
#    define NOVIRTUALKEYCODES // - VK_*
#    define NOWINMESSAGES     // - WM_*, EM_*, LB_*, CB_*
#    define NOWINSTYLES       // - WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
#    define NOSYSMETRICS      // - SM_*
#    define NOMENUS           // - MF_*
#    define NOICONS           // - IDI_*
#    define NOKEYSTATES       // - MK_*
#    define NOSYSCOMMANDS     // - SC_*
#    define NORASTEROPS       // - Binary and Tertiary raster ops
#    define NOSHOWWINDOW      // - SW_*
#    define OEMRESOURCE       // - OEM Resource values
#    define NOATOM            // - Atom Manager routines
#    define NOCLIPBOARD       // - Clipboard routines
#    define NOCOLOR           // - Screen colors
#    define NOCTLMGR          // - Control and Dialog routines
#    define NODRAWTEXT        // - DrawText() and DT_*
#    define NOGDI             // - All GDI defines and routines
#    define NOKERNEL          // - All KERNEL defines and routines
#    define NOUSER            // - All USER defines and routines
#    define NONLS             // - All NLS defines and routines
#    define NOMB              // - MB_* and MessageBox()
#    define NOMEMMGR          // - GMEM_*, LMEM_*, GHND, LHND, associated routines
#    define NOMETAFILE        // - typedef METAFILEPICT
#    define NOMSG             // - typedef MSG and associated routines
#    define NOOPENFILE        // - OpenFile(), OemToAnsi, AnsiToOem, and OF_*
#    define NOSCROLL          // - SB_* and scrolling routines
#    define NOSERVICE         // - All Service Controller routines, SERVICE_ equates, etc.
#    define NOSOUND           // - Sound driver routines
#    define NOTEXTMETRIC      // - typedef TEXTMETRIC and associated routines
#    define NOWH              // - SetWindowsHook and WH_*
#    define NOWINOFFSETS      // - GWL_*, GCL_*, associated routines
#    define NOCOMM            // - COMM driver routines
#    define NOKANJI           // - Kanji support stuff.
#    define NOHELP            // - Help engine interface.
#    define NOPROFILER        // - Profiler interface.
#    define NODEFERWINDOWPOS  // - DeferWindowPos routines
#    define NOMCX             // - Modem Configuration Extensions

#    include <Windows.h>

#else

#    include <signal.h>

#endif

// Impostazioni avvisi specifici

#ifdef WIN32
#    pragma warning(3:4'061) // Enumerator 'identifier' in a switch of enum is not explicitly handled by a case label.
#    pragma warning(3:4'062) // Enumerator 'identifier' in a switch of enum 'enumeration' is not handled.
#    pragma warning(3:4'211) // Nonstandard extension used : redefined extern to static
#    pragma warning(3:4'263) // Member function does not override any base class virtual member function.
#    pragma warning(3:4'264) // No override available for virtual member function from base 'class'.
#    pragma warning(3:4'265) // 'class': class has virtual functions, but destructor is not virtual.
#    pragma warning(3:4'266) // No override available for one overload of virtual member function from base 'type'.
#    pragma warning(3:4'287) // 'operator': unsigned/negative constant mismatch.
#    pragma warning(3:4'296) // 'operator': expression is always false.
#    pragma warning(3:4'619) // #pragma warning: there is no warning number 'number'.
#    pragma warning(3:4'822) // 'member': local class member function does not have a body.
#    pragma warning(3:4'946) // Reinterpret_cast used between related classes: 'class1' and 'class2'.
#    pragma warning(3:5'038) // Data member 'member1' will be initialized after data member 'member2'.
#    pragma warning(3:5'233) // explicit lambda capture 'identifier' is not used.
#    pragma warning(3:5'240) // 'attribute-name': attribute is ignored in this syntactic position.
#    pragma warning(3:5'262) // Use [[fallthrough]] when a break statement is intentionally omitted between cases.
#    pragma warning(3:5'264) // 'variable-name': 'const' variable is not used.
#    pragma warning(3:5'266) // 'const' qualifier on return type has no effect.
#endif
