#ifndef FONCTION_HPP
#define FONCTION_HPP

#include <iostream>
#include <vector>
#include <sstream>
#include <string>

/**
 * @brief Cette classe implemente une partie du fonctionnement des fonctions reelles a variables reelles
 *
 */
class Fonction
{
private:
    std::vector<double> f_coefficients;
    double f_variable_b = 0;   // Le  'b' dans ax + b, a1x1 + a2x2 + ... + anxn + b
    char f_ch_variable = '\0'; // Caractere, lettre de la variable: x, y, t ... ou autre

public:
    Fonction();
    Fonction(std::vector<double>);
    Fonction(std::string);
    Fonction(double);
    Fonction &operator=(Fonction const &fonction) = default;

    void add_at(std::size_t pos, double valeur); // pos est l'indice, cet indice commencant a 0
    void delete_at(std::size_t pos);
    void resize(std::size_t); // augmente la taille de la fonction et mets a zero les nouveaux champs
    double get_variable_b() const;
    std::vector<double> get_coefficients() const;
    std::size_t get_index_premier_element() const;
    std::string to_string() const;

    friend Fonction operator+(Fonction const &fonction_a, Fonction const &fonction_b);
    friend Fonction operator-(Fonction const &fonction_a, Fonction const &fonction_b);
    friend void operator+=(Fonction &fonction_a, Fonction const &fonction_b);
    friend void operator-=(Fonction &fonction_a, Fonction const &fonction_b);
    friend Fonction operator*(Fonction const &fonction_a, double n);
    friend Fonction operator*(double n, Fonction const &fonction_a);
};

std::ostream &operator<<(std::ostream &stream, Fonction const &fn);
std::vector<double> operator+(std::vector<double> vec_a, std::vector<double> vec_b);
std::vector<double> operator-(std::vector<double> vec_a, std::vector<double> vec_b);
std::vector<double> operator*(std::vector<double> const &vecteur, double n);
std::vector<double> operator*(double n, std::vector<double> const &vecteur);

/////////////////////////////////////////////////////////////////////

std::string dtos(double n); // Double to string
std::vector<std::string> split(std::string const &, char delimiter = ' ');

#endif