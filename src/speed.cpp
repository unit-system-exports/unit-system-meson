#include "cmath"
#include "unit_system.hpp"

sakurajin::unit_system::speed::speed()
    : speed{0.0} {}
sakurajin::unit_system::speed::speed(long double v)
    : speed{v, 1, 0} {}
sakurajin::unit_system::speed::speed(long double v, long double mult)
    : speed{v, mult, 0} {}
sakurajin::unit_system::speed::speed(long double v, long double mult, long double off)
    : value{v},
      multiplier{mult},
      offset{off} {}


long double sakurajin::unit_system::speed::mult() const {
    return multiplier;
}
long double sakurajin::unit_system::speed::val() const {
    return value;
}
long double sakurajin::unit_system::speed::off() const {
    return offset;
}
long double sakurajin::unit_system::speed::rel_err() const {
    return rel_error;
}

long double& sakurajin::unit_system::speed::mult() {
    return multiplier;
}
long double& sakurajin::unit_system::speed::val() {
    return value;
}
long double& sakurajin::unit_system::speed::off() {
    return offset;
}
long double& sakurajin::unit_system::speed::rel_err() {
    return rel_error;
}

// const functions
sakurajin::unit_system::speed sakurajin::unit_system::speed::operator*(long double scalar) const {
    return sakurajin::unit_system::speed{value * scalar, multiplier, offset};
}

sakurajin::unit_system::speed operator*(long double scalar, const sakurajin::unit_system::speed& val) {
    return sakurajin::unit_system::speed{val.val() * scalar, val.mult(), val.off()};
}

long double sakurajin::unit_system::speed::operator/(const sakurajin::unit_system::speed& other) const {
    return value / other.convert_like(*this).val();
}

sakurajin::unit_system::speed sakurajin::unit_system::speed::operator/(long double scalar) const {
    return sakurajin::unit_system::speed{value / scalar, multiplier, offset};
}

sakurajin::unit_system::speed sakurajin::unit_system::speed::operator+(const sakurajin::unit_system::speed& other) const {
    auto retval = convert_like(other);
    retval.val() += other.val();
    return retval;
}

sakurajin::unit_system::speed sakurajin::unit_system::speed::operator-(const sakurajin::unit_system::speed& other) const {
    auto retval = convert_like(other);
    retval.val() -= other.val();
    return retval;
}

sakurajin::unit_system::speed sakurajin::unit_system::speed::operator-() const {
    return sakurajin::unit_system::speed{-value, multiplier, offset};
}

sakurajin::unit_system::speed::operator long double() const {
    return convert_copy(1, 0).val();
}

sakurajin::unit_system::speed sakurajin::unit_system::speed::convert_multiplier(long double new_multiplier) const {
    return convert_copy(new_multiplier, offset);
}

sakurajin::unit_system::speed sakurajin::unit_system::speed::convert_offset(long double new_offset) const {
    return convert_copy(multiplier, new_offset);
}

sakurajin::unit_system::speed sakurajin::unit_system::speed::convert_copy(long double new_multiplier, long double new_offset) const {
    auto                          valBase0 = value * multiplier + offset;
    sakurajin::unit_system::speed retval{valBase0 / new_multiplier - new_offset, new_multiplier, new_offset};
    return retval;
}

sakurajin::unit_system::speed sakurajin::unit_system::speed::convert_like(const sakurajin::unit_system::speed& other) const {
    return convert_copy(other.multiplier, other.offset);
}

// comparison operators
#if __cplusplus >= 202002L
int sakurajin::unit_system::speed::operator<=>(const sakurajin::unit_system::speed& other) const {
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
bool sakurajin::unit_system::speed::operator<(const sakurajin::unit_system::speed& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    return value < retval.val();
}

bool sakurajin::unit_system::speed::operator>(const sakurajin::unit_system::speed& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    return value > retval.val();
}

bool sakurajin::unit_system::speed::operator<=(const sakurajin::unit_system::speed& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    return value <= retval.val();
}

bool sakurajin::unit_system::speed::operator>=(const sakurajin::unit_system::speed& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    return value >= retval.val();
}

bool sakurajin::unit_system::speed::operator==(const sakurajin::unit_system::speed& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    return value == retval.val();
}

bool sakurajin::unit_system::speed::operator!=(const sakurajin::unit_system::speed& other) const {
    return !(*this == other);
}

// non const member functions
void sakurajin::unit_system::speed::operator*=(long double scalar) {
    value *= scalar;
}

void sakurajin::unit_system::speed::operator/=(long double scalar) {
    value /= scalar;
}

void sakurajin::unit_system::speed::operator+=(const sakurajin::unit_system::speed& other) {
    const auto otherVal = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    value += otherVal.val();
}

void sakurajin::unit_system::speed::operator-=(const sakurajin::unit_system::speed& other) {
    const auto otherVal = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    value -= otherVal.val();
}

void sakurajin::unit_system::speed::operator=(const sakurajin::unit_system::speed& other) {
    const auto otherVal = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    value               = otherVal.val();
}


sakurajin::unit_system::time_si sakurajin::unit_system::speed::operator/(const sakurajin::unit_system::acceleration& other) const {
    sakurajin::unit_system::speed        _v1 = convert_offset(0);
    sakurajin::unit_system::acceleration _v2 = other.convert_offset(0);
    return sakurajin::unit_system::time_si{_v1.val() / _v2.val(), _v1.mult() / _v2.mult()};
}

sakurajin::unit_system::acceleration sakurajin::unit_system::speed::operator/(const sakurajin::unit_system::time_si& other) const {
    sakurajin::unit_system::speed   _v1 = convert_offset(0);
    sakurajin::unit_system::time_si _v2 = other.convert_offset(0);
    return sakurajin::unit_system::acceleration{_v1.val() / _v2.val(), _v1.mult() / _v2.mult()};
}


sakurajin::unit_system::length sakurajin::unit_system::speed::operator*(const sakurajin::unit_system::time_si& other) const {
    sakurajin::unit_system::speed   _v1 = convert_offset(0);
    sakurajin::unit_system::time_si _v2 = other.convert_offset(0);
    return sakurajin::unit_system::length{_v1.val() * _v2.val(), _v1.mult() * _v2.mult()};
}

sakurajin::unit_system::energy sakurajin::unit_system::speed::operator*(const sakurajin::unit_system::momentum& other) const {
    sakurajin::unit_system::speed    _v1 = convert_offset(0);
    sakurajin::unit_system::momentum _v2 = other.convert_offset(0);
    return sakurajin::unit_system::energy{_v1.val() * _v2.val(), _v1.mult() * _v2.mult()};
}

sakurajin::unit_system::momentum sakurajin::unit_system::speed::operator*(const sakurajin::unit_system::mass& other) const {
    sakurajin::unit_system::speed _v1 = convert_offset(0);
    sakurajin::unit_system::mass  _v2 = other.convert_offset(0);
    return sakurajin::unit_system::momentum{_v1.val() * _v2.val(), _v1.mult() * _v2.mult()};
}

sakurajin::unit_system::power sakurajin::unit_system::speed::operator*(const sakurajin::unit_system::force& other) const {
    sakurajin::unit_system::speed _v1 = convert_offset(0);
    sakurajin::unit_system::force _v2 = other.convert_offset(0);
    return sakurajin::unit_system::power{_v1.val() * _v2.val(), _v1.mult() * _v2.mult()};
}


// external functions

sakurajin::unit_system::speed sakurajin::unit_system::operator*(long double scalar, const sakurajin::unit_system::speed& value) {
    return value * scalar;
}


sakurajin::unit_system::speed
sakurajin::unit_system::unit_cast(const sakurajin::unit_system::speed& unit, long double new_multiplier, long double new_offset) {
    return unit.convert_copy(new_multiplier, new_offset);
}

sakurajin::unit_system::speed sakurajin::unit_system::clamp(const sakurajin::unit_system::speed& unit,
                                                            const sakurajin::unit_system::speed& lower,
                                                            const sakurajin::unit_system::speed& upper) {
    auto _lower = unit_cast(lower, unit.mult(), unit.off());
    auto _upper = unit_cast(upper, unit.mult(), unit.off());

    auto val = unit.val() > _lower.val() ? (unit.val() < _upper.val() ? unit.val() : _upper.val()) : _lower.val();
    return sakurajin::unit_system::speed{val, unit.mult(), unit.off()};
}


sakurajin::unit_system::speed std::abs(const sakurajin::unit_system::speed& unit) {
    auto inv = -unit;
    return unit > inv ? unit : inv;
}

std::ostream& sakurajin::unit_system::operator<<(std::ostream& os, const sakurajin::unit_system::speed& t) {
    auto t1 = sakurajin::unit_system::unit_cast(t, 1);
    return os << t1.val() << " metre per second";
}


// literals


sakurajin::unit_system::speed sakurajin::unit_system::literals::operator"" _mps(long double len) {
    return sakurajin::unit_system::speed{len, 1.0, 0.0};
}

sakurajin::unit_system::speed sakurajin::unit_system::literals::operator"" _mps(unsigned long long int len) {
    return sakurajin::unit_system::speed{static_cast<long double>(len), 1.0, 0.0};
}

sakurajin::unit_system::speed sakurajin::unit_system::literals::operator"" _kmph(long double len) {
    return sakurajin::unit_system::speed{len, 0.2777777777777778, 0.0};
}

sakurajin::unit_system::speed sakurajin::unit_system::literals::operator"" _kmph(unsigned long long int len) {
    return sakurajin::unit_system::speed{static_cast<long double>(len), 0.2777777777777778, 0.0};
}
