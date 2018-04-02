void printArray(struct tablo * tmp) : non parallélisée
long long max(long long a, long long b) : non parallélisée
long long max_tablo(struct tablo * t) : non parallélisée

void montee(struct tablo * source, struct tablo * destination) : parallélisée
void montee_max(struct tablo * source, struct tablo * destination) : parallélisée

void montee_inversee(struct tablo * source, struct tablo * destination) : parallélisée
void montee_inversee_max(struct tablo * source, struct tablo * destination) : parallélisée

void descente(struct tablo * a, struct tablo * b) : parallélisée
void descente_max(struct tablo * a, struct tablo * b) : parallélisée

void final(struct tablo * a, struct tablo *b) : parallélisée
void final_max(struct tablo * a, struct tablo *b) : parallélisée

void sum_prefix(struct tablo *source, struct tablo * psum) : parallélisée
void sum_suffix(struct tablo *source, struct tablo * ssum) : parallélisée
void max_suffix(struct tablo * source, struct tablo * smax) : parallélisée
void max_prefix(struct tablo *source, struct tablo * pmax) : parallélisée

void sum_subsequence(struct tablo *source, struct tablo *pmax, struct tablo *ssum, struct tablo *smax, struct tablo * psum, struct tablo *m) : parallélisée
void find_subsequence_max(struct tablo * source, struct tablo * m) : non parallélisée

void generateArray_from_file(struct tablo * s, char * file_name) : non parallélisée
