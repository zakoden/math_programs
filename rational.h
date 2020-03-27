#ifndef RATIONAL_H_INCLUDED
#define RATIONAL_H_INCLUDED

#include <cmath>
#include <iostream>
#include <numeric>
#include <string>

#include "print_modes.h"

using NumType = int64_t;

class Rational;
Rational operator+ (const Rational& fir, const Rational& sec);
Rational operator- (const Rational& fir, const Rational& sec);
Rational operator* (const Rational& fir, const Rational& sec);
Rational operator/ (const Rational& fir, const Rational& sec);

class Rational {
private:
    NumType a, b; // rational a/b

    void reduce() {
        NumType cur_gcd = std::gcd(abs(a), abs(b));
        a /= cur_gcd;
        b /= cur_gcd;
    }

    // to calculate string expression
    Rational getExpr(const std::string& s, size_t& pos) const;
    Rational getSum(const std::string& s, size_t& pos) const;
    Rational getProduct(const std::string& s, size_t& pos) const;
    Rational getSubExpr(const std::string& s, size_t& pos) const;
    Rational getNum(const std::string& s, size_t& pos) const;

public:
    Rational(NumType numer = 0, NumType denom = 1): a(numer), b(denom) {
        if (numer != 0) {
            if (b < 0) {
                a = - a;
                b = - b;
            }
            reduce();
        } else {
            b = 1;
        }
    }

    // construct from string expression
    Rational(const std::string& expr) {
        size_t ind = 0;
        std::string s;
        for (const char& sym : expr) {
            if (sym != ' ') {
                s += sym;
            }
        }
        Rational res = getExpr(s, ind);
        a = res.numerator();
        b = res.denominator();
    }

    NumType numerator() const {
        return a;
    }

    NumType denominator() const {
        return b;
    }

    Rational& operator+= (const Rational& sec) {
        *this = *this + sec;
        return *this;
    }

    Rational& operator-= (const Rational& sec) {
        *this = *this - sec;
        return *this;
    }

    Rational& operator*= (const Rational& sec) {
        *this = *this * sec;
        return *this;
    }

    Rational& operator/= (const Rational& sec) {
        *this = *this / sec;
        return *this;
    }

    Rational& operator++ () {
        *this += Rational(1, 1);
        return *this;
    }

    Rational operator++ (int) {
        Rational save = *this;
        *this += Rational(1, 1);
        return save;
    }

    Rational& operator-- () {
        *this -= Rational(1, 1);
        return *this;
    }

    Rational operator-- (int) {
        Rational save = *this;
        *this -= Rational(1, 1);
        return save;
    }
};

Rational operator+ (const Rational& num) {
    return num;
}

Rational operator- (const Rational& num) {
    return Rational(- num.numerator(), num.denominator());
}

Rational operator+ (const Rational& fir, const Rational& sec) {
    return Rational(fir.numerator() * sec.denominator()
                  + sec.numerator() * fir.denominator(),
                    fir.denominator() * sec.denominator());
}

Rational operator+ (const Rational& fir, const NumType& sec) {
    return fir + Rational(sec, 1);
}

Rational operator+ (const NumType& fir, const Rational& sec) {
    return Rational(fir, 1) + sec;
}

Rational operator- (const Rational& fir, const Rational& sec) {
    return fir + (- sec);
}

Rational operator- (const Rational& fir, const NumType& sec) {
    return fir + (- sec);
}

Rational operator- (const NumType& fir, const Rational& sec) {
    return fir + (- sec);
}

Rational operator* (const Rational& fir, const Rational& sec) {
    return Rational(fir.numerator() * sec.numerator(),
                    fir.denominator() * sec.denominator());
}

Rational operator* (const Rational& fir, const NumType& sec) {
    return fir * Rational(sec, 1);
}

Rational operator* (const NumType& fir, const Rational& sec) {
    return Rational(fir, 1) * sec;
}

Rational operator/ (const Rational& fir, const Rational& sec) {
    return fir * Rational(sec.denominator(), sec.numerator());
}

Rational operator/ (const Rational& fir, const NumType& sec) {
    return fir / Rational(sec, 1);
}

Rational operator/ (const NumType& fir, const Rational& sec) {
    return Rational(fir, 1) / sec;
}

bool operator== (const Rational& fir, const Rational& sec) {
    return (fir.numerator() == sec.numerator())
        && (fir.denominator() == sec.denominator());
}

bool operator!= (const Rational& fir, const Rational& sec) {
    return !(fir == sec);
}

bool operator< (const Rational& fir, const Rational& sec) {
    Rational res = fir - sec;
    return (res.numerator() < 0);
}

bool operator> (const Rational& fir, const Rational& sec) {
    return (sec < fir);
}

std::ostream& operator<< (std::ostream& out, const Rational& r) {
    std::string str;
    switch (print_modes::printMode) {
    case print_modes::SIMPLE: // examples: (5), (-11/12)
        if (r.denominator() == 1) {
            out << "(" << r.numerator() << ")";
        } else {
            out << "(" << r.numerator() << "/" << r.denominator() << ")";
        }
        break;
    case print_modes::VISUAL: // examples: 5, -11/12
        if (r.denominator() == 1) {
            str = std::to_string(r.numerator());
        } else {
            str = std::to_string(r.numerator()) + "/" +
                  std::to_string(r.denominator());
        }
        out << std::left << str;
        break;
    case print_modes::LATEX: // examples: 5, -\frac{11}{12}
        if (r.denominator() == 1) {
            out << r.numerator();
        } else {
            if (r.numerator() < 0) out << "-";
            out << "\frac{"  << abs(r.numerator()) << "}{" << r.denominator() << "}";
        }
        break;
    default:
        break;
    }
    return out;
}

std::istream& operator>> (std::istream& in, Rational& r) {
    std::string s;
    in >> s;
    r = Rational(s);
    return in;
}

Rational Rational::getExpr(const std::string& s, size_t& pos) const {
    return getSum(s, pos);
}

// operations: +, -
// arguments: Product
Rational Rational::getSum(const std::string& s, size_t& pos) const {
    Rational res, other;
    res = getProduct(s, pos);
    char oper;
    while (pos < s.size() && (s[pos] == '+' || s[pos] == '-')) {
        oper = s[pos];
        ++pos;
        other = getProduct(s, pos);
        if (oper == '+') {
            res += other;
        } else {
            res -= other;
        }
    }
    return res;
}

// operations: *, /
// arguments: SubExpr
Rational Rational::getProduct(const std::string& s, size_t& pos) const {
    Rational res, other;
    res = getSubExpr(s, pos);
    char oper;
    while (pos < s.size() && (s[pos] == '*' || s[pos] == '/')) {
        oper = s[pos];
        ++pos;
        other = getSubExpr(s, pos);
        if (oper == '*') {
            res *= other;
        } else {
            res /= other;
        }
    }
    return res;
}

// SubExpr equal to (Sum) or Num
Rational Rational::getSubExpr(const std::string& s, size_t& pos) const {
    Rational res;
    if (s[pos] == '(') {
        ++pos;
        res = getSum(s, pos);
        ++pos;
    } else {
        res = getNum(s, pos);
    }
    return res;
}

// convert substring to NumType
Rational Rational::getNum(const std::string& s, size_t& pos) const {
    Rational res;
    if (s[pos] == '-') {
        res = -1;
        ++pos;
    } else {
        res = 1;
    }
    NumType num = 0;
    while ('0' <= s[pos] && s[pos] <= '9') {
        num *= 10ll;
        num += static_cast<NumType>(s[pos] - '0');
        ++pos;
    }
    res *= num;
    return res;
}

#endif // RATIONAL_H_INCLUDED
