/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Original work Copyright (c) 2018 Julian Eisel
 *
 * ***** END GPL LICENSE BLOCK *****
 */

#pragma once

#include <utility>

namespace bWidgetsDemo {

struct F26p6 {
  using value_type = int;
  static const int bits_number = 26;
  static const int bits_fraction = 6;
};
struct F16p16 {
  using value_type = int;
  static const int bits_number = 16;
  static const int bits_fraction = 16;
};

/**
 * \name Fixed Point Binary Number
 *
 * Although it's probably simpler to just pass around some ints and do
 * (bitwise) operations/conversions as needed, this class gives a much more
 * type save approach. It's easy to accidentally mix up conversions of raw ints
 * (i.e. doing `val >> 16` instead of `val >> 6`). But using this type, such
 * mistakes either lead to compile errors or invoke proper conversion.
 */
template<typename _Type> class FixedNum {
 public:
  FixedNum() = default;
  FixedNum(typename _Type::value_type _value) : value(_value)
  {
  }

  static_assert((_Type::bits_number + _Type::bits_fraction) <=
                    (sizeof(typename _Type::value_type) * 8),
                "value_type of given fixed point type does not provide enough bits to "
                "store its number and fractional part.");

  static FixedNum fromInt(typename _Type::value_type _value);
  int toInt() const;
  double toReal() const;
  double getFractionAsReal() const;
  FixedNum &round();
  FixedNum &floor();

  FixedNum &operator+=(const FixedNum &);
  template<typename _OtherType> operator FixedNum<_OtherType>() const;

  friend FixedNum operator+(const FixedNum &lhs, const FixedNum &rhs)
  {
    return lhs.value + rhs.value;
  }
  friend FixedNum operator-(const FixedNum &lhs, const FixedNum &rhs)
  {
    return lhs.value - rhs.value;
  }
  friend bool operator>(const FixedNum &lhs, const FixedNum &rhs)
  {
    return lhs.value > rhs.value;
  }

 private:
  typename _Type::value_type value{0};
};

}  // namespace bWidgetsDemo
