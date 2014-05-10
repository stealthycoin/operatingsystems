#define FUNCTIONIZE(a,b) a(b)
#define STRINGIZE(a)     #a
#define INT2STRING(i)    FUNCTIONIZE(STRINGIZE, i)
#define FILE_POS         __FILE__ ":" INT2STRING(__LINE__)
#define malloc(s)       slug_malloc((s), FILE_POS)
#define free(a)         slug_free((a), FILE_POS)

void *slug_malloc(size_t size, char *WHERE);
void *slug_free(void *addr, char *WHERE);
void slug_memstats(void);
