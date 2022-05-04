#ifndef EQLIN_HPP
#define EQLIN_HPP

#include "Fonction.hpp"

std::string const operateurs{"<=>"};

/**
 * @brief Classe implementant une equation lineaire
 *
 */
class EqLin
{

private:
    Fonction f1;
    std::string op; // >= , <= , = ; Better use enum ?
    Fonction f2;

public:
    EqLin(Fonction fonction_1 = 0, std::string operateur = "=", Fonction fonction_2 = 0);
    EqLin(EqLin const &eqlin) = default;
    EqLin &operator=(EqLin const &eqlin) = default;

    void add_at(std::size_t, double d = 1);
    void supprimer(std::size_t);
    void formaliser();
    std::string to_string() const;
};

#endif