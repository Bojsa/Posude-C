#include<stdio.h>
#include<stdlib.h>

typedef struct cvor_st{			//Clanovi reda
	char info;
	struct cvor_st *sledeci;
}CVOR;

FILE *safe_open(char *imeFajla, char *rezim, int num){
	FILE *pf;
	pf = fopen(imeFajla, rezim);
	if(pf == NULL) {
		printf("Nije moguce otvoriti datoteku.\n");
		exit(num);
	}
}

void ubaciURed(CVOR **pocetak, CVOR **kraj, int info){
	CVOR *novi = malloc(sizeof(CVOR));       		//Da NAGLASIM, ovde je novi zapravo pomocna (temp). [tako sam navikao]
		if(novi == NULL){
			printf("Nije moguce rezervisati memoriju.\n");
			exit(222);
		}
	novi->info=info;
	novi->sledeci = NULL;

	if(*pocetak == NULL && *kraj == NULL){
		*pocetak = *kraj = novi;
		return;
	}

	(*kraj)->sledeci = novi;
	*kraj = novi;
}

int daLiJeDete(int deteA, int deteB,int cupA , int cupB, int kapacitetA,int kapacitetB){

	// prima vrednosti obe case u iteraciji k i u k-1
	// vraca 1 ako se iz iteracije k moglo doci u k-1
	// vraca -1 ako nije moglo
    if(cupA == 0){
        //napunjeno A
        if(deteA == kapacitetA & deteB == cupB){
            return 1;
        }
    }

    if(cupB == 0){
       					 //napunjeno B
        if(deteB == kapacitetB & deteA == cupA){
            return 1;
        }
    }

    if(deteA == 0 && cupB == deteB){
       					 // ispraznjeno A
        return 1;
    }

    if(deteB == 0 && cupA == deteA){
      					  // ispraznjeo B
        return 1;
    }

   					 // presipamo A u B
    int mestauB = kapacitetB - cupB;
    if(cupA >= mestauB){
        if(cupA - mestauB == deteA && deteB == kapacitetB)
            return 1;
    }else{
        if(deteA == 0 && deteB == cupB + cupA)
            return 1;
    }

					// presipamo B u A
    int mestauA = kapacitetA - cupA;
    if(cupB >= mestauA){
        if(cupB - mestauA == deteB && deteA == kapacitetA)
            return 1;
    }else{
        if(deteB == 0 && deteA == cupA + cupB)
            return 1;
    }

    return -1;
}

void izbaciIzReda(CVOR **pocetak, CVOR **kraj){				//izbacuje clanove iz reda
	 if(*pocetak == NULL && *kraj == NULL)
		return;
	   CVOR *novi = *pocetak;
	   if( *pocetak == *kraj){
		*pocetak = *kraj = NULL;
	    }else{
		*pocetak = (*pocetak)->sledeci;
	    }
	    free(novi);
}

int vrati_velicinu_reda(CVOR *pocetak){
	int brojac = 0;
	    if(pocetak == NULL){
		printf("Red je prazan \n");
		return 0;
	    }
	    CVOR *novi = pocetak;
	    while(novi != NULL){
		brojac++;
		novi = novi->sledeci;;
	    }
	    return brojac;
}

/*void stampaj(CVOR * pocetak){				// ispisuje ceo red 
    int brojac = 0;					// ne koristi se moze se obrisati
    if(pocetak == NULL){
        printf("Red je prazan \n");
        return;
    }
    CVOR *novi = pocetak;
    printf("Red: \n");
    while(novi != NULL){
        printf("%d ", novi->info);
        novi = novi->sledeci;
    }
} */

int ProveriPrazno(CVOR *pocetak){			
    if(pocetak == NULL){				
        return 1;
    }else
        return -1;
}

int uzmi_prednji(CVOR **  pocetak){			
    if(*pocetak == NULL)
        return -1000;
    return (*pocetak)->info;
}

void trazi(int x,int y,int V,FILE *izlaz){
    	//scanf("%d %d %d",&x,&y,&V);
   
    int i,j;
    int korak[x+1][y+1];		
    for (i=0; i<=x; i++){
        for (j=0; j<=y; j++){
            korak[i][j] = -1;
        }
    }
    
    korak[x][y] = 0;
    int matricaCasaA[10000][4];		// koristice se za pracenje koraka
    int matricaCasaB[10000][4];
    for(i=0;i<10000;i++){		// inicijalizacija
        for(j=0;j<4;j++){
            matricaCasaA[i][j] = -1;
            matricaCasaB[i][j] = -1;
        }
    }
    CVOR * qxPocetak = NULL;		
    CVOR * qxKraj = NULL;

    CVOR * qyPocetak = NULL;		
    CVOR * qyKraj = NULL;

    ubaciURed(&qxPocetak,&qxKraj,x); ubaciURed(&qxPocetak,&qxKraj,0);
    ubaciURed(&qyPocetak,&qyKraj,0); ubaciURed(&qyPocetak,&qyKraj,y);


    int nadji = -1;
    j = 1;
    int kolona = 0;
    while(ProveriPrazno(qxPocetak) == -1 && nadji == -1){
        int duzina = vrati_velicinu_reda(qxPocetak);
        int vrsta=0;
        for (int i=0; i<duzina; i++){
            int cupA = uzmi_prednji(&qxPocetak); izbaciIzReda(&qxPocetak,&qxKraj);
            int cupB = uzmi_prednji(&qyPocetak); izbaciIzReda(&qyPocetak,&qyKraj);
            matricaCasaA[kolona][vrsta] = cupA;
            matricaCasaB[kolona][vrsta] = cupB;
            vrsta++;
            if (korak[cupA][cupB] != -1) continue;	
            else if (cupA==V || cupB==V){		// ukoliko se u nekoj casi nalazi
                nadji = 1;				// trazena kolicina -> kraj
                break;
            }
            else{
                korak[cupA][cupB] = j;

                // 4 uslova,slucaja,kombinacije
                if (cupA == 0){
                    // ako je casa A prazna punimo je
                    ubaciURed(&qxPocetak,&qxKraj,x);
                    ubaciURed(&qyPocetak,&qyKraj,cupB);

                    // presipamo casu B u casu A
                    if (cupB > x){
                        ubaciURed(&qxPocetak,&qxKraj,x); ubaciURed(&qyPocetak,&qyKraj,cupB - x);
                    }
                    else{
                        ubaciURed(&qxPocetak,&qxKraj,cupB);  ubaciURed(&qyPocetak,&qyKraj,0);
                    }
                }
                else if (cupB == 0){
                    // ako je casa B prazna punimo je
                    ubaciURed(&qxPocetak,&qxKraj,cupA); ubaciURed(&qyPocetak,&qyKraj,y);

                    // presipamo casu A u casu B
                    if (cupA > y){
                        ubaciURed(&qxPocetak,&qxKraj,cupA - y);
                        ubaciURed(&qyPocetak,&qyKraj,y);
                    }
                    else{
                        ubaciURed(&qxPocetak,&qxKraj,0);
                        ubaciURed(&qyPocetak,&qyKraj,cupA);
                    }
                }
                else if (cupA == x){
                    // praznim casu A
                    ubaciURed(&qxPocetak,&qxKraj,0); ubaciURed(&qyPocetak,&qyKraj,cupB);

                    if (cupA > (y - cupB)){
                    // ukoliko se ne moze presuti cela casa A u casu B
                        ubaciURed(&qxPocetak,&qxKraj,cupA-(y-cupB)); ubaciURed(&qyPocetak,&qyKraj,y);
                    }
                    else{
                    // ukoliko se cela casa A moze presuti u casu B
                        ubaciURed(&qxPocetak,&qxKraj,0); ubaciURed(&qyPocetak,&qyKraj,cupA + cupB);
                    }
                }
                else if (cupB == y){
                    // praznimo casu B
                    ubaciURed(&qxPocetak,&qxKraj,cupA); ubaciURed(&qyPocetak,&qyKraj,0);

                    if (cupB > (x - cupA)){
                        ubaciURed(&qxPocetak,&qxKraj,x);  ubaciURed(&qyPocetak,&qyKraj,cupB-(x-cupA));
                    }
                    else{
                        ubaciURed(&qxPocetak,&qxKraj,cupA + cupB); ubaciURed(&qyPocetak,&qyKraj,0);
                    }
                }
            }
        }
        j++;
        kolona++;
    }

    if (nadji == -1){
        fprintf(izlaz,"\nNije moguce doci do resenja");
    }
    int it;
    int brojac=0;
    int cuvamA,cuvamB;
    int resenjaA[10000];
    int resenjaB[10000];
    j = j-2;				
    for(i=0;i<4;i++){			// nalazimo rezultat
        if(matricaCasaA[j][i] == V || matricaCasaB[j][i] == V) {
            cuvamA = matricaCasaA[j][i];
            cuvamB = matricaCasaB[j][i];
            break;
        }
    }


    for(i=j;i>=0;i--){			 // krecemo od rezultata i u svakom koraku dobijamo
        for(it=0;it<4;it++){ 		 // prethodni par i cuvamo informacije
            if(daLiJeDete(cuvamA,cuvamB,matricaCasaA[i][it],matricaCasaB[i][it],x,y) == 1){
                cuvamA = matricaCasaA[i][it];
                cuvamB = matricaCasaB[i][it];
                resenjaA[brojac] = cuvamA;
                resenjaB[brojac] = cuvamB;
                brojac++;
                break;
            }
        }
    }
    if(nadji == 1){			// ispisujemo korake
       fprintf(izlaz,"\n");
        for(i=brojac-1;i>0;i--){
            fprintf(izlaz,"(%d,%d)->",resenjaA[i],resenjaB[i]);
        }
        fprintf(izlaz,"(%d,%d)",resenjaA[0],resenjaB[0]);
    }

}


int main(){
int x,y,z;

FILE *ulaz=safe_open("posude.txt", "r", 1);
FILE *izlaz=safe_open("operacije.txt", "w", 2);

while(fscanf(ulaz,"%d %d %d",&x,&y,&z) != EOF){
        trazi(x,y,z,izlaz);
    }

fclose(ulaz);
fclose(izlaz);
return 0;
}
