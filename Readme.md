# Germen Pulchrum

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++20](https://img.shields.io/badge/C++-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux-lightgrey.svg)]()
[![Vulkan](https://img.shields.io/badge/Vulkan-1.1-red.svg)](https://www.vulkan.org/)
[![ImGui](https://img.shields.io/badge/ImGui-docking-orange.svg)](https://github.com/ocornut/imgui)
[![SDL3](https://img.shields.io/badge/SDL-3-green.svg)](https://www.libsdl.org/)
[![Boost](https://img.shields.io/badge/Boost-Locale-purple.svg)](https://www.boost.org/)

[🇮🇹 Italiano](README.md) | [🇬🇧 English](README.en.md)

## Italiano

**Germen Pulchrum** è un template di alta qualità, pronto per la produzione, per costruire applicazioni GUI
cross-platform con Dear ImGui e Vulkan. Progettato come punto di partenza solido per sviluppatori di qualsiasi livello
di esperienza, combina eccellenza tecnica e attenzione all'esperienza utente.

### Caratteristiche principali

**🌍 Internazionalizzazione completa**
Costruito da zero con codifica UTF-8 e integrazione Boost.Locale. Include traduzioni per inglese (UK) e italiano, più
font accuratamente selezionati: IBM Plex Sans per testo nitido e scalabile in molte lingue, Noto Color Emoji per il
supporto emoji a colori e BabelStone per le icone delle bandiere nazionali.

**🪟 Docking flessibile delle finestre**
Il sistema di docking di ImGui permette di creare layout professionali e personalizzabili. Attacca le finestre tra loro
o ai bordi dello schermo, proprio come Visual Studio o altri IDE professionali. I layout vengono salvati automaticamente
tra una sessione e l'altra.

**📐 Ridimensionamento DPI**
Il ridimensionamento automatico si adatta ai DPI del tuo schermo sia su Windows che su Linux, garantendo rendering
nitido su qualsiasi monitor. Override manuale disponibile nelle impostazioni per un controllo preciso.

**⚡ Rendering ottimizzato**
Rendering limitato dal VSync (massimo 60 Hz) mantiene basso l'uso delle risorse e prolunga la durata della batteria dei
laptop senza sacrificare la reattività.

**🖥️ Multi-piattaforma**
Funziona su Windows 10/11 e Debian 13 (e probabilmente altre distribuzioni Linux moderne). Si compila con
Visual Studio 2022 o GCC 12/13/14.

**📦 Deploy a file singolo**
Tutte le risorse—font, traduzioni e asset—sono incorporate nell'eseguibile. Un file, zero dipendenze, massima portabilità.

Il template include anche supporto per i temi, fornendoti una base completa per costruire applicazioni desktop raffinate.

---

## Requisiti per la compilazione

### Windows

* Windows 10 o Windows 11 a 64 bit;
* Visual Studio 2022 con tool-set v143;
* CMake 4.x;
* Conan 2.x;
* SDK Vulkan 1.4.309.0:
    * nota: il programma utilizza le Vulkan 1.1 quindi probabilmente anche una versione del SDK più vecchia va bene).

### Linux

* Una distribuzione linux a 64 bit (verificato il funzionamento su debian);
* CMake 4.x;
* Conan 2.x;
* GCC 12.x, 13.x o 14.x;
* Ninja build system se si usa GCC 12 o GCC 13;
* make se si usa GCC 14;
* SDK Vulkan 1.4.309.0 (libvulkan-dev e vulkan-utility-libraries-dev):
    * nota: il programma utilizza le Vulkan 1.1 quindi probabilmente anche una versione del SDK più vecchia va bene).

NOTA: Conan potrebbe installare dei pacchetti tramite il gestore dei pacchetti di sistema al fine di compilare alcune
delle dipendenze, in tal caso Conan utilizzerà sudo per eseguire il gestore dei pacchetti e di conseguenza potrebbe
esser necessario digitare la password sul terminale.

## Test
Sono stati svolti dei test di funzionamento del programma nelle seguenti condizioni.

### Windows

* Windows 11;
* NVidia RTX A1000 6GB laptop GPU;
* Intel® Arc™ & Iris® Xe Graphics laptop GPU;

### Linux

* Debian 13 (trixie) tramite WSL 2 su Windows 11;
* X11 tramite Xlib;
* llvmpipe (LLVM 19.1.7, 256 bits) software driver;
