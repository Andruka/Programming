#include <iostream>
#include <fstream>
#include <string.h>
#define CP1251_INDEX    1
#define CP1251_SYMBOL_O 0xee

#define CP866_INDEX     2
#define CP866_SYMBOL_O  0xae

#define KOI8R_INDEX     3
#define KOI8R_SYMBOL_O  0xcf

using namespace std;

static const unsigned char Cp1251ToCp866[ 128 ] = {

    0x80,  0x81,  0x82,  0x83,  0x84,  0x85,  0x86,  0x87,
    0x88,  0x89,  0x8a,  0x8b,  0x8c,  0x8d,  0x8e,  0x8f,
    0x90,  0x91,  0x92,  0x93,  0x94,  0xf9,  0x96,  0xc4,
    0x98,  0x99,  0x9a,  0x9b,  0x9c,  0x9d,  0x9e,  0x9f,
    0xff,  0xf6,  0xf7,  0xa3,  0xfd,  0xa5,  0xa6,  0xa7,
    0xf0,  0xa9,  0xf2,  0xab,  0xac,  0xad,  0xae,  0xf4,
    0xf8,  0xb1,  0xb2,  0xb3,  0xb4,  0xb5,  0xb6,  0xfa,
    0xf1,  0xfc,  0xf3,  0xbb,  0xbc,  0xbd,  0xbe,  0xf5,
    0x80,  0x81,  0x82,  0x83,  0x84,  0x85,  0x86,  0x87,
    0x88,  0x89,  0x8a,  0x8b,  0x8c,  0x8d,  0x8e,  0x8f,
    0x90,  0x91,  0x92,  0x93,  0x94,  0x95,  0x96,  0x97,
    0x98,  0x99,  0x9a,  0x9b,  0x9c,  0x9d,  0x9e,  0x9f,
    0xa0,  0xa1,  0xa2,  0xa3,  0xa4,  0xa5,  0xa6,  0xa7,
    0xa8,  0xa9,  0xaa,  0xab,  0xac,  0xad,  0xae,  0xaf,
    0xe0,  0xe1,  0xe2,  0xe3,  0xe4,  0xe5,  0xe6,  0xe7,
    0xe8,  0xe9,  0xea,  0xeb,  0xec,  0xed,  0xee,  0xef
};

static const unsigned char Cp1251ToKoi8R[ 128 ] = {

    0x80,  0x81,  0x82,  0x83,  0x84,  0x85,  0x86,  0x87,
    0x88,  0x89,  0x8a,  0x8b,  0x8c,  0x8d,  0x8e,  0x8f,
    0x90,  0x91,  0x92,  0x93,  0x94,  0x95,  0x96,  0x80,
    0x98,  0x99,  0x9a,  0x9b,  0x9c,  0x9d,  0x9e,  0x9f,
    0x9a,  0xa1,  0xa2,  0xa3,  0xa4,  0xa5,  0xa6,  0xa7,
    0xb3,  0xbf,  0xaa,  0xab,  0xac,  0xad,  0xae,  0xaf,
    0x9c,  0xb1,  0xb2,  0xb3,  0xb4,  0xb5,  0xb6,  0x9e,
    0xa3,  0xb9,  0xba,  0xbb,  0xbc,  0xbd,  0xbe,  0xbf,
    0xe1,  0xe2,  0xf7,  0xe7,  0xe4,  0xe5,  0xf6,  0xfa,
    0xe9,  0xea,  0xeb,  0xec,  0xed,  0xee,  0xef,  0xf0,
    0xf2,  0xf3,  0xf4,  0xf5,  0xe6,  0xe8,  0xe3,  0xfe,
    0xfb,  0xfd,  0xff,  0xf9,  0xf8,  0xfc,  0xe0,  0xf1,
    0xc1,  0xc2,  0xd7,  0xc7,  0xc4,  0xc5,  0xd6,  0xda,
    0xc9,  0xca,  0xcb,  0xcc,  0xcd,  0xce,  0xcf,  0xd0,
    0xd2,  0xd3,  0xd4,  0xd5,  0xc6,  0xc8,  0xc3,  0xde,
    0xdb,  0xdd,  0xdf,  0xd9,  0xd8,  0xdc,  0xc0,  0xd1
};

static const unsigned char Cp866ToCp1251[ 128 ] = {

    0xc0,  0xc1,  0xc2,  0xc3,  0xc4,  0xc5,  0xc6,  0xc7,
    0xc8,  0xc9,  0xca,  0xcb,  0xcc,  0xcd,  0xce,  0xcf,
    0xd0,  0xd1,  0xd2,  0xd3,  0xd4,  0xd5,  0xd6,  0xd7,
    0xd8,  0xd9,  0xda,  0xdb,  0xdc,  0xdd,  0xde,  0xdf,
    0xe0,  0xe1,  0xe2,  0xe3,  0xe4,  0xe5,  0xe6,  0xe7,
    0xe8,  0xe9,  0xea,  0xeb,  0xec,  0xed,  0xee,  0xef,
    0xb0,  0xb1,  0xb2,  0xb3,  0xb4,  0xb5,  0xb6,  0xb7,
    0xb8,  0xb9,  0xba,  0xbb,  0xbc,  0xbd,  0xbe,  0xbf,
    0xc0,  0xc1,  0xc2,  0xc3,  0x97,  0xc5,  0xc6,  0xc7,
    0xc8,  0xc9,  0xca,  0xcb,  0xcc,  0xcd,  0xce,  0xcf,
    0xd0,  0xd1,  0xd2,  0xd3,  0xd4,  0xd5,  0xd6,  0xd7,
    0xd8,  0xd9,  0xda,  0xdb,  0xdc,  0xdd,  0xde,  0xdf,
    0xf0,  0xf1,  0xf2,  0xf3,  0xf4,  0xf5,  0xf6,  0xf7,
    0xf8,  0xf9,  0xfa,  0xfb,  0xfc,  0xfd,  0xfe,  0xff,
    0xa8,  0xb8,  0xaa,  0xba,  0xaf,  0xbf,  0xa1,  0xa2,
    0xb0,  0x95,  0xb7,  0xfb,  0xb9,  0xa4,  0xfe,  0xa0
};

static const unsigned char Cp866ToKoi8R[ 128 ] = {

    0xe1,  0xe2,  0xf7,  0xe7,  0xe4,  0xe5,  0xf6,  0xfa,
    0xe9,  0xea,  0xeb,  0xec,  0xed,  0xee,  0xef,  0xf0,
    0xf2,  0xf3,  0xf4,  0xf5,  0xe6,  0xe8,  0xe3,  0xfe,
    0xfb,  0xfd,  0xff,  0xf9,  0xf8,  0xfc,  0xe0,  0xf1,
    0xc1,  0xc2,  0xd7,  0xc7,  0xc4,  0xc5,  0xd6,  0xda,
    0xc9,  0xca,  0xcb,  0xcc,  0xcd,  0xce,  0xcf,  0xd0,
    0x90,  0x91,  0x92,  0x81,  0x87,  0xb2,  0xb4,  0xa7,
    0xa6,  0xb5,  0xa1,  0xa8,  0xae,  0xad,  0xac,  0x83,
    0x84,  0x89,  0x88,  0x86,  0x80,  0x8a,  0xaf,  0xb0,
    0xab,  0xa5,  0xbb,  0xb8,  0xb1,  0xa0,  0xbe,  0xb9,
    0xba,  0xb6,  0xb7,  0xaa,  0xa9,  0xa2,  0xa4,  0xbd,
    0xbc,  0x85,  0x82,  0x8d,  0x8c,  0x8e,  0x8f,  0x8b,
    0xd2,  0xd3,  0xd4,  0xd5,  0xc6,  0xc8,  0xc3,  0xde,
    0xdb,  0xdd,  0xdf,  0xd9,  0xd8,  0xdc,  0xc0,  0xd1,
    0xb3,  0xa3,  0xf2,  0xf3,  0xf4,  0xf5,  0xf6,  0xf7,
    0x9c,  0x95,  0x9e,  0x96,  0xfc,  0xfd,  0x94,  0x9a
};

static const unsigned char Koi8RToCp1251[ 128 ] = {

    0x97,  0x81,  0x82,  0x83,  0x84,  0x85,  0x86,  0x87,
    0x88,  0x89,  0x8a,  0x8b,  0x8c,  0x8d,  0x8e,  0x8f,
    0x90,  0x91,  0x92,  0x93,  0x94,  0x95,  0x96,  0x97,
    0x98,  0x99,  0xa0,  0x9b,  0xb0,  0x9d,  0xb7,  0x9f,
    0xa0,  0xa1,  0xa2,  0xb8,  0xa4,  0xa5,  0xa6,  0xa7,
    0xa8,  0xa9,  0xaa,  0xab,  0xac,  0xad,  0xae,  0xaf,
    0xb0,  0xb1,  0xb2,  0xa8,  0xb4,  0xb5,  0xb6,  0xb7,
    0xb8,  0xb9,  0xba,  0xbb,  0xbc,  0xbd,  0xbe,  0xa9,
    0xfe,  0xe0,  0xe1,  0xf6,  0xe4,  0xe5,  0xf4,  0xe3,
    0xf5,  0xe8,  0xe9,  0xea,  0xeb,  0xec,  0xed,  0xee,
    0xef,  0xff,  0xf0,  0xf1,  0xf2,  0xf3,  0xe6,  0xe2,
    0xfc,  0xfb,  0xe7,  0xf8,  0xfd,  0xf9,  0xf7,  0xfa,
    0xde,  0xc0,  0xc1,  0xd6,  0xc4,  0xc5,  0xd4,  0xc3,
    0xd5,  0xc8,  0xc9,  0xca,  0xcb,  0xcc,  0xcd,  0xce,
    0xcf,  0xdf,  0xd0,  0xd1,  0xd2,  0xd3,  0xc6,  0xc2,
    0xdc,  0xdb,  0xc7,  0xd8,  0xdd,  0xd9,  0xd7,  0xda
};

static const unsigned char Koi8RToCp866[ 128 ] = {

    0xc4,  0xb3,  0xda,  0xbf,  0xc0,  0xd9,  0xc3,  0xb4,
    0xc2,  0xc1,  0xc5,  0xdf,  0xdc,  0xdb,  0xdd,  0xde,
    0xb0,  0xb1,  0xb2,  0x93,  0xfe,  0xf9,  0xfb,  0x97,
    0x98,  0x99,  0xff,  0x9b,  0xf8,  0x9d,  0xfa,  0x9f,
    0xcd,  0xba,  0xd5,  0xf1,  0xd6,  0xc9,  0xb8,  0xb7,
    0xbb,  0xd4,  0xd3,  0xc8,  0xbe,  0xbd,  0xbc,  0xc6,
    0xc7,  0xcc,  0xb5,  0xf0,  0xb6,  0xb9,  0xd1,  0xd2,
    0xcb,  0xcf,  0xd0,  0xca,  0xd8,  0xd7,  0xce,  0xbf,
    0xee,  0xa0,  0xa1,  0xe6,  0xa4,  0xa5,  0xe4,  0xa3,
    0xe5,  0xa8,  0xa9,  0xaa,  0xab,  0xac,  0xad,  0xae,
    0xaf,  0xef,  0xe0,  0xe1,  0xe2,  0xe3,  0xa6,  0xa2,
    0xec,  0xeb,  0xa7,  0xe8,  0xed,  0xe9,  0xe7,  0xea,
    0x9e,  0x80,  0x81,  0x96,  0x84,  0x85,  0x94,  0x83,
    0x95,  0x88,  0x89,  0x8a,  0x8b,  0x8c,  0x8d,  0x8e,
    0x8f,  0x9f,  0x90,  0x91,  0x92,  0x93,  0x86,  0x82,
    0x9c,  0x9b,  0x87,  0x98,  0x9d,  0x99,  0x97,  0x9a
};

class Decryptor{
    int frequency [128];
    unsigned char* letter [4];
    int baseKod, deep, indent;

    char     iFileName[100];
    ifstream iFile;
    char     oFileName[100];
    ofstream oFile;

public:
    Decryptor(){
        int i, j, el = 1;
        baseKod = deep = indent = 0;
        for ( i = 0; i< 128; i++){
            frequency[ i ] = 0;
        }
        for (j=0;j<4;j++){
            letter[j] = new unsigned char[el];
            for(i=0;i < el; ++i){
                letter[j][i] = 0;
            }
            el = el * 6;
        }
    }

    ~Decryptor(){
        for (int j = 0; j < 4; j++){
            delete [] letter[j];
        }
    }

    int start(const char* iName, const char* oName){
        strcpy(iFileName, iName);
        strcpy(oFileName, oName);
        if (readFile() == 1){
            cout << "file is not finded" << endl;
            return 1;
        }
        if (findMostPopular() == 1){
            return 2;
        }
        generateLetter();
        if (checkLetter() == 1) return 3;
        showBase();

        if (deep == 0){
            cout << "file is correct" << endl;
            return 0;
        }
        writeFile();
        return 0;
    }

    int readFile(){
        char c;
        iFile.open(iFileName);
        if(!iFile) return 1;
        for ( int i = 0; i < 1000; i++){
        if (iFile.eof()) break;
            iFile.get(c);
            if(c&0x80) ++frequency[ c&0x7f ];
        }
        iFile.close();
        return 0;
    }

    int findMostPopular(){
        int mostPopular = 0;
        for (int i = 0; i < 128; i++){
            if (frequency [mostPopular] < frequency[ i ]){
                mostPopular = i;
            }
        }
        if (frequency[mostPopular] == 0) return 1;
        letter[0][0] = mostPopular | 0x80;
        return 0;
    }


    void writeFile(){
        unsigned char fromTo [128];
        int steps [deep];
        int i,j = indent;
        char c;
        for ( i = deep - 1; i != -1 ; --i) {
            steps [ i ] = j % 6;
            j = j / 6;
        }
        for ( j = 0;j < deep; ++j){
            showStep ( steps[j] );
        }

        for (i = 0; i < 128; ++i){
            fromTo[i] = i|0x80;
            for( j = 0;j<deep;++j){
                fromTo[i]= newSymbol(fromTo[i],steps[j]);
            }
        }

        iFile.open (iFileName);
        oFile.open (oFileName);
        while ( !iFile.eof() ) {
            iFile.get ( c );
            if ( c & 0x80 ) oFile << (char)fromTo[ c & 0x7f ];
            else oFile << c;
        }
    }

    unsigned char newSymbol(unsigned char symbol,int rule){
        if (rule == 0) return Cp1251ToCp866 [ symbol & 0x7f ];
        if (rule == 1) return Cp1251ToKoi8R [ symbol & 0x7f ];
        if (rule == 2) return Cp866ToCp1251 [ symbol & 0x7f ];
        if (rule == 3) return Cp866ToKoi8R  [ symbol & 0x7f ];
        if (rule == 4) return Koi8RToCp1251 [ symbol & 0x7f ];
        if (rule == 5) return Koi8RToCp866  [ symbol & 0x7f ];
    }

    void generateLetter(){
        int i,j, max = 1;
        for( i = 0; i < 3; i++){
            for( j = 0; j < max;j++){
                letter [i + 1] [j * 6 + 0] = Cp1251ToCp866[ letter [i][j] & 0x7f ];
                letter [i + 1] [j * 6 + 1] = Cp1251ToKoi8R[ letter [i][j] & 0x7f ];
                letter [i + 1] [j * 6 + 2] = Cp866ToCp1251[ letter [i][j] & 0x7f ];
                letter [i + 1] [j * 6 + 3] = Cp866ToKoi8R [ letter [i][j] & 0x7f ];
                letter [i + 1] [j * 6 + 4] = Koi8RToCp1251[ letter [i][j] & 0x7f ];
                letter [i + 1] [j * 6 + 5] = Koi8RToCp866 [ letter [i][j] & 0x7f ];
            }
            max = max * 6;
        }
    }

    int checkLetter(){
       int i,j,max = 1;
       for ( i = 0; i< 4; ++i){
           for( j = 0; j < max; ++j){
               if(checkSymbol(letter[i][j])){
                   deep = i ;
                   indent = j;
                   return 0;
               }
           }
           max = max * 6;
       }
       return 1;
    }

    int checkSymbol(unsigned char letter){
        if (letter == CP1251_SYMBOL_O ) baseKod = CP1251_INDEX;
        if (letter == CP866_SYMBOL_O  ) baseKod = CP866_INDEX;
        if (letter == KOI8R_SYMBOL_O  ) baseKod = KOI8R_INDEX;
        return baseKod;
    }

    void showBase(){
        if (baseKod == CP1251_INDEX) cout << "file is CP1251" <<endl;
        if (baseKod == CP866_INDEX ) cout << "file is CP866"  <<endl;
        if (baseKod == KOI8R_INDEX ) cout << "file is KOI8R"  <<endl;
    }

    void showStep(int step){
        if (step == 0) std::cout << "Cp1251 -> Cp866 " << std::endl ;
        if (step == 1) std::cout << "Cp1251 -> Koi8R " << std::endl ;
        if (step == 2) std::cout << "Cp866  -> Cp1251" << std::endl ;
        if (step == 3) std::cout << "Cp866  -> Koi8R " << std::endl ;
        if (step == 4) std::cout << "Koi8R  -> Cp1251" << std::endl ;
        if (step == 5) std::cout << "Koi8R  -> Cp866 " << std::endl ;

    }
};

int main(int argc, char *argv[])
{
    Decryptor decryptor;
    if (argc<2) return 1;
    decryptor.start(argv[1],argv[2]);
    return 0;
}