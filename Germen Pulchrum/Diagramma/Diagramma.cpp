#include "IntestazionePrecompilata.h"

#include "Diagramma.h"

#include "Vec2.h"

#include <cgraph.h>
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

    static void DisegnaArchi(ImDrawList *const draw, const Vec2 &cursorPos, Agnode_t *const n);
    static void DisegnaPuntaFreccia(ImDrawList *const lista, const Vec2 &apice, const Vec2 &da, ImU32 colore);
    static void DisegnaScritta(
        ImDrawList *const draw,
        const Vec2 &cursorPos,
        const textlabel_t *const label,
        ImU32 colore,
        const pointf *const posizione = nullptr);

    /// @brief Converte un punto dal sistema di coordinate usato da Graphviz a pixel.
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

        // Traslazione del punto rispetto alla posizione del diagramma sullo schermo.
        p += origine;

        return p;
    }

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

        // Da qui in vanti currentGraph contiene il diagramma da disegnare.

        // -----

        ImDrawList *draw     = ImGui::GetWindowDrawList();
        const Vec2 cursorPos = ImGui::GetCursorScreenPos();

        // -----

        const float altezzaDiagramma = static_cast<float>(GD_bb(currentGraph).UR.y);

        // ----- Disegna lo sfondo del diagramma

        {
            // GD_bb(currentGraph) = bounding box del diagramma.
            //   .LL = vertice min della bounding box.
            //   .UR = vertice max della bounding box.

            // Coordinate punto max della bounding box del diagramma.
            Vec2 posizione = Vec2(GD_bb(currentGraph).UR) * PIXEL_PER_PUNTO;

            Vec2 pmin(cursorPos.x, cursorPos.y);
            Vec2 pmax = cursorPos + posizione;
            draw->AddRectFilled(pmin, pmax, IM_COL32(255, 255, 255, 255));
        }

        // ----- Disegna i nodi

        for (Agnode_t *nodo = agfstnode(currentGraph); nodo; nodo = agnxtnode(currentGraph, nodo))
        {
            // Conversione da pollici a punti tipografici
            Vec2 metàDimensione(
                static_cast<float>(ND_width(nodo)) * 0.5f * PUNTI_PER_POLLICE,
                static_cast<float>(ND_height(nodo)) * 0.5f * PUNTI_PER_POLLICE);

            // Coordinate del centro del nodo.
            const pointf &centro = ND_coord(nodo);

            const shape_desc *shape = ND_shape(nodo);
            if (std::strcmp(shape->name, "diamond") == 0)
            {
                const Vec2 cima = ConvertiPunto({ centro.x, centro.y + metàDimensione.y }, altezzaDiagramma, cursorPos);
                const Vec2 fondo =
                    ConvertiPunto({ centro.x, centro.y - metàDimensione.y }, altezzaDiagramma, cursorPos);
                const Vec2 sinistra =
                    ConvertiPunto({ centro.x - metàDimensione.x, centro.y }, altezzaDiagramma, cursorPos);
                const Vec2 destra =
                    ConvertiPunto({ centro.x + metàDimensione.x, centro.y }, altezzaDiagramma, cursorPos);

                draw->AddQuadFilled(cima, destra, fondo, sinistra, IM_COL32(255, 255, 255, 255));
                draw->AddQuad(cima, destra, fondo, sinistra, IM_COL32(0, 0, 0, 255));
            }
            else if (std::strcmp(shape->name, "ellipse") == 0 || std::strcmp(shape->name, "oval") == 0)
            {
                const Vec2 raggio        = metàDimensione * PIXEL_PER_PUNTO;
                const Vec2 centroInPixel = ConvertiPunto(centro, altezzaDiagramma, cursorPos);

                draw->AddEllipseFilled(centroInPixel, raggio, IM_COL32(255, 255, 255, 255));
                draw->AddEllipse(centroInPixel, raggio, IM_COL32(0, 0, 0, 255));
            }
            else if (std::strcmp(shape->name, "circle") == 0)
            {
                // Graphviz garantisce larghezza == altezza per circle.
                const float raggio       = metàDimensione.x * PIXEL_PER_PUNTO;
                const Vec2 centroInPixel = ConvertiPunto(centro, altezzaDiagramma, cursorPos);

                draw->AddCircleFilled(centroInPixel, raggio, IM_COL32(255, 255, 255, 255));
                draw->AddCircle(centroInPixel, raggio, IM_COL32(0, 0, 0, 255));
            }
            // Default è box, rect oppure rectangle.
            else
            {
                const Vec2 min = ConvertiPunto(centro - metàDimensione, altezzaDiagramma, cursorPos);
                const Vec2 max = ConvertiPunto(centro + metàDimensione, altezzaDiagramma, cursorPos);

                draw->AddRectFilled(min, max, IM_COL32(255, 255, 255, 255));
                draw->AddRect(min, max, IM_COL32(0, 0, 0, 255));
            }

            // ----- Disegno etichette

            const textlabel_t *const label = ND_label(nodo);
            DisegnaScritta(draw, cursorPos, label, IM_COL32(0, 0, 0, 255), &centro);

            // -----

            DisegnaArchi(draw, cursorPos, nodo);
        }
    }

    static void Diagramma::DisegnaArchi(ImDrawList *const draw, const Vec2 &cursorPos, Agnode_t *const nodo)
    {
        const float altezzaDiagramma = static_cast<float>(GD_bb(currentGraph).UR.y);

        for (Agedge_t *arco = agfstout(currentGraph, nodo); arco; arco = agnxtout(currentGraph, arco))
        {
            const splines *spline = ED_spl(arco);
            if (!spline) continue;

            for (size_t i = 0; i < spline->size; ++i)
            {
                const bezier &bezier = spline->list[i];

                // I punti di controllo arrivano in gruppi di 3 (segmenti cubici),
                // preceduti da un punto iniziale: layout è [p0, c1, c2, p1, c1, c2, p1, ...]
                // bezier.list[0] è il punto di partenza della prima bezier.
                for (size_t j = 0; j + 3 < bezier.size; j += 3)
                {
                    const Vec2 p0 = ConvertiPunto(bezier.list[j + 0], altezzaDiagramma, cursorPos);
                    const Vec2 c1 = ConvertiPunto(bezier.list[j + 1], altezzaDiagramma, cursorPos);
                    const Vec2 c2 = ConvertiPunto(bezier.list[j + 2], altezzaDiagramma, cursorPos);
                    const Vec2 p1 = ConvertiPunto(bezier.list[j + 3], altezzaDiagramma, cursorPos);

                    draw->AddBezierCubic(p0, c1, c2, p1, IM_COL32(0, 0, 0, 255), 1.0f);
                }

                // Punta della freccia all'inizio dell'arco.
                if (bezier.sflag)
                {
                    const Vec2 apice = ConvertiPunto(bezier.sp, altezzaDiagramma, cursorPos);
                    const Vec2 da    = ConvertiPunto(bezier.list[1], altezzaDiagramma, cursorPos);
                    DisegnaPuntaFreccia(draw, apice, da, IM_COL32(0, 0, 0, 255));
                }

                // Punta della freccia alla fine dell'arco.
                if (bezier.eflag)
                {
                    const Vec2 apice = ConvertiPunto(bezier.ep, altezzaDiagramma, cursorPos);
                    const Vec2 da    = ConvertiPunto(bezier.list[bezier.size - 1], altezzaDiagramma, cursorPos);
                    DisegnaPuntaFreccia(draw, apice, da, IM_COL32(0, 0, 0, 255));
                }
            }

            // ----- Disegno etichetta

            const textlabel_t *const label = ED_label(arco);
            DisegnaScritta(draw, cursorPos, label, IM_COL32(0, 0, 0, 255));
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
        const Vec2 base = apice - direzione * LUNGHEZZA_FRECCIA;

        const Vec2 v0 = apice;
        const Vec2 v1 = base + n * LARGHEZZA_FRECCIA;
        const Vec2 v2 = base - n * LARGHEZZA_FRECCIA;

        draw->AddTriangleFilled(v0, v1, v2, colore);
    }

    static void DisegnaScritta(
        ImDrawList *const draw,
        const Vec2 &cursorPos,
        const textlabel_t *const label,
        ImU32 colore,
        const pointf *const posizione)
    {
        if (!label || !label->text || label->text[0] == '\0') return;
        if (!posizione && !label->set) return;

        const float altezzaDiagramma = static_cast<float>(GD_bb(currentGraph).UR.y);
        const Vec2 dimensioneTesto   = ImGui::CalcTextSize(label->text);

        Vec2 pos;

        if (label->set) pos = ConvertiPunto(label->pos, altezzaDiagramma, cursorPos);
        else pos = ConvertiPunto(*posizione, altezzaDiagramma, cursorPos);

        pos -= dimensioneTesto / 2.0f;

        draw->AddText(pos, colore, label->text);
    }
}
