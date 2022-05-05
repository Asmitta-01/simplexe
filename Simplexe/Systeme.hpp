#ifndef SYSTEME_HPP
#define SYSTEME_HPP

#include "EqLin.hpp"
#include "Matrice.hpp"

class Systeme
{
private:
    std::vector<EqLin> s_system;
    bool en_forme = false;

public:
    Systeme(std::vector<EqLin>);
    Systeme(Systeme const &systeme) = default;

    void add_equation(EqLin);
    Matrice get_matrice();
    std::size_t get_nbre_de_variables() const;
    std::vector<double> get_second_membre();
    void mise_en_forme();
    Systeme version_standard() const;

    friend std::ostream &operator<<(std::ostream &stream, Systeme const &systeme);
};

#endif