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

#include "FixedNum.h"

using namespace bWidgetsDemo;

// Instantiations needed to avoid linker errors.
template class bWidgetsDemo::FixedNum<F16p16>;
template class bWidgetsDemo::FixedNum<F26p6>;
template bWidgetsDemo::FixedNum<F26p6>::operator FixedNum<F16p16>() const;


template<typename _Type>
constexpr unsigned int getScaleFactor()
{
	// Simple, type safe bitshift, e.g. `1 << 16` for 16.16, `1L << 32` for
	// 32.32 fixed number. Note that this gets evaluated at compile time
	// (constexpr).
	return (typename _Type::value_type(1) << _Type::bits_fraction);
}

template<typename _Type>
FixedNum<_Type> FixedNum<_Type>::fromInt(typename _Type::value_type _value)
{
	return FixedNum(_value * getScaleFactor<_Type>());
}
template<typename _Type>
int FixedNum<_Type>::toInt() const
{
	return value / getScaleFactor<_Type>();
}

template<typename _Type>
double FixedNum<_Type>::toReal() const
{
	return double(value) / getScaleFactor<_Type>();
}

template<typename _Type>
double FixedNum<_Type>::getFractionAsReal() const
{
	return toReal() - toInt();
}

template<typename _Type>
FixedNum<_Type>& FixedNum<_Type>::round()
{
	value += getScaleFactor<_Type>() / 2;
	return floor();
}

template<typename _Type>
FixedNum<_Type>& FixedNum<_Type>::floor()
{
	value &= ~(getScaleFactor<_Type>() - 1);
	return *this;
}

template<typename _Type>
FixedNum<_Type>& FixedNum<_Type>::operator+=(const FixedNum& other)
{
	value += other.value;
	return *this;
}

template<typename _Type>
template<typename _OtherType>
FixedNum<_Type>::operator FixedNum<_OtherType>() const
{
	return FixedNum<_OtherType>(value * (double(getScaleFactor<_OtherType>()) / getScaleFactor<_Type>()));
}
