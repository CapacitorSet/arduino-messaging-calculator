/*
 *  This file defines a postfix evaluation algorithm.
 *
 *  Copyright (C) 2010  Efstathios Chatzikyriakidis (contact@efxa.org)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

// header defining the interface of the source.
#ifndef _EVALUATE_POSTFIX_H
#define _EVALUATE_POSTFIX_H

// include Arduino basic header.
#include <Arduino.h>

// postfix expression evaluation algorithm.
bool evaluate_postfix (String & postfix, double & result);

#endif // _EVALUATE_POSTFIX_H
