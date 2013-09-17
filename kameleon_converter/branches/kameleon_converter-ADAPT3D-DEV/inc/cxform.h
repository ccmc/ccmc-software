/*
** cxform.h  --  prototypes and headers for Ed's coordinate transform package
*/

#ifndef CXFORM_H
#define CXFORM_H


typedef double Vec[3];
typedef double Mat[3][3];

int cxform(const char *from,const char *to,const double et,Vec v_in,Vec v_out);

char *cxform_err(void);

/*
** Utility functions
*/
double gregorian_calendar_to_jd(int y, int m, int d, int h, int mi, int s);
long cxRound(double doub);
long date2es(int yyyy, int mm, int dd, int hh, int mm2, int ss);

/*
** Matrix multiplication and transposition
*/
void mat_transpose(Mat m_in, Mat m_out);
void mat_times_mat(Mat m1,   Mat m2, Mat m_out);
void mat_times_vec(Mat m1,   Vec v1, Vec v_out);

enum direction { FORWARD, BACK };
typedef enum direction Direction;


#endif
