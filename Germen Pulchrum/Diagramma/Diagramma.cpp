#include "IntestazionePrecompilata.h"

#include "Diagramma.h"

#include "Disegnatore.h"
#include "Impostazioni.h"
#include "Vec2.h"

#include <cgraph.h>
#include <colorprocs.h>
#include <gvc.h>
#include <gvplugin.h>

namespace Diagramma
{
    // Numero di punti tipografici per pollice.
    static const constexpr float PUNTI_PER_POLLICE = 72.0f;

    // Numero di pixel per punto tipografico.
    // Siccome:
    //  - 1 punto tipografico = 1/72 di pollice;
    //  - DPI = numero di punti per pollice;
    //  - gli schermi digitali hanno sempre un pixel per punto;
    // allora
    //  numero pixel = (punto tipografico) * (DPI / 72)
    static const constexpr float PIXEL_PER_PUNTO = 96.0f / PUNTI_PER_POLLICE;

    struct Colore
    {
        ImColor colore;
        bool valido;
    };

    extern "C"
    {
        extern gvplugin_library_t gvplugin_core_LTX_library;
        extern gvplugin_library_t gvplugin_dot_layout_LTX_library;
    }

    static constexpr lt_symlist_t gvPlugins[] = { { "gvplugin_core_LTX_library", &gvplugin_core_LTX_library },
                                                  { "gvplugin_dot_layout_LTX_library",
                                                    &gvplugin_dot_layout_LTX_library },
                                                  { 0, 0 } };

    static GVC_t *contestoGV      = nullptr;
    static Agraph_t *currentGraph = nullptr;
    static float Scala            = 1.0f;
    float ScalaDiagramma          = 1.5f;

    static void DisegnaNodi(ImDrawList *const draw, const Vec2 &posDiagramma);
    static void DisegnaArchi(ImDrawList *const draw, const Vec2 &posDiagramma, Agnode_t *const n);
    static void DisegnaPuntaFreccia(ImDrawList *const lista, const Vec2 &apice, const Vec2 &da, ImU32 colore);
    static void DisegnaScritta(
        ImDrawList *const draw,
        const Vec2 &posDiagramma,
        const textlabel_t *const label,
        ImU32 colorePredefinito,
        const pointf *const posizione = nullptr);
    static Colore EstraiColore(void *oggetto, const char *nome, ImColor colorePredefinito);
    static Colore EstraiColore(const char *nome, ImColor colorePredefinito);
    static Vec2 ConvertiPunto(const Vec2 &punto, const float altezzaDiagramma, const Vec2 &origine);

    bool Inizializza()
    {
        contestoGV = gvContextPlugins(gvPlugins, 0);
        return contestoGV != nullptr;
    }

    void Rilascia()
    {
        gvFreeContext(contestoGV);
    }

    void Disegna(const std::string &codice)
    {
        // ----- Costruzione e disposizione diagramma

        {
            Agraph_t *newGraph = agmemread(codice.c_str());

            if (!newGraph)
            {
                // sintassi non valida: mantenere a video l'ultimo layout valido,
                // mostrare l'errore catturato (vedi 4.3)
            }
            else
            {
                if (currentGraph)
                {
                    gvFreeLayout(contestoGV, currentGraph);
                    agclose(currentGraph);
                }
                gvLayout(contestoGV, newGraph, "dot");
                currentGraph = newGraph;
            }

            if (!currentGraph) return;
        }

        // Da qui in vanti currentGraph contiene il diagramma da disegnare.

        // -----

        ImDrawList *draw      = ImGui::GetWindowDrawList();
        const Vec2 cursorPos  = ImGui::GetCursorScreenPos();
        const Vec2 spaceAvail = ImGui::GetContentRegionAvail();

        // GD_bb(currentGraph) = bounding box del diagramma:
        // .LL = coordinate vertice min della bounding box.
        // .UR = coordinate vertice max della bounding box.

        // Coordinate punto max della bounding box del diagramma.
        const Vec2 dimensione = Vec2(GD_bb(currentGraph).UR) * PIXEL_PER_PUNTO * Scala;

        // ----- Allineamento diagramma

        // Allinea al centro.
        // const Vec2 posDiagramma(cursorPos.x + (spaceAvail.x - dimensione.x) / 2.0f, cursorPos.y);

        // Allinea a destra.
        // const Vec2 posDiagramma(cursorPos.x + (spaceAvail.x - dimensione.x), cursorPos.y);

        // Allinea a sinistra.
        const Vec2 posDiagramma(cursorPos);

        // ----- Fattore di scala del diagramma

        Scala = ScalaDPI * Impostazioni.scalaGUI * ScalaDiagramma;

        // ----- Disegna lo sfondo del diagramma

        {
            const Colore colore = EstraiColore(currentGraph, "bgcolor", IM_COL32(255, 255, 255, 255));
            if (colore.valido)
            {
                const Vec2 min = posDiagramma;
                const Vec2 max = posDiagramma + dimensione;
                draw->AddRectFilled(min, max, colore.colore);
            }
        }

        // -----

        DisegnaNodi(draw, posDiagramma);
    }

    static void DisegnaNodi(ImDrawList *const draw, const Vec2 &posDiagramma)
    {
        const float altezzaDiagramma = static_cast<float>(GD_bb(currentGraph).UR.y);

        // -----

        for (Agnode_t *nodo = agfstnode(currentGraph); nodo; nodo = agnxtnode(currentGraph, nodo))
        {
            // Conversione da pollici a punti tipografici
            Vec2 metàDimensione(
                static_cast<float>(ND_width(nodo)) * 0.5f * PUNTI_PER_POLLICE,
                static_cast<float>(ND_height(nodo)) * 0.5f * PUNTI_PER_POLLICE);

            // Coordinate del centro del nodo.
            const pointf &centro = ND_coord(nodo);

            const Colore coloreBordo       = EstraiColore(nodo, "color", IM_COL32(0, 0, 0, 255));
            const Colore coloreRiempimento = EstraiColore(nodo, "fillcolor", IM_COL32(255, 255, 255, 255));

            const shape_desc *shape = ND_shape(nodo);
            if (std::strcmp(shape->name, "diamond") == 0)
            {
                const Vec2 cima =
                    ConvertiPunto({ centro.x, centro.y + metàDimensione.y }, altezzaDiagramma, posDiagramma);
                const Vec2 fondo =
                    ConvertiPunto({ centro.x, centro.y - metàDimensione.y }, altezzaDiagramma, posDiagramma);
                const Vec2 sinistra =
                    ConvertiPunto({ centro.x - metàDimensione.x, centro.y }, altezzaDiagramma, posDiagramma);
                const Vec2 destra =
                    ConvertiPunto({ centro.x + metàDimensione.x, centro.y }, altezzaDiagramma, posDiagramma);

                if (coloreRiempimento.valido)
                    draw->AddQuadFilled(cima, destra, fondo, sinistra, coloreRiempimento.colore);
                draw->AddQuad(cima, destra, fondo, sinistra, coloreBordo.colore);
            }
            else if (std::strcmp(shape->name, "ellipse") == 0 || std::strcmp(shape->name, "oval") == 0)
            {
                const Vec2 raggio        = metàDimensione * PIXEL_PER_PUNTO * Scala;
                const Vec2 centroInPixel = ConvertiPunto(centro, altezzaDiagramma, posDiagramma);

                if (coloreRiempimento.valido) draw->AddEllipseFilled(centroInPixel, raggio, coloreRiempimento.colore);
                draw->AddEllipse(centroInPixel, raggio, coloreBordo.colore);
            }
            else if (std::strcmp(shape->name, "circle") == 0)
            {
                // Graphviz garantisce larghezza == altezza per circle.
                const float raggio       = metàDimensione.x * PIXEL_PER_PUNTO * Scala;
                const Vec2 centroInPixel = ConvertiPunto(centro, altezzaDiagramma, posDiagramma);

                if (coloreRiempimento.valido) draw->AddCircleFilled(centroInPixel, raggio, coloreRiempimento.colore);
                draw->AddCircle(centroInPixel, raggio, coloreBordo.colore);
            }
            // Default è box, rect oppure rectangle.
            else
            {
                const Vec2 min = ConvertiPunto(centro - metàDimensione, altezzaDiagramma, posDiagramma);
                const Vec2 max = ConvertiPunto(centro + metàDimensione, altezzaDiagramma, posDiagramma);

                if (coloreRiempimento.valido) draw->AddRectFilled(min, max, coloreRiempimento.colore);
                draw->AddRect(min, max, coloreBordo.colore);
            }

            // ----- Disegno etichette

            {
                const textlabel_t *const label = ND_label(nodo);
                DisegnaScritta(draw, posDiagramma, label, IM_COL32(0, 0, 0, 255), &centro);
            }

            // -----

            DisegnaArchi(draw, posDiagramma, nodo);
        }
    }

    static void DisegnaArchi(ImDrawList *const draw, const Vec2 &posDiagramma, Agnode_t *const nodo)
    {
        const float altezzaDiagramma = static_cast<float>(GD_bb(currentGraph).UR.y);

        for (Agedge_t *arco = agfstout(currentGraph, nodo); arco; arco = agnxtout(currentGraph, arco))
        {
            const splines *spline = ED_spl(arco);
            if (!spline) continue;

            const Colore colore = EstraiColore(arco, "color", IM_COL32(0, 0, 0, 255));

            for (size_t i = 0; i < spline->size; ++i)
            {
                const bezier &bezier = spline->list[i];

                // I punti di controllo arrivano in gruppi di 3 (segmenti cubici),
                // preceduti da un punto iniziale: layout è [p0, c1, c2, p1, c1, c2, p1, ...]
                // bezier.list[0] è il punto di partenza della prima bezier.
                for (size_t j = 0; j + 3 < bezier.size; j += 3)
                {
                    const Vec2 p0 = ConvertiPunto(bezier.list[j + 0], altezzaDiagramma, posDiagramma);
                    const Vec2 c1 = ConvertiPunto(bezier.list[j + 1], altezzaDiagramma, posDiagramma);
                    const Vec2 c2 = ConvertiPunto(bezier.list[j + 2], altezzaDiagramma, posDiagramma);
                    const Vec2 p1 = ConvertiPunto(bezier.list[j + 3], altezzaDiagramma, posDiagramma);

                    draw->AddBezierCubic(p0, c1, c2, p1, colore.colore, 1.0f);
                }

                // Punta della freccia all'inizio dell'arco.
                if (bezier.sflag)
                {
                    const Vec2 apice = ConvertiPunto(bezier.sp, altezzaDiagramma, posDiagramma);
                    const Vec2 da    = ConvertiPunto(bezier.list[1], altezzaDiagramma, posDiagramma);
                    DisegnaPuntaFreccia(draw, apice, da, colore.colore);
                }

                // Punta della freccia alla fine dell'arco.
                if (bezier.eflag)
                {
                    const Vec2 apice = ConvertiPunto(bezier.ep, altezzaDiagramma, posDiagramma);
                    const Vec2 da    = ConvertiPunto(bezier.list[bezier.size - 1], altezzaDiagramma, posDiagramma);
                    DisegnaPuntaFreccia(draw, apice, da, colore.colore);
                }
            }

            // ----- Disegno etichetta

            {
                const textlabel_t *const label = ED_label(arco);
                DisegnaScritta(draw, posDiagramma, label, IM_COL32(0, 0, 0, 255));
            }
        }
    }

    static void DisegnaPuntaFreccia(ImDrawList *const draw, const Vec2 &apice, const Vec2 &da, ImU32 colore)
    {
        // Direzione verso l'apice della freccia.
        Vec2 direzione(apice.x - da.x, apice.y - da.y);
        if (!direzione.Normalize()) return;

        // Versore perpendicolare.
        const Vec2 n(-direzione.y, direzione.x);

        static constexpr float LUNGHEZZA_FRECCIA = 15.0f;                    // pixel
        static constexpr float LARGHEZZA_FRECCIA = LUNGHEZZA_FRECCIA * 0.5f; // pixel

        // Punto centrale della base del triangolo, arretrata rispetto alla punta
        const Vec2 base = apice - direzione * LUNGHEZZA_FRECCIA * Scala;

        const Vec2 v0 = apice;
        const Vec2 v1 = base + n * LARGHEZZA_FRECCIA * Scala;
        const Vec2 v2 = base - n * LARGHEZZA_FRECCIA * Scala;

        draw->AddTriangleFilled(v0, v1, v2, colore);
    }

    static void DisegnaScritta(
        ImDrawList *const draw,
        const Vec2 &posDiagramma,
        const textlabel_t *const label,
        ImU32 colorePredefinito,
        const pointf *const posizione)
    {
        if (!label || !label->text || label->text[0] == '\0') return;
        if (!posizione && !label->set) return;

        const float altezzaDiagramma = static_cast<float>(GD_bb(currentGraph).UR.y);

        ImFont *const font         = ImGui::GetIO().Fonts->Fonts[0];
        const float dimensioneFont = static_cast<float>(label->fontsize) * Scala;
        const Vec2 dimensioneTesto =
            font->CalcTextSizeA(dimensioneFont, std::numeric_limits<float>::max(), -1.0f, label->text);

        const Colore colore = EstraiColore(label->fontcolor, colorePredefinito);

        Vec2 pos;

        if (label->set) pos = ConvertiPunto(label->pos, altezzaDiagramma, posDiagramma);
        else pos = ConvertiPunto(*posizione, altezzaDiagramma, posDiagramma);

        pos -= dimensioneTesto / 2.0f;

        draw->AddText(font, dimensioneFont, pos, colore.colore, label->text);
    }

    /// @brief Recupera ed estrae un colore in formato Graphviz da una proprietà di un oggetto di Graphviz e lo converte
    /// in formato ImGui.
    /// @param oggetto L'oggetto di Graphviz.
    /// @param nome Il nome della proprietà dell'oggetto.
    /// @param colorePredefinito Il colore in formato ImGui da restituire se l'estrazione fallisce.
    /// @return Il colore estratto con validità a True in caso di successo, il colore predefinito e la validità a False
    ///         in caso di fallimento.
    static Colore EstraiColore(void *oggetto, const char *nome, ImColor colorePredefinito)
    {
        const char *colore = agget(oggetto, const_cast<char *>(nome));
        return EstraiColore(colore, colorePredefinito);
    }

    /// @brief Data una stringa con un colore in formato Graphviz, ne estrae il colore e lo converte in formato ImGui.
    /// @param colore Il colore in formato Graphviz.
    /// @param colorePredefinito Il colore in formato ImGui da restituire se l'estrazione fallisce.
    /// @return Il colore estratto con validità a True in caso di successo, il colore predefinito e la validità a False
    ///         in caso di fallimento.
    static Colore EstraiColore(const char *colore, ImColor colorePredefinito)
    {
        if (!colore || colore[0] == '\0') return { colorePredefinito, false };

        gvcolor_t coloreGV;
        if (colorxlate(colore, &coloreGV, RGBA_BYTE) == COLOR_OK)
            return { IM_COL32(coloreGV.u.rgba[0], coloreGV.u.rgba[1], coloreGV.u.rgba[2], coloreGV.u.rgba[3]), true };
        else return { colorePredefinito, false };
    }

    /// @brief Converte un punto dal sistema di coordinate usato da Graphviz a pixel (sistema di riferimento di ImGui).
    /// @param punto Il punto da convertire in punti per pollice.
    /// @param altezzaDiagramma L'altezza totale del diagramma in punti per pollice.
    /// @param origine L'origine del diagramma in pixel.
    /// @return Le coordinate del punto convertite in pixel e ribaltate sull'asse x (sotto-sopra).
    static Vec2 ConvertiPunto(const Vec2 &punto, const float altezzaDiagramma, const Vec2 &origine)
    {
        // Ribalto il diagramma in orizzontale (intorno all'asse x).
        Vec2 p(punto.x, altezzaDiagramma - punto.y);

        // Conversione da punti tipografici a pixel.
        p *= PIXEL_PER_PUNTO;

        // Fattore di scala del diagramma.
        p *= Scala;

        // Traslazione del punto rispetto alla posizione del diagramma sullo schermo.
        p += origine;

        return p;
    }
}
