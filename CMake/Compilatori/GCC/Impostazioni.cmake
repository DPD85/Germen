# ----- Impostazioni specifiche per GCC -----

message(STATUS "GCC: abilitati quasi tutti gli avvisi.")
message(STATUS "GCC: abilitati gli avvisi extra.")
message(STATUS "GCC: abilitati gli avvisi relativi al rispetto rigoroso degli standard ISO C e C++.")
message(STATUS "GCC: avvisa quando la dichiarazione di un tipo ne nasconde un'altra.")
message(STATUS "GCC: avvisa quando una class ha funzioni virtuali ma non ha un distruttore virtuale.")
message(STATUS "GCC debug: abilito le informazioni di debug complete.")
message(STATUS "GCC debug: abilito le ottimizzazioni compatibili con le informazioni di debug.")
message(STATUS "GCC release: abilitata una maggiore protezione verso errori che interessano lo stack.")

add_library(ImpostazioniCompilatore INTERFACE)

target_compile_options(ImpostazioniCompilatore
    INTERFACE
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wnon-virtual-dtor
        $<$<CONFIG:Debug>: -g3>
        $<$<CONFIG:Debug>: -Og>
        $<$<CONFIG:Release>: -fstack-protector-strong>
)
