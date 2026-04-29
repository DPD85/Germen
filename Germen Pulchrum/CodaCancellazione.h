#pragma once

class CodaCancellazione
{
  public:
    using Pulitore = std::function<void()>;

    ~CodaCancellazione();

    void Aggiungi(const Pulitore &proc);
    void Ripulisci();
    void Svuota();

  private:
    std::stack<Pulitore> coda;
};
