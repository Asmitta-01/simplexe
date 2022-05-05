#include "Matrice.hpp"

#include <cassert>
#include <cmath>

Matrice::Matrice(int l, int c)
    : m_nb_lignes(l), m_nb_colonnes(c), m_matrice(l, std::vector<double>(c))
{
    assert(m_nb_colonnes > 0 && m_nb_lignes > 0 && "On ne peut avoir une matrice avec une dimension nulle.");
}

Matrice::Matrice(std::vector<double> const &vec)
    : m_nb_lignes(1), m_nb_colonnes(vec.size()), m_matrice(1, std::vector<double>{vec})
{
    assert(m_nb_colonnes > 0 && m_nb_lignes > 0 && "On ne peut avoir une matrice avec une dimension nulle.");
}

Matrice::Matrice(std::vector<std::vector<double>> const &vec)
    : m_nb_lignes(vec.size()), m_nb_colonnes(vec[0].size()), m_matrice(vec)
{
    assert(m_nb_colonnes > 0 && m_nb_lignes > 0 && "On ne peut avoir une matrice avec une dimension nulle.");
    // assert(m_nb_colonnes >= m_nb_lignes && "Le nombre de contraintes doit etre inferieur au nombre de variables.");
}

///////////////////////////////////////////////////////////////////////////

bool Matrice::est_vide() const
{
    return m_matrice.empty();
}

bool Matrice::est_nulle() const
{
    for (auto ligne : m_matrice)
    {
        for (auto elt : ligne)
            if (elt != 0)
                return false;
    }
    return true;
}

void Matrice::representation()
{
    if (m_nb_colonnes < 25 && m_nb_lignes < 25) // Pas besoin de representation pour les grandes matrices, ca prendra trop d'espace dans le terminal
    {
        std::cout << "Representation de votre matrice: \n  A =";
        for (int i = 0; i < m_nb_lignes; i++)
        {
            std::cout << "\t|  ";
            for (int j = 0; j < m_nb_colonnes; j++)
            {
                std::cout << i + 1 << j + 1 << "  ";
            }
            std::cout << "|\n";
        }
    }
    std::cout << "\n";
}

void Matrice::remplissage()
{
    representation();
    std::cout << "\nVeuillez remplir votre matrice en entrant la valeur correspondante, conformement a la representation ci-dessus." << std::endl;
    double elt;
    for (int i = 0; i < m_nb_lignes; i++)
    {
        for (int j = 0; j < m_nb_colonnes; j++)
        {
            std::cout << "Champ " << i + 1 << j + 1 << " :";
            std::cin >> elt;
            m_matrice[i][j] = elt;
        }
    }
}

//////////////////////////////////////////////////////////////////////////

std::vector<double> Matrice::get_ligne(int indice) const
{
    assert(indice < m_nb_lignes && "Indice superieur au nombre de lignes de la matrice");
    return m_matrice[indice];
}

std::vector<double> Matrice::get_colonne(int indice) const
{
    assert(indice < m_nb_colonnes && "Indice superieur au nombre de colonnes de la matrice");
    std::vector<double> colonne;
    for (int i = 0; i < m_nb_lignes; i++)
    {
        colonne.push_back(m_matrice[i][indice]);
    }
    return colonne;
}

int Matrice::nb_lignes() const
{
    return this->m_nb_lignes;
}
int Matrice::nb_colonnes() const
{
    return this->m_nb_colonnes;
}

Matrice Matrice::get_colonnes(std::vector<std::size_t> base) const
{
    assert(base.size() == m_nb_lignes && "La matrice de base doit etre une matrice carree.");
    std::vector<std::vector<double>> M;
    for (auto i : base)
        M.push_back(get_colonne(i));

    return Matrice(M).transposee();
}

Matrice Matrice::sans_ligne_et_colonne(int ligne, int colonne) const
{
    assert(ligne < m_nb_lignes && colonne < m_nb_colonnes && "Hors des limites de la matrice");
    Matrice new_matrice(*this);
    new_matrice.m_matrice.erase(std::begin(new_matrice.m_matrice) + ligne); // Supprime la ligne "ligne"
    --new_matrice.m_nb_lignes;
    for (int i = 0; i < new_matrice.m_nb_lignes; i++) // supprime la colonne "colonne"
        new_matrice.m_matrice[i].erase(std::begin(new_matrice.m_matrice[i]) + colonne);
    --new_matrice.m_nb_colonnes;

    return new_matrice;
}

double Matrice::determinant() const
{
    // assert(m_nb_lignes > 1 && m_nb_colonnes > 1 && "Impossible de definir le determinant d'un vecteur");
    assert(m_nb_colonnes == m_nb_lignes && "Determinant applicable seulement sur les matrices carrees");
    if (m_nb_colonnes == 1)
        return m_matrice[0][0];
    else if (m_nb_lignes == 2)
        return m_matrice[0][0] * m_matrice[1][1] - m_matrice[0][1] * m_matrice[1][0];
    else
    {
        int i = 0; // premiere ligne
        double result{0};
        for (int j = 0; j < m_nb_colonnes; j++)
        {
            result += m_matrice[i][j] * std::pow(-1, i + j) * this->sans_ligne_et_colonne(i, j).determinant();
        }
        return result;
    }
}

Matrice Matrice::transposee() const
{
    Matrice matrice_t(m_nb_colonnes, m_nb_lignes);
    matrice_t.m_matrice.erase(std::begin(matrice_t.m_matrice), std::end(matrice_t.m_matrice));
    for (int i = 0; i < m_nb_colonnes; i++)
    {
        matrice_t.m_matrice.push_back(get_colonne(i));
    }
    return matrice_t;
}

Matrice Matrice::inverse() const
{
    assert(m_nb_colonnes == m_nb_lignes && "L'inverse de la matrice ne peut etre determinee sur une mtrice non carree");
    assert(determinant() != 0 && "Determinant nul. Cette matrice n'est pas inversible");
    Matrice matrice_i(m_nb_lignes, m_nb_colonnes);
    for (int i = 0; i < m_nb_lignes; i++)
    {
        for (int j = 0; j < m_nb_colonnes; j++)
        {
            matrice_i.m_matrice[i][j] = std::pow(-1, i + j) * this->sans_ligne_et_colonne(i, j).determinant();
        }
    }

    return (1 / determinant()) * matrice_i.transposee();
}

////////////////////////////////////////////////////////////////////////////

Matrice operator+(Matrice const &matrice_a, Matrice const &matrice_b)
{
    assert(
        matrice_a.nb_colonnes() == matrice_b.nb_colonnes() &&
        matrice_a.nb_lignes() == matrice_b.nb_lignes() &&
        "Les deux matrices doivent avoir les meme dimensions");
    Matrice matrice_c(matrice_a.nb_lignes(), matrice_a.nb_colonnes());

    if (matrice_a.est_nulle())
        return matrice_b;
    else if (matrice_b.est_nulle())
        return matrice_a;

    for (int i = 0; i < matrice_c.nb_lignes(); i++)
        for (int j{0}; j < matrice_c.nb_colonnes(); j++)
            matrice_c.m_matrice[i][j] = matrice_a.get_ligne(i)[j] + matrice_b.get_ligne(i)[j];

    return matrice_c;
}

Matrice operator-(Matrice const &matrice_a, Matrice const &matrice_b)
{
    return matrice_a + (matrice_b * -1);
}

Matrice operator*(Matrice const &matrice_a, Matrice const &matrice_b)
{
    assert(matrice_a.nb_colonnes() == matrice_b.nb_lignes() &&
           "Multiplication impossible. Les dimensions correspondent pas.");
    Matrice matrice_c(matrice_a.nb_lignes(), matrice_b.nb_colonnes());
    if (matrice_a.est_nulle() || matrice_b.est_nulle())
        return matrice_c;

    for (int i = 0; i < matrice_c.nb_lignes(); i++)
    {
        for (int j{0}; j < matrice_c.nb_colonnes(); j++)
        {
            for (int k = 0; k < matrice_a.nb_colonnes(); k++)
            {
                matrice_c.m_matrice[i][j] += matrice_a.get_ligne(i)[k] * matrice_b.get_ligne(k)[j];
            }
        }
    }
    return matrice_c;
}

bool operator==(Matrice const &matrice_a, Matrice const &matrice_b)
{
    assert(matrice_a.m_nb_colonnes == matrice_b.m_nb_colonnes &&
           matrice_a.m_nb_lignes == matrice_b.m_nb_lignes &&
           "L'egalite entre matrice se fait entre matrices de meme dimensions");

    for (std::size_t i = 0; i < matrice_a.m_nb_lignes; i++)
    {
        for (std::size_t j = 0; j < matrice_a.m_nb_colonnes; j++)
        {
            if (matrice_a.m_matrice[i][j] != matrice_b.m_matrice[i][j])
                return false;
        }
    }
    return true;
}

std::vector<double>
operator*(Matrice const &matrice, std::vector<double> const &vecteur)
{
    Matrice matrice_v(std::vector<std::vector<double>>(1, vecteur));
    return (matrice * matrice_v.transposee()).transposee().get_ligne(0);
}

std::vector<double> operator*(std::vector<double> const &vecteur, Matrice const &matrice)
{
    return matrice * vecteur;
}

Matrice operator*(Matrice const &matrice, double const &n)
{
    Matrice matrice_resultat(matrice);
    if (matrice.est_nulle() || n == 0)
        return Matrice(matrice.nb_lignes(), matrice.nb_colonnes());

    for (int i = 0; i < matrice_resultat.nb_lignes(); i++)
    {
        for (int j = 0; j < matrice_resultat.nb_colonnes(); j++)
        {
            matrice_resultat.m_matrice[i][j] *= n;
        }
    }
    return matrice_resultat;
}
Matrice operator*(double const &n, Matrice const &matrice)
{
    return matrice * n;
}

std::ostream &operator<<(std::ostream &stream, Matrice const &matrice)
{
    for (int i = 0; i < matrice.nb_lignes(); i++)
    {
        for (double elt : matrice.get_ligne(i))
        {
            stream << elt << "\t";
        }
        stream << "\n";
    }

    return stream;
}
std::istream &operator>>(std::istream &stream, Matrice &matrice)
{
    matrice.remplissage();
    return stream;
}
