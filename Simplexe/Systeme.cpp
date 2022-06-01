#include "Systeme.hpp"

#include <cassert>
#include <algorithm>

Systeme::Systeme(std::vector<EqLin> sys) : s_system(sys)
{
    assert(!sys.empty() && "Impossible d'avoir un systeme avec aucune equation.");
}

void Systeme::add_equation(EqLin eq)
{
    assert(eq.get_nombre_de_variables() > 0 && "Inutile d'ajouter une equation vide au systeme");
    s_system.push_back(eq);
    en_forme = false;
}

std::size_t Systeme::get_nbre_de_variables() const
{
    std::size_t max{0};
    for (auto eq : s_system)
        max = std::max(eq.get_nombre_de_variables(), max);

    return max;
}

Matrice Systeme::get_matrice()
{
    std::vector<std::vector<double>> m;
    for (auto &eq : s_system)
    {
        eq.get_vecteur_de_fonction().resize(get_nbre_de_variables());
        m.push_back(eq.get_vecteur_de_fonction());
    }

    return Matrice(m);
}

std::vector<double> Systeme::get_second_membre()
{
    if (!en_forme)
        mise_en_forme();
    std::vector<double> vec;
    for (auto eq : s_system)
    {
        vec.push_back(eq.get_seconde_fonction());
    }
    return vec;
}

void Systeme::mise_en_forme()
{
    for (auto &eq : s_system)
    {
        eq.formaliser();
        eq.f2 = eq.f1.get_variable_b() * -1;
        eq.f1 -= Fonction(eq.f1.get_variable_b());
    }
    en_forme = true;
}

Systeme Systeme::version_standard() const
{
    Systeme s(*this);
    if (!s.en_forme)
        s.mise_en_forme();

    std::size_t i{s.get_nbre_de_variables()};
    for (auto &eq : s.s_system)
    {
        if (eq.op != "=")
        {
            int val = eq.op == "<=" ? 1 : -1;
            eq.add_at(i, val);
            eq.set_operateur("=");
            i++;
        }
    }
    for (auto &eq : s.s_system)
        eq.f1.resize(i);

    return s;
}

///////////////////////////////////////////////////////////////////////

std::ostream &operator<<(std::ostream &stream, Systeme const &systeme)
{
    for (auto eq : systeme.s_system)
    {
        stream << "  |\t" << eq << std::endl;
    }
    return stream;
}