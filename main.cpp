#include "fileBuffers.hpp"

int main()
{
	textFileBuffer text("main.cpp");
	if (text.data == nullptr) { puts("ERROR"); }
	else { puts(text.data); }
	printf("Size: %u bytes\n\n", text.size);

	binaryFileBuffer binary("main.cpp");
	binary.printBinary(8);
	printf("Size: %u bytes\n", binary.size);

	return 0;
}
