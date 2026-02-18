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
static int ScalaGUIPercentuale          = 100; // [%]

// Macro utile a rendere il codice più corto e compatto. Il parametro testo verrà tradotto nella lingua corrente ed allo
// stesso tempo, in versione originale, è usato come identificativo di ImGui.
#define t(testo) (boost::locale::translate(testo).str() + "###" + (testo)).data()

static bool LinguaSelezionabile(const size_t i);

// ----- -----

void InizializzaGUI()
{
    ScalaGUIPercentuale = static_cast<int>(Impostazioni.scalaGUI * 100); // [%]
}

void GUI()
{
    // Possibilità di fare il docking di una finestra sui bordi dello schermo e non solo su un'altra finestra.
    ImGuiID idDockSpace = ImGui::DockSpaceOverViewport();

    // -----

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::MenuItem(t("Esci"))) Esci = true;
        if (ImGui::MenuItem(t("Impostazioni"))) finestraImpostazioniAperta = true;
        if (ImGui::BeginMenu(t("Debug")))
        {
            ImGui::MenuItem(t("Demo"), nullptr, &finestraDemoAperta);
            ImGui::MenuItem(t("Demo plot"), nullptr, &finestraDemoPlotAperta);
            ImGui::MenuItem(t("Editor stile"), nullptr, &finestraEditorStileAperta);
            ImGui::MenuItem(t("Metriche ImGui"), nullptr, &finestraMetricheImGuiAperta);
            ImGui::MenuItem(t("Debug log"), nullptr, &finestraDebugLogAperta);

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (ImGui::BeginMainStatusBar())
    {
        if (ImGui::SmallButton(t("Demo"))) finestraDemoAperta = !finestraDemoAperta;
        if (ImGui::SmallButton(t("Demo plot"))) finestraDemoPlotAperta = !finestraDemoPlotAperta;
        if (ImGui::SmallButton(t("Editor stile"))) finestraEditorStileAperta = !finestraEditorStileAperta;
        if (ImGui::SmallButton(t("Metriche ImGui"))) finestraMetricheImGuiAperta = !finestraMetricheImGuiAperta;
        if (ImGui::SmallButton(t("Debug log"))) finestraDebugLogAperta = !finestraDebugLogAperta;

        ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);

        char testoStatistiche[512];
        int lunghezzaTesto = ImFormatString(
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
        if (ImGui::Begin(t("Editor stile"), &finestraEditorStileAperta))
            ImGui::ShowStyleEditor(&Temi[Impostazioni.temaSelezionato].stile);
        ImGui::End();
    }

    if (finestraImpostazioniAperta)
    {
        bool impostazioniModificate = false;

        if (ImGui::Begin(t("Impostazioni"), &finestraImpostazioniAperta, ImGuiWindowFlags_AlwaysAutoResize))
        {
            if (ImGui::BeginCombo(t("Lingua"), Lingue[Impostazioni.linguaSelezionata].nome.data()))
            {
                if (LinguaSelezionabile(0)) impostazioniModificate = true;

                if (ImGui::BeginTable("", 2))
                {
                    for (size_t i = 1; i < Lingue.size(); ++i)
                    {
                        ImGui::TableNextColumn();
                        ImGui::TextAligned(0.5f, -FLT_MIN, Lingue[i].bandiera);
                        ImGui::TableNextColumn();

                        if (LinguaSelezionabile(i)) impostazioniModificate = true;
                    }

                    ImGui::EndTable();
                }

                ImGui::EndCombo();
            }

            if (ImGui::BeginCombo(t("Tema"), Temi[Impostazioni.temaSelezionato].nome.data()))
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
                const int incremento = 10;
                ImGui::InputScalar(
                    t("Zoom IU"), ImGuiDataType_S32, &ScalaGUIPercentuale, &incremento, nullptr, "%d %%");
                if (ImGui::IsItemDeactivatedAfterEdit())
                {
                    Impostazioni.scalaGUI  = ScalaGUIPercentuale / 100.0f;
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

            if (ImGui::Button(t("Default")))
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

            if (ImGui::Button(t("Annulla"))) finestraImpostazioniAperta = false;
        }
        ImGui::End();

        if (impostazioniModificate) SalvaImpostazioni();
    }

    if (finestraDemoPlotAperta) ImPlot::ShowDemoWindow(&finestraDemoPlotAperta);
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
    else if (i == Impostazioni.linguaSelezionata) ImGui::SetItemDefaultFocus();

    return false;
}
