#include "IntestazionePrecompilata.h"

#include "Disegnatore.h"
#include "Impostazioni.h"
#include "Internazionalizzazione.h"

using namespace boost::locale;

int main(int, char **)
{
    // ----- -----

    CaricaImpostazioni();

    // ----- Inizializzazione internazionalizzazione -----

    {
        InizializzaInternazionalizzazione();

        // ----- Stampa informazioni sull'internazionalizzazione

        std::cout << "----- Informazioni internazionalizzazione -----\n\n";

        const auto gestoreBackEnd = localization_backend_manager::global();
        std::cout << "Back-end disponibili:\n";
        for (std::string &name : gestoreBackEnd.get_all_backends())
            std::cout << "  " << name << '\n';
        std::cout << '\n';

        const std::locale localeCorrente = std::locale();
        const auto &proprietà = std::use_facet<info>(localeCorrente);
        std::cout << "Internazionalizzazione utilizzata:\n";
        std::cout << "  Lingua  : " << proprietà.language() << '\n'
                  << "  Stato   : " << proprietà.country() << '\n'
                  << "  Codifica: " << proprietà.encoding() << '\n'
                  << "  Variante: " << proprietà.variant() << '\n';

        std::cout << '\n';

        // Se viene utilizzata la lingua di sistema
        if (Impostazioni.linguaSelezionata == 0)
        {
            // Carica la informazioni in base alle impostazioni dell'ambiente e del S.O.
            const std::locale localeDefault    = GeneratoreMultiLingua("");
            const auto &proprietàLocaleDefault = std::use_facet<info>(localeDefault);

            if (proprietàLocaleDefault.language() == "it")
            {
                std::cout << "Nota: utilizzo sempre it_IT per la lingua italiana indipendentemente dalla nazione.\n";
                std::cout << '\n';
            }
            else if (proprietàLocaleDefault.language() == "en")
            {
                std::cout << "Nota: utilizzo sempre en_GB per la lingua inglese indipendentemente dalla nazione.\n";
                std::cout << '\n';
            }
        }
    }

    // ----- -----

    return Disegnatore();
}

static constexpr bool ByteUguali(const char *a, const char8_t *b)
{
    while (*a || *b)
    {
        if (static_cast<unsigned char>(*a) != static_cast<unsigned char>(*b)) return false;
        ++a;
        ++b;
    }
    return true;
}

// ----- Verifica codifica stringhe in UTF-8 -----
// I programmi in C++ hanno due codifiche per i caratteri, una per i file sorgenti ed una per le stringhe del programma.
// La prima codifica è valida durante la compilazione mentre la seconda durante l'esecuzione.

static_assert(
    ByteUguali("Riga con caratteri UTF-8: à è ì", u8"Riga con caratteri UTF-8: à è ì"),
    "Il programma non e' compilato con in UTF-8 (execution charset), le stringhe al suo interno sono codificate "
    "diversamente, di conseguenza potrebbe non funzionare correttamente.");
