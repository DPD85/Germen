#include "IntestazionePrecompilata.h"

#include "Internazionalizzazione.h"

#include "CostantiEdAltro.h"
#include "Disegnatore.h"
#include "Icone bandiere.h"
#include "Impostazioni.h"
#include "Lingue/en_GB.h"

using namespace boost::locale;

generator GeneratoreMultiLingua;
std::array<Lingua, 3> Lingue;
struct TestiGUI TestiGUI;

static gnu_gettext::messages_info infoMsg;

static void AggiornaNomiLingue();
static void AggiornaTestiGUI();

// ----- -----

void InizializzaInternazionalizzazione()
{
    infoMsg.paths.emplace_back("");
    infoMsg.domains.emplace_back("Lingua");
    infoMsg.callback = CaricaCatalogoMessaggi;

    // ----- -----

    size_t index = 0;

    Lingue[index].bandiera = nullptr;
    Lingue[index].codice   = "";

    ++index;
    Lingue[index].bandiera = Fonts::ICON_IT;
    Lingue[index].codice   = "it_IT.UTF-8";

    ++index;
    Lingue[index].bandiera = Fonts::ICON_GB;
    Lingue[index].codice   = "en_GB.UTF-8";

    // ----- -----

    ImpostaLingua(Impostazioni.linguaSelezionata);
}

void ImpostaLingua(const size_t indice)
{
    const char *codice = Lingue[indice].codice;

    std::locale locale = GeneratoreMultiLingua(codice);

    // Se va impostata la lingua di sistema allora ignoro la nazione per l'italiano ed l'inglese.
    if (codice[0] == '\0')
    {
        const info &localeProp = std::use_facet<info>(locale);

        if (localeProp.language() == "it")
        {
            locale = GeneratoreMultiLingua("it_IT.UTF-8");
        }
        else if (localeProp.language() == "en")
        {
            locale = GeneratoreMultiLingua("en_GB.UTF-8");
        }
    }

    const info &proprietà = std::use_facet<info>(locale);

    infoMsg.language = proprietà.language();
    infoMsg.country  = proprietà.country();
    infoMsg.encoding = proprietà.encoding();
    infoMsg.variant  = proprietà.variant();

    // Configurazione caricamento del catalogo dei messaggi per i caratteri di tipo char.
    locale = std::locale(locale, gnu_gettext::create_messages_facet<char>(infoMsg));
    // Configurazione caricamento del catalogo dei messaggi per i caratteri UTF-8.
    locale = std::locale(locale, gnu_gettext::create_messages_facet<char8_t>(infoMsg));

    // Imposta per il C
    std::setlocale(LC_ALL, codice);

    // Imposta per il C++
    std::locale::global(locale);
    std::cin.imbue(locale);
    std::cout.imbue(locale);
    std::cerr.imbue(locale);
    std::wcin.imbue(locale);
    std::wcout.imbue(locale);
    std::wcerr.imbue(locale);

    // Imposta per ImGui
    if (ImGui::GetCurrentContext() != nullptr)
        ImGui::GetPlatformIO().Platform_LocaleDecimalPoint =
            static_cast<unsigned char>(*std::localeconv()->decimal_point);

    // -----

    AggiornaNomiLingue();
    AggiornaTestiGUI();
    AggiornaNomiTemi();
}

// Carica il file con il catalogo dei messaggi tradotti.
std::vector<char> CaricaCatalogoMessaggi(const std::string &nomeFile, const std::string & /*codifica*/)
{
    constexpr char prefisso[] = "/en_GB/";

    if (nomeFile.starts_with(prefisso))
        return { CatalogoLingue::en_GB::Data, CatalogoLingue::en_GB::Data + std::size(CatalogoLingue::en_GB::Data) };

    return {};
}

// ----- -----

static void AggiornaNomiLingue()
{
    size_t index = 0;

    Lingue[index++].nome = translate("Lingua di sistema");
    Lingue[index++].nome = translate("Italiano");
    Lingue[index++].nome = translate("Inglese (Inghilterra)");
}

static void AggiornaTestiGUI()
{
#define TESTO_GUI(var, testo) (TestiGUI.var = translate(testo).str() + "###" testo)
#include "TestiGUI.h"

#undef TESTO_GUI

    TestiDemoMarkdown[0].name = translate("Vuoto").str();

    TestiDemoMarkdown[1].name  = translate("Funzionalità").str();
    TestiDemoMarkdown[1].testo = da_u8string(translate(u8R"(## Unicode
⏏ ⏳🌟  

# Intestazione H1 [collegamento a Duck Duck Go](https://duckduckgo.com)
Testo normale.

## Intestazione H2 *corsivo* **grassetto** __sottolineato__ ***corsivo e grassetto*** __***sottolineato, corsivo e grassetto***__
Testo normale.

### Intestazione H3
Testo normale.

#### Intestazione H4
Testo normale.

##### Intestazione H5
Testo normale.

###### Intestazione H6
Testo normale.

###### Intestazione H7
Testo normale.

## Enfasi

***Testo corsivo e grassetto***.

*Testo corsivo*.

**Testo grassetto**.

__Testo sottolineato__.

**__Testo sottolineato e grassetto__**.

*__Testo sottolineato e corsivo__*.

***__Testo sottolineato, grassetto e corsivo__***.

~~Testo barrato~~

~~*Testo barrato e corsivo*~~

~~**Testo barrato e in grassetto**~~

~~***Testo barrato, corsivo e in grassetto***~~

~~__Testo barrato e sottolineato__~~

~~*__Testo barrato, corsivo e sottolineato__*~~

~~***__Testo barrato, corsivo, sottolineato e in grassetto__***~~

## Collegamenti

[Collegamento normale](https://example.com)

*[Collegamento in corsivo](https://example.com)*

**[Collegamento in grassetto](https://example.com)**

***[Collegamento in corsivo e grassetto](https://example.com)***

[Collegamento con titolo](https://example.com "Collegamento al dominio di esempio") non funziona.

## Collegamenti rapidi

<https://www.markdownguide.org>

<fake@example.com>

## Collegamenti con riferimenti

In un buco nella terra viveva un hobbit. Non si trattava di un buco sgradevole, sporco e umido,
riempito con gli avanzi di vermi e un odore di fango e melma, né di un buco asciutto, nudo e
sabbioso senza nulla su cui sedersi o da mangiare: era una [tana hobbit][1], e ciò significa comodità.

[1]: <https://it.wikipedia.org/wiki/Hobbit#Stile_di_vita> "Stili di vita degli Hobbit"

## Linee orizzontali

---
ABC
***
ABC
___

## Interruzioni di riga

In un buco<br>nella terra<br>viveva<br>un hobbit.

## Escape

\*\*\*Testo corsivo e grassetto\*\*\*.

\*Testo corsivo\*.

\*\*Testo grassetto\*\*.

\_\_Testo sottolineato\_\_.

\[Collegamento normale\]\(https://example.com\)

\<fake@example.com\>

\# Intestazione H1

\- elemento lista<br>
\- elemento lista

\* elemento lista<br>
\* elemento lista

\+ elemento lista<br>
\+ elemento lista

1\. elemento lista<br>
2\. elemento lista

\<br\>

\| Sintassi      \| Descrizione \|<br>
\| ------------- \| ----------- \|<br>
\| Intestazione  \| Titolo      \|<br>
\| Paragrafo     \| Testo       \|


## Elenco - numerato

Elenco con numeri sequenziali:
1. abc
2. elemento 2
    1. elemento 2.1
    2. elemento 2.2
3. elemento 3

Elenco con singolo numero ripetuto:
1. abc
1. elemento 2
   1. elemento 2.1
   1. elemento 2.2
1. elemento 3

Elenco con numeri fuori ordine:
1. abc
8. elemento 2
    1. elemento 2.1
    1. elemento 2.2
3. elemento 3

## Elenco - non numerato

Elenco non numerato con trattino:
- abc
- elemento 2
   - elemento 2.1
   - elemento 2.2
- elemento 3

Elenco non numerato con asterisco:
* abc
* elemento 2
    * elemento 2.1
    * elemento 2.2
* elemento 3

Elenco non numerato con segno più:
+ abc
+ elemento 2
    + elemento 2.1
    + elemento 2.2
+ elemento 3

## Tabelle

| Sintassi    | Descrizione |
| ----------- | ----------- |
| Intestazione| Titolo      |
| Paragrafo   | Testo       |

| Sintassi | Descrizione |
| --- | ----------- |
| Intestazione | Titolo |
| Paragrafo | Testo |

### Allineamento
| Sinistra  | Centro  | Destra |
| :---        |    :----:   |          ---: |
| Intestazione      | Titolo       | Siamo qui   |
| Paragrafo   | Testo        | Ed altro      |

## Tabelle con enfasi

| Sintassi  | Descrizione  |
| ----------- | ----------- |
| Intestazione      | Titolo       |
| *corsivo*   | normale        |
| normale   | **grassetto**        |
| normale   | __sottolineato__        |
| ***corsivo e grassetto***   | normale        |
| __***corsivo, grassetto e sottolineato***__   | normale        |

## Tabelle con collegamenti

| Sintassi  | Descrizione  |
| ----------- | ----------- |
| [normale](http://example.com)   | normale        |
| *[corsivo](http://example.com)*   | normale        |
| normale   | **[grassetto](http://example.com)**        |
| ***[corsivo e grassetto](http://example.com)***   | normale        |
)")
                                             .str());

    TestiDemoMarkdown[2].name  = "Lorem ipsum";
    TestiDemoMarkdown[2].testo = R"(# Aetate ego terras habebas

## Esse pars gravi consule

Lorem markdownum et neve meritis terrae festum, corpora quem Ampycides laudemque
velamina Halesi nitidissimus. Fatebor his tetigit enim, est placare; o Pallas
pro flammas. Spatio efferre, sonuere et nondum lugent plenos, verbis sit dictis
crevit amor [rogari horrida auctoris](https://duckduckgo.com/) crine adflata. Et magna crescit Tritoniaca
nec habere, non undas concutiens vaticinatus vixque.

## Eadem lunaribus ad spectatas

Atque ait quam possimne proceres; et mihi, est in voles. Scire temporis aliquid
et nostra per **aequorea** umbra quondam fassusque utque intremuit. Profusis
nostra gravis digitosque membra quicquid nostrae cum avem carpsit, est. Illa
iugulata: ante videri altera Cadmeida ter.

- Pereat Iovi signa invenit tepidique fessis veretur
- Tura seque ut venit aeternum possim
- Ereptas preces sole nostri superbia in sacro

## Fulgura tendentemque notavi saepe

Forma dolens tamen iuppiter veni circumspicit vidit; retinete satyri. Inquit
suspiria litus notissima nomen, lapidosos, arreptum eripe? Est tardi ad usae;
toto mors ira [vinaque cantare](https://www.example.com/), timidus nos dicta
amplexas quaeras molle. Iunctoque desierat vero, inquirant scilicet Idaeo magnas
feruntur augustae posse, Pelion? In pruinosas cunctantem, nostris **in qua**
Tigris venire.

1. Recursus fibris recessit geminum
  1. Suspicit addere animum nec femineos sanguine luna
  2. Aquoso aequales triceps ego
2. Iam vera
3. Ad detinet duo Erycina quod fores sic

## Imbribus orbis ex ictus

Harenis manes sua tempora? Bacchaeque sustinuere resoluta ardere nostrae frater,
Calydonius saltus simulacra. Tacetve carpitur moveres.

- Regi dentibus Liternum notum effecta
  - Si aurea
  - Sed ni qui parenti isdem meus carinas
    - Vesana fremit domo meos pariter
    - Suam pulcherrime mane studiosius Liber tumet
  - Manu opus munus obtusum
- Hylonome Danaas deprensum medullis coniunx inventa

## Hector Alemonides

Fluctus ille, enodisque defrenato partes mirabile Nycteus vixque clamantem
tellus et. Perdidit membra comites serpentis Sabaea, illic hanc aevo negabo.
Ante bis pectore futuri *et quid utque* ait exsul **nubibus**. Et erat ultimus,
has habuere artis, movit tuos nisi relinqui *curis* dotatissima, mea annua.
Defendit postquam hastas, ore ut cruor umida nescio: ubi miratur!

- Ille comitem Phoebus ostia potentia gratare auxilium
- Finemque vincula est nomen herbas adest
- Hylonome Danaas deprensum medullis coniunx inventa
- Manu opus munus obtusum
- Dum ater
- Ede feci Achille)";
}

// ----- -----

std::array<TestoDemoMarkdown, 3> TestiDemoMarkdown;
