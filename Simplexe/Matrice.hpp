#ifndef MATRICE_HPP
#define MATRICE_HPP

#include <vector>
#include <ostream>
#include <istream>
#include <iostream>

/**
 * @brief Cette classe reprend le fonctionnement d'une matrice
 * avec en plus quelques methods et attributs requis pour la
 * resolution d'un probleme pr la methode du Simplexe
 *
 */
class Matrice
{
private:
    int m_nb_lignes;
    int m_nb_colonnes;
    bool m_matrice_simple; // False signifie que cette matrice n'est pas simple: Elle doit respecter les normes d'ne matrice de Simplexe
    std::vector<std::vector<double>> m_matrice;

public:
    Matrice(int, int, bool m_s = false);
    Matrice(std::vector<std::vector<double>> const &, bool m_s = false);
    Matrice(Matrice const &matrice) = default;
    Matrice &operator=(Matrice const &matrice) = default;

    /**
     * @brief Definit le mode d'affichage, a true l'affichage est sous forme de matrice.
     * A false c'est sous forme de systeme.
     *
     */
    bool modeMatrice = true;

    /**
     * @brief Renvoie un booleen determinant l'etat de la matrice.
     * @b True si la matrice est vide, et @b False sinon
     */
    bool est_vide() const;

    /**
     * @brief Renvoie True si tous les elements de la matrice valent 0.
     * Et @b False sinon
     */
    bool est_nulle() const;

    /**
     * @brief Affiche la matrice sous un forme ou chaque champ a pour valeur
     *  son numero de ligne concatene a son numero de colonnne.
     * @n Par exemple
     * @n la matrice de taille (2, 3) aura pour representation
     *          |   11   12   13   |
     *          |   21   22   23   |
     */
    void representation();

    /**
     * @brief Recois les entrees de l'utilisateur puis les enregistre dans
     * la matrice
     * @deprecated Simple test
     */
    void remplissage();

    std::vector<double> get_ligne(int) const;
    std::vector<double> get_colonne(int) const;
    int nb_lignes() const;
    int nb_colonnes() const;

    /**
     * @brief Supprime la ligne i et la colonne j de la matrice et renvoie la nouvelle matrice obtenue
     *
     * @return Matrice
     */
    Matrice sans_ligne_et_colonne(int i, int j) const;

    /**
     * @brief Calcul le determinant d'une matrice carree
     *
     * @return double
     */
    double determinant() const;

    /**
     * @brief Retourne la transposee de la matrice
     *
     * @return Matrice
     */
    Matrice transposee() const;

    /**
     * @brief Retourne la matrice inverse de celle-ci (this)
     *
     * @return Matrice
     */
    Matrice inverse() const;

    friend Matrice operator+(Matrice const &matrice_a, Matrice const &matrice_b);
    friend Matrice operator-(Matrice const &matrice_a, Matrice const &matrice_b);
    friend Matrice operator*(Matrice const &matrice_a, Matrice const &matrice_b);
    friend std::vector<double> operator*(Matrice const &matrice, std::vector<double> const &vecteur);
    friend std::vector<double> operator*(std::vector<double> const &vecteur, Matrice const &matrice);
    friend Matrice operator*(double const &, Matrice const &);
    friend Matrice operator*(Matrice const &, double const &);
    friend std::ostream &operator<<(std::ostream &stream, Matrice const &matrice);
    friend std::istream &operator>>(std::istream &stream, Matrice &matrice);

    friend class Simplexe;
};

#endif