

unsigned int randomNumber(unsigned int rangeFrom, unsigned int rangeTo)
{
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<unsigned int> distr(rangeFrom, rangeTo);
    return distr(generator);
}

std::string random_string( size_t length )
{
    auto randchar = []() -> char
    {
        const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    //str[length - 1] = '\n';
    return str;
}

std::wstring randomUnicode_string( size_t length)
{
    auto randchar = []() -> wchar_t
    {
        const wchar_t charset[] =
        L"0123456789"
        L"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        L"abcdefghijklmnopqrstuvwxyz"
        L"!@#$%^&*()_+=-./<>?"
        L"Σὲγνωρίζωἀπὸτὴν κόψη";
        const size_t max_index = (sizeof(charset)/sizeof (wchar_t) - 1);
        return charset[ rand() % max_index ];
    };

    std::wstring str(length,0);
    std::generate_n( str.begin(), length, randchar);

    return str;
}


bool genFileWithNumLines(string fileName, unsigned long long totallLines, unsigned int minSizeOfLine, unsigned int maxSizeOfLine)
{
    unsigned int sizeLine;
    string bufferStr;

    ofstream hFile;
    hFile.open(fileName, std::ios_base::out);

    for(unsigned long long i = 0; i < totallLines; i++)
    {
        sizeLine = randomNumber(minSizeOfLine, maxSizeOfLine);
        bufferStr = random_string(sizeLine);
         hFile<<bufferStr<<endl;
    }
    hFile.close();

    return true;
}

bool genFileWithUnicodeString(string fileName, unsigned long long totallLines, unsigned int minSizeOfLine, unsigned int maxSizeOfLine)
{
    size_t sizeLine;
    wstring bufferStr;

    wofstream hFile;
    hFile.open(fileName, std::ios_base::out);
    // Support UTF-8 encoded byte string and UTF-16 encoded character string
    hFile.imbue(g_loc);

    for(unsigned long long i = 0; i < totallLines; i++)
    {
        sizeLine = randomNumber(minSizeOfLine, maxSizeOfLine);
        bufferStr = randomUnicode_string(sizeLine);
         hFile<<bufferStr<<endl;
    }
    hFile.close();

    return true;
}

bool genFileWithNumBytesLimit(string fileName, unsigned long long numBytesLimit)
{
    unsigned int sizeLine = 20;
    unsigned int sizeLineRemain = 0;
    unsigned long long totallLines = numBytesLimit/sizeLine;
    sizeLineRemain = numBytesLimit%20;
    string bufferStr;

    ofstream hFile;
    hFile.open(fileName, std::ios_base::out);

    if(sizeLineRemain != 0)
    {
        bufferStr = random_string(sizeLineRemain);
        hFile<<bufferStr;
    }

    for(unsigned long long i = 0; i < totallLines; i++)
    {
        bufferStr = random_string(sizeLine);
         hFile<<bufferStr<<endl;
    }

    return true;
}
