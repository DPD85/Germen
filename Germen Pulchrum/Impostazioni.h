#pragma once

struct Impostazioni
{
    size_t temaSelezionato   = { 0 };
    float scalaGUI           = { 1.0f };
    size_t linguaSelezionata = { 0 };
};

extern struct Impostazioni Impostazioni;
extern const std::filesystem::path DIRETTORIO_IMPOSTAZIONI;
extern const std::string PERCORSO_IMGUI_INI_FILE;

bool CaricaImpostazioni();
bool SalvaImpostazioni();
