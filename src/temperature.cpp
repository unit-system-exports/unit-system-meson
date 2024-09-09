#include "cmath"
#include "unit_system.hpp"

sakurajin::unit_system::temperature::temperature()
    : temperature{0.0} {}
sakurajin::unit_system::temperature::temperature(long double v)
    : temperature{v, 1, 0} {}
sakurajin::unit_system::temperature::temperature(long double v, long double mult)
    : temperature{v, mult, 0} {}
sakurajin::unit_system::temperature::temperature(long double v, long double mult, long double off)
    : value{v},
      multiplier{mult},
      offset{off} {}


long double sakurajin::unit_system::temperature::mult() const {
    return multiplier;
}
long double sakurajin::unit_system::temperature::val() const {
    return value;
}
long double sakurajin::unit_system::temperature::off() const {
    return offset;
}
long double sakurajin::unit_system::temperature::rel_err() const {
    return rel_error;
}

long double& sakurajin::unit_system::temperature::mult() {
    return multiplier;
}
long double& sakurajin::unit_system::temperature::val() {
    return value;
}
long double& sakurajin::unit_system::temperature::off() {
    return offset;
}
long double& sakurajin::unit_system::temperature::rel_err() {
    return rel_error;
}

// const functions
sakurajin::unit_system::temperature sakurajin::unit_system::temperature::operator*(long double scalar) const {
    return sakurajin::unit_system::temperature{value * scalar, multiplier, offset};
}

sakurajin::unit_system::temperature operator*(long double scalar, const sakurajin::unit_system::temperature& val) {
    return sakurajin::unit_system::temperature{val.val() * scalar, val.mult(), val.off()};
}

long double sakurajin::unit_system::temperature::operator/(const sakurajin::unit_system::temperature& other) const {
    return value / other.convert_like(*this).val();
}

sakurajin::unit_system::temperature sakurajin::unit_system::temperature::operator/(long double scalar) const {
    return sakurajin::unit_system::temperature{value / scalar, multiplier, offset};
}

sakurajin::unit_system::temperature sakurajin::unit_system::temperature::operator+(const sakurajin::unit_system::temperature& other) const {
    auto retval = convert_like(other);
    retval.val() += other.val();
    return retval;
}

sakurajin::unit_system::temperature sakurajin::unit_system::temperature::operator-(const sakurajin::unit_system::temperature& other) const {
    auto retval = convert_like(other);
    retval.val() -= other.val();
    return retval;
}

sakurajin::unit_system::temperature sakurajin::unit_system::temperature::operator-() const {
    return sakurajin::unit_system::temperature{-value, multiplier, offset};
}

sakurajin::unit_system::temperature::operator long double() const {
    return convert_copy(1, 0).val();
}

sakurajin::unit_system::temperature sakurajin::unit_system::temperature::convert_multiplier(long double new_multiplier) const {
    return convert_copy(new_multiplier, offset);
}

sakurajin::unit_system::temperature sakurajin::unit_system::temperature::convert_offset(long double new_offset) const {
    return convert_copy(multiplier, new_offset);
}

sakurajin::unit_system::temperature sakurajin::unit_system::temperature::convert_copy(long double new_multiplier,
                                                                                      long double new_offset) const {
    auto                                valBase0 = value * multiplier + offset;
    sakurajin::unit_system::temperature retval{valBase0 / new_multiplier - new_offset, new_multiplier, new_offset};
    return retval;
}

sakurajin::unit_system::temperature
sakurajin::unit_system::temperature::convert_like(const sakurajin::unit_system::temperature& other) const {
    return convert_copy(other.multiplier, other.offset);
}

// comparison operators
#if __cplusplus >= 202002L
int sakurajin::unit_system::temperature::operator<=>(const sakurajin::unit_system::temperature& other) const {
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
bool sakurajin::unit_system::temperature::operator<(const sakurajin::unit_system::temperature& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    return value < retval.val();
}

bool sakurajin::unit_system::temperature::operator>(const sakurajin::unit_system::temperature& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    return value > retval.val();
}

bool sakurajin::unit_system::temperature::operator<=(const sakurajin::unit_system::temperature& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    return value <= retval.val();
}

bool sakurajin::unit_system::temperature::operator>=(const sakurajin::unit_system::temperature& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    return value >= retval.val();
}

bool sakurajin::unit_system::temperature::operator==(const sakurajin::unit_system::temperature& other) const {
    const auto retval = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    return value == retval.val();
}

bool sakurajin::unit_system::temperature::operator!=(const sakurajin::unit_system::temperature& other) const {
    return !(*this == other);
}

// non const member functions
void sakurajin::unit_system::temperature::operator*=(long double scalar) {
    value *= scalar;
}

void sakurajin::unit_system::temperature::operator/=(long double scalar) {
    value /= scalar;
}

void sakurajin::unit_system::temperature::operator+=(const sakurajin::unit_system::temperature& other) {
    const auto otherVal = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    value += otherVal.val();
}

void sakurajin::unit_system::temperature::operator-=(const sakurajin::unit_system::temperature& other) {
    const auto otherVal = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    value -= otherVal.val();
}

void sakurajin::unit_system::temperature::operator=(const sakurajin::unit_system::temperature& other) {
    const auto otherVal = sakurajin::unit_system::unit_cast(other, multiplier, offset);
    value               = otherVal.val();
}


// external functions

sakurajin::unit_system::temperature sakurajin::unit_system::operator*(long double                                scalar,
                                                                      const sakurajin::unit_system::temperature& value) {
    return value * scalar;
}


sakurajin::unit_system::temperature
sakurajin::unit_system::unit_cast(const sakurajin::unit_system::temperature& unit, long double new_multiplier, long double new_offset) {
    return unit.convert_copy(new_multiplier, new_offset);
}

sakurajin::unit_system::temperature sakurajin::unit_system::clamp(const sakurajin::unit_system::temperature& unit,
                                                                  const sakurajin::unit_system::temperature& lower,
                                                                  const sakurajin::unit_system::temperature& upper) {
    auto _lower = unit_cast(lower, unit.mult(), unit.off());
    auto _upper = unit_cast(upper, unit.mult(), unit.off());

    auto val = unit.val() > _lower.val() ? (unit.val() < _upper.val() ? unit.val() : _upper.val()) : _lower.val();
    return sakurajin::unit_system::temperature{val, unit.mult(), unit.off()};
}


sakurajin::unit_system::temperature std::abs(const sakurajin::unit_system::temperature& unit) {
    auto inv = -unit;
    return unit > inv ? unit : inv;
}

std::ostream& sakurajin::unit_system::operator<<(std::ostream& os, const sakurajin::unit_system::temperature& t) {
    auto t1 = sakurajin::unit_system::unit_cast(t, 1);
    return os << t1.val() << " Kelvin";
}


// literals


sakurajin::unit_system::temperature sakurajin::unit_system::literals::operator"" _K(long double len) {
    return sakurajin::unit_system::temperature{len, 1.0, 0.0};
}

sakurajin::unit_system::temperature sakurajin::unit_system::literals::operator"" _K(unsigned long long int len) {
    return sakurajin::unit_system::temperature{static_cast<long double>(len), 1.0, 0.0};
}

sakurajin::unit_system::temperature sakurajin::unit_system::literals::operator"" _C(long double len) {
    return sakurajin::unit_system::temperature{len, 1.0, 273.15};
}

sakurajin::unit_system::temperature sakurajin::unit_system::literals::operator"" _C(unsigned long long int len) {
    return sakurajin::unit_system::temperature{static_cast<long double>(len), 1.0, 273.15};
}
