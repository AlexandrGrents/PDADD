
void findCandidats(float **S, float r, int N, int n, int L, int p, int **I, int *Insert, int *Bottom, int *Count);

void getCandidats(int **I, int *Candidats, int *Bottom, int *Count, int L, int p, int &H);

void get_C(float **S, float **C, int *Candidats, int *H, int n);

void selection(float **S, float **C, float r, int N, int n, int L, int p, int *H, int *Candidats);