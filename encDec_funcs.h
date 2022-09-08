#ifndef ENCDEC_FUNCS
#define ENCDEC_FUNCS

void   parseArgs(int, char *[], char **, char **, char **);     /* in: argc, argv; out: ifile, ofile, kfile */
void  *Malloc(size_t);                                          /* in: bytes to allocate; return: ptr of type void to allocated memory */
FILE  *Fopen(const char *, const char *);                       /* in: path/file, mode; return: file ptr */
void   readKey(unsigned char *, size_t, FILE *);                /* in: buffer ptr, key length, key file ptr */
void   encryptDecrypt(unsigned char *, unsigned char *, size_t, FILE *, FILE *);  /* in: buffer ptr, key ptr, buffer size, input file ptr, output file ptr */
size_t Fread(void *, size_t, size_t, FILE *);                   /* in: buffer ptr, buffer size, num buffers, input file ptr; return: bytes read */
size_t Fwrite(const void *, size_t, size_t, FILE *);            /* in: buffer ptr, buffer size, num buffers, output file ptr; return: bytes written */
void   Fclose(FILE *);                                          /* in: file ptr */
void   help();

#endif
