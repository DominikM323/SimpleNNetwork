#include <stdio.h>
#include <stdlib.h>
double exp(double x);

int liczba_danych = 3;
float oczekiwane_wyniki[][10] = {{1,0,0,0,0,0,0,0,0,0},{0,1,0,0,0,0,0,0,0,0},{0,0,1,0,0,0,0,0,0,0}};
float dane_wejsciowe[][25]={
    {0,1,1,1,0,//0
    0,1,0,1,0,
    0,1,0,1,0,
    0,1,0,1,0,
    0,1,1,1,0},
    {0,0,0,1,0//1
    ,0,0,1,1,0,
    0,0,0,1,0,
    0,0,0,1,0,
    0,0,0,1,0},
    {0,1,1,1,0//2
    ,0,0,0,1,0,
    0,1,1,1,0,
    0,1,0,0,0,
    0,1,1,1,0},

    };

float wagi_1[25][25];
float wagi_2[25][25];
float wagi_3[25][10];
//wagi dla polaczenia: od neuronu i, polaczenie j

int fAkt = 0; //wybor funkcji aktywacji 1-f.sigmoid.; nie 1 -f.liniowa ograniczona

float wejscie[25]=
{1,1,1,1,0,
 0,1,0,1,0,
 0,1,0,1,0,
 0,1,0,1,0,
 0,1,1,1,0};

float wejscie2[25]=
{0,1,1,1,0,
 0,0,0,1,0,
 0,1,1,1,0,
 1,1,0,0,0,
 0,1,1,1,0};
//2 zestawy danych wejsciowych : 'zaszumione' rysunki 0 i 2

float wyjscie_1[25];
float wyjscie_2[25];
float wyjscie_3[10];
//wart. na wyjsciu z kazdego neur. (po f. aktywacji)

float wspBledu_1[25];
float wspBledu_2[25];
float wspBledu_out[10];
//wsp bledu dla danych neur danych warstw

float wartWzbudz_1[25];
float wartWzbudz_2[25];
float wartWzbudz_out[10];
//wart wzbudzenia dla neur (wyniki z neur przed f aktywacji)

float f1(float x,int wybor){
    //f. aktywacji z wyborem: 1= f. sigmoid, !1= f.liniowa ograniczona.
    if(wybor == 1){
        //f. sigmoidalna unipolarna
        return 1.0/(1.0+exp((-5*x)+5));

    }else{
        //f.liniowa ograniczona
        if(x>1)return 1;
        if(x<0)return 0;
        else return x;
    }
}

float df1(float x,int wybor){
    //pochodna f. aktywacji z wyborem
    if(wybor == 1){
        return 5*f1(x,1)*(1-(f1(x,1)));//pochodna f. sigmoidalnej
    }else return 1.0; //pochodna funkcji liniowej ogr.
}

void init_w(int out_size,float a[25][out_size]){
    //inicjuje losowe wagi
    for(int i =0;i<25;i++){
        for(int j =0;j<out_size;j++){
            a[i][j]=(float)rand()/RAND_MAX+0.0001;
            if((float)rand()/RAND_MAX>0.5) a[i][j]*=-1;
        }
    }
}

void warstwa(int out_size,float in[25],float wagi[25][out_size],float s[out_size],float out[]){
    //propagacja przez 1 warstwe
    for(int i =0;i<out_size;i++){
        s[i]=0;
        out[i]=0;
        for(int j =0;j<out_size;j++){
            s[i]+= (in[j])*(wagi[j][i]);
        }
        out[i]=f1(s[i],fAkt);
    }
}

void propWstecznaWyjscia(float s[10],float oczekiwany[10],float wynik[10],float wspBledu[10]){
    //obl bledow neuronow wyjscia
    for(int i =0;i<10;i++){
        wspBledu[i]= 0.1*(oczekiwany[i]-wynik[i]);
        printf("\nwsp bl: %f\n",wspBledu[i]);
       // printf("\n%f",s[i]);
    }
}

void propWstecznaWarstwy(int out_size,float s[25],float wagi[25][out_size],float bledy_dalszych[out_size],float wspBledu[25]){
    //obliczenie bledow neuronow 1 warstwy
    for(int i =0;i<25;i++){ //dla kazdego neur obecnej warstwy
        float suma = 0;
        for(int j=0;j<out_size;j++){ //dla kazdego neur z nast warstwy
            suma+=wagi[i][j]*bledy_dalszych[j];
        }
        wspBledu[i]=0.1*suma;
    }
}

void poprawka(float wagi_1[25][25],float wspBledu_1[25],float wagi_2[25][25],float wspBledu_2[25],float wagi_3[25][10],float wspBledu_out[10],float w1[25],float w2[25],float w3[25]){
    //zmiana wag
    for(int i =0;i<25;i++){
        for(int j =0;j<25;j++){
            wagi_1[i][j]+=df1(w1[i],fAkt)*wspBledu_1[j]*(w1[j]+0.1);
            wagi_2[i][j]+=df1(w2[i],fAkt)*wspBledu_2[j]*(w2[j]+0.1);
        }
    }
     for(int i =0;i<25;i++){
        for(int j =0;j<10;j++){
            wagi_3[i][j]+=df1(w3[i],fAkt)*wspBledu_out[j]*(w3[j]+0.1);
           // printf("\nw3%i: %f",j,w3[j]);
        }
    }
}

void znajdzPewnosc(float pewnosci[10]){
    //interpretacja wyniku
    int maxInd = -1;
    float maxPewn = -100;
    for(int i=0;i<10;i++){
        if(pewnosci[i]>maxPewn){
            maxInd = i;
            maxPewn=pewnosci[i];
        }
    }
    printf("\n mysle ze to %i z pewnoscia %f\n",maxInd,maxPewn);
}

void ucz(
// 1 iteracja uczenia
         float wagi_1[25][25],
         float wspBledu_1[25],
         float wagi_2[25][25],
         float wspBledu_2[25],
         float wagi_3[25][10],
         float wspBledu_out[10],
         float w1[25],
         float w2[25],
         float w3[25],
         float dane_w[][25],
         float wyniki_oczek[][10],
         float s1[25],
         float s2[25],
         float s3[10],
         int liczba_danych){

    for(int i =0;i<liczba_danych;i++){
        warstwa(25,dane_w[i],wagi_1,s1,wyjscie_1);
        warstwa(25,wyjscie_1,wagi_2,s2,wyjscie_2);
        warstwa(10,wyjscie_2,wagi_3,s3,wyjscie_3);

        propWstecznaWyjscia(s3,wyniki_oczek[i],wyjscie_3,wspBledu_out);
        propWstecznaWarstwy(10,s2,wagi_3,wspBledu_out,wspBledu_2);
        propWstecznaWarstwy(25,s1,wagi_2,wspBledu_2,wspBledu_1);
        poprawka(wagi_1,wspBledu_1,wagi_2,wspBledu_2,wagi_3,wspBledu_out,dane_w[i],wyjscie_1,wyjscie_2);
        for(int j =0;j<10;j++){
            printf(" %f",wyjscie_3[j]);
        }
    }
}

int main()
{
    //ustaw losowe wagi
    init_w(25,wagi_1);
    init_w(25,wagi_2);
    init_w(10,wagi_3);

    //uczenie z i iteracji
    for(int i = 0; i< 800;i++){
        ucz(wagi_1,wspBledu_1,wagi_2,wspBledu_2,wagi_3,wspBledu_out,wyjscie_1,wyjscie_2,wyjscie_3,
            dane_wejsciowe,oczekiwane_wyniki,wartWzbudz_1,wartWzbudz_2,wartWzbudz_out,liczba_danych);
    }
    printf("\nkoniec uczenia\n");
    printf("\n----------------\n");

    //test 1 dane: rysunek 0 z 1 niepoprawnym bitem
    warstwa(25,wejscie,wagi_1,wartWzbudz_1,wyjscie_1);
    warstwa(25,wyjscie_1,wagi_2,wartWzbudz_2,wyjscie_2);
    warstwa(10,wyjscie_2,wagi_3,wartWzbudz_out,wyjscie_3);

    printf("\n\n----------------\n");
    printf("test 1 dane: rys. 0 z 1 niepoprawnym bitem\n");
    for(int i =0;i<10;i++){
        printf(" %f",wyjscie_3[i]);
    }
    znajdzPewnosc(wyjscie_3);

    //test 2 dane: rysunek 2 z 1 niepoprawnym bitem
    warstwa(25,wejscie2,wagi_1,wartWzbudz_1,wyjscie_1);
    warstwa(25,wyjscie_1,wagi_2,wartWzbudz_2,wyjscie_2);
    warstwa(10,wyjscie_2,wagi_3,wartWzbudz_out,wyjscie_3);

    printf("\n\n----------------\n");
    printf("test 2 dane: rys. 2 z 1 niepoprawnym bitem\n");
    for(int i =0;i<10;i++){
        printf(" %f",wyjscie_3[i]);
    }
    znajdzPewnosc(wyjscie_3);
    return 0;
}
