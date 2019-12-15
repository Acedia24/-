#include "fileBuffers.hpp"

int main()
{
	textFileBuffer text("main.cpp");
	if (text.data == nullptr) { puts("ERROR"); }
	else { puts(text.data); }

	binaryFileBuffer binary("main.cpp");
	binary.printBinary(8);

	return 0;
}