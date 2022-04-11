#ifndef SIMPLEXE_HPP
#define SIMPLEXE_HPP

#include "Matrice.hpp"

#include <cassert>

class Simplexe
{
private:
    Matrice s_fonction;
    Matrice s_matrice;
    std::vector<double> s_second_membre;
    std::vector<std::size_t> s_base;
    std::vector<std::size_t> s_hors_base;
    std::vector<double> s_sln_de_base_realisable;

public:
    Simplexe(Matrice const &fonction, Matrice const &matrice);
    std::vector<double> vecteur_de_cout_reduit() const;
    std::vector<double> z_vector(std::size_t) const;

    void set_matrice(Matrice);
    void set_second_membre(std::vector<double>);
    void set_second_membre();
    void set_base(std::vector<std::size_t>);
    void set_base();
    void update_hors_base();
    void update_solution_de_base_realisable();
    void update_base(std::size_t, std::size_t);
    void show_resultat_final();

    std::vector<std::size_t> get_base() const;
    std::vector<double> get_solution_de_base_realisable() const;
    std::vector<double> get_solution_hors_base() const;
    std::vector<double> get_solution_de_base() const;
    std::vector<std::vector<double>> get_matrice_hors_base(bool matrice = true) const; // Avec true on aura la matrice hors-base de la matrice elle-mm, avec false on aura la partie hors-base de la fontion objective
    std::vector<std::vector<double>> get_matrice_de_base(bool matrice = true) const;
    std::size_t get_variable_entrante(bool) const;
    std::size_t get_variable_sortante(bool) const;

    friend std::ostream &operator<<(std::ostream &stream, Simplexe const &simplexe);
};

std::size_t indice_du_max(std::vector<double> const &lth);
std::size_t indice_du_min(std::vector<double> const &lth);

bool operator<=(std::vector<double> const &vec, int n);
bool operator>=(std::vector<double> const &vec, int n);
bool operator==(std::vector<double> const &vec, int n);
std::ostream &operator<<(std::ostream &stream, std::vector<double> const &vec);
std::ostream &operator<<(std::ostream &stream, std::vector<std::size_t> const &vec);
std::vector<double> operator/(std::vector<double> const &vecteur_a, std::vector<double> const &vecteur_b);

/**
 * TEST FUNCTION......
 */
inline int sub_step{0};
void checkSubAdvance();

#endif