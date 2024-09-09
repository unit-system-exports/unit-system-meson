#include "cmath"
#include "unit_system.hpp"

sakurajin::unit_system::force::force()
    : force{0.0} {}
sakurajin::unit_system::force::force(long double v)
    : force{v, 1, 0} {}
sakurajin::unit_system::force::force(long double v, long double mult)
    : force{v, mult, 0} {}
sakurajin::unit_system::force::force(long double v, long double mult, long double off)
    : value{v},
      multiplier{mult},
      offset{off} {}


long double sakurajin::unit_system::force::mult() const {
    return multiplier;
}
long double sakurajin::unit_system::force::val() const {
    return value;
}
long double sakurajin::unit_system::force::off() const {
    return offset;
}
long double sakurajin::unit_system::force::rel_err() const {
    return rel_error;
}

long double& sakurajin::unit_system::force::mult() {
    return multiplier;
}
long double& sakurajin::unit_system::force::val() {
    return value;
}
long double& sakurajin::unit_system::force::off() {
    return offset;
}
long double& sakurajin::unit_system::force::rel_err() {
    return rel_error;
}

// const functions
sakurajin::unit_system::force sakurajin::unit_system::force::operator*(long double scalar) const {
    return sakurajin::unit_system::force{value * scalar, multiplier, offset};
}

sakurajin::unit_system::force operator*(long double scalar, const sakurajin::unit_system::force& val) {
    return sakurajin::unit_system::force{val.val() * scalar, val.mult(), val.off()};
}

long double sakurajin::unit_system::force::operator/(const sakurajin::unit_system::force& other) const {
    return value / other.convert_like(*this).val();
}

sakurajin::unit_system::force sakurajin::unit_system::force::operator/(long double scalar) const {
    return sakurajin::unit_system::force{value / scalar, multiplier, offset};
}

sakurajin::unit_system::force sakurajin::unit_system::force::operator+(const sakurajin::unit_system::force& other) const {
    auto retval = convert_like(other);
    retval.val() += other.val();
    return retval;
}

sakurajin::unit_system::force sakurajin::unit_system::force::operator-(const sakurajin::unit_system::force& other) const {
    auto retval = convert_like(other);
    retval.val() -= other.val();
    return retval;
}

sakurajin::unit_system::force sakurajin::unit_system::force::operator-() const {
    return sakurajin::unit_system::force{-value, multiplier, offset};
}

sakurajin::unit_system::force::operator long double() const {
    return convert_copy(1, 0).val();
}

sakurajin::unit_system::force sakurajin::unit_system::force::convert_multiplier(long double new_multiplier) const {
    return convert_copy(new_multiplier, offset);
}

sakurajin::unit_system::force sakurajin::unit_system::force::convert_offset(long double new_offset) const {
    return convert_copy(multiplier, new_offset);
}

sakurajin::unit_system::force sakurajin::unit_system::force::convert_copy(long double new_multiplier, long double new_offset) const {
    auto                          valBase0 = value * multiplier + offset;
    sakurajin::unit_system::force retval{valBase0 / new_multiplier - new_offset, new_multiplier, new_offset};
    return retval;
}

sakurajin::unit_system::force sakurajin::unit_system::force::convert_like(const sakurajin::unit_system::force& other) const {
    return convert_copy(other.multiplier, other.offset);
}

// comparison operators
#if __cplusplus >= 202002L
int sakurajin::unit_system::force::operator<=>(const sakurajin::unit_system::force& other) const {
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
bool sakurajin::unit_system::force::operator<(const sakurajin::unit_system::force& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    return value < retval.val();
}

bool sakurajin::unit_system::force::operator>(const sakurajin::unit_system::force& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    return value > retval.val();
}

bool sakurajin::unit_system::force::operator<=(const sakurajin::unit_system::force& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    return value <= retval.val();
}

bool sakurajin::unit_system::force::operator>=(const sakurajin::unit_system::force& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    return value >= retval.val();
}

bool sakurajin::unit_system::force::operator==(const sakurajin::unit_system::force& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    return value == retval.val();
}

bool sakurajin::unit_system::force::operator!=(const sakurajin::unit_system::force& other) const {
    return !(*this == other);
}

// non const member functions
void sakurajin::unit_system::force::operator*=(long double scalar) {
    value *= scalar;
}

void sakurajin::unit_system::force::operator/=(long double scalar) {
    value /= scalar;
}

void sakurajin::unit_system::force::operator+=(const sakurajin::unit_system::force& other) {
    const auto otherVal = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    value += otherVal.val();
}

void sakurajin::unit_system::force::operator-=(const sakurajin::unit_system::force& other) {
    const auto otherVal = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    value -= otherVal.val();
}

void sakurajin::unit_system::force::operator=(const sakurajin::unit_system::force& other) {
    const auto otherVal = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    value               = otherVal.val();
}


sakurajin::unit_system::acceleration sakurajin::unit_system::force::operator/(const sakurajin::unit_system::mass& other) const {
    sakurajin::unit_system::force _v1 = convert_offset(0);
    sakurajin::unit_system::mass  _v2 = other.convert_offset(0);
    return sakurajin::unit_system::acceleration{_v1.val() / _v2.val(), _v1.mult() / _v2.mult()};
}

sakurajin::unit_system::mass sakurajin::unit_system::force::operator/(const sakurajin::unit_system::acceleration& other) const {
    sakurajin::unit_system::force        _v1 = convert_offset(0);
    sakurajin::unit_system::acceleration _v2 = other.convert_offset(0);
    return sakurajin::unit_system::mass{_v1.val() / _v2.val(), _v1.mult() / _v2.mult()};
}


sakurajin::unit_system::energy sakurajin::unit_system::force::operator*(const sakurajin::unit_system::length& other) const {
    sakurajin::unit_system::force  _v1 = convert_offset(0);
    sakurajin::unit_system::length _v2 = other.convert_offset(0);
    return sakurajin::unit_system::energy{_v1.val() * _v2.val(), _v1.mult() * _v2.mult()};
}

sakurajin::unit_system::momentum sakurajin::unit_system::force::operator*(const sakurajin::unit_system::time_si& other) const {
    sakurajin::unit_system::force   _v1 = convert_offset(0);
    sakurajin::unit_system::time_si _v2 = other.convert_offset(0);
    return sakurajin::unit_system::momentum{_v1.val() * _v2.val(), _v1.mult() * _v2.mult()};
}

sakurajin::unit_system::power sakurajin::unit_system::force::operator*(const sakurajin::unit_system::speed& other) const {
    sakurajin::unit_system::force _v1 = convert_offset(0);
    sakurajin::unit_system::speed _v2 = other.convert_offset(0);
    return sakurajin::unit_system::power{_v1.val() * _v2.val(), _v1.mult() * _v2.mult()};
}


// external functions

sakurajin::unit_system::force sakurajin::unit_system::operator*(long double scalar, const sakurajin::unit_system::force& value) {
    return value * scalar;
}


sakurajin::unit_system::force
sakurajin::unit_system::unit_cast(const sakurajin::unit_system::force& unit, long double new_multiplier, long double new_offset) {
    return unit.convert_copy(new_multiplier, new_offset);
}

sakurajin::unit_system::force sakurajin::unit_system::clamp(const sakurajin::unit_system::force& unit,
                                                            const sakurajin::unit_system::force& lower,
                                                            const sakurajin::unit_system::force& upper) {
    auto _lower = unit_cast(lower, unit.mult(), unit.off());
    auto _upper = unit_cast(upper, unit.mult(), unit.off());

    auto val = unit.val() > _lower.val() ? (unit.val() < _upper.val() ? unit.val() : _upper.val()) : _lower.val();
    return sakurajin::unit_system::force{val, unit.mult(), unit.off()};
}


sakurajin::unit_system::force std::abs(const sakurajin::unit_system::force& unit) {
    auto inv = -unit;
    return unit > inv ? unit : inv;
}

std::ostream& sakurajin::unit_system::operator<<(std::ostream& os, const sakurajin::unit_system::force& t) {
    auto t1 = sakurajin::unit_system::unit_cast(t, 1);
    return os << t1.val() << " Newton";
}


// literals


sakurajin::unit_system::force sakurajin::unit_system::literals::operator"" _N(long double len) {
    return sakurajin::unit_system::force{len, 1.0, 0.0};
}

sakurajin::unit_system::force sakurajin::unit_system::literals::operator"" _N(unsigned long long int len) {
    return sakurajin::unit_system::force{static_cast<long double>(len), 1.0, 0.0};
}

sakurajin::unit_system::force sakurajin::unit_system::literals::operator"" _PN(long double len) {
    return sakurajin::unit_system::force{len, 1000000000000000.0, 0.0};
}

sakurajin::unit_system::force sakurajin::unit_system::literals::operator"" _PN(unsigned long long int len) {
    return sakurajin::unit_system::force{static_cast<long double>(len), 1000000000000000.0, 0.0};
}

sakurajin::unit_system::force sakurajin::unit_system::literals::operator"" _TN(long double len) {
    return sakurajin::unit_system::force{len, 1000000000000.0, 0.0};
}

sakurajin::unit_system::force sakurajin::unit_system::literals::operator"" _TN(unsigned long long int len) {
    return sakurajin::unit_system::force{static_cast<long double>(len), 1000000000000.0, 0.0};
}

sakurajin::unit_system::force sakurajin::unit_system::literals::operator"" _GN(long double len) {
    return sakurajin::unit_system::force{len, 1000000000.0, 0.0};
}

sakurajin::unit_system::force sakurajin::unit_system::literals::operator"" _GN(unsigned long long int len) {
    return sakurajin::unit_system::force{static_cast<long double>(len), 1000000000.0, 0.0};
}

sakurajin::unit_system::force sakurajin::unit_system::literals::operator"" _MN(long double len) {
    return sakurajin::unit_system::force{len, 1000000.0, 0.0};
}

sakurajin::unit_system::force sakurajin::unit_system::literals::operator"" _MN(unsigned long long int len) {
    return sakurajin::unit_system::force{static_cast<long double>(len), 1000000.0, 0.0};
}

sakurajin::unit_system::force sakurajin::unit_system::literals::operator"" _kN(long double len) {
    return sakurajin::unit_system::force{len, 1000.0, 0.0};
}

sakurajin::unit_system::force sakurajin::unit_system::literals::operator"" _kN(unsigned long long int len) {
    return sakurajin::unit_system::force{static_cast<long double>(len), 1000.0, 0.0};
}

sakurajin::unit_system::force sakurajin::unit_system::literals::operator"" _mN(long double len) {
    return sakurajin::unit_system::force{len, 0.001, 0.0};
}

sakurajin::unit_system::force sakurajin::unit_system::literals::operator"" _mN(unsigned long long int len) {
    return sakurajin::unit_system::force{static_cast<long double>(len), 0.001, 0.0};
}

sakurajin::unit_system::force sakurajin::unit_system::literals::operator"" _uN(long double len) {
    return sakurajin::unit_system::force{len, 1e-06, 0.0};
}

sakurajin::unit_system::force sakurajin::unit_system::literals::operator"" _uN(unsigned long long int len) {
    return sakurajin::unit_system::force{static_cast<long double>(len), 1e-06, 0.0};
}

sakurajin::unit_system::force sakurajin::unit_system::literals::operator"" _nN(long double len) {
    return sakurajin::unit_system::force{len, 1e-09, 0.0};
}

sakurajin::unit_system::force sakurajin::unit_system::literals::operator"" _nN(unsigned long long int len) {
    return sakurajin::unit_system::force{static_cast<long double>(len), 1e-09, 0.0};
}

sakurajin::unit_system::force sakurajin::unit_system::literals::operator"" _pN(long double len) {
    return sakurajin::unit_system::force{len, 1e-12, 0.0};
}

sakurajin::unit_system::force sakurajin::unit_system::literals::operator"" _pN(unsigned long long int len) {
    return sakurajin::unit_system::force{static_cast<long double>(len), 1e-12, 0.0};
}

sakurajin::unit_system::force sakurajin::unit_system::literals::operator"" _fN(long double len) {
    return sakurajin::unit_system::force{len, 1e-15, 0.0};
}

sakurajin::unit_system::force sakurajin::unit_system::literals::operator"" _fN(unsigned long long int len) {
    return sakurajin::unit_system::force{static_cast<long double>(len), 1e-15, 0.0};
}

sakurajin::unit_system::force sakurajin::unit_system::literals::operator"" _aN(long double len) {
    return sakurajin::unit_system::force{len, 1e-18, 0.0};
}

sakurajin::unit_system::force sakurajin::unit_system::literals::operator"" _aN(unsigned long long int len) {
    return sakurajin::unit_system::force{static_cast<long double>(len), 1e-18, 0.0};
}
