#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <map>

enum OrderType { LEX, PERMUTED_LEX, GRADED_LEX };

struct MonomialOrder {
    OrderType type;
    std::vector<int> permutation; 
};

struct Monomial {
    std::vector<int> exponents;

    int totalDegree() const {
        int sum = 0;
        for (int e : exponents) sum += e;
        return sum;
    }

    bool lessThan(const Monomial& other, const MonomialOrder& order) const {
        size_t n = std::max(exponents.size(), other.exponents.size());
        
        auto getExp = [](const Monomial& m, size_t idx) {
            return idx < m.exponents.size() ? m.exponents[idx] : 0;
        };

        if (order.type == GRADED_LEX) {
            int deg1 = this->totalDegree();
            int deg2 = other.totalDegree();
            if (deg1 != deg2) {
                return deg1 < deg2; 
            }
        }

        if (order.type == PERMUTED_LEX) {
            for (int varIdx : order.permutation) {
                int e1 = getExp(*this, varIdx);
                int e2 = getExp(other, varIdx);
                if (e1 != e2) return e1 < e2;
            }
            return false;
        } else { 
            for (size_t i = 0; i < n; ++i) {
                int e1 = getExp(*this, i);
                int e2 = getExp(other, i);
                if (e1 != e2) return e1 < e2;
            }
            return false;
        }
    }
};

struct Term {
    double coeff;
    Monomial mono;
};


class MultivariatePolynomial {
public:
    std::vector<Term> terms;

    void SIMPLIFY() {
        std::vector<Term> cleanTerms;
        for (const auto& t : terms) {
            if (std::abs(t.coeff) < 1e-12) continue;
            
            bool found = false;
            for (auto& ct : cleanTerms) {
                if (ct.mono.exponents == t.mono.exponents) {
                    ct.coeff += t.coeff;
                    found = true;
                    break;
                }
            }
            if (!found) {
                cleanTerms.push_back(t);
            }
        }
        
        terms.clear();
        for (const auto& t : cleanTerms) {
            if (std::abs(t.coeff) >= 1e-12) {
                terms.push_back(t);
            }
        }
    }

    void sortTerms(const MonomialOrder& order) {
        SIMPLIFY();
        std::sort(terms.begin(), terms.end(), [&order](const Term& a, const Term& b) {
            return b.mono.lessThan(a.mono, order); 
        });
    }

    bool isZero() const {
        return terms.empty();
    }

    Term LT() const {
        if (isZero()) return {0.0, {{}}};
        return terms.front();
    }
    
    MultivariatePolynomial operator+(const MultivariatePolynomial& other) const {
        MultivariatePolynomial res = *this;
        res.terms.insert(res.terms.end(), other.terms.begin(), other.terms.end());
        res.SIMPLIFY();
        return res;
    }

    
    MultivariatePolynomial operator-(const MultivariatePolynomial& other) const {
        MultivariatePolynomial res = *this;
        for (const auto& t : other.terms) {
            res.terms.push_back({-t.coeff, t.mono});
        }
        res.SIMPLIFY();
        return res;
    }


    MultivariatePolynomial multiplyByTerm(const Term& t) const {
        MultivariatePolynomial res;
        for (const auto& current : this->terms) {
            Term newTerm;
            newTerm.coeff = current.coeff * t.coeff;
            
            size_t maxVars = std::max(current.mono.exponents.size(), t.mono.exponents.size());
            newTerm.mono.exponents.resize(maxVars, 0);
            for (size_t i = 0; i < maxVars; ++i) {
                int e1 = i < current.mono.exponents.size() ? current.mono.exponents[i] : 0;
                int e2 = i < t.mono.exponents.size() ? t.mono.exponents[i] : 0;
                newTerm.mono.exponents[i] = e1 + e2;
            }
            res.terms.push_back(newTerm);
        }
        res.SIMPLIFY();
        return res;
    }
};

bool divideTerms(const Term& dividend, const Term& divisor, Term& quotient) {
    size_t maxVars = std::max(dividend.mono.exponents.size(), divisor.mono.exponents.size());
    quotient.mono.exponents.resize(maxVars, 0);
    
    for (size_t i = 0; i < maxVars; ++i) {
        int e_div = i < dividend.mono.exponents.size() ? dividend.mono.exponents[i] : 0;
        int e_sor = i < divisor.mono.exponents.size() ? divisor.mono.exponents[i] : 0;
        
        if (e_div < e_sor) return false; 
        quotient.mono.exponents[i] = e_div - e_sor;
    }
    
    quotient.coeff = dividend.coeff / divisor.coeff;
    return true;
}

std::pair<std::vector<MultivariatePolynomial>, MultivariatePolynomial> 
PolynomialReduce(MultivariatePolynomial f, std::vector<MultivariatePolynomial> G, const MonomialOrder& order) {
    
    size_t n = G.size();
    std::vector<MultivariatePolynomial> alpha(n); 
    MultivariatePolynomial r; 

  
    f.sortTerms(order);
    for (size_t i = 0; i < n; ++i) {
        G[i].sortTerms(order);
    }

    while (!f.isZero()) {
        Term lt_f = f.LT();
        bool divided = false;

        for (size_t i = 0; i < n; ++i) {
            if (G[i].isZero()) continue;
            Term lt_g = G[i].LT();
            Term q_term;

            if (divideTerms(lt_f, lt_g, q_term)) {

                MultivariatePolynomial q_poly;
                q_poly.terms.push_back(q_term);

                alpha[i] = alpha[i] + q_poly;
                f = f - G[i].multiplyByTerm(q_term);
                f.sortTerms(order); 

                divided = true;
                break; 
            }
        }

        if (!divided) {
            MultivariatePolynomial lt_f_poly;
            lt_f_poly.terms.push_back(lt_f);
            
            r.terms.push_back(lt_f);
            f = f - lt_f_poly;
            f.sortTerms(order);
        }
    }

    r.sortTerms(order);
    return {alpha, r};
}
void printMultivariate(const MultivariatePolynomial& p) {
    if (p.isZero()) {
        std::cout << "0";
        return;
    }
    bool first = true;
    for (const auto& t : p.terms) {
        if (t.coeff > 0 && !first) std::cout << " + ";
        else if (t.coeff < 0) std::cout << " - ";
        
        double absCoeff = std::abs(t.coeff);
        bool hasVars = false;
        for (int exp : t.mono.exponents) if (exp > 0) hasVars = true;
        
        if (absCoeff != 1.0 || !hasVars) {
            std::cout << absCoeff;
        }

        char varNames[] = {'x', 'y', 'z'};
        for (size_t i = 0; i < t.mono.exponents.size(); ++i) {
            if (t.mono.exponents[i] > 0) {
                std::cout << varNames[i % 3];
                if (t.mono.exponents[i] > 1) {
                    std::cout << "^" << t.mono.exponents[i];
                }
            }
        }
        first = false;
    }
}

int main() {
    MultivariatePolynomial h;
    h.terms.push_back({1.0,  {{2, 7, 0}}}); 
    h.terms.push_back({-1.0,  {{0, 9, 7}}});
    h.terms.push_back({1.0, {{5, 0, 2}}}); 

    MultivariatePolynomial g1, g2;
    g1.terms.push_back({1.0,  {{2, 7, 0}}}); 
    g1.terms.push_back({-1.0, {{0, 0, 1}}});

    g2.terms.push_back({1.0,  {{0, 9, 7}}}); 
    g2.terms.push_back({-1.0, {{1, 0, 0}}}); 

    std::vector<MultivariatePolynomial> G = {g1, g2};

    std::cout << "LABORATORIUM 4 - INDEKS 279752 (UPROSZCZONE G)\n";

    MonomialOrder lexOrder = { LEX, {} };
    auto [alpha_lex, r_lex] = PolynomialReduce(h, G, lexOrder);
    std::cout << "--- [1] Porządek STANDARDOWY LEX (x > y > z) ---\n";
    std::cout << "alpha_1 = "; printMultivariate(alpha_lex[0]); std::cout << "\n";
    std::cout << "alpha_2 = "; printMultivariate(alpha_lex[1]); std::cout << "\n";
    std::cout << "Reszta r = "; printMultivariate(r_lex); std::cout << "\n\n";

    MonomialOrder permutedLexOrder = { PERMUTED_LEX, {2, 1, 0} };
    auto [alpha_perm, r_perm] = PolynomialReduce(h, G, permutedLexOrder);
    std::cout << "--- [2] Porządek PERMUTOWANY LEX (z > y > x) ---\n";
    std::cout << "alpha_1 = "; printMultivariate(alpha_perm[0]); std::cout << "\n";
    std::cout << "alpha_2 = "; printMultivariate(alpha_perm[1]); std::cout << "\n";
    std::cout << "Reszta r = "; printMultivariate(r_perm); std::cout << "\n\n";

    MonomialOrder glexOrder = { GRADED_LEX, {} };
    auto [alpha_glex, r_glex] = PolynomialReduce(h, G, glexOrder);
    std::cout << "--- [3] Porządek GRADED LEX (GLEX) ---\n";
    std::cout << "alpha_1 = "; printMultivariate(alpha_glex[0]); std::cout << "\n";
    std::cout << "alpha_2 = "; printMultivariate(alpha_glex[1]); std::cout << "\n";
    std::cout << "Reszta r = "; printMultivariate(r_glex); std::cout << "\n\n";

    std::vector<MultivariatePolynomial> G_reversed = {g2, g1};
    auto [alpha_rev, r_rev] = PolynomialReduce(h, G_reversed, lexOrder);
    std::cout << "--- [4d] Test kolejności (Standardowy LEX, ciąg {g2, g1}) ---\n";
    std::cout << "alpha dla g2 = "; printMultivariate(alpha_rev[0]); std::cout << "\n";
    std::cout << "alpha dla g1 = "; printMultivariate(alpha_rev[1]); std::cout << "\n";
    std::cout << "Reszta r = "; printMultivariate(r_rev); std::cout << "\n";
    std::cout << "====================================================\n";
    MultivariatePolynomial f_37;
    f_37.terms.push_back({1.0,  {{3, 0, 0}}}); 
    f_37.terms.push_back({-1.0, {{2, 1, 0}}}); 
    f_37.terms.push_back({-1.0, {{2, 0, 1}}}); 

    MultivariatePolynomial g1_37, g2_37;
    g1_37.terms.push_back({1.0,  {{2, 1, 0}}}); g1_37.terms.push_back({-1.0, {{0, 0, 1}}}); 
    g2_37.terms.push_back({1.0,  {{1, 1, 0}}}); g2_37.terms.push_back({-1.0, {{0, 0, 0}}});

    MonomialOrder glex = { GRADED_LEX, {} };

    auto [alpha_1, r1] = PolynomialReduce(f_37, {g1_37, g2_37}, glex);
    std::cout << "1. Redukcja przez (g1, g2):\n";
    std::cout << "   alpha_1 = "; printMultivariate(alpha_1[0]); std::cout << "\n";
    std::cout << "   alpha_2 = "; printMultivariate(alpha_1[1]); std::cout << "\n";
    std::cout << "   Reszta r1 = "; printMultivariate(r1); std::cout << "\n\n";

    auto [alpha_2, r2] = PolynomialReduce(f_37, {g2_37, g1_37}, glex);
    std::cout << "2. Redukcja przez (g2, g1):\n";
    std::cout << "   alpha_1 = "; printMultivariate(alpha_2[0]); std::cout << "\n";
    std::cout << "   alpha_2 = "; printMultivariate(alpha_2[1]); std::cout << "\n";
    std::cout << "   Reszta r2 = "; printMultivariate(r2); std::cout << "\n\n";

    MultivariatePolynomial diff = r1 - r2;
    std::cout << "3. Roznica r1 - r2 = "; printMultivariate(diff); std::cout << "\n";
    
    auto [alpha_diff, r_diff] = PolynomialReduce(diff, {g1_37, g2_37}, glex);
    std::cout << "   Reszta z redukcji (r1 - r2) przez G: "; printMultivariate(r_diff); std::cout << "\n";
}