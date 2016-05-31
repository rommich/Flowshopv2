#include "FlowShop.h"
int main(int argc, char* argv[]) {
	char* file = argv[1];
	FlowShop test = FlowShop();
	test.Run(argv);
	return 0;
}