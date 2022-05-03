#include "Fonction.hpp"

#include <utility>
#include <cassert>
#include <cctype>
#include <cstring>
#include <sstream>
#include <typeinfo>

Fonction::Fonction() : f_ch_variable('\0'), f_coefficients(std::vector<double>())
{
}

Fonction::Fonction(std::vector<double> coefs) : f_coefficients(coefs)
{
}

Fonction::Fonction(std::string fn)
{
    std::string operators("+-"); // + - : /
    std::vector<std::string> parties = split(fn);
    int signe = 1;
    for (auto partie : parties)
    {
        std::vector<std::string> spl_partie = split(partie, '.');
        assert(operators.find(partie) != std::string::npos ||
               std::isdigit(static_cast<unsigned char>(spl_partie.back().back())) &&
                   "String has bad format");
        if (operators.find(partie) != std::string::npos)
            signe = partie == "-" ? -1 : 1;
        else
        {
            try
            {
                double num = std::stod(spl_partie.back());
                f_variable_b = num * signe;
            }
            catch (const std::invalid_argument &e)
            {
                f_ch_variable = spl_partie.back().at(0);
                int pos = std::stoi(std::string(spl_partie.back().begin() + 1, spl_partie.back().end()));
                f_coefficients.resize(pos);
                f_coefficients.at(pos - 1) += spl_partie[0] == partie ? 1 * signe : std::stod(spl_partie[0]) * signe;
            }
            signe = 1;
        }
    }
}

Fonction::Fonction(double d) : f_variable_b(d) {}

void Fonction::add_at(std::size_t pos, double valeur)
{
    if (f_coefficients.size() < pos + 1)
        f_coefficients.resize(pos + 1);
    f_coefficients.at(pos) += valeur;
}

std::vector<double> Fonction::get_coefficients() const
{
    return f_coefficients;
}

std::size_t Fonction::get_index_premier_element() const
{
    for (std::size_t i = 0; i < f_coefficients.size(); i++)
        if (f_coefficients[i] != 0)
            return i;
    return 0;
}

std::string Fonction::to_string() const
{
    std::string str, signe;
    for (std::size_t i = 0; i < f_coefficients.size(); i++)
    {
        if (f_coefficients.at(i) != 0)
        {
            signe = i == get_index_premier_element()
                        ? (f_coefficients.at(i) < 0 ? "-" : "")
                        : (f_coefficients.at(i) < 0 ? " - " : " + ");
            str.append(signe);
            if (std::abs(f_coefficients.at(i)) == 1)
                str.append(f_ch_variable + dtos(i + 1));
            else if (f_coefficients.at(i) != 0)
                str.append(dtos(std::abs(f_coefficients[i])) + (f_ch_variable) + dtos(i + 1));
        }
    }
    if (f_variable_b != 0)
        signe = f_variable_b < 0 ? " - " : " + ";
    return f_variable_b != 0 ? str + signe + dtos(std::abs(f_variable_b)) : str;
}

/////////////////////////////////////////////////////////////////////////

Fonction operator+(Fonction const &fonction_a, Fonction const &fonction_b)
{
    Fonction f;
    f.f_coefficients = fonction_a.f_coefficients + fonction_b.f_coefficients;
    f.f_ch_variable = fonction_a.f_ch_variable == '\0' ? fonction_b.f_ch_variable : fonction_a.f_ch_variable;
    f.f_variable_b = fonction_a.f_variable_b + fonction_b.f_variable_b;

    return f;
}
Fonction operator-(Fonction const &fonction_a, Fonction const &fonction_b) { return fonction_a + (fonction_b * -1); }

std::ostream &operator<<(std::ostream &stream, Fonction const &fn)
{
    stream << fn.to_string();
    return stream;
}

std::vector<double> operator+(std::vector<double> vec_a, std::vector<double> vec_b)
{
    std::size_t max_size = __max(vec_a.size(), vec_b.size());
    std::vector<double> result(max_size);
    vec_b.resize(max_size);
    vec_a.resize(max_size);
    for (std::size_t i = 0; i < max_size; i++)
        result[i] = vec_a[i] + vec_b[i];

    return result;
}
std::vector<double> operator-(std::vector<double> vec_a, std::vector<double> vec_b)
{
    return vec_a + (vec_b * -1);
}

void operator+=(Fonction &fonction_a, Fonction const &fonction_b) { fonction_a = fonction_a + fonction_b; }
void operator-=(Fonction &fonction_a, Fonction const &fonction_b) { fonction_a = fonction_a - fonction_b; }

std::vector<double> operator*(std::vector<double> const &vecteur, double n)
{
    std::vector<double> v;
    for (auto elt : vecteur)
        v.push_back(elt * n);
    return v;
}
std::vector<double> operator*(double n, std::vector<double> const &vecteur) { return vecteur * n; }

Fonction operator*(Fonction const &fonction_a, double n)
{
    Fonction f;
    f.f_coefficients = fonction_a.f_coefficients * n;
    f.f_variable_b = fonction_a.f_variable_b * n;

    return f;
}
Fonction operator*(double n, Fonction const &fonction_a) { return fonction_a * n; }

/////////////////////////////////////////////////////////////////////////

std::string dtos(double n)
{
    std::stringstream ss;
    ss << n;
    return ss.str();
}

std::vector<std::string> split(std::string const &str_to_split, char delimiter)
{
    std::vector<std::string> strings;
    std::istringstream stream(str_to_split);
    std::string str;
    while (getline(stream, str, delimiter))
        strings.push_back(str);

    return strings;
}
