#include "cmath"
#include "unit_system.hpp"

sakurajin::unit_system::time_si::time_si()
    : time_si{0.0} {}
sakurajin::unit_system::time_si::time_si(long double v)
    : time_si{v, 1, 0} {}
sakurajin::unit_system::time_si::time_si(long double v, long double mult)
    : time_si{v, mult, 0} {}
sakurajin::unit_system::time_si::time_si(long double v, long double mult, long double off)
    : value{v},
      multiplier{mult},
      offset{off} {}


long double sakurajin::unit_system::time_si::mult() const {
    return multiplier;
}
long double sakurajin::unit_system::time_si::val() const {
    return value;
}
long double sakurajin::unit_system::time_si::off() const {
    return offset;
}
long double sakurajin::unit_system::time_si::rel_err() const {
    return rel_error;
}

long double& sakurajin::unit_system::time_si::mult() {
    return multiplier;
}
long double& sakurajin::unit_system::time_si::val() {
    return value;
}
long double& sakurajin::unit_system::time_si::off() {
    return offset;
}
long double& sakurajin::unit_system::time_si::rel_err() {
    return rel_error;
}

// const functions
sakurajin::unit_system::time_si sakurajin::unit_system::time_si::operator*(long double scalar) const {
    return sakurajin::unit_system::time_si{value * scalar, multiplier, offset};
}

sakurajin::unit_system::time_si operator*(long double scalar, const sakurajin::unit_system::time_si& val) {
    return sakurajin::unit_system::time_si{val.val() * scalar, val.mult(), val.off()};
}

long double sakurajin::unit_system::time_si::operator/(const sakurajin::unit_system::time_si& other) const {
    return value / other.convert_like(*this).val();
}

sakurajin::unit_system::time_si sakurajin::unit_system::time_si::operator/(long double scalar) const {
    return sakurajin::unit_system::time_si{value / scalar, multiplier, offset};
}

sakurajin::unit_system::time_si sakurajin::unit_system::time_si::operator+(const sakurajin::unit_system::time_si& other) const {
    auto retval = convert_like(other);
    retval.val() += other.val();
    return retval;
}

sakurajin::unit_system::time_si sakurajin::unit_system::time_si::operator-(const sakurajin::unit_system::time_si& other) const {
    auto retval = convert_like(other);
    retval.val() -= other.val();
    return retval;
}

sakurajin::unit_system::time_si sakurajin::unit_system::time_si::operator-() const {
    return sakurajin::unit_system::time_si{-value, multiplier, offset};
}

sakurajin::unit_system::time_si::operator long double() const {
    return convert_copy(1, 0).val();
}

sakurajin::unit_system::time_si sakurajin::unit_system::time_si::convert_multiplier(long double new_multiplier) const {
    return convert_copy(new_multiplier, offset);
}

sakurajin::unit_system::time_si sakurajin::unit_system::time_si::convert_offset(long double new_offset) const {
    return convert_copy(multiplier, new_offset);
}

sakurajin::unit_system::time_si sakurajin::unit_system::time_si::convert_copy(long double new_multiplier, long double new_offset) const {
    auto                            valBase0 = value * multiplier + offset;
    sakurajin::unit_system::time_si retval{valBase0 / new_multiplier - new_offset, new_multiplier, new_offset};
    return retval;
}

sakurajin::unit_system::time_si sakurajin::unit_system::time_si::convert_like(const sakurajin::unit_system::time_si& other) const {
    return convert_copy(other.multiplier, other.offset);
}

// comparison operators
#if __cplusplus >= 202002L
int sakurajin::unit_system::time_si::operator<=>(const sakurajin::unit_system::time_si& other) const {
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
bool sakurajin::unit_system::time_si::operator<(const sakurajin::unit_system::time_si& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    return value < retval.val();
}

bool sakurajin::unit_system::time_si::operator>(const sakurajin::unit_system::time_si& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    return value > retval.val();
}

bool sakurajin::unit_system::time_si::operator<=(const sakurajin::unit_system::time_si& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    return value <= retval.val();
}

bool sakurajin::unit_system::time_si::operator>=(const sakurajin::unit_system::time_si& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    return value >= retval.val();
}

bool sakurajin::unit_system::time_si::operator==(const sakurajin::unit_system::time_si& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    return value == retval.val();
}

bool sakurajin::unit_system::time_si::operator!=(const sakurajin::unit_system::time_si& other) const {
    return !(*this == other);
}

// non const member functions
void sakurajin::unit_system::time_si::operator*=(long double scalar) {
    value *= scalar;
}

void sakurajin::unit_system::time_si::operator/=(long double scalar) {
    value /= scalar;
}

void sakurajin::unit_system::time_si::operator+=(const sakurajin::unit_system::time_si& other) {
    const auto otherVal = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    value += otherVal.val();
}

void sakurajin::unit_system::time_si::operator-=(const sakurajin::unit_system::time_si& other) {
    const auto otherVal = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    value -= otherVal.val();
}

void sakurajin::unit_system::time_si::operator=(const sakurajin::unit_system::time_si& other) {
    const auto otherVal = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    value               = otherVal.val();
}


sakurajin::unit_system::length sakurajin::unit_system::time_si::operator*(const sakurajin::unit_system::speed& other) const {
    sakurajin::unit_system::time_si _v1 = convert_offset(0);
    sakurajin::unit_system::speed   _v2 = other.convert_offset(0);
    return sakurajin::unit_system::length{_v1.val() * _v2.val(), _v1.mult() * _v2.mult()};
}

sakurajin::unit_system::speed sakurajin::unit_system::time_si::operator*(const sakurajin::unit_system::acceleration& other) const {
    sakurajin::unit_system::time_si      _v1 = convert_offset(0);
    sakurajin::unit_system::acceleration _v2 = other.convert_offset(0);
    return sakurajin::unit_system::speed{_v1.val() * _v2.val(), _v1.mult() * _v2.mult()};
}

sakurajin::unit_system::energy sakurajin::unit_system::time_si::operator*(const sakurajin::unit_system::power& other) const {
    sakurajin::unit_system::time_si _v1 = convert_offset(0);
    sakurajin::unit_system::power   _v2 = other.convert_offset(0);
    return sakurajin::unit_system::energy{_v1.val() * _v2.val(), _v1.mult() * _v2.mult()};
}

sakurajin::unit_system::momentum sakurajin::unit_system::time_si::operator*(const sakurajin::unit_system::force& other) const {
    sakurajin::unit_system::time_si _v1 = convert_offset(0);
    sakurajin::unit_system::force   _v2 = other.convert_offset(0);
    return sakurajin::unit_system::momentum{_v1.val() * _v2.val(), _v1.mult() * _v2.mult()};
}


// external functions

sakurajin::unit_system::time_si sakurajin::unit_system::operator*(long double scalar, const sakurajin::unit_system::time_si& value) {
    return value * scalar;
}


sakurajin::unit_system::time_si
sakurajin::unit_system::unit_cast(const sakurajin::unit_system::time_si& unit, long double new_multiplier, long double new_offset) {
    return unit.convert_copy(new_multiplier, new_offset);
}

sakurajin::unit_system::time_si sakurajin::unit_system::clamp(const sakurajin::unit_system::time_si& unit,
                                                              const sakurajin::unit_system::time_si& lower,
                                                              const sakurajin::unit_system::time_si& upper) {
    auto _lower = unit_cast(lower, unit.mult(), unit.off());
    auto _upper = unit_cast(upper, unit.mult(), unit.off());

    auto val = unit.val() > _lower.val() ? (unit.val() < _upper.val() ? unit.val() : _upper.val()) : _lower.val();
    return sakurajin::unit_system::time_si{val, unit.mult(), unit.off()};
}


sakurajin::unit_system::time_si std::abs(const sakurajin::unit_system::time_si& unit) {
    auto inv = -unit;
    return unit > inv ? unit : inv;
}

std::ostream& sakurajin::unit_system::operator<<(std::ostream& os, const sakurajin::unit_system::time_si& t) {
    auto t1 = sakurajin::unit_system::unit_cast(t, 1);
    return os << t1.val() << " second";
}


// literals


sakurajin::unit_system::time_si sakurajin::unit_system::literals::operator"" _a(long double len) {
    return sakurajin::unit_system::time_si{len, 31536000000.0, 0.0};
}

sakurajin::unit_system::time_si sakurajin::unit_system::literals::operator"" _a(unsigned long long int len) {
    return sakurajin::unit_system::time_si{static_cast<long double>(len), 31536000000.0, 0.0};
}

sakurajin::unit_system::time_si sakurajin::unit_system::literals::operator"" _d(long double len) {
    return sakurajin::unit_system::time_si{len, 86400000.0, 0.0};
}

sakurajin::unit_system::time_si sakurajin::unit_system::literals::operator"" _d(unsigned long long int len) {
    return sakurajin::unit_system::time_si{static_cast<long double>(len), 86400000.0, 0.0};
}

sakurajin::unit_system::time_si sakurajin::unit_system::literals::operator"" _h(long double len) {
    return sakurajin::unit_system::time_si{len, 3600.0, 0.0};
}

sakurajin::unit_system::time_si sakurajin::unit_system::literals::operator"" _h(unsigned long long int len) {
    return sakurajin::unit_system::time_si{static_cast<long double>(len), 3600.0, 0.0};
}

sakurajin::unit_system::time_si sakurajin::unit_system::literals::operator"" _minute(long double len) {
    return sakurajin::unit_system::time_si{len, 60.0, 0.0};
}

sakurajin::unit_system::time_si sakurajin::unit_system::literals::operator"" _minute(unsigned long long int len) {
    return sakurajin::unit_system::time_si{static_cast<long double>(len), 60.0, 0.0};
}

sakurajin::unit_system::time_si sakurajin::unit_system::literals::operator"" _s(long double len) {
    return sakurajin::unit_system::time_si{len, 1.0, 0.0};
}

sakurajin::unit_system::time_si sakurajin::unit_system::literals::operator"" _s(unsigned long long int len) {
    return sakurajin::unit_system::time_si{static_cast<long double>(len), 1.0, 0.0};
}

sakurajin::unit_system::time_si sakurajin::unit_system::literals::operator"" _ms(long double len) {
    return sakurajin::unit_system::time_si{len, 0.001, 0.0};
}

sakurajin::unit_system::time_si sakurajin::unit_system::literals::operator"" _ms(unsigned long long int len) {
    return sakurajin::unit_system::time_si{static_cast<long double>(len), 0.001, 0.0};
}

sakurajin::unit_system::time_si sakurajin::unit_system::literals::operator"" _us(long double len) {
    return sakurajin::unit_system::time_si{len, 1e-06, 0.0};
}

sakurajin::unit_system::time_si sakurajin::unit_system::literals::operator"" _us(unsigned long long int len) {
    return sakurajin::unit_system::time_si{static_cast<long double>(len), 1e-06, 0.0};
}

sakurajin::unit_system::time_si sakurajin::unit_system::literals::operator"" _ns(long double len) {
    return sakurajin::unit_system::time_si{len, 1e-09, 0.0};
}

sakurajin::unit_system::time_si sakurajin::unit_system::literals::operator"" _ns(unsigned long long int len) {
    return sakurajin::unit_system::time_si{static_cast<long double>(len), 1e-09, 0.0};
}

sakurajin::unit_system::time_si sakurajin::unit_system::literals::operator"" _ps(long double len) {
    return sakurajin::unit_system::time_si{len, 1e-12, 0.0};
}

sakurajin::unit_system::time_si sakurajin::unit_system::literals::operator"" _ps(unsigned long long int len) {
    return sakurajin::unit_system::time_si{static_cast<long double>(len), 1e-12, 0.0};
}

sakurajin::unit_system::time_si sakurajin::unit_system::literals::operator"" _fs(long double len) {
    return sakurajin::unit_system::time_si{len, 1e-15, 0.0};
}

sakurajin::unit_system::time_si sakurajin::unit_system::literals::operator"" _fs(unsigned long long int len) {
    return sakurajin::unit_system::time_si{static_cast<long double>(len), 1e-15, 0.0};
}

sakurajin::unit_system::time_si sakurajin::unit_system::literals::operator"" _as(long double len) {
    return sakurajin::unit_system::time_si{len, 1e-18, 0.0};
}

sakurajin::unit_system::time_si sakurajin::unit_system::literals::operator"" _as(unsigned long long int len) {
    return sakurajin::unit_system::time_si{static_cast<long double>(len), 1e-18, 0.0};
}
