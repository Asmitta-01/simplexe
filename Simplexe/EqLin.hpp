#ifndef EQLIN_HPP
#define EQLIN_HPP

#include "Fonction.hpp"

std::string const operateurs{"<=>="};
class Systeme;

/**
 * @brief Classe implementant une equation lineaire
 *
 */
class EqLin
{
    friend class Systeme;

private:
    Fonction f1;
    std::string op; // >= , <= , = ; Better use enum ?
    Fonction f2;

public:
    EqLin(Fonction fonction_1 = 0, std::string operateur = "=", Fonction fonction_2 = 0);
    EqLin(std::string);
    EqLin(EqLin const &eqlin) = default;
    EqLin &operator=(EqLin const &eqlin) = default;

    void add_at(std::size_t, double d = 1);
    void set_operateur(std::string);
    void supprimer(std::size_t);
    void formaliser();
    std::size_t get_nombre_de_variables() const;
    std::vector<double> get_vecteur_de_fonction() const;
    double get_seconde_fonction() const;
    std::string to_string() const;

    // friend void Systeme::mise_en_forme();
    friend std::ostream &operator<<(std::ostream &stream, EqLin const &eq);
};

#endif