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

static ImGui::MarkdownConfig configurazioneMarkdown;

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

static void CallbackApriCollegamentoMarkdown(ImGui::MarkdownLinkCallbackData data);
static void CallbackSuggerimentoMarkdown(ImGui::MarkdownTooltipCallbackData data);
static void CallbackFormattazioneMarkdown(const ImGui::MarkdownFormatInfo &infoFormattazione, bool inizio);

// ----- -----

void InizializzaGUI()
{
    ScalaGUIPercentuale = static_cast<int>(Impostazioni.scalaGUI * 100); // [%]

    ImGui::GetPlatformIO().Platform_LocaleDecimalPoint = '.';

    configurazioneMarkdown = { .linkCallback    = CallbackApriCollegamentoMarkdown,
                               .tooltipCallback = CallbackSuggerimentoMarkdown, // opzionale
                               .imageCallback   = nullptr,
                               .linkIcon        = reinterpret_cast<const char *>(u8"🔗"),
                               .headingFormats  = { { .font = FontGrassettoH1, .separator = true },
                                                    { .font = FontGrassettoH2, .separator = true },
                                                    { .font = FontGrassettoH3, .separator = false } },
                               .userData        = nullptr,
                               .formatCallback  = CallbackFormattazioneMarkdown,
                               .formatFlags     = ImGuiMarkdownFormatFlags_CommonMarkAll };
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

    if (finestraMarkdownAperta)
    {
        if (ImGui::Begin("Markdown", &finestraMarkdownAperta))
        {
            constexpr bool OrientamentoVerticale = true;

            ImVec2 dimensioneInputTesto = { -1, -1 };
            if constexpr (OrientamentoVerticale) dimensioneInputTesto.x = ImGui::GetContentRegionAvail().x * 0.5f;
            else dimensioneInputTesto.y = ImGui::GetContentRegionAvail().y * 0.4f;

            ImGui::InputTextMultiline(
                "###TestoMarkdown",
                &testoMarkdown,
                dimensioneInputTesto,
                ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_WordWrap);

            if constexpr (OrientamentoVerticale) ImGui::SameLine();
            else ImGui::Spacing();

            ImGui::BeginChild("###Anteprima", ImVec2(0, 0), ImGuiChildFlags_Borders);
            ImGui::Markdown(testoMarkdown.data(), testoMarkdown.size(), configurazioneMarkdown);
            ImGui::EndChild();
        }
        ImGui::End();
    }
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

static void CallbackApriCollegamentoMarkdown(ImGui::MarkdownLinkCallbackData data)
{
    const std::string url(data.link, data.linkLength);
    if (!SDL_OpenURL(url.data()))
        std::cout << "[Errore] Impossibile aprire il URL '" << url << "': " << SDL_GetError() << '\n';
}

/// @brief Invocata da ImGui Markdown quando è necessario mostrare un suggerimento (tooltip).
/// Al momento è invocata quando il mouse si trova sopra un collegamento.
static void CallbackSuggerimentoMarkdown(ImGui::MarkdownTooltipCallbackData data)
{
    if (data.linkData.isImage) ImGui::SetTooltip("%.*s", data.linkData.linkLength, data.linkData.link);
    else ImGui::SetTooltip("%s %.*s", data.linkIcon, data.linkData.linkLength, data.linkData.link);
}

/// @brief Invocata da ImGui Markdown subito prima e subito dopo il rendering di un elemento così da poter
/// personalizzare l'aspetto di quest'ultimo.
/// @param infoFormattazione informazioni sull'elemento disegnato e sulla configurazione di ImGui Markdown.
/// @param inizio value True prima del disegno dell'elemento e False dopo.
static void CallbackFormattazioneMarkdown(const ImGui::MarkdownFormatInfo &infoFormattazione, const bool inizio)
{
    switch (infoFormattazione.type)
    {
        case ImGui::MarkdownFormatType::NORMAL_TEXT:
            break;
        case ImGui::MarkdownFormatType::EMPHASIS:
            // Italico (enfasi normale)
            if (infoFormattazione.level == 1)
                if (inizio) ImGui::PushFont(FontItalico, 0.0f);
                else ImGui::PopFont();
            // Grassetto (enfasi forte)
            else if (inizio) ImGui::PushFont(FontGrassetto, 0.0f);
            else ImGui::PopFont();

            break;
        case ImGui::MarkdownFormatType::HEADING:
        {
            const ImGui::MarkdownHeadingFormat &formatoIntestazione =
                infoFormattazione.level > ImGui::MarkdownConfig::NUMHEADINGS
                    ? infoFormattazione.config->headingFormats[ImGui::MarkdownConfig::NUMHEADINGS - 1]
                    : infoFormattazione.config->headingFormats[infoFormattazione.level - 1];

            if (inizio)
            {
                if (!(infoFormattazione.config->formatFlags & ImGuiMarkdownFormatFlags_NoNewLineBeforeHeading))
                    ImGui::NewLine();
                if (formatoIntestazione.font)
                    ImGui::PushFont(
                        formatoIntestazione.font,
                        formatoIntestazione.fontSize > 0.0f ? formatoIntestazione.fontSize
                                                            : formatoIntestazione.font->LegacySize);
            }
            else
            {
                if (formatoIntestazione.separator)
                {
                    const ImVec2 cursor = ImGui::GetCursorPos();
                    ImGui::Separator();
                    if (infoFormattazione.config->formatFlags & ImGuiMarkdownFormatFlags_SeparatorDoesNotAdvance)
                        ImGui::SetCursorPos(cursor);
                }
                if (formatoIntestazione.font) ImGui::PopFont();
                ImGui::NewLine();
            }
            break;
        }
        case ImGui::MarkdownFormatType::UNORDERED_LIST:
            break;
        case ImGui::MarkdownFormatType::LINK:
            if (inizio) ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
            else
            {
                ImGui::PopStyleColor();
                if (infoFormattazione.itemHovered) ImGui::UnderLine(ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
                else ImGui::UnderLine(ImGui::GetStyle().Colors[ImGuiCol_Button]);
            }
            break;
    }
}
