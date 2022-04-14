#include "Simplexe.hpp"

#include <algorithm>
#include <cmath>

Simplexe::Simplexe(Matrice const &f, Matrice const &m)
    : s_fonction(f), s_matrice(m)
{
    assert(!s_fonction.est_vide() && !s_matrice.est_vide() && "La fonction ou la matrice ne peut etre vide.");
    assert(s_matrice.m_matrice_simple == false && "Le simplexe doit recevoir une matrice respectant les restrictions.");
}

std::vector<double> Simplexe::vecteur_de_cout_reduit() const
{
    Matrice cth = Matrice(get_matrice_hors_base(false), true);
    Matrice ctb = Matrice(get_matrice_de_base(false), true);
    Matrice ab = Matrice(get_matrice_de_base());
    Matrice ah = Matrice(get_matrice_hors_base(), true);

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

void Simplexe::set_second_membre()
{
    double x;
    std::vector<double> vec;
    for (int i = 0; i < s_matrice.nb_lignes(); i++)
    {
        std::cout << "Champ " << i << " : ";
        std::cin >> x;
        vec.push_back(x);
    }
    s_second_membre = vec;
}

void Simplexe::set_base(std::vector<::size_t> b)
{
    s_base = b;
    update_hors_base();
    update_solution_de_base_realisable();
}

void Simplexe::set_base()
{
    std::size_t x;
    std::vector<std::size_t> vec;
    for (int i = 0; i < s_matrice.nb_lignes(); i++)
    {
        std::cout << "Champ " << i << " : ";
        std::cin >> x;
        vec.push_back(x);
    }
    set_base(vec);
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
    std::vector<double> temp_vec(s_matrice.m_nb_colonnes, 0);
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
    s_fonction.modeMatrice = false;
    std::cout << "\n---------------------------\n"
              << "Resultat final\n"
              << "---------------------------\n"
              << "Fonction economique : " << s_fonction << " = " << s_fonction * get_solution_de_base_realisable()
              << "Base finale: " << s_base
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
    std::vector<double> l_mhb;
    for (int j = 0; j < s_fonction.nb_colonnes(); j++)
    {
        if (s_sln_de_base_realisable[j] == 0)
            l_mhb.push_back(s_fonction.get_ligne(0)[j]);
    }
    return l_mhb;
}

std::vector<std::vector<double>> Simplexe::get_matrice_hors_base(bool matrice) const
{
    assert(!s_sln_de_base_realisable.empty() && "Aucune Base n'a ete ajoutee. Impossible d'avoir la base");

    std::vector<std::vector<double>> mhb;
    Matrice ap = matrice ? s_matrice : s_fonction;

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
    Matrice ap = matrice ? s_matrice : s_fonction;
    for (int j = 0; j < s_base.size(); j++)
        mhb.push_back(ap.get_colonne(s_base[j]));

    return Matrice(mhb, true).transposee().m_matrice;
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
    stream << "Simplexe\n"
           << "-------------------------\n"
           << "Fonction : "
           << simplexe.s_fonction << std::endl
           << "Matrice associee: \n"
           << simplexe.s_matrice << std::endl
           << "Systeme final : \n"
           << "---Pas encore dispo---" << std::endl;

    return stream;
}



/////////////////TEST FUNCTION
void checkSubAdvance()
{
    std::cout << "\nReached sub-step " << sub_step << std::endl;
    ++sub_step;
}