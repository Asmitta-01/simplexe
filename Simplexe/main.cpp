#include <iostream>
#include <cstdlib>

#include "Simplexe.hpp"

int step{0};
/**
 * @brief These functions are for a personal debugging
 */
void checkAdvance()
{
    std::cout << "\nReached step " << step << std::endl;
    ++step;
}

int main()
{
    std::cout << "-----------------------------------------\n"
              << "| Resolution par la methode du simplexe |\n"
              << "-----------------------------------------\n"
              << "C'est un probleme de : \n\ta. Maximisation\tb. Minimisation ?\n";
    char t;
    std::cin >> t;
    bool MAXIMISATION{false};
    if (t == 'a')
        MAXIMISATION = true;
    std::cout << "----|\t La fonction\n"
              << "Entrez clairement votre fonction. Exemple: a1.x1 + a2.x2 + ... ; 2.x1 - 3.x3.\n";
    std::string fn;
    std::getline(std::cin >> std::ws, fn);
    Fonction F(fn);
    std::cout << "Fonction: " << F << std::endl;

    std::cout << "----|\t Les contraintes\n"
              << "Entrez successivement les contraintes du probleme et rien du tout pour terminer la saisie.\n"
              << "Exemple: a1.x1 + a2.x2 <= b." << std::endl;
    std::vector<EqLin> eqs;
    while (std::getline(std::cin, fn) && !fn.empty())
    {
        eqs.push_back(EqLin(fn));
    }
    Systeme sys(eqs);
    std::cout << "Systeme: \n"
              << sys << std::endl;

    Simplexe S(F, sys);
    std::cout << S << std::endl;

    S.set_base();

    // .................................Phase 2 du simplexe....................................... //
    bool OPTIMAL{false};
    do
    {
        if (MAXIMISATION ? S.vecteur_de_cout_reduit() <= 0 : S.vecteur_de_cout_reduit() >= 0)
        {
            OPTIMAL = true;
        }
        else
        {
            std::size_t variable_entrante, variable_sortante;
            variable_entrante = S.get_variable_entrante(MAXIMISATION);
            variable_sortante = S.get_variable_sortante(MAXIMISATION);
            std::cout << "\nVariable entrante: " << variable_entrante + 1 << "\nVariable sortante: " << variable_sortante + 1 << std::endl;

            S.update_base(variable_entrante, variable_sortante);
        }
    } while (!OPTIMAL);
    S.show_resultat_final();

    getchar();
    return 0;
}
