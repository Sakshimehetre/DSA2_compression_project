#include "huffmantree.h"
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<sys/types.h>
#include<string.h>
#include<errno.h>
#define N 256

queue* qinit(int size) {

	queue *q = (queue*)malloc(sizeof(queue));
	q->i = q->j = 0;
	q->size = size;
	q->array = (node**)malloc(size * sizeof(node*));
	return q;

}
int qempty(queue *q) {

	 if(q->i == q->j)
        return 1;
     return 0;

}
int qfull(queue *q) {

	return q->j == q->size;

}
void enqueue(queue *q, node *p) {

	q->array[q->j] = p;
	(q->j)++;

}
node* dequeue(queue *q){

	(q->i)++;
	return q->array[q->i - 1];

}
int qone(queue *q) {

	return (q->j - q->i) == 1;

}
node *qmin(queue *q) {

	node *a = q->array[q->i];  //return the first element of queue having min priority
	return a;

}
void swap(data *xp, data *yp) {

	data temp = *xp;
	*xp = *yp;
	*yp = temp;

}
void bubblesort(data a[], int n) {

	int i, j;
	for (i = 0; i < n-1; i++)
	for (j = 0; j < n-i-1; j++)
     		if (a[j].f > a[j+1].f) //Compare the frequency and arrange in ascending order
           		swap(&a[j], &a[j+1]);

}
int compress(char *in, char *out){

	int size, fd, x, d,freq[N] ={0}, arr[N] = {0}, i, j = 0, actualsize = 0;
	data *a;
	hdcomp *b;
	char c, character[N] = {0};

	fd = open(in, O_RDONLY);
	if(fd == -1) {
		perror("File Open Failed\n");
		exit(1);
	}

	while(1) {
		x = read(fd, &c, sizeof(c));
		if(x == 0)   //Reached end of file else Returns number of bytes successfully read
			break;
		d = c;     // storing character in int array according to ascii value
		actualsize++;
		freq[d]++;  //Increase freq count
	}
  //Actualsize is the total number of characters in the file
	for(i = 0;i < 256; i++) {
		if(freq[i] != 0) {
			arr[j] = freq[i]; //storing whose freq is not zero in arr
			character[j] = i;  //and storing that character in character array
			j++;
		}

	}
//size is the number of types of characters
	size = j;

	a = (data *)malloc (sizeof(data) * j);  // 'data' type hold freq and that character and made 'data' array pointed by a
	i = 0;

	while(i < j) {
		a[i].ch = character[i];  //assigning char and freq to each element in 'data' array
		a[i].f = arr[i];
		i++;
	}

	bubblesort(a,j);  // Arranging the 'data' array in increasing order of the freq
	b = (hdcomp*) malloc (size * sizeof(hdcomp));
	huffman(a, b, size);
    outfile(in, out, a,b, size, actualsize);
	return 0;

}

void huffman(data* a, hdcomp *b, int size){
    int top = 0, arr[N];
	struct node *root = huffmantree(a, size);
	printcode(root, arr, top, a, b);
}
node *huffmantree(data *a, int size){

	int i = 0;
	node *left, *right,*top;
	queue *q1 = qinit(size);
	queue *q2 = qinit(size);

	for(i = 0; i < size; i++){
		if(!qfull(q1))
			enqueue(q1, newnode(a[i].ch, a[i].f)); //data array is sorted in ascending order according to frequency
	}                                              //Enqueued node in q1 with min freq

	while(!(qempty(q1) && qone(q2))) {
		left = removmin(q1, q2);  //extract node with min freq from q1 and q2 and assign to left
		right = removmin(q1, q2);  //extract node with min freq from q1 and q2 and assign to right
		top = (node *) malloc(sizeof(node));
		top -> freq = left -> freq + right -> freq;
		top -> ch = '$';
		top -> left = left;
		top -> right = right;
		enqueue(q2, top);
	}

	return dequeue(q2);  //q2 store the pointer to huffman tree formed

}
node *newnode(char ch, int freq) {

	node *tmp = (node*) malloc(sizeof(node));
	tmp->left = tmp->right = NULL;
	tmp->ch = ch;
	tmp->freq = freq;
	return tmp;

}
node *removmin(queue *q1, queue *q2) {

	if(qempty(q1) && !qempty(q2))
		return dequeue (q2);

	else if(!qempty(q1) && qempty(q2))
		return dequeue (q1);

	if(qmin(q1)->freq < qmin(q2)->freq)  //return code with minimum frequency by comparing q1 and q2
		return dequeue (q1);

	return dequeue (q2);

}
void printcode(node *root, int arr[], int top, data *a, hdcomp *b) {

	int i = 0;

	if(root -> left) {  //Assign 0 if left node exist
		arr[top] = 0;
		printcode(root -> left, arr, top + 1, a, b);
	}

	if(root -> right) { //assign 1 if right node exist
		arr[top] = 1;
		printcode(root -> right, arr, top + 1, a, b);
	}

	if( !(root -> left) && !(root -> right)){  //leaf node
		arr[top] = '\0';
		while(a[i].ch != root->ch)
			i++;
		int j = i;

		b[j].ch = a[j].ch;
	 	b[j].l = top;  // top means the length of code for that character
		for(i = 0; i < top; i++ ){
			b[j].string[i] = arr[i];  //copy the code maintained till the character i.e leaf node reaches to 'string' of b
		}
	}
}
void outfile(char *in, char* out, data *a, hdcomp *b, int size, int actualsize) {

	int i, cnt, len, p;
	int bits = 0;
	char ch;
	int fr, fw;
	dict *m;
	fr = open(in, O_RDONLY);

	if(fr == -1) {
		perror("File Open Failed123\n");
		exit(errno);
	}

	fw = open(out,O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR); //O_creat create file if not
	if(fw == -1) {
		perror("File Open Failed456 \n");
		exit(errno);
	}
	int w=size;
	int bitcnt=0,chara_no=0;
	printf("character \t code\n");
	while(w){
        printf("%c \t \t",b[w].ch);
        chara_no++;
        int m=b[w].l;
        for(int i=0;i<m;i++){
            printf("%d",b[w].string[i]);
            bitcnt++;
        }
        printf("\n");
        w--;
	}
  printf("Before compression total bits: %d\n",chara_no*8);
  printf("After compression total bits: %d\n",bitcnt);

	write(fw, &size, sizeof(size));
	write(fw, &actualsize, sizeof(actualsize));
	m = (dict*) malloc(sizeof(dict) * size);

	while(read(fr, &ch, sizeof(ch))) { //Reading chracter from input file
		i = 0;
		while(i < size) {
			if(ch == b[i].ch) {
				break;
			}
			else
				i++;

		}

		len = b[i].l;
		int k = 0;
		k = len;
		cnt = 0;
		bits = 0;
		int j = 1;
  //converting string of hdcomp to int code in dict
		while(cnt != len){  //traverse the code in hdcomp
				if(b[i].string[cnt] == 1){
					bits |= (1 <<(k - j));  //set the bit at (k-j)th location bit of 'bits'
				}
				cnt++;
				j++;
		}
		m[i].c = b[i].ch;
		m[i].code = bits;

	}

	/*for(i = 0; i < size; i++)
		for(p = i + 1; p < size; p++)
			if(m[i].code == m[p].code){
				m[p].code = m[p].code + 11111111;
			}*/

	i = 0;
	while(i < size){  //writing dictionary
		write(fw, &m[i], sizeof(dict));
		i++;
	}

	lseek(fr, 0 , 0); //Set file to offset at begining
	int  x;
	i = 0;

	while(i < actualsize){
		x = read(fr, &ch, sizeof(ch));
		for(p = 0;p <= size; p++){
			if(ch == m[p].c){ //character read from fr & matched with char in dict and code written in fw
				write(fw, &m[p].code, sizeof(m[p].code));
				break;
			}
		}
		if(x == 0) //EOF
			break;
		i++;
	}

	close(fr);
	close(fw);
}
void decompress(char *in, char *out) {

	int size, i = 0, bits = 0, j = 0, actualsize = 0;
	dict *b;
	int fd = open(in, O_RDONLY);

	if(fd == -1){
		printf("ERROR OPENING FILE\n");
		exit(errno);
	}

	FILE *fw = fopen(out, "w");
	if(fw == NULL){
		printf("ERROR OPENING FILE\n");
		exit(errno);
	}

	read(fd, &size, sizeof(size));
	read(fd, &actualsize, sizeof(actualsize));
	b = (dict *) malloc(sizeof(dict) * size);

	while(i < size) {
		read(fd, &b[i], sizeof(b)); //Read the dictionary present at the start of input file
		i++;

	}

	while(read(fd, &bits, sizeof(bits))) {

		for(j = 0;j < actualsize; j++){

			if(bits == b[j].code){
				fprintf(fw, "%c", b[j].c);

				break;
			}
		}
	}

	close(fd);
	fclose(fw);

}

