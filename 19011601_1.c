#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX 8 // girilebilecek maksimum N sayisi
#define MIN 3 // girilebilecek minimum N sayisi
 
int N;
int * shift_num; // her bir satir icin kaydirma miktarini tutacak dizi
char selectedColors[MAX][10]; // kullanicinin N sayisina gore sececegi renkler
char Color_List[MAX][10] = {"kirmizi", "turuncu", "sari", "mavi", "yesil", "lacivert", "mor", "siyah"};

/*
düzenlenen her bir satir icin ekrana
once rengin karsiligi olan sayi
daha sonra renk yazdirilir
secilen renklerin icerisinde
bulunma siralarina gore yazdirilir
*/
void printGrid(int grid[N][N])
{
	printf("\n");
	int row, col;
    for (row = 0; row < N; row++) {
        for (col = 0; col < N; col++)
            printf("%2d", grid[row][col]);
        printf("---");
        for(col = 0; col < N; col++)
        	printf("%-10s", selectedColors[grid[row][col]]);
        printf("\n");
    }
}

/*
matrisin sonucu bulunduysa
renk matrisini yazdiran fonksiyon
*/
void printColorGrid(int grid[N][N])
{
	int row, col;
    for (row = 0; row < N; row++) {
        for (col = 0; col < N; col++)
            printf("%-10s", selectedColors[grid[row][col]]);
        printf("\n");
    }
}

/*
matriste parametre olarak gonderilen
satiri bir saga kaydiracak fonksiyon
*/
void shift(int row[])
{
	int temp = row[N-1], i; // satirdaki son eleman shiftten sonra basa gelecegi icin tutuluyor
    for(i = N-1; i > 0; i--)
    {
        row[i] = row[i-1]; // her bir deger saga kayar
    }
    row[0] = temp; // en son satir basina tutulan temp yerlestirilir
}

/*
flag : satirda bir kaydirma olduysa flag = 1 yapilir.
herhangi bir kaydirma olmadan da ilgili sutundaki degerler
birbirine esit olmayabilir, flag = 0 durumunda kalir
parametre olarak gonderilen ilgili satir icin sutunlarda
tekrar olup olmadigi kontrol edilir.
eger kaydirma N'e ulastiysa ve sonuc yoksa 0
kaydirmadan sonra ilgili sutunda tekrar olmadiysa 1
*/
int flag = 0;
int isDuplicated(int grid[N][N], int row, int col)
{
	int x;
	for (x = row-1; x >= 0; x--){ // gonderilen satirin bir ust degerinden itibaren dongu
		if (grid[row][col] == grid[x][col]){ // esit ise kaydirmaya gidilecek
        	if(shift_num[row] == N){ // ilgili satir icin kaydirma sayisi N'e esitse
        		return 0; // kaydirma ise yaramadi demektir, ust satirda kaydirma yapmak uzere 0 dondurulur.
			}
			flag = 1; 
        	shift(grid[row]); // kaydirma
        	shift_num[row]++; // kaydirilan satirin kaydirilma miktari bir arttirildi
        	return isDuplicated(grid, row, col); // sutunda farklilik saglandi mi, tekrar bakiliyor.
        }
	}
    return 1;
}

/*
flag_2 : bir ust satirda kaydirma yapildiysa flag_2 = 1 yapilir.
bu flag bir ust satirda kaydirma yapildiktan sonra
yine o satirin ustleriyle kontrolu saglanacagi icin,
donguden cikip main icinde satir satir gezilen while dongusunde
sayinin arttirilmadan tekrardan kontrol edilmesi icin kullanilir.
eger flag_2 kullanilmadiysa dongu aynen devam eder.
satira renkleri yerlestirme islemi basarili ise 1
sutunlarda tekrar bulunmasi durumunda 0, sonuc yok
*/
int flag_2 = 0;		
int place_color(int grid[N][N], int curr_row, int curr_col)
{
	int row;
	for(row = (N-1); row >= 0; row--){ // bir satir icin tum sutunlari gezmek icin indis en yuksek N-1
		if(curr_col < 0){ // sutun 0 da kontrol edildiyse
			if(shift_num[curr_row] == N) // ve kaydirma ile ayni yere gelindiyse
				return 0; // cozum yok
			return 1; // cozum var, diger sutunlara devam
		}
		if(isDuplicated(grid, curr_row, curr_col) == 0){ // ilgili satir icin sutunda tekrar varsa ve kaydirma ile asilamadiysa
			shift(grid[curr_row - 1]); // bir ust satir kaydirilir
			shift_num[curr_row - 1]++; // bir ust satirin kaydirma miktari arttilir.
			shift_num[curr_row] = 0; // asil satirda yeni siralama ile kaydirmaya 0dan baslanir.
			flag_2 = 1; // ust satirda kaydirma yapildi.
			return place_color(grid, curr_row-1, N-1);
		}
		else if(flag == 1){ // eger icinde bulundugumuz sutun icin ilgili satir kaydiysa
			flag = 0; // tum sutunlari tekrar var mi kontrol etmek amaciyla, sutun = N-1 
			return place_color(grid, curr_row, N-1);
		}
		return place_color(grid, curr_row, curr_col-1); // sutun basarili, bir soldaki sutuna geciliyor
	}
}

/*
kullanicinin girdigi renklerin 8 rengin arasindan
sectigi renklerin icerisinde olup olmadigini
kontrol eden fonksiyon
*/
int is_in_colors(char c[10])
{
	int i;
	for(i = 0; i<MAX; i++){
		if(strcmp(selectedColors[i], c) == 0){
			return i;
		}
	}
	return -1;
}

/*
kullanicinin girdigi renk satirlarinda
renklerin tekrar edip etmemesini kontrol eden fonksiyon
*/
int k = 0;
bool is_duplicated_in_row(char color_row[N][10], int k)
{
	if(k == N-1){
		return false;
	}
	int i;
	for(i = k+1; i<N; i++){
		if(strcmp(color_row[k],color_row[i]) == 0){
			return true;
		}
	}
	k++;
	return is_duplicated_in_row(color_row, k);
}

/*
main icerisinde N'e gore olusturulan dongude
satir satir renkleri alan
icerisinde gerekli kontrolleri yapan fonksiyon
bir sorun yoksa renk satiri matrise eklenir.
*/
bool fill_row(int row, char color_row[N][10], int index_row[N])
{
	int j, r = 0;
	for(j = 0; j<N; j++){
		char c[10];
		printf(" Enter the color for [%d][%d] : ", row+1, j+1);
		scanf("%s",c);
		int index = is_in_colors(c);
		if(index == -1){ // renk secilen renklerde varsa
			return false;
		}
		strcpy(color_row[j], c);
		index_row[r] = index;
		r++;
	}
	return !is_duplicated_in_row(color_row, k); // tekrar yoksa true
}

int main()
{
	int i, j;
	char is_num;
	printf("----------------------------------\n");
	printf(" Enter the dimension of color matrix : ");
	if(scanf("%d", &N) != 1){
		printf(" Failed--entered non numeric value..");
		exit(3);
	}
	while(N > MAX || N < MIN){
		printf(" Please enter a number in range [3-8] : ");
		scanf("%d", &N);
	}
	printf("\n");
	for(i = 0; i < MAX; i++){
		printf(" %s ", Color_List[i]);
	}
	printf("\n");
	
	// kullanicinin sectigi renkler
	for(i = 0; i < N; i++){
		printf(" Select the %d. color for matrix : ", i+1);
		scanf("%s", selectedColors[i]);
	}
	
	printf("\n");
	char color_row[N][10]; // renk satir 
	char color_grid[N][N][10]; // renk matrisi
	int grid[N][N]; // renk -> sayi matrisi
	shift_num = (int*)calloc(N, sizeof(int));
	
	for(i = 0; i<N; i++){
		int index_row[N];
		if(fill_row(i, color_row, index_row)){
			memcpy(color_grid[i], color_row, sizeof(color_row));
			memcpy(grid[i], index_row, sizeof(index_row));
			k = 0;
		}
		else{ // her satir icin sutun degerleri unique ve renkler secilen renklerin icinde olmali
			printf(" Failed--entered no unique numbers for each row");
			printf("\n or color that not in selected colors..");
			exit(3);
		}
	}
	
	i = 1;
	int solution = 1;
	while(i < N){
		if(place_color(grid, i, N-1) == 1){
			if(flag_2 == 1){
				flag_2 = 0; // bir ust satira donulup kaydirma yapilmis
				i--; // tekrar ayni satira donulerek kontrol edilmeli
			}
			else{
				printf("\n Place %d. row \n", i);
				printGrid(grid); // satir yerlesmis, yazdirilir.
				i++;
			}
		}
		else{
			printf("\nNo solution found.");
			solution = 0;
			break;
		}
	}	
	
	if(solution == 1){
		printf("\n ------Solution------ \n\n");
		printColorGrid(grid);
	}
	
	return 0;
}
