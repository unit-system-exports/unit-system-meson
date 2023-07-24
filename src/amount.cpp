#include "unit_system/amount.hpp"

sakurajin::unit_system::amount::amount(): amount{0.0}{}
sakurajin::unit_system::amount::amount(long double v): amount{v,1,0}{}
sakurajin::unit_system::amount::amount(long double v, long double mult): amount{v,mult,0}{}
sakurajin::unit_system::amount::amount(long double v, long double mult, long double off): multiplier{mult}, value{v}, offset{off}{}

//const functions
sakurajin::unit_system::amount sakurajin::unit_system::amount::operator*(long double scalar) const{
    sakurajin::unit_system::amount retval{value*scalar, multiplier, offset};
    return retval;
}

sakurajin::unit_system::amount operator*(long double scalar, const sakurajin::unit_system::amount& val){
    return {val.value*scalar, val.multiplier, val.offset};
}

long double sakurajin::unit_system::amount::operator/(const sakurajin::unit_system::amount& other) const{
    auto otherVal = unit_cast(other, multiplier, offset);
    return value/otherVal.value;
}

sakurajin::unit_system::amount sakurajin::unit_system::amount::operator/(long double scalar) const{
    sakurajin::unit_system::amount retval{value/scalar, multiplier, offset};
    return retval;
}

sakurajin::unit_system::amount sakurajin::unit_system::amount::operator+(const sakurajin::unit_system::amount& other) const{
    auto retval = sakurajin::unit_system::unit_cast(other, multiplier,offset);
    retval.value += value;
    return retval;
}

sakurajin::unit_system::amount sakurajin::unit_system::amount::operator-(const sakurajin::unit_system::amount& other) const{
    auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    retval.value = value - retval.value;
    return retval;
}

sakurajin::unit_system::amount sakurajin::unit_system::amount::operator-() const{
    return sakurajin::unit_system::amount{-value,multiplier,offset};
}

sakurajin::unit_system::amount::operator long double() const{
    auto retval = sakurajin::unit_system::unit_cast(*this, 1, 0);
    return retval.value;
}

//comparison operators
#if __cplusplus >= 202002L
    int sakurajin::unit_system::amount::operator<=>(const sakurajin::unit_system::amount& other) const{
        const auto retval = sakurajin::unit_system::unit_cast(other,multiplier,offset);
        if(*this < retval){
            return -1;
        }

        if(*this > retval){
            return 1;
        }

        return 0;
    }
#else
    bool sakurajin::unit_system::amount::operator<(const sakurajin::unit_system::amount& other) const{
        const auto retval = sakurajin::unit_system::unit_cast(other,multiplier,offset);
        return value < retval.value;
    }

    bool sakurajin::unit_system::amount::operator>(const sakurajin::unit_system::amount& other) const{
        const auto retval = sakurajin::unit_system::unit_cast(other,multiplier,offset);
        return value > retval.value;
    }

    bool sakurajin::unit_system::amount::operator<=(const sakurajin::unit_system::amount& other) const{
        const auto retval = sakurajin::unit_system::unit_cast(other,multiplier,offset);
        return value <= retval.value;
    }

    bool sakurajin::unit_system::amount::operator>=(const sakurajin::unit_system::amount& other) const{
        const auto retval = sakurajin::unit_system::unit_cast(other,multiplier,offset);
        return value >= retval.value;
    }

    bool sakurajin::unit_system::amount::operator==(const sakurajin::unit_system::amount& other) const{
        const auto retval = sakurajin::unit_system::unit_cast(other,multiplier,offset);
        return value == retval.value;
    }

    bool sakurajin::unit_system::amount::operator!=(const sakurajin::unit_system::amount& other) const{
        return ! (*this == other);
    }
#endif

//non const member functions
void sakurajin::unit_system::amount::operator*=(long double scalar){
    value*=scalar;
}

void sakurajin::unit_system::amount::operator/=(long double scalar){
    value/=scalar;
}

void sakurajin::unit_system::amount::operator+=(const sakurajin::unit_system::amount& other){
    const auto otherVal = sakurajin::unit_system::unit_cast(other,multiplier,offset);
    value += otherVal.value;
}

void sakurajin::unit_system::amount::operator-=(const sakurajin::unit_system::amount& other){
    const auto otherVal = sakurajin::unit_system::unit_cast(other,multiplier,offset);
    value -= otherVal.value;
}

void sakurajin::unit_system::amount::operator=(const sakurajin::unit_system::amount& other){
    const auto otherVal = sakurajin::unit_system::unit_cast(other,multiplier,offset);
    value = otherVal.value;
}

// external functions
std::ostream& sakurajin::unit_system::operator<<(std::ostream& os, const sakurajin::unit_system::amount& t){
    auto t1 = sakurajin::unit_system::unit_cast(t,1);
    return os << t1.value << " amount";
}

sakurajin::unit_system::amount sakurajin::unit_system::unit_cast(const sakurajin::unit_system::amount& unit, long double new_multiplier, long double new_offset){
    auto valBase0 = (unit.value + unit.offset) * unit.multiplier;
    sakurajin::unit_system::amount retval{valBase0/new_multiplier-new_offset, new_multiplier, new_offset};
    return retval;
}

sakurajin::unit_system::amount sakurajin::unit_system::clamp(const sakurajin::unit_system::amount& unit, const sakurajin::unit_system::amount& lower, const sakurajin::unit_system::amount& upper){
    auto _lower = unit_cast(lower, unit.multiplier, unit.offset);
    auto _upper = unit_cast(upper, unit.multiplier, unit.offset);

    auto val = unit.value > _lower.value ? ( unit.value < _upper.value ? unit.value : _upper.value ) : _lower.value;
    return sakurajin::unit_system::amount{val, unit.multiplier, unit.offset};
}

sakurajin::unit_system::amount std::abs(const sakurajin::unit_system::amount& unit){
    auto inv = -unit;
    return unit > inv ? unit : inv;
}

// literals



sakurajin::unit_system::amount sakurajin::unit_system::literals::operator "" _mol(long double len){
    return sakurajin::unit_system::amount{len,6.02214076e+23, 0.0};
}

sakurajin::unit_system::amount sakurajin::unit_system::literals::operator "" _mol(unsigned long long int len){
    return sakurajin::unit_system::amount{static_cast<long double>(len),6.02214076e+23, 0.0};
}



sakurajin::unit_system::amount sakurajin::unit_system::literals::operator "" _things(long double len){
    return sakurajin::unit_system::amount{len,1.0, 0.0};
}

sakurajin::unit_system::amount sakurajin::unit_system::literals::operator "" _things(unsigned long long int len){
    return sakurajin::unit_system::amount{static_cast<long double>(len),1.0, 0.0};
}


