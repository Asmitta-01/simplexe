#ifndef SIMPLEXE_HPP
#define SIMPLEXE_HPP

#include "Systeme.hpp"

#include <cassert>

class Simplexe
{
private:
    Fonction s_fonction;
    Systeme s_systeme;
    Matrice s_matrice;
    std::vector<double> s_second_membre;
    std::vector<std::size_t> s_base;
    std::vector<std::size_t> s_hors_base;
    std::vector<double> s_sln_de_base_realisable;

public:
    Simplexe(Fonction const &fonction, Systeme systeme);
    Simplexe &operator=(Simplexe const &) = default;
    std::vector<double> vecteur_de_cout_reduit() const;
    std::vector<double> z_vector(std::size_t) const;

    void set_matrice(Matrice);
    void set_second_membre(std::vector<double>);
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
std::vector<double> operator/(std::vector<double> const &vecteur_a, std::vector<double> const &vecteur_b);

template <typename T>
std::ostream &operator<<(std::ostream &stream, std::vector<T> const &vec)
{
    stream << "{ ";
    for (auto elt : vec)
    {
        std::cout << elt;
        if (elt == vec.back())
            std::cout << " }" << std::endl;
        else
            std::cout << ", ";
    }
    if (vec.empty())
        stream << "}";

    return stream;
}

/**
 * @brief Determine les combinaisons possibles de taille taille, pour les nombres de 0 a v_max, sans repetitions
 *
 * @param combinaisons Le vecteur (std::vector<std::vector<>>) qui devra contenir ces combinaisons.
 * @param v_max valeur maximale d'un chiffre
 * @param taille taille de la concatenation de ces chiffres
 * @return void
 */
void combinaisons_possibles(std::vector<std::vector<std::size_t>> &combinaisons, std::size_t const v_max, std::size_t const taille, std::vector<std::size_t> vecteur = std::vector<std::size_t>(), std::size_t pos = 0);
// void combinaisons_possibles(std::vector<std::vector<std::size_t>> &combinaisons, int v_max, int taille)  @PierrotLeFou -OpenClassrooms

/**
 * TEST FUNCTION......
 */
inline int sub_step{0};
void checkSubAdvance();

#endif