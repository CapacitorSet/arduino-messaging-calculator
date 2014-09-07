/*
 *  This file implements a postfix evaluation algorithm.
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

// include some external libraries' headers.
#include <StackList.h>

// include some calculator libraries' headers.
#include "evaluate_postfix.h"
#include "parse_tools.h"
#include "recovery_tools.h"

long powint(int factor, unsigned int exponent)
{
    long product = 1;
    while (exponent--)
       product *= factor;
    return product;
}

// postfix expression evaluation algorithm.
bool
evaluate_postfix (String & postfix, double & result) {
  StackList <double> stack; // the operands stack.
  char c;                   // the character parsed.

  // check if the expression is empty.
  if (postfix.length () <= 0) {
    #if SERIAL_DEBUG
      Serial.println("err1");
      // POSTFIX-EVALUATION: the expression is empty.
    #endif
    return false;
  }

  // set the printer of the stack.
  stack.setPrinter (Serial);

  // handle each character from the postfix expression.
  for (int i = 0; i < postfix.length (); i++) {
    // get the character.
    c = postfix.charAt (i);

    // if the character is not white space.
    if (c != ' ' && c != '\t') {
      // if the character is an identifier.
      if (is_identifier (c)) {
        // necessary for later reference.
        stack.push (0);

        // try to fetch / calculate a multi-digit integer number.
        for (; i < postfix.length () && is_identifier (c = postfix.charAt (i)); i++)
          // calculate the number so far with its digits.
          stack.push (10.0 * stack.pop () + (c - '0'));

        // fix the index in order to 'ungetch' the non-identifier character.
        i--;
      }
      // if the character is an operator.
      else if (is_operator (c)) {
        // get the number of operator's arguments.
        int nargs = op_operands_count (c);

        // if there aren't enough arguments in the stack.
        if (nargs > stack.count ()) {
          #if SERIAL_DEBUG
            Serial.println("err2");
            // POSTFIX-EVALUATION: not sufficient operator arguments in the expression.
          #endif
          return false;
        }

        // allocate enough memory for the arguments of the operator.
        double *vargs = (double *) malloc (sizeof (double) * nargs);

        // if there was memory allocation error.
        if (vargs == NULL)
          reset ("err3");
          // POSTFIX-EVALUATION: insufficient memory for storing operator arguments.

        // fetch all the arguments of the operator.
        for (int arg = 0; arg < nargs; arg++)
          vargs[arg] = stack.pop ();

        // evaluate the operator with its operands.
        
        switch (c){
          case '+':
            stack.push (vargs[1] + vargs[0]);
            break;

          case '-':
            stack.push (vargs[1] - vargs[0]);
            break;

          case '/':
            if (vargs[0] != 0)
              stack.push (vargs[1] / vargs[0]);
            else
              reset ("err3");
              // POSTFIX-EVALUATION: value division by zero.
            break;

          case '*':
            stack.push (vargs[1] * vargs[0]);
            break;

          case '.':
            if (vargs[0] == 0) {
              stack.push(vargs[1]);
            } else {
              stack.push (vargs[1] + (1/pow(10, ceil(log10(vargs[0]))))*vargs[0] * 10 - 1);
            }
            break;
            
          case 's':
            stack.push(sin(vargs[0]));
            break;
            
          case 'c':
            stack.push(cos(vargs[0]));
            break;
          
          case 't':
            stack.push(tan(vargs[0]));
            break;
            
          case 'S':
            stack.push(asin(vargs[0]));
            break;
          
          case 'C':
            stack.push(acos(vargs[0]));
            break;
          
          case 'T':
            stack.push(atan(vargs[0]));
            break;
            
          case '^':
            if ((vargs[0]-(int)vargs[0] == 0) && (vargs[1]-(int)vargs[1] == 0) && vargs[0] > 0) {
              stack.push (powint(vargs[1], vargs[0]));
            } else {
              stack.push (pow(vargs[1], vargs[0]));
            }
            break;
            
          case 'l':
            if (vargs[0] > 0) {
              stack.push(log(vargs[0]));
            } else {
              reset("ln of a nonpositive number");
            }
            break;
          
          case 'L':
            if (vargs[0] > 0) {
              stack.push(log10(vargs[0]));
            } else {
              reset("log of a nonpositive number");
            }
            break;
        }

        // deallocate memory for operands.
        free (vargs);
      } else {
        // the character is unknown.
        #if SERIAL_DEBUG
          Serial.println("err4");
          // POSTFIX-EVALUATION: there is an unknown token in the expression.
        #endif
        return false;
      }
    }
  }

  // if there is only one element in the stack.
  if (stack.count () == 1)
    // return the result of the expression.
    result = stack.pop ();
  else {
    #if SERIAL_DEBUG
      Serial.println("err5");
      // POSTFIX-EVALUATION: expression has too many values.
    #endif
    return false;
  }

  // postfix evaluation done.
  return true;
}
