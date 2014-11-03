/*
 *  This file implements some data type conversion tools.
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

// include header defining the interface of the library.
#include "conversion_tools.h"

// produce a formatted string in a buffer corresponding to the double value provided.
char *
double_string (char *string, double value, int places, int width) {
  // variables used for the conversion.
  double tens = 0.1, temp_double = value;
  int digit, tens_count = 0, c = 0;
  int i, char_count = 1, extra = 0;

  // defensive issue.
  if (width < 0)
    width = 0;

  // defensive issue.
  if (places > 8)
    places = 8;
  else if (places < 0)
    places = 0;

  // make sure we round properly.

  // calculate rounding term d: 0.5 / pow (10, places).  
  double d = 0.5;
  if (value < 0)
    d *= -1.0;

  // divide by ten for each decimal place.
  for (i = 0; i < places; i++)
    d /= 10.0;

  // this small addition, combined with truncation will round our values properly.
  temp_double += d;

  // first get value tens to be the large power of ten less than value.
  if (value < 0)
    temp_double *= -1.0;

  while ((tens * 10.0) <= temp_double) {
    tens *= 10.0;
    tens_count += 1;
  }

  if (tens_count > 0)
    char_count += tens_count;
  else
    char_count += 1;

  if (value < 0)
    char_count += 1;

  char_count += 1 + places;

  // both count the null final character.
  width += 1;
  if (width > char_count) {
    extra = width - char_count;
    char_count = width;
  }

  // write out the negative if needed.
  if (value < 0)
    string[c++] = '-';

  if (tens_count == 0)
    string[c++] = '0';

  for (i = 0; i < tens_count; i++) {
    digit = (int) (temp_double / tens);
    itoa (digit, &string[c++], 10);
    temp_double = temp_double - ((double) digit * tens);
    tens /= 10.0;
  }

  // otherwise, write the point and continue on.
  if (places > 0)
    string[c++] = '.';

  // write out each decimal place by shifting digits (one by
  // one) into the ones place and write the truncated value.
  for (i = 0; i < places; i++) {
    temp_double *= 10.0;
    digit = (int) temp_double;
    itoa (digit, &string[c++], 10);

    // once written, subtract off that digit.
    temp_double = temp_double - (double) digit;
  }

  // terminate the string.
  string[c++] = '\0';

  // return the string.
  return string;
}

char* int_string(char* string, int value) {
  itoa(value, string, 10);
  return string;
}
