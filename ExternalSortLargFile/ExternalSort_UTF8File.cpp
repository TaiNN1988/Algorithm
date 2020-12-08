#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <random>
#include <filesystem>
#include <chrono>
#include <locale>
#include <codecvt>

//Define solution optimize
#define OPTIMIZE_EXTERNAL_SORT_USING_MIN_HEAP 1

#define ONE_4MB 4194304 //4*1024*1024
#define ONE_GB 1073741824 //1024*1024*1024

unsigned int g_OutFileCount = 0;
const char* dicTemp = "FolderTemp/";

//Support UTF-8 encoded byte string
std::locale g_loc(std::locale::classic(), new std::codecvt_utf8<wchar_t>);

using namespace std;

typedef struct FileInfo
{
    wstring currentLine;
    std::shared_ptr<wifstream> hFile;
    bool isEndFile = false;

    FileInfo()
    {
        currentLine = L"";
         isEndFile = false;
    }

    FileInfo(string fileName)
    {
        try
        {
            hFile = shared_ptr<wifstream>(new wifstream(fileName, std::ios_base::in),std::default_delete<wifstream>());

            // Support UTF-8 encoded byte string and UTF-16 encoded character string
            hFile->imbue(g_loc);

            if(!hFile->is_open())
            {
               cout<<"Error! Can not open new file:"<<fileName<<endl;
               isEndFile = true;
               hFile->clear();
               return;
            }

            if(!getline(*hFile, this->currentLine))
            {
                isEndFile = true;
                hFile->close();
            }
        }
        catch (const std::runtime_error& re) {
            cout<<"Runtime error: " <<re.what()<<endl;
        }
        catch(const std::exception& ex)
        {
            cout<<"Error: " <<ex.what()<<endl;
        }
        catch(...)
        {
             cout<<"Error:Unknown failure occurred "<<endl;
        }
    }

    FileInfo(std::shared_ptr<wifstream> &hFilevalue)
    {
        this->hFile = hFilevalue;
        if(!getline(*hFile, this->currentLine))
        {
            isEndFile = true;
            hFile->close();
        }

    }

}FileInfo;

bool externalSortLargFile(string inFileName, string outFileName, unsigned long long numBytesLimit, bool optimizeMemoryLimit);
void mergeKfilesByUsingMinHeap(string outFileName, unsigned long long totallFile);
void mergeKfilesByUsingRecursion(string outFileName, unsigned long long leftFileIdx, unsigned long long rightFileIdx);
void sliptAndSortFile(string inFileName, unsigned long long maxBytesOfSlipFile, unsigned int& countFile);

void mergeTwoSmallFile(string inFile1, string inFile2, string outFile);
void directSortSmallFileOnMemory(string inFileName, string outFileName);

//Helper function
bool genFileWithNumLines(string fileName, unsigned long long totallLines, unsigned int minSizeOfLine, unsigned int maxSizeOfLine);
bool genFileWithNumBytesLimit(string fileName, unsigned long long numBytesLimit);
std::string random_string( size_t length );
void writeToFile(string fileName, vector<wstring> listLine);
unsigned int randomNumber(unsigned int rangeFrom, unsigned int rangeTo);
void writeToFile(string infileName, string outfileName);
bool checkExitFileAndReadable(string fileName);
bool genFileWithUnicodeString(string fileName, unsigned long long totallLines, unsigned int minSizeOfLine, unsigned int maxSizeOfLine);

int main(int argc, char* argv[])
{

    // =============Gen file for data test===================
//    unsigned long long totallLines = 70;
//    unsigned int minSizeOfLine = 20;
//    unsigned int maxSizeOfLine = 50;
//    genFileWithUnicodeString("TestUnicode_70", totallLines, minSizeOfLine, maxSizeOfLine);


    int bFunc = EXIT_SUCCESS;

    string inFileName = "";
    string outFilename="";

    unsigned long long numBytesLimit = 0;
    bool isOptimizeMemoryLimit = false;

    if(argc == 5)
    {
        // get paramater from command line
        inFileName = argv[1];
        outFilename = argv[2];
        numBytesLimit = std::stoull(argv[3]);

        isOptimizeMemoryLimit = std::stoull(argv[4]) > 0 ? true: false;

        if(inFileName == "" || outFilename == "" || numBytesLimit == 0)
        {
            cout<<"Error! Invalid paramater from command line.\n";
            return EXIT_FAILURE;
        }
    }
    else
    {
        cout<<"Error! Invalid paramater from command line."<<endl;
        cout<<"Format commandline should be: [inFile outFile memoryLimit optimize(0 or 1)]."<<endl;
        cout<<"Example: [UTF-8-demo OuputData 4000 1]."<<endl;
        return EXIT_FAILURE;
    }

    // Show config
    cout<<"Input file: "<<inFileName<<"  Output file: "<<outFilename<<endl;
    string strTemp = isOptimizeMemoryLimit != true ? "FALSE" : "TRUE";
    cout<<"Memory limit: "<<numBytesLimit<<"  Optimize memory need to slip file: "<<strTemp<<endl;

    // set start time
    auto start = std::chrono::high_resolution_clock::now();

    bFunc = externalSortLargFile(inFileName, outFilename, numBytesLimit, isOptimizeMemoryLimit);

    // Get execution time
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(end - start);
    cout<<"Execution time on externalSortLargFile function:"<<duration.count()<<"(s)"<<endl;

    return bFunc;
}

bool externalSortLargFile(string inFileName, string outFileName, unsigned long long numBytesLimit, bool optimizeMemoryLimit)
{
    if(!checkExitFileAndReadable(inFileName))
    {
        cout<<"Error Open file! Please check ["<<inFileName<<"]file is exit or not."<<endl;
        return EXIT_FAILURE;
    }

    std::uintmax_t fileSize =  std::filesystem::file_size(inFileName);
    double sumGigagbyte = double(fileSize)/1000000000;
    cout<<"Total size of file: "<< fileSize<<"(Byte)"<<"    about: "<<sumGigagbyte<<"(GB)"<<endl;
    unsigned long long maxBytesOfSlipFile = 0;

    // Check case file empty
    if(fileSize == 0)
    {
        writeToFile(inFileName, outFileName);
        return EXIT_SUCCESS;
    }

    //=========Caculate max size of slipt file====================//
    //Default it will use numBytesLimit for each file
    // one char uf8 is 4 byte
    if(optimizeMemoryLimit)
    {
        unsigned long long sz10_4MB = (unsigned long long)ONE_4MB*10;
        unsigned long long sz100_4MB = (unsigned long long)ONE_4MB*100;
        unsigned long long sz200_4MB = (unsigned long long)ONE_4MB*200;
        unsigned long long sz1000_4MB = (unsigned long long)ONE_4MB*1000;

        unsigned long long sz10GB = (unsigned long long)ONE_GB*10;
        unsigned long long sz100GB = (unsigned long long)ONE_GB*100;
        unsigned long long sz200GB = (unsigned long long)ONE_GB*200;

        if(fileSize < ONE_GB && numBytesLimit > ONE_4MB)
        {
            // Case small file
            if(fileSize < ONE_4MB)
            {
                directSortSmallFileOnMemory(inFileName, outFileName);
                return EXIT_SUCCESS;
            }

            maxBytesOfSlipFile = ONE_4MB;

        }
        else if(fileSize >= ONE_GB && fileSize < sz10GB && numBytesLimit > sz10_4MB)
        {
            maxBytesOfSlipFile = sz10_4MB;
        }
        else if (fileSize >= sz10GB &&  fileSize < sz100GB && numBytesLimit > sz100_4MB)
        {
            maxBytesOfSlipFile = sz100_4MB;
        }
        else if (fileSize >= sz100GB &&  fileSize < sz200GB && numBytesLimit > sz200_4MB)
        {
             maxBytesOfSlipFile = sz200_4MB;
        }
        else if (fileSize >= sz200GB && numBytesLimit > sz200_4MB)
        {
             maxBytesOfSlipFile = sz1000_4MB;
        }
        else
        {
            maxBytesOfSlipFile = numBytesLimit;
        }
    }
    else
    {
        maxBytesOfSlipFile = numBytesLimit;
    }

    cout<<"Max size of slipt file: "<< maxBytesOfSlipFile<<"(Byte)"<<endl;
    //================================================================================//

    // Clean foldr and file temp
    filesystem::remove_all(outFileName);
    filesystem::remove_all(dicTemp);

    // Create folder temp for processing data
    if(!filesystem::create_directories(dicTemp))
    {
        cout<<"Error! Can not create folder "<<dicTemp<<endl;
        return EXIT_FAILURE;
    }

    unsigned int countFile = 0;
    cout<<"Part1: Processing slipt file and sort write to k files"<<endl;
    sliptAndSortFile(inFileName, maxBytesOfSlipFile, countFile);
    cout<<"    => Number of Slipt files:"<<countFile<<endl;

    cout<<"Part2: Processing merg k sored small file to final output file"<<endl;
#ifdef OPTIMIZE_EXTERNAL_SORT_USING_MIN_HEAP
    mergeKfilesByUsingMinHeap(outFileName, countFile);
#else
    mergeKfilesByUsingRecursion(outFileName, 1, countFile);
#endif

    cout<<"Part3: Clean all temp file after sorted larg file"<<endl;
    if(!filesystem::remove_all(dicTemp))
    {
        cout<<"Error! Can not remve all file in folder "<<dicTemp<<endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void directSortSmallFileOnMemory(string inFileName, string outFileName)
{
    vector<wstring> lstLines;
    wifstream hInfile(inFileName, std::ios_base::in);
    hInfile.imbue(g_loc);

    wstring lineBuffer;

    while (getline(hInfile, lineBuffer))
    {
         lstLines.push_back(lineBuffer);
    }

    std::sort(lstLines.begin(), lstLines.end(), std::less<wstring>());

    writeToFile(outFileName, lstLines);

    hInfile.close();
}

void sliptAndSortFile(string inFileName, unsigned long long maxBytesOfSlipFile, unsigned int& countFile)
{
    wstring bufferLine;
    wifstream hInfile(inFileName, std::ios_base::in);

    // Support UTF-8 encoded byte string and UTF-16 encoded character string
    hInfile.imbue(std::locale(std::locale(),new std::codecvt_utf8_utf16<wchar_t, 0x10ffff, std::codecvt_mode(std::consume_header|std::generate_header)>));
    hInfile.imbue(g_loc);

    unsigned int bufferNumBytes;
    countFile = 0;

    vector<wstring> lstLines;
    bufferNumBytes = 0;

    while(getline(hInfile, bufferLine))
    {
        unsigned long long szByteLine = bufferLine.length() * sizeof (wchar_t);
        if(bufferNumBytes + szByteLine > maxBytesOfSlipFile)
        {
            //Don't create empty file if lstLines is empty
            if(!lstLines.empty())
            {
                countFile++;
                std::sort(lstLines.begin(), lstLines.end(), std::less<wstring>());
                string fileName = std::string(dicTemp) + std::to_string(countFile);
                writeToFile(fileName, lstLines);
            }

            bufferNumBytes = 0;
            lstLines.clear();
        }
        bufferNumBytes += szByteLine;
        lstLines.push_back(bufferLine);
    }

    // In case: pointe file go to end file and remain buffer. it should be check and write to file
    if(!lstLines.empty())
    {
        countFile++;
        std::sort(lstLines.begin(), lstLines.end(), std::less<wstring>());

        string fileName =  std::string(dicTemp) + std::to_string(countFile);
        writeToFile(fileName, lstLines);
    }

    hInfile.close();
}

void mergeKfilesByUsingRecursion(string outFileName, unsigned long long leftFileIdx, unsigned long long rightFileIdx)
{
    // Base case 1
    if(leftFileIdx == rightFileIdx)
    {
        writeToFile( std::string(dicTemp) + std::to_string(leftFileIdx), outFileName);
        return;
    }

    // Base case 2
    if(rightFileIdx - leftFileIdx == 1)
    {
        mergeTwoSmallFile( std::string(dicTemp) + std::to_string(leftFileIdx),  std::string(dicTemp) + std::to_string(rightFileIdx), outFileName);
        return;
    }

    g_OutFileCount++;
    string outFileName1 =  std::string(dicTemp) + "outTem1_" + std::to_string(g_OutFileCount);
    string outFileName2 =  std::string(dicTemp)+ "outTem2_" + std::to_string(g_OutFileCount);

    mergeKfilesByUsingRecursion(outFileName1, leftFileIdx, (leftFileIdx + rightFileIdx)/2);
    mergeKfilesByUsingRecursion(outFileName2, (leftFileIdx + rightFileIdx)/2 + 1, rightFileIdx);

    mergeTwoSmallFile(outFileName1, outFileName2, outFileName);
}


bool compareLess(const FileInfo& fileInfo1, const FileInfo& fileInfo2)
{
    return (fileInfo1.currentLine > fileInfo2.currentLine);
}

void mergeKfilesByUsingMinHeap(string outFileName, unsigned long long totallFile)
{
    vector<FileInfo> hListInfo(totallFile);
    int fileIdx = 0;

    for(unsigned long long idx = 0; idx < totallFile; idx++)
    {
        fileIdx++;
        FileInfo item(std::string(dicTemp) + std::to_string(fileIdx));

        // Check open file
        if(item.isEndFile)
        {
            cout<<"Please chose another number bytes limit"<<endl;
            return;
        }

        hListInfo[idx] = item;
    }

    wofstream hOutFile(outFileName,ios_base::out);
    hOutFile.imbue(g_loc);
    if(!hOutFile.is_open())
    {
       cout<<"Error! Can not open new file"<<endl;
       return;
    }

    // Create and build Min Heap
    std::make_heap(hListInfo.begin(), hListInfo.end(), compareLess);

    while (!hListInfo.empty())
    {
        hOutFile<<hListInfo.front().currentLine<<endl;

        FileInfo nextItemRoot = FileInfo(hListInfo.front().hFile);

        //Remote root
        pop_heap(hListInfo.begin(), hListInfo.end(), compareLess);
        hListInfo.pop_back();

        if(!nextItemRoot.isEndFile)
        {
            //Update next min root of Min Heap
            hListInfo.push_back(nextItemRoot);
            std::push_heap(hListInfo.begin(), hListInfo.end(), compareLess);
        }
    }
}

void mergeTwoSmallFile(string inFile1, string inFile2, string outFile)
{
    wfstream hInFile1(inFile1, std::ios_base::in);
    wfstream hInFile2(inFile2, std::ios_base::in);
    hInFile1.imbue(g_loc);
    hInFile2.imbue(g_loc);

    wofstream hOutFile(outFile,ios_base::out);
    hOutFile.imbue(g_loc);

    wstring outLine;
    wstreampos previPos1, previPos2;

    bool isNotEnd1 = true;
    bool isNotEnd2 = true;

    while(true)
    {
        wstring bufferLine1, bufferLine2;

        if(isNotEnd1 && !getline(hInFile1, bufferLine1))
        {
            isNotEnd1 = false;
        }

        if(isNotEnd2 && !getline(hInFile2, bufferLine2))
        {
            isNotEnd2 = false;
        }

         if (isNotEnd1 && isNotEnd2)
         {
              outLine = bufferLine1 < bufferLine2 ? bufferLine1: bufferLine2;

              if( bufferLine1 < bufferLine2)
              {
                  // Back possition pointer file
                  hInFile2.seekg(previPos2);
              }
              else if (bufferLine1 > bufferLine2) {
                  // Back possition pointer file
                  hInFile1.seekg(previPos1);
              }
         }
        else if (isNotEnd1 == true && isNotEnd2 == false) {
            outLine = bufferLine1;
        }
        else if (isNotEnd1 == false && isNotEnd2 == true) {
            outLine = bufferLine2;
        }
        else
        {
           break;
        }

        hOutFile<<outLine<<endl;

        if(bufferLine1 == bufferLine2)
        {
            hOutFile<<outLine<<endl;
        }

        previPos1 = hInFile1.tellg();
        previPos2 = hInFile2.tellg();
    }
}

bool checkExitFileAndReadable(string fileName)
{
     wifstream hfile(fileName, std::ios_base::in);
     return hfile.is_open();
}

void writeToFile(string infileName, string outfileName)
{
    wfstream hInFile(infileName, std::ios_base::in);
    wofstream hOutFile(outfileName,ios_base::out);
    hInFile.imbue(g_loc);
    hOutFile.imbue(g_loc);

    wstring bufferLine;

    while(getline(hInFile, bufferLine))
    {
        hOutFile<<bufferLine<<endl;
    }
    hOutFile.close();
}

void writeToFile(string fileName, vector<wstring> listLine)
{
    wofstream hFile;
    hFile.open(fileName, std::ios_base::out);
    hFile.imbue(g_loc);

    for(auto strLine : listLine)
    {
        hFile<<strLine<<endl;
    }
    hFile.close();
}

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

