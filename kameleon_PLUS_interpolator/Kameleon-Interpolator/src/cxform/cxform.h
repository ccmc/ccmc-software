/*
** cxform.h  --  prototypes and headers for Ed's coordinate transform package
*/

#ifndef CXFORM_H
#define CXFORM_H

#ifdef __cplusplus
#define _C_ "C"
#endif

#ifndef __cplusplus
#define _C_
#endif

typedef double Vec[3];
typedef double Mat[3][3];


extern _C_ int cxform(const char *from,const char *to,const double et,Vec v_in,Vec v_out);

extern _C_ char *cxform_err(void);

/*
** Utility functions
*/
extern _C_ double gregorian_calendar_to_jd(int y, int m, int d, int h, int mi, int s);
extern _C_ long cxRound(double doub);
extern _C_ long date2es(int yyyy, int mm, int dd, int hh, int mm2, int ss);

/*
** Matrix multiplication and transposition
*/
extern _C_ void mat_transpose(Mat m_in, Mat m_out);
extern _C_ void mat_times_mat(Mat m1,   Mat m2, Mat m_out);
extern _C_ void mat_times_vec(Mat m1,   Vec v1, Vec v_out);

enum direction { FORWARD, BACK };
typedef enum direction Direction;


#endif
