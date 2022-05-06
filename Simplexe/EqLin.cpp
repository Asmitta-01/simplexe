#include "EqLin.hpp"

#include <cassert>

EqLin::EqLin(Fonction fn, std::string op, Fonction fn2) : f1(fn), f2(fn2), op(op)
{
    assert(operateurs.find(op) != std::string::npos && "Operateur invalide. Operateurs possible : <= >= et =");
}

EqLin::EqLin(std::string str)
{
    std::vector<std::string> parts = split(str, '=');
    f2 = parts.back();
    op = parts.at(0).back();
    op.push_back('=');
    op = op == " =" ? "=" : op;
    parts.at(0).pop_back();
    f1 = parts.at(0);

    assert(operateurs.find(op) != std::string::npos && "Operateur invalide. Operateurs possible : <= >= et =");
}

void EqLin::add_at(std::size_t pos, double d)
{
    f1.add_at(pos, d);
}

void EqLin::set_operateur(std::string opr)
{
    assert(operateurs.find(opr) != std::string::npos && "Operateur invalide. Operateurs possible : <= >= et =");
    op = opr;
}

void EqLin::supprimer(std::size_t pos)
{
    assert(pos < f1.get_coefficients().size() - 1 && "Invalid offset - out of bounds");
    f1.delete_at(pos);
}

void EqLin::formaliser()
{
    f1 -= f2;
    f2 = 0;
}

std::size_t EqLin::get_nombre_de_variables() const
{
    // formaliser();
    return f1.get_coefficients().size();
}

std::vector<double> EqLin::get_vecteur_de_fonction() const
{
    // formaliser();
    return f1.get_coefficients();
}

double EqLin::get_seconde_fonction() const
{
    // return f2.get_coefficients();
    return f2.get_variable_b();
}

std::string EqLin::to_string() const
{
    return f1.to_string() + " " + op + " " + f2.to_string();
}

//////////////////////////////////////////////////////////////////

std::ostream &operator<<(std::ostream &stream, EqLin const &eq)
{
    stream << eq.to_string();
    return stream;
}