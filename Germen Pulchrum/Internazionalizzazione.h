struct Lingua
{
    std::string nome;
    const char *bandiera;
    const char *codice;
};

extern std::array<Lingua, 3> Lingue;
extern boost::locale::generator GeneratoreMultiLingua;

void InizializzaInternazionalizzazione();
void ImpostaLingua(size_t indice);
std::vector<char> CaricaCatalogoMessaggi(const std::string &nomeFile, const std::string &codifica);
