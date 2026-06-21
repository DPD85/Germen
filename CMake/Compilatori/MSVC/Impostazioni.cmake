# ----- Impostazioni specifiche per MSVC -----

set(CMAKE_CONFIGURATION_TYPES Debug Release)

message(STATUS "MSVC: abilitata compilazione multi processore.")
add_compile_options("/MP")

message(STATUS "MSVC: codifica sempre le stringhe in UTF-8.")
message(STATUS "MSVC: abilitati gli avvisi di livello 3.")
message(STATUS "MSVC: abilitata modalità di conformità.")
message(STATUS "MSVC: abilitate le funzioni intrinsic.")
message(STATUS "MSVC: il linker genera informazioni di debug in configurazione release.")

add_library(ImpostazioniCompilatore INTERFACE)

target_compile_options(ImpostazioniCompilatore
    INTERFACE
        /utf-8
        /W3
        /permissive-
        /Oi
        /DEBUG
)

# Configurazione globale di MSBuild:
#   * disabilita l'analisi statica del codice
configure_file(Directory.Build.props.in Directory.Build.props COPYONLY)
