#include "IntestazionePrecompilata.h"

#include "CodaCancellazione.h"
#include "Disegnatore.h"
#include "Internazionalizzazione.h"

using namespace boost::locale;

int main(int, char **)
{
    // ----- Inizializzazione internazionalizzazione -----

    {
        // Carica la informazioni in base alle impostazioni dell'ambiente e del S.O.
        std::locale locale = GeneratoreMultiLingua("");

        {
            const boost::locale::info &localeProp = std::use_facet<boost::locale::info>(locale);

            if (localeProp.language() == "it")
            {
                locale = GeneratoreMultiLingua("it_IT.UTF-8");
            }
            else if (localeProp.language() == "en")
            {
                locale = GeneratoreMultiLingua("en_GB.UTF-8");
            }
        }

        const boost::locale::info &localeProp = std::use_facet<boost::locale::info>(locale);

        boost::locale::gnu_gettext::messages_info info;
        info.paths.push_back("");
        info.domains.push_back(boost::locale::gnu_gettext::messages_info::domain("Lingua"));
        info.callback = CaricaCatalogoMessaggi;
        info.language = localeProp.language();
        info.country  = localeProp.country();
        info.encoding = localeProp.encoding();
        info.variant  = localeProp.variant();

        locale = std::locale(locale, boost::locale::gnu_gettext::create_messages_facet<char>(info));
        locale = std::locale(locale, boost::locale::gnu_gettext::create_messages_facet<char8_t>(info));

        // Imposta per il C
        char *str = std::setlocale(LC_ALL, ".UTF-8");

        // Imposta per il C++
        std::locale::global(locale);
        std::cin.imbue(locale);
        std::cout.imbue(locale);
        std::cerr.imbue(locale);
        std::wcin.imbue(locale);
        std::wcout.imbue(locale);
        std::wcerr.imbue(locale);

        InizializzaInternazionalizzazione();

        // ----- Stampa informazioni sull'internazionalizzazione

        std::cout << "----- Informazioni internazionalizzazione -----\n\n";

        const localization_backend_manager gestoreBackEnd = localization_backend_manager::global();
        std::cout << "Back-end disponibili:\n";
        for (std::string &name : gestoreBackEnd.get_all_backends())
            std::cout << "  " << name << '\n';
        std::cout << '\n';

        const boost::locale::info &proprietà = std::use_facet<boost::locale::info>(locale);
        std::cout << "Internazionalizzazione utilizzata:\n";
        std::cout << "  Lingua  : " << proprietà.language() << '\n'
                  << "  Stato   : " << proprietà.country() << '\n'
                  << "  Codifica: " << proprietà.encoding() << '\n'
                  << "  Variante: " << proprietà.variant() << '\n';

        std::cout << '\n';

        if (localeProp.language() == "it")
        {
            std::cout << "Nota: utilizzo sempre it_IT per la lingua italiana indipendentemente dalla nazione.\n";
            std::cout << '\n';
        }
        else if (localeProp.language() == "en")
        {
            std::cout << "Nota: utilizzo sempre en_GB per la lingua inglese indipendentemente dalla nazione.\n";
            std::cout << '\n';
        }
    }

    // ----- -----

    return Disegnatore();
}
