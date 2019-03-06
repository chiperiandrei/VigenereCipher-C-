#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <algorithm>
#include <math.h>
#include <stdlib.h>
#include <string.h>
char cheie[256];

using namespace std;

int main(int argc, char* argv[])  {
  if (argc<1)
  {
    printf("Trebuie sa introduceti cheia pentru criptare!\n");
  }
  else
    strcpy(cheie,argv[1]);
  string line;
  ifstream myfile ;
  myfile.open("test.txt");
  ofstream afara ("out.txt");
  char str[8123];
  for(int i=0;i<strlen(str);i++)
    str[i]='\0';
  if (myfile.is_open())
  {
    while ( myfile.good() )
    {
      getline (myfile,line);

      for (int i = 0; i < line.size(); i++) {
        if (line[i] < 'A' || line[i] > 'Z' &&
            line[i] < 'a' || line[i] > 'z')
        {
            line.erase(i, 1);
            i--;
        }
    }
      std::transform(line.begin(), line.end(),line.begin(), ::toupper);
      strcat(str,line.c_str());
    }
    myfile.close();
  }

  else cout << "Unable to open file";
    char criptat[9999];
    int msgLen=strlen(str), lenCheie=strlen(cheie),i,j;


    int k=0;
    for(i=0;i<msgLen;++i){
        criptat[i]=( (str[i] + cheie[k] ) %26 ) + 'A';
        k++;
        if(k==lenCheie)
            k=0;
    }
    criptat[msgLen]='\0';
    afara<<criptat<<endl;

  return 0;
}
