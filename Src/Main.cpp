// =============================================================================
// @FILE
// @BRIEF This program is used to convert files into C++ headers so they can be
//        embedded in a binary.
// @AUTHOR Vik Pandher
// @DATE 2023-12-20

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

// #define SANITY_CHECK_OUTPUT_FILE



int CompareStrings(const char* string1, const char* string2);
int ProcessString(int argc, char** argv);
int ProcessHex(int argc, char** argv);
bool ValidateArrayName(const char* arrayName);

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "ERROR: Not enough arguments" << std::endl;
        std::cerr << "    Try /? or /help" << std::endl;

        return 1;
    }

    if (CompareStrings(argv[1], "/?") == 0 ||
        CompareStrings(argv[1], "/help") == 0)
    {
        if (argc > 2)
        {
            std::cerr << "ERROR: Too many arguments" << std::endl;
            std::cerr << "    Try /? or /help" << std::endl;

            return 1;
        }

        std::cout << "Usage:" << std::endl;
        std::cout << "    ./" << PROJECT_NAME << " [option] <input_file> <output_file> <array_name>" << std::endl;
        std::cout << std::endl;
        std::cout << "Description:" << std::endl;
        std::cout << "    This program copies data from the input file into the output file." << std::endl;
        std::cout << "    If the output file does not exist, it is created; otherwise it is overwritten." << std::endl;
        std::cout << "    The output file is formatted as a C++ header file." << std::endl;
        std::cout << "    By default the data is stored into a static const char array as hex values." << std::endl;
        std::cout << std::endl;
        std::cout << "Options:" << std::endl;
        std::cout << "    /s or /string           Store the data as a static const char* string instead." << std::endl;
        std::cout << "    /? or /help             Displays this help message." << std::endl;

        return 0;
    }

    if (CompareStrings(argv[1], "/s") == 0 ||
        CompareStrings(argv[1], "/string") == 0)
    {
        if (argc < 5)
        {
            std::cerr << "ERROR: Not enough arguments" << std::endl;
            std::cerr << "    Try /? or /help" << std::endl;

            return 1;
        }

        if (argc > 5)
        {
            std::cerr << "ERROR: Too many arguments" << std::endl;
            std::cerr << "    Try /? or /help" << std::endl;

            return 1;
        }

        return ProcessString(argc, argv);
    }

    if (argv[1][0] == '/')
    {
        std::cerr << "ERROR: Unrecognized option" << std::endl;
        std::cerr << "    Try /? or /help" << std::endl;

        return 1;
    }

    if (argc < 4)
    {
        std::cerr << "ERROR: Not enough arguments" << std::endl;
        std::cerr << "    Try /? or /help" << std::endl;

        return 1;
    }

    if (argc > 4)
    {
        std::cerr << "ERROR: Too many arguments" << std::endl;
        std::cerr << "    Try /? or /help" << std::endl;

        return 1;
    }

    return ProcessHex(argc, argv);
}

int CompareStrings(const char* string1, const char* string2)
{
    while (*string1 != '\0' && *string2 != '\0' && *string1 == *string2)
    {
        string1++;
        string2++;
    }

    if (*string1 == '\0' && *string2 == '\0') {
        return 0; // Strings are equal
    }
    else {
        return *string1 - *string2; // Return the ASCII difference
    }
}

int ProcessString(int argc, char** argv)
{
    const char* inputFileName = argv[2];
    const char* outputFileName = argv[3];
    const char* arrayName = argv[4];

    if (CompareStrings(inputFileName, outputFileName) == 0)
    {
        std::cerr << "Error: Input file and output file can't have the same name" << std::endl;
        return 1;
    }

    std::ifstream inputFileStream(inputFileName, std::ios::binary);
    if (!inputFileStream.is_open())
    {
        std::cerr << "Error: Couldn't open input file" << std::endl;
        return 1;
    }

    std::ofstream outputFileStream(outputFileName);
    if (!outputFileStream.is_open())
    {
        std::cerr << "Error: Couldn't open output file" << std::endl;
        return 1;
    }

    if (!ValidateArrayName(arrayName))
    {
        return 1;
    }

    outputFileStream << "#pragma once" << std::endl << std::endl;
    outputFileStream << "static const char " << arrayName << "[] = R\"(";

    std::string line;
    while (std::getline(inputFileStream, line))
    {
        outputFileStream << line;
    }

    outputFileStream << ")\";";

    inputFileStream.close();
    outputFileStream.close();

    return 0;
}

int ProcessHex(int argc, char** argv)
{
    const char* inputFileName = argv[1];
    const char* outputFileName = argv[2];
    const char* arrayName = argv[3];

    if (CompareStrings(inputFileName, outputFileName) == 0)
    {
        std::cerr << "Error: Input file and output file can't have the same name" << std::endl;
        return 1;
    }

    std::ifstream inputFileStream(inputFileName, std::ios::binary);
    if (!inputFileStream.is_open())
    {
        std::cerr << "Error: Couldn't open input file" << std::endl;
        return 1;
    }

    std::ofstream outputFileStream(outputFileName);
    if (!outputFileStream.is_open())
    {
        std::cerr << "Error: Couldn't open output file" << std::endl;
        return 1;
    }

#ifdef SANITY_CHECK_OUTPUT_FILE
    std::ofstream outputFileStream2(std::string(outputFileName)+ "_sanity", std::ios::binary);
    if (!outputFileStream2.is_open())
    {
        std::cerr << "Error: Couldn't open output file" << std::endl;
        return 1;
    }
#endif

    if (!ValidateArrayName(arrayName))
    {
        return 1;
    }

    outputFileStream << "#pragma once" << std::endl << std::endl;
    outputFileStream << "static const unsigned char " << arrayName << "[] = {";

    unsigned char byte;
    int count = 0;
    const int countPerLine = 8;
    bool isFirstElement = true;
    while (inputFileStream >> std::noskipws >> byte)
    {
        if (!isFirstElement)
        {
            outputFileStream << ",";
        }
        else
        {
            isFirstElement = false;
        }

        if (count % countPerLine == 0)
        {
            outputFileStream << std::endl << "   ";
        }

        outputFileStream << " 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(byte);

#ifdef SANITY_CHECK_OUTPUT_FILE
        outputFileStream2 << std::noskipws << byte;
#endif

        count++;
    }

    outputFileStream << std::endl << "};";

    inputFileStream.close();
    outputFileStream.close();

    return 0;
}

bool ValidateArrayName(const char* arrayName)
{
    bool validName = true;

    if (!(arrayName[0] == '_' ||
        (arrayName[0] >= 'A' && arrayName[0] <= 'Z') ||
        (arrayName[0] >= 'a' && arrayName[0] <= 'z')))
    {
        validName = false;
    }

    for (const char* characterPtr = arrayName; *characterPtr != '\0'; ++characterPtr)
    {
        const char& character = *characterPtr;
        if (!(character == '_' ||
            (character >= '0' && character <= '9') ||
            (character >= 'A' && character <= 'Z') ||
            (character >= 'a' && character <= 'z')))
        {
            validName = false;
            break;
        }
    }

    if (!validName)
    {
        std::cerr << "Error: Invalid array name" << std::endl;
        std::cerr << "    Only the following ASCII characters are allowed:" << std::endl;
        std::cerr << "        '_'" << std::endl;
        std::cerr << "        '0' to '9'" << std::endl;
        std::cerr << "        'A' to 'Z'" << std::endl;
        std::cerr << "        'a' to 'z'" << std::endl;
        std::cerr << "    The first character can't be '0' to '9'." << std::endl;

        return false;
    }

    return true;
}