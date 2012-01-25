
#define WILL_WRITE_TO_LOG       1
//#define LOG_WRITE(a) if (WILL_WRITE_TO_LOG == 1) { printf (a"\n"); }
#define LOG_WRITE(a)                   fprintf(stdout, a);     \
                                       fprintf(stdout, "\n");  \
                                       fflush(stdout);

#define LOG_FLUSH                      fflush(stdout);

#define LOG_WRITE_ERROR(a)             fprintf(stderr, a);
