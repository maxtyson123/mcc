int x = 4;
int y = 3;

int main() {
	int a = 2;
	int b = 4;

	// Without parentheses:     x + y  *  a + b   =  4 + (3 * 2) + 4    =    14    = 14
	//   With  parentheses:    (x + y) * (a + b)  =  (4 + 3) * (2 + 4)  =  7 * 6  =  42

	return (x + y) * (a + b);
}