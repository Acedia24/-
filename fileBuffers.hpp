#ifdef _WIN64
#define FSEEK _fseeki64
#define FTELL _ftelli64
#else
#define FSEEK fseek
#define FTELL ftell
#endif

#ifndef MELS_FILE_SYSTEM_HEADER
#define MELS_FILE_SYSTEM_HEADER

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>

// ����� ��� ������ � ���������� �������.
class textFileBuffer
{
public:
	char* data;				/* ����� ����� ������. */
	size_t size;			/* ������ ������ � ������. */

	textFileBuffer() : data(nullptr), size(0) { }

	textFileBuffer(const char* filePath) : textFileBuffer()
	{
		readFileToStr(filePath);
	}

	~textFileBuffer()
	{
		free(this->data);
	}

	// ���������� ��������� ����.
	bool readFileToStr(const char* filePath)
	{
		// ���� ������ ��� ����������?
		if (this->data != nullptr)
		{
			free(this->data);
			this->size = 0;
		}

		// ��������� ����.
		FILE* file = nullptr;
		errno_t error = fopen_s(&file, filePath, "rt");
		if (error != 0 || file == nullptr)
		{
			printf("readFileToStr(): can't open file %s.\n", filePath);
			return false;
		}

		// ��������� ����� �����.
		long int savePos = FTELL(file);
		FSEEK(file, 0L, SEEK_END);
		size_t bufferLength = FTELL(file);
		FSEEK(file, savePos, SEEK_SET);

		// ������� �������� ���� ������.
		char* fBuffer = (char*)malloc(sizeof(char) * bufferLength);
		if (fBuffer == nullptr)
		{
			puts("readFileToStr(): can't allocate memory.");
			fclose(file);
			return false;
		}

		// ���������� ����.
		size_t rCount = fread(fBuffer, sizeof(char), bufferLength, file) + 1;

		// ���������������� ������, ���� �����.
		if (rCount != bufferLength)
		{
			char* tempMemory = (char*)realloc(fBuffer, sizeof(char) * rCount);
			if (tempMemory != nullptr) { fBuffer = tempMemory; }
		}

		// ���������� ������ ����� ������.
		fBuffer[rCount - 1] = '\0';

		// ��������� ����.
		fclose(file);

		// ���������� ����� � ������ ������.
		this->data = fBuffer;
		this->size = rCount;

		return true;
	}

	// ���������� ���������� ������ � ��������� ����.
	void writeStrToFile(const char* filePath)
	{
		// ��������� ��� ������ ����.
		FILE* file = nullptr;
		errno_t error = fopen_s(&file, filePath, "wt");
		if (error != 0 || file == nullptr)
		{
			printf("writeStrToFile(): can't open file %s.", filePath);
			return;
		}

		// ���������� ������ � ����.
		size_t wCount = fwrite(this->data, sizeof(char), this->size - 1, file);
		if (wCount == 0)
		{
			puts("writeStrToFile(): can't write string in file.");
		}

		// ��������� ����.
		fclose(file);
	}

	// ���������� ��������� �� ���� ������ ������.
	char* getData() { return this->data; }

	// ���������� ������ ����� ������ � ������.
	size_t getSize() { return this->size; }

	// �������� ���������� ������ ��� �����.
	void printText() { if (this->data != nullptr) { puts(this->data); } }
};

// ����� ��� ������ � ��������� �������.
class binaryFileBuffer
{
public:
	uint8_t* data;			/* ����� ����� ������. */
	size_t size;			/* ������ ������ � ������. */

	binaryFileBuffer() : data(nullptr), size(0) { }

	binaryFileBuffer(const char* filePath) : binaryFileBuffer()
	{
		readBinaryFileToArray(filePath);
	}

	~binaryFileBuffer()
	{
		free(this->data);
	}

	// ���������� �������� ����.
	bool readBinaryFileToArray(const char* filePath)
	{
		// ���� ������ ��� ����������?
		if (this->data != nullptr)
		{
			free(this->data);
			this->size = 0;
		}

		// ��������� ����.
		FILE* file = nullptr;
		errno_t error = fopen_s(&file, filePath, "rb");
		if (error != 0 || file == nullptr)
		{
			printf("readBinaryFileToArray(): can't open file %s.", filePath);
			return false;
		}

		// ��������� ����� �����.
		long int savePos = FTELL(file);
		FSEEK(file, 0L, SEEK_END);
		size_t bufferLength = FTELL(file);
		FSEEK(file, savePos, SEEK_SET);

		// ������� �������� ���� ������.
		uint8_t* fBuffer = (uint8_t*)malloc(sizeof(uint8_t) * bufferLength);
		if (fBuffer == nullptr)
		{
			puts("readBinaryFileToArray(): can't allocate memory.");
			fclose(file);
			return false;
		}

		// ���������� ����.
		size_t rCount = fread(fBuffer, sizeof(uint8_t), bufferLength, file);

		// ��������� ����.
		fclose(file);

		// ���������� ����� � ������ ������.
		this->data = fBuffer;
		this->size = rCount;

		return true;
	}

	// ���������� ������ ���� � �������� ����.
	void writeBytesInBinaryFile(const char* filePath)
	{
		// ��������� ��� ������ ����.
		FILE* file = nullptr;
		errno_t error = fopen_s(&file, filePath, "wb");
		if (error != 0 || file == nullptr)
		{
			printf("writeBytesInBinaryFile(): can't open file %s.", filePath);
			return;
		}

		// ���������� ������ ���� � ����.
		size_t wCount = fwrite(this->data, sizeof(uint8_t), this->size, file);
		if (wCount == 0)
		{
			puts("writeBytesInBinaryFile(): can't write array in file.");
		}

		// ��������� ����.
		fclose(file);
	}

	// ���������� ��������� �� ���� ������ ������.
	unsigned char* getData() { return this->data; }

	// ���������� ������ ����� ������ � ������.
	size_t getSize() { return this->size; }

	// �������� ���������� ������ ��� ������ ����.
	void printBinary()
	{
		// ����� ������ �� ����������?
		if (this->data == nullptr) { return; }

		// �������� �������� ���� � �������.
		uint8_t* it = this->data;
		size_t i = this->size;
		while (i > 0)
		{
			printf("%02x ", *it);

			--i; ++it;
		}
		puts("");
	}

	// �������� ���������� ������ ��� ������ ���� �� N � ������.
	void printBinary(size_t N)
	{
		// ����� ������ �� ����������?
		if (this->data == nullptr) { return; }

		// �������� �������� ���� � �������.
		uint8_t* it = this->data;
		size_t i = 0;
		while (i < this->size)
		{
			if ((i + 1) % N == 0) { printf("%02x\n", *it); }
			else { printf("%02x ", *it); }

			++i; ++it;
		}
		if (this->size % N) { puts(""); }
	}
};

#endif