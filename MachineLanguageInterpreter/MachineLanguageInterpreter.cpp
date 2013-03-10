//
// Project: MACHINE LANGUAGE INTERPRETER
// Author : FIRAT KARAKUSOGLU
// Email  : firatkarakusoglu@gmail.com
// Date   : 2007
//

#include <cstdlib>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <cstdlib>
#include <math.h>
#include <fstream>
#include <stdlib.h>

using namespace std;

//LOAD M(X) Transfer M(X) to the accumulator
char LOAD1[] = "1"; //load = 1

//LOAD - M(X) Transfer -M(X) to the accumulator
char LOAD2[] = "10"; //load = 2

//LOAD |M(X)| Transfer  absolute value of M(X) to the accumulator
char LOAD3[] = "11"; //load = 3

//LOAD - |M(X)| Transfer -|M(X)| to the accumulator
char LOAD4[] = "100"; //load = 4

//STORE - M(X) Store contents of accumulator on memory location X
char STOR[] = "101"; //store = 5

//LSH Multiply accumulator by 2
char LSH[]  = "110"; //lsh = 6

//RSH Divide accumulator by 2
char RSH[]  = "111"; //Rsh = 7

//ADD M(X) Add M(X) to AC; put the result in AC
char ADD[] = "1000"; //add = 8

//SUB M(X) Substract M(X) from AC; put the result in AC
char SUB[] = "1001"; //sub = 9

//IN Request (hex. based) data from the user and store it in AC
char IN[] ="1010"; //in = 10

//OUT Show (hex. based) data stored in AC on the screen
char OUT[] ="1011"; //in = 11

//HLT Ends program execution
char HLT[] ="1100"; //hlt = 12

int PC=0;
int MAR=0;
int MBR=0;
int IR=0;
int ACC=0;
int whatToDo=0;
int addressPart=0;
int countLine=0;
int dataInRam=0;
int cycyleCount=0;

struct Ram
{
       int oppCode;
       int addPart;
       int value;//opCode + addPart
       string strValue;
       };

typedef Ram ramType;
ramType _ram[4095];

int ram[4095]; //FROM "000" TO "FFF"

//SPLITTED LINE//
char *firstPart;
char *secondPart;
/////////////////

string convert(int i, int key)
{
  char buffer[33];
  switch(key)
  {
             case 2:  return itoa (i,buffer,2);break;
             case 10: return itoa (i,buffer,10);break;
             case 16: return itoa (i,buffer,16);break;
             default: return 0;
             }
}

void showMemory()
{
    cout<<"---------------------------"<<endl;
    cout<<"Values In Memory(Not Zero)"<<endl;
    cout<<"---------------------------"<<endl;
     for(int i=0;i<4095;i++)
     {
                        if(_ram[i].value != 0)
                        cout<<"M["<<convert(i,16)<<"] = "<<convert(_ram[i].value,16)<<endl;
                        }
     cout<<"---------------------------"<<endl;
     }

void write(char * caption="")
{
     cout<<endl<<"-------------------------"<<endl<<caption<<endl;
     cout<<"PC: "<<convert(PC,16)<<endl;
     cout<<"MAR : "<<convert(MAR,16)<<endl;
     cout<<"MBR : "<<convert(MBR,16) <<endl;
     cout<<"IR : "<<convert(IR,16) <<endl;
     cout<<"ACC : "<<convert(ACC,16) <<endl;
     cout<<"Op Code : "<<convert(whatToDo,16) <<endl;
     cout<<"Address Part : "<< convert(addressPart,16)<<endl;
     cout<<"-------------------------"<<endl;
     char c;
     cin>>c;

     if(c == 'm' || c=='M'){showMemory();cin.ignore();}
}

bool hexToDec (char* HexNumber, int& Number)
{
    char* pStopString;
    Number = strtol (HexNumber, &pStopString, 16);
    return (bool)(Number != LONG_MAX);
}

int binToDec(char *bin)
{
        int  b, k, m, n;
        int  len, sum = 0;

        len = strlen(bin) - 1;
        for(k = 0; k <= len; k++)
        {
                n = (bin[k] - '0');
                if ((n > 1) || (n < 0))
                {
                        //puts("\n\n ERROR! BINARY has only 1 and 0!\n");
                        return (0);
                }
                for(b = 1, m = len; m > k; m--){b *= 2;};
                sum = sum + n * b;
        }
        return(sum);
}

void fetch()
{
    cycyleCount++;
    cout<<endl<<cycyleCount<<". Fetch Cycle"<<endl;
    MAR = PC; //pc is equalized to memory address register
    PC++;
    dataInRam = _ram[MAR].value;
    MBR = dataInRam;
    write("End Fetching  Values");
};

void execute()
{
     cout<<endl<<cycyleCount<<". Execution Cycle"<<endl;
     IR = MBR;
     whatToDo = _ram[MAR].oppCode;
     addressPart = _ram[MAR].addPart;
     MAR = addressPart;
     dataInRam = _ram[MAR].value;
     MBR = dataInRam;

     char buffer[33];
     itoa(whatToDo,buffer,2);

     if(strcmp(buffer,LOAD1)==0)
     {
            cout<<endl<<"\"LOAD1\" Command";
            cout<<endl<<"LOAD M(X) Transfer M(X) to the accumulator";
            ACC = MBR;
            }
     else if(strcmp(buffer,LOAD2)==0)
     {
          cout<<endl<<"\"LOAD2\" Command";
          cout<<endl<<"LOAD - M(X) Transfer -M(X) to the accumulator";
          ACC = -MBR;
          }
     else if(strcmp(buffer,LOAD3)==0)
     {
          cout<<endl<<"\"LOAD3\" Command";
          cout<<endl<<"LOAD |M(X)| Transfer  absolute value of M(X) to the accumulator";
          ACC = abs(MBR);
          }
     else if(strcmp(buffer,LOAD4)==0)
     {
          cout<<endl<<"\"LOAD4\" Command";
          cout<<endl<<"LOAD - |M(X)| Transfer -|M(X)| to the accumulator";
          ACC = -(abs(MBR));
          }
     else if(strcmp(buffer,STOR)==0)
     {
          cout<<endl<<"\"STOR\" Command";
          cout<<endl<<"STORE - M(X) Store contents of accumulator on memory location X";
          _ram[addressPart].value = ACC;
          }
     else if(strcmp(buffer,LSH)==0)
     {
          cout<<endl<<"\"LSH\" Command";
          cout<<endl<<"LSH Multiply accumulator by 2";
          ACC *= 2;
          }
     else if(strcmp(buffer,RSH)==0)
     {
          cout<<endl<<"\"RSH\" Command";
          cout<<endl<<"RSH Divide accumulator by 2";
          ACC /= 2;
          }
     else if(strcmp(buffer,ADD)==0)
     {
          cout<<endl<<"\"ADD\" Command";
          cout<<endl<<"ADD M(X) Add M(X) to AC; put the result in AC";
          ACC += _ram[addressPart].value;
          }
     else if(strcmp(buffer,SUB)==0)
     {
          cout<<endl<<"\"SUB\" Command";
          cout<<endl<<"SUB M(X) Substract M(X) from AC; put the result in AC ";
          ACC -= _ram[addressPart].value;
          }
     else if(strcmp(buffer,IN)==0)
     {
          cout<<endl<<"\"IN\" Command";
          cout<<endl<<"IN Request (hex. based) data from the user and store it in AC";
          cout<<endl<<"Enter a number between 0 and FFFF: ";
          char chrTemp[10];
          int intTemp;
          cin>>chrTemp;
          hexToDec(chrTemp,intTemp);
          if(intTemp<65535 && intTemp>0){hexToDec(chrTemp,ACC);}
          else
          {
              while(!(intTemp<65535 && intTemp>0))
              {
                         cout<<endl<<"ERROR! Given value should be less than FFFF in hex base";
                         cout<<endl<<"Enter a number between 0 and FFFF: ";
                         chrTemp[0] = '\0';
                         cin>>chrTemp; hexToDec(chrTemp,intTemp);
                         }
              hexToDec(chrTemp,ACC);
              };


          }
     else if(strcmp(buffer,OUT)==0)
     {
          cout<<endl<<"\"OUT\" Command";
          cout<<endl<<"OUT Show (hex. based) data stored in AC on the screen";
          cout<<endl<<"Value is "<<convert(ACC,16)<<endl; cin.ignore();
          }
     else if(strcmp(buffer,HLT)==0)
     {
          cout<<endl<<"\"HLT\" Command"<<endl;
          cout<<endl<<"HLT Ends program execution";
          cin.ignore(); exit(0);
          }

     write("End Execution  Values");
}

string::size_type position;
void split(string str,int pos)
{
     int i,j;
     firstPart = new char[pos+1];
     secondPart = new char[str.length()-pos];

     for(i=0;i<pos;i++){firstPart[i]=str[i]; }
     firstPart[i] = '\0';
     i++;

     for(j=0;i<str.length();j++,i++){ secondPart[j]=str[i]; }
     secondPart[j] = '\0';
     }

bool isInclude(string str, char c)
{
     for(int i=0;i<str.length();i++)
     {
             if(str[i] == c) return true;
             }
     return false;
     }

void startProgram()
{
     for(int i;i<4095;i++) ram[i]=0; //FFFF in hex. = 4095 in dec.

    //reading from file line by line
    string str;
    ifstream myfile ("codes.txt");

    cout<<"-------------------------"<<endl;
    cout<<"Executed Program"<<endl;
    cout<<"-------------------------"<<endl;

    if (myfile.is_open())
    {
       while (! myfile.eof() )
       {
             getline (myfile,str);
             cout<<str<<endl;
             str.append(1,' ');

             if(isInclude(str,' '))
             {
                           position = str.find(" ");
                           split(str,position);
                           int a;
                           hexToDec(secondPart,a);
                           if(a<0 || a>4095)
                           {
                                  cout<<endl<<"ERROR - Adresses can be between 0000 and FFFF!";
                                  cin.ignore();
                                  exit(0);
                                  }
             }
             else
             {
                 firstPart = new char[str.length()+1];
                 strcpy(firstPart, str.c_str());firstPart[str.length()+1]='\0';
                 }
             if(binToDec(firstPart)) {  _ram[countLine].oppCode = binToDec(firstPart);  }
             else
             {
                     if(strcmp(firstPart,"LOAD1")==0)
                     { _ram[countLine].oppCode = binToDec(LOAD1); }
                     else if(strcmp(firstPart,"LOAD2")==0)
                     { _ram[countLine].oppCode = binToDec(LOAD2); }
                     else if(strcmp(firstPart,"LOAD3")==0)
                     { _ram[countLine].oppCode = binToDec(LOAD3); }
                     else if(strcmp(firstPart,"LOAD4")==0)
                     { _ram[countLine].oppCode = binToDec(LOAD4); }
                     else if(strcmp(firstPart,"STOR")==0)
                     { _ram[countLine].oppCode = binToDec(STOR); }
                     else if(strcmp(firstPart,"LSH")==0)
                     { _ram[countLine].oppCode = binToDec(LSH); }
                     else if(strcmp(firstPart,"RSH")==0)
                     { _ram[countLine].oppCode = binToDec(RSH); }
                     else if(strcmp(firstPart,"ADD")==0)
                     { _ram[countLine].oppCode = binToDec(ADD); }
                     else if(strcmp(firstPart,"SUB")==0)
                     { _ram[countLine].oppCode = binToDec(SUB); }
                     else if(strcmp(firstPart,"IN")==0)
                     { _ram[countLine].oppCode = binToDec(IN); }
                     else if(strcmp(firstPart,"OUT")==0)
                     { _ram[countLine].oppCode = binToDec(OUT); }
                     else if(strcmp(firstPart,"HLT")==0)
                     { _ram[countLine].oppCode = binToDec(HLT); }
                     else { cout<<"ERROR Unsupported Command Given"<<endl; cin.ignore(); exit(0); }
                 }

             hexToDec(secondPart,_ram[countLine].addPart);
             _ram[countLine].value = (_ram[countLine].oppCode * 4096) + _ram[countLine].addPart;
             _ram[countLine].strValue = convert(((_ram[countLine].oppCode * 4096) + _ram[countLine].addPart),16);

             countLine++;
             }
       myfile.close();
       cout<<"-------------------------"<<endl;
    }
}//end startProgram()

int main()
{
    startProgram();
    cout<<"-------------------------"<<endl;
    cout<<"Program Codes In Memory"<<endl;
    cout<<"-------------------------"<<endl;
    showMemory();
    cout<<"-------------------------"<<endl;
    for(int i=0;i<countLine;i++)
    {
            PC = i;
            fetch();
            execute();
            }
    int i;
    cin>>i;
}





