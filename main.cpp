//#include "fileBuffers.hpp"
#include <MELS/fileSystem.hpp>
using MELS::textFileBuffer;
using MELS::binaryFileBuffer;

int main()
{
	textFileBuffer text("main.cpp");
	if (text.data == nullptr) { puts("ERROR"); }
	else { puts(text.data); }
	printf("Size: %zu bytes\n\n", text.size);

	binaryFileBuffer binary("main.cpp");
	binary.printBinary(8);
	printf("Size: %zu bytes\n", binary.size);

	return 0;
}