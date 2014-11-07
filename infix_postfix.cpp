/*
 *  This file implements the shunting-yard algorithm for infix->postfix.
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
#include "infix_postfix.h"
#include "parse_tools.h"

// infix expression conversion to postfix (shunting-yard algorithm).
bool
infix_postfix (String & infix, String & postfix) {
  StackList <char> stack; // the operators stack.
  char c, sc;             // the characters parsed.

  // check if the expression is empty.
  if (infix.length () <= 0) {
    #if DEBUG
      Serial.println("err6");
      // POSTFIX-EVALUATION: expression has too many values.
    #endif
    return false;
  }

  bool isDot = false;
  // handle each character from the infix expression.
  for (int i = 0; i < infix.length (); i++) {
    // get the character.
    c = infix.charAt (i);

    // if the character is not white space.
    if (c != ' ' && c != '\t') {
      // if the character is an identifier.
      if (is_identifier (c)) {
        if(isDot){
//          postfix += '1';
          isDot = false;
        }
        // add the character to postfix.
        postfix += c;

        // try to fetch more digits for multi-digit integer number and add them to postfix.
        for (++i; i < infix.length () && is_identifier (c = infix.charAt (i)); postfix += c, i++)
          continue;

        // fix the index in order to 'ungetch' the non-identifier character.
        i--;

        // add a space separator.
        postfix += ' ';
      }
      // if the character is an operator.
      else if (is_operator (c)) {
        isDot = c == '.';
        // while the stack is not empty.
        while (!stack.isEmpty ()) {
          // get the top of the stack.
          sc = stack.peek ();

          // if the top of the stack is an operator, check the
          // precedence and associativity of the two operators.
          if (is_operator (sc) &&
              (op_precedence (c) <= op_precedence (sc))) {
            // add the appropriate operator to postfix.
            postfix += sc;

            // add a space separator.
            postfix += ' ';

            // pop the operator from the stack.
            stack.pop ();
          }
          else
            break; // non-operator found.
        }

        // push the operator to the stack.
        stack.push (c);
      }
      // if the character is a left parenthesis.
      else if (c == '(') {
        // push it to the stack.
        stack.push (c);
      }
      // if the character is a right parenthesis.
      else if (c == ')') {
        bool pe = false;

        // until the token at the top of the stack
        // is left parenthesis, pop operators off
        // the stack onto the postfix expression.
        while (!stack.isEmpty ()) {
          sc = stack.peek ();

          if (sc == '(') {
            pe = true;
            break;
          }
          else {
            // add the character to postfix.
            postfix += sc;

            // add a space separator.
            postfix += ' ';

            // pop the character from the stack.
            stack.pop ();
          }
        }

        // if the stack runs out without finding a left
        // parenthesis there are mismatched parentheses.
        if (!pe) {
          #if DEBUG
            Serial.println("err7");
            // SHUNTING-YARD: parentheses mismatched in the expression.
          #endif
          return false;
        }

        // pop the left parenthesis from the stack.
        stack.pop ();
      }
      else {
        #if DEBUG
          Serial.println("err8");
          // SHUNTING-YARD: there is an unknown token in the expression.
        #endif
        return false;
      }
    }
  }

  // when there are no more tokens to read: while
  // there are still operator tokens in the stack.
  while (!stack.isEmpty ()) {
    sc = stack.peek ();

    if (sc == '(' || sc == ')') {
      #if DEBUG
        Serial.println("err9");
        // SHUNTING-YARD: parentheses mismatched in the expression.
      #endif
      return false;
    }

    // add the character to postfix.
    postfix += sc;

    // add a space separator.
    postfix += ' ';

    // pop the character from the stack.
    stack.pop ();
  }

  // infix->postfix parsing done.
  return true;
}
