#include "EqLin.hpp"

#include <cassert>

EqLin::EqLin(Fonction fn, std::string op, Fonction fn2) : f1(fn), f2(fn2), op(op)
{
    assert(operateurs.find(op) != std::string::npos && "Operateur invalide. Operateurs possible : <= >= et =");
}

void EqLin::add_at(std::size_t pos, double d)
{
    f1.add_at(pos, d);
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

std::string EqLin::to_string() const
{
    return f1.to_string() + " " + op + " " + f2.to_string();
}