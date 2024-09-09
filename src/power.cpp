#include "cmath"
#include "unit_system.hpp"

sakurajin::unit_system::power::power()
    : power{0.0} {}
sakurajin::unit_system::power::power(long double v)
    : power{v, 1, 0} {}
sakurajin::unit_system::power::power(long double v, long double mult)
    : power{v, mult, 0} {}
sakurajin::unit_system::power::power(long double v, long double mult, long double off)
    : value{v},
      multiplier{mult},
      offset{off} {}


long double sakurajin::unit_system::power::mult() const {
    return multiplier;
}
long double sakurajin::unit_system::power::val() const {
    return value;
}
long double sakurajin::unit_system::power::off() const {
    return offset;
}
long double sakurajin::unit_system::power::rel_err() const {
    return rel_error;
}

long double& sakurajin::unit_system::power::mult() {
    return multiplier;
}
long double& sakurajin::unit_system::power::val() {
    return value;
}
long double& sakurajin::unit_system::power::off() {
    return offset;
}
long double& sakurajin::unit_system::power::rel_err() {
    return rel_error;
}

// const functions
sakurajin::unit_system::power sakurajin::unit_system::power::operator*(long double scalar) const {
    return sakurajin::unit_system::power{value * scalar, multiplier, offset};
}

sakurajin::unit_system::power operator*(long double scalar, const sakurajin::unit_system::power& val) {
    return sakurajin::unit_system::power{val.val() * scalar, val.mult(), val.off()};
}

long double sakurajin::unit_system::power::operator/(const sakurajin::unit_system::power& other) const {
    return value / other.convert_like(*this).val();
}

sakurajin::unit_system::power sakurajin::unit_system::power::operator/(long double scalar) const {
    return sakurajin::unit_system::power{value / scalar, multiplier, offset};
}

sakurajin::unit_system::power sakurajin::unit_system::power::operator+(const sakurajin::unit_system::power& other) const {
    auto retval = convert_like(other);
    retval.val() += other.val();
    return retval;
}

sakurajin::unit_system::power sakurajin::unit_system::power::operator-(const sakurajin::unit_system::power& other) const {
    auto retval = convert_like(other);
    retval.val() -= other.val();
    return retval;
}

sakurajin::unit_system::power sakurajin::unit_system::power::operator-() const {
    return sakurajin::unit_system::power{-value, multiplier, offset};
}

sakurajin::unit_system::power::operator long double() const {
    return convert_copy(1, 0).val();
}

sakurajin::unit_system::power sakurajin::unit_system::power::convert_multiplier(long double new_multiplier) const {
    return convert_copy(new_multiplier, offset);
}

sakurajin::unit_system::power sakurajin::unit_system::power::convert_offset(long double new_offset) const {
    return convert_copy(multiplier, new_offset);
}

sakurajin::unit_system::power sakurajin::unit_system::power::convert_copy(long double new_multiplier, long double new_offset) const {
    auto                          valBase0 = value * multiplier + offset;
    sakurajin::unit_system::power retval{valBase0 / new_multiplier - new_offset, new_multiplier, new_offset};
    return retval;
}

sakurajin::unit_system::power sakurajin::unit_system::power::convert_like(const sakurajin::unit_system::power& other) const {
    return convert_copy(other.multiplier, other.offset);
}

// comparison operators
#if __cplusplus >= 202002L
int sakurajin::unit_system::power::operator<=>(const sakurajin::unit_system::power& other) const {
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
bool sakurajin::unit_system::power::operator<(const sakurajin::unit_system::power& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    return value < retval.val();
}

bool sakurajin::unit_system::power::operator>(const sakurajin::unit_system::power& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    return value > retval.val();
}

bool sakurajin::unit_system::power::operator<=(const sakurajin::unit_system::power& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    return value <= retval.val();
}

bool sakurajin::unit_system::power::operator>=(const sakurajin::unit_system::power& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    return value >= retval.val();
}

bool sakurajin::unit_system::power::operator==(const sakurajin::unit_system::power& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    return value == retval.val();
}

bool sakurajin::unit_system::power::operator!=(const sakurajin::unit_system::power& other) const {
    return !(*this == other);
}

// non const member functions
void sakurajin::unit_system::power::operator*=(long double scalar) {
    value *= scalar;
}

void sakurajin::unit_system::power::operator/=(long double scalar) {
    value /= scalar;
}

void sakurajin::unit_system::power::operator+=(const sakurajin::unit_system::power& other) {
    const auto otherVal = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    value += otherVal.val();
}

void sakurajin::unit_system::power::operator-=(const sakurajin::unit_system::power& other) {
    const auto otherVal = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    value -= otherVal.val();
}

void sakurajin::unit_system::power::operator=(const sakurajin::unit_system::power& other) {
    const auto otherVal = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    value               = otherVal.val();
}


sakurajin::unit_system::speed sakurajin::unit_system::power::operator/(const sakurajin::unit_system::force& other) const {
    sakurajin::unit_system::power _v1 = convert_offset(0);
    sakurajin::unit_system::force _v2 = other.convert_offset(0);
    return sakurajin::unit_system::speed{_v1.val() / _v2.val(), _v1.mult() / _v2.mult()};
}

sakurajin::unit_system::force sakurajin::unit_system::power::operator/(const sakurajin::unit_system::speed& other) const {
    sakurajin::unit_system::power _v1 = convert_offset(0);
    sakurajin::unit_system::speed _v2 = other.convert_offset(0);
    return sakurajin::unit_system::force{_v1.val() / _v2.val(), _v1.mult() / _v2.mult()};
}


sakurajin::unit_system::energy sakurajin::unit_system::power::operator*(const sakurajin::unit_system::time_si& other) const {
    sakurajin::unit_system::power   _v1 = convert_offset(0);
    sakurajin::unit_system::time_si _v2 = other.convert_offset(0);
    return sakurajin::unit_system::energy{_v1.val() * _v2.val(), _v1.mult() * _v2.mult()};
}


// external functions

sakurajin::unit_system::power sakurajin::unit_system::operator*(long double scalar, const sakurajin::unit_system::power& value) {
    return value * scalar;
}


sakurajin::unit_system::power
sakurajin::unit_system::unit_cast(const sakurajin::unit_system::power& unit, long double new_multiplier, long double new_offset) {
    return unit.convert_copy(new_multiplier, new_offset);
}

sakurajin::unit_system::power sakurajin::unit_system::clamp(const sakurajin::unit_system::power& unit,
                                                            const sakurajin::unit_system::power& lower,
                                                            const sakurajin::unit_system::power& upper) {
    auto _lower = unit_cast(lower, unit.mult(), unit.off());
    auto _upper = unit_cast(upper, unit.mult(), unit.off());

    auto val = unit.val() > _lower.val() ? (unit.val() < _upper.val() ? unit.val() : _upper.val()) : _lower.val();
    return sakurajin::unit_system::power{val, unit.mult(), unit.off()};
}


sakurajin::unit_system::power std::abs(const sakurajin::unit_system::power& unit) {
    auto inv = -unit;
    return unit > inv ? unit : inv;
}

std::ostream& sakurajin::unit_system::operator<<(std::ostream& os, const sakurajin::unit_system::power& t) {
    auto t1 = sakurajin::unit_system::unit_cast(t, 1);
    return os << t1.val() << " Watt";
}


// literals


sakurajin::unit_system::power sakurajin::unit_system::literals::operator"" _W(long double len) {
    return sakurajin::unit_system::power{len, 1.0, 0.0};
}

sakurajin::unit_system::power sakurajin::unit_system::literals::operator"" _W(unsigned long long int len) {
    return sakurajin::unit_system::power{static_cast<long double>(len), 1.0, 0.0};
}

sakurajin::unit_system::power sakurajin::unit_system::literals::operator"" _PW(long double len) {
    return sakurajin::unit_system::power{len, 1000000000000000.0, 0.0};
}

sakurajin::unit_system::power sakurajin::unit_system::literals::operator"" _PW(unsigned long long int len) {
    return sakurajin::unit_system::power{static_cast<long double>(len), 1000000000000000.0, 0.0};
}

sakurajin::unit_system::power sakurajin::unit_system::literals::operator"" _TW(long double len) {
    return sakurajin::unit_system::power{len, 1000000000000.0, 0.0};
}

sakurajin::unit_system::power sakurajin::unit_system::literals::operator"" _TW(unsigned long long int len) {
    return sakurajin::unit_system::power{static_cast<long double>(len), 1000000000000.0, 0.0};
}

sakurajin::unit_system::power sakurajin::unit_system::literals::operator"" _GW(long double len) {
    return sakurajin::unit_system::power{len, 1000000000.0, 0.0};
}

sakurajin::unit_system::power sakurajin::unit_system::literals::operator"" _GW(unsigned long long int len) {
    return sakurajin::unit_system::power{static_cast<long double>(len), 1000000000.0, 0.0};
}

sakurajin::unit_system::power sakurajin::unit_system::literals::operator"" _MW(long double len) {
    return sakurajin::unit_system::power{len, 1000000.0, 0.0};
}

sakurajin::unit_system::power sakurajin::unit_system::literals::operator"" _MW(unsigned long long int len) {
    return sakurajin::unit_system::power{static_cast<long double>(len), 1000000.0, 0.0};
}

sakurajin::unit_system::power sakurajin::unit_system::literals::operator"" _kW(long double len) {
    return sakurajin::unit_system::power{len, 1000.0, 0.0};
}

sakurajin::unit_system::power sakurajin::unit_system::literals::operator"" _kW(unsigned long long int len) {
    return sakurajin::unit_system::power{static_cast<long double>(len), 1000.0, 0.0};
}

sakurajin::unit_system::power sakurajin::unit_system::literals::operator"" _mW(long double len) {
    return sakurajin::unit_system::power{len, 0.001, 0.0};
}

sakurajin::unit_system::power sakurajin::unit_system::literals::operator"" _mW(unsigned long long int len) {
    return sakurajin::unit_system::power{static_cast<long double>(len), 0.001, 0.0};
}

sakurajin::unit_system::power sakurajin::unit_system::literals::operator"" _uW(long double len) {
    return sakurajin::unit_system::power{len, 1e-06, 0.0};
}

sakurajin::unit_system::power sakurajin::unit_system::literals::operator"" _uW(unsigned long long int len) {
    return sakurajin::unit_system::power{static_cast<long double>(len), 1e-06, 0.0};
}

sakurajin::unit_system::power sakurajin::unit_system::literals::operator"" _nW(long double len) {
    return sakurajin::unit_system::power{len, 1e-09, 0.0};
}

sakurajin::unit_system::power sakurajin::unit_system::literals::operator"" _nW(unsigned long long int len) {
    return sakurajin::unit_system::power{static_cast<long double>(len), 1e-09, 0.0};
}

sakurajin::unit_system::power sakurajin::unit_system::literals::operator"" _pW(long double len) {
    return sakurajin::unit_system::power{len, 1e-12, 0.0};
}

sakurajin::unit_system::power sakurajin::unit_system::literals::operator"" _pW(unsigned long long int len) {
    return sakurajin::unit_system::power{static_cast<long double>(len), 1e-12, 0.0};
}

sakurajin::unit_system::power sakurajin::unit_system::literals::operator"" _fW(long double len) {
    return sakurajin::unit_system::power{len, 1e-15, 0.0};
}

sakurajin::unit_system::power sakurajin::unit_system::literals::operator"" _fW(unsigned long long int len) {
    return sakurajin::unit_system::power{static_cast<long double>(len), 1e-15, 0.0};
}

sakurajin::unit_system::power sakurajin::unit_system::literals::operator"" _aW(long double len) {
    return sakurajin::unit_system::power{len, 1e-18, 0.0};
}

sakurajin::unit_system::power sakurajin::unit_system::literals::operator"" _aW(unsigned long long int len) {
    return sakurajin::unit_system::power{static_cast<long double>(len), 1e-18, 0.0};
}
