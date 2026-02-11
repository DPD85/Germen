set(CMAKE_FOLDER "Terze parti")

# ----- vk-bootstrap -----

CPMAddPackage(
    NAME vk-bootstrap
    VERSION 1.4.336
    GITHUB_REPOSITORY charles-lunarg/vk-bootstrap
)

# ----- ImGui -----

CPMAddPackage(
    NAME ImGui
    VERSION 1.92.5-docking
    GITHUB_REPOSITORY ocornut/imgui
    DOWNLOAD_ONLY ON
)

message(STATUS "ImGui_SOURCE_DIR = ${ImGui_SOURCE_DIR}")

# Libreria principale

add_library(ImGui STATIC)

target_sources(ImGui
    PRIVATE
        ${ImGui_SOURCE_DIR}/imgui.cpp
        ${ImGui_SOURCE_DIR}/imgui.h
        ${ImGui_SOURCE_DIR}/imgui_demo.cpp
        ${ImGui_SOURCE_DIR}/imgui_draw.cpp
        ${ImGui_SOURCE_DIR}/imgui_internal.h
        ${ImGui_SOURCE_DIR}/imgui_tables.cpp
        ${ImGui_SOURCE_DIR}/imgui_widgets.cpp
        ${ImGui_SOURCE_DIR}/imstb_rectpack.h
        ${ImGui_SOURCE_DIR}/imstb_textedit.h
        ${ImGui_SOURCE_DIR}/imstb_truetype.h

        ${CMAKE_CURRENT_SOURCE_DIR}/ImGui-Config.h
)

target_compile_definitions(ImGui
    PUBLIC
        IMGUI_USER_CONFIG="${CMAKE_CURRENT_SOURCE_DIR}/ImGui-Config.h"
)

target_include_directories(ImGui PUBLIC ${ImGui_SOURCE_DIR})

if(MSVC)
    target_compile_options(ImGui PRIVATE "/utf-8")
endif()

make_source_group(TARGET ImGui ADDITIONAL_SOURCE_DIR ${ImGui_SOURCE_DIR})

# Backend SDL3

find_package(SDL3 REQUIRED)

add_library(ImGui-SDL3 STATIC)

target_sources(ImGui-SDL3
    PRIVATE
        ${ImGui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
        ${ImGui_SOURCE_DIR}/backends/imgui_impl_sdl3.h
)

target_link_libraries(ImGui-SDL3
    PUBLIC
        ImGui
        SDL3::SDL3
)

target_include_directories(ImGui-SDL3 PUBLIC ${ImGui_SOURCE_DIR}/backends)

if(MSVC)
    target_compile_options(ImGui-SDL3 PRIVATE "/utf-8")
endif()

make_source_group(TARGET ImGui-SDL3 ADDITIONAL_SOURCE_DIR ${ImGui_SOURCE_DIR}/backends)

# Backend Vulkan

find_package(Vulkan REQUIRED)

add_library(ImGui-Vulkan STATIC)

target_sources(ImGui-Vulkan
    PRIVATE
        ${ImGui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
        ${ImGui_SOURCE_DIR}/backends/imgui_impl_vulkan.h
)

target_link_libraries(ImGui-Vulkan
    PUBLIC
        ImGui
        Vulkan::Vulkan
)

target_include_directories(ImGui-Vulkan PUBLIC ${ImGui_SOURCE_DIR}/backends)

if(MSVC)
    target_compile_options(ImGui-Vulkan PRIVATE "/utf-8")
endif()

make_source_group(TARGET ImGui-Vulkan ADDITIONAL_SOURCE_DIR ${ImGui_SOURCE_DIR}/backends)

# FreeType

find_package(Freetype REQUIRED)
find_package(plutosvg REQUIRED)

add_library(ImGui-FreeType STATIC)

target_sources(ImGui-FreeType
    PRIVATE
        ${ImGui_SOURCE_DIR}/misc/freetype/imgui_freetype.cpp
        ${ImGui_SOURCE_DIR}/misc/freetype/imgui_freetype.h
)

target_link_libraries(ImGui-FreeType
    PUBLIC
        ImGui
        Freetype::Freetype
        plutosvg::plutosvg
)

target_include_directories(ImGui-FreeType PUBLIC ${ImGui_SOURCE_DIR}/misc/freetype)

make_source_group(TARGET ImGui-FreeType ADDITIONAL_SOURCE_DIR ${ImGui_SOURCE_DIR}/misc/freetype)

# Aiuti debuggers

if(MSVC)
    add_library(ImGui-Debuggers INTERFACE)

    target_sources(ImGui-Debuggers
        INTERFACE
            ${ImGui_SOURCE_DIR}/misc/debuggers/imgui.natstepfilter
            ${ImGui_SOURCE_DIR}/misc/debuggers/imgui.natvis
    )

    make_source_group(TARGET ImGui-Debuggers ADDITIONAL_SOURCE_DIR ${ImGui_SOURCE_DIR}/misc/debuggers)
endif()


# ----- ImPlot -----

CPMAddPackage(
    NAME ImPlot
    VERSION 0.17
    GITHUB_REPOSITORY epezent/implot
    DOWNLOAD_ONLY ON
)

add_library(ImPlot STATIC)

target_sources(ImPlot
    PRIVATE
        ${ImPlot_SOURCE_DIR}/implot.cpp
        ${ImPlot_SOURCE_DIR}/implot.h
        ${ImPlot_SOURCE_DIR}/implot_demo.cpp
        ${ImPlot_SOURCE_DIR}/implot_internal.h
        ${ImPlot_SOURCE_DIR}/implot_items.cpp
)

target_include_directories(ImPlot PUBLIC ${ImPlot_SOURCE_DIR})

if(MSVC)
    target_compile_options(ImPlot PRIVATE "/utf-8")
endif()

target_link_libraries(ImPlot PUBLIC ImGui)

make_source_group(TARGET ImPlot ADDITIONAL_SOURCE_DIR ${ImPlot_SOURCE_DIR})

