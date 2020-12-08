#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <random>
#include <filesystem>
#include <chrono>

#define OPTIMIZE_EXTERNAL_SORT_USING_MIN_HEAP 1

#define ONE_MB 1048576 //1024*1024
#define ONE_GB 1073741824 //1024*1024*1024

unsigned int g_OutFileCount = 0;
const char* dicTemp = "FolderTemp/";

using namespace std;

typedef struct FileInfo
{
    string currentLine;
    std::shared_ptr<fstream> hFile;
    bool isEndFile = false;

    FileInfo()
    {
        currentLine = "";
         isEndFile = false;
    }

    FileInfo(string fileName)
    {
        hFile = shared_ptr<fstream>(new fstream(fileName, std::ios_base::in),std::default_delete<fstream>());
        if(!getline(*hFile, this->currentLine, '\n'))
        {
            isEndFile = true;
            hFile->close();
        }
    }

    FileInfo(std::shared_ptr<fstream> &hFilevalue)
    {
        this->hFile = hFilevalue;
        if(!getline(*hFile, this->currentLine, '\n'))
        {
            isEndFile = true;
            hFile->close();
        }

    }

}FileInfo;

bool externalSortLargFile(string inFileName, string outFileName, unsigned long long numBytesLimit);
void mergeKfilesByUsingMinHeap(string outFileName, unsigned long long totallFile);
void mergeTwoSmallFile(string inFile1, string inFile2, string outFile);
void sliptAndSortFile(string inFileName, unsigned long long maxBytesOfSlipFile, unsigned int& countFile);
void mergeKfiles(string outFileName, unsigned long long leftFileIdx, unsigned long long rightFileIdx);
void directSortSmallFileOnMemory(string inFileName, string outFileName);

//Helper function
bool genFileWithNumLines(string fileName, unsigned long long totallLines, unsigned int minSizeOfLine, unsigned int maxSizeOfLine);
bool genFileWithNumBytesLimit(string fileName, unsigned long long numBytesLimit);
std::string random_string( size_t length );
void writeToFile(string fileName, vector<string> listLine);
unsigned int randomNumber(unsigned int rangeFrom, unsigned int rangeTo);
void writeToFile(string infileName, string outfileName);
bool checkExitFileAndReadable(string fileName);
bool compareGreaters(const FileInfo& fileInfo1, const FileInfo& fileInfo2)
{
    return (fileInfo1.currentLine > fileInfo2.currentLine);
}

int main(int argc, char* argv[])
{
    int bFunc = EXIT_SUCCESS;

    string inFileName = "";
    string outFilename="";

    unsigned long long numBytesLimit = 0;
    if(argc == 4)
    {
        // get paramater from command line
        inFileName = argv[1];
        outFilename = argv[2];
        numBytesLimit = std::stoull(argv[3]);

        if(inFileName == "" || outFilename == "" || numBytesLimit == 0)
        {
            cout<<"Error! Invalid paramater from command line.\n";
            return EXIT_FAILURE;
        }
    }
    else
    {
        return EXIT_FAILURE;
    }

    // set start time
    auto start = std::chrono::high_resolution_clock::now();

    bFunc = externalSortLargFile(inFileName, outFilename, numBytesLimit);

    // Get execution time
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(end - start);
    cout<<"Execution time on externalSortLargFile function:"<<duration.count()<<"(s)"<<endl;

    return bFunc;
}

bool externalSortLargFile(string inFileName, string outFileName, unsigned long long numBytesLimit)
{
    if(!checkExitFileAndReadable(inFileName))
    {
        cout<<"Error Open file! Please check ["<<inFileName<<"]file is exit or not."<<endl;
        return EXIT_FAILURE;
    }

    // Create folder temp for processing data
    if(!filesystem::create_directories(dicTemp))
    {
        cout<<"Error! Can not create folder "<<dicTemp<<endl;
        return EXIT_FAILURE;
    }

    std::uintmax_t fileSize =  std::filesystem::file_size(inFileName);
    double sumGigagbyte = double(fileSize)/1000000000;
    cout<<"Total size of file as bytes: "<< fileSize<<"(Byte)"<<endl;
    cout<<"Total size of file as gigagbyte: "<< sumGigagbyte<<"(GB)"<<endl;
    unsigned long long maxBytesOfSlipFile = 0;

    //=========Caculate max size of slipt file====================//
    //Default it will use numBytesLimit for each file
    unsigned long long sz10MB = (unsigned long long)ONE_MB*10;
    unsigned long long sz100MB = (unsigned long long)ONE_MB*100;
    unsigned long long sz200MB = (unsigned long long)ONE_MB*200;
    unsigned long long sz1000MB = (unsigned long long)ONE_MB*1000;

    unsigned long long sz10GB = (unsigned long long)ONE_GB*10;
    unsigned long long sz100GB = (unsigned long long)ONE_GB*100;
    unsigned long long sz200GB = (unsigned long long)ONE_GB*200;

    if(fileSize < ONE_GB && numBytesLimit > ONE_MB)
    {
        // Case small file
        if(fileSize < ONE_MB)
        {
            directSortSmallFileOnMemory(inFileName, outFileName);
            return EXIT_SUCCESS;
        }

        maxBytesOfSlipFile = ONE_MB;

    }
    else if(fileSize >= ONE_GB && fileSize < sz10GB && numBytesLimit > sz10MB)
    {
        maxBytesOfSlipFile = sz10MB;
    }
    else if (fileSize >= sz10GB &&  fileSize < sz100GB && numBytesLimit > sz100MB)
    {
        maxBytesOfSlipFile = sz100MB;
    }
    else if (fileSize >= sz100GB &&  fileSize < sz200GB && numBytesLimit > sz200MB)
    {
         maxBytesOfSlipFile = sz200MB;
    }
    else if (fileSize >= sz200GB && numBytesLimit > sz200MB)
    {
         maxBytesOfSlipFile = sz1000MB;
    }
    else
    {
        maxBytesOfSlipFile = numBytesLimit;
    }
    cout<<"Max size of slipt file: "<< maxBytesOfSlipFile<<"(Byte)"<<endl;
    //================================================================================//

    unsigned int countFile = 0;
    cout<<"Part1: Processing slipt file and sort write to k files"<<endl;
    sliptAndSortFile(inFileName, maxBytesOfSlipFile, countFile);
    cout<<"    => Number of Slipt file:"<<countFile<<endl;

    cout<<"Part2: Processing merg k sored small file to final output file"<<endl;
#ifdef OPTIMIZE_EXTERNAL_SORT_USING_MIN_HEAP
    mergeKfilesByUsingMinHeap(outFileName, countFile);
#else
    mergeKfiles(outFileName, 1, countFile);
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
    vector<string> lstLines;
    fstream hInfile(inFileName, std::ios_base::in);
    string lineBuffer;

    while (getline(hInfile, lineBuffer, '\n'))
    {
         lstLines.push_back(lineBuffer);
    }

    std::sort(lstLines.begin(), lstLines.end());

    writeToFile(outFileName, lstLines);

    hInfile.close();
}

void sliptAndSortFile(string inFileName, unsigned long long maxBytesOfSlipFile, unsigned int& countFile)
{
    string bufferLine;
    fstream hInfile(inFileName, std::ios_base::in);
    unsigned int bufferNumBytes;
    streampos previPos;
    countFile = 0;

    vector<string> lstLines;
    bufferNumBytes = 0;

    while(getline(hInfile, bufferLine, '\n'))
    {
        if(bufferNumBytes + bufferLine.length() > maxBytesOfSlipFile)
        {
            countFile++;
            std::sort(lstLines.begin(), lstLines.end());
            string fileName = std::string(dicTemp) + std::to_string(countFile);
            writeToFile(fileName, lstLines);

            bufferNumBytes = 0;
            lstLines.clear();
        }
        bufferNumBytes += bufferLine.length();
        lstLines.push_back(bufferLine);
    }

    // In case: pointe file go to end file and remain buffer. it should be check and write to file
    if(!lstLines.empty())
    {
        countFile++;
        std::sort(lstLines.begin(), lstLines.end());
        string fileName =  std::string(dicTemp) + std::to_string(countFile);
        writeToFile(fileName, lstLines);
    }

}

void mergeKfiles(string outFileName, unsigned long long leftFileIdx, unsigned long long rightFileIdx)
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

    mergeKfiles(outFileName1, leftFileIdx, (leftFileIdx + rightFileIdx)/2);
    mergeKfiles(outFileName2, (leftFileIdx + rightFileIdx)/2 + 1, rightFileIdx);

    mergeTwoSmallFile(outFileName1, outFileName2, outFileName);
}

void mergeKfilesByUsingMinHeap(string outFileName, unsigned long long totallFile)
{
    vector<FileInfo> hListInfo(totallFile);
    int fileIdx = 0;

    for(unsigned long long idx = 0; idx < totallFile; idx++)
    {
        fileIdx++;
        hListInfo[idx] = FileInfo(std::string(dicTemp) + std::to_string(fileIdx));
    }

    ofstream hOutFile(outFileName,ios_base::out);

    // Create Min Heap
    std::make_heap(hListInfo.begin(), hListInfo.end(), compareGreaters);

    //compareGreaters
    while (!hListInfo.empty())
    {
        hOutFile<<hListInfo.front().currentLine<<endl;

        FileInfo nextItemRoot = FileInfo(hListInfo.front().hFile);

        //Remote root
        pop_heap(hListInfo.begin(), hListInfo.end(), compareGreaters);
        hListInfo.pop_back();

        if(!nextItemRoot.isEndFile)
        {
            //Update next root of Min Heap
            hListInfo.push_back(nextItemRoot);
            std::push_heap(hListInfo.begin(), hListInfo.end(), compareGreaters);
        }
    }
}

void mergeTwoSmallFile(string inFile1, string inFile2, string outFile)
{
    fstream hInFile1(inFile1, std::ios_base::in);
    fstream hInFile2(inFile2, std::ios_base::in);

    ofstream hOutFile(outFile,ios_base::out);
    string outLine;
    streampos previPos1, previPos2;

    while(true)
    {
        string bufferLine1 = "", bufferLine2 = "";

        getline(hInFile1, bufferLine1, '\n');
        getline(hInFile2, bufferLine2, '\n');

         if (bufferLine1 != "" && bufferLine2 != "")
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
        else if (bufferLine1 != "" && bufferLine2 == "") {
            outLine = bufferLine1;
        }
        else if (bufferLine1 == "" && bufferLine2 != "") {
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
     fstream hfile(fileName, std::ios_base::in);
     return hfile.is_open();
}

void writeToFile(string infileName, string outfileName)
{
    fstream hInFile(infileName, std::ios_base::in);
    ofstream hOutFile(outfileName,ios_base::out);
    string bufferLine;

    while(getline(hInFile, bufferLine, '\n'))
    {
        hOutFile<<bufferLine<<endl;
    }
    hOutFile.close();
}

void writeToFile(string fileName, vector<string> listLine)
{
    ofstream hFile;
    hFile.open(fileName, std::ios_base::out);

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
//        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    //str[length - 1] = '\n';
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

void createDataTestByGenLargFile(string inFileName)
{

    //Test Case 1
//    unsigned long long totallLines = 20000000;
//    unsigned int minSizeOfLine = 20;
//    unsigned int maxSizeOfLine = 100;
//    genFileWithNumLines(std::string("InTestData/") + inFileName, totallLines, minSizeOfLine, maxSizeOfLine);

    // gen in file
    //int numBytesIn = 600;
    unsigned long long totallLines = 70;
    unsigned int minSizeOfLine = 20;
    unsigned int maxSizeOfLine = 100;
    genFileWithNumLines(inFileName, totallLines, minSizeOfLine, maxSizeOfLine);

}
