#include "main.h"

//citim din fisierul out.txt
char* read_file (char* my_file, long* length)
{
	printf("Reading the file \n \n");
	FILE* file = fopen (my_file, "r");

	char* my_string = NULL;
	*length = 0;
	int char_read = 0;
	int offset = 0;

	if (file != NULL)
	{
		fseek (file, 0, SEEK_END);
		*length = ftell (file);
		rewind(file);

		printf("Fiserul are %li caractere\n", *length);

		my_string = (char*) malloc(*length);

		while (!feof(file))
		{
			char_read = fread(&my_string[offset], sizeof(char), 100, file);
			offset += char_read;
		}
		fclose (file);
	}
	return my_string;
}
// calculam indexul de coincidenta pentru un string
double compute_coincidence (int *tab, int s_length)
{
	// suma frecventelor fiecarei litere din alfabet
	int k;
	double sum = -1;
	for (k=0; k < ALPHABET_SIZE ; k++)
	{
		sum += tab[k]*(tab[k]-1);
	}

	return sum / ((s_length-1)*s_length);
}
// calculam MIC pentru doua stringuri de distanta G
double compute_mutual_coincidence(int *tab, int *tab2, int s_length, int g)
{
	// suma freceventelor pentru fiecare litera din alfabet cu distanta g
	double sum = -1;
	int k;
	for (k=0; k < ALPHABET_SIZE; k++)
	{
		sum += tab[k]*tab2[(k+g)%26];
	}
	return sum / (s_length*s_length);
}
//decriptare
int decrypt (char* my_string, int length, char* key, int key_length)
{
	int i;
	for (i=0; i < length; i ++)
	{
		int caracterCurent = 	(my_string[i]-ASCII_MAJ_OFFSET)-(key[i%key_length]-ASCII_MAJ_OFFSET);
		if (caracterCurent < 0)
		{
			caracterCurent +=26;
		}
		printf ("%c" ,caracterCurent  + ASCII_MIN_OFFSET);
	}
	printf ("\n");

	return EXIT_SUCCESS;
}
//cautam maximul din vector si returnam pozitia acestuia
int find_index_max_tab (int* tab, int tab_size)
{
	int max = -1;
	int ret = -1;
	int i;
	for (i=0; i < tab_size; i++)
	{
		if (tab[i]> max)
		{
			max = tab[i];
			ret = i;
		}
	}

	return ret;
}
//afisam cheia
int print_key (char* key, int key_length)
{
	printf("cheia este : ");

	int i;
	for (i=0; i < key_length; i++)
	{
		printf ("%c", key[i]);
	}

	printf("\n \n");

	return EXIT_SUCCESS;
}
//metoda de resetare de tabele , la valoarea 0
int rez_tab (int *tab, int tab_length)
{
	int i;
	for (i =0; i < tab_length; i++)
	{
		tab[i]=0;
	}

	return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
	char* my_string; 

	int** frequencies; 

	long length;

	char* myTest;

	if (argc >1)
	{
		my_string = read_file(argv[1], &length);
	}
	else
	{
		my_string = read_file ("out.txt", &length);
	}
//tratare eroare pentru cazul in care fisierul este gol, sau nu este dat prin niciun parametru
	if(length == 0)
	{
		printf ("Fisierul introdus este gol, sau nu ati dat niciun parametru pentru functie\nIncercati ./decriptare fisier");
		return EXIT_FAILURE;
	}


	frequencies = (int**) malloc (sizeof(int*)*length);

	frequencies [0] = (int*) malloc(sizeof(int)*ALPHABET_SIZE); //alocam memorie pentru tab[0]

	//calculam IC pentru toate subsecventele
	int m = 1;
	double coincidence = 0.0;
	while (coincidence < ENGLISH_COINCIDENCE && m < length)
	{
		frequencies [m] = (int*) malloc(sizeof(int)*ALPHABET_SIZE); //alocam memorie pentru tab[m]

		int i;
		for (i = 0; i < m; i++)
			 {
				rez_tab(frequencies[i], ALPHABET_SIZE); //resetam tabela, pana la lungimea cheii
			 }
		int j;
		for (j = 0; j < length; j++)
		{
			int index = my_string[j]-ASCII_MAJ_OFFSET;
			if ( index < ALPHABET_SIZE && index >=0)
			{
				frequencies[j%(m)] [index]+=1; //pentru fiecare litera din alfabet gasita in string facem o tabela de vizitare si incrementam
			}
		}
		//calculam IC
		coincidence = 0.0;
		for (j = 0; j < m; j++)
		{
			coincidence += compute_coincidence(frequencies[j%m], length/m) / m;
		}

		m++;
	}
	m--; //scadem o pozitie din m, pentru ca cheia m+1 a dat eroare, asadar lungimea m este cea corecta

	printf("Lungimea cheii este de %i caractere cu o coincidenta de %f. \n \n", m, coincidence);
	//o noua strucata de date de retine distantele dintre prima pozitie a cheii si pozitia i
	int* offsets = (int*) malloc(sizeof(int)*m);

	int i;
	//calculam MIC dintre primul subsir si celelate
	for (i=0; i< m; i++)
	{
		double mutual_coincidence = 0;
		int g =0;
		while (mutual_coincidence < ENGLISH_COINCIDENCE && g < ALPHABET_SIZE)
		{

			mutual_coincidence = compute_mutual_coincidence (frequencies[0], frequencies[i], length/m, g);
			g++;
		}
		g--; // g+1 a dat eroare, asadar g este distanta cea corecta

		offsets[i] = g;
	}

	char* key = (char*) malloc(sizeof(char)*m);
	//prima litera din cheie este litera care se gaseste cel mai frecventa in prima subsecventa
	key [0] = find_index_max_tab(frequencies[0], ALPHABET_SIZE)- MOST_USE_LETTER + ASCII_MAJ_OFFSET;

	//am gasit prima litere si avem distanta dintre ea si celelalte, calculam restul cheii
	for (i = 1; i < m; i++)
	{
		key[i] = ((key[0] - ASCII_MAJ_OFFSET)+ offsets[i]) %ALPHABET_SIZE + ASCII_MAJ_OFFSET;
	}

	print_key (key, m);

	printf ("Decriptare : \n");

	decrypt (my_string, length, key, m);

	for (i =0; i< m; i++)
	{
		free(frequencies[i]);
	}
	free(frequencies);
	free(key);
	free(offsets);
	free (my_string);


	return EXIT_SUCCESS;
}
