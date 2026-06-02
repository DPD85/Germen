set(CMAKE_FOLDER "Strumenti")

# ----- IncorporaFile -----

add_executable(IncorporaFile)

target_sources(IncorporaFile
    PRIVATE
        IncorporaFile.cpp
)

make_source_group(TARGET IncorporaFile)
