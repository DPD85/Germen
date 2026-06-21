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
    VERSION 1.92.6-docking
    GITHUB_REPOSITORY ocornut/imgui
    # Include il file "ImGui-Config.h" quando la macro IMGUI_USER_CONFIG è definita ignorando il contenuto della macro.
    PATCHES ImGui-Config.patch
    DOWNLOAD_ONLY ON
)

# ----- ImGui - Libreria principale

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
        IMGUI_USER_CONFIG
)

target_include_directories(ImGui
    PUBLIC
        ${ImGui_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
)

if(MSVC)
    target_compile_options(ImGui PRIVATE "/utf-8")
endif()

make_source_group(TARGET ImGui ADDITIONAL_SOURCE_DIR ${ImGui_SOURCE_DIR})

# ----- ImGui - Backend SDL3

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

# ----- ImGui - Backend Vulkan

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

# ----- ImGui - FreeType

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

# ----- ImGui - StdLib

add_library(ImGui-StdLib STATIC)

target_sources(ImGui-StdLib
    PRIVATE
    ${ImGui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
    ${ImGui_SOURCE_DIR}/misc/cpp/imgui_stdlib.h
)

target_link_libraries(ImGui-StdLib
    PUBLIC
    ImGui
)

target_include_directories(ImGui-StdLib PUBLIC ${ImGui_SOURCE_DIR}/misc/cpp)

make_source_group(TARGET ImGui-StdLib ADDITIONAL_SOURCE_DIR ${ImGui_SOURCE_DIR}/misc/cpp)

# ----- ImGui - Aiuti debuggers

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

# ----- MD4C -----

CPMAddPackage(
    NAME MD4C
    VERSION 0.5.3
    GIT_TAG release-0.5.3
    GITHUB_REPOSITORY mity/md4c
    DOWNLOAD_ONLY ON
)

add_library(MD4C STATIC)

target_sources(MD4C
    PRIVATE
        ${MD4C_SOURCE_DIR}/src/md4c.c
        ${MD4C_SOURCE_DIR}/src/md4c.h
)

target_include_directories(MD4C PUBLIC ${MD4C_SOURCE_DIR}/src)

if(MSVC)
    target_compile_options(MD4C PRIVATE "/utf-8")
endif()

target_link_libraries(MD4C PUBLIC ImGui)

make_source_group(TARGET MD4C ADDITIONAL_SOURCE_DIR ${MD4C_SOURCE_DIR})

# ----- ImGui MD -----

CPMAddPackage(
    NAME ImGui-MD
    VERSION 1.0.0
    GITHUB_REPOSITORY DPD85/imgui_md
    DOWNLOAD_ONLY ON
)

add_library(ImGui-MD STATIC)

target_sources(ImGui-MD
    PRIVATE
        ${ImGui-MD_SOURCE_DIR}/imgui_md.cpp
        ${ImGui-MD_SOURCE_DIR}/imgui_md.h
)

target_include_directories(ImGui-MD PUBLIC ${ImGui-MD_SOURCE_DIR})

if(MSVC)
    target_compile_options(ImGui-MD PRIVATE "/utf-8")
endif()

target_link_libraries(ImGui-MD
    PUBLIC
        MD4C
        ImGui
)

make_source_group(TARGET ImGui-MD ADDITIONAL_SOURCE_DIR ${ImGui-MD_SOURCE_DIR})

# ----- Graphviz -----

set(CMAKE_FOLDER "Terze parti/Graphviz")

CPMAddPackage(
    NAME Graphviz
    VERSION 15.1.0
    GIT_TAG 15.1.0
    GITLAB_REPOSITORY graphviz/graphviz
    PATCHES Graphviz-FlexBison.patch
    OPTIONS
        "BUILD_SHARED_LIBS 0"
        "BUILD_TESTING 0"
        "ENABLE_D OFF"
        "ENABLE_GO OFF"
        "ENABLE_GUILE OFF"
        "ENABLE_JAVA OFF"
        "ENABLE_JAVASCRIPT OFF"
        "ENABLE_LTDL AUTO"
        "ENABLE_LUA OFF"
        "ENABLE_PERL OFF"
        "ENABLE_PHP OFF"
        "ENABLE_PYTHON OFF"
        "ENABLE_R OFF"
        "ENABLE_RUBY OFF"
        "ENABLE_SHARP OFF"
        "ENABLE_SWIG OFF"
        "ENABLE_TCL OFF"
        "WITH_EXPAT OFF"
        "WITH_GDK OFF"
        "WITH_GHOSTSCRIPT OFF"
        "WITH_GTK OFF"
        "WITH_GVEDIT OFF"
        "WITH_POPPLER OFF"
        "WITH_QUARTZ OFF"
        "WITH_RSVG OFF"
        "WITH_SMYRNA OFF"
        "WITH_WEBP OFF"
        "WITH_X OFF"
        "WITH_ZLIB OFF"
        "CMAKE_DISABLE_FIND_PACKAGE_CAIRO ON"
        "CMAKE_DISABLE_FIND_PACKAGE_PANGOCAIRO ON"
        "install_win_dependency_dlls 0"
)

set(CMAKE_FOLDER "Terze parti")
