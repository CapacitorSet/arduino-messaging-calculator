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
#include "StackList.h"

// include some calculator libraries' headers.
#include "evaluate_postfix.h"
#include "parse_tools.h"

 int mcd(int a, int b) {
    for (;;)
    {
        if (a == 0) return b;
        b %= a;
        if (b == 0) return a;
        a %= b;
    }
}

int mcm(int a, int b) {
    int temp = mcd(a, b);
    return temp ? (a / temp * b) : 0;
}

void simplify(int &a, int &b) {
  int fattore = mcd(a, b);
  a = (int) a / fattore;
  b = (int) b / fattore;
}

void subtract(double numeratore1, int divisore1, double numeratore2, int divisore2, StackList<double> &numeratori, StackList<int> &divisori) {
  if (fmod(numeratore1, 1) != 0) { // If the first arg is a double
    numeratore2 = numeratore2 / divisore2;
              // Casts the second item to a double (no need to convert args_divisore[1])
    numeratori.push(numeratore2 - numeratore1);
    divisori.push(1);
  } else if (fmod(numeratore2,  1) != 0) { // If the second arg is a double
    numeratore1 = numeratore1 / divisore1;
    // Casts the first item to a double (no need to convert args_divisore[0])
    numeratori.push(numeratore2 - numeratore1);
    divisori.push(1);
  } else {
    int divisore = mcm(divisore1, divisore2);
    numeratori.push((numeratore2 * divisore / divisore2) - (numeratore1 * divisore / divisore1));
    divisori.push(divisore);
  }
}

void divide(double numeratore1, int divisore1, double numeratore2, int divisore2, StackList<double> &numeratori, StackList<int> &divisori) {
  if (numeratore1 == 0) {
    Serial.print("div0");
    return;
  }
  if (fmod(numeratore1, 1) != 0) { // If the first arg is a double
    numeratore2 = numeratore2 / divisore2;
              // Casts the second item to a double (no need to convert args_divisore[1])
    numeratori.push(numeratore2 / numeratore1);
    divisori.push(1);
  } else if (fmod(numeratore2,  1) != 0) { // If the second arg is a double
    numeratore1 = numeratore1 / divisore1;
    // Casts the first item to a double (no need to convert args_divisore[0])
    numeratori.push(numeratore2 / numeratore1);
    divisori.push(1);
  } else {
    // a/b / c/d = a/b * d/c = ad/bc
    int divisore = divisore1 * numeratore2;
    int numeratore = numeratore1 * divisore2;
    simplify(numeratore, divisore);
    numeratori.push(numeratore);
    divisori.push(divisore);
  }
}

long powint(int factor, unsigned int exponent)
{
    long product = 1;
    while (exponent--)
       product *= factor;
    return product;
}

// postfix expression evaluation algorithm.
bool
evaluate_postfix (String & postfix, double &numeratore, int &divisore) {
  StackList <double> numeratori;
  StackList <int> divisori; // gli operandi
  char c;                   // the character parsed.

  // check if the expression is empty.
  if (postfix.length () <= 0) {
    #if DEBUG
      Serial.println("err1");
      // POSTFIX-EVALUATION: the expression is empty.
    #endif
    return false;
  }

  // handle each character from the postfix expression.
  for (int i = 0; i < postfix.length (); i++) {
    // get the character.
    c = postfix.charAt (i);

    // if the character is not white space.
    if (c != ' ' && c != '\t') {
      // if the character is an identifier.
      if (is_identifier (c)) {
        // necessary for later reference.
        numeratori.push (0);

        // try to fetch / calculate a multi-digit integer number.
        for (; i < postfix.length () && is_identifier (c = postfix.charAt (i)); i++) {
          // calculate the number so far with its digits.
          numeratori.push(10.0 * numeratori.pop() + (c - '0'));
        }
        divisori.push(1); // An integer number has divisor 1

        // fix the index in order to 'ungetch' the non-identifier character.
        i--;
      }
      // if the character is an operator.
      else if (is_operator (c)) {
        // get the number of operator's arguments.
        int nargs = op_operands_count (c);

        // if there aren't enough arguments in the stack.
        if (nargs > numeratori.count ()) {
          #if DEBUG
            Serial.println("err2");
            // POSTFIX-EVALUATION: not sufficient operator arguments in the expression.
          #endif
          return false;
        }

        // allocate enough memory for the arguments of the operator.
        double *args_numeratore = (double *) malloc (sizeof (double) * nargs);
        double *args_divisore   = (double *) malloc (sizeof (double) * nargs);

        // if there was memory allocation error.
        if (args_numeratore == NULL) {
          Serial.println("err3");
          // POSTFIX-EVALUATION: insufficient memory for storing operator arguments.
        }

        // fetch all the arguments of the operator.
        for (int arg = 0; arg < nargs; arg++) {
          args_numeratore[arg] = numeratori.pop();
          args_divisore[arg] = divisori.pop();
        }

        // evaluate the operator with its operands.
        
        switch (c){
          case '+':
            subtract(-args_numeratore[0], args_divisore[0], args_numeratore[1], args_divisore[1], numeratori, divisori);
            break;

          case '-':
            subtract(args_numeratore[0], args_divisore[0], args_numeratore[1], args_divisore[1], numeratori, divisori);
            break;

          case '/':
            divide(args_numeratore[1], args_divisore[1], args_numeratore[0], args_divisore[0], numeratori, divisori);
            break;

          case '*':
            divide(args_numeratore[0], args_divisore[0], args_divisore[1], args_numeratore[1], numeratori, divisori);
            // Notare che divisore e numeratore del secondo numero sono scambiati: questo lo rende equivalente a una moltiplicazione.
            // a/b * c/d = a/b / d/c
            break;

/*        case '.':
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
              Serial.println("ln of a nonpositive number");
            }
            break;
          
          case 'L':
            if (vargs[0] > 0) {
              stack.push(log10(vargs[0]));
            } else {
              Serial.println("log of a nonpositive number");
            }
            break;*/
        }

        // deallocate memory for operands.
        free(args_numeratore);
        free(args_divisore);
      } else {
        // the character is unknown.
        #if DEBUG
          Serial.println("err4");
          // POSTFIX-EVALUATION: there is an unknown token in the expression.
        #endif
        return false;
      }
    }
  }

  // if there is only one element in the stack.
  if (numeratori.count() == 1) {
    // return the result of the expression.
    numeratore = numeratori.pop();
    divisore = divisori.pop();
  } else {
    #if DEBUG
      Serial.println("err5");
      // POSTFIX-EVALUATION: expression has too many values.
    #endif
    return false;
  }

  // postfix evaluation done.
  return true;
}
