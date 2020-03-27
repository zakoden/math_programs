#ifndef POLYNOMIAL_H_INCLUDED
#define POLYNOMIAL_H_INCLUDED

#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "print_modes.h"

template<typename T>
class Polynomial {
private:
    std::vector<T> coef;

    void deleteNulls() {
        if (coef.empty()) {
            coef.push_back(T(0));
        }
        while (coef.size() > 1 && coef.back() == T(0)) {
            coef.pop_back();
        }
    }

    void reduce() {
        deleteNulls();
        if (coef.size() > 0 && coef[coef.size() - 1] != T(0)) {
            for (size_t i = 0; i < coef.size(); ++i) {
                coef[i] = coef[i] / coef[coef.size() - 1];
            }
        }
    }

public:
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    Polynomial(const std::vector<T>& v) {
        coef = v;
        deleteNulls();
    }

    Polynomial(T val = T()) {
        coef.push_back(val);
    }

    template<typename Iter>
    Polynomial(Iter first, Iter last) {
        while (first != last) {
            coef.push_back(*first);
            ++first;
        }
        deleteNulls();
    }

    iterator begin() {
        return coef.begin();
    }

    iterator end() {
        return coef.end();
    }

    const_iterator begin() const {
        return coef.begin();
    }

    const_iterator end() const {
        return coef.end();
    }

    T operator[] (size_t ind) const {
        if (ind >= coef.size()) {
            return T(0);
        } else {
            return coef[ind];
        }
    }

    // equivalent to f(arg)
    T operator() (const T& arg) const {
        T ans = coef[0];
        T cur = arg;
        for (size_t i = 1; i < coef.size(); ++i) {
            ans += (cur * coef[i]);
            cur *= arg;
        }
        return ans;
    }

    // Degree == -1 for zero polynomial
    int Degree() const {
        if (coef.size() == 1 && coef[0] == T(0)) {
            return - 1;
        } else {
            return coef.size() - 1;
        }
    }

    bool operator== (const Polynomial& other) const {
        return coef == other.coef;
    }

    bool operator!= (const Polynomial& other) const {
        return !(*this == other);
    }

    Polynomial& operator+= (const Polynomial& other) {
        size_t len = std::max(coef.size(), other.coef.size());
        coef.resize(len);
        for (size_t i = 0; i < other.coef.size(); ++i) {
            coef[i] += other.coef[i];
        }
        deleteNulls();
        return *this;
    }

    Polynomial& operator+= (const T& other) {
        *this += Polynomial(other);
        return *this;
    }

    Polynomial& operator-= (const Polynomial& other) {
        size_t len = std::max(coef.size(), other.coef.size());
        coef.resize(len);
        for (size_t i = 0; i < other.coef.size(); ++i) {
            coef[i] -= other.coef[i];
        }
        deleteNulls();
        return *this;
    }

    Polynomial& operator-= (const T& other) {
        *this -= Polynomial(other);
        return *this;
    }

    Polynomial& operator*= (const Polynomial& other) {
        Polynomial save = *this;
        coef.resize(save.coef.size() * other.coef.size());
        for (size_t i = 0; i < coef.size(); ++i) {
            coef[i] = T(0);
            for (size_t j = 0; j <= i; ++j) {
                coef[i] += save[j] * other[i - j];
            }
        }
        deleteNulls();
        return *this;
    }

    Polynomial& operator*= (const T& other) {
        for (size_t i = 0; i < coef.size(); ++i) {
            coef[i] *= other;
        }
        deleteNulls();
        return *this;
    }

    // equivalent to f(g(x))
    Polynomial operator& (const Polynomial& other) const {
        Polynomial ans(coef[0]);
        Polynomial cur(other);
        for (size_t i = 1; i < coef.size(); ++i) {
            ans += cur * coef[i];
            cur *= other;
        }
        ans.deleteNulls();
        return ans;
    }

    Polynomial operator+ () const {
        return (*this);
    }

    Polynomial operator- () const {
        return (*this) * Polynomial(-1);
    }

    Polynomial operator/ (const Polynomial& other) const {
        Polynomial ans(T(0));
        if (Degree() < other.Degree()) {
            return ans;
        }
        Polynomial cur(*this);
        int power = cur.Degree() - other.Degree();
        Polynomial delt;
        delt.coef.resize(power + 1, T(0));
        for (; power >= 0; --power) {
            delt.coef[delt.coef.size() - 1] = (cur[other.Degree() + power] / other[other.Degree()]);
            ans += delt;
            cur -= other * delt;
            delt.coef.pop_back();
        }
        ans.deleteNulls();
        return ans;
    }

    Polynomial operator% (const Polynomial& other) const {
        Polynomial ans = *this - (*this / other) * other;
        ans.deleteNulls();
        return ans;
    }

    Polynomial operator, (const Polynomial& other) const {
        Polynomial a(*this), b(other);
        a.deleteNulls();
        b.deleteNulls();
        if (a == T(0) && b == T(0)) {
            return Polynomial(T(0));
        }
        Polynomial ans;
        while (true) {
            if (a.Degree() == -1) {
                ans = b;
                break;
            }
            if (b.Degree() == -1) {
                ans = a;
                break;
            }
            if (a.Degree() >= b.Degree()) {
                a = a % b;
                a.reduce();
            } else {
                b = b % a;
                b.reduce();
            }
        }
        ans.deleteNulls();
        ans.reduce();
        return ans;
    }
};

template<typename T>
bool operator== (const Polynomial<T>& fir, const T& sec) {
    return fir == Polynomial(sec);
}

template<typename T>
bool operator!= (const Polynomial<T>& fir, const T& sec) {
    return fir != Polynomial(sec);
}

template<typename T>
bool operator== (const T& fir, const Polynomial<T>& sec) {
    return Polynomial(fir) == sec;
}

template<typename T>
bool operator!= (const T& fir, const Polynomial<T>& sec) {
    return Polynomial(fir) != sec;
}

template<typename T>
Polynomial<T> operator+ (const Polynomial<T>& fir, const Polynomial<T>& sec) {
    Polynomial<T> ans(fir);
    ans += sec;
    return ans;
}

template<typename T>
Polynomial<T> operator+ (const Polynomial<T>& fir, const T& sec) {
    return fir + Polynomial(sec);
}

template<typename T>
Polynomial<T> operator+ (const T& fir, const Polynomial<T>& sec) {
    return Polynomial(fir) + sec;
}

template<typename T>
Polynomial<T> operator- (const Polynomial<T>& fir, const Polynomial<T>& sec) {
    Polynomial<T> ans(fir);
    ans -= sec;
    return ans;
}

template<typename T>
Polynomial<T> operator- (const Polynomial<T>& fir, const T& sec) {
    return fir - Polynomial(sec);
}

template<typename T>
Polynomial<T> operator- (const T& fir, const Polynomial<T>& sec) {
    return Polynomial(fir) - sec;
}

template<typename T>
Polynomial<T> operator* (const Polynomial<T>& fir, const Polynomial<T>& sec) {
    Polynomial<T> ans(fir);
    ans *= sec;
    return ans;
}

template<typename T>
Polynomial<T> operator* (const Polynomial<T>& fir, const T& sec) {
    return fir * Polynomial(sec);
}

template<typename T>
Polynomial<T> operator* (const T& fir, const Polynomial<T>& sec) {
    return Polynomial(fir) * sec;
}


template<typename T>
void print_simple(std::stringstream& out, const Polynomial<T>& p) {
    for (int deg = p.Degree(); deg > 0; --deg) {
        if (p[deg] == T(0)) {
            continue;
        }
        if (deg != p.Degree()) {
            out << "+";
        }
        out << p[deg];
        out << "x";
        if (deg > 1) {
            out << "^" << deg;
        }
    }
    if (p[0] != T(0) || p.Degree() == -1) {
        if (p.Degree() > 0) {
            out << "+";
        }
        out << p[0];
    }
}

template<typename T>
void print_visual(std::stringstream& s_out, const Polynomial<T>& p) {
    for (int deg = p.Degree(); deg > 0; --deg) {
        if (p[deg] == T(0)) {
            continue;
        }
        if (p[deg] > T(0) && deg < p.Degree()) {
            s_out << "+";
        }
        if (p[deg] == T(-1)) {
            s_out << "-";
        }
        if (p[deg] != T(1) && p[deg] != T(-1)) {
            s_out << p[deg] << "*";
        }
        s_out << "x";
        if (deg > 1) {
            s_out << "^" << deg;
        }
    }
    if (p[0] != T(0) || p.Degree() == -1) {
        if (p[0] > T(0) && p.Degree() > 0) {
            s_out << "+";
        }
        s_out << p[0];
    }
}

template<typename T>
void print_latex(std::stringstream& s_out, const Polynomial<T>& p) {
    for (int deg = p.Degree(); deg > 0; --deg) {
        if (p[deg] == T(0)) {
            continue;
        }
        if (p[deg] > T(0) && deg < p.Degree()) {
            s_out << "+";
        }
        if (p[deg] == T(-1)) {
            s_out << "-";
        }
        if (p[deg] != T(1) && p[deg] != T(-1)) {
            s_out << p[deg];
        }
        s_out << "x";
        if (deg > 1) {
            s_out << "^{" << deg << "}";
        }
    }
    if (p[0] != T(0) || p.Degree() == -1) {
        if (p[0] > T(0) && p.Degree() > 0) {
            s_out << "+";
        }
        s_out << p[0];
    }
}

template<typename T>
std::ostream& operator<< (std::ostream& out, const Polynomial<T>& p) {
    std::stringstream s_out;

    switch (print_modes::printMode) {
    case print_modes::SIMPLE:
        print_simple(s_out, p);
        out << s_out.str();
        break;
    case print_modes::VISUAL:
        print_visual(s_out, p);
        out << std::left << s_out.str();
        break;
    case print_modes::LATEX:
        print_latex(s_out, p);
        out << s_out.str();
        break;
    default:
        break;
    }
    return out;
}


// works only for degree 0
template<typename T>
std::istream& operator>> (std::istream& in, Polynomial<T>& p) {
    std::string parse;
    in >> parse;
    p = Polynomial<T>(T(parse));
    return in;
}

#endif // POLYNOMIAL_H_INCLUDED
