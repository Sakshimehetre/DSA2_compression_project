//Adt for storing char and freq of that char
typedef struct data{
	char ch;
	unsigned f;
}data;
//Dictionary
typedef struct dict{
	char c;
	int code;
}dict;
//Adt for storing code obtained by traversing huffman tree
typedef struct hdcomp{
	char ch;
	int string[32];
	int l;

}hdcomp;
//Node structure for huffman tree
typedef struct node{
	char ch;
	unsigned freq;
	struct node *left, *right;

}node;

typedef struct queue{
	int i, j;
	int count;
	int size;
	node **array;
}queue;
node *removmin(queue *q1, queue *q2);//remove node with min freq
int compress(char *in, char *out);
void huffman(data* a, hdcomp *b, int size);
node *huffmantree(data *a, int size);
node *newnode(char ch, int freq);

void printcode(node *root, int arr[], int top, data *a, hdcomp *b);
void outfile(char *in, char* out, data *a, hdcomp *b, int size, int actualsize);
queue * qinit(int size);
int qempty(queue *q);
int qfull(queue *q);
void enqueue(queue *q, node *n);
node *dequeue(queue *q);
node *qmin(queue *q);  //return the first element of queue having min priority
int one(queue *q);

void decompress(char *in, char *out);
void bubblesort(data a[], int n);
void swap(data *xp, data *yp) ;

