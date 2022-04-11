#include <iostream>

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
    std::cout << "--------------------------------------\n"
              << "Resolution par la methode du simplexe.\n"
              << "--------------------------------------\n"
              << "1. La fonction\n"
              << "C'est un probleme de : \ta. Maximisation\tb. Minimisation ?\n";
    char t;
    std::cin >> t;
    bool MAXIMISATION{false};
    if (t == 'a')
        MAXIMISATION = true;
    int row, col;
    std::cout << "Nombre de contraintes: ";
    std::cin >> row;
    std::cout << "Nombre de variables: ";
    std::cin >> col;
    Matrice F(1, col);
    std::cout << "\nLa fonction est sous forme de matrice. "
              << "10x1 + 2x2 + 3x4 sous forme matricielle devient: | 10  2  0  3 |\n"
              << "Entrez la fonction suivant cette logique.\n";
    std::cin >> F;
    F.modeMatrice = false;
    std::cout << "Fonction: " << F << std::endl;

    std::cout << "2. La matrice.\n";
    Matrice M(row, col);
    std::cin >> M;
    std::cout << "Matrice: \n"
              << M << std::endl;

    Simplexe S(F, M);

    std::cout << "3. Le second membre de la matrice , ou encore b\n"
              << "L'ensemble des valeurs apres l'egalite dans le systeme\n"
              << "Exemple: 10x1 + 12x2 = b...\n\n"
              << "Veuillez entrer les differents champs de ce 'vecteur':\n";
    S.set_second_membre();
    std::cout << S << std::endl;

    // Phase 2 du simplexe.....................................................
    // ........................................................................
    // ........................................................................
    S.set_base();

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

    return 0;
}
