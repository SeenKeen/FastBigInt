#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

std::string operator * (std::string s, size_t n) {
    std::string result;
    result.resize(s.size() * n);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < s.size(); ++j) {
            result[i * s.size() + j] = s[j];
        }
    }
    return result;
}

#define S std::string

class Long {

public:


    static Long FromString(const std::string str) {
        auto s = str;
        std::reverse(s.begin(), s.end());
        return s;
    }

    Long operator + (const Long& rhs) const {
        std::string buf = S("0") * (std::max(rhs.Divs(), Divs()) + 1);
        
        for (size_t i = 0; i < buf.size(); ++i) {
            if (i < Divs()) {
                buf[i] += divs_[i] - '0';
            }
            if (i < rhs.Divs()) {
                buf[i] += rhs.At(i) - '0';
            }
            if (buf[i] > '9') {
                buf[i + 1] += (buf[i] - '0') / 10;
                buf[i] = (buf[i] - '0') % 10 + '0';
            }
        }
        if (buf.back() == '0') {
            buf.pop_back();
        }
        return buf;
    }

    Long operator * (const Long& rhs) const {
        size_t divs = std::max(rhs.Divs(), Divs());
        if (divs != 1) divs += divs % 2;
        if (divs == 1) {
            int mul = (rhs.At(0) - '0') * (divs_[0] - '0');
            std::string buf;
            buf.push_back(mul % 10 + '0');
            if (mul / 10) {
                buf.push_back(mul / 10 + '0');
            }
            return buf;
        }
        Long b = SliceFirstN(divs / 2);
        Long a = SliceSecondN(divs / 2);
        Long d = rhs.SliceFirstN(divs / 2);
        Long c = rhs.SliceSecondN(divs / 2);
        Long x2_cf = a * c;
        Long x_cf = a * d + b * c;
        Long free_cf = b * d;
        x2_cf.MultByPow10(divs);
        x_cf.MultByPow10(divs / 2);
        return x2_cf + x_cf + free_cf;
    }

    size_t Divs() const {
        return divs_.size();
    }

    char At(size_t idx) const {
        return divs_[idx];
    }

    friend std::ostream& operator << (std::ostream&, const Long&);

private:

    Long(std::string divs) : divs_(divs) {
        while (divs_.back() == '0' && divs_.size() > 1) {
            divs_.pop_back();
        }
    }

    void MultByPow10(size_t pow) {
        auto buf = S("0") * (Divs() + pow);
        for (size_t i = 0; i < Divs(); ++i) {
            buf[i + pow] = divs_[i];
        }
        divs_ = buf;
    }

    Long SliceFirstN(size_t n) const {
        std::string buf;
        for (size_t i = 0; i < std::min(n, Divs()); ++i) {
            buf.push_back(divs_[i]);
        }
        if (buf.empty()) {
            buf += "0";
        }
       
        return buf;
    }

    Long SliceSecondN(size_t n) const {
        std::string buf;
        for (size_t i = std::min(n, Divs()); i < std::min(2*n, Divs()); ++i) {
            buf.push_back(divs_[i]);
        }
        if (buf.empty()) {
            buf += "0";
        }
        return buf;
    }

private:
    std::string divs_;
    
};

#include <ranges>

std::ostream& operator << (std::ostream& os, const Long& num) {
    for (auto it = num.divs_.rbegin(); it != num.divs_.rend(); ++it) {
        os << *it;
    }
    return os;
}


int main()
{
    Long x = Long::FromString("5890359399");
    Long y = Long::FromString("95345353459");
    std::cout << x * y;
    return 0;
}
