#pragma once


#if __cplusplus < 202002L
    #error "C++ 20 support is required"
#endif


#ifndef UNIT_SYSTEM_DEFAULT_TYPE
    #define UNIT_SYSTEM_DEFAULT_TYPE long double
#endif

#include <algorithm>
#include <chrono>
#include <cmath>
#include <concepts>
#include <iostream>
#include <ratio>

#ifndef UNIT_SYSTEM_EXPORT_MACRO
    #define UNIT_SYSTEM_EXPORT_MACRO
#endif


namespace sakurajin {
    namespace unit_system {


        template <std::floating_point base_type>
        class time_si_t {
          private:
            base_type value{static_cast<base_type>(0.0)};
            base_type multiplier{static_cast<base_type>(1.0)};
            base_type offset{static_cast<base_type>(0.0)};
            base_type rel_error{static_cast<base_type>(0.000001)};

          public:
            time_si_t()                       = default;
            time_si_t(const time_si_t& other) = default;

            template <class value_t = base_type, class mult_t = base_type, class offset_t = base_type>
                requires std::convertible_to<value_t, base_type> && std::convertible_to<mult_t, base_type> &&
                             std::convertible_to<offset_t, base_type>
            explicit time_si_t(value_t v, mult_t mult = 1.0, offset_t off = 0.0)
                : value{static_cast<base_type>(v)},
                  multiplier{static_cast<base_type>(mult)},
                  offset{static_cast<base_type>(off)} {}

            template <std::intmax_t numerator, std::intmax_t denumerator = 1, class value_t = base_type, class offset_t = base_type>
            explicit time_si_t(value_t v, std::ratio<numerator, denumerator>, offset_t off = 0.0)
                : time_si_t{v, static_cast<long double>(numerator) / static_cast<long double>(denumerator), off} {}


            base_type val() const { return value; }
            base_type mult() const { return multiplier; }
            base_type off() const { return offset; }
            base_type rel_err() const { return rel_error; }

            base_type& val() { return value; }
            base_type& mult() { return multiplier; }
            base_type& off() { return offset; }
            base_type& rel_err() { return rel_error; }


            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            time_si_t<base_type> operator*(scalar_t scalar) const {
                return time_si_t<base_type>{value * static_cast<base_type>(scalar), multiplier, offset};
            }

            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            void operator*=(scalar_t scalar) {
                value *= static_cast<base_type>(scalar);
            }

            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            time_si_t<base_type> operator/(scalar_t scalar) const {
                return time_si_t<base_type>{value / static_cast<base_type>(scalar), multiplier, offset};
            }
            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            void operator/=(scalar_t scalar) {
                value /= static_cast<base_type>(scalar);
            }


            long double operator/(const time_si_t& other) const {
                return static_cast<long double>(this->value) / static_cast<long double>(other.convert_like(*this).val());
            }

            time_si_t operator+(const time_si_t& other) const {
                auto retval = other.convert_like(*this);
                retval.val() += this->value;
                return retval;
            }
            void operator+=(const time_si_t& other) {
                auto retval = other.convert_like(*this);
                this->value += retval.val();
            }

            time_si_t operator-(const time_si_t& other) const {
                auto retval = this->convert_like(other);
                retval.val() -= other.val();
                return retval;
            }
            void operator-=(const time_si_t& other) {
                auto retval = other.convert_like(*this);
                this->value -= retval.val();
            }

            time_si_t operator-() const { return time_si_t{-value, multiplier, offset}; }

            time_si_t& operator=(const time_si_t& other) = default;

            explicit operator long double() const { return convert_copy(1.0, 0.0).val(); }

            template <class mult_t = base_type, class offset_t = base_type>
                requires std::convertible_to<mult_t, base_type> && std::convertible_to<offset_t, base_type>
            [[nodiscard]]
            time_si_t convert_copy(mult_t new_multiplier, offset_t new_offset) const {
                auto new_mult = static_cast<base_type>(new_multiplier);
                auto new_off  = static_cast<base_type>(new_offset);
                auto new_val  = (value * multiplier + (offset - new_off)) / new_mult;
                return time_si_t{new_val, new_mult, new_off};
            }

            template <class mult_t = base_type>
                requires std::convertible_to<mult_t, base_type>
            [[nodiscard]]
            auto convert_multiplier(mult_t new_multiplier) const {
                return convert_copy(new_multiplier, this->offset);
            }

            template <class offset_t = base_type>
                requires std::convertible_to<offset_t, base_type>
            [[nodiscard]]
            auto convert_offset(offset_t new_offset) const {
                return convert_copy(this->multiplier, new_offset);
            }

            // returns a copy of the unit with the same multiplier and offset as the other unit
            [[nodiscard]]
            auto convert_like(const time_si_t& other) const {
                return convert_copy(other.mult(), other.off());
            }

            auto operator<=>(const time_si_t& other) const { return this->val() <=> other.convert_like(*this).val(); }

            auto operator==(const time_si_t& other) const { return this->val() == other.convert_like(*this).val(); }
            auto operator!=(const time_si_t& other) const { return this->val() != other.convert_like(*this).val(); }
        };


        template <std::floating_point base_type, class sclar_t>
            requires std::convertible_to<sclar_t, base_type>
        time_si_t<base_type> operator*(sclar_t scalar, const time_si_t<base_type>& value) {
            return value * scalar;
        }

        template <std::floating_point base_type, class mult_t = base_type, class offset_t = base_type>
            requires std::convertible_to<mult_t, base_type> && std::convertible_to<offset_t, base_type>
        time_si_t<base_type> unit_cast(const time_si_t<base_type>& unit, mult_t new_multiplier = 1, offset_t new_offset = 0) {
            return unit.convert_copy(new_multiplier, new_offset);
        }

        template <std::floating_point base_type>
        time_si_t<base_type> clamp(const time_si_t<base_type>& unit, const time_si_t<base_type>& lower, const time_si_t<base_type>& upper) {
            auto low  = lower.convert_like(unit);
            auto high = upper.convert_like(unit);
            return time_si_t<base_type>{std::clamp(unit.val(), low.val(), high.val()), unit.mult(), unit.off()};
        }

        template <std::floating_point base_type>
        std::ostream& operator<<(std::ostream& os, const time_si_t<base_type>& val) {
            auto val_raw = val.convert_copy(1.0, 0.0);
            return os << val_raw.val() << " ";
        }


        template <std::floating_point base_type>
        class length_t {
          private:
            base_type value{static_cast<base_type>(0.0)};
            base_type multiplier{static_cast<base_type>(1.0)};
            base_type offset{static_cast<base_type>(0.0)};
            base_type rel_error{static_cast<base_type>(0.000001)};

          public:
            length_t()                      = default;
            length_t(const length_t& other) = default;

            template <class value_t = base_type, class mult_t = base_type, class offset_t = base_type>
                requires std::convertible_to<value_t, base_type> && std::convertible_to<mult_t, base_type> &&
                             std::convertible_to<offset_t, base_type>
            explicit length_t(value_t v, mult_t mult = 1.0, offset_t off = 0.0)
                : value{static_cast<base_type>(v)},
                  multiplier{static_cast<base_type>(mult)},
                  offset{static_cast<base_type>(off)} {}

            template <std::intmax_t numerator, std::intmax_t denumerator = 1, class value_t = base_type, class offset_t = base_type>
            explicit length_t(value_t v, std::ratio<numerator, denumerator>, offset_t off = 0.0)
                : length_t{v, static_cast<long double>(numerator) / static_cast<long double>(denumerator), off} {}


            base_type val() const { return value; }
            base_type mult() const { return multiplier; }
            base_type off() const { return offset; }
            base_type rel_err() const { return rel_error; }

            base_type& val() { return value; }
            base_type& mult() { return multiplier; }
            base_type& off() { return offset; }
            base_type& rel_err() { return rel_error; }


            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            length_t<base_type> operator*(scalar_t scalar) const {
                return length_t<base_type>{value * static_cast<base_type>(scalar), multiplier, offset};
            }

            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            void operator*=(scalar_t scalar) {
                value *= static_cast<base_type>(scalar);
            }

            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            length_t<base_type> operator/(scalar_t scalar) const {
                return length_t<base_type>{value / static_cast<base_type>(scalar), multiplier, offset};
            }
            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            void operator/=(scalar_t scalar) {
                value /= static_cast<base_type>(scalar);
            }


            long double operator/(const length_t& other) const {
                return static_cast<long double>(this->value) / static_cast<long double>(other.convert_like(*this).val());
            }

            length_t operator+(const length_t& other) const {
                auto retval = other.convert_like(*this);
                retval.val() += this->value;
                return retval;
            }
            void operator+=(const length_t& other) {
                auto retval = other.convert_like(*this);
                this->value += retval.val();
            }

            length_t operator-(const length_t& other) const {
                auto retval = this->convert_like(other);
                retval.val() -= other.val();
                return retval;
            }
            void operator-=(const length_t& other) {
                auto retval = other.convert_like(*this);
                this->value -= retval.val();
            }

            length_t operator-() const { return length_t{-value, multiplier, offset}; }

            length_t& operator=(const length_t& other) = default;

            explicit operator long double() const { return convert_copy(1.0, 0.0).val(); }

            template <class mult_t = base_type, class offset_t = base_type>
                requires std::convertible_to<mult_t, base_type> && std::convertible_to<offset_t, base_type>
            [[nodiscard]]
            length_t convert_copy(mult_t new_multiplier, offset_t new_offset) const {
                auto new_mult = static_cast<base_type>(new_multiplier);
                auto new_off  = static_cast<base_type>(new_offset);
                auto new_val  = (value * multiplier + (offset - new_off)) / new_mult;
                return length_t{new_val, new_mult, new_off};
            }

            template <class mult_t = base_type>
                requires std::convertible_to<mult_t, base_type>
            [[nodiscard]]
            auto convert_multiplier(mult_t new_multiplier) const {
                return convert_copy(new_multiplier, this->offset);
            }

            template <class offset_t = base_type>
                requires std::convertible_to<offset_t, base_type>
            [[nodiscard]]
            auto convert_offset(offset_t new_offset) const {
                return convert_copy(this->multiplier, new_offset);
            }

            // returns a copy of the unit with the same multiplier and offset as the other unit
            [[nodiscard]]
            auto convert_like(const length_t& other) const {
                return convert_copy(other.mult(), other.off());
            }

            auto operator<=>(const length_t& other) const { return this->val() <=> other.convert_like(*this).val(); }

            auto operator==(const length_t& other) const { return this->val() == other.convert_like(*this).val(); }
            auto operator!=(const length_t& other) const { return this->val() != other.convert_like(*this).val(); }
        };


        template <std::floating_point base_type, class sclar_t>
            requires std::convertible_to<sclar_t, base_type>
        length_t<base_type> operator*(sclar_t scalar, const length_t<base_type>& value) {
            return value * scalar;
        }

        template <std::floating_point base_type, class mult_t = base_type, class offset_t = base_type>
            requires std::convertible_to<mult_t, base_type> && std::convertible_to<offset_t, base_type>
        length_t<base_type> unit_cast(const length_t<base_type>& unit, mult_t new_multiplier = 1, offset_t new_offset = 0) {
            return unit.convert_copy(new_multiplier, new_offset);
        }

        template <std::floating_point base_type>
        length_t<base_type> clamp(const length_t<base_type>& unit, const length_t<base_type>& lower, const length_t<base_type>& upper) {
            auto low  = lower.convert_like(unit);
            auto high = upper.convert_like(unit);
            return length_t<base_type>{std::clamp(unit.val(), low.val(), high.val()), unit.mult(), unit.off()};
        }

        template <std::floating_point base_type>
        std::ostream& operator<<(std::ostream& os, const length_t<base_type>& val) {
            auto val_raw = val.convert_copy(1.0, 0.0);
            return os << val_raw.val() << " ";
        }


        template <std::floating_point base_type>
        class mass_t {
          private:
            base_type value{static_cast<base_type>(0.0)};
            base_type multiplier{static_cast<base_type>(1.0)};
            base_type offset{static_cast<base_type>(0.0)};
            base_type rel_error{static_cast<base_type>(0.000001)};

          public:
            mass_t()                    = default;
            mass_t(const mass_t& other) = default;

            template <class value_t = base_type, class mult_t = base_type, class offset_t = base_type>
                requires std::convertible_to<value_t, base_type> && std::convertible_to<mult_t, base_type> &&
                             std::convertible_to<offset_t, base_type>
            explicit mass_t(value_t v, mult_t mult = 1.0, offset_t off = 0.0)
                : value{static_cast<base_type>(v)},
                  multiplier{static_cast<base_type>(mult)},
                  offset{static_cast<base_type>(off)} {}

            template <std::intmax_t numerator, std::intmax_t denumerator = 1, class value_t = base_type, class offset_t = base_type>
            explicit mass_t(value_t v, std::ratio<numerator, denumerator>, offset_t off = 0.0)
                : mass_t{v, static_cast<long double>(numerator) / static_cast<long double>(denumerator), off} {}


            base_type val() const { return value; }
            base_type mult() const { return multiplier; }
            base_type off() const { return offset; }
            base_type rel_err() const { return rel_error; }

            base_type& val() { return value; }
            base_type& mult() { return multiplier; }
            base_type& off() { return offset; }
            base_type& rel_err() { return rel_error; }


            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            mass_t<base_type> operator*(scalar_t scalar) const {
                return mass_t<base_type>{value * static_cast<base_type>(scalar), multiplier, offset};
            }

            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            void operator*=(scalar_t scalar) {
                value *= static_cast<base_type>(scalar);
            }

            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            mass_t<base_type> operator/(scalar_t scalar) const {
                return mass_t<base_type>{value / static_cast<base_type>(scalar), multiplier, offset};
            }
            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            void operator/=(scalar_t scalar) {
                value /= static_cast<base_type>(scalar);
            }


            long double operator/(const mass_t& other) const {
                return static_cast<long double>(this->value) / static_cast<long double>(other.convert_like(*this).val());
            }

            mass_t operator+(const mass_t& other) const {
                auto retval = other.convert_like(*this);
                retval.val() += this->value;
                return retval;
            }
            void operator+=(const mass_t& other) {
                auto retval = other.convert_like(*this);
                this->value += retval.val();
            }

            mass_t operator-(const mass_t& other) const {
                auto retval = this->convert_like(other);
                retval.val() -= other.val();
                return retval;
            }
            void operator-=(const mass_t& other) {
                auto retval = other.convert_like(*this);
                this->value -= retval.val();
            }

            mass_t operator-() const { return mass_t{-value, multiplier, offset}; }

            mass_t& operator=(const mass_t& other) = default;

            explicit operator long double() const { return convert_copy(1.0, 0.0).val(); }

            template <class mult_t = base_type, class offset_t = base_type>
                requires std::convertible_to<mult_t, base_type> && std::convertible_to<offset_t, base_type>
            [[nodiscard]]
            mass_t convert_copy(mult_t new_multiplier, offset_t new_offset) const {
                auto new_mult = static_cast<base_type>(new_multiplier);
                auto new_off  = static_cast<base_type>(new_offset);
                auto new_val  = (value * multiplier + (offset - new_off)) / new_mult;
                return mass_t{new_val, new_mult, new_off};
            }

            template <class mult_t = base_type>
                requires std::convertible_to<mult_t, base_type>
            [[nodiscard]]
            auto convert_multiplier(mult_t new_multiplier) const {
                return convert_copy(new_multiplier, this->offset);
            }

            template <class offset_t = base_type>
                requires std::convertible_to<offset_t, base_type>
            [[nodiscard]]
            auto convert_offset(offset_t new_offset) const {
                return convert_copy(this->multiplier, new_offset);
            }

            // returns a copy of the unit with the same multiplier and offset as the other unit
            [[nodiscard]]
            auto convert_like(const mass_t& other) const {
                return convert_copy(other.mult(), other.off());
            }

            auto operator<=>(const mass_t& other) const { return this->val() <=> other.convert_like(*this).val(); }

            auto operator==(const mass_t& other) const { return this->val() == other.convert_like(*this).val(); }
            auto operator!=(const mass_t& other) const { return this->val() != other.convert_like(*this).val(); }
        };


        template <std::floating_point base_type, class sclar_t>
            requires std::convertible_to<sclar_t, base_type>
        mass_t<base_type> operator*(sclar_t scalar, const mass_t<base_type>& value) {
            return value * scalar;
        }

        template <std::floating_point base_type, class mult_t = base_type, class offset_t = base_type>
            requires std::convertible_to<mult_t, base_type> && std::convertible_to<offset_t, base_type>
        mass_t<base_type> unit_cast(const mass_t<base_type>& unit, mult_t new_multiplier = 1, offset_t new_offset = 0) {
            return unit.convert_copy(new_multiplier, new_offset);
        }

        template <std::floating_point base_type>
        mass_t<base_type> clamp(const mass_t<base_type>& unit, const mass_t<base_type>& lower, const mass_t<base_type>& upper) {
            auto low  = lower.convert_like(unit);
            auto high = upper.convert_like(unit);
            return mass_t<base_type>{std::clamp(unit.val(), low.val(), high.val()), unit.mult(), unit.off()};
        }

        template <std::floating_point base_type>
        std::ostream& operator<<(std::ostream& os, const mass_t<base_type>& val) {
            auto val_raw = val.convert_copy(1.0, 0.0);
            return os << val_raw.val() << " ";
        }


        template <std::floating_point base_type>
        class temperature_t {
          private:
            base_type value{static_cast<base_type>(0.0)};
            base_type multiplier{static_cast<base_type>(1.0)};
            base_type offset{static_cast<base_type>(0.0)};
            base_type rel_error{static_cast<base_type>(0.000001)};

          public:
            temperature_t()                           = default;
            temperature_t(const temperature_t& other) = default;

            template <class value_t = base_type, class mult_t = base_type, class offset_t = base_type>
                requires std::convertible_to<value_t, base_type> && std::convertible_to<mult_t, base_type> &&
                             std::convertible_to<offset_t, base_type>
            explicit temperature_t(value_t v, mult_t mult = 1.0, offset_t off = 0.0)
                : value{static_cast<base_type>(v)},
                  multiplier{static_cast<base_type>(mult)},
                  offset{static_cast<base_type>(off)} {}

            template <std::intmax_t numerator, std::intmax_t denumerator = 1, class value_t = base_type, class offset_t = base_type>
            explicit temperature_t(value_t v, std::ratio<numerator, denumerator>, offset_t off = 0.0)
                : temperature_t{v, static_cast<long double>(numerator) / static_cast<long double>(denumerator), off} {}


            base_type val() const { return value; }
            base_type mult() const { return multiplier; }
            base_type off() const { return offset; }
            base_type rel_err() const { return rel_error; }

            base_type& val() { return value; }
            base_type& mult() { return multiplier; }
            base_type& off() { return offset; }
            base_type& rel_err() { return rel_error; }


            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            temperature_t<base_type> operator*(scalar_t scalar) const {
                return temperature_t<base_type>{value * static_cast<base_type>(scalar), multiplier, offset};
            }

            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            void operator*=(scalar_t scalar) {
                value *= static_cast<base_type>(scalar);
            }

            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            temperature_t<base_type> operator/(scalar_t scalar) const {
                return temperature_t<base_type>{value / static_cast<base_type>(scalar), multiplier, offset};
            }
            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            void operator/=(scalar_t scalar) {
                value /= static_cast<base_type>(scalar);
            }


            long double operator/(const temperature_t& other) const {
                return static_cast<long double>(this->value) / static_cast<long double>(other.convert_like(*this).val());
            }

            temperature_t operator+(const temperature_t& other) const {
                auto retval = other.convert_like(*this);
                retval.val() += this->value;
                return retval;
            }
            void operator+=(const temperature_t& other) {
                auto retval = other.convert_like(*this);
                this->value += retval.val();
            }

            temperature_t operator-(const temperature_t& other) const {
                auto retval = this->convert_like(other);
                retval.val() -= other.val();
                return retval;
            }
            void operator-=(const temperature_t& other) {
                auto retval = other.convert_like(*this);
                this->value -= retval.val();
            }

            temperature_t operator-() const { return temperature_t{-value, multiplier, offset}; }

            temperature_t& operator=(const temperature_t& other) = default;

            explicit operator long double() const { return convert_copy(1.0, 0.0).val(); }

            template <class mult_t = base_type, class offset_t = base_type>
                requires std::convertible_to<mult_t, base_type> && std::convertible_to<offset_t, base_type>
            [[nodiscard]]
            temperature_t convert_copy(mult_t new_multiplier, offset_t new_offset) const {
                auto new_mult = static_cast<base_type>(new_multiplier);
                auto new_off  = static_cast<base_type>(new_offset);
                auto new_val  = (value * multiplier + (offset - new_off)) / new_mult;
                return temperature_t{new_val, new_mult, new_off};
            }

            template <class mult_t = base_type>
                requires std::convertible_to<mult_t, base_type>
            [[nodiscard]]
            auto convert_multiplier(mult_t new_multiplier) const {
                return convert_copy(new_multiplier, this->offset);
            }

            template <class offset_t = base_type>
                requires std::convertible_to<offset_t, base_type>
            [[nodiscard]]
            auto convert_offset(offset_t new_offset) const {
                return convert_copy(this->multiplier, new_offset);
            }

            // returns a copy of the unit with the same multiplier and offset as the other unit
            [[nodiscard]]
            auto convert_like(const temperature_t& other) const {
                return convert_copy(other.mult(), other.off());
            }

            auto operator<=>(const temperature_t& other) const { return this->val() <=> other.convert_like(*this).val(); }

            auto operator==(const temperature_t& other) const { return this->val() == other.convert_like(*this).val(); }
            auto operator!=(const temperature_t& other) const { return this->val() != other.convert_like(*this).val(); }
        };


        template <std::floating_point base_type, class sclar_t>
            requires std::convertible_to<sclar_t, base_type>
        temperature_t<base_type> operator*(sclar_t scalar, const temperature_t<base_type>& value) {
            return value * scalar;
        }

        template <std::floating_point base_type, class mult_t = base_type, class offset_t = base_type>
            requires std::convertible_to<mult_t, base_type> && std::convertible_to<offset_t, base_type>
        temperature_t<base_type> unit_cast(const temperature_t<base_type>& unit, mult_t new_multiplier = 1, offset_t new_offset = 0) {
            return unit.convert_copy(new_multiplier, new_offset);
        }

        template <std::floating_point base_type>
        temperature_t<base_type>
        clamp(const temperature_t<base_type>& unit, const temperature_t<base_type>& lower, const temperature_t<base_type>& upper) {
            auto low  = lower.convert_like(unit);
            auto high = upper.convert_like(unit);
            return temperature_t<base_type>{std::clamp(unit.val(), low.val(), high.val()), unit.mult(), unit.off()};
        }

        template <std::floating_point base_type>
        std::ostream& operator<<(std::ostream& os, const temperature_t<base_type>& val) {
            auto val_raw = val.convert_copy(1.0, 0.0);
            return os << val_raw.val() << " ";
        }


        template <std::floating_point base_type>
        class amount_t {
          private:
            base_type value{static_cast<base_type>(0.0)};
            base_type multiplier{static_cast<base_type>(1.0)};
            base_type offset{static_cast<base_type>(0.0)};
            base_type rel_error{static_cast<base_type>(0.000001)};

          public:
            amount_t()                      = default;
            amount_t(const amount_t& other) = default;

            template <class value_t = base_type, class mult_t = base_type, class offset_t = base_type>
                requires std::convertible_to<value_t, base_type> && std::convertible_to<mult_t, base_type> &&
                             std::convertible_to<offset_t, base_type>
            explicit amount_t(value_t v, mult_t mult = 1.0, offset_t off = 0.0)
                : value{static_cast<base_type>(v)},
                  multiplier{static_cast<base_type>(mult)},
                  offset{static_cast<base_type>(off)} {}

            template <std::intmax_t numerator, std::intmax_t denumerator = 1, class value_t = base_type, class offset_t = base_type>
            explicit amount_t(value_t v, std::ratio<numerator, denumerator>, offset_t off = 0.0)
                : amount_t{v, static_cast<long double>(numerator) / static_cast<long double>(denumerator), off} {}


            base_type val() const { return value; }
            base_type mult() const { return multiplier; }
            base_type off() const { return offset; }
            base_type rel_err() const { return rel_error; }

            base_type& val() { return value; }
            base_type& mult() { return multiplier; }
            base_type& off() { return offset; }
            base_type& rel_err() { return rel_error; }


            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            amount_t<base_type> operator*(scalar_t scalar) const {
                return amount_t<base_type>{value * static_cast<base_type>(scalar), multiplier, offset};
            }

            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            void operator*=(scalar_t scalar) {
                value *= static_cast<base_type>(scalar);
            }

            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            amount_t<base_type> operator/(scalar_t scalar) const {
                return amount_t<base_type>{value / static_cast<base_type>(scalar), multiplier, offset};
            }
            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            void operator/=(scalar_t scalar) {
                value /= static_cast<base_type>(scalar);
            }


            long double operator/(const amount_t& other) const {
                return static_cast<long double>(this->value) / static_cast<long double>(other.convert_like(*this).val());
            }

            amount_t operator+(const amount_t& other) const {
                auto retval = other.convert_like(*this);
                retval.val() += this->value;
                return retval;
            }
            void operator+=(const amount_t& other) {
                auto retval = other.convert_like(*this);
                this->value += retval.val();
            }

            amount_t operator-(const amount_t& other) const {
                auto retval = this->convert_like(other);
                retval.val() -= other.val();
                return retval;
            }
            void operator-=(const amount_t& other) {
                auto retval = other.convert_like(*this);
                this->value -= retval.val();
            }

            amount_t operator-() const { return amount_t{-value, multiplier, offset}; }

            amount_t& operator=(const amount_t& other) = default;

            explicit operator long double() const { return convert_copy(1.0, 0.0).val(); }

            template <class mult_t = base_type, class offset_t = base_type>
                requires std::convertible_to<mult_t, base_type> && std::convertible_to<offset_t, base_type>
            [[nodiscard]]
            amount_t convert_copy(mult_t new_multiplier, offset_t new_offset) const {
                auto new_mult = static_cast<base_type>(new_multiplier);
                auto new_off  = static_cast<base_type>(new_offset);
                auto new_val  = (value * multiplier + (offset - new_off)) / new_mult;
                return amount_t{new_val, new_mult, new_off};
            }

            template <class mult_t = base_type>
                requires std::convertible_to<mult_t, base_type>
            [[nodiscard]]
            auto convert_multiplier(mult_t new_multiplier) const {
                return convert_copy(new_multiplier, this->offset);
            }

            template <class offset_t = base_type>
                requires std::convertible_to<offset_t, base_type>
            [[nodiscard]]
            auto convert_offset(offset_t new_offset) const {
                return convert_copy(this->multiplier, new_offset);
            }

            // returns a copy of the unit with the same multiplier and offset as the other unit
            [[nodiscard]]
            auto convert_like(const amount_t& other) const {
                return convert_copy(other.mult(), other.off());
            }

            auto operator<=>(const amount_t& other) const { return this->val() <=> other.convert_like(*this).val(); }

            auto operator==(const amount_t& other) const { return this->val() == other.convert_like(*this).val(); }
            auto operator!=(const amount_t& other) const { return this->val() != other.convert_like(*this).val(); }
        };


        template <std::floating_point base_type, class sclar_t>
            requires std::convertible_to<sclar_t, base_type>
        amount_t<base_type> operator*(sclar_t scalar, const amount_t<base_type>& value) {
            return value * scalar;
        }

        template <std::floating_point base_type, class mult_t = base_type, class offset_t = base_type>
            requires std::convertible_to<mult_t, base_type> && std::convertible_to<offset_t, base_type>
        amount_t<base_type> unit_cast(const amount_t<base_type>& unit, mult_t new_multiplier = 1, offset_t new_offset = 0) {
            return unit.convert_copy(new_multiplier, new_offset);
        }

        template <std::floating_point base_type>
        amount_t<base_type> clamp(const amount_t<base_type>& unit, const amount_t<base_type>& lower, const amount_t<base_type>& upper) {
            auto low  = lower.convert_like(unit);
            auto high = upper.convert_like(unit);
            return amount_t<base_type>{std::clamp(unit.val(), low.val(), high.val()), unit.mult(), unit.off()};
        }

        template <std::floating_point base_type>
        std::ostream& operator<<(std::ostream& os, const amount_t<base_type>& val) {
            auto val_raw = val.convert_copy(1.0, 0.0);
            return os << val_raw.val() << " ";
        }


        template <std::floating_point base_type>
        class electric_current_t {
          private:
            base_type value{static_cast<base_type>(0.0)};
            base_type multiplier{static_cast<base_type>(1.0)};
            base_type offset{static_cast<base_type>(0.0)};
            base_type rel_error{static_cast<base_type>(0.000001)};

          public:
            electric_current_t()                                = default;
            electric_current_t(const electric_current_t& other) = default;

            template <class value_t = base_type, class mult_t = base_type, class offset_t = base_type>
                requires std::convertible_to<value_t, base_type> && std::convertible_to<mult_t, base_type> &&
                             std::convertible_to<offset_t, base_type>
            explicit electric_current_t(value_t v, mult_t mult = 1.0, offset_t off = 0.0)
                : value{static_cast<base_type>(v)},
                  multiplier{static_cast<base_type>(mult)},
                  offset{static_cast<base_type>(off)} {}

            template <std::intmax_t numerator, std::intmax_t denumerator = 1, class value_t = base_type, class offset_t = base_type>
            explicit electric_current_t(value_t v, std::ratio<numerator, denumerator>, offset_t off = 0.0)
                : electric_current_t{v, static_cast<long double>(numerator) / static_cast<long double>(denumerator), off} {}


            base_type val() const { return value; }
            base_type mult() const { return multiplier; }
            base_type off() const { return offset; }
            base_type rel_err() const { return rel_error; }

            base_type& val() { return value; }
            base_type& mult() { return multiplier; }
            base_type& off() { return offset; }
            base_type& rel_err() { return rel_error; }


            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            electric_current_t<base_type> operator*(scalar_t scalar) const {
                return electric_current_t<base_type>{value * static_cast<base_type>(scalar), multiplier, offset};
            }

            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            void operator*=(scalar_t scalar) {
                value *= static_cast<base_type>(scalar);
            }

            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            electric_current_t<base_type> operator/(scalar_t scalar) const {
                return electric_current_t<base_type>{value / static_cast<base_type>(scalar), multiplier, offset};
            }
            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            void operator/=(scalar_t scalar) {
                value /= static_cast<base_type>(scalar);
            }


            long double operator/(const electric_current_t& other) const {
                return static_cast<long double>(this->value) / static_cast<long double>(other.convert_like(*this).val());
            }

            electric_current_t operator+(const electric_current_t& other) const {
                auto retval = other.convert_like(*this);
                retval.val() += this->value;
                return retval;
            }
            void operator+=(const electric_current_t& other) {
                auto retval = other.convert_like(*this);
                this->value += retval.val();
            }

            electric_current_t operator-(const electric_current_t& other) const {
                auto retval = this->convert_like(other);
                retval.val() -= other.val();
                return retval;
            }
            void operator-=(const electric_current_t& other) {
                auto retval = other.convert_like(*this);
                this->value -= retval.val();
            }

            electric_current_t operator-() const { return electric_current_t{-value, multiplier, offset}; }

            electric_current_t& operator=(const electric_current_t& other) = default;

            explicit operator long double() const { return convert_copy(1.0, 0.0).val(); }

            template <class mult_t = base_type, class offset_t = base_type>
                requires std::convertible_to<mult_t, base_type> && std::convertible_to<offset_t, base_type>
            [[nodiscard]]
            electric_current_t convert_copy(mult_t new_multiplier, offset_t new_offset) const {
                auto new_mult = static_cast<base_type>(new_multiplier);
                auto new_off  = static_cast<base_type>(new_offset);
                auto new_val  = (value * multiplier + (offset - new_off)) / new_mult;
                return electric_current_t{new_val, new_mult, new_off};
            }

            template <class mult_t = base_type>
                requires std::convertible_to<mult_t, base_type>
            [[nodiscard]]
            auto convert_multiplier(mult_t new_multiplier) const {
                return convert_copy(new_multiplier, this->offset);
            }

            template <class offset_t = base_type>
                requires std::convertible_to<offset_t, base_type>
            [[nodiscard]]
            auto convert_offset(offset_t new_offset) const {
                return convert_copy(this->multiplier, new_offset);
            }

            // returns a copy of the unit with the same multiplier and offset as the other unit
            [[nodiscard]]
            auto convert_like(const electric_current_t& other) const {
                return convert_copy(other.mult(), other.off());
            }

            auto operator<=>(const electric_current_t& other) const { return this->val() <=> other.convert_like(*this).val(); }

            auto operator==(const electric_current_t& other) const { return this->val() == other.convert_like(*this).val(); }
            auto operator!=(const electric_current_t& other) const { return this->val() != other.convert_like(*this).val(); }
        };


        template <std::floating_point base_type, class sclar_t>
            requires std::convertible_to<sclar_t, base_type>
        electric_current_t<base_type> operator*(sclar_t scalar, const electric_current_t<base_type>& value) {
            return value * scalar;
        }

        template <std::floating_point base_type, class mult_t = base_type, class offset_t = base_type>
            requires std::convertible_to<mult_t, base_type> && std::convertible_to<offset_t, base_type>
        electric_current_t<base_type>
        unit_cast(const electric_current_t<base_type>& unit, mult_t new_multiplier = 1, offset_t new_offset = 0) {
            return unit.convert_copy(new_multiplier, new_offset);
        }

        template <std::floating_point base_type>
        electric_current_t<base_type> clamp(const electric_current_t<base_type>& unit,
                                            const electric_current_t<base_type>& lower,
                                            const electric_current_t<base_type>& upper) {
            auto low  = lower.convert_like(unit);
            auto high = upper.convert_like(unit);
            return electric_current_t<base_type>{std::clamp(unit.val(), low.val(), high.val()), unit.mult(), unit.off()};
        }

        template <std::floating_point base_type>
        std::ostream& operator<<(std::ostream& os, const electric_current_t<base_type>& val) {
            auto val_raw = val.convert_copy(1.0, 0.0);
            return os << val_raw.val() << " ";
        }


        template <std::floating_point base_type>
        class luminous_intensity_t {
          private:
            base_type value{static_cast<base_type>(0.0)};
            base_type multiplier{static_cast<base_type>(1.0)};
            base_type offset{static_cast<base_type>(0.0)};
            base_type rel_error{static_cast<base_type>(0.000001)};

          public:
            luminous_intensity_t()                                  = default;
            luminous_intensity_t(const luminous_intensity_t& other) = default;

            template <class value_t = base_type, class mult_t = base_type, class offset_t = base_type>
                requires std::convertible_to<value_t, base_type> && std::convertible_to<mult_t, base_type> &&
                             std::convertible_to<offset_t, base_type>
            explicit luminous_intensity_t(value_t v, mult_t mult = 1.0, offset_t off = 0.0)
                : value{static_cast<base_type>(v)},
                  multiplier{static_cast<base_type>(mult)},
                  offset{static_cast<base_type>(off)} {}

            template <std::intmax_t numerator, std::intmax_t denumerator = 1, class value_t = base_type, class offset_t = base_type>
            explicit luminous_intensity_t(value_t v, std::ratio<numerator, denumerator>, offset_t off = 0.0)
                : luminous_intensity_t{v, static_cast<long double>(numerator) / static_cast<long double>(denumerator), off} {}


            base_type val() const { return value; }
            base_type mult() const { return multiplier; }
            base_type off() const { return offset; }
            base_type rel_err() const { return rel_error; }

            base_type& val() { return value; }
            base_type& mult() { return multiplier; }
            base_type& off() { return offset; }
            base_type& rel_err() { return rel_error; }


            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            luminous_intensity_t<base_type> operator*(scalar_t scalar) const {
                return luminous_intensity_t<base_type>{value * static_cast<base_type>(scalar), multiplier, offset};
            }

            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            void operator*=(scalar_t scalar) {
                value *= static_cast<base_type>(scalar);
            }

            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            luminous_intensity_t<base_type> operator/(scalar_t scalar) const {
                return luminous_intensity_t<base_type>{value / static_cast<base_type>(scalar), multiplier, offset};
            }
            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            void operator/=(scalar_t scalar) {
                value /= static_cast<base_type>(scalar);
            }


            long double operator/(const luminous_intensity_t& other) const {
                return static_cast<long double>(this->value) / static_cast<long double>(other.convert_like(*this).val());
            }

            luminous_intensity_t operator+(const luminous_intensity_t& other) const {
                auto retval = other.convert_like(*this);
                retval.val() += this->value;
                return retval;
            }
            void operator+=(const luminous_intensity_t& other) {
                auto retval = other.convert_like(*this);
                this->value += retval.val();
            }

            luminous_intensity_t operator-(const luminous_intensity_t& other) const {
                auto retval = this->convert_like(other);
                retval.val() -= other.val();
                return retval;
            }
            void operator-=(const luminous_intensity_t& other) {
                auto retval = other.convert_like(*this);
                this->value -= retval.val();
            }

            luminous_intensity_t operator-() const { return luminous_intensity_t{-value, multiplier, offset}; }

            luminous_intensity_t& operator=(const luminous_intensity_t& other) = default;

            explicit operator long double() const { return convert_copy(1.0, 0.0).val(); }

            template <class mult_t = base_type, class offset_t = base_type>
                requires std::convertible_to<mult_t, base_type> && std::convertible_to<offset_t, base_type>
            [[nodiscard]]
            luminous_intensity_t convert_copy(mult_t new_multiplier, offset_t new_offset) const {
                auto new_mult = static_cast<base_type>(new_multiplier);
                auto new_off  = static_cast<base_type>(new_offset);
                auto new_val  = (value * multiplier + (offset - new_off)) / new_mult;
                return luminous_intensity_t{new_val, new_mult, new_off};
            }

            template <class mult_t = base_type>
                requires std::convertible_to<mult_t, base_type>
            [[nodiscard]]
            auto convert_multiplier(mult_t new_multiplier) const {
                return convert_copy(new_multiplier, this->offset);
            }

            template <class offset_t = base_type>
                requires std::convertible_to<offset_t, base_type>
            [[nodiscard]]
            auto convert_offset(offset_t new_offset) const {
                return convert_copy(this->multiplier, new_offset);
            }

            // returns a copy of the unit with the same multiplier and offset as the other unit
            [[nodiscard]]
            auto convert_like(const luminous_intensity_t& other) const {
                return convert_copy(other.mult(), other.off());
            }

            auto operator<=>(const luminous_intensity_t& other) const { return this->val() <=> other.convert_like(*this).val(); }

            auto operator==(const luminous_intensity_t& other) const { return this->val() == other.convert_like(*this).val(); }
            auto operator!=(const luminous_intensity_t& other) const { return this->val() != other.convert_like(*this).val(); }
        };


        template <std::floating_point base_type, class sclar_t>
            requires std::convertible_to<sclar_t, base_type>
        luminous_intensity_t<base_type> operator*(sclar_t scalar, const luminous_intensity_t<base_type>& value) {
            return value * scalar;
        }

        template <std::floating_point base_type, class mult_t = base_type, class offset_t = base_type>
            requires std::convertible_to<mult_t, base_type> && std::convertible_to<offset_t, base_type>
        luminous_intensity_t<base_type>
        unit_cast(const luminous_intensity_t<base_type>& unit, mult_t new_multiplier = 1, offset_t new_offset = 0) {
            return unit.convert_copy(new_multiplier, new_offset);
        }

        template <std::floating_point base_type>
        luminous_intensity_t<base_type> clamp(const luminous_intensity_t<base_type>& unit,
                                              const luminous_intensity_t<base_type>& lower,
                                              const luminous_intensity_t<base_type>& upper) {
            auto low  = lower.convert_like(unit);
            auto high = upper.convert_like(unit);
            return luminous_intensity_t<base_type>{std::clamp(unit.val(), low.val(), high.val()), unit.mult(), unit.off()};
        }

        template <std::floating_point base_type>
        std::ostream& operator<<(std::ostream& os, const luminous_intensity_t<base_type>& val) {
            auto val_raw = val.convert_copy(1.0, 0.0);
            return os << val_raw.val() << " ";
        }


        template <std::floating_point base_type>
        class energy_t {
          private:
            base_type value{static_cast<base_type>(0.0)};
            base_type multiplier{static_cast<base_type>(1.0)};
            base_type offset{static_cast<base_type>(0.0)};
            base_type rel_error{static_cast<base_type>(0.000001)};

          public:
            energy_t()                      = default;
            energy_t(const energy_t& other) = default;

            template <class value_t = base_type, class mult_t = base_type, class offset_t = base_type>
                requires std::convertible_to<value_t, base_type> && std::convertible_to<mult_t, base_type> &&
                             std::convertible_to<offset_t, base_type>
            explicit energy_t(value_t v, mult_t mult = 1.0, offset_t off = 0.0)
                : value{static_cast<base_type>(v)},
                  multiplier{static_cast<base_type>(mult)},
                  offset{static_cast<base_type>(off)} {}

            template <std::intmax_t numerator, std::intmax_t denumerator = 1, class value_t = base_type, class offset_t = base_type>
            explicit energy_t(value_t v, std::ratio<numerator, denumerator>, offset_t off = 0.0)
                : energy_t{v, static_cast<long double>(numerator) / static_cast<long double>(denumerator), off} {}


            base_type val() const { return value; }
            base_type mult() const { return multiplier; }
            base_type off() const { return offset; }
            base_type rel_err() const { return rel_error; }

            base_type& val() { return value; }
            base_type& mult() { return multiplier; }
            base_type& off() { return offset; }
            base_type& rel_err() { return rel_error; }


            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            energy_t<base_type> operator*(scalar_t scalar) const {
                return energy_t<base_type>{value * static_cast<base_type>(scalar), multiplier, offset};
            }

            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            void operator*=(scalar_t scalar) {
                value *= static_cast<base_type>(scalar);
            }

            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            energy_t<base_type> operator/(scalar_t scalar) const {
                return energy_t<base_type>{value / static_cast<base_type>(scalar), multiplier, offset};
            }
            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            void operator/=(scalar_t scalar) {
                value /= static_cast<base_type>(scalar);
            }


            long double operator/(const energy_t& other) const {
                return static_cast<long double>(this->value) / static_cast<long double>(other.convert_like(*this).val());
            }

            energy_t operator+(const energy_t& other) const {
                auto retval = other.convert_like(*this);
                retval.val() += this->value;
                return retval;
            }
            void operator+=(const energy_t& other) {
                auto retval = other.convert_like(*this);
                this->value += retval.val();
            }

            energy_t operator-(const energy_t& other) const {
                auto retval = this->convert_like(other);
                retval.val() -= other.val();
                return retval;
            }
            void operator-=(const energy_t& other) {
                auto retval = other.convert_like(*this);
                this->value -= retval.val();
            }

            energy_t operator-() const { return energy_t{-value, multiplier, offset}; }

            energy_t& operator=(const energy_t& other) = default;

            explicit operator long double() const { return convert_copy(1.0, 0.0).val(); }

            template <class mult_t = base_type, class offset_t = base_type>
                requires std::convertible_to<mult_t, base_type> && std::convertible_to<offset_t, base_type>
            [[nodiscard]]
            energy_t convert_copy(mult_t new_multiplier, offset_t new_offset) const {
                auto new_mult = static_cast<base_type>(new_multiplier);
                auto new_off  = static_cast<base_type>(new_offset);
                auto new_val  = (value * multiplier + (offset - new_off)) / new_mult;
                return energy_t{new_val, new_mult, new_off};
            }

            template <class mult_t = base_type>
                requires std::convertible_to<mult_t, base_type>
            [[nodiscard]]
            auto convert_multiplier(mult_t new_multiplier) const {
                return convert_copy(new_multiplier, this->offset);
            }

            template <class offset_t = base_type>
                requires std::convertible_to<offset_t, base_type>
            [[nodiscard]]
            auto convert_offset(offset_t new_offset) const {
                return convert_copy(this->multiplier, new_offset);
            }

            // returns a copy of the unit with the same multiplier and offset as the other unit
            [[nodiscard]]
            auto convert_like(const energy_t& other) const {
                return convert_copy(other.mult(), other.off());
            }

            auto operator<=>(const energy_t& other) const { return this->val() <=> other.convert_like(*this).val(); }

            auto operator==(const energy_t& other) const { return this->val() == other.convert_like(*this).val(); }
            auto operator!=(const energy_t& other) const { return this->val() != other.convert_like(*this).val(); }
        };


        template <std::floating_point base_type, class sclar_t>
            requires std::convertible_to<sclar_t, base_type>
        energy_t<base_type> operator*(sclar_t scalar, const energy_t<base_type>& value) {
            return value * scalar;
        }

        template <std::floating_point base_type, class mult_t = base_type, class offset_t = base_type>
            requires std::convertible_to<mult_t, base_type> && std::convertible_to<offset_t, base_type>
        energy_t<base_type> unit_cast(const energy_t<base_type>& unit, mult_t new_multiplier = 1, offset_t new_offset = 0) {
            return unit.convert_copy(new_multiplier, new_offset);
        }

        template <std::floating_point base_type>
        energy_t<base_type> clamp(const energy_t<base_type>& unit, const energy_t<base_type>& lower, const energy_t<base_type>& upper) {
            auto low  = lower.convert_like(unit);
            auto high = upper.convert_like(unit);
            return energy_t<base_type>{std::clamp(unit.val(), low.val(), high.val()), unit.mult(), unit.off()};
        }

        template <std::floating_point base_type>
        std::ostream& operator<<(std::ostream& os, const energy_t<base_type>& val) {
            auto val_raw = val.convert_copy(1.0, 0.0);
            return os << val_raw.val() << " ";
        }


        template <std::floating_point base_type>
        class power_t {
          private:
            base_type value{static_cast<base_type>(0.0)};
            base_type multiplier{static_cast<base_type>(1.0)};
            base_type offset{static_cast<base_type>(0.0)};
            base_type rel_error{static_cast<base_type>(0.000001)};

          public:
            power_t()                     = default;
            power_t(const power_t& other) = default;

            template <class value_t = base_type, class mult_t = base_type, class offset_t = base_type>
                requires std::convertible_to<value_t, base_type> && std::convertible_to<mult_t, base_type> &&
                             std::convertible_to<offset_t, base_type>
            explicit power_t(value_t v, mult_t mult = 1.0, offset_t off = 0.0)
                : value{static_cast<base_type>(v)},
                  multiplier{static_cast<base_type>(mult)},
                  offset{static_cast<base_type>(off)} {}

            template <std::intmax_t numerator, std::intmax_t denumerator = 1, class value_t = base_type, class offset_t = base_type>
            explicit power_t(value_t v, std::ratio<numerator, denumerator>, offset_t off = 0.0)
                : power_t{v, static_cast<long double>(numerator) / static_cast<long double>(denumerator), off} {}


            base_type val() const { return value; }
            base_type mult() const { return multiplier; }
            base_type off() const { return offset; }
            base_type rel_err() const { return rel_error; }

            base_type& val() { return value; }
            base_type& mult() { return multiplier; }
            base_type& off() { return offset; }
            base_type& rel_err() { return rel_error; }


            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            power_t<base_type> operator*(scalar_t scalar) const {
                return power_t<base_type>{value * static_cast<base_type>(scalar), multiplier, offset};
            }

            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            void operator*=(scalar_t scalar) {
                value *= static_cast<base_type>(scalar);
            }

            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            power_t<base_type> operator/(scalar_t scalar) const {
                return power_t<base_type>{value / static_cast<base_type>(scalar), multiplier, offset};
            }
            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            void operator/=(scalar_t scalar) {
                value /= static_cast<base_type>(scalar);
            }


            long double operator/(const power_t& other) const {
                return static_cast<long double>(this->value) / static_cast<long double>(other.convert_like(*this).val());
            }

            power_t operator+(const power_t& other) const {
                auto retval = other.convert_like(*this);
                retval.val() += this->value;
                return retval;
            }
            void operator+=(const power_t& other) {
                auto retval = other.convert_like(*this);
                this->value += retval.val();
            }

            power_t operator-(const power_t& other) const {
                auto retval = this->convert_like(other);
                retval.val() -= other.val();
                return retval;
            }
            void operator-=(const power_t& other) {
                auto retval = other.convert_like(*this);
                this->value -= retval.val();
            }

            power_t operator-() const { return power_t{-value, multiplier, offset}; }

            power_t& operator=(const power_t& other) = default;

            explicit operator long double() const { return convert_copy(1.0, 0.0).val(); }

            template <class mult_t = base_type, class offset_t = base_type>
                requires std::convertible_to<mult_t, base_type> && std::convertible_to<offset_t, base_type>
            [[nodiscard]]
            power_t convert_copy(mult_t new_multiplier, offset_t new_offset) const {
                auto new_mult = static_cast<base_type>(new_multiplier);
                auto new_off  = static_cast<base_type>(new_offset);
                auto new_val  = (value * multiplier + (offset - new_off)) / new_mult;
                return power_t{new_val, new_mult, new_off};
            }

            template <class mult_t = base_type>
                requires std::convertible_to<mult_t, base_type>
            [[nodiscard]]
            auto convert_multiplier(mult_t new_multiplier) const {
                return convert_copy(new_multiplier, this->offset);
            }

            template <class offset_t = base_type>
                requires std::convertible_to<offset_t, base_type>
            [[nodiscard]]
            auto convert_offset(offset_t new_offset) const {
                return convert_copy(this->multiplier, new_offset);
            }

            // returns a copy of the unit with the same multiplier and offset as the other unit
            [[nodiscard]]
            auto convert_like(const power_t& other) const {
                return convert_copy(other.mult(), other.off());
            }

            auto operator<=>(const power_t& other) const { return this->val() <=> other.convert_like(*this).val(); }

            auto operator==(const power_t& other) const { return this->val() == other.convert_like(*this).val(); }
            auto operator!=(const power_t& other) const { return this->val() != other.convert_like(*this).val(); }
        };


        template <std::floating_point base_type, class sclar_t>
            requires std::convertible_to<sclar_t, base_type>
        power_t<base_type> operator*(sclar_t scalar, const power_t<base_type>& value) {
            return value * scalar;
        }

        template <std::floating_point base_type, class mult_t = base_type, class offset_t = base_type>
            requires std::convertible_to<mult_t, base_type> && std::convertible_to<offset_t, base_type>
        power_t<base_type> unit_cast(const power_t<base_type>& unit, mult_t new_multiplier = 1, offset_t new_offset = 0) {
            return unit.convert_copy(new_multiplier, new_offset);
        }

        template <std::floating_point base_type>
        power_t<base_type> clamp(const power_t<base_type>& unit, const power_t<base_type>& lower, const power_t<base_type>& upper) {
            auto low  = lower.convert_like(unit);
            auto high = upper.convert_like(unit);
            return power_t<base_type>{std::clamp(unit.val(), low.val(), high.val()), unit.mult(), unit.off()};
        }

        template <std::floating_point base_type>
        std::ostream& operator<<(std::ostream& os, const power_t<base_type>& val) {
            auto val_raw = val.convert_copy(1.0, 0.0);
            return os << val_raw.val() << " ";
        }


        template <std::floating_point base_type>
        class speed_t {
          private:
            base_type value{static_cast<base_type>(0.0)};
            base_type multiplier{static_cast<base_type>(1.0)};
            base_type offset{static_cast<base_type>(0.0)};
            base_type rel_error{static_cast<base_type>(0.000001)};

          public:
            speed_t()                     = default;
            speed_t(const speed_t& other) = default;

            template <class value_t = base_type, class mult_t = base_type, class offset_t = base_type>
                requires std::convertible_to<value_t, base_type> && std::convertible_to<mult_t, base_type> &&
                             std::convertible_to<offset_t, base_type>
            explicit speed_t(value_t v, mult_t mult = 1.0, offset_t off = 0.0)
                : value{static_cast<base_type>(v)},
                  multiplier{static_cast<base_type>(mult)},
                  offset{static_cast<base_type>(off)} {}

            template <std::intmax_t numerator, std::intmax_t denumerator = 1, class value_t = base_type, class offset_t = base_type>
            explicit speed_t(value_t v, std::ratio<numerator, denumerator>, offset_t off = 0.0)
                : speed_t{v, static_cast<long double>(numerator) / static_cast<long double>(denumerator), off} {}


            base_type val() const { return value; }
            base_type mult() const { return multiplier; }
            base_type off() const { return offset; }
            base_type rel_err() const { return rel_error; }

            base_type& val() { return value; }
            base_type& mult() { return multiplier; }
            base_type& off() { return offset; }
            base_type& rel_err() { return rel_error; }


            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            speed_t<base_type> operator*(scalar_t scalar) const {
                return speed_t<base_type>{value * static_cast<base_type>(scalar), multiplier, offset};
            }

            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            void operator*=(scalar_t scalar) {
                value *= static_cast<base_type>(scalar);
            }

            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            speed_t<base_type> operator/(scalar_t scalar) const {
                return speed_t<base_type>{value / static_cast<base_type>(scalar), multiplier, offset};
            }
            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            void operator/=(scalar_t scalar) {
                value /= static_cast<base_type>(scalar);
            }


            long double operator/(const speed_t& other) const {
                return static_cast<long double>(this->value) / static_cast<long double>(other.convert_like(*this).val());
            }

            speed_t operator+(const speed_t& other) const {
                auto retval = other.convert_like(*this);
                retval.val() += this->value;
                return retval;
            }
            void operator+=(const speed_t& other) {
                auto retval = other.convert_like(*this);
                this->value += retval.val();
            }

            speed_t operator-(const speed_t& other) const {
                auto retval = this->convert_like(other);
                retval.val() -= other.val();
                return retval;
            }
            void operator-=(const speed_t& other) {
                auto retval = other.convert_like(*this);
                this->value -= retval.val();
            }

            speed_t operator-() const { return speed_t{-value, multiplier, offset}; }

            speed_t& operator=(const speed_t& other) = default;

            explicit operator long double() const { return convert_copy(1.0, 0.0).val(); }

            template <class mult_t = base_type, class offset_t = base_type>
                requires std::convertible_to<mult_t, base_type> && std::convertible_to<offset_t, base_type>
            [[nodiscard]]
            speed_t convert_copy(mult_t new_multiplier, offset_t new_offset) const {
                auto new_mult = static_cast<base_type>(new_multiplier);
                auto new_off  = static_cast<base_type>(new_offset);
                auto new_val  = (value * multiplier + (offset - new_off)) / new_mult;
                return speed_t{new_val, new_mult, new_off};
            }

            template <class mult_t = base_type>
                requires std::convertible_to<mult_t, base_type>
            [[nodiscard]]
            auto convert_multiplier(mult_t new_multiplier) const {
                return convert_copy(new_multiplier, this->offset);
            }

            template <class offset_t = base_type>
                requires std::convertible_to<offset_t, base_type>
            [[nodiscard]]
            auto convert_offset(offset_t new_offset) const {
                return convert_copy(this->multiplier, new_offset);
            }

            // returns a copy of the unit with the same multiplier and offset as the other unit
            [[nodiscard]]
            auto convert_like(const speed_t& other) const {
                return convert_copy(other.mult(), other.off());
            }

            auto operator<=>(const speed_t& other) const { return this->val() <=> other.convert_like(*this).val(); }

            auto operator==(const speed_t& other) const { return this->val() == other.convert_like(*this).val(); }
            auto operator!=(const speed_t& other) const { return this->val() != other.convert_like(*this).val(); }
        };


        template <std::floating_point base_type, class sclar_t>
            requires std::convertible_to<sclar_t, base_type>
        speed_t<base_type> operator*(sclar_t scalar, const speed_t<base_type>& value) {
            return value * scalar;
        }

        template <std::floating_point base_type, class mult_t = base_type, class offset_t = base_type>
            requires std::convertible_to<mult_t, base_type> && std::convertible_to<offset_t, base_type>
        speed_t<base_type> unit_cast(const speed_t<base_type>& unit, mult_t new_multiplier = 1, offset_t new_offset = 0) {
            return unit.convert_copy(new_multiplier, new_offset);
        }

        template <std::floating_point base_type>
        speed_t<base_type> clamp(const speed_t<base_type>& unit, const speed_t<base_type>& lower, const speed_t<base_type>& upper) {
            auto low  = lower.convert_like(unit);
            auto high = upper.convert_like(unit);
            return speed_t<base_type>{std::clamp(unit.val(), low.val(), high.val()), unit.mult(), unit.off()};
        }

        template <std::floating_point base_type>
        std::ostream& operator<<(std::ostream& os, const speed_t<base_type>& val) {
            auto val_raw = val.convert_copy(1.0, 0.0);
            return os << val_raw.val() << " ";
        }


        template <std::floating_point base_type>
        class acceleration_t {
          private:
            base_type value{static_cast<base_type>(0.0)};
            base_type multiplier{static_cast<base_type>(1.0)};
            base_type offset{static_cast<base_type>(0.0)};
            base_type rel_error{static_cast<base_type>(0.000001)};

          public:
            acceleration_t()                            = default;
            acceleration_t(const acceleration_t& other) = default;

            template <class value_t = base_type, class mult_t = base_type, class offset_t = base_type>
                requires std::convertible_to<value_t, base_type> && std::convertible_to<mult_t, base_type> &&
                             std::convertible_to<offset_t, base_type>
            explicit acceleration_t(value_t v, mult_t mult = 1.0, offset_t off = 0.0)
                : value{static_cast<base_type>(v)},
                  multiplier{static_cast<base_type>(mult)},
                  offset{static_cast<base_type>(off)} {}

            template <std::intmax_t numerator, std::intmax_t denumerator = 1, class value_t = base_type, class offset_t = base_type>
            explicit acceleration_t(value_t v, std::ratio<numerator, denumerator>, offset_t off = 0.0)
                : acceleration_t{v, static_cast<long double>(numerator) / static_cast<long double>(denumerator), off} {}


            base_type val() const { return value; }
            base_type mult() const { return multiplier; }
            base_type off() const { return offset; }
            base_type rel_err() const { return rel_error; }

            base_type& val() { return value; }
            base_type& mult() { return multiplier; }
            base_type& off() { return offset; }
            base_type& rel_err() { return rel_error; }


            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            acceleration_t<base_type> operator*(scalar_t scalar) const {
                return acceleration_t<base_type>{value * static_cast<base_type>(scalar), multiplier, offset};
            }

            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            void operator*=(scalar_t scalar) {
                value *= static_cast<base_type>(scalar);
            }

            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            acceleration_t<base_type> operator/(scalar_t scalar) const {
                return acceleration_t<base_type>{value / static_cast<base_type>(scalar), multiplier, offset};
            }
            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            void operator/=(scalar_t scalar) {
                value /= static_cast<base_type>(scalar);
            }


            long double operator/(const acceleration_t& other) const {
                return static_cast<long double>(this->value) / static_cast<long double>(other.convert_like(*this).val());
            }

            acceleration_t operator+(const acceleration_t& other) const {
                auto retval = other.convert_like(*this);
                retval.val() += this->value;
                return retval;
            }
            void operator+=(const acceleration_t& other) {
                auto retval = other.convert_like(*this);
                this->value += retval.val();
            }

            acceleration_t operator-(const acceleration_t& other) const {
                auto retval = this->convert_like(other);
                retval.val() -= other.val();
                return retval;
            }
            void operator-=(const acceleration_t& other) {
                auto retval = other.convert_like(*this);
                this->value -= retval.val();
            }

            acceleration_t operator-() const { return acceleration_t{-value, multiplier, offset}; }

            acceleration_t& operator=(const acceleration_t& other) = default;

            explicit operator long double() const { return convert_copy(1.0, 0.0).val(); }

            template <class mult_t = base_type, class offset_t = base_type>
                requires std::convertible_to<mult_t, base_type> && std::convertible_to<offset_t, base_type>
            [[nodiscard]]
            acceleration_t convert_copy(mult_t new_multiplier, offset_t new_offset) const {
                auto new_mult = static_cast<base_type>(new_multiplier);
                auto new_off  = static_cast<base_type>(new_offset);
                auto new_val  = (value * multiplier + (offset - new_off)) / new_mult;
                return acceleration_t{new_val, new_mult, new_off};
            }

            template <class mult_t = base_type>
                requires std::convertible_to<mult_t, base_type>
            [[nodiscard]]
            auto convert_multiplier(mult_t new_multiplier) const {
                return convert_copy(new_multiplier, this->offset);
            }

            template <class offset_t = base_type>
                requires std::convertible_to<offset_t, base_type>
            [[nodiscard]]
            auto convert_offset(offset_t new_offset) const {
                return convert_copy(this->multiplier, new_offset);
            }

            // returns a copy of the unit with the same multiplier and offset as the other unit
            [[nodiscard]]
            auto convert_like(const acceleration_t& other) const {
                return convert_copy(other.mult(), other.off());
            }

            auto operator<=>(const acceleration_t& other) const { return this->val() <=> other.convert_like(*this).val(); }

            auto operator==(const acceleration_t& other) const { return this->val() == other.convert_like(*this).val(); }
            auto operator!=(const acceleration_t& other) const { return this->val() != other.convert_like(*this).val(); }
        };


        template <std::floating_point base_type, class sclar_t>
            requires std::convertible_to<sclar_t, base_type>
        acceleration_t<base_type> operator*(sclar_t scalar, const acceleration_t<base_type>& value) {
            return value * scalar;
        }

        template <std::floating_point base_type, class mult_t = base_type, class offset_t = base_type>
            requires std::convertible_to<mult_t, base_type> && std::convertible_to<offset_t, base_type>
        acceleration_t<base_type> unit_cast(const acceleration_t<base_type>& unit, mult_t new_multiplier = 1, offset_t new_offset = 0) {
            return unit.convert_copy(new_multiplier, new_offset);
        }

        template <std::floating_point base_type>
        acceleration_t<base_type>
        clamp(const acceleration_t<base_type>& unit, const acceleration_t<base_type>& lower, const acceleration_t<base_type>& upper) {
            auto low  = lower.convert_like(unit);
            auto high = upper.convert_like(unit);
            return acceleration_t<base_type>{std::clamp(unit.val(), low.val(), high.val()), unit.mult(), unit.off()};
        }

        template <std::floating_point base_type>
        std::ostream& operator<<(std::ostream& os, const acceleration_t<base_type>& val) {
            auto val_raw = val.convert_copy(1.0, 0.0);
            return os << val_raw.val() << " ";
        }


        template <std::floating_point base_type>
        class area_t {
          private:
            base_type value{static_cast<base_type>(0.0)};
            base_type multiplier{static_cast<base_type>(1.0)};
            base_type offset{static_cast<base_type>(0.0)};
            base_type rel_error{static_cast<base_type>(0.000001)};

          public:
            area_t()                    = default;
            area_t(const area_t& other) = default;

            template <class value_t = base_type, class mult_t = base_type, class offset_t = base_type>
                requires std::convertible_to<value_t, base_type> && std::convertible_to<mult_t, base_type> &&
                             std::convertible_to<offset_t, base_type>
            explicit area_t(value_t v, mult_t mult = 1.0, offset_t off = 0.0)
                : value{static_cast<base_type>(v)},
                  multiplier{static_cast<base_type>(mult)},
                  offset{static_cast<base_type>(off)} {}

            template <std::intmax_t numerator, std::intmax_t denumerator = 1, class value_t = base_type, class offset_t = base_type>
            explicit area_t(value_t v, std::ratio<numerator, denumerator>, offset_t off = 0.0)
                : area_t{v, static_cast<long double>(numerator) / static_cast<long double>(denumerator), off} {}


            base_type val() const { return value; }
            base_type mult() const { return multiplier; }
            base_type off() const { return offset; }
            base_type rel_err() const { return rel_error; }

            base_type& val() { return value; }
            base_type& mult() { return multiplier; }
            base_type& off() { return offset; }
            base_type& rel_err() { return rel_error; }


            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            area_t<base_type> operator*(scalar_t scalar) const {
                return area_t<base_type>{value * static_cast<base_type>(scalar), multiplier, offset};
            }

            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            void operator*=(scalar_t scalar) {
                value *= static_cast<base_type>(scalar);
            }

            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            area_t<base_type> operator/(scalar_t scalar) const {
                return area_t<base_type>{value / static_cast<base_type>(scalar), multiplier, offset};
            }
            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            void operator/=(scalar_t scalar) {
                value /= static_cast<base_type>(scalar);
            }


            long double operator/(const area_t& other) const {
                return static_cast<long double>(this->value) / static_cast<long double>(other.convert_like(*this).val());
            }

            area_t operator+(const area_t& other) const {
                auto retval = other.convert_like(*this);
                retval.val() += this->value;
                return retval;
            }
            void operator+=(const area_t& other) {
                auto retval = other.convert_like(*this);
                this->value += retval.val();
            }

            area_t operator-(const area_t& other) const {
                auto retval = this->convert_like(other);
                retval.val() -= other.val();
                return retval;
            }
            void operator-=(const area_t& other) {
                auto retval = other.convert_like(*this);
                this->value -= retval.val();
            }

            area_t operator-() const { return area_t{-value, multiplier, offset}; }

            area_t& operator=(const area_t& other) = default;

            explicit operator long double() const { return convert_copy(1.0, 0.0).val(); }

            template <class mult_t = base_type, class offset_t = base_type>
                requires std::convertible_to<mult_t, base_type> && std::convertible_to<offset_t, base_type>
            [[nodiscard]]
            area_t convert_copy(mult_t new_multiplier, offset_t new_offset) const {
                auto new_mult = static_cast<base_type>(new_multiplier);
                auto new_off  = static_cast<base_type>(new_offset);
                auto new_val  = (value * multiplier + (offset - new_off)) / new_mult;
                return area_t{new_val, new_mult, new_off};
            }

            template <class mult_t = base_type>
                requires std::convertible_to<mult_t, base_type>
            [[nodiscard]]
            auto convert_multiplier(mult_t new_multiplier) const {
                return convert_copy(new_multiplier, this->offset);
            }

            template <class offset_t = base_type>
                requires std::convertible_to<offset_t, base_type>
            [[nodiscard]]
            auto convert_offset(offset_t new_offset) const {
                return convert_copy(this->multiplier, new_offset);
            }

            // returns a copy of the unit with the same multiplier and offset as the other unit
            [[nodiscard]]
            auto convert_like(const area_t& other) const {
                return convert_copy(other.mult(), other.off());
            }

            auto operator<=>(const area_t& other) const { return this->val() <=> other.convert_like(*this).val(); }

            auto operator==(const area_t& other) const { return this->val() == other.convert_like(*this).val(); }
            auto operator!=(const area_t& other) const { return this->val() != other.convert_like(*this).val(); }
        };


        template <std::floating_point base_type, class sclar_t>
            requires std::convertible_to<sclar_t, base_type>
        area_t<base_type> operator*(sclar_t scalar, const area_t<base_type>& value) {
            return value * scalar;
        }

        template <std::floating_point base_type, class mult_t = base_type, class offset_t = base_type>
            requires std::convertible_to<mult_t, base_type> && std::convertible_to<offset_t, base_type>
        area_t<base_type> unit_cast(const area_t<base_type>& unit, mult_t new_multiplier = 1, offset_t new_offset = 0) {
            return unit.convert_copy(new_multiplier, new_offset);
        }

        template <std::floating_point base_type>
        area_t<base_type> clamp(const area_t<base_type>& unit, const area_t<base_type>& lower, const area_t<base_type>& upper) {
            auto low  = lower.convert_like(unit);
            auto high = upper.convert_like(unit);
            return area_t<base_type>{std::clamp(unit.val(), low.val(), high.val()), unit.mult(), unit.off()};
        }

        template <std::floating_point base_type>
        std::ostream& operator<<(std::ostream& os, const area_t<base_type>& val) {
            auto val_raw = val.convert_copy(1.0, 0.0);
            return os << val_raw.val() << " ";
        }


        template <std::floating_point base_type>
        class force_t {
          private:
            base_type value{static_cast<base_type>(0.0)};
            base_type multiplier{static_cast<base_type>(1.0)};
            base_type offset{static_cast<base_type>(0.0)};
            base_type rel_error{static_cast<base_type>(0.000001)};

          public:
            force_t()                     = default;
            force_t(const force_t& other) = default;

            template <class value_t = base_type, class mult_t = base_type, class offset_t = base_type>
                requires std::convertible_to<value_t, base_type> && std::convertible_to<mult_t, base_type> &&
                             std::convertible_to<offset_t, base_type>
            explicit force_t(value_t v, mult_t mult = 1.0, offset_t off = 0.0)
                : value{static_cast<base_type>(v)},
                  multiplier{static_cast<base_type>(mult)},
                  offset{static_cast<base_type>(off)} {}

            template <std::intmax_t numerator, std::intmax_t denumerator = 1, class value_t = base_type, class offset_t = base_type>
            explicit force_t(value_t v, std::ratio<numerator, denumerator>, offset_t off = 0.0)
                : force_t{v, static_cast<long double>(numerator) / static_cast<long double>(denumerator), off} {}


            base_type val() const { return value; }
            base_type mult() const { return multiplier; }
            base_type off() const { return offset; }
            base_type rel_err() const { return rel_error; }

            base_type& val() { return value; }
            base_type& mult() { return multiplier; }
            base_type& off() { return offset; }
            base_type& rel_err() { return rel_error; }


            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            force_t<base_type> operator*(scalar_t scalar) const {
                return force_t<base_type>{value * static_cast<base_type>(scalar), multiplier, offset};
            }

            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            void operator*=(scalar_t scalar) {
                value *= static_cast<base_type>(scalar);
            }

            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            force_t<base_type> operator/(scalar_t scalar) const {
                return force_t<base_type>{value / static_cast<base_type>(scalar), multiplier, offset};
            }
            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            void operator/=(scalar_t scalar) {
                value /= static_cast<base_type>(scalar);
            }


            long double operator/(const force_t& other) const {
                return static_cast<long double>(this->value) / static_cast<long double>(other.convert_like(*this).val());
            }

            force_t operator+(const force_t& other) const {
                auto retval = other.convert_like(*this);
                retval.val() += this->value;
                return retval;
            }
            void operator+=(const force_t& other) {
                auto retval = other.convert_like(*this);
                this->value += retval.val();
            }

            force_t operator-(const force_t& other) const {
                auto retval = this->convert_like(other);
                retval.val() -= other.val();
                return retval;
            }
            void operator-=(const force_t& other) {
                auto retval = other.convert_like(*this);
                this->value -= retval.val();
            }

            force_t operator-() const { return force_t{-value, multiplier, offset}; }

            force_t& operator=(const force_t& other) = default;

            explicit operator long double() const { return convert_copy(1.0, 0.0).val(); }

            template <class mult_t = base_type, class offset_t = base_type>
                requires std::convertible_to<mult_t, base_type> && std::convertible_to<offset_t, base_type>
            [[nodiscard]]
            force_t convert_copy(mult_t new_multiplier, offset_t new_offset) const {
                auto new_mult = static_cast<base_type>(new_multiplier);
                auto new_off  = static_cast<base_type>(new_offset);
                auto new_val  = (value * multiplier + (offset - new_off)) / new_mult;
                return force_t{new_val, new_mult, new_off};
            }

            template <class mult_t = base_type>
                requires std::convertible_to<mult_t, base_type>
            [[nodiscard]]
            auto convert_multiplier(mult_t new_multiplier) const {
                return convert_copy(new_multiplier, this->offset);
            }

            template <class offset_t = base_type>
                requires std::convertible_to<offset_t, base_type>
            [[nodiscard]]
            auto convert_offset(offset_t new_offset) const {
                return convert_copy(this->multiplier, new_offset);
            }

            // returns a copy of the unit with the same multiplier and offset as the other unit
            [[nodiscard]]
            auto convert_like(const force_t& other) const {
                return convert_copy(other.mult(), other.off());
            }

            auto operator<=>(const force_t& other) const { return this->val() <=> other.convert_like(*this).val(); }

            auto operator==(const force_t& other) const { return this->val() == other.convert_like(*this).val(); }
            auto operator!=(const force_t& other) const { return this->val() != other.convert_like(*this).val(); }
        };


        template <std::floating_point base_type, class sclar_t>
            requires std::convertible_to<sclar_t, base_type>
        force_t<base_type> operator*(sclar_t scalar, const force_t<base_type>& value) {
            return value * scalar;
        }

        template <std::floating_point base_type, class mult_t = base_type, class offset_t = base_type>
            requires std::convertible_to<mult_t, base_type> && std::convertible_to<offset_t, base_type>
        force_t<base_type> unit_cast(const force_t<base_type>& unit, mult_t new_multiplier = 1, offset_t new_offset = 0) {
            return unit.convert_copy(new_multiplier, new_offset);
        }

        template <std::floating_point base_type>
        force_t<base_type> clamp(const force_t<base_type>& unit, const force_t<base_type>& lower, const force_t<base_type>& upper) {
            auto low  = lower.convert_like(unit);
            auto high = upper.convert_like(unit);
            return force_t<base_type>{std::clamp(unit.val(), low.val(), high.val()), unit.mult(), unit.off()};
        }

        template <std::floating_point base_type>
        std::ostream& operator<<(std::ostream& os, const force_t<base_type>& val) {
            auto val_raw = val.convert_copy(1.0, 0.0);
            return os << val_raw.val() << " ";
        }


        template <std::floating_point base_type>
        class momentum_t {
          private:
            base_type value{static_cast<base_type>(0.0)};
            base_type multiplier{static_cast<base_type>(1.0)};
            base_type offset{static_cast<base_type>(0.0)};
            base_type rel_error{static_cast<base_type>(0.000001)};

          public:
            momentum_t()                        = default;
            momentum_t(const momentum_t& other) = default;

            template <class value_t = base_type, class mult_t = base_type, class offset_t = base_type>
                requires std::convertible_to<value_t, base_type> && std::convertible_to<mult_t, base_type> &&
                             std::convertible_to<offset_t, base_type>
            explicit momentum_t(value_t v, mult_t mult = 1.0, offset_t off = 0.0)
                : value{static_cast<base_type>(v)},
                  multiplier{static_cast<base_type>(mult)},
                  offset{static_cast<base_type>(off)} {}

            template <std::intmax_t numerator, std::intmax_t denumerator = 1, class value_t = base_type, class offset_t = base_type>
            explicit momentum_t(value_t v, std::ratio<numerator, denumerator>, offset_t off = 0.0)
                : momentum_t{v, static_cast<long double>(numerator) / static_cast<long double>(denumerator), off} {}


            base_type val() const { return value; }
            base_type mult() const { return multiplier; }
            base_type off() const { return offset; }
            base_type rel_err() const { return rel_error; }

            base_type& val() { return value; }
            base_type& mult() { return multiplier; }
            base_type& off() { return offset; }
            base_type& rel_err() { return rel_error; }


            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            momentum_t<base_type> operator*(scalar_t scalar) const {
                return momentum_t<base_type>{value * static_cast<base_type>(scalar), multiplier, offset};
            }

            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            void operator*=(scalar_t scalar) {
                value *= static_cast<base_type>(scalar);
            }

            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            momentum_t<base_type> operator/(scalar_t scalar) const {
                return momentum_t<base_type>{value / static_cast<base_type>(scalar), multiplier, offset};
            }
            template <class scalar_t>
                requires std::convertible_to<scalar_t, base_type>
            void operator/=(scalar_t scalar) {
                value /= static_cast<base_type>(scalar);
            }


            long double operator/(const momentum_t& other) const {
                return static_cast<long double>(this->value) / static_cast<long double>(other.convert_like(*this).val());
            }

            momentum_t operator+(const momentum_t& other) const {
                auto retval = other.convert_like(*this);
                retval.val() += this->value;
                return retval;
            }
            void operator+=(const momentum_t& other) {
                auto retval = other.convert_like(*this);
                this->value += retval.val();
            }

            momentum_t operator-(const momentum_t& other) const {
                auto retval = this->convert_like(other);
                retval.val() -= other.val();
                return retval;
            }
            void operator-=(const momentum_t& other) {
                auto retval = other.convert_like(*this);
                this->value -= retval.val();
            }

            momentum_t operator-() const { return momentum_t{-value, multiplier, offset}; }

            momentum_t& operator=(const momentum_t& other) = default;

            explicit operator long double() const { return convert_copy(1.0, 0.0).val(); }

            template <class mult_t = base_type, class offset_t = base_type>
                requires std::convertible_to<mult_t, base_type> && std::convertible_to<offset_t, base_type>
            [[nodiscard]]
            momentum_t convert_copy(mult_t new_multiplier, offset_t new_offset) const {
                auto new_mult = static_cast<base_type>(new_multiplier);
                auto new_off  = static_cast<base_type>(new_offset);
                auto new_val  = (value * multiplier + (offset - new_off)) / new_mult;
                return momentum_t{new_val, new_mult, new_off};
            }

            template <class mult_t = base_type>
                requires std::convertible_to<mult_t, base_type>
            [[nodiscard]]
            auto convert_multiplier(mult_t new_multiplier) const {
                return convert_copy(new_multiplier, this->offset);
            }

            template <class offset_t = base_type>
                requires std::convertible_to<offset_t, base_type>
            [[nodiscard]]
            auto convert_offset(offset_t new_offset) const {
                return convert_copy(this->multiplier, new_offset);
            }

            // returns a copy of the unit with the same multiplier and offset as the other unit
            [[nodiscard]]
            auto convert_like(const momentum_t& other) const {
                return convert_copy(other.mult(), other.off());
            }

            auto operator<=>(const momentum_t& other) const { return this->val() <=> other.convert_like(*this).val(); }

            auto operator==(const momentum_t& other) const { return this->val() == other.convert_like(*this).val(); }
            auto operator!=(const momentum_t& other) const { return this->val() != other.convert_like(*this).val(); }
        };


        template <std::floating_point base_type, class sclar_t>
            requires std::convertible_to<sclar_t, base_type>
        momentum_t<base_type> operator*(sclar_t scalar, const momentum_t<base_type>& value) {
            return value * scalar;
        }

        template <std::floating_point base_type, class mult_t = base_type, class offset_t = base_type>
            requires std::convertible_to<mult_t, base_type> && std::convertible_to<offset_t, base_type>
        momentum_t<base_type> unit_cast(const momentum_t<base_type>& unit, mult_t new_multiplier = 1, offset_t new_offset = 0) {
            return unit.convert_copy(new_multiplier, new_offset);
        }

        template <std::floating_point base_type>
        momentum_t<base_type>
        clamp(const momentum_t<base_type>& unit, const momentum_t<base_type>& lower, const momentum_t<base_type>& upper) {
            auto low  = lower.convert_like(unit);
            auto high = upper.convert_like(unit);
            return momentum_t<base_type>{std::clamp(unit.val(), low.val(), high.val()), unit.mult(), unit.off()};
        }

        template <std::floating_point base_type>
        std::ostream& operator<<(std::ostream& os, const momentum_t<base_type>& val) {
            auto val_raw = val.convert_copy(1.0, 0.0);
            return os << val_raw.val() << " ";
        }


        // define all unit combination operators


        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator*(const time_si_t<base_type>& val, const speed_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::length_t<base_type>{_v1.val() * _v2.val(), _v1.mult() / _v2.mult()};
        }

        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator*(const time_si_t<base_type>& val, const acceleration_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::speed_t<base_type>{_v1.val() * _v2.val(), _v1.mult() / _v2.mult()};
        }

        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator*(const time_si_t<base_type>& val, const power_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::energy_t<base_type>{_v1.val() * _v2.val(), _v1.mult() / _v2.mult()};
        }

        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator*(const time_si_t<base_type>& val, const force_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::momentum_t<base_type>{_v1.val() * _v2.val(), _v1.mult() / _v2.mult()};
        }


        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator/(const length_t<base_type>& val, const speed_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::time_si_t<base_type>{_v1.val() / _v2.val(), _v1.mult() / _v2.mult()};
        }

        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator/(const length_t<base_type>& val, const time_si_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::speed_t<base_type>{_v1.val() / _v2.val(), _v1.mult() / _v2.mult()};
        }


        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator*(const length_t<base_type>& val, const length_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::area_t<base_type>{_v1.val() * _v2.val(), _v1.mult() / _v2.mult()};
        }

        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator*(const length_t<base_type>& val, const force_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::energy_t<base_type>{_v1.val() * _v2.val(), _v1.mult() / _v2.mult()};
        }


        template <std::floating_point base_type>
        [[nodiscard]]
        auto square(const length_t<base_type>& val) {
            return val * val;
        }


        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator*(const mass_t<base_type>& val, const acceleration_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::force_t<base_type>{_v1.val() * _v2.val(), _v1.mult() / _v2.mult()};
        }

        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator*(const mass_t<base_type>& val, const speed_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::momentum_t<base_type>{_v1.val() * _v2.val(), _v1.mult() / _v2.mult()};
        }


        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator/(const energy_t<base_type>& val, const force_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::length_t<base_type>{_v1.val() / _v2.val(), _v1.mult() / _v2.mult()};
        }

        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator/(const energy_t<base_type>& val, const length_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::force_t<base_type>{_v1.val() / _v2.val(), _v1.mult() / _v2.mult()};
        }

        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator/(const energy_t<base_type>& val, const power_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::time_si_t<base_type>{_v1.val() / _v2.val(), _v1.mult() / _v2.mult()};
        }

        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator/(const energy_t<base_type>& val, const time_si_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::power_t<base_type>{_v1.val() / _v2.val(), _v1.mult() / _v2.mult()};
        }

        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator/(const energy_t<base_type>& val, const momentum_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::speed_t<base_type>{_v1.val() / _v2.val(), _v1.mult() / _v2.mult()};
        }

        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator/(const energy_t<base_type>& val, const speed_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::momentum_t<base_type>{_v1.val() / _v2.val(), _v1.mult() / _v2.mult()};
        }


        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator/(const power_t<base_type>& val, const force_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::speed_t<base_type>{_v1.val() / _v2.val(), _v1.mult() / _v2.mult()};
        }

        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator/(const power_t<base_type>& val, const speed_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::force_t<base_type>{_v1.val() / _v2.val(), _v1.mult() / _v2.mult()};
        }


        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator*(const power_t<base_type>& val, const time_si_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::energy_t<base_type>{_v1.val() * _v2.val(), _v1.mult() / _v2.mult()};
        }


        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator/(const speed_t<base_type>& val, const acceleration_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::time_si_t<base_type>{_v1.val() / _v2.val(), _v1.mult() / _v2.mult()};
        }

        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator/(const speed_t<base_type>& val, const time_si_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::acceleration_t<base_type>{_v1.val() / _v2.val(), _v1.mult() / _v2.mult()};
        }


        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator*(const speed_t<base_type>& val, const time_si_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::length_t<base_type>{_v1.val() * _v2.val(), _v1.mult() / _v2.mult()};
        }

        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator*(const speed_t<base_type>& val, const momentum_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::energy_t<base_type>{_v1.val() * _v2.val(), _v1.mult() / _v2.mult()};
        }

        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator*(const speed_t<base_type>& val, const mass_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::momentum_t<base_type>{_v1.val() * _v2.val(), _v1.mult() / _v2.mult()};
        }

        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator*(const speed_t<base_type>& val, const force_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::power_t<base_type>{_v1.val() * _v2.val(), _v1.mult() / _v2.mult()};
        }


        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator*(const acceleration_t<base_type>& val, const time_si_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::speed_t<base_type>{_v1.val() * _v2.val(), _v1.mult() / _v2.mult()};
        }

        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator*(const acceleration_t<base_type>& val, const mass_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::force_t<base_type>{_v1.val() * _v2.val(), _v1.mult() / _v2.mult()};
        }


        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator/(const area_t<base_type>& val, const length_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::length_t<base_type>{_v1.val() / _v2.val(), _v1.mult() / _v2.mult()};
        }


        template <std::floating_point base_type>
        [[nodiscard]]
        auto sqrt(const area_t<base_type>& val) {
            return sakurajin::unit_system::length_t<base_type>{std::sqrt(val.val()), std::sqrt(val.mult()), val.off()};
        }


        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator/(const force_t<base_type>& val, const mass_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::acceleration_t<base_type>{_v1.val() / _v2.val(), _v1.mult() / _v2.mult()};
        }

        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator/(const force_t<base_type>& val, const acceleration_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::mass_t<base_type>{_v1.val() / _v2.val(), _v1.mult() / _v2.mult()};
        }


        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator*(const force_t<base_type>& val, const length_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::energy_t<base_type>{_v1.val() * _v2.val(), _v1.mult() / _v2.mult()};
        }

        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator*(const force_t<base_type>& val, const time_si_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::momentum_t<base_type>{_v1.val() * _v2.val(), _v1.mult() / _v2.mult()};
        }

        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator*(const force_t<base_type>& val, const speed_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::power_t<base_type>{_v1.val() * _v2.val(), _v1.mult() / _v2.mult()};
        }


        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator/(const momentum_t<base_type>& val, const force_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::time_si_t<base_type>{_v1.val() / _v2.val(), _v1.mult() / _v2.mult()};
        }

        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator/(const momentum_t<base_type>& val, const time_si_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::force_t<base_type>{_v1.val() / _v2.val(), _v1.mult() / _v2.mult()};
        }

        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator/(const momentum_t<base_type>& val, const mass_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::speed_t<base_type>{_v1.val() / _v2.val(), _v1.mult() / _v2.mult()};
        }

        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator/(const momentum_t<base_type>& val, const speed_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::mass_t<base_type>{_v1.val() / _v2.val(), _v1.mult() / _v2.mult()};
        }


        template <std::floating_point base_type>
        [[nodiscard]]
        auto operator*(const momentum_t<base_type>& val, const speed_t<base_type>& other) {
            auto _v1 = val.convert_offset(0);
            auto _v2 = other.convert_offset(0);
            return sakurajin::unit_system::energy_t<base_type>{_v1.val() * _v2.val(), _v1.mult() / _v2.mult()};
        }


        // forward declare all units

        typedef time_si_t<UNIT_SYSTEM_DEFAULT_TYPE> time_si;

        typedef length_t<UNIT_SYSTEM_DEFAULT_TYPE> length;

        typedef mass_t<UNIT_SYSTEM_DEFAULT_TYPE> mass;

        typedef temperature_t<UNIT_SYSTEM_DEFAULT_TYPE> temperature;

        typedef amount_t<UNIT_SYSTEM_DEFAULT_TYPE> amount;

        typedef electric_current_t<UNIT_SYSTEM_DEFAULT_TYPE> electric_current;

        typedef luminous_intensity_t<UNIT_SYSTEM_DEFAULT_TYPE> luminous_intensity;

        typedef energy_t<UNIT_SYSTEM_DEFAULT_TYPE> energy;

        typedef power_t<UNIT_SYSTEM_DEFAULT_TYPE> power;

        typedef speed_t<UNIT_SYSTEM_DEFAULT_TYPE> speed;

        typedef acceleration_t<UNIT_SYSTEM_DEFAULT_TYPE> acceleration;

        typedef area_t<UNIT_SYSTEM_DEFAULT_TYPE> area;

        typedef force_t<UNIT_SYSTEM_DEFAULT_TYPE> force;

        typedef momentum_t<UNIT_SYSTEM_DEFAULT_TYPE> momentum;


        // define all literals
        inline namespace literals {


            inline auto operator"" _a(long double val) {
                return sakurajin::unit_system::time_si{val, 31536000000.0, 0.0};
            }
            inline auto operator"" _a(unsigned long long int val) {
                return sakurajin::unit_system::time_si{static_cast<long double>(val), 31536000000.0, 0.0};
            }

            inline auto operator"" _d(long double val) {
                return sakurajin::unit_system::time_si{val, 86400000.0, 0.0};
            }
            inline auto operator"" _d(unsigned long long int val) {
                return sakurajin::unit_system::time_si{static_cast<long double>(val), 86400000.0, 0.0};
            }

            inline auto operator"" _h(long double val) {
                return sakurajin::unit_system::time_si{val, 3600.0, 0.0};
            }
            inline auto operator"" _h(unsigned long long int val) {
                return sakurajin::unit_system::time_si{static_cast<long double>(val), 3600.0, 0.0};
            }

            inline auto operator"" _minute(long double val) {
                return sakurajin::unit_system::time_si{val, 60.0, 0.0};
            }
            inline auto operator"" _minute(unsigned long long int val) {
                return sakurajin::unit_system::time_si{static_cast<long double>(val), 60.0, 0.0};
            }

            inline auto operator"" _s(long double val) {
                return sakurajin::unit_system::time_si{val, 1.0, 0.0};
            }
            inline auto operator"" _s(unsigned long long int val) {
                return sakurajin::unit_system::time_si{static_cast<long double>(val), 1.0, 0.0};
            }

            inline auto operator"" _ms(long double val) {
                return sakurajin::unit_system::time_si{val, 0.001, 0.0};
            }
            inline auto operator"" _ms(unsigned long long int val) {
                return sakurajin::unit_system::time_si{static_cast<long double>(val), 0.001, 0.0};
            }

            inline auto operator"" _us(long double val) {
                return sakurajin::unit_system::time_si{val, 1e-06, 0.0};
            }
            inline auto operator"" _us(unsigned long long int val) {
                return sakurajin::unit_system::time_si{static_cast<long double>(val), 1e-06, 0.0};
            }

            inline auto operator"" _ns(long double val) {
                return sakurajin::unit_system::time_si{val, 1e-09, 0.0};
            }
            inline auto operator"" _ns(unsigned long long int val) {
                return sakurajin::unit_system::time_si{static_cast<long double>(val), 1e-09, 0.0};
            }

            inline auto operator"" _ps(long double val) {
                return sakurajin::unit_system::time_si{val, 1e-12, 0.0};
            }
            inline auto operator"" _ps(unsigned long long int val) {
                return sakurajin::unit_system::time_si{static_cast<long double>(val), 1e-12, 0.0};
            }

            inline auto operator"" _fs(long double val) {
                return sakurajin::unit_system::time_si{val, 1e-15, 0.0};
            }
            inline auto operator"" _fs(unsigned long long int val) {
                return sakurajin::unit_system::time_si{static_cast<long double>(val), 1e-15, 0.0};
            }

            inline auto operator"" _as(long double val) {
                return sakurajin::unit_system::time_si{val, 1e-18, 0.0};
            }
            inline auto operator"" _as(unsigned long long int val) {
                return sakurajin::unit_system::time_si{static_cast<long double>(val), 1e-18, 0.0};
            }


            inline auto operator"" _m(long double val) {
                return sakurajin::unit_system::length{val, 1.0, 0.0};
            }
            inline auto operator"" _m(unsigned long long int val) {
                return sakurajin::unit_system::length{static_cast<long double>(val), 1.0, 0.0};
            }

            inline auto operator"" _km(long double val) {
                return sakurajin::unit_system::length{val, 1000.0, 0.0};
            }
            inline auto operator"" _km(unsigned long long int val) {
                return sakurajin::unit_system::length{static_cast<long double>(val), 1000.0, 0.0};
            }

            inline auto operator"" _dm(long double val) {
                return sakurajin::unit_system::length{val, 0.1, 0.0};
            }
            inline auto operator"" _dm(unsigned long long int val) {
                return sakurajin::unit_system::length{static_cast<long double>(val), 0.1, 0.0};
            }

            inline auto operator"" _cm(long double val) {
                return sakurajin::unit_system::length{val, 0.01, 0.0};
            }
            inline auto operator"" _cm(unsigned long long int val) {
                return sakurajin::unit_system::length{static_cast<long double>(val), 0.01, 0.0};
            }

            inline auto operator"" _mm(long double val) {
                return sakurajin::unit_system::length{val, 0.001, 0.0};
            }
            inline auto operator"" _mm(unsigned long long int val) {
                return sakurajin::unit_system::length{static_cast<long double>(val), 0.001, 0.0};
            }

            inline auto operator"" _um(long double val) {
                return sakurajin::unit_system::length{val, 1e-06, 0.0};
            }
            inline auto operator"" _um(unsigned long long int val) {
                return sakurajin::unit_system::length{static_cast<long double>(val), 1e-06, 0.0};
            }

            inline auto operator"" _nm(long double val) {
                return sakurajin::unit_system::length{val, 1e-09, 0.0};
            }
            inline auto operator"" _nm(unsigned long long int val) {
                return sakurajin::unit_system::length{static_cast<long double>(val), 1e-09, 0.0};
            }

            inline auto operator"" _pm(long double val) {
                return sakurajin::unit_system::length{val, 1e-12, 0.0};
            }
            inline auto operator"" _pm(unsigned long long int val) {
                return sakurajin::unit_system::length{static_cast<long double>(val), 1e-12, 0.0};
            }

            inline auto operator"" _fm(long double val) {
                return sakurajin::unit_system::length{val, 1e-15, 0.0};
            }
            inline auto operator"" _fm(unsigned long long int val) {
                return sakurajin::unit_system::length{static_cast<long double>(val), 1e-15, 0.0};
            }

            inline auto operator"" _am(long double val) {
                return sakurajin::unit_system::length{val, 1e-18, 0.0};
            }
            inline auto operator"" _am(unsigned long long int val) {
                return sakurajin::unit_system::length{static_cast<long double>(val), 1e-18, 0.0};
            }


            inline auto operator"" _t(long double val) {
                return sakurajin::unit_system::mass{val, 1000.0, 0.0};
            }
            inline auto operator"" _t(unsigned long long int val) {
                return sakurajin::unit_system::mass{static_cast<long double>(val), 1000.0, 0.0};
            }

            inline auto operator"" _kg(long double val) {
                return sakurajin::unit_system::mass{val, 1.0, 0.0};
            }
            inline auto operator"" _kg(unsigned long long int val) {
                return sakurajin::unit_system::mass{static_cast<long double>(val), 1.0, 0.0};
            }

            inline auto operator"" _g(long double val) {
                return sakurajin::unit_system::mass{val, 0.001, 0.0};
            }
            inline auto operator"" _g(unsigned long long int val) {
                return sakurajin::unit_system::mass{static_cast<long double>(val), 0.001, 0.0};
            }

            inline auto operator"" _mg(long double val) {
                return sakurajin::unit_system::mass{val, 1e-06, 0.0};
            }
            inline auto operator"" _mg(unsigned long long int val) {
                return sakurajin::unit_system::mass{static_cast<long double>(val), 1e-06, 0.0};
            }

            inline auto operator"" _ug(long double val) {
                return sakurajin::unit_system::mass{val, 1e-09, 0.0};
            }
            inline auto operator"" _ug(unsigned long long int val) {
                return sakurajin::unit_system::mass{static_cast<long double>(val), 1e-09, 0.0};
            }

            inline auto operator"" _ng(long double val) {
                return sakurajin::unit_system::mass{val, 1.0000000000000002e-12, 0.0};
            }
            inline auto operator"" _ng(unsigned long long int val) {
                return sakurajin::unit_system::mass{static_cast<long double>(val), 1.0000000000000002e-12, 0.0};
            }

            inline auto operator"" _pg(long double val) {
                return sakurajin::unit_system::mass{val, 1e-15, 0.0};
            }
            inline auto operator"" _pg(unsigned long long int val) {
                return sakurajin::unit_system::mass{static_cast<long double>(val), 1e-15, 0.0};
            }

            inline auto operator"" _fg(long double val) {
                return sakurajin::unit_system::mass{val, 1e-18, 0.0};
            }
            inline auto operator"" _fg(unsigned long long int val) {
                return sakurajin::unit_system::mass{static_cast<long double>(val), 1e-18, 0.0};
            }

            inline auto operator"" _ag(long double val) {
                return sakurajin::unit_system::mass{val, 1.0000000000000001e-21, 0.0};
            }
            inline auto operator"" _ag(unsigned long long int val) {
                return sakurajin::unit_system::mass{static_cast<long double>(val), 1.0000000000000001e-21, 0.0};
            }


            inline auto operator"" _K(long double val) {
                return sakurajin::unit_system::temperature{val, 1.0, 0.0};
            }
            inline auto operator"" _K(unsigned long long int val) {
                return sakurajin::unit_system::temperature{static_cast<long double>(val), 1.0, 0.0};
            }

            inline auto operator"" _C(long double val) {
                return sakurajin::unit_system::temperature{val, 1.0, 273.15};
            }
            inline auto operator"" _C(unsigned long long int val) {
                return sakurajin::unit_system::temperature{static_cast<long double>(val), 1.0, 273.15};
            }


            inline auto operator"" _mol(long double val) {
                return sakurajin::unit_system::amount{val, 6.02214076e+23, 0.0};
            }
            inline auto operator"" _mol(unsigned long long int val) {
                return sakurajin::unit_system::amount{static_cast<long double>(val), 6.02214076e+23, 0.0};
            }

            inline auto operator"" _things(long double val) {
                return sakurajin::unit_system::amount{val, 1.0, 0.0};
            }
            inline auto operator"" _things(unsigned long long int val) {
                return sakurajin::unit_system::amount{static_cast<long double>(val), 1.0, 0.0};
            }


            inline auto operator"" _A(long double val) {
                return sakurajin::unit_system::electric_current{val, 1.0, 0.0};
            }
            inline auto operator"" _A(unsigned long long int val) {
                return sakurajin::unit_system::electric_current{static_cast<long double>(val), 1.0, 0.0};
            }

            inline auto operator"" _PA(long double val) {
                return sakurajin::unit_system::electric_current{val, 1000000000000000.0, 0.0};
            }
            inline auto operator"" _PA(unsigned long long int val) {
                return sakurajin::unit_system::electric_current{static_cast<long double>(val), 1000000000000000.0, 0.0};
            }

            inline auto operator"" _TA(long double val) {
                return sakurajin::unit_system::electric_current{val, 1000000000000.0, 0.0};
            }
            inline auto operator"" _TA(unsigned long long int val) {
                return sakurajin::unit_system::electric_current{static_cast<long double>(val), 1000000000000.0, 0.0};
            }

            inline auto operator"" _GA(long double val) {
                return sakurajin::unit_system::electric_current{val, 1000000000.0, 0.0};
            }
            inline auto operator"" _GA(unsigned long long int val) {
                return sakurajin::unit_system::electric_current{static_cast<long double>(val), 1000000000.0, 0.0};
            }

            inline auto operator"" _MA(long double val) {
                return sakurajin::unit_system::electric_current{val, 1000000.0, 0.0};
            }
            inline auto operator"" _MA(unsigned long long int val) {
                return sakurajin::unit_system::electric_current{static_cast<long double>(val), 1000000.0, 0.0};
            }

            inline auto operator"" _kA(long double val) {
                return sakurajin::unit_system::electric_current{val, 1000.0, 0.0};
            }
            inline auto operator"" _kA(unsigned long long int val) {
                return sakurajin::unit_system::electric_current{static_cast<long double>(val), 1000.0, 0.0};
            }

            inline auto operator"" _mA(long double val) {
                return sakurajin::unit_system::electric_current{val, 0.001, 0.0};
            }
            inline auto operator"" _mA(unsigned long long int val) {
                return sakurajin::unit_system::electric_current{static_cast<long double>(val), 0.001, 0.0};
            }

            inline auto operator"" _uA(long double val) {
                return sakurajin::unit_system::electric_current{val, 1e-06, 0.0};
            }
            inline auto operator"" _uA(unsigned long long int val) {
                return sakurajin::unit_system::electric_current{static_cast<long double>(val), 1e-06, 0.0};
            }

            inline auto operator"" _nA(long double val) {
                return sakurajin::unit_system::electric_current{val, 1e-09, 0.0};
            }
            inline auto operator"" _nA(unsigned long long int val) {
                return sakurajin::unit_system::electric_current{static_cast<long double>(val), 1e-09, 0.0};
            }

            inline auto operator"" _pA(long double val) {
                return sakurajin::unit_system::electric_current{val, 1e-12, 0.0};
            }
            inline auto operator"" _pA(unsigned long long int val) {
                return sakurajin::unit_system::electric_current{static_cast<long double>(val), 1e-12, 0.0};
            }

            inline auto operator"" _fA(long double val) {
                return sakurajin::unit_system::electric_current{val, 1e-15, 0.0};
            }
            inline auto operator"" _fA(unsigned long long int val) {
                return sakurajin::unit_system::electric_current{static_cast<long double>(val), 1e-15, 0.0};
            }

            inline auto operator"" _aA(long double val) {
                return sakurajin::unit_system::electric_current{val, 1e-18, 0.0};
            }
            inline auto operator"" _aA(unsigned long long int val) {
                return sakurajin::unit_system::electric_current{static_cast<long double>(val), 1e-18, 0.0};
            }


            inline auto operator"" _cd(long double val) {
                return sakurajin::unit_system::luminous_intensity{val, 1.0, 0.0};
            }
            inline auto operator"" _cd(unsigned long long int val) {
                return sakurajin::unit_system::luminous_intensity{static_cast<long double>(val), 1.0, 0.0};
            }


            inline auto operator"" _J(long double val) {
                return sakurajin::unit_system::energy{val, 1.0, 0.0};
            }
            inline auto operator"" _J(unsigned long long int val) {
                return sakurajin::unit_system::energy{static_cast<long double>(val), 1.0, 0.0};
            }

            inline auto operator"" _Nm(long double val) {
                return sakurajin::unit_system::energy{val, 1.0, 0.0};
            }
            inline auto operator"" _Nm(unsigned long long int val) {
                return sakurajin::unit_system::energy{static_cast<long double>(val), 1.0, 0.0};
            }

            inline auto operator"" _eV(long double val) {
                return sakurajin::unit_system::energy{val, 1.602176634e-19, 0.0};
            }
            inline auto operator"" _eV(unsigned long long int val) {
                return sakurajin::unit_system::energy{static_cast<long double>(val), 1.602176634e-19, 0.0};
            }

            inline auto operator"" _Wh(long double val) {
                return sakurajin::unit_system::energy{val, 3600.0, 0.0};
            }
            inline auto operator"" _Wh(unsigned long long int val) {
                return sakurajin::unit_system::energy{static_cast<long double>(val), 3600.0, 0.0};
            }

            inline auto operator"" _Ws(long double val) {
                return sakurajin::unit_system::energy{val, 1.0, 0.0};
            }
            inline auto operator"" _Ws(unsigned long long int val) {
                return sakurajin::unit_system::energy{static_cast<long double>(val), 1.0, 0.0};
            }

            inline auto operator"" _PJ(long double val) {
                return sakurajin::unit_system::energy{val, 1000000000000000.0, 0.0};
            }
            inline auto operator"" _PJ(unsigned long long int val) {
                return sakurajin::unit_system::energy{static_cast<long double>(val), 1000000000000000.0, 0.0};
            }

            inline auto operator"" _TJ(long double val) {
                return sakurajin::unit_system::energy{val, 1000000000000.0, 0.0};
            }
            inline auto operator"" _TJ(unsigned long long int val) {
                return sakurajin::unit_system::energy{static_cast<long double>(val), 1000000000000.0, 0.0};
            }

            inline auto operator"" _GJ(long double val) {
                return sakurajin::unit_system::energy{val, 1000000000.0, 0.0};
            }
            inline auto operator"" _GJ(unsigned long long int val) {
                return sakurajin::unit_system::energy{static_cast<long double>(val), 1000000000.0, 0.0};
            }

            inline auto operator"" _MJ(long double val) {
                return sakurajin::unit_system::energy{val, 1000000.0, 0.0};
            }
            inline auto operator"" _MJ(unsigned long long int val) {
                return sakurajin::unit_system::energy{static_cast<long double>(val), 1000000.0, 0.0};
            }

            inline auto operator"" _kJ(long double val) {
                return sakurajin::unit_system::energy{val, 1000.0, 0.0};
            }
            inline auto operator"" _kJ(unsigned long long int val) {
                return sakurajin::unit_system::energy{static_cast<long double>(val), 1000.0, 0.0};
            }

            inline auto operator"" _mJ(long double val) {
                return sakurajin::unit_system::energy{val, 0.001, 0.0};
            }
            inline auto operator"" _mJ(unsigned long long int val) {
                return sakurajin::unit_system::energy{static_cast<long double>(val), 0.001, 0.0};
            }

            inline auto operator"" _uJ(long double val) {
                return sakurajin::unit_system::energy{val, 1e-06, 0.0};
            }
            inline auto operator"" _uJ(unsigned long long int val) {
                return sakurajin::unit_system::energy{static_cast<long double>(val), 1e-06, 0.0};
            }

            inline auto operator"" _nJ(long double val) {
                return sakurajin::unit_system::energy{val, 1e-09, 0.0};
            }
            inline auto operator"" _nJ(unsigned long long int val) {
                return sakurajin::unit_system::energy{static_cast<long double>(val), 1e-09, 0.0};
            }

            inline auto operator"" _pJ(long double val) {
                return sakurajin::unit_system::energy{val, 1e-12, 0.0};
            }
            inline auto operator"" _pJ(unsigned long long int val) {
                return sakurajin::unit_system::energy{static_cast<long double>(val), 1e-12, 0.0};
            }

            inline auto operator"" _fJ(long double val) {
                return sakurajin::unit_system::energy{val, 1e-15, 0.0};
            }
            inline auto operator"" _fJ(unsigned long long int val) {
                return sakurajin::unit_system::energy{static_cast<long double>(val), 1e-15, 0.0};
            }

            inline auto operator"" _aJ(long double val) {
                return sakurajin::unit_system::energy{val, 1e-18, 0.0};
            }
            inline auto operator"" _aJ(unsigned long long int val) {
                return sakurajin::unit_system::energy{static_cast<long double>(val), 1e-18, 0.0};
            }

            inline auto operator"" _GNm(long double val) {
                return sakurajin::unit_system::energy{val, 1000000000.0, 0.0};
            }
            inline auto operator"" _GNm(unsigned long long int val) {
                return sakurajin::unit_system::energy{static_cast<long double>(val), 1000000000.0, 0.0};
            }

            inline auto operator"" _MNm(long double val) {
                return sakurajin::unit_system::energy{val, 1000000.0, 0.0};
            }
            inline auto operator"" _MNm(unsigned long long int val) {
                return sakurajin::unit_system::energy{static_cast<long double>(val), 1000000.0, 0.0};
            }

            inline auto operator"" _kNm(long double val) {
                return sakurajin::unit_system::energy{val, 1000.0, 0.0};
            }
            inline auto operator"" _kNm(unsigned long long int val) {
                return sakurajin::unit_system::energy{static_cast<long double>(val), 1000.0, 0.0};
            }

            inline auto operator"" _mNm(long double val) {
                return sakurajin::unit_system::energy{val, 0.001, 0.0};
            }
            inline auto operator"" _mNm(unsigned long long int val) {
                return sakurajin::unit_system::energy{static_cast<long double>(val), 0.001, 0.0};
            }

            inline auto operator"" _uNm(long double val) {
                return sakurajin::unit_system::energy{val, 1e-06, 0.0};
            }
            inline auto operator"" _uNm(unsigned long long int val) {
                return sakurajin::unit_system::energy{static_cast<long double>(val), 1e-06, 0.0};
            }

            inline auto operator"" _PeV(long double val) {
                return sakurajin::unit_system::energy{val, 0.0001602176634, 0.0};
            }
            inline auto operator"" _PeV(unsigned long long int val) {
                return sakurajin::unit_system::energy{static_cast<long double>(val), 0.0001602176634, 0.0};
            }

            inline auto operator"" _TeV(long double val) {
                return sakurajin::unit_system::energy{val, 1.602176634e-07, 0.0};
            }
            inline auto operator"" _TeV(unsigned long long int val) {
                return sakurajin::unit_system::energy{static_cast<long double>(val), 1.602176634e-07, 0.0};
            }

            inline auto operator"" _GeV(long double val) {
                return sakurajin::unit_system::energy{val, 1.6021766339999998e-10, 0.0};
            }
            inline auto operator"" _GeV(unsigned long long int val) {
                return sakurajin::unit_system::energy{static_cast<long double>(val), 1.6021766339999998e-10, 0.0};
            }

            inline auto operator"" _MeV(long double val) {
                return sakurajin::unit_system::energy{val, 1.6021766339999998e-13, 0.0};
            }
            inline auto operator"" _MeV(unsigned long long int val) {
                return sakurajin::unit_system::energy{static_cast<long double>(val), 1.6021766339999998e-13, 0.0};
            }

            inline auto operator"" _keV(long double val) {
                return sakurajin::unit_system::energy{val, 1.602176634e-16, 0.0};
            }
            inline auto operator"" _keV(unsigned long long int val) {
                return sakurajin::unit_system::energy{static_cast<long double>(val), 1.602176634e-16, 0.0};
            }

            inline auto operator"" _meV(long double val) {
                return sakurajin::unit_system::energy{val, 1.6021766339999998e-22, 0.0};
            }
            inline auto operator"" _meV(unsigned long long int val) {
                return sakurajin::unit_system::energy{static_cast<long double>(val), 1.6021766339999998e-22, 0.0};
            }

            inline auto operator"" _ueV(long double val) {
                return sakurajin::unit_system::energy{val, 1.602176634e-25, 0.0};
            }
            inline auto operator"" _ueV(unsigned long long int val) {
                return sakurajin::unit_system::energy{static_cast<long double>(val), 1.602176634e-25, 0.0};
            }

            inline auto operator"" _PWh(long double val) {
                return sakurajin::unit_system::energy{val, 3.6e+18, 0.0};
            }
            inline auto operator"" _PWh(unsigned long long int val) {
                return sakurajin::unit_system::energy{static_cast<long double>(val), 3.6e+18, 0.0};
            }

            inline auto operator"" _TWh(long double val) {
                return sakurajin::unit_system::energy{val, 3600000000000000.0, 0.0};
            }
            inline auto operator"" _TWh(unsigned long long int val) {
                return sakurajin::unit_system::energy{static_cast<long double>(val), 3600000000000000.0, 0.0};
            }

            inline auto operator"" _GWh(long double val) {
                return sakurajin::unit_system::energy{val, 3600000000000.0, 0.0};
            }
            inline auto operator"" _GWh(unsigned long long int val) {
                return sakurajin::unit_system::energy{static_cast<long double>(val), 3600000000000.0, 0.0};
            }

            inline auto operator"" _MWh(long double val) {
                return sakurajin::unit_system::energy{val, 3600000000.0, 0.0};
            }
            inline auto operator"" _MWh(unsigned long long int val) {
                return sakurajin::unit_system::energy{static_cast<long double>(val), 3600000000.0, 0.0};
            }

            inline auto operator"" _kWh(long double val) {
                return sakurajin::unit_system::energy{val, 3600000.0, 0.0};
            }
            inline auto operator"" _kWh(unsigned long long int val) {
                return sakurajin::unit_system::energy{static_cast<long double>(val), 3600000.0, 0.0};
            }


            inline auto operator"" _W(long double val) {
                return sakurajin::unit_system::power{val, 1.0, 0.0};
            }
            inline auto operator"" _W(unsigned long long int val) {
                return sakurajin::unit_system::power{static_cast<long double>(val), 1.0, 0.0};
            }

            inline auto operator"" _PW(long double val) {
                return sakurajin::unit_system::power{val, 1000000000000000.0, 0.0};
            }
            inline auto operator"" _PW(unsigned long long int val) {
                return sakurajin::unit_system::power{static_cast<long double>(val), 1000000000000000.0, 0.0};
            }

            inline auto operator"" _TW(long double val) {
                return sakurajin::unit_system::power{val, 1000000000000.0, 0.0};
            }
            inline auto operator"" _TW(unsigned long long int val) {
                return sakurajin::unit_system::power{static_cast<long double>(val), 1000000000000.0, 0.0};
            }

            inline auto operator"" _GW(long double val) {
                return sakurajin::unit_system::power{val, 1000000000.0, 0.0};
            }
            inline auto operator"" _GW(unsigned long long int val) {
                return sakurajin::unit_system::power{static_cast<long double>(val), 1000000000.0, 0.0};
            }

            inline auto operator"" _MW(long double val) {
                return sakurajin::unit_system::power{val, 1000000.0, 0.0};
            }
            inline auto operator"" _MW(unsigned long long int val) {
                return sakurajin::unit_system::power{static_cast<long double>(val), 1000000.0, 0.0};
            }

            inline auto operator"" _kW(long double val) {
                return sakurajin::unit_system::power{val, 1000.0, 0.0};
            }
            inline auto operator"" _kW(unsigned long long int val) {
                return sakurajin::unit_system::power{static_cast<long double>(val), 1000.0, 0.0};
            }

            inline auto operator"" _mW(long double val) {
                return sakurajin::unit_system::power{val, 0.001, 0.0};
            }
            inline auto operator"" _mW(unsigned long long int val) {
                return sakurajin::unit_system::power{static_cast<long double>(val), 0.001, 0.0};
            }

            inline auto operator"" _uW(long double val) {
                return sakurajin::unit_system::power{val, 1e-06, 0.0};
            }
            inline auto operator"" _uW(unsigned long long int val) {
                return sakurajin::unit_system::power{static_cast<long double>(val), 1e-06, 0.0};
            }

            inline auto operator"" _nW(long double val) {
                return sakurajin::unit_system::power{val, 1e-09, 0.0};
            }
            inline auto operator"" _nW(unsigned long long int val) {
                return sakurajin::unit_system::power{static_cast<long double>(val), 1e-09, 0.0};
            }

            inline auto operator"" _pW(long double val) {
                return sakurajin::unit_system::power{val, 1e-12, 0.0};
            }
            inline auto operator"" _pW(unsigned long long int val) {
                return sakurajin::unit_system::power{static_cast<long double>(val), 1e-12, 0.0};
            }

            inline auto operator"" _fW(long double val) {
                return sakurajin::unit_system::power{val, 1e-15, 0.0};
            }
            inline auto operator"" _fW(unsigned long long int val) {
                return sakurajin::unit_system::power{static_cast<long double>(val), 1e-15, 0.0};
            }

            inline auto operator"" _aW(long double val) {
                return sakurajin::unit_system::power{val, 1e-18, 0.0};
            }
            inline auto operator"" _aW(unsigned long long int val) {
                return sakurajin::unit_system::power{static_cast<long double>(val), 1e-18, 0.0};
            }


            inline auto operator"" _mps(long double val) {
                return sakurajin::unit_system::speed{val, 1.0, 0.0};
            }
            inline auto operator"" _mps(unsigned long long int val) {
                return sakurajin::unit_system::speed{static_cast<long double>(val), 1.0, 0.0};
            }

            inline auto operator"" _kmph(long double val) {
                return sakurajin::unit_system::speed{val, 0.2777777777777778, 0.0};
            }
            inline auto operator"" _kmph(unsigned long long int val) {
                return sakurajin::unit_system::speed{static_cast<long double>(val), 0.2777777777777778, 0.0};
            }


            inline auto operator"" _mps2(long double val) {
                return sakurajin::unit_system::acceleration{val, 1.0, 0.0};
            }
            inline auto operator"" _mps2(unsigned long long int val) {
                return sakurajin::unit_system::acceleration{static_cast<long double>(val), 1.0, 0.0};
            }

            inline auto operator"" _G(long double val) {
                return sakurajin::unit_system::acceleration{val, 9.80665, 0.0};
            }
            inline auto operator"" _G(unsigned long long int val) {
                return sakurajin::unit_system::acceleration{static_cast<long double>(val), 9.80665, 0.0};
            }


            inline auto operator"" _m2(long double val) {
                return sakurajin::unit_system::area{val, 1.0, 0.0};
            }
            inline auto operator"" _m2(unsigned long long int val) {
                return sakurajin::unit_system::area{static_cast<long double>(val), 1.0, 0.0};
            }

            inline auto operator"" _are(long double val) {
                return sakurajin::unit_system::area{val, 100.0, 0.0};
            }
            inline auto operator"" _are(unsigned long long int val) {
                return sakurajin::unit_system::area{static_cast<long double>(val), 100.0, 0.0};
            }

            inline auto operator"" _hectare(long double val) {
                return sakurajin::unit_system::area{val, 10000.0, 0.0};
            }
            inline auto operator"" _hectare(unsigned long long int val) {
                return sakurajin::unit_system::area{static_cast<long double>(val), 10000.0, 0.0};
            }

            inline auto operator"" _km2(long double val) {
                return sakurajin::unit_system::area{val, 1000000.0, 0.0};
            }
            inline auto operator"" _km2(unsigned long long int val) {
                return sakurajin::unit_system::area{static_cast<long double>(val), 1000000.0, 0.0};
            }

            inline auto operator"" _mm2(long double val) {
                return sakurajin::unit_system::area{val, 1e-06, 0.0};
            }
            inline auto operator"" _mm2(unsigned long long int val) {
                return sakurajin::unit_system::area{static_cast<long double>(val), 1e-06, 0.0};
            }

            inline auto operator"" _um2(long double val) {
                return sakurajin::unit_system::area{val, 1e-12, 0.0};
            }
            inline auto operator"" _um2(unsigned long long int val) {
                return sakurajin::unit_system::area{static_cast<long double>(val), 1e-12, 0.0};
            }

            inline auto operator"" _nm2(long double val) {
                return sakurajin::unit_system::area{val, 1e-18, 0.0};
            }
            inline auto operator"" _nm2(unsigned long long int val) {
                return sakurajin::unit_system::area{static_cast<long double>(val), 1e-18, 0.0};
            }


            inline auto operator"" _N(long double val) {
                return sakurajin::unit_system::force{val, 1.0, 0.0};
            }
            inline auto operator"" _N(unsigned long long int val) {
                return sakurajin::unit_system::force{static_cast<long double>(val), 1.0, 0.0};
            }

            inline auto operator"" _PN(long double val) {
                return sakurajin::unit_system::force{val, 1000000000000000.0, 0.0};
            }
            inline auto operator"" _PN(unsigned long long int val) {
                return sakurajin::unit_system::force{static_cast<long double>(val), 1000000000000000.0, 0.0};
            }

            inline auto operator"" _TN(long double val) {
                return sakurajin::unit_system::force{val, 1000000000000.0, 0.0};
            }
            inline auto operator"" _TN(unsigned long long int val) {
                return sakurajin::unit_system::force{static_cast<long double>(val), 1000000000000.0, 0.0};
            }

            inline auto operator"" _GN(long double val) {
                return sakurajin::unit_system::force{val, 1000000000.0, 0.0};
            }
            inline auto operator"" _GN(unsigned long long int val) {
                return sakurajin::unit_system::force{static_cast<long double>(val), 1000000000.0, 0.0};
            }

            inline auto operator"" _MN(long double val) {
                return sakurajin::unit_system::force{val, 1000000.0, 0.0};
            }
            inline auto operator"" _MN(unsigned long long int val) {
                return sakurajin::unit_system::force{static_cast<long double>(val), 1000000.0, 0.0};
            }

            inline auto operator"" _kN(long double val) {
                return sakurajin::unit_system::force{val, 1000.0, 0.0};
            }
            inline auto operator"" _kN(unsigned long long int val) {
                return sakurajin::unit_system::force{static_cast<long double>(val), 1000.0, 0.0};
            }

            inline auto operator"" _mN(long double val) {
                return sakurajin::unit_system::force{val, 0.001, 0.0};
            }
            inline auto operator"" _mN(unsigned long long int val) {
                return sakurajin::unit_system::force{static_cast<long double>(val), 0.001, 0.0};
            }

            inline auto operator"" _uN(long double val) {
                return sakurajin::unit_system::force{val, 1e-06, 0.0};
            }
            inline auto operator"" _uN(unsigned long long int val) {
                return sakurajin::unit_system::force{static_cast<long double>(val), 1e-06, 0.0};
            }

            inline auto operator"" _nN(long double val) {
                return sakurajin::unit_system::force{val, 1e-09, 0.0};
            }
            inline auto operator"" _nN(unsigned long long int val) {
                return sakurajin::unit_system::force{static_cast<long double>(val), 1e-09, 0.0};
            }

            inline auto operator"" _pN(long double val) {
                return sakurajin::unit_system::force{val, 1e-12, 0.0};
            }
            inline auto operator"" _pN(unsigned long long int val) {
                return sakurajin::unit_system::force{static_cast<long double>(val), 1e-12, 0.0};
            }

            inline auto operator"" _fN(long double val) {
                return sakurajin::unit_system::force{val, 1e-15, 0.0};
            }
            inline auto operator"" _fN(unsigned long long int val) {
                return sakurajin::unit_system::force{static_cast<long double>(val), 1e-15, 0.0};
            }

            inline auto operator"" _aN(long double val) {
                return sakurajin::unit_system::force{val, 1e-18, 0.0};
            }
            inline auto operator"" _aN(unsigned long long int val) {
                return sakurajin::unit_system::force{static_cast<long double>(val), 1e-18, 0.0};
            }


            inline auto operator"" _kgmps(long double val) {
                return sakurajin::unit_system::momentum{val, 1.0, 0.0};
            }
            inline auto operator"" _kgmps(unsigned long long int val) {
                return sakurajin::unit_system::momentum{static_cast<long double>(val), 1.0, 0.0};
            }


        } // namespace literals
    } // namespace unit_system
} // namespace sakurajin


namespace std {

    template <std::floating_point base_type>
    sakurajin::unit_system::time_si_t<base_type> abs(const sakurajin::unit_system::time_si_t<base_type>& unit) {
        const auto raw_val = unit.val();
        const auto inv_val = -raw_val;
        const auto abs_val = (raw_val > inv_val) ? raw_val : inv_val;
        return sakurajin::unit_system::time_si_t<base_type>{abs_val, unit.mult(), unit.off()};
    }

    template <std::floating_point base_type>
    sakurajin::unit_system::length_t<base_type> abs(const sakurajin::unit_system::length_t<base_type>& unit) {
        const auto raw_val = unit.val();
        const auto inv_val = -raw_val;
        const auto abs_val = (raw_val > inv_val) ? raw_val : inv_val;
        return sakurajin::unit_system::length_t<base_type>{abs_val, unit.mult(), unit.off()};
    }

    template <std::floating_point base_type>
    sakurajin::unit_system::mass_t<base_type> abs(const sakurajin::unit_system::mass_t<base_type>& unit) {
        const auto raw_val = unit.val();
        const auto inv_val = -raw_val;
        const auto abs_val = (raw_val > inv_val) ? raw_val : inv_val;
        return sakurajin::unit_system::mass_t<base_type>{abs_val, unit.mult(), unit.off()};
    }

    template <std::floating_point base_type>
    sakurajin::unit_system::temperature_t<base_type> abs(const sakurajin::unit_system::temperature_t<base_type>& unit) {
        const auto raw_val = unit.val();
        const auto inv_val = -raw_val;
        const auto abs_val = (raw_val > inv_val) ? raw_val : inv_val;
        return sakurajin::unit_system::temperature_t<base_type>{abs_val, unit.mult(), unit.off()};
    }

    template <std::floating_point base_type>
    sakurajin::unit_system::amount_t<base_type> abs(const sakurajin::unit_system::amount_t<base_type>& unit) {
        const auto raw_val = unit.val();
        const auto inv_val = -raw_val;
        const auto abs_val = (raw_val > inv_val) ? raw_val : inv_val;
        return sakurajin::unit_system::amount_t<base_type>{abs_val, unit.mult(), unit.off()};
    }

    template <std::floating_point base_type>
    sakurajin::unit_system::electric_current_t<base_type> abs(const sakurajin::unit_system::electric_current_t<base_type>& unit) {
        const auto raw_val = unit.val();
        const auto inv_val = -raw_val;
        const auto abs_val = (raw_val > inv_val) ? raw_val : inv_val;
        return sakurajin::unit_system::electric_current_t<base_type>{abs_val, unit.mult(), unit.off()};
    }

    template <std::floating_point base_type>
    sakurajin::unit_system::luminous_intensity_t<base_type> abs(const sakurajin::unit_system::luminous_intensity_t<base_type>& unit) {
        const auto raw_val = unit.val();
        const auto inv_val = -raw_val;
        const auto abs_val = (raw_val > inv_val) ? raw_val : inv_val;
        return sakurajin::unit_system::luminous_intensity_t<base_type>{abs_val, unit.mult(), unit.off()};
    }

    template <std::floating_point base_type>
    sakurajin::unit_system::energy_t<base_type> abs(const sakurajin::unit_system::energy_t<base_type>& unit) {
        const auto raw_val = unit.val();
        const auto inv_val = -raw_val;
        const auto abs_val = (raw_val > inv_val) ? raw_val : inv_val;
        return sakurajin::unit_system::energy_t<base_type>{abs_val, unit.mult(), unit.off()};
    }

    template <std::floating_point base_type>
    sakurajin::unit_system::power_t<base_type> abs(const sakurajin::unit_system::power_t<base_type>& unit) {
        const auto raw_val = unit.val();
        const auto inv_val = -raw_val;
        const auto abs_val = (raw_val > inv_val) ? raw_val : inv_val;
        return sakurajin::unit_system::power_t<base_type>{abs_val, unit.mult(), unit.off()};
    }

    template <std::floating_point base_type>
    sakurajin::unit_system::speed_t<base_type> abs(const sakurajin::unit_system::speed_t<base_type>& unit) {
        const auto raw_val = unit.val();
        const auto inv_val = -raw_val;
        const auto abs_val = (raw_val > inv_val) ? raw_val : inv_val;
        return sakurajin::unit_system::speed_t<base_type>{abs_val, unit.mult(), unit.off()};
    }

    template <std::floating_point base_type>
    sakurajin::unit_system::acceleration_t<base_type> abs(const sakurajin::unit_system::acceleration_t<base_type>& unit) {
        const auto raw_val = unit.val();
        const auto inv_val = -raw_val;
        const auto abs_val = (raw_val > inv_val) ? raw_val : inv_val;
        return sakurajin::unit_system::acceleration_t<base_type>{abs_val, unit.mult(), unit.off()};
    }

    template <std::floating_point base_type>
    sakurajin::unit_system::area_t<base_type> abs(const sakurajin::unit_system::area_t<base_type>& unit) {
        const auto raw_val = unit.val();
        const auto inv_val = -raw_val;
        const auto abs_val = (raw_val > inv_val) ? raw_val : inv_val;
        return sakurajin::unit_system::area_t<base_type>{abs_val, unit.mult(), unit.off()};
    }

    template <std::floating_point base_type>
    sakurajin::unit_system::force_t<base_type> abs(const sakurajin::unit_system::force_t<base_type>& unit) {
        const auto raw_val = unit.val();
        const auto inv_val = -raw_val;
        const auto abs_val = (raw_val > inv_val) ? raw_val : inv_val;
        return sakurajin::unit_system::force_t<base_type>{abs_val, unit.mult(), unit.off()};
    }

    template <std::floating_point base_type>
    sakurajin::unit_system::momentum_t<base_type> abs(const sakurajin::unit_system::momentum_t<base_type>& unit) {
        const auto raw_val = unit.val();
        const auto inv_val = -raw_val;
        const auto abs_val = (raw_val > inv_val) ? raw_val : inv_val;
        return sakurajin::unit_system::momentum_t<base_type>{abs_val, unit.mult(), unit.off()};
    }

} // namespace std


// add compatibility with std::chrono
namespace sakurajin {
    namespace unit_system {
        template <class Rep, class Period = std::ratio<1>, class mult_t = UNIT_SYSTEM_DEFAULT_TYPE>
            requires std::convertible_to<mult_t, UNIT_SYSTEM_DEFAULT_TYPE>
        time_si unit_cast(const std::chrono::duration<Rep, Period>& other, mult_t new_multiplier = 1.0) {
            auto t      = std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1, 1>>>(other);
            auto retval = time_si{t.count(), 1};
            return retval.convert_multiplier(new_multiplier);
        }
    } // namespace unit_system
} // namespace sakurajin


// add all constants
namespace sakurajin {
    namespace unit_system {
        namespace constants {

            const long double avogadro_constant = 6.02214076e+23;

            const long double waterFreezingPoint = 273.15;

        } // namespace constants
    } // namespace unit_system
} // namespace sakurajin