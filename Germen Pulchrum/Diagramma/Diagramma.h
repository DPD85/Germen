#pragma once

namespace Diagramma
{
    extern float ScalaDiagramma;

    bool Inizializza();
    void Rilascia();
    void Disegna(const std::string &codice);
}
