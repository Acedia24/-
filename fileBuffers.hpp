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

// Класс для работы с текстовыми файлами.
class textFileBuffer
{
public:
	char* data;				/* Адрес блока данных. */
	size_t size;			/* Размер данных в байтах. */

	textFileBuffer() : data(nullptr), size(0) { }

	textFileBuffer(const char* filePath) : textFileBuffer()
	{
		readFileToStr(filePath);
	}

	~textFileBuffer()
	{
		free(this->data);
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
			printf("readFileToStr(): can't open file %s.\n", filePath);
			return false;
		}

		// Отключаем буферизацию данных потоком связанным с file.
		error = setvbuf(file, NULL, _IONBF, NULL);
		if (error != 0)
		{
			puts("readFileToStr(): can't turn off buffering");
			fclose(file);
			return false;
		}
		
		// Вычисляем длину файла.
		FSEEK(file, 0L, SEEK_END);
		size_t bufferLength = FTELL(file);
		FSEEK(file, 0L, SEEK_SET);

		// Пробуем выделить блок памяти.
		char* fBuffer = (char*)malloc(sizeof(char) * bufferLength);
		if (fBuffer == nullptr)
		{
			puts("readFileToStr(): can't allocate memory.");
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
		FILE* file = nullptr;
		errno_t error = fopen_s(&file, filePath, "wt");
		if (error != 0 || file == nullptr)
		{
			printf("writeStrToFile(): can't open file %s.", filePath);
			return;
		}

		// Записываем строку в файл.
		size_t wCount = fwrite(this->data, sizeof(char), this->size - 1, file);
		if (wCount == 0)
		{
			puts("writeStrToFile(): can't write string in file.");
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
	uint8_t* data;			/* Адрес блока данных. */
	size_t size;			/* Размер данных в байтах. */

	binaryFileBuffer() : data(nullptr), size(0) { }

	binaryFileBuffer(const char* filePath) : binaryFileBuffer()
	{
		readBinaryFileToArray(filePath);
	}

	~binaryFileBuffer()
	{
		free(this->data);
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
			printf("readBinaryFileToArray(): can't open file %s.", filePath);
			return false;
		}

		
		// Отключаем буферизацию данных потоком связанным с file.
		error = setvbuf(file, NULL, _IONBF, NULL);
		if (error != 0)
		{
			puts("readBinaryFileToArray(): can't turn off buffering");
			fclose(file);
			return false;
		}
		
		// Вычисляем длину файла.
		FSEEK(file, 0L, SEEK_END);
		size_t bufferLength = FTELL(file);
		FSEEK(file, 0L, SEEK_SET);

		// Пробуем выделить блок памяти.
		uint8_t* fBuffer = (uint8_t*)malloc(sizeof(uint8_t) * bufferLength);
		if (fBuffer == nullptr)
		{
			puts("readBinaryFileToArray(): can't allocate memory.");
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
		FILE* file = nullptr;
		errno_t error = fopen_s(&file, filePath, "wb");
		if (error != 0 || file == nullptr)
		{
			printf("writeBytesInBinaryFile(): can't open file %s.", filePath);
			return;
		}

		// Записываем массив байт в файл.
		size_t wCount = fwrite(this->data, sizeof(uint8_t), this->size, file);
		if (wCount == 0)
		{
			puts("writeBytesInBinaryFile(): can't write array in file.");
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
	void printBinary(size_t N)
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
