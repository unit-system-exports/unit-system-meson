#include "cmath"
#include "unit_system.hpp"

sakurajin::unit_system::length::length()
    : length{0.0} {}
sakurajin::unit_system::length::length(long double v)
    : length{v, 1, 0} {}
sakurajin::unit_system::length::length(long double v, long double mult)
    : length{v, mult, 0} {}
sakurajin::unit_system::length::length(long double v, long double mult, long double off)
    : value{v},
      multiplier{mult},
      offset{off} {}


long double sakurajin::unit_system::length::mult() const {
    return multiplier;
}
long double sakurajin::unit_system::length::val() const {
    return value;
}
long double sakurajin::unit_system::length::off() const {
    return offset;
}
long double sakurajin::unit_system::length::rel_err() const {
    return rel_error;
}

long double& sakurajin::unit_system::length::mult() {
    return multiplier;
}
long double& sakurajin::unit_system::length::val() {
    return value;
}
long double& sakurajin::unit_system::length::off() {
    return offset;
}
long double& sakurajin::unit_system::length::rel_err() {
    return rel_error;
}

// const functions
sakurajin::unit_system::length sakurajin::unit_system::length::operator*(long double scalar) const {
    return sakurajin::unit_system::length{value * scalar, multiplier, offset};
}

sakurajin::unit_system::length operator*(long double scalar, const sakurajin::unit_system::length& val) {
    return sakurajin::unit_system::length{val.val() * scalar, val.mult(), val.off()};
}

long double sakurajin::unit_system::length::operator/(const sakurajin::unit_system::length& other) const {
    return value / other.convert_like(*this).val();
}

sakurajin::unit_system::length sakurajin::unit_system::length::operator/(long double scalar) const {
    return sakurajin::unit_system::length{value / scalar, multiplier, offset};
}

sakurajin::unit_system::length sakurajin::unit_system::length::operator+(const sakurajin::unit_system::length& other) const {
    auto retval = convert_like(other);
    retval.val() += other.val();
    return retval;
}

sakurajin::unit_system::length sakurajin::unit_system::length::operator-(const sakurajin::unit_system::length& other) const {
    auto retval = convert_like(other);
    retval.val() -= other.val();
    return retval;
}

sakurajin::unit_system::length sakurajin::unit_system::length::operator-() const {
    return sakurajin::unit_system::length{-value, multiplier, offset};
}

sakurajin::unit_system::length::operator long double() const {
    return convert_copy(1, 0).val();
}

sakurajin::unit_system::length sakurajin::unit_system::length::convert_multiplier(long double new_multiplier) const {
    return convert_copy(new_multiplier, offset);
}

sakurajin::unit_system::length sakurajin::unit_system::length::convert_offset(long double new_offset) const {
    return convert_copy(multiplier, new_offset);
}

sakurajin::unit_system::length sakurajin::unit_system::length::convert_copy(long double new_multiplier, long double new_offset) const {
    auto                           valBase0 = value * multiplier + offset;
    sakurajin::unit_system::length retval{valBase0 / new_multiplier - new_offset, new_multiplier, new_offset};
    return retval;
}

sakurajin::unit_system::length sakurajin::unit_system::length::convert_like(const sakurajin::unit_system::length& other) const {
    return convert_copy(other.multiplier, other.offset);
}

// comparison operators
#if __cplusplus >= 202002L
int sakurajin::unit_system::length::operator<=>(const sakurajin::unit_system::length& other) const {
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
bool sakurajin::unit_system::length::operator<(const sakurajin::unit_system::length& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    return value < retval.val();
}

bool sakurajin::unit_system::length::operator>(const sakurajin::unit_system::length& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    return value > retval.val();
}

bool sakurajin::unit_system::length::operator<=(const sakurajin::unit_system::length& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    return value <= retval.val();
}

bool sakurajin::unit_system::length::operator>=(const sakurajin::unit_system::length& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    return value >= retval.val();
}

bool sakurajin::unit_system::length::operator==(const sakurajin::unit_system::length& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    return value == retval.val();
}

bool sakurajin::unit_system::length::operator!=(const sakurajin::unit_system::length& other) const {
    return !(*this == other);
}

// non const member functions
void sakurajin::unit_system::length::operator*=(long double scalar) {
    value *= scalar;
}

void sakurajin::unit_system::length::operator/=(long double scalar) {
    value /= scalar;
}

void sakurajin::unit_system::length::operator+=(const sakurajin::unit_system::length& other) {
    const auto otherVal = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    value += otherVal.val();
}

void sakurajin::unit_system::length::operator-=(const sakurajin::unit_system::length& other) {
    const auto otherVal = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    value -= otherVal.val();
}

void sakurajin::unit_system::length::operator=(const sakurajin::unit_system::length& other) {
    const auto otherVal = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    value               = otherVal.val();
}


sakurajin::unit_system::time_si sakurajin::unit_system::length::operator/(const sakurajin::unit_system::speed& other) const {
    sakurajin::unit_system::length _v1 = convert_offset(0);
    sakurajin::unit_system::speed  _v2 = other.convert_offset(0);
    return sakurajin::unit_system::time_si{_v1.val() / _v2.val(), _v1.mult() / _v2.mult()};
}

sakurajin::unit_system::speed sakurajin::unit_system::length::operator/(const sakurajin::unit_system::time_si& other) const {
    sakurajin::unit_system::length  _v1 = convert_offset(0);
    sakurajin::unit_system::time_si _v2 = other.convert_offset(0);
    return sakurajin::unit_system::speed{_v1.val() / _v2.val(), _v1.mult() / _v2.mult()};
}


sakurajin::unit_system::area sakurajin::unit_system::length::operator*(const sakurajin::unit_system::length& other) const {
    sakurajin::unit_system::length _v1 = convert_offset(0);
    sakurajin::unit_system::length _v2 = other.convert_offset(0);
    return sakurajin::unit_system::area{_v1.val() * _v2.val(), _v1.mult() * _v2.mult()};
}

sakurajin::unit_system::energy sakurajin::unit_system::length::operator*(const sakurajin::unit_system::force& other) const {
    sakurajin::unit_system::length _v1 = convert_offset(0);
    sakurajin::unit_system::force  _v2 = other.convert_offset(0);
    return sakurajin::unit_system::energy{_v1.val() * _v2.val(), _v1.mult() * _v2.mult()};
}


sakurajin::unit_system::area sakurajin::unit_system::length::square() const {
    return sakurajin::unit_system::area{value * value, multiplier * multiplier, offset};
}


// external functions

sakurajin::unit_system::length sakurajin::unit_system::operator*(long double scalar, const sakurajin::unit_system::length& value) {
    return value * scalar;
}


sakurajin::unit_system::area sakurajin::unit_system::square(const length& unit) {
    return unit.square();
}


sakurajin::unit_system::length
sakurajin::unit_system::unit_cast(const sakurajin::unit_system::length& unit, long double new_multiplier, long double new_offset) {
    return unit.convert_copy(new_multiplier, new_offset);
}

sakurajin::unit_system::length sakurajin::unit_system::clamp(const sakurajin::unit_system::length& unit,
                                                             const sakurajin::unit_system::length& lower,
                                                             const sakurajin::unit_system::length& upper) {
    auto _lower = unit_cast(lower, unit.mult(), unit.off());
    auto _upper = unit_cast(upper, unit.mult(), unit.off());

    auto val = unit.val() > _lower.val() ? (unit.val() < _upper.val() ? unit.val() : _upper.val()) : _lower.val();
    return sakurajin::unit_system::length{val, unit.mult(), unit.off()};
}


sakurajin::unit_system::length std::abs(const sakurajin::unit_system::length& unit) {
    auto inv = -unit;
    return unit > inv ? unit : inv;
}

std::ostream& sakurajin::unit_system::operator<<(std::ostream& os, const sakurajin::unit_system::length& t) {
    auto t1 = sakurajin::unit_system::unit_cast(t, 1);
    return os << t1.val() << " metre";
}


// literals


sakurajin::unit_system::length sakurajin::unit_system::literals::operator"" _m(long double len) {
    return sakurajin::unit_system::length{len, 1.0, 0.0};
}

sakurajin::unit_system::length sakurajin::unit_system::literals::operator"" _m(unsigned long long int len) {
    return sakurajin::unit_system::length{static_cast<long double>(len), 1.0, 0.0};
}

sakurajin::unit_system::length sakurajin::unit_system::literals::operator"" _km(long double len) {
    return sakurajin::unit_system::length{len, 1000.0, 0.0};
}

sakurajin::unit_system::length sakurajin::unit_system::literals::operator"" _km(unsigned long long int len) {
    return sakurajin::unit_system::length{static_cast<long double>(len), 1000.0, 0.0};
}

sakurajin::unit_system::length sakurajin::unit_system::literals::operator"" _dm(long double len) {
    return sakurajin::unit_system::length{len, 0.1, 0.0};
}

sakurajin::unit_system::length sakurajin::unit_system::literals::operator"" _dm(unsigned long long int len) {
    return sakurajin::unit_system::length{static_cast<long double>(len), 0.1, 0.0};
}

sakurajin::unit_system::length sakurajin::unit_system::literals::operator"" _cm(long double len) {
    return sakurajin::unit_system::length{len, 0.01, 0.0};
}

sakurajin::unit_system::length sakurajin::unit_system::literals::operator"" _cm(unsigned long long int len) {
    return sakurajin::unit_system::length{static_cast<long double>(len), 0.01, 0.0};
}

sakurajin::unit_system::length sakurajin::unit_system::literals::operator"" _mm(long double len) {
    return sakurajin::unit_system::length{len, 0.001, 0.0};
}

sakurajin::unit_system::length sakurajin::unit_system::literals::operator"" _mm(unsigned long long int len) {
    return sakurajin::unit_system::length{static_cast<long double>(len), 0.001, 0.0};
}

sakurajin::unit_system::length sakurajin::unit_system::literals::operator"" _um(long double len) {
    return sakurajin::unit_system::length{len, 1e-06, 0.0};
}

sakurajin::unit_system::length sakurajin::unit_system::literals::operator"" _um(unsigned long long int len) {
    return sakurajin::unit_system::length{static_cast<long double>(len), 1e-06, 0.0};
}

sakurajin::unit_system::length sakurajin::unit_system::literals::operator"" _nm(long double len) {
    return sakurajin::unit_system::length{len, 1e-09, 0.0};
}

sakurajin::unit_system::length sakurajin::unit_system::literals::operator"" _nm(unsigned long long int len) {
    return sakurajin::unit_system::length{static_cast<long double>(len), 1e-09, 0.0};
}

sakurajin::unit_system::length sakurajin::unit_system::literals::operator"" _pm(long double len) {
    return sakurajin::unit_system::length{len, 1e-12, 0.0};
}

sakurajin::unit_system::length sakurajin::unit_system::literals::operator"" _pm(unsigned long long int len) {
    return sakurajin::unit_system::length{static_cast<long double>(len), 1e-12, 0.0};
}

sakurajin::unit_system::length sakurajin::unit_system::literals::operator"" _fm(long double len) {
    return sakurajin::unit_system::length{len, 1e-15, 0.0};
}

sakurajin::unit_system::length sakurajin::unit_system::literals::operator"" _fm(unsigned long long int len) {
    return sakurajin::unit_system::length{static_cast<long double>(len), 1e-15, 0.0};
}

sakurajin::unit_system::length sakurajin::unit_system::literals::operator"" _am(long double len) {
    return sakurajin::unit_system::length{len, 1e-18, 0.0};
}

sakurajin::unit_system::length sakurajin::unit_system::literals::operator"" _am(unsigned long long int len) {
    return sakurajin::unit_system::length{static_cast<long double>(len), 1e-18, 0.0};
}
