/*
 *  This file defines some parsing tools related to operators.
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
#ifndef _PARSE_TOOLS_H
#define _PARSE_TOOLS_H

// include Arduino basic header.
#include <Arduino.h>

// get the number of the operands of an operator symbol.
int op_operands_count (const char c);

// get the precedence of an operator symbol.
int op_precedence (const char c);

// check if a character is an identifier symbol.
bool is_identifier (const char c);

// check if a character is an operator symbol.
bool is_operator (const char c);

#endif // _PARSE_TOOLS_H
