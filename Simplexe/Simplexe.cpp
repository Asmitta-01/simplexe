#include "Simplexe.hpp"

#include <algorithm>
#include <cmath>

Simplexe::Simplexe(Fonction const &f, Systeme s)
    : s_fonction(f), s_systeme(s), s_matrice(s.version_standard().get_matrice()), s_second_membre(s.get_second_membre())
{
    assert(!s_fonction.get_coefficients().empty() && !s_matrice.est_vide() && "La fonction ou le systeme de contraintes ne peut etre vide.");
    s_fonction.resize(s_matrice.nb_colonnes());
}

std::vector<double> Simplexe::vecteur_de_cout_reduit() const
{
    Matrice cth = Matrice(get_matrice_hors_base(false));
    Matrice ctb = Matrice(get_matrice_de_base(false));
    Matrice ab = Matrice(get_matrice_de_base());
    Matrice ah = Matrice(get_matrice_hors_base());

    Matrice lth = cth - ctb * ab.inverse() * ah;
    return lth.m_matrice[0];
}

std::vector<double> Simplexe::z_vector(std::size_t xe) const
{
    Matrice a_b(get_matrice_de_base());
    return a_b.inverse() * s_matrice.get_colonne(xe);
}

void Simplexe::set_matrice(Matrice m)
{
    s_matrice = m;
}

void Simplexe::set_second_membre(std::vector<double> vec)
{
    s_second_membre = vec;
}

void Simplexe::set_base(std::vector<::size_t> b)
{
    s_base = b;
    std::cout << "Base de depart: " << s_base + 1 << std::endl;
    update_hors_base();
    update_solution_de_base_realisable();
}

void Simplexe::set_base()
{
    bool set = false;
    std::vector<std::vector<std::size_t>> toutes_les_bases;
    combinaisons_possibles(toutes_les_bases, s_matrice.m_nb_colonnes, s_matrice.m_nb_lignes);
    for (auto une_base : toutes_les_bases)
    {
        Matrice m{s_matrice.get_colonnes(une_base)};
        // if (m.determinant() != 0)
        if (m == m.transposee() && m.determinant() == 1)
        {
            set_base(une_base);
            set = true;
            break;
        }
    }

    assert(set && "Probleme mal defini. Tres mal defini meme ! ");
}

void Simplexe::update_hors_base()
{
    std::vector<std::size_t> vec;
    for (std::size_t i = 0, k{0}; i < s_matrice.nb_colonnes(); i++)
    {
        if (s_base[k] != i)
            vec.push_back(i);
        else
            k++;
    }
    s_hors_base = vec;
}

void Simplexe::update_solution_de_base_realisable()
{
    std::vector<double> temp_vec(s_matrice.nb_colonnes(), 0);
    for (std::size_t i = 0; i < s_base.size(); i++)
        temp_vec[s_base[i]] = get_solution_de_base()[i];

    s_sln_de_base_realisable = temp_vec;
    // for (auto s : s_sln_de_base_realisable)
    // {
    //     std::cout << s << std::endl;
    // }
}

void Simplexe::update_base(std::size_t x_entrant, std::size_t x_sortant)
{
    s_base.erase(std::find(std::begin(s_base), std::end(s_base), x_sortant));
    s_base.push_back(x_entrant);
    std::sort(std::begin(s_base), std::end(s_base));
    update_hors_base();
    update_solution_de_base_realisable();
}

void Simplexe::show_resultat_final()
{
    std::cout << "\n---------------------------\n"
              << "Resultat final\n"
              << "---------------------------\n"
              << "Fonction economique : " << s_fonction << " = " << Matrice(s_fonction.get_coefficients()) * get_solution_de_base_realisable()
              << "Base finale: " << s_base + 1
              << "Solution de base optimale: " << get_solution_de_base_realisable() << std::endl;
}

////////////////////////////////////////////////////////////////////////////

std::vector<std::size_t> Simplexe::get_base() const
{
    return s_base;
}

std::vector<double> Simplexe::get_solution_de_base_realisable() const
{
    assert(!s_sln_de_base_realisable.empty() && "Aucun solution de base n'a ete enregistree");
    return s_sln_de_base_realisable;
}

std::vector<double> Simplexe::get_solution_de_base() const
{
    Matrice ab(get_matrice_de_base());
    return ab.inverse() * s_second_membre;
}

std::vector<double> Simplexe::get_solution_hors_base() const
{
    Matrice M(s_fonction.get_coefficients());
    std::vector<double> l_mhb;
    for (int j = 0; j < M.nb_colonnes(); j++)
    {
        if (s_sln_de_base_realisable[j] == 0)
            l_mhb.push_back(M.get_ligne(0)[j]);
    }
    return l_mhb;
}

std::vector<std::vector<double>> Simplexe::get_matrice_hors_base(bool matrice) const
{
    assert(!s_sln_de_base_realisable.empty() && "Aucune Base n'a ete ajoutee. Impossible d'avoir la base");

    std::vector<std::vector<double>> mhb;
    Matrice ap = matrice ? s_matrice : Matrice(s_fonction.get_coefficients());

    for (int i = 0; i < ap.nb_lignes(); i++)
    {
        std::vector<double> l_mhb;
        for (int j = 0; j < ap.nb_colonnes(); j++)
        {
            if (s_sln_de_base_realisable[j] == 0)
                l_mhb.push_back(ap.get_ligne(i)[j]);
        }
        mhb.push_back(l_mhb);
    }
    return mhb;
}

std::vector<std::vector<double>> Simplexe::get_matrice_de_base(bool matrice) const
{
    assert(!s_base.empty() && "Aucune Base n'a ete ajoutee. Impossible d'avoir la base");
    std::vector<std::vector<double>> mhb;
    Matrice ap = matrice ? s_matrice : Matrice(s_fonction.get_coefficients());
    for (int j = 0; j < s_base.size(); j++)
        mhb.push_back(ap.get_colonne(s_base[j]));

    return Matrice(mhb).transposee().m_matrice;
}

std::size_t Simplexe::get_variable_entrante(bool MAX_FUNCTION) const
{
    if (MAX_FUNCTION)
        return s_hors_base[indice_du_max(vecteur_de_cout_reduit())];
    else
        return s_hors_base[indice_du_min(vecteur_de_cout_reduit())];
}

std::size_t Simplexe::get_variable_sortante(bool MAX_FUNCTION) const
{
    std::size_t index;
    if (MAX_FUNCTION)
    {
        assert(z_vector(get_variable_entrante(MAX_FUNCTION)) >= 0 && "Probleme mal defini. La fonction F n'est pas majoree, l'optimum est infini");
        index = s_base[indice_du_min(get_solution_de_base() / z_vector(get_variable_entrante(MAX_FUNCTION)))];
    }
    else
    {
        assert(z_vector(get_variable_entrante(MAX_FUNCTION)) <= 0 && "Probleme mal defini. La fonction F n'est pas minoree, l'optimum est infini");
        index = s_base[indice_du_max(get_solution_de_base() / z_vector(get_variable_entrante(MAX_FUNCTION)))];
    }
    return index;
}

////////////////////////////////////////////////////////////////////////////

std::size_t indice_du_max(std::vector<double> const &lth)
{
    std::size_t index{0};
    for (std::size_t i = 0; i < lth.size(); i++)
    {
        if (std::isfinite(lth[i]) && lth[index] < lth[i])
            index = i;
    }
    return index;
}
std::size_t indice_du_min(std::vector<double> const &lth)
{
    std::size_t index{0};
    for (std::size_t i = 0; i < lth.size(); i++)
    {
        if (std::isfinite(lth[i]) && lth[index] > lth[i])
            index = i;
    }
    return index; // return index_du_max(-lth);
}

///////////////////////////////////////--------------------------------------

bool operator<=(std::vector<double> const &vec, int n)
{
    for (auto elt : vec)
    {
        if (elt > n)
        {
            return false;
        }
    }
    return true;
}
bool operator>=(std::vector<double> const &vec, int n)
{
    for (auto elt : vec)
    {
        if (elt < n)
        {
            return false;
        }
    }
    return true;
}
bool operator==(std::vector<double> const &vec, int n)
{
    for (auto elt : vec)
    {
        if (elt != n)
        {
            return false;
        }
    }
    return true;
}

std::vector<double> operator/(std::vector<double> const &vecteur_a, std::vector<double> const &vecteur_b)
{
    assert(vecteur_a.size() == vecteur_b.size() && "Division impossible, les vecteurs doivent etre de meme taille");
    std::vector<double> vecteur_c;
    for (int i = 0; i < vecteur_a.size(); i++)
    {
        // assert(vecteur_b[i] != 0 && "Division impossible avec un denominateur nul");
        vecteur_c.push_back(vecteur_a[i] / vecteur_b[i]);
    }
    return vecteur_c;
}

std::ostream &operator<<(std::ostream &stream, Simplexe const &simplexe)
{
    stream << "----|\tSimplexe\n"
           << "-------------------------\n"
           << "----|\tFonction : "
           << simplexe.s_fonction << std::endl
           << "----|\tMatrice associee: \n"
           << simplexe.s_matrice << std::endl
           << "----|\tSysteme : \n"
           << simplexe.s_systeme << std::endl;

    return stream;
}

////////////////////////////////////////////////////

// @PierrotLeFou - OpenClassrooms
// void combinaisons_possibles(std::vector<std::vector<std::size_t>> &combinaisons, int v_max, int taille)
// {
//     std::vector<std::size_t> passe(taille);
//     int i = 0;
//     int v = 0;
//     while (i >= 0)
//     {
//         while (i < taille && v + taille - i <= v_max)
//         {
//             passe[i++] = v++;
//         }
//         if (i == taille)
//             combinaisons.push_back(passe);
//         i--;
//         if (i >= 0)
//             v = passe[i] + 1;
//     }
// }

void combinaisons_possibles(std::vector<std::vector<std::size_t>> &combinaisons, std::size_t const v_max, std::size_t const taille, std::vector<std::size_t> vecteur, std::size_t pos)
{
    assert(v_max > 1 && taille > 0 && "Combinaison, probleme.");
    if (vecteur.size() == 0)
        vecteur.resize(taille);

    // std::vector<std::vector<std::size_t>> combinaisons;
    for (std::size_t i = 0; i < v_max; i++)
    {
        if (pos == 0 || i > vecteur[pos - 1])
        {
            vecteur[pos] = i;
            if (taille == 1)
                combinaisons.push_back(vecteur);
            else
                combinaisons_possibles(combinaisons, v_max, taille - 1, vecteur, pos + 1);
        }
    }

    // return combinaisons;
}

///////////////// TEST FUNCTION ////////////////////////
void checkSubAdvance()
{
    std::cout << "\nReached sub-step " << sub_step << std::endl;
    ++sub_step;
}