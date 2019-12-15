#ifndef FILE_BUFFER_HEADER
#define FILE_BUFFER_HEADER

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>

// Класс для работы с текстовыми файла.
class textFileBuffer
{
public:
	char* data;
	size_t size;

	textFileBuffer() : data(nullptr), size(0) { }

	textFileBuffer(const char* filePath) : textFileBuffer()
	{
		readFileToStr(filePath);
	}

	~textFileBuffer()
	{
		if (this->data != nullptr) { free(this->data); }
	}

	// Буферизует текстовый файл.
	bool readFileToStr(const char* filePath)
	{
		// Блок памяти уже существует?
		if (this->data != nullptr)
		{
			free(this->data);
			this->size = 0;
		}

		// Открываем файл.
		FILE* file = nullptr;
		errno_t error = fopen_s(&file, filePath, "rt");
		if (error != 0 || file == nullptr)
		{
			printf("readFileToStr(): не удалось открыть файл %s.\n", filePath);
			return false;
		}

		// Вычисляем длину файла.
		long int savePos = ftell(file);
		fseek(file, 0L, SEEK_END);
		size_t bufferLength = ftell(file);
		fseek(file, savePos, SEEK_SET);

		// Пробуем выделить блок памяти.
		char* fBuffer = (char*)malloc(sizeof(char) * bufferLength);
		if (fBuffer == nullptr)
		{
			puts("readFileToStr(): не удалось выделить память.");
			fclose(file);
			return false;
		}

		// Буферизуем файл.
		size_t rCount = fread(fBuffer, sizeof(char), bufferLength, file) + 1;

		// Перераспределяем память, если нужно.
		if (rCount != bufferLength)
		{
			char* tempMemory = (char*)realloc(fBuffer, sizeof(char) * rCount);
			if (tempMemory != nullptr) { fBuffer = tempMemory; }
		}

		// Дописываем символ конца строки.
		fBuffer[rCount - 1] = '\0';

		// Закрываем файл.
		fclose(file);

		// Запоминаем адрес и размер буфера.
		this->data = fBuffer;
		this->size = rCount;

		return true;
	}

	// Записывает содержимое буфера в текстовый файл.
	void writeStrToFile(const char* filePath)
	{
		// Открываем или создаём файл.
		FILE* file = NULL;
		errno_t error = fopen_s(&file, filePath, "wt");
		if (error != 0 || file == NULL)
		{
			printf("writeStrToFile(): не удалось открыть файл %s.", filePath);
			return;
		}

		// Записываем строку в файл.
		size_t wCount = fwrite(this->data, sizeof(char), this->size - 1, file);
		if (wCount == 0)
		{
			puts("writeStrToFile(): не удалось записать строку в файл.");
		}

		// Закрываем файл.
		fclose(file);
	}

	// Возвращает указатель на блок памяти данных.
	char* getData() { return this->data; }

	// Возвращает размер блока данных в байтах.
	size_t getSize() { return this->size; }

	// Печатает содержимое буфера как текст.
	void printText() { if (this->data != nullptr) { puts(this->data); } }
};

// Класс для работы с бинарными файлами.
class binaryFileBuffer
{
public:
	uint8_t* data;
	size_t size;

	binaryFileBuffer() : data(nullptr), size(0) { }

	binaryFileBuffer(const char* filePath) : binaryFileBuffer()
	{
		readBinaryFileToArray(filePath);
	}

	~binaryFileBuffer()
	{
		if (this->data != nullptr) { free(this->data); }
	}

	// Буферизует бинарный файл.
	bool readBinaryFileToArray(const char* filePath)
	{
		// Блок памяти уже существует?
		if (this->data != nullptr)
		{
			free(this->data);
			this->size = 0;
		}

		// Открываем файл.
		FILE* file = nullptr;
		errno_t error = fopen_s(&file, filePath, "rb");
		if (error != 0 || file == nullptr)
		{
			printf("readBinaryFileToArray(): не удалось открыть файл %s.", filePath);
			return false;
		}

		// Вычисляем длину файла.
		long int savePos = ftell(file);
		fseek(file, 0L, SEEK_END);
		size_t bufferLength = ftell(file);
		fseek(file, savePos, SEEK_SET);

		// Пробуем выделить блок памяти.
		uint8_t* fBuffer = (uint8_t*)malloc(sizeof(uint8_t) * bufferLength);
		if (fBuffer == nullptr)
		{
			puts("readBinaryFileToArray(): не удалось выделить память.");
			fclose(file);
			return false;
		}

		// Буферизуем файл.
		size_t rCount = fread(fBuffer, sizeof(uint8_t), bufferLength, file);

		// Закрываем файл.
		fclose(file);

		// Запоминаем адрес и размер буфера.
		this->data = fBuffer;
		this->size = rCount;

		return true;
	}

	// Записывает массив байт в бинарный файл.
	void writeBytesInBinaryFile(const char* filePath)
	{
		// Открываем или создаём файл.
		FILE* file = NULL;
		errno_t error = fopen_s(&file, filePath, "wb");
		if (error != 0 || file == NULL)
		{
			printf("writeBytesInBinaryFile(): не удалось открыть файл %s.", filePath);
			return;
		}

		// Записываем массив байт в файл.
		size_t wCount = fwrite(this->data, sizeof(uint8_t), this->size, file);
		if (wCount == 0)
		{
			puts("writeBytesInBinaryFile(): не удалось записать массив в файл.");
		}

		// Закрываем файл.
		fclose(file);
	}

	// Возвращает указатель на блок памяти данных.
	unsigned char* getData() { return this->data; }

	// Возвращает размер блока данных в байтах.
	size_t getSize() { return this->size; }

	// Печатает содержимое буфера как массив байт.
	void printBinary()
	{
		// Блока данных не существует?
		if (this->data == nullptr) { return; }

		// Печатаем значения байт в консоль.
		uint8_t* it = this->data;
		size_t i = this->size;
		while (i > 0)
		{
			printf("%02x ", *it);

			--i; ++it;
		}
		puts("");
	}

	// Печатает содержимое буфера как массив байт по N в строке.
	void printBinary(unsigned int N)
	{
		// Блока данных не существует?
		if (this->data == nullptr) { return; }

		// Печатаем значения байт в консоль.
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