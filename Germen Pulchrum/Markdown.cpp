#include "IntestazionePrecompilata.h"

#include "Markdown.h"

#include "Disegnatore.h"

void Markdown::Disegna(const std::string &testo)
{
    print(testo.data(), testo.data()+testo.size());
}

void Markdown::Disegna(const char *inizioTesto, const char *fineTesto)
{
    if(fineTesto == nullptr) fineTesto = inizioTesto + strlen(inizioTesto);
    print(inizioTesto, fineTesto);
}

void Markdown::get_font(font_info &info) const
{
    // Tipo di font

    // Grassetto ed italico
    if (m_is_strong && m_is_em) info.font = FontItalicoGrassetto;
    // Italico
    else if (m_is_em) info.font = FontItalico;
    // Testo normale
    else if (m_hlevel == 0) info.font = FontNormale;
    // Grassetto
    else info.font = FontGrassetto;

    // Dimensione font - scala la dimensione in base al livello dell'intestazione.

    // I valori di scala corrispondono alle dimensioni del testo definite nel HTML.
    constexpr float scalaPerLivello[] = {
        1.0f,  // Testo normale, nessuna intestazione
        2.0f,  // H1 [em]
        1.5f,  // H2 [em]
        1.17f, // H3 [em]
        1.0f,  // H4 [em]
        0.83f, // H5 [em]
        0.67f  // H6 [em]
    };

    if (m_hlevel < std::size(scalaPerLivello)) info.size = DimensioneDefaultFont * scalaPerLivello[m_hlevel];
}

void Markdown::open_url() const
{
    if (!SDL_OpenURL(m_href.data()))
        std::cout << "[Errore] Impossibile aprire il URL '" << m_href << "': " << SDL_GetError() << '\n';
}
