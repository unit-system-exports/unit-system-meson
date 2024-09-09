#include "cmath"
#include "unit_system.hpp"

sakurajin::unit_system::electric_current::electric_current()
    : electric_current{0.0} {}
sakurajin::unit_system::electric_current::electric_current(long double v)
    : electric_current{v, 1, 0} {}
sakurajin::unit_system::electric_current::electric_current(long double v, long double mult)
    : electric_current{v, mult, 0} {}
sakurajin::unit_system::electric_current::electric_current(long double v, long double mult, long double off)
    : value{v},
      multiplier{mult},
      offset{off} {}


long double sakurajin::unit_system::electric_current::mult() const {
    return multiplier;
}
long double sakurajin::unit_system::electric_current::val() const {
    return value;
}
long double sakurajin::unit_system::electric_current::off() const {
    return offset;
}
long double sakurajin::unit_system::electric_current::rel_err() const {
    return rel_error;
}

long double& sakurajin::unit_system::electric_current::mult() {
    return multiplier;
}
long double& sakurajin::unit_system::electric_current::val() {
    return value;
}
long double& sakurajin::unit_system::electric_current::off() {
    return offset;
}
long double& sakurajin::unit_system::electric_current::rel_err() {
    return rel_error;
}

// const functions
sakurajin::unit_system::electric_current sakurajin::unit_system::electric_current::operator*(long double scalar) const {
    return sakurajin::unit_system::electric_current{value * scalar, multiplier, offset};
}

sakurajin::unit_system::electric_current operator*(long double scalar, const sakurajin::unit_system::electric_current& val) {
    return sakurajin::unit_system::electric_current{val.val() * scalar, val.mult(), val.off()};
}

long double sakurajin::unit_system::electric_current::operator/(const sakurajin::unit_system::electric_current& other) const {
    return value / other.convert_like(*this).val();
}

sakurajin::unit_system::electric_current sakurajin::unit_system::electric_current::operator/(long double scalar) const {
    return sakurajin::unit_system::electric_current{value / scalar, multiplier, offset};
}

sakurajin::unit_system::electric_current
sakurajin::unit_system::electric_current::operator+(const sakurajin::unit_system::electric_current& other) const {
    auto retval = convert_like(other);
    retval.val() += other.val();
    return retval;
}

sakurajin::unit_system::electric_current
sakurajin::unit_system::electric_current::operator-(const sakurajin::unit_system::electric_current& other) const {
    auto retval = convert_like(other);
    retval.val() -= other.val();
    return retval;
}

sakurajin::unit_system::electric_current sakurajin::unit_system::electric_current::operator-() const {
    return sakurajin::unit_system::electric_current{-value, multiplier, offset};
}

sakurajin::unit_system::electric_current::operator long double() const {
    return convert_copy(1, 0).val();
}

sakurajin::unit_system::electric_current sakurajin::unit_system::electric_current::convert_multiplier(long double new_multiplier) const {
    return convert_copy(new_multiplier, offset);
}

sakurajin::unit_system::electric_current sakurajin::unit_system::electric_current::convert_offset(long double new_offset) const {
    return convert_copy(multiplier, new_offset);
}

sakurajin::unit_system::electric_current sakurajin::unit_system::electric_current::convert_copy(long double new_multiplier,
                                                                                                long double new_offset) const {
    auto                                     valBase0 = value * multiplier + offset;
    sakurajin::unit_system::electric_current retval{valBase0 / new_multiplier - new_offset, new_multiplier, new_offset};
    return retval;
}

sakurajin::unit_system::electric_current
sakurajin::unit_system::electric_current::convert_like(const sakurajin::unit_system::electric_current& other) const {
    return convert_copy(other.multiplier, other.offset);
}

// comparison operators
#if __cplusplus >= 202002L
int sakurajin::unit_system::electric_current::operator<=>(const sakurajin::unit_system::electric_current& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    if (*this < retval) {
        return -1;
    }

    if (*this > retval) {
        return 1;
    }

    return 0;
}
#endif
bool sakurajin::unit_system::electric_current::operator<(const sakurajin::unit_system::electric_current& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    return value < retval.val();
}

bool sakurajin::unit_system::electric_current::operator>(const sakurajin::unit_system::electric_current& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    return value > retval.val();
}

bool sakurajin::unit_system::electric_current::operator<=(const sakurajin::unit_system::electric_current& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    return value <= retval.val();
}

bool sakurajin::unit_system::electric_current::operator>=(const sakurajin::unit_system::electric_current& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    return value >= retval.val();
}

bool sakurajin::unit_system::electric_current::operator==(const sakurajin::unit_system::electric_current& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    return value == retval.val();
}

bool sakurajin::unit_system::electric_current::operator!=(const sakurajin::unit_system::electric_current& other) const {
    return !(*this == other);
}

// non const member functions
void sakurajin::unit_system::electric_current::operator*=(long double scalar) {
    value *= scalar;
}

void sakurajin::unit_system::electric_current::operator/=(long double scalar) {
    value /= scalar;
}

void sakurajin::unit_system::electric_current::operator+=(const sakurajin::unit_system::electric_current& other) {
    const auto otherVal = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    value += otherVal.val();
}

void sakurajin::unit_system::electric_current::operator-=(const sakurajin::unit_system::electric_current& other) {
    const auto otherVal = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    value -= otherVal.val();
}

void sakurajin::unit_system::electric_current::operator=(const sakurajin::unit_system::electric_current& other) {
    const auto otherVal = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    value               = otherVal.val();
}


// external functions

sakurajin::unit_system::electric_current sakurajin::unit_system::operator*(long double                                     scalar,
                                                                           const sakurajin::unit_system::electric_current& value) {
    return value * scalar;
}


sakurajin::unit_system::electric_current sakurajin::unit_system::unit_cast(const sakurajin::unit_system::electric_current& unit,
                                                                           long double                                     new_multiplier,
                                                                           long double                                     new_offset) {
    return unit.convert_copy(new_multiplier, new_offset);
}

sakurajin::unit_system::electric_current sakurajin::unit_system::clamp(const sakurajin::unit_system::electric_current& unit,
                                                                       const sakurajin::unit_system::electric_current& lower,
                                                                       const sakurajin::unit_system::electric_current& upper) {
    auto _lower = unit_cast(lower, unit.mult(), unit.off());
    auto _upper = unit_cast(upper, unit.mult(), unit.off());

    auto val = unit.val() > _lower.val() ? (unit.val() < _upper.val() ? unit.val() : _upper.val()) : _lower.val();
    return sakurajin::unit_system::electric_current{val, unit.mult(), unit.off()};
}


sakurajin::unit_system::electric_current std::abs(const sakurajin::unit_system::electric_current& unit) {
    auto inv = -unit;
    return unit > inv ? unit : inv;
}

std::ostream& sakurajin::unit_system::operator<<(std::ostream& os, const sakurajin::unit_system::electric_current& t) {
    auto t1 = sakurajin::unit_system::unit_cast(t, 1);
    return os << t1.val() << " Ampere";
}


// literals


sakurajin::unit_system::electric_current sakurajin::unit_system::literals::operator"" _A(long double len) {
    return sakurajin::unit_system::electric_current{len, 1.0, 0.0};
}

sakurajin::unit_system::electric_current sakurajin::unit_system::literals::operator"" _A(unsigned long long int len) {
    return sakurajin::unit_system::electric_current{static_cast<long double>(len), 1.0, 0.0};
}

sakurajin::unit_system::electric_current sakurajin::unit_system::literals::operator"" _PA(long double len) {
    return sakurajin::unit_system::electric_current{len, 1000000000000000.0, 0.0};
}

sakurajin::unit_system::electric_current sakurajin::unit_system::literals::operator"" _PA(unsigned long long int len) {
    return sakurajin::unit_system::electric_current{static_cast<long double>(len), 1000000000000000.0, 0.0};
}

sakurajin::unit_system::electric_current sakurajin::unit_system::literals::operator"" _TA(long double len) {
    return sakurajin::unit_system::electric_current{len, 1000000000000.0, 0.0};
}

sakurajin::unit_system::electric_current sakurajin::unit_system::literals::operator"" _TA(unsigned long long int len) {
    return sakurajin::unit_system::electric_current{static_cast<long double>(len), 1000000000000.0, 0.0};
}

sakurajin::unit_system::electric_current sakurajin::unit_system::literals::operator"" _GA(long double len) {
    return sakurajin::unit_system::electric_current{len, 1000000000.0, 0.0};
}

sakurajin::unit_system::electric_current sakurajin::unit_system::literals::operator"" _GA(unsigned long long int len) {
    return sakurajin::unit_system::electric_current{static_cast<long double>(len), 1000000000.0, 0.0};
}

sakurajin::unit_system::electric_current sakurajin::unit_system::literals::operator"" _MA(long double len) {
    return sakurajin::unit_system::electric_current{len, 1000000.0, 0.0};
}

sakurajin::unit_system::electric_current sakurajin::unit_system::literals::operator"" _MA(unsigned long long int len) {
    return sakurajin::unit_system::electric_current{static_cast<long double>(len), 1000000.0, 0.0};
}

sakurajin::unit_system::electric_current sakurajin::unit_system::literals::operator"" _kA(long double len) {
    return sakurajin::unit_system::electric_current{len, 1000.0, 0.0};
}

sakurajin::unit_system::electric_current sakurajin::unit_system::literals::operator"" _kA(unsigned long long int len) {
    return sakurajin::unit_system::electric_current{static_cast<long double>(len), 1000.0, 0.0};
}

sakurajin::unit_system::electric_current sakurajin::unit_system::literals::operator"" _mA(long double len) {
    return sakurajin::unit_system::electric_current{len, 0.001, 0.0};
}

sakurajin::unit_system::electric_current sakurajin::unit_system::literals::operator"" _mA(unsigned long long int len) {
    return sakurajin::unit_system::electric_current{static_cast<long double>(len), 0.001, 0.0};
}

sakurajin::unit_system::electric_current sakurajin::unit_system::literals::operator"" _uA(long double len) {
    return sakurajin::unit_system::electric_current{len, 1e-06, 0.0};
}

sakurajin::unit_system::electric_current sakurajin::unit_system::literals::operator"" _uA(unsigned long long int len) {
    return sakurajin::unit_system::electric_current{static_cast<long double>(len), 1e-06, 0.0};
}

sakurajin::unit_system::electric_current sakurajin::unit_system::literals::operator"" _nA(long double len) {
    return sakurajin::unit_system::electric_current{len, 1e-09, 0.0};
}

sakurajin::unit_system::electric_current sakurajin::unit_system::literals::operator"" _nA(unsigned long long int len) {
    return sakurajin::unit_system::electric_current{static_cast<long double>(len), 1e-09, 0.0};
}

sakurajin::unit_system::electric_current sakurajin::unit_system::literals::operator"" _pA(long double len) {
    return sakurajin::unit_system::electric_current{len, 1e-12, 0.0};
}

sakurajin::unit_system::electric_current sakurajin::unit_system::literals::operator"" _pA(unsigned long long int len) {
    return sakurajin::unit_system::electric_current{static_cast<long double>(len), 1e-12, 0.0};
}

sakurajin::unit_system::electric_current sakurajin::unit_system::literals::operator"" _fA(long double len) {
    return sakurajin::unit_system::electric_current{len, 1e-15, 0.0};
}

sakurajin::unit_system::electric_current sakurajin::unit_system::literals::operator"" _fA(unsigned long long int len) {
    return sakurajin::unit_system::electric_current{static_cast<long double>(len), 1e-15, 0.0};
}

sakurajin::unit_system::electric_current sakurajin::unit_system::literals::operator"" _aA(long double len) {
    return sakurajin::unit_system::electric_current{len, 1e-18, 0.0};
}

sakurajin::unit_system::electric_current sakurajin::unit_system::literals::operator"" _aA(unsigned long long int len) {
    return sakurajin::unit_system::electric_current{static_cast<long double>(len), 1e-18, 0.0};
}
