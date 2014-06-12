/* #define DEBUGX */
/* #define DEBUG */
/* #define DEBUGS */
/*
#define TEST_CASE
*/
#define DIPOLE
#define LINEAR_INTERPOL


/*--------------------------------------------------------------

       program test_unstructured_search  

-------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>               /* for srand */
#include <math.h>

#include "adapt3d_cdf_interface_variables.h"


#define min(x,y) ((x) < (y) ? (x) : (y))
#define max(x,y) ((x) > (y) ? (x) : (y))


void setup_test_unkno();

void srand(unsigned int seed);
int rand(void);
float ffindmin(float array[], long n);
float ffindmax(float array[], long n);
float fminloc(float array[], long n, int mask[]);
float fmaxloc(float array[], long n, int mask[]);




/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */


int setup_search_unstructured_grid()
/*-----------------------------------------------------------------
!
!
! This routine sets up a look-up style index for use in searching
! for the element containing any arbitrary location in an unstructured
! grid.
!
!
!-----------------------------------------------------------------*/
{


      int ipa,ipb,ipc,ipd;
      int i_s,j_s,k_s,ielem;
      int i,j,k;
      int ii;
      int nelems_in_cell[nz_sg][ny_sg][nx_sg];
      int countup, countdown;


      float xlo,xhi;
      float ylo,yhi;
      float zlo,zhi;
      float xmean,ymean,zmean;
      float side_l_1,side_l_2,side_l_3;
      float side_l_4,side_l_5,side_l_6;
      float max_element_length,max_length_sqrd;
      float max_length_sqrd_old;
      float dxyz[3];
      float dxyz_min;
      float arr2[2];
      float arr4[4];
      float arr7[7];
      long   len;


/*-----------------------------------------------------------------*/
      printf("Entered Structured Search Grid\n");


/* allocate for ELEM_INDEX_STRUCT */
       printf("Begin allocation of elem_index_struct \n");
       int (*elem_index_struct)[3] = (int (*)[3])malloc(sizeof(int)*(int)nelem*3);
       if(elem_index_struct == NULL)
       {
         printf("Error: allocation of elem_index_struct failed\n");
         return 1;
       }
       printf("Allocation of elem_index_struct complete \n");

      last_element_found = -1;


      xl_sg=1.e30;
      xr_sg=-1.e30;
      yl_sg=1.e30;
      yr_sg=-1.e30;
      zl_sg=1.e30;
      zr_sg=-1.e30;

      printf("npoin,ndimn %d %d \n",npoin, ndimn);

/* coord is a 1D vector where the first ndimn words are x,y,z of node 0, the next ndimn words
 * for node 1, etc
 */
      for ( i=0; i<(int)npoin; i++) {
        xl_sg=min(xl_sg,coord[ index_2d_to_1d(0,i,0,npoin) ]);
        xr_sg=max(xr_sg,coord[ index_2d_to_1d(0,i,0,npoin) ]);
        yl_sg=min(yl_sg,coord[ index_2d_to_1d(1,i,0,npoin) ]);
        yr_sg=max(yr_sg,coord[ index_2d_to_1d(1,i,0,npoin) ]);
        zl_sg=min(zl_sg,coord[ index_2d_to_1d(2,i,0,npoin) ]);
        zr_sg=max(zr_sg,coord[ index_2d_to_1d(2,i,0,npoin) ]); 
      }

      printf("-------------------------------\n");
      printf("Range of Structured Search Grid\n");
      printf("xl_sg= %e \n",xl_sg);
      printf("xr_sg= %e \n",xr_sg);
      printf("yl_sg= %e \n",yl_sg);
      printf("yr_sg= %e \n",yr_sg);
      printf("zl_sg= %e \n",zl_sg);
      printf("zr_sg= %e \n",zr_sg);
      printf("-------------------------------\n");


/* Step 1 - Define structured grid */

      dx_sg = (xr_sg - xl_sg)/( (float)nx_sg );
      dy_sg = (yr_sg - yl_sg)/( (float)ny_sg );
      dz_sg = (zr_sg - zl_sg)/( (float)nz_sg );
      dxyz[0] = dx_sg;
      dxyz[1] = dy_sg;
      dxyz[2] = dz_sg;
      len=3;
      printf("dxyz[0]= %e \n",dxyz[0]);
      printf("dxyz[1]= %e \n",dxyz[1]);
      printf("dxyz[2]= %e \n",dxyz[2]);
      dxyz_min = ffindmin(dxyz,len);
      printf("dxyzmin= %e \n",dxyz_min);


/* Initialize the counters for the number of elements in each grid cell */
      for ( k=0; k<nz_sg; k++) {
      for ( j=0; j<ny_sg; j++) {
      for ( i=0; i<nx_sg; i++) {
        nelems_in_cell[k][j][i] = 0;
      }}}


/* Step 2 - Compute index relative to structured grid for each element of
            the unstructured grid, using the element mid-point coord.
*/

      max_length_sqrd_old=0.;
      max_length_sqrd=0.;


/* intmat is a 1D vector where the first 4 words are the node numbers of element 0, the next 4 words
 * for element 1, etc
 */
      for ( ielem=0; ielem<(int)nelem; ielem++) {

         ipa = intmat[ index_2d_to_1d(ielem,0,nelem,4) ] -1 ;
         ipb = intmat[ index_2d_to_1d(ielem,1,nelem,4) ] -1 ;
         ipc = intmat[ index_2d_to_1d(ielem,2,nelem,4) ] -1 ;
         ipd = intmat[ index_2d_to_1d(ielem,3,nelem,4) ] -1 ;
/* coord is a 1D vector where the first ndimn words are x,y,z of node 0, the next ndimn words
 * for node 1, etc
 */
         side_l_1= pow(coord[ index_2d_to_1d(0,ipa,0,npoin) ]-coord[ index_2d_to_1d(0,ipb,0,npoin) ],2) +
                   pow(coord[ index_2d_to_1d(1,ipa,0,npoin) ]-coord[ index_2d_to_1d(1,ipb,0,npoin) ],2) +
                   pow(coord[ index_2d_to_1d(2,ipa,0,npoin) ]-coord[ index_2d_to_1d(2,ipb,0,npoin) ],2) ;
         side_l_2= pow(coord[ index_2d_to_1d(0,ipa,0,npoin) ]-coord[ index_2d_to_1d(0,ipc,0,npoin) ],2) +
                   pow(coord[ index_2d_to_1d(1,ipa,0,npoin) ]-coord[ index_2d_to_1d(1,ipc,0,npoin) ],2) +
                   pow(coord[ index_2d_to_1d(2,ipa,0,npoin) ]-coord[ index_2d_to_1d(2,ipc,0,npoin) ],2) ;
         side_l_3= pow(coord[ index_2d_to_1d(0,ipa,0,npoin) ]-coord[ index_2d_to_1d(0,ipd,0,npoin) ],2) +
                   pow(coord[ index_2d_to_1d(1,ipa,0,npoin) ]-coord[ index_2d_to_1d(1,ipd,0,npoin) ],2) +
                   pow(coord[ index_2d_to_1d(2,ipa,0,npoin) ]-coord[ index_2d_to_1d(2,ipd,0,npoin) ],2) ;
         side_l_4= pow(coord[ index_2d_to_1d(0,ipb,0,npoin) ]-coord[ index_2d_to_1d(0,ipc,0,npoin) ],2) +
                   pow(coord[ index_2d_to_1d(1,ipb,0,npoin) ]-coord[ index_2d_to_1d(1,ipc,0,npoin) ],2) +
                   pow(coord[ index_2d_to_1d(2,ipb,0,npoin) ]-coord[ index_2d_to_1d(2,ipc,0,npoin) ],2) ;
         side_l_5= pow(coord[ index_2d_to_1d(0,ipb,0,npoin) ]-coord[ index_2d_to_1d(0,ipd,0,npoin) ],2) +
                   pow(coord[ index_2d_to_1d(1,ipb,0,npoin) ]-coord[ index_2d_to_1d(1,ipd,0,npoin) ],2) +
                   pow(coord[ index_2d_to_1d(2,ipb,0,npoin) ]-coord[ index_2d_to_1d(2,ipd,0,npoin) ],2) ;
         side_l_6= pow(coord[ index_2d_to_1d(0,ipc,0,npoin) ]-coord[ index_2d_to_1d(0,ipd,0,npoin) ],2) +
                   pow(coord[ index_2d_to_1d(1,ipc,0,npoin) ]-coord[ index_2d_to_1d(1,ipd,0,npoin) ],2) +
                   pow(coord[ index_2d_to_1d(2,ipc,0,npoin) ]-coord[ index_2d_to_1d(2,ipd,0,npoin) ],2) ;

         arr7[0] = max_length_sqrd;
         arr7[1] = side_l_1;
         arr7[2] = side_l_2;
         arr7[3] = side_l_3;
         arr7[4] = side_l_4;
         arr7[5] = side_l_5;
         arr7[6] = side_l_6;

         max_length_sqrd=ffindmax(arr7,7);

         arr4[0] = coord[ index_2d_to_1d(0,ipa,0,npoin) ];
         arr4[1] = coord[ index_2d_to_1d(0,ipb,0,npoin) ];
         arr4[2] = coord[ index_2d_to_1d(0,ipc,0,npoin) ];
         arr4[3] = coord[ index_2d_to_1d(0,ipd,0,npoin) ];
         xlo = ffindmin(arr4,4);
         xhi = ffindmax(arr4,4);
         arr4[0] = coord[ index_2d_to_1d(1,ipa,0,npoin) ];
         arr4[1] = coord[ index_2d_to_1d(1,ipb,0,npoin) ];
         arr4[2] = coord[ index_2d_to_1d(1,ipc,0,npoin) ];
         arr4[3] = coord[ index_2d_to_1d(1,ipd,0,npoin) ];
         ylo = ffindmin(arr4,4);
         yhi = ffindmax(arr4,4);
         arr4[0] = coord[ index_2d_to_1d(2,ipa,0,npoin) ];
         arr4[1] = coord[ index_2d_to_1d(2,ipb,0,npoin) ];
         arr4[2] = coord[ index_2d_to_1d(2,ipc,0,npoin) ];
         arr4[3] = coord[ index_2d_to_1d(2,ipd,0,npoin) ];
         zlo = ffindmin(arr4,4);
         zhi = ffindmax(arr4,4);

         xmean = 0.5*(xlo+xhi);
         ymean = 0.5*(ylo+yhi);
         zmean = 0.5*(zlo+zhi);

         
         i_s = (int)( (xmean-xl_sg)/dx_sg ) ;
         j_s = (int)( (ymean-yl_sg)/dy_sg ) ;
         k_s = (int)( (zmean-zl_sg)/dz_sg ) ;
         elem_index_struct[ielem][0] = i_s;
         elem_index_struct[ielem][1] = j_s;
         elem_index_struct[ielem][2] = k_s;
         nelems_in_cell[k_s][j_s][i_s] = nelems_in_cell[k_s][j_s][i_s] + 1;



#ifdef DEBUGX
/*         if(ielem < 10) { */
         if ( max_length_sqrd > arr7[0] ) {
      printf("coord(0-10) \n");
      printf("coord(0) %e \n",coord[0]);
      printf("coord(1) %e \n",coord[1]);
      printf("coord(2) %e \n",coord[2]);
      printf("coord(3) %e \n",coord[3]);
      printf("coord(4) %e \n",coord[4]);
      printf("coord(5) %e \n",coord[5]);
      printf("coord(6) %e \n",coord[6]);
      printf("coord(7) %e \n",coord[7]);
      printf("coord(8) %e \n",coord[8]);
      printf("coord(9) %e \n",coord[9]);
      printf("intmat(0-10) \n");
      printf("intmat(0) %d \n",intmat[0]);
      printf("intmat(1) %d \n",intmat[1]);
      printf("intmat(2) %d \n",intmat[2]);
      printf("intmat(3) %d \n",intmat[3]);
      printf("intmat(4) %d \n",intmat[4]);
      printf("intmat(5) %d \n",intmat[5]);
      printf("intmat(6) %d \n",intmat[6]);
      printf("intmat(7) %d \n",intmat[7]);
      printf("intmat(8) %d \n",intmat[8]);
      printf("intmat(9) %d \n",intmat[9]);
      printf("unkno(0-10) \n");
      printf("unkno(0) %e \n",unkno[0]);
      printf("unkno(1) %e \n",unkno[1]);
      printf("unkno(2) %e \n",unkno[2]);
      printf("unkno(3) %e \n",unkno[3]);
      printf("unkno(4) %e \n",unkno[4]);
      printf("unkno(5) %e \n",unkno[5]);
      printf("unkno(6) %e \n",unkno[6]);
      printf("unkno(7) %e \n",unkno[7]);
      printf("unkno(8) %e \n",unkno[8]);
      printf("unkno(9) %e \n",unkno[9]);
      printf("\n New Max length \n");
      printf("New max_length_sqrd=%e \n",max_length_sqrd);
      printf("arr7: %e %e %e %e %e %e\n",arr7[1],arr7[2],arr7[3],arr7[4],arr7[5],arr7[6]);
      printf("indexes %d %d %d %d \n",
         index_2d_to_1d(ielem,0,nelem,4),
         index_2d_to_1d(ielem,1,nelem,4),
         index_2d_to_1d(ielem,2,nelem,4),
         index_2d_to_1d(ielem,3,nelem,4));
           printf("npoin ndimn %d %d\n",npoin,ndimn);
           printf("ielem %d xmean ymean zmean %e %e %e \n",ielem,xmean,ymean,zmean);
           printf("ielem %d xlo ylo zlo %e %e %e \n",ielem,xlo,ylo,zlo);
           printf("ielem %d xhi yhi zhi %e %e %e \n",ielem,xhi,yhi,zhi);
           printf("ielem %d index_2d_to_1d of nodes are %d %d %d %d \n",ielem,
          index_2d_to_1d(0,ipa,0,npoin) ,
          index_2d_to_1d(0,ipb,0,npoin),
          index_2d_to_1d(0,ipc,0,npoin),
          index_2d_to_1d(0,ipd,0,npoin) );
           printf("ielem %d x of nodes are %e %e %e %e \n",ielem,
          coord[ index_2d_to_1d(0,ipa,0,npoin)] ,
          coord[ index_2d_to_1d(0,ipb,0,npoin)],
          coord[ index_2d_to_1d(0,ipc,0,npoin)],
          coord[ index_2d_to_1d(0,ipd,0,npoin)] );
           printf("ielem %d ipa ipb ipc ipd %d %d %d %d \n",ielem,ipa,ipb,ipc,ipd);
           printf("ielem %d dx_sg dy_sg dz_sg %e %e %e \n",ielem,dx_sg, dy_sg, dz_sg);
           printf("ielem %d i_s j_s k_s %d %d %d \n",ielem,i_s,j_s,k_s);
           printf("ielem %d i_s j_s k_s %d %d %d \n",ielem,i_s,j_s,k_s);
           printf("ielem %d nelems_in_cell[k_s][j_s][i_s]=%d \n",ielem,nelems_in_cell[k_s][j_s][i_s]);
         }
         if(elem_index_struct[ielem][0] > nx_sg){
           printf("ielem %d elem_index_struct[ielem][0] too big %d %d \n",ielem,elem_index_struct[ielem][0]);
         }
         if(elem_index_struct[ielem][1] > ny_sg){
           printf("ielem %d elem_index_struct[ielem][1] too big %d %d \n",ielem,elem_index_struct[ielem][1]);
         }
         if(elem_index_struct[ielem][2] > nz_sg){
           printf("ielem %d elem_index_struct[ielem][2] too big %d %d \n",ielem,elem_index_struct[ielem][2]);
         }
#endif

      }
      max_element_length = sqrt(max_length_sqrd);

      printf("Maximum element length = %e \n",max_element_length);
      printf("Grid cell spacing = %e \n",dxyz_min);

      printf("Ratio of search grid spacing to max element length (Must be greater than 1) = %e \n",
                                   dxyz_min/max_element_length);
      if(max_element_length > dxyz_min) {
        printf("ERROR: UNSTRUCTURED SEARCH GRID SPACING IS TOO FINE \n");
        printf("ERROR: SOLUTION - INCREASE NO OF GRID POINTS BY FACTOR %e \n", max_element_length/dxyz_min);
        exit;
      }


/* Step 3 - place starting and ending indeces of element list into each structured cell */

/* Create start and end pointers for each grid cell's section of the
 index to the element list */
      countup   = 0;
      countdown = nelem-1;
      for ( k=0; k<nz_sg; k++ ) {
      for ( j=0; j<ny_sg; j++ ) {
      for ( i=0; i<nx_sg; i++ ) {
         start_index[k][j][i] = countup;
         countup = countup + nelems_in_cell[k][j][i];
         end_index[nz_sg-1-k][ny_sg-1-j][nx_sg-1-i] = countdown;
         countdown = countdown - nelems_in_cell[nz_sg-1-k][ny_sg-1-j][nx_sg-1-i];
      }}}
      for ( k=0; k<nz_sg; k++ ) {
      for ( j=0; j<ny_sg; j++ ) {
      for ( i=0; i<nx_sg; i++ ) {
        end_index[k][j][i] = max(start_index[k][j][i],end_index[k][j][i]);
      }}}


/* Step 4 - Create the index */
      for ( k=0; k<nz_sg; k++) {
      for ( j=0; j<ny_sg; j++) {
      for ( i=0; i<nx_sg; i++) {
        nelems_in_cell[k][j][i] = 0;
      }}}
      for ( ielem=0; ielem<nelem; ielem++) {
         i = elem_index_struct[ielem][0];
         j = elem_index_struct[ielem][1];
         k = elem_index_struct[ielem][2];
         ii = start_index[k][j][i] + nelems_in_cell[k][j][i];
         indx[ii] = ielem;
         nelems_in_cell[k][j][i] = nelems_in_cell[k][j][i] + 1;
      }

         free(elem_index_struct);

/*     end subroutine setup_search_unstructured_grid */
         return 0;
}


int find_element(float cintp[ndimn], int clear_cache)
{

       int                 ielem,kelem;
       int                 i_s,j_s,k_s,i,j,k,indx_start,indx_end;
       int                 indx1,ifound,just_found,jelem;
       float              x,y,z,shapex[nnode];



         kelem=-1;
         ielem=-1;

         if(clear_cache == 1) last_element_found=-1;
#ifdef DEBUG
       printf("0find_element: coord[0][0-2] : %e %e %e \n",coord[ index_2d_to_1d(0,0,0,npoin) ],coord[ index_2d_to_1d(0,1,0,npoin) ],coord[ index_2d_to_1d(0,2,0,npoin) ]);
#endif

/* If available, use the last element found to begin the search */
         if(last_element_found != -1) {
#ifdef DEBUG
       printf("find_element: coord[0][0-2] : %e %e %e \n",coord[ index_2d_to_1d(0,0,0,npoin) ],coord[ index_2d_to_1d(0,1,0,npoin) ],coord[ index_2d_to_1d(0,2,0,npoin) ]);
#endif
           kelem = smart_search(cintp); 
         }

/* If there is no starting guess for the element number in last_element_found 
   or the smart search failed, use the grid based search
*/
        if(kelem == -1) {


         x = cintp[0];
         y = cintp[1];
         z = cintp[2];
#ifdef DEBUG
         printf("Searching for point x y z = %e %e %e\n",x,y,z); 
#endif
         i_s = (int)( (x-xl_sg)/dx_sg );
         j_s = (int)( (y-yl_sg)/dy_sg );
         k_s = (int)( (z-zl_sg)/dz_sg );

#ifdef DEBUG
         printf("Located in structured cell %d %d %d\n",i_s,j_s,k_s); 
#endif
         indx_start = start_index[k_s][j_s][i_s];
         indx_end   = end_index[k_s][j_s][i_s];

/* test each element between indx_start and indx_end to find the cell
   containing coord1 = (x,y,z)         */
#ifdef DEBUG
         printf("Searching index list %d to %d \n",indx_start,indx_end);
#endif

         indx1 = indx_start;
         ifound = 1;
         while ( (ifound == 1) && (indx1 <= indx_end) && (indx1 > -1) )
         {
           jelem=indx[indx1];
           ifound = chkineln(cintp ,jelem ,shapex);
           if (ifound == 0) ielem=indx1;
           indx1=indx1+1;
         }
#ifdef DEBUG
         if (ifound == 0) {
           printf("Found point in first cell\n");
         } else {
           printf("Did not find point in first cell\n");
         }
#endif

 
/* If element has still not been found then search in list for
    neighboring structured grid cells */
         if (ifound == 1) {
         for ( k=max(0,k_s-1); k<min(k_s+2,nz_sg-1); k++) {
         for ( j=max(0,j_s-1); j<min(j_s+2,ny_sg-1); j++) {
         for ( i=max(0,i_s-1); i<min(i_s+2,nx_sg-1); i++) {
           if(ifound == 1) {
             just_found=1;
             if( ( (i != i_s) || (j != j_s) || (k != k_s) ) ) {
               indx_start = start_index[k][j][i];
               indx_end   = end_index[k][j][i];
               indx1 = indx_start;
               ifound = 1;
               while ( (ifound == 1) && (indx1 <= indx_end) && (indx1 > -1) ) {
                 jelem=indx[indx1];
                 ifound = chkineln(cintp ,jelem ,shapex);
                 if (ifound == 0 ) {
                   ielem=indx1;
                   just_found=0;
                 }
                 indx1=indx1+1;
               }
             }
#ifdef DEBUG
             if ( (ifound == 0) && (just_found == 0) ) {
               printf("Found point in neighbor cell %d %d %d \n",i,j,k);
             } else {
               printf("Did not find point in neighbor cell %d %d %d \n",i,j,k);
             }
#endif
           }
         }}}
         }

#ifdef DEBUG
           printf("ielem is now ielem=%d\n",ielem);
#endif

         if( ielem == -1) {
           printf("Failed to find element using smart search\n");
           printf("Using Brute force now!\n");
         }
         kelem=-1;
         if( ielem != -1) kelem=indx[ielem];

         }
         last_element_found=kelem;

         return kelem;

/*       end subroutine find_element  */
}

       int chkineln( float cintp[3] ,int ielem , float *shapex)
{
       
/*
!...  mesh arrays
*/
/*
       integer,intent(in) ::  ndimn,npoin,nnode,nelem
       integer,intent(in) ::  intmat(nnode,nelem)
       integer,intent(in) ::  ielem
       real*8,intent(in)  ::  coord(ndimn,npoin),cintp(ndimn)

       real*8,intent(out) ::  shapex(nnode)
*/

       int   ipa,ipb,ipc,ipd;
       int   ierro;
       float xa,ya,za,xba,yba,zba,xca,yca,zca,xda,yda,zda;
       float xpa,ypa,zpa;
       float deter,detin,shmin,shmax;
       float rin11,rin12,rin13;
       float rin21,rin22,rin23;
       float rin31,rin32,rin33;


/*       
!       data tolow/ -0.005 /
!       data tolhi/  1.005 /
!       data   c00/  0.0   /
!       data   c10/  1.0   /
!
!...  this sub sees if element ielem contains point cintp,
!     writing the shape-function values into ==> shape
!
!...  find the local coordinates
!
*/

       ipa = intmat[ index_2d_to_1d(ielem,0,nelem,4) ]-1;
       ipb = intmat[ index_2d_to_1d(ielem,1,nelem,4) ]-1;
       ipc = intmat[ index_2d_to_1d(ielem,2,nelem,4) ]-1;
       ipd = intmat[ index_2d_to_1d(ielem,3,nelem,4) ]-1;
       xa  = coord[ index_2d_to_1d(0,ipa,0,npoin) ];
       ya  = coord[ index_2d_to_1d(1,ipa,0,npoin) ];
       za  = coord[ index_2d_to_1d(2,ipa,0,npoin) ];
       xba = coord[ index_2d_to_1d(0,ipb,0,npoin) ] - xa;
       yba = coord[ index_2d_to_1d(1,ipb,0,npoin) ] - ya;
       zba = coord[ index_2d_to_1d(2,ipb,0,npoin) ] - za;
       xca = coord[ index_2d_to_1d(0,ipc,0,npoin) ] - xa;
       yca = coord[ index_2d_to_1d(1,ipc,0,npoin) ] - ya;
       zca = coord[ index_2d_to_1d(2,ipc,0,npoin) ] - za;
       xda = coord[ index_2d_to_1d(0,ipd,0,npoin) ] - xa;
       yda = coord[ index_2d_to_1d(1,ipd,0,npoin) ] - ya;
       zda = coord[ index_2d_to_1d(2,ipd,0,npoin) ] - za;

       deter = xba*(yca*zda-zca*yda) - yba*(xca*zda-zca*xda) + zba*(xca*yda-yca*xda);
#ifdef DEBUG
         printf("coord[ipa]= %d %e %e %e \n",ipa,coord[ index_2d_to_1d(0,ipa,0,npoin) ],coord[ index_2d_to_1d(1,ipa,0,npoin) ],coord[ index_2d_to_1d(2,ipa,0,npoin) ]);
         printf("deter= %e \n",deter);
#endif 
/*       detin = c10/deter */
       detin = 1.0/deter;

       rin11 = detin*(yca*zda-zca*yda);
       rin12 =-detin*(xca*zda-zca*xda);
       rin13 = detin*(xca*yda-yca*xda);
       rin21 =-detin*(yba*zda-zba*yda);
       rin22 = detin*(xba*zda-zba*xda);
       rin23 =-detin*(xba*yda-yba*xda);
       rin31 = detin*(yba*zca-zba*yca);
       rin32 =-detin*(xba*zca-zba*xca);
       rin33 = detin*(xba*yca-yba*xca);

       xpa = cintp[0]-xa;
       ypa = cintp[1]-ya;
       zpa = cintp[2]-za;
/*
!...  local coordinates & shape-function values
*/
       shapex[1] = rin11*xpa + rin12*ypa + rin13*zpa;
       shapex[2] = rin21*xpa + rin22*ypa + rin23*zpa;
       shapex[3] = rin31*xpa + rin32*ypa + rin33*zpa;
       shapex[0] = 1.0 - shapex[1] - shapex[2] - shapex[3];
#ifdef DEBUG
         printf("cintp= %e %e %e \n",cintp[0],cintp[1],cintp[2]);
         printf("xa-za= %e %e %e \n",xa,ya,za);
         printf("shapex = %e %e %e %e \n",shapex[0],shapex[1],shapex[2],shapex[3]);
#endif 
/*       shape(1) = c10 - shape(2) - shape(3) - shape(4)
!
!...  max/min of these shape-functions
!
*/
       shmin = ffindmin(shapex,4);
       shmax = ffindmax(shapex,4);
/*
!...  see if in the element
!
!       if(shmin .ge. tolow .and. shmax .le. tolhi) then
*/
       if ( (shmin > 0.) && (shmax <= 1.0)) {
              ierro = 0;
       } else {
              ierro = 1;
       }

/*
!...  control output
!     write(*,*)' ielem,shmin,shmax,ierro=',ielem,shmin,shmax,ierro
*/

#ifdef DEBUG
       if( ierro == 0) {
         printf("shmin= %e \n",shmin);
         printf("shmax= %e \n",shmax);
         printf("ierro= %d \n",ierro);
         printf("cintp= %e %e %e \n",cintp[0],cintp[1],cintp[2]);
         printf("node 1 = %e %e %e %d \n",coord[ index_2d_to_1d(0,ipa,0,npoin) ],coord[ index_2d_to_1d(1,ipa,0,npoin) ],coord[ index_2d_to_1d(2,ipa,0,npoin) ],ipa);
         printf("node 2 = %e %e %e %d \n",coord[ index_2d_to_1d(0,ipb,0,npoin) ],coord[ index_2d_to_1d(1,ipb,0,npoin) ],coord[ index_2d_to_1d(2,ipb,0,npoin) ],ipb);
         printf("node 3 = %e %e %e %d \n",coord[ index_2d_to_1d(0,ipc,0,npoin) ],coord[ index_2d_to_1d(1,ipc,0,npoin) ],coord[ index_2d_to_1d(2,ipc,0,npoin) ],ipc);
         printf("node 4 = %e %e %e %d \n",coord[ index_2d_to_1d(0,ipd,0,npoin) ],coord[ index_2d_to_1d(1,ipd,0,npoin) ],coord[ index_2d_to_1d(2,ipd,0,npoin) ],ipd);
        }
#endif 

        return ierro;

/*       end subroutine chkineln */
}


void setup_test_unkno()
{

/*
*    Sets up test solution for conserved quantities in unkno
*/


int               i;
float            m0,r,r2,r3,r5;
float            pmass=1.67e-24;


       for( i=0; i<(int)npoin; i++)
       {
         unkno[ index_2d_to_1d(i,0,npoin,NVARS_ADAPT3D) ] = pmass*1.e10;
         unkno[ index_2d_to_1d(i,1,npoin,NVARS_ADAPT3D) ] = 1.e7*unkno[ index_2d_to_1d(i,0,npoin,NVARS_ADAPT3D) ];
         unkno[ index_2d_to_1d(i,2,npoin,NVARS_ADAPT3D) ] = 2.e7*unkno[ index_2d_to_1d(i,0,npoin,NVARS_ADAPT3D) ];
         unkno[ index_2d_to_1d(i,3,npoin,NVARS_ADAPT3D) ] = 3.e7*unkno[ index_2d_to_1d(i,0,npoin,NVARS_ADAPT3D) ];
         unkno[ index_2d_to_1d(i,4,npoin,NVARS_ADAPT3D) ] = 1.e2;
         unkno[ index_2d_to_1d(i,8,npoin,NVARS_ADAPT3D) ] = 1.e6;

#ifdef DIPOLE
         r2 = coord[ index_2d_to_1d(0,i,0,npoin) ]*coord[ index_2d_to_1d(0,i,0,npoin) ]
             +coord[ index_2d_to_1d(1,i,0,npoin) ]*coord[ index_2d_to_1d(1,i,0,npoin) ]
             +coord[ index_2d_to_1d(2,i,0,npoin) ]*coord[ index_2d_to_1d(2,i,0,npoin) ];
         r = sqrt(r2);
         r3 = r*r2;
         r5 = r3*r2;
         m0 = 10.;
         unkno[ index_2d_to_1d(i,5,npoin,NVARS_ADAPT3D) ] = 3.*m0*coord[ index_2d_to_1d(0,i,0,npoin) ]*coord[ index_2d_to_1d(2,i,0,npoin) ]/r5;
         unkno[ index_2d_to_1d(i,6,npoin,NVARS_ADAPT3D) ] = 3.*m0*coord[ index_2d_to_1d(1,i,0,npoin) ]*coord[ index_2d_to_1d(2,i,0,npoin) ]/r5;
         unkno[ index_2d_to_1d(i,7,npoin,NVARS_ADAPT3D) ] = m0*(3.*coord[ index_2d_to_1d(2,i,0,npoin) ]*coord[ index_2d_to_1d(2,i,0,npoin) ]/r2-1.)/r3;
#endif 
         unkno[ index_2d_to_1d(i,0,npoin,NVARS_ADAPT3D) ] = r;

       }

/*       end subroutine setup_test_unkno */
}


/*----------------------------------------------------------------
!
! Routines that perform a smart search for a point in an unstructured
! grid. Smart search is based on the assumption that we already know the
! element that contains a nearby point.
!
!----------------------------------------------------------------*/



int smart_search(float search_point_coords[ndimn])
{
       int lfound, mask[nnode], try_grid_search;

       int  i,j,k,ielem,inode,jnode ;
       int  ifound, jelem, kelem;
       int  node_order[nnode];
       int  i_node, j_node, k_node, k_node_hi;
       int  next_node, i_order;

       int  nelems_checked;
       int  clear_cache;

       float  shapex[nnode];
       float  radius;

       float  distance[nnode];


/*----------------------------------------------------------------
!
! Step A
!
! First check the last_element_found to see if the new point is still
! inside it. If yes, then set ifound=.true'
*/
       ifound = -1;
       if( last_element_found >= 0 ) {
#ifdef DEBUGS
       printf("Checkin if still in last element \n");
#endif

         ifound = chkineln( search_point_coords, last_element_found ,shapex);
         nelems_checked = 1;


       }

/*--------*/
       if( ifound == 0 ) {
/*--------*/

#ifdef DEBUGS
          printf("Point is still in starting element! \n");
#endif
          kelem = last_element_found;

/*--------*/
       } else {
/*--------*/


/* If we have a starting_element number set to begin the search  */
       if( last_element_found >= 0 ) { 


#ifdef DEBUGS
         printf("Point is not still in starting element! \n");
#endif

/*
! 
! Step B
!
! Compute the distances of the new point from each of the nodes of the
! starting_element. 
*/
       for (jnode=0; jnode<nnode; jnode++) {
         mask[jnode]=1;
         inode = intmat[ index_2d_to_1d(last_element_found,jnode,nelem,4) ] -1 ;
         distance[jnode] = 
     (coord[ index_2d_to_1d(0,inode,0,npoin) ]-search_point_coords[0])*(coord[ index_2d_to_1d(0,inode,0,npoin) ]-search_point_coords[0])
   + (coord[ index_2d_to_1d(1,inode,0,npoin) ]-search_point_coords[1])*(coord[ index_2d_to_1d(1,inode,0,npoin) ]-search_point_coords[1])
   + (coord[ index_2d_to_1d(2,inode,0,npoin) ]-search_point_coords[2])*(coord[ index_2d_to_1d(2,inode,0,npoin) ]-search_point_coords[2]) ;
       }

/*
!
! Step C
!
! Sort the starting element nodes based on distance from the new search point
*/
       node_order[0]       = fminloc1d(distance,nnode,mask);
       node_order[nnode-1] = fmaxloc1d(distance,nnode,mask);
       mask[node_order[0]] = 0;                        /* false */
       mask[node_order[nnode-1]] = 0;
       if(nnode == 3) {
         for (j=0; j<nnode; j++) {
           if(mask[j]) node_order[1] = j;
         }
       }
       if(nnode == 4) {
         node_order[1] = fminloc1d(distance,nnode,mask);
         node_order[2] = fmaxloc1d(distance,nnode,mask);
       }
       if(nnode > 4) {
         printf("Error : Code only works for nnode=3 or 4 ! \n");
         exit(EXIT_FAILURE);
       }
       
/*
! Step D
!
! Begin search through the element lists for these nodes
*/

       i_order = -1;
/*++++*/
       while( (ifound != 0) && (i_order < nnode ) ) { 
/*++++*/

        i_order += 1;
        next_node = node_order[i_order];


/* Now we search the list of elements that contain this node */
        inode = intmat[ index_2d_to_1d(last_element_found,next_node,nelem,4) ] -1 ;

#ifdef DEBUGS
        printf("node list for this element is %i %i %i %i \n",
                   intmat[ index_2d_to_1d(last_element_found,0,nelem,4) ]-1,
                   intmat[ index_2d_to_1d(last_element_found,1,nelem,4) ]-1,
                   intmat[ index_2d_to_1d(last_element_found,2,nelem,4) ]-1,
                   intmat[ index_2d_to_1d(last_element_found,3,nelem,4) ]-1);
        printf("First node in list is %i ",inode);
#endif



        k_node    = esup2[inode]   +1 ;
        k_node_hi = esup2[inode+1] +1 ;

        jelem =  esup1[k_node];
        while( (ifound != 0) && (k_node < k_node_hi) ) {

          ifound = chkineln( search_point_coords, jelem ,shapex);

          nelems_checked = nelems_checked + 1;
          if(ifound != 0) {
#ifdef DEBUGS
             printf("Not found in elem %i \n",jelem);
#endif
             k_node += 1;
             jelem =  esup1[k_node];
#ifdef DEBUGS
             printf("Next element to check is %i %i %i \n",jelem,i_node,i_order);
#endif
          }
          if(ifound == 0) {
            kelem = jelem;
          }
#ifdef DEBUGS
          if(ifound == 0) {
              printf("Found in elem %i \n",jelem);
              printf("Found after checking %i elements \n",nelems_checked);
          }
#endif

        }    /* while */


/*++++*/
       }     /* while */
/*++++*/

       }      /*   if( last_element_found .ge. 0 )  */

/*--------*/
       }      /*   if( ifound .eq. 0)  */
/*--------*/

       if( ifound != 0) {
#ifdef DEBUGS
          printf("Smart search failed! \n");
          printf("search_point_coords %e %e %e \n",search_point_coords[0] 
                          ,search_point_coords[1] ,search_point_coords[2]);
#endif

/* Check to see if the point is still within the grid bounds */
          try_grid_search = point_within_grid(search_point_coords);

          kelem=-1;
          if(try_grid_search) {
#ifdef DEBUGS
            radius=sqrt( search_point_coords[0]*search_point_coords[0]+
                         search_point_coords[1]*search_point_coords[1]+
                         search_point_coords[2]*search_point_coords[2] );
            printf("Using grid based search \n");
            printf("search_point_coords %e %e %e \n",search_point_coords[0] 
                          ,search_point_coords[1] ,search_point_coords[2]);
            printf("radius %e \n",radius);
#endif
            clear_cache=1;
            kelem=find_element(search_point_coords,clear_cache);
          }
#ifdef DEBUGS
          if(kelem > 0) {
              printf("Found in element %i \n",kelem);
          } else {
              printf("Failed to locate element in grid \n");
          }
#endif
       }
 
       return kelem;

}




/*------------------------------------------------------------------*/

void smart_search_setup()
{

/*
!
!
! Written:    Original by Hong Luo
!             Modified by Peter MacNeice - Aug 2010
!
*/


       int ip,ie,inode,nstor,ielem,istor;
       int nnodes, ipoin;


/*----------------------------------------------------------------
! Start of smart search setup
!----------------------------------------------------------------*/

       nnodes = nelem*nnode;     /* number of element/node couples */


/*
       esup1   = (int *) malloc(nnodes*sizeof(int));
       esup2   = (int *) malloc((npoin+1)*sizeof(int));
*/

/*
!
!...  this sub finds the elements that surround each point
!
!...  loop over the points, seeing how many elements surround each point
! 
!...  set esup2=0
!
*/

      for( ip=0; ip<npoin+1; ip++) {
        esup2[ip] = 0;
      }

/*
!
!...  loop over the elements, storing 'ahead'
!
*/
      for( ie=0; ie<nelem; ie++) {
      for( inode=0; inode<nnode; inode++) {
        ip         = intmat[ index_2d_to_1d(ie,inode,nelem,4) ];
        esup2[ip]  = esup2[ip] + 1;
      }}
/*
!
!...  reshuffle esup2
!
*/
      for( ip=1; ip<npoin+1; ip++) {
        esup2[ip] = esup2[ip] + esup2[ip-1];
      }

      nstor = esup2[npoin];

/*
!
!...  check
!     
!      if(nstor .gt. mesup) print 1,nstor
!      if(nstor .gt. mesup) stop
!    1 format(' please increase mesup in sub fielsup1',
!     &     /,' needed:',i8)

!
!...  now store the surrounding elements in esup1
!
*/
      for( ielem=0; ielem<nelem; ielem++) {
      for( inode=0; inode<nnode; inode++) {
        ipoin        = intmat[ index_2d_to_1d(ielem,inode,nelem,4) ] -1; 
        istor        = esup2[ipoin] + 1;
        esup2[ipoin] = istor;
        esup1[istor-1] = ielem;
      }}
/*
!
!...  finally, reorder esup2
!
*/
      for( ip=npoin; ip>0; ip--) {
        esup2[ip] = esup2[ip-1];
      }

      esup2[0] = 0;


/*    int nesup    = esup2[npoin];
      printf(" nesup = %i in fielsup1 \n",nesup); */


/* The routine locate_facing_elements takes too long to execute when
    used to optimize support fieldline tracing */
/*      locate_facing_elements; */

/*----------------------------------------------------------------
! End of smart search setup
!----------------------------------------------------------------*/

       }




/*

      subroutine locate_facing_elements
!
!...  this sub finds the elements that are adjacent to
!     each element ==> facing_elements
!
!
!
! Written:    Original by Hong Luo
!             Modified by Peter MacNeice - Aug 2010
!

      use adapt3d_config

      implicit none

      integer :: lhelp(3,4)
      integer :: ielem,inode,ip,ieadj,je
      integer :: in1,in2,in3,ip1,ip2,ip3
      integer :: iesu0 ,iesu1,iesup,icoun

      data lhelp/ 2 , 3 , 4 ,
     &            3 , 1 , 4 ,
     &            4 , 1 , 2 ,
     &            1 , 3 , 2 /

!
!...  initialize facing_elements=0
!


      allocate(facing_elements(nnode,nelem))
      do ielem = 1, nelem
      do inode = 1, nnode
        facing_elements(inode,ielem) = 0
      enddo
      enddo

!...  initialize lpoin
!
      allocate(lpoin(npoin))
      do ip = 1, npoin
        lpoin(ip) = 0
      enddo
!
!...  loop over the elements
!
      do ielem = 1, nelem
!
!...  loop over the faces (nodes)
!
      do inode = 1, nnode
!
!...  store nodes and faces
!
      in1 = lhelp(1,inode)
      in2 = lhelp(2,inode)
      in3 = lhelp(3,inode)

      ip1 = intmat(in1,ielem)
      ip2 = intmat(in2,ielem)
      ip3 = intmat(in3,ielem)
!
!...  mark lpoin
!
      lpoin(ip1) = 1
      lpoin(ip2) = 1
      lpoin(ip3) = 1
!
!...  then the loop over the surrounding elements
!

      iesu0 = esup2(ip1)+1
      iesu1 = esup2(ip1+1)
c
      ieadj = -999
c
      iesup = iesu0
      do while ((iesup .le. iesu1) .and. (ieadj .eq. -999))
        je = esup1(iesup)
        if(je .ne. ielem) then
            icoun = lpoin(intmat(1,je))+lpoin(intmat(2,je))
     &            + lpoin(intmat(3,je))+lpoin(intmat(4,je))
            if(icoun .eq. 3) ieadj=je
        endif
        iesup = iesup+1
      enddo


!
!...  this is the element !
!
      facing_elements(inode,ielem) = ieadj

!
!...  re-mark lpoin
!
      lpoin(ip1) = 0
      lpoin(ip2) = 0
      lpoin(ip3) = 0
!
!...  end of loop over the faces (nodes)
!
      enddo
!
!...  end of loop over the elements
!
      enddo


      return
      end subroutine locate_facing_elements
*/

int point_within_grid( float scoord[3] )
{
/*
!
! This function test to see if the given point (coord) is inside
! the grid bounds. This function requires specific knowledge of the
! grid type and range. It will need a customized function for each model
! used with this search.
*/


      float  radius;
      int within_bounds = 1;

      radius=sqrt(scoord[0]*scoord[0]+scoord[1]*scoord[1]+scoord[2]*scoord[2]);
      if(scoord[0] < xl_sg) within_bounds = 0;
      if(scoord[0] > xr_sg) within_bounds = 0;
      if(scoord[1] < yl_sg) within_bounds = 0;
      if(scoord[1] > yr_sg) within_bounds = 0;
      if(scoord[2] < zl_sg) within_bounds = 0;
      if(scoord[2] > zr_sg) within_bounds = 0;
      if(radius < INNER_RADIUS) within_bounds = 0;
      if(radius > OUTER_RADIUS) within_bounds = 0;


      return within_bounds;

}

int index_2d_to_1d( int i1, int i2, int n1, int n2)
{
/* converts a 2D array index into a flat 1D index */
      int idx = n2*i1 + i2;

      return idx;
}
