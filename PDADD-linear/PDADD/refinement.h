
void findDiscords(float **S, float **C, int * Candidats, float r, int N, int n, int H, int p, bool **B);

void getDiscords(bool ** B, int *candidats, int *Discords, int H, int *D, int p);

void refinement(float **S, float **C, int *Candidats, int *Discords, float r, int N, int n, int H, int *D, int p);