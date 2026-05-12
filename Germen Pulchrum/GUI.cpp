#include "IntestazionePrecompilata.h"

#include "CostantiEdAltro.h"
#include "Disegnatore.h"
#include "Estensioni ImGui/Estensioni.h"
#include "Impostazioni.h"
#include "Internazionalizzazione.h"

// ----- -----

static bool finestraDemoAperta          = false;
static bool finestraImpostazioniAperta  = false;
static bool finestraEditorStileAperta   = false;
static bool finestraMetricheImGuiAperta = false;
static bool finestraDebugLogAperta      = false;
static bool finestraDemoPlotAperta      = false;
static bool finestraMarkdownAperta      = false;
static int ScalaGUIPercentuale          = 100; // [%]

static std::string testoMarkdown = R"(# Aetate ego terras habebas

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

static bool LinguaSelezionabile(size_t i);

static void FinestraMarkDown();

// ----- -----

void InizializzaGUI()
{
    ScalaGUIPercentuale = static_cast<int>(Impostazioni.scalaGUI * 100); // [%]

    ImGui::GetPlatformIO().Platform_LocaleDecimalPoint = '.';
}

void GUI()
{
    // Possibilità di fare il docking di una finestra sui bordi dello schermo e non solo su un'altra finestra.
    ImGui::DockSpaceOverViewport();

    // -----

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::MenuItem(TestiGUI.menuEsci.data())) Esci = true;
        if (ImGui::MenuItem(TestiGUI.impostazioni.data())) finestraImpostazioniAperta = true;
        if (ImGui::BeginMenu(TestiGUI.menuDebug.data()))
        {
            ImGui::MenuItem(TestiGUI.demo.data(), nullptr, &finestraDemoAperta);
            ImGui::MenuItem(TestiGUI.demoPlot.data(), nullptr, &finestraDemoPlotAperta);
            ImGui::MenuItem(TestiGUI.editorStile.data(), nullptr, &finestraEditorStileAperta);
            ImGui::MenuItem(TestiGUI.metricheImGui.data(), nullptr, &finestraMetricheImGuiAperta);
            ImGui::MenuItem(TestiGUI.debugLog.data(), nullptr, &finestraDebugLogAperta);
            ImGui::MenuItem("Markdown", nullptr, &finestraMarkdownAperta);

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (ImGui::BeginMainStatusBar())
    {
        if (ImGui::SmallButton(TestiGUI.demo.data())) finestraDemoAperta = !finestraDemoAperta;
        if (ImGui::SmallButton(TestiGUI.demoPlot.data())) finestraDemoPlotAperta = !finestraDemoPlotAperta;
        if (ImGui::SmallButton(TestiGUI.editorStile.data())) finestraEditorStileAperta = !finestraEditorStileAperta;
        if (ImGui::SmallButton(TestiGUI.metricheImGui.data()))
            finestraMetricheImGuiAperta = !finestraMetricheImGuiAperta;
        if (ImGui::SmallButton(TestiGUI.debugLog.data())) finestraDebugLogAperta = !finestraDebugLogAperta;
        if (ImGui::SmallButton("Markdown")) finestraMarkdownAperta = !finestraMarkdownAperta;

        ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);

        char testoStatistiche[512];
        const int lunghezzaTesto = ImFormatString(
            testoStatistiche,
            std::size(testoStatistiche),
            "%.3f ms (%*.3f FPS)",
            DurataFotogramma,
            AbilitàVSync ? 6 : 8,
            1000.0 / DurataFotogramma);

        // Allineamento a destra del testo
        ImGui::SetCursorPosX(
            ImGui::GetCursorPosX()
            + ImMax(
                0.0f,
                ImGui::GetContentRegionAvail().x
                    - ImGui::CalcTextSize(testoStatistiche, testoStatistiche + lunghezzaTesto).x));

        ImGui::TextUnformatted(testoStatistiche, testoStatistiche + lunghezzaTesto);

        ImGui::EndMainStatusBar();
    }

    // -----

    if (finestraDemoAperta) ImGui::ShowDemoWindow(&finestraDemoAperta);
    if (finestraMetricheImGuiAperta) ImGui::ShowMetricsWindow(&finestraMetricheImGuiAperta);
    if (finestraDebugLogAperta) ImGui::ShowDebugLogWindow(&finestraDebugLogAperta);

    if (finestraEditorStileAperta)
    {
        if (ImGui::Begin(TestiGUI.editorStile.data(), &finestraEditorStileAperta))
            ImGui::ShowStyleEditor(&Temi[Impostazioni.temaSelezionato].stile);
        ImGui::End();
    }

    if (finestraImpostazioniAperta)
    {
        bool impostazioniModificate = false;

        if (ImGui::Begin(TestiGUI.impostazioni.data(), &finestraImpostazioniAperta, ImGuiWindowFlags_AlwaysAutoResize))
        {
            if (ImGui::BeginCombo(TestiGUI.lingua.data(), Lingue[Impostazioni.linguaSelezionata].nome.data()))
            {
                if (LinguaSelezionabile(0)) impostazioniModificate = true;

                if (ImGui::BeginTable("", 2))
                {
                    for (size_t i = 1; i < Lingue.size(); ++i)
                    {
                        ImGui::TableNextColumn();
                        ImGui::TextAligned(0.5f, -std::numeric_limits<float>::min(), Lingue[i].bandiera);
                        ImGui::TableNextColumn();

                        if (LinguaSelezionabile(i)) impostazioniModificate = true;
                    }

                    ImGui::EndTable();
                }

                ImGui::EndCombo();
            }

            if (ImGui::BeginCombo(TestiGUI.tema.data(), Temi[Impostazioni.temaSelezionato].nome.data()))
            {
                for (size_t i = 0; i < Temi.size(); ++i)
                    if (ImGui::Selectable(
                            Temi[i].nome.data(), i == Impostazioni.temaSelezionato, ImGuiSelectableFlags_SelectOnNav)
                        && i != Impostazioni.temaSelezionato)
                    {
                        Impostazioni.temaSelezionato = i;
                        ImGui::GetStyle()            = Temi[i].stile;
                        impostazioniModificate       = true;
                    }
                    else if (i == Impostazioni.temaSelezionato) ImGui::SetItemDefaultFocus();

                ImGui::EndCombo();
            }

            {
                constexpr int incremento = 10;
                ImGui::InputScalar(
                    TestiGUI.zoomIU.data(), ImGuiDataType_S32, &ScalaGUIPercentuale, &incremento, nullptr, "%d %%");
                if (ImGui::IsItemDeactivatedAfterEdit())
                {
                    Impostazioni.scalaGUI  = static_cast<float>(ScalaGUIPercentuale) / 100.0f;
                    Impostazioni.scalaGUI  = std::max(Impostazioni.scalaGUI, 0.3f);
                    Impostazioni.scalaGUI  = std::min(Impostazioni.scalaGUI, 2.0f);
                    ScalaGUIPercentuale    = static_cast<int>(Impostazioni.scalaGUI * 100.0f);
                    impostazioniModificate = true;
                    AggiornaScalaGUI();
                }
            }

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            if (ImGui::Button(TestiGUI.default_.data()))
            {
                Impostazioni.linguaSelezionata = 0;
                ImpostaLingua(0);

                Impostazioni.temaSelezionato = 0;

                if (Impostazioni.scalaGUI != 1.0f)
                {
                    Impostazioni.scalaGUI = 1.0f;
                    ScalaGUIPercentuale   = 100;
                    AggiornaScalaGUI();
                }
                // AggiornaScalaGUI() provvede già a re-impostare il tema.
                else ImGui::GetStyle() = Temi[0].stile;

                impostazioniModificate = true;
            }

            ImGui::SameLine();

            if (ImGui::Button(TestiGUI.annulla.data())) finestraImpostazioniAperta = false;
        }
        ImGui::End();

        if (impostazioniModificate) SalvaImpostazioni();
    }

    if (finestraDemoPlotAperta) ImPlot::ShowDemoWindow(&finestraDemoPlotAperta);

    if (finestraMarkdownAperta) FinestraMarkDown();
}

void AggiornaScalaGUI()
{
    InizializzaTemi();
    ImGui::GetStyle() = Temi[Impostazioni.temaSelezionato].stile;
}

// ----- -----

// Restituisce True se viene cambiata la lingua.
static bool LinguaSelezionabile(const size_t i)
{
    if (ImGui::Selectable(
            Lingue[i].nome.data(),
            i == Impostazioni.linguaSelezionata,
            ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SpanAllColumns)
        && i != Impostazioni.linguaSelezionata)
    {
        Impostazioni.linguaSelezionata = i;
        ImpostaLingua(i);
        return true;
    }

    if (i == Impostazioni.linguaSelezionata) ImGui::SetItemDefaultFocus();

    return false;
}

// ----- Markdown -----

static std::string testTestoMarkdown = (char *)u8R"(
## Unicode
⏏ ⏳🌟  

# Intestazione H1 [Link: Duck Duck Go](https://duckduckgo.com)
Testo normale.

## Intestazione H2 *italico* **grassetto** __sottolineato__ ***italico e grassetto*** __***sottolineato, italico e grassetto***__
Testo normale.

### Intestazione H3
Testo normale.

#### Intestazione H4
Testo normale.

##### Intestazione H5
Testo normale.

##### Intestazione H6
Testo normale.

###### Intestazione H7
Testo normale.

## Enfasi

***Testo italico e grassetto***.

*Testo italico*.

**Testo grassetto**.

__Testo sottolineato__.

**__Testo sottolineato e grassetto__**.

*__Testo sottolineato ed italico__*.

***__Testo sottolineato, grassetto ed italico__***.

~~Testo barrato~~

~~*Testo barrato ed italico*~~

~~**Testo barrato in grassetto**~~

~~***Testo barrato, italico ed in grassetto***~~

~~__Testo barrato e sottolineato__~~

~~*__Testo barrato, italico e sottolineato__*~~

~~***__Testo barrato, italico, sottolineato ed in grassetto__***~~

## Collegamenti

[Collegamento normale](https://example.com)

*[Collegamento in italico](https://example.com)*

**[Collegamento in grassetto](https://example.com)**

***[Collegamento in italico e grassetto](https://example.com)***

[Collegamento con titolo](https://example.com "Collegamento al dominio di esempio") non funziona.

## Collegamenti veloci

<https://www.markdownguide.org>

<fake@example.com>

## Collegamenti con riferimenti

In a hole in the ground there lived a hobbit. Not a nasty, dirty, wet hole, filled with the ends
of worms and an oozy smell, nor yet a dry, bare, sandy hole with nothing in it to sit down on or to
eat: it was a [hobbit-hole][1], and that means comfort.

[1]: <https://en.wikipedia.org/wiki/Hobbit#Lifestyle> "Hobbit lifestyles"

## Linee orizzontali

---
ABC
***
ABC
___

## Interruzioni di linea

In a hole<br>in the ground<br>there<br>lived a hobbit.

## Escape

\*\*\*Testo italico e grassetto\*\*\*.

\*Testo italico\*.

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

\| Syntax      \| Description \|<br>
\| ----------- \| ----------- \|<br>
\| Header      \| Title       \|<br>
\| Paragraph   \| Text        \|


## Liste - numerate

Lista con numeri sequenziali:
1. abc
2. elemento 2
    1. elemento 2.1
    2. elemento 2.2
3. elemento 3

Lista con singolo numero ripetuto:
1. abc
1. elemento 2
   1. elemento 2.1
   1. elemento 2.2
1. elemento 3

Lista con numeri fuori ordine:
1. abc
8. elemento 2
    1. elemento 2.1
    1. elemento 2.2
3. elemento 3

## Liste - non numerate

Lista non numerata col trattino:
- abc
- elemento 2
   - elemento 2.1
   - elemento 2.2
- elemento 3

Lista non numerata con l'asterisco:
* abc
* elemento 2
    * elemento 2.1
    * elemento 2.2
* elemento 3

Lista non numerata col più:
+ abc
+ elemento 2
    + elemento 2.1
    + elemento 2.2
+ elemento 3

## Tabelle

| Syntax      | Description |
| ----------- | ----------- |
| Header      | Title       |
| Paragraph   | Text        |

| Syntax | Description |
| --- | ----------- |
| Header | Title |
| Paragraph | Text |

### Allineamento
| Sinistra  | Centro  | Destra |
| :---        |    :----:   |          ---: |
| Header      | Title       | Here's this   |
| Paragraph   | Text        | And more      |

## Tabelle con enfasi

| Syntax  | Description  |
| ----------- | ----------- |
| Header      | Title       |
| *italico*   | normale        |
| normale   | **grassetto**        |
| normale   | __sottolineato__        |
| ***italico e grassetto***   | normale        |
| __***italico, grassetto e sottolineato***__   | normale        |

## Tabelle con collegamenti

| Syntax  | Description  |
| ----------- | ----------- |
| [normale](http://example.com)   | normale        |
| *[italico](http://example.com)*   | normale        |
| normale   | **[grassetto](http://example.com)**        |
| ***[italico e grassetto](http://example.com)***   | normale        |
)";

static std::string tabella = R"(
### Allineamento

| Sinistra  | Centro  | Destra |
| :---        |    :----:   |          ---: |
| Header      | Title       | Here's this   |
| Paragraph   | Text        | And more      |

## Tabelle con enfasi

| Syntax  | Description  |
| ----------- | ----------- |
| Header      | Title       |
| *italico*   | normale        |
| normale   | **grassetto**        |
| normale   | __sottolineato__        |
| ***italico e grassetto***   | normale        |
| __***italico, grassetto e sottolineato***__   | normale        |

## Tabelle con collegamenti

| Syntax  | Description  |
| ----------- | ----------- |
| [normale](http://example.com)   | normale        |
| *[italico](http://example.com)*   | normale        |
| normale   | **[grassetto](http://example.com)**        |
| ***[italico e grassetto](http://example.com)***   | normale        |

## Tabella

| Syntax | Description |
| ----------- | ----------- |
| Paragraph Paragraph Paragraph Paragraph Paragraph Paragraph Paragraph Paragraph Paragraph   | Text        |
| *italico*   | normale        |
| normale   | **grassetto**        |
| normale   | __sottolineato__        |
| ***italico e grassetto***   | normale        |
| __***italico, grassetto e sottolineato***__   | normale        |
| [normale](http://example.com)   | normale        |
| *[italico](http://example.com)*   | normale        |
| normale   | **[grassetto](http://example.com)**        |
| ***[italico e grassetto](http://example.com)***   | normale        |
)";

struct Markdown: public imgui_md
{
    void get_font(font_info &info) const override
    {
        if (m_is_table_header)
        {
            info.font = FontGrassetto;
            info.size = DimensioneDefaultFont;
            return;
        }

        // Tipo di font

        // Grassetto ed italico
        if (m_is_strong && m_is_em) info.font = FontItalicoGrassetto;
        // Grassetto
        else if (m_is_strong) info.font = FontGrassetto;
        // Italico
        else if (m_is_em) info.font = FontItalico;
        // Testo normale
        else if (m_hlevel == 0) info.font = FontNormale;
        else info.font = FontGrassetto;

        // Dimensione font - scala la dimensione in base al livello dell'intestazione.

        // I valori di scala corrispondono alle dimensioni del testo definite nel HTML.
        float scalaPerLivello[] = {
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

    void open_url() const override
    {
        if (!SDL_OpenURL(m_href.data()))
            std::cout << "[Errore] Impossibile aprire il URL '" << m_href << "': " << SDL_GetError() << '\n';
    }
};

static Markdown md;

static void FinestraMarkDown()
{
    if (ImGui::Begin("Markdown", &finestraMarkdownAperta))
    {
        constexpr bool OrientamentoVerticale = true;

        ImVec2 dimensioneInputTesto = { -1, -1 };
        if constexpr (OrientamentoVerticale) dimensioneInputTesto.x = ImGui::GetContentRegionAvail().x * 0.5f;
        else dimensioneInputTesto.y = ImGui::GetContentRegionAvail().y * 0.4f;

        ImGui::InputTextMultiline(
            "###TestoMarkdown",
            &testTestoMarkdown,
            dimensioneInputTesto,
            ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_WordWrap);

        if constexpr (OrientamentoVerticale) ImGui::SameLine();
        else ImGui::Spacing();

        ImGui::BeginChild("###Anteprima", ImVec2(0, 0), ImGuiChildFlags_Borders);
        md.print(testTestoMarkdown.data(), testTestoMarkdown.data() + testTestoMarkdown.size());
        ImGui::EndChild();
    }
    ImGui::End();
}
