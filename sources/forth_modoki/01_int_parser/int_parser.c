#include <stdio.h>
#include <assert.h>

static const char* const input = "123 456  1203";


int main() {
    int answer1 = 0;
    int answer2 = 0;
    int answer3 = 0;

    // write something here.
    int i = 0;
    int j = 0;
    int a[3] = {0};
    while (input[i]) {
       if (input[i] == ' ') {
	  if (a[j] != 0)
	     j++;
       }
       else {
	  a[j] *= 10;
	  a[j] += input[i] - '0';
       }
       i++;
    }

    answer1 = a[0];
    answer2 = a[1];
    answer3 = a[2];

    // verity result.
    assert(answer1 == 123);
    assert(answer2 == 456);
    assert(answer3 == 1203);

    return 1;
}
