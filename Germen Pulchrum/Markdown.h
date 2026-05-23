#pragma once

class Markdown: imgui_md
{
  public:
    /// @brief Disegna del testo interpretandolo come Markdown.
    /// @param testo Il testo da disegnare.
    void Disegna(const std::string &testo);

    /// @brief Disegna del testo interpretandolo come Markdown.
    /// @param inizioTesto Puntatore alla stringa o all'inizio del testo da disegnare.
    /// @param fineTesto Puntatore alla fine del testo da disegnare, se nullptr il testo verrà processato fino al
    ///                  terminatore di stringa.
    void Disegna(const char *inizioTesto, const char *fineTesto = nullptr);

  protected:
    void get_font(font_info &info) const override;
    void open_url() const override;
};
