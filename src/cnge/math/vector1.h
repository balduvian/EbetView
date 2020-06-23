/*******************************************************************************
 *
 * Copyright (c) 2019 Gnarwhal
 *
 * -----------------------------------------------------------------------------
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files(the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *******************************************************************************/

#include "types.h"

#ifndef CNGE_VECTOR_FORWARD
#define CNGE_VECTOR_FORWARD

template <u32 N, typename T>
class Vector;

#endif // CNGE_VECTOR_FORWARD

#ifndef CNGE_VECTOR1
#define CNGE_VECTOR1

#include "Vector.h"

template <typename T> using Vector1 = Vector<1, T>;

using Vector1b  = Vector1< i8 >;
using Vector1ub = Vector1<u8 >;
using Vector1s  = Vector1< i16>;
using Vector1us = Vector1<u16>;
using Vector1i  = Vector1< i32>;
using Vector1ui = Vector1<u32>;
using Vector1l  = Vector1< i64>;
using Vector1ul = Vector1<u64>;
using Vector1f  = Vector1<float >;
using Vector1d  = Vector1<double>;

#define CNGE_VECTOR1_FORWARD
#endif // CNGE_VECTOR1
