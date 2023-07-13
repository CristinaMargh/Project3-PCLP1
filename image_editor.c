// Copyright Cristina-Andreea Margheanu 313CAb 2022-2023
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h> // functiile de memorie
#define LENGHT 3
#define COMMENT_L 100
#define MAX 255

typedef struct {
	int **matrix;
	int x1, x2, y1, y2;
	int image_type;
	int rows, columns;
} pic;

int no_loaded(pic *image)
{
	if (!image->matrix) {
		printf("No image loaded\n");
		return 1;
	}
	return 0;
}

// functie pentru alocarea memoriei intr-o matrice

int **alloc_matrix(int row, int column)
{
	int **a;
	// alocam memorie pentru vectorul de pointeri(de linii)
	a = malloc(row * sizeof(int *));
	if (a == 0) {
		fprintf(stderr, "malloc failed\n");
		return NULL;
	}
	for (int i = 0; i < row; i++) {
		*(a + i) = malloc(column * sizeof(int));
		if (a[i] == 0) {
			fprintf(stderr, "malloc failed\n");
			while (--i >= 0)
				free(a[i]);
			free(a);
			return NULL;
		}
}

return a;
}

int check_memory(int **a)
{
	// verificarea programarii defensive
	if (!a) {
		fprintf(stderr, "malloc failed\n");
		return 0;
	}
	return 1;
}

// functie pentru eliberarea memoriei

void free_matrix(int row, int **a)
{
	for (int i = 0; i < row; i++)
		free(a[i]);
	free(a);
}

void swap(int *x, int *y)
{
	// interschimbarea a 2 valori folosita ulterior
	// pentru setarea corecta a coordonatelor in cadrul selectiei
	int aux;
	aux = *x;
	*x = *y;
	*y = aux;
}

double clamp(double the_min, double the_max, double x)
{
	// functie ce ne va limita valoarea lui x intr-un interval
	// cuprins intre o valoare minima si una maxima
	if (x > the_max)
		x = the_max;
	if (x < the_min)
		x = the_min;
	return x;
}

int find_image_type(char *word)
{
	// aflam tipul imaginii in functie de cuvantul introdus
	int type;
	if (strstr(word, "P1") == word)
		type = 1;
	if (strstr(word, "P2") == word)
		type = 2;
	if (strstr(word, "P3") == word)
		type = 3;
	if (strstr(word, "P4") == word)
		type = 4;
	if (strstr(word, "P5") == word)
		type = 5;
	if (strstr(word, "P6") == word)
		type = 6;
	return type;
}

void read(FILE *in, pic *image, char *word)
{
int row = image->rows;
int column = image->columns;

int **matr;
unsigned char b;
int type = find_image_type(word);
// impartirea imaginilor in cele 2 categorii
if (type == 2 || type == 5)
	image->image_type = 1;
if (type == 3 || type == 6)
	image->image_type = 2;

if (image->image_type == 1)
	matr = alloc_matrix(row, column);
else
	matr = alloc_matrix(row, 3 * column);

if (type == 2)
	for (int i = 0; i < row; i++)
		for (int j = 0; j < column; j++)
			fscanf(in, "%d", &matr[i][j]);
// binar
if (type == 5)
	for (int i = 0; i < row; i++)
		for (int j = 0; j < column; j++) {
			fread(&b, sizeof(unsigned char), 1, in);
		matr[i][j] = (int)b;
}

if (type == 3)
	for (int i = 0; i < row; i++)
		for (int j = 0; j < 3 * column; j++)
			fscanf(in, "%d", &matr[i][j]);
//binar
if (type == 6)
	for (int i = 0; i < row; i++)
		for (int j = 0; j < 3 * column; j++) {
			fread(&b, sizeof(unsigned char), 1, in);
		matr[i][j] = (int)b;
}

image->matrix = matr;
}

void full_selection(pic *image)
{	if (no_loaded(image)) //==1
		return;
	int row = image->rows;
	int column = image->columns;
	image->x1 = 0;
	image->y1 = 0;
	image->y2 = row;
	image->x2 = column;
	printf("Selected ALL\n");
}

void selection(pic *image)
{
	char *line = malloc(50 * sizeof(char));
	fgets(line, 50, stdin);
	char *all = malloc(10 * sizeof(char));
	// pentru SELECT ALL
	int ret_code = sscanf(line, "%s", all);
	if (strcmp(all, "ALL") == 0) {
		full_selection(image);
		free(line);
		free(all);
		return;
	}
	int a, b, c, d;
	// cazul de eroare in care coordonatele nu sunt 4 numere
	ret_code = sscanf(line, "%d%d%d%d", &a, &b, &c, &d);
	if (ret_code != 4) {
		printf("Invalid command\n");
		free(line);
		free(all);
		return;
	}
	free(line);
	free(all);
	if (no_loaded(image)) //==1
		return;
	int row = image->rows;
	int column = image->columns;
	// verificarea datelor introduse
	if (a > c)
		swap(&a, &c);
	if (b > d)
		swap(&b, &d);
	int verify = 1;
	if (a < 0 || b < 0 || c < 0 || d < 0)
		verify = 0;
	if (a > column || c > column)
		verify = 0;
	if (b > row || d > row)
		verify = 0;
	if (a == c || b == d)
		verify = 0;
	if (verify == 0) {
		printf("Invalid set of coordinates\n");
	} else {
		// atribuirea valorilor
		image->x1 = a;
		image->x2 = c;
		image->y1 = b;
		image->y2 = d;
		printf("Selected %d %d %d %d\n", a, b, c, d); }
}

void save(pic *image)
{
	int row = image->rows;
	int column = image->columns;
	if (no_loaded(image)) {
		while (getchar() != '\n')
			;
		return;
	}
	int type = image->image_type;
	char *input = malloc(50 * sizeof(char));
	fgets(input, 50, stdin);
	char *filename = malloc(30 * sizeof(char));
	char *ascii = malloc(7 * sizeof(char));
	int ret_code = sscanf(input, "%s%s", filename, ascii);
	int **matr = image->matrix;
	FILE *wr;
	wr = fopen(filename, "w");
	// am citit ascii
	if (ret_code == 2) {
		if (type == 1) {
			// imagine gri ascii
			fprintf(wr, "P2\n");
			fprintf(wr, "%d %d\n", column, row);
			fprintf(wr, "%d\n", MAX);
			for (int i = 0; i < row; i++) {
				for (int j = 0; j < column; j++)
					fprintf(wr, "%d ", matr[i][j]);
				fprintf(wr, "\n");
			}
		} else {
			// imagine color ascii
			fprintf(wr, "P3\n");
			fprintf(wr, "%d %d\n", column, row);
			fprintf(wr, "%d\n", MAX);
			for (int i = 0; i < row; i++) {
				for (int j = 0; j < 3 * column; j++)
					fprintf(wr, "%d ", matr[i][j]);
				fprintf(wr, "\n");
			}
		}
	} else {
		// se returneaza doar filename
		if (type == 1) {
			// imagine gri binar
			fprintf(wr, "P5\n");
			fprintf(wr, "%d %d\n", column, row);
			fprintf(wr, "%d\n", MAX);
			for (int i = 0; i < row; i++)
				for (int j = 0; j < column; j++)
					fwrite(&matr[i][j], sizeof(unsigned char), 1, wr);
		} else {
			// imagine color binar
			fprintf(wr, "P6\n");
			fprintf(wr, "%d %d\n", column, row);
			fprintf(wr, "%d\n", MAX);
			for (int i = 0; i < row; i++)
				for (int j = 0; j < 3 * column; j++)
					fwrite(&matr[i][j], sizeof(unsigned char), 1, wr);
		}
	}
	printf("Saved %s\n", filename);
	fclose(wr);
	free(input);
	free(filename);
	free(ascii);
	matr = NULL;
}

void loading(char *magic_word, pic *image)
{
	char *filename = malloc(50 * sizeof(char));
	scanf("%s", filename);
	FILE *in = fopen(filename, "r");
	// verificari output
	if (!in) {
		printf("Failed to load %s\n", filename);
		free(filename);
		if (image->matrix) {
			free_matrix(image->rows, image->matrix);
			image->matrix = NULL;
		}
		return;
	}
	if (image->matrix)
		free_matrix(image->rows, image->matrix);

	fgets(magic_word, LENGHT, in);

	// tratarea ignorarii comentariilor
	char *comment;
	comment = malloc(COMMENT_L * sizeof(char *));
	char c;
	fscanf(in, "%c", &c);
	while (c == '#') {
		fgets(comment, COMMENT_L, in);
		fscanf(in, "%c", &c);
	}
	free(comment);
	//primul caracter nu mai e # acum
	fseek(in, -1, SEEK_CUR);
	// dimensiunile imaginii
	fscanf(in, "%d%d\n", &image->columns, &image->rows);
	int max_val;
	fscanf(in, "%d", &max_val);
	fseek(in, 1, SEEK_CUR);
	read(in, image, magic_word);
	image->x1 = 0;
	image->x2 = 0;
	image->x2 = image->columns;
	image->y2 = image->rows;
	printf("Loaded %s\n", filename);
	fclose(in);
	free(filename);
}

void exiting(pic *image)
{
	if (no_loaded(image))
		return;
	free_matrix(image->rows, image->matrix);
}

void histogram(char *word, pic *image)
{ // mesajele posibile de eroare
	if (no_loaded(image)) {
		while (getchar() != '\n')
			;
	return;
	}
	if (getchar() == '\n') {
		printf("Invalid command\n");
		return; }
	int type = find_image_type(word);
	if (type == 3 || type == 6) {
		printf("Black and white image needed\n");
	while (getchar() != '\n')
		;
	return;
	}
	int x, y;
	scanf("%d%d", &x, &y);
	if (getchar() != '\n') {
		printf("Invalid command\n");
		return;
	}
	int fr[257] = {0};
	for (int i = 0; i < image->rows; i++)
		for (int j = 0; j < image->columns; j++)
			fr[image->matrix[i][j]]++;
	int group;
	group = 256 / y;
	int sum_fr = 0;
	int max_fr = 0;
	// aflam maximul frecventei din fiecare bin
	for (int m = 0; m < MAX + 1; m++) {
		sum_fr = sum_fr + fr[m];
		if ((m + 1) % group == 0) {
			if (sum_fr > max_fr)
				max_fr = sum_fr;
			sum_fr = 0;
		}
	}
	double sum = 0;
	int star_number;
	for (int i = 0; i < MAX + 1; i++) {
		// frecventa valoare
		sum = sum + fr[i];
			if ((i + 1) % group == 0) {
				star_number = (int)(1.0 * sum / max_fr * x);
				printf("%d\t|\t", star_number);
				for (int s = 0 ; s < star_number; s++)
					printf("*");
				printf("\n");
			sum = 0;
		}
	}
}

void equalize(pic *image)
{	if (no_loaded(image)) //==1
		return;
	if (image->image_type == 2) {
		printf("Black and white image needed\n");
		return;
	}
	long long area;
	area = image->rows * image->columns;
	// folosim un vector de frecventa pentru numarul de aparitii
	int fr[MAX + 1] = {0};
	for (int i = 0; i < image->rows; i++)
		for (int j = 0; j < image->columns; j++)
			fr[image->matrix[i][j]]++;

	long long f[MAX + 1] = {0};
	for (int k = 0 ; k <= MAX ; k++) {
		long long s_fr = 0;
		for (int l = 0; l <= k; l++)
			s_fr = s_fr + fr[l];
		f[k] = 1.0 * MAX / area * s_fr;
	}
	// aducem imaginea la noua forma
	for (int i = 0; i < image->rows; i++)
		for (int j = 0; j < image->columns; j++)
			image->matrix[i][j] = (int)clamp(0, MAX, f[image->matrix[i][j]]);
	printf("Equalize done\n");
}

void crop(pic *image)
{
	if (no_loaded(image))
		return;
	// introducem o matrice auxiliara noua
	int **new;
	int new_wid, new_leng;
	new_wid = image->x2 - image->x1;
	new_leng = image->y2 - image->y1;

	if (image->image_type == 1)
		new = alloc_matrix(new_leng, new_wid);
	else
		new = alloc_matrix(new_leng, 3 * new_wid);
	if (!check_memory(new))
		return;
	// construirea noii imagini
	if (image->image_type == 1) {
		// imagini grayscale
		for (int i = 0; i < new_leng; i++)
			for (int j = 0; j < new_wid; j++)
				new[i][j] = image->matrix[i + image->y1][j + image->x1];
	free_matrix(image->rows, image->matrix);
	// revenim la formatul initial si setam noile dimensiuni
	image->matrix = alloc_matrix(new_leng, new_wid);
	for (int i = 0; i < new_leng; i++)
		for (int j = 0; j < new_wid; j++)
			image->matrix[i][j] = new[i][j];
	image->x1 = 0;
	image->y1 = 0;
	image->x2 = new_wid;
	image->y2 = new_leng;
	image->rows = new_leng;
	image->columns = new_wid;
	printf("Image cropped\n");
	free_matrix(new_leng, new);
	} else {
		// rgb
		for (int i = 0; i < new_leng; i++)
			for (int j = 0; j < 3 * new_wid; j++)
				new[i][j] = image->matrix[i + image->y1][j + 3 * image->x1];
		free_matrix(image->rows, image->matrix);
		// revenim la matricea initiala si setam noile dimensiuni
		image->matrix = alloc_matrix(new_leng, 3 * new_wid);
		for (int i = 0; i < new_leng; i++)
			for (int j = 0; j < 3 * new_wid; j++)
				image->matrix[i][j] = new[i][j];
		image->x1 = 0;
		image->y1 = 0;
		image->x2 = new_wid;
		image->y2 = new_leng;
		image->rows = new_leng;
		image->columns = new_wid;
		printf("Image cropped\n");
		free_matrix(new_leng, new);
	}
}

double **alloc_filter(void)
{
	double **a;
	a = malloc(3 * sizeof(double *));
	for (int i = 0; i < 3; i++)
		a[i] = malloc(3 * sizeof(double));
	return a;
}

double **edge(void)
{
	double **e = alloc_filter();
	e[0][0] = -1;
	e[0][1] = -1;
	e[0][2] = -1;
	e[1][0] = -1;
	e[1][1] = 8;
	e[1][2] = -1;
	e[2][0] = -1;
	e[2][1] = -1;
	e[2][2] = -1;
	return e;
}

double **sharpen(void)
{
	double **s = alloc_filter();
	s[0][0] = 0;
	s[0][1] = -1;
	s[0][2] = 0;
	s[1][0] = -1;
	s[1][1] = 5;
	s[1][2] = -1;
	s[2][0] = 0;
	s[2][1] = -1;
	s[2][2] = 0;
	return s;
}

double **blur(void)
{
	double **b = alloc_filter();
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			b[i][j] = 1.0 / 9.0;
	return b;
}

double **gauss(void)
{
	double **g = alloc_filter();
	g[0][0] = 1.;
	g[0][1] = 2.;
	g[0][2] = 1.;
	g[1][0] = 2.;
	g[1][1] = 4.;
	g[1][2] = 2.;
	g[2][0] = 1.;
	g[2][1] = 2.;
	g[2][2] = 1.;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			g[i][j] /= 16;
	return g;
}

void apply(pic *image)
{
	if (no_loaded(image)) {
		while (getchar() != '\n')
			;
		return;
	}
	if (getchar() == '\n') {
		printf("Invalid command\n");
		return;
	}
	char *word = malloc(20 * sizeof(char));
	scanf("%s", word);
	if (image->image_type == 1) {
		printf("Easy, Charlie Chaplin\n");
		free(word);
		return;
	}
	if (!strstr(word, "EDGE") && !strstr(word, "SHARPEN") &&
		!strstr(word, "BLUR") && !strstr(word, "GAUSSIAN_BLUR")) {
		printf("APPLY parameter invalid\n");
		free(word);
		return;
	}
	double **filter;
	if (strstr(word, "EDGE") == word) {
		filter = edge();
		printf("APPLY EDGE done\n");
	} else {
		if (strstr(word, "SHARPEN") == word) {
			filter = sharpen();
			printf("APPLY SHARPEN done\n");
		} else {
			if (strstr(word, "BLUR") == word) {
				filter = blur();
				printf("APPLY BLUR done\n");
			} else {
				if (strstr(word, "GAUSSIAN_BLUR") == word) {
					filter = gauss();
					printf("APPLY GAUSSIAN_BLUR done\n");
				} } } }
	free(word);
	int i1 = image->y1, i2 = image->y2, j1 = image->x1, j2 = image->x2;
	// verificari margini
	if (image->y1 == 0)
		i1++;
	if (image->y2 == image->rows)
		i2--;
	if (image->x1 == 0)
		j1++;
	if (image->x2 == image->columns)
		j2--;
	int dim1 = image->y2 - image->y1;
	int dim2 = image->x2 - image->x1;
	int **mat = alloc_matrix(dim1, 3 * dim2);
	for (int i = i1; i < i2; i++)
		for (int j = 3 * j1; j < 3 * j2; j += 3) {
			double sum_r = 0.0, sum_g = 0.0, sum_b = 0.0;
			for (int k1 = 0; k1 < LENGHT; k1++)
				for (int k2 = 0; k2 < LENGHT; k2++) {
					sum_r = sum_r + filter[k1][k2] * image->matrix[i
+ k1 - 1][j + 3 * k2 - 3];
					sum_g = sum_g + filter[k1][k2] * image->matrix[i
+ k1 - 1][j + 3 * k2 - 2];
					sum_b = sum_b + filter[k1][k2] * image->matrix[i
+ k1 - 1][j + 3 * k2 - 1];
				}
			sum_r = clamp(0, MAX, sum_r);
			sum_g = clamp(0, MAX, sum_g);
			sum_b = clamp(0, MAX, sum_b);
			mat[i - i1][j - 3 * j1] = round(sum_r);
			mat[i - i1][j - 3 * j1 + 1] = round(sum_g);
			mat[i - i1][j - 3 * j1 + 2] = round(sum_b);
		}
	for (int i = i1; i < i2; i++)
		for (int j = 3 * j1; j < 3 * j2; j++)
			image->matrix[i][j] = mat[i - i1][j - 3 * j1];
	free_matrix(dim1, mat);
	for (int i = 0; i < LENGHT; i++)
		free(filter[i]);
	free(filter);
}

// rotire la stanga

void rot_with_90(pic *image)
{
	int dim1 = image->y2 - image->y1;
	int dim2 = image->x2 - image->x1;
	int **matr;
	if (image->image_type == 1) {
		matr = alloc_matrix(dim2, dim1);
		// coordonatele isi schimba pozitiile
		for (int i = image->y1; i < image->y2; i++)
			for (int j = image->x1; j < image->x2; j++)
				matr[dim2 - 1 - (j - image->x1)][i - image->y1]
= image->matrix[i][j];
	if (dim1 != image->rows || dim2 != image->columns) {
		// rotire selectie
		for (int i = image->y1 ; i < image->y2; i++)
			for (int j = image->x1; j < image->x2; j++)
				image->matrix[i][j] = matr[i - image->y1][j - image->x1];
		free_matrix(dim1, matr);
		} else {
			// rotire totala
			free_matrix(image->rows, image->matrix);
			image->matrix = matr;
			matr = NULL;
			swap(&image->rows, &image->columns);
			swap(&image->x2, &image->y2); }
		} else {
			matr = alloc_matrix(3 * dim2, dim1);
			for (int i = image->y1; i < image->y2; i++)
				for (int j = 3 * (image->x1); j < 3 * (image->x2); j++)
					matr[3 * dim2 - 1 - (j - 3 * image->x1)][i - image->y1]
= image->matrix[i][j];
		if (dim1 != image->rows || dim2 != image->columns) {
			// rotire selectie
			for (int i = image->y1; i < image->y2; i++)
				for (int j = 3 * image->x1; j < 3 * image->x2; j += 3) {
					image->matrix[i][j] = matr[(i
- image->y1) * 3 + 2][(j - 3 * image->x1) / 3];
					image->matrix[i][j + 1] = matr[(i
- image->y1) * 3 + 1][(j - 3 * image->x1) / 3];
					image->matrix[i][j + 2] = matr[(i
- image->y1) * 3][(j - 3 * image->x1) / 3];
		}
		free_matrix(3 * dim2, matr);
		} else {
			free_matrix(image->rows, image->matrix);
			image->matrix = alloc_matrix(image->columns, 3 * image->rows);
			for (int i = 0; i < image->columns; i++)
				for (int j = 0; j < 3 * image->rows; j += 3) {
					image->matrix[i][j] = matr[(i - image->y1)
* 3 + 2][(j - 3 * image->x1) / 3];
					image->matrix[i][j + 1] = matr[(i - image->y1)
* 3 + 1][(j - 3 * image->x1) / 3];
					image->matrix[i][j + 2] = matr[(i - image->y1)
* 3][(j - 3 * image->x1) / 3];
			}
			free_matrix(3 * dim2, matr);
			swap(&image->rows, &image->columns);
			swap(&image->x2, &image->y2);
			swap(&image->x1, &image->y1);
		}
	}
}

void rotate(pic *image)
{
	// tratam cazurile de eroare
	if (no_loaded(image)) {
		while (getchar() != '\n')
			;
		return;
	}
	int dim1 = image->y2 - image->y1;
	int dim2 = image->x2 - image->x1;
	if (dim1 != dim2 && (dim1 != image->rows || dim2 !=
		image->columns)) {
		printf("The selection must be square\n");
	while (getchar() != '\n')
		;
	return;
	}
	int angl;
	scanf("%d", &angl);
	if (angl == -360 || angl == 360 || angl == 0) {
		printf("Rotated %d\n", angl);
	} else {
		if (angl == -90 || angl == 270) {
			rot_with_90(image);
			printf("Rotated %d\n", angl);
	} else {
		if (angl == 180 || angl == -180) {
			rot_with_90(image);
			rot_with_90(image);
			printf("Rotated %d\n", angl);
	} else {
		if (angl == -270 || angl == 90) {
			rot_with_90(image);
			rot_with_90(image);
			rot_with_90(image);
			printf("Rotated %d\n", angl);
		} else {
			printf("Unsupported rotation angle\n");
				}
			}
		}
	}
}

int main(void)
{	// numele comenzii
	char q[20];
	char magic_word[LENGHT];
	// alocam memorie pentru imaginea noastra
	pic *collection = malloc(1 * sizeof(pic));
	collection->matrix = NULL;
	scanf("%s", q);
	do { if (strstr(q, "LOAD") == q)
	loading(magic_word, collection);
	else if (strstr(q, "SELECT") == q)
		selection(collection);
	else if (strstr(q, "CROP") == q)
		crop(collection);
	else if (strstr(q, "HISTOGRAM") == q)
		histogram(magic_word, collection);
	else if (strstr(q, "EQUALIZE") == q)
		equalize(collection);
	else if (strstr(q, "APPLY") == q)
		apply(collection);
	else if (strstr(q, "SAVE") == q)
		save(collection);
	else
		if (strstr(q, "ROTATE") == q) {
			rotate(collection);
	} else {
		printf("Invalid command\n");
	while (getchar() != '\n')
		;
	}
	scanf("%s", q);
	} while (strcmp(q, "EXIT") != 0);
	exiting(collection);
	free(collection);
	return 0;
}
