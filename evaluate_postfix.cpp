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
#include "numero.h"

#define SERIAL_DEBUG true

// include some calculator libraries' headers.
#include "evaluate_postfix.h"
#include "parse_tools.h"

unsigned int mcd(unsigned int a, unsigned int b) {
    for (;;)
    {
        if (a == 0) return b;
        b %= a;
        if (b == 0) return a;
        a %= b;
    }
}

int mcm(int a, int b) {
    unsigned int temp = mcd(abs(a), abs(b));
    return temp ? (a / temp * b) : 0;
}

void simplify(struct Numero &n) {
  if (n.isRational) {
    int fattore = mcd(n.numeratore, n.denominatore);
    n.numeratore = n.numeratore / fattore;
    n.denominatore = n.denominatore / fattore;
  }
}

unsigned int countDigits(int number) {
  unsigned int digitNumber = 0; do { number /= 10; digitNumber++; } while (number != 0); // http://stackoverflow.com/a/1489861/1541408 
  return digitNumber;
}

double powdouble(double factor, unsigned int exponent) {
    double product = 1;
    while (exponent--)
       product *= factor;
    return product;
}

long powint(int factor, unsigned int exponent) {
    long product = 1;
    while (exponent--)
       product *= factor;
    return product;
}

// postfix expression evaluation algorithm.
bool
evaluate_postfix (String & postfix, struct Numero & result) {
  StackList <Numero> stack; // the operands stack.
  char c;                   // the character parsed.

  // check if the expression is empty.
  if (postfix.length () <= 0) {
    #if SERIAL_DEBUG
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
        struct Numero prodotto;
        prodotto.numeratore = 0.0;
        prodotto.denominatore = 1;
        prodotto.isRational = true;
        stack.push(prodotto);

        // try to fetch / calculate a multi-digit integer number.
        for (; i < postfix.length () && is_identifier (c = postfix.charAt (i)); i++) {
          // calculate the number so far with its digits.
          stack.pop();
          prodotto.numeratore = prodotto.numeratore * 10.0 + (c - '0');
          stack.push(prodotto);
        }

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
        Numero *vargs = (Numero *) malloc (sizeof (Numero) * nargs);

        // if there was memory allocation error.
        if (vargs == NULL)
          Serial.println("err3");
          // POSTFIX-EVALUATION: insufficient memory for storing operator arguments.

        // fetch all the arguments of the operator.
        for (int arg = 0; arg < nargs; arg++)
          vargs[arg] = stack.pop ();

        // evaluate the operator with its operands.
        
        Serial.print(vargs[0].numeratore);
        Serial.print('/');
        Serial.print(vargs[0].denominatore);
        Serial.print(' ');
        Serial.print(c);
        Serial.print(' ');
        Serial.print(vargs[1].numeratore);
        Serial.print('/');
        Serial.println(vargs[1].denominatore);
        
        struct Numero risultato;
        
        switch (c){
          case '.': {
/*          unsigned int digitAmount;
            digitAmount = countDigits(args_numeratore[0]);
            long factor = pow(10, digitAmount */
            long factor = powint(10, countDigits(vargs[0].numeratore));
            risultato.numeratore = vargs[1].numeratore*factor + vargs[0].numeratore;
            risultato.denominatore = factor;
            risultato.isRational = true;
            break;
          }
          case '+':
            risultato.numeratore = vargs[1].numeratore*vargs[0].denominatore + vargs[0].numeratore*vargs[1].denominatore;
            goto sum;
          case '-':
            risultato.numeratore = vargs[1].numeratore*vargs[0].denominatore - vargs[0].numeratore*vargs[1].denominatore;
            goto sum;
          case '*':
            risultato.numeratore = vargs[1].numeratore * vargs[0].numeratore;
            risultato.denominatore = vargs[1].denominatore * vargs[0].denominatore;
            break;
          case '/':
            risultato.numeratore = vargs[1].numeratore * vargs[0].denominatore;
            risultato.denominatore = vargs[1].denominatore * vargs[0].numeratore;
            break;
          case '^': {
            if ((vargs[0].numeratore/vargs[0].denominatore)<0) {
              struct Numero temp;
              temp.denominatore = vargs[1].numeratore;
              temp.numeratore = vargs[1].denominatore;
              temp.isRational = vargs[1].isRational;
              vargs[1] = temp;
              vargs[0].numeratore = abs(vargs[0].numeratore);
              vargs[0].denominatore = abs(vargs[0].denominatore);
            }
            if (fmod(vargs[0].numeratore, 1.0) == 0.0 && vargs[0].denominatore == 1) {
              risultato.numeratore = powdouble(vargs[1].numeratore, vargs[0].numeratore);
              risultato.denominatore = powint(vargs[1].denominatore, vargs[0].numeratore);
              risultato.isRational = true;
            } else {
              risultato.numeratore = pow(vargs[1].numeratore, vargs[0].numeratore / vargs[0].denominatore) / pow(vargs[1].denominatore, vargs[0].numeratore / vargs[0].denominatore);
              goto irrational;
            }
            break;
          }
          case 's':
            risultato.numeratore = sin(vargs[0].numeratore / vargs[0].denominatore);
            goto irrational;
          case 'S':
            risultato.numeratore = asin(vargs[0].numeratore / vargs[0].denominatore);
            goto irrational;
          case 'c':
            risultato.numeratore = cos(vargs[0].numeratore / vargs[0].denominatore);
            goto irrational;
          case 'C':
            // Yay properties of trigonometrical functions! 22 bytes saved.
            risultato.numeratore = HALF_PI - asin(vargs[0].numeratore / vargs[0].denominatore);
            goto irrational;
          case 't':
            // Apparently, tan(x) occupies more space than sin(x)/cos(x). Wow.
            risultato.numeratore = sin(vargs[0].numeratore / vargs[0].denominatore)/cos(vargs[0].numeratore / vargs[0].denominatore);
            goto irrational;
          case 'T':
            risultato.numeratore = atan(vargs[0].numeratore / vargs[0].denominatore);
            goto irrational;
          case 'l':
            risultato.numeratore = log(vargs[0].numeratore / vargs[0].denominatore);
            goto irrational;
          case 'L':
            // Yay properties of logarithms! Enables us to save a few bytes of flash
            risultato.numeratore = log(vargs[0].numeratore / vargs[0].denominatore)/log(10);
            goto irrational;
        }
        goto rest;

irrational:
  risultato.denominatore = 1;
  risultato.isRational = false;
  goto rest;

sum:
            risultato.denominatore = vargs[1].denominatore * vargs[0].denominatore;
            risultato.isRational = vargs[0].isRational && vargs[1].isRational;
            goto rest;

rest:
        if (risultato.denominatore < 0) {
          risultato.denominatore = -risultato.denominatore;
          risultato.numeratore   = -risultato.numeratore;
        }
        simplify(risultato);
        stack.push(risultato);

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
  if (stack.count () == 1) {
    // return the result of the expression.
    result = stack.pop();
    return true;
  } else {
    #if SERIAL_DEBUG
      Serial.println("err5");
      // POSTFIX-EVALUATION: expression has too many values.
    #endif
    return false;
  }

  // postfix evaluation done.
  return true;
}
