/*
 * =====================================================================
 *
 *       Filename:  template.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/22/2012 01:47:57 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dengos (), dengos.w@gmail.com
 *        Company:  scut
 *
 * =====================================================================
 */


#include <vector>
#include <string>

using namespace std; 

vector <string> NonEmpty(vector <string> V) {
  // count the number of non-empty strings in V
  int num = 0;
  for (int k=0; k<V.size(); k++) {
    if (V[k] != "") num++;
  }
  
  // declare a vector of the right size and fill it in
  vector <string> newV(num);
  int index = 0;
  for (int k=0; k<V.size(); k++) {
    if (V[k] != "") {
      newV[index] = V[k];
      index++;
    }
  }
  
  // return result
  return newV;
}


