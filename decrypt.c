#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 100

int main(int argc, char **argv)
{
    char refTextName[SIZE];
    char cipTextName[SIZE];
    char plnTextName[SIZE];

    int i = 0;
    int ispGiven = 0;

    for (i = 1; i < argc; i++){
        if(argv[i][0] == '-'){
            switch(argv[i][1]){
                case 'r':
                    strcpy(refTextName, argv[i+1]);
                    ++i;
                break;
                case 'c':
                    strcpy(cipTextName, argv[i+1]);
                    ++i;
                break;
                case 'p':
                    strcpy(plnTextName, argv[i+1]);
                    ++i;
                    ispGiven = 1;
                break;

                default:
                    printf("Desteklenmeyen opsiyon!");
                    exit(1);
            }
        }
        else{
            printf("Desteklenmeyen opsiyon!");
            exit(1);
        }
    }
    
    if(!ispGiven) strcpy(plnTextName, "defaultplaintext.txt");
    
    FILE * refText;
    refText = fopen(refTextName, "r");
    
    FILE * cipText;
    cipText = fopen(cipTextName, "r");
    
    if(refText == NULL || cipText == NULL){
        if(refText == NULL) printf("%s dosyasi bulunamadi.\n", refTextName);
        if(cipText == NULL) printf("%s dosyasi bulunamadi.\n", cipTextName);
        exit(1);
    }
    
    const int numOfLetters = 'z' - 'a' + 1;
    
    int lettersRef[numOfLetters] = {0};
    int lettersCip[numOfLetters] = {0};
    
    char ch;
    
    while (fscanf(refText, " %c", &ch) == 1){
        if (feof(refText)) break;
        
        if(ch>='A' && ch<='Z')
            ch = ch + 'a' - 'A';
        
        if(ch>='a' && ch<='z')
            lettersRef[ch - 'a']++;
    }
    
    while (fscanf(cipText, " %c", &ch) == 1){
        if (feof(cipText)) break;
        
        if(ch>='A' && ch<='Z')
            ch = ch + 'a' - 'A';
        
        if(ch>='a' && ch<='z')
            lettersCip[ch - 'a']++;
    }
    
    int maxRef = lettersRef[0];
    int freqRefChar = 0;
    int maxCip = lettersCip[0];
    int freqCipChar = 0;
    
    int minRef = lettersRef[0];
    int rareRefChar = 0;
    int minCip = lettersCip[0];
    int rareCipChar = 0;
    
    for(i=0; i<numOfLetters; i++){
        if(lettersRef[i] > maxRef){
            maxRef = lettersRef[i];
            freqRefChar = i;
        }
        if(lettersCip[i] > maxCip){
            maxCip = lettersCip[i];
            freqCipChar = i;
        }
        if(lettersRef[i] < minRef){
            minRef = lettersRef[i];
            rareRefChar = i;
        }
        if(lettersCip[i] < minCip){
            minCip = lettersCip[i];
            rareCipChar = i;
        }
    }
    
    int freqShiftAmount = freqCipChar - freqRefChar;
    int rareShiftAmount = rareCipChar - rareRefChar;
    
    if(freqShiftAmount < 0) freqShiftAmount += numOfLetters;
    if(rareShiftAmount < 0) rareShiftAmount += numOfLetters;
    
    FILE * plnText0;
    FILE * plnText1;
    
    char plnTextName0[SIZE];
    char plnTextName1[SIZE];
    
    if(freqShiftAmount == rareShiftAmount){
        plnText0 = fopen(plnTextName, "w");
        
        printf("Kayirma miktari %d olarak tespit edilmistir.\n", freqShiftAmount);
        printf("Cozumlenmis metin %s dosyasina yazilmistir.\n", plnTextName);
    }
    else{
        strcpy(plnTextName0, plnTextName);
        strcpy(plnTextName1, plnTextName);
        
        if(strlen(plnTextName) >= 4){
            plnTextName0[strlen(plnTextName0) - 4] = '\0';
            plnTextName1[strlen(plnTextName1) - 4] = '\0';
        }
        
        plnText0 = fopen(strcat(plnTextName0, "_0.txt"), "w");
        plnText1 = fopen(strcat(plnTextName1, "_1.txt"), "w");
        
        printf("Kayirma miktari %d (en sik harf) ve %d (en seyrek harf) olarak tespit edilmistir.\n", freqShiftAmount, rareShiftAmount);
        printf("Cozumlenmis metin %s ve %s dosyalarina yazilmistir.\n", plnTextName0, plnTextName1);
    }
    
    rewind(cipText);
    
    while (fscanf(cipText, " %c", &ch) == 1){
        if (feof(cipText)) break;
        
        if(ch>='A' && ch<='Z')
            ch = ch + 'a' - 'A';
        
        if(ch>='a' && ch<='z'){
            if (freqShiftAmount == rareShiftAmount){
                fprintf(plnText0, "%c",
                            ( (ch - freqShiftAmount) >= 'a' && (ch - freqShiftAmount) <= 'z' ) ?
                                (ch - freqShiftAmount) :
                                ( (ch - freqShiftAmount) <= 'z' ?
                                    (ch - freqShiftAmount + numOfLetters) : (ch - freqShiftAmount - numOfLetters)
                                 )
                        );
            }
            else{
                fprintf(plnText0, "%c",
                            ( (ch - freqShiftAmount) >= 'a' && (ch - freqShiftAmount) <= 'z' ) ?
                                (ch - freqShiftAmount) :
                                ( (ch - freqShiftAmount) <= 'z' ?
                                    (ch - freqShiftAmount + numOfLetters) : (ch - freqShiftAmount - numOfLetters)
                                 )
                        );
                fprintf(plnText1, "%c",
                            ( (ch - rareShiftAmount) >= 'a' && (ch - rareShiftAmount) <= 'z' ) ?
                                (ch - rareShiftAmount) :
                                ( (ch - rareShiftAmount) <= 'z' ?
                                    (ch - rareShiftAmount + numOfLetters) : (ch - rareShiftAmount - numOfLetters)
                                 )
                        );
            }
        }
    }
    
    fclose(refText);
    fclose(cipText);
    fclose(plnText0);
    if (freqShiftAmount != rareShiftAmount)
        fclose(plnText1);
    
    return 0;
}

