/*
    Copyright 2013 Renwick James Hudspith

    This file (MOMgg.c) is part of GLU.

    GLU is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    GLU is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GLU.  If not, see <http://www.gnu.org/licenses/>.
*/
/**
   @file MOMgg.c
   @brief computation of the MOMgg two and three point functions

   @warning Expects A to be Landau gauge fixed Lie fields
 */


#include "Mainfile.h"

#include "cut_output.h" // output file formatting
#include "geometry.h"   // general geometry for a flattened 1D array
#include "lie_mats.h"   // Lie algebra calculations

//#define LIE_PROJECTION

/**
  I provide three (equivalent) contractions
  The first is based on matrix products, the second traces and the final lie elements
  They all compute the contraction
  G^{(3)} = p_{\rho}/p^2 * ( g_{\mu\nu} - p_{\mu}p_{\nu}/p^2 ) * G_{\mu\nu\rho)(-p,0,p)
 */
static double complex
contraction_matrices( A , posit , conj , lzero_mat , mom , spsq )
     const struct site *__restrict A ;
     const int posit ; 
     const int conj ;
     const int lzero_mat ;
     const double mom[ ND ] ;
     const double spsq ;
{
  // Zero_Mat all of these matrices
  GLU_complex AA[NCNC] = {} , AM[ NCNC ] = {} ;
  GLU_complex Arho[NCNC] = {} , AP[ NCNC ] = {} ;
  GLU_complex temp[ NCNC ] ; 
  int mu ;
  for( mu = 0 ; mu < ND ; mu++ ) {
    // AA is g_{\mu\nu}A_\mu(p) A_\nu(-p)
    multab( temp , A[ posit ].O[mu] , A[ conj ].O[mu] ) ;
    a_plus_b( AA , temp ) ;
    // Arho portion
    a_plus_Sxb( Arho , A[ lzero_mat ].O[mu] , mom[mu] ) ;
    // AP portion
    a_plus_Sxb( AP , A[ posit ].O[mu] , mom[mu] ) ;
    // AM portion
    a_plus_Sxb( AM , A[ conj ].O[mu] , -mom[mu] / spsq ) ;
  }
  GLU_complex WIV , TRANS ;
  trace_ab( &TRANS , AA , Arho ) ;
  trace_abc( &WIV , Arho , AP , AM ) ;
  return (double complex)( TRANS + WIV ) / spsq ;
}

// contraction using the lie matrices
static double complex
contraction_lies( A , posit , conj , lzero_mat , mom , spsq )
     const struct site *__restrict A ;
     const int posit ; 
     const int conj ;
     const int lzero_mat ;
     const double mom[ ND ] ;
     const double spsq ;
{
  double complex res = 0.0 , tr ;
  int mu , nu , rho ;
  for( mu = 0 ; mu < ND ; mu++ ) {
    for( nu = 0 ; nu < ND ; nu++ ) {
      for( rho = 0 ; rho < ND ; rho++ ) {
	// OK so the vertex should be proportional to if^{abc} unless we messed up somewhere
	// I checked the symmetric piece and it averages to 0
	//tr = ifabc_dabc_ABC( A[ posit ].O[mu] , A[ conj ].O[nu] , A[ lzero_mat ].O[rho] ) ;
	//tr = dabc_ABC( A[ posit ].O[mu] , A[ conj ].O[nu] , A[ lzero_mat ].O[rho] ) ;
	tr = ifabc_ABC( A[ posit ].O[mu] , A[ conj ].O[nu] , A[ lzero_mat ].O[rho] ) ;
	tr = (mu!=nu) ? (-mom[mu]*mom[nu]/spsq)*mom[rho]*tr : (1.0-mom[mu]*mom[nu]/spsq)*mom[rho]*tr ;
	res += tr ;
      }
    }
  }
  return res / ( 4.0 * spsq ) ;
}

// contraction using the trace of three matrices identity
static double complex
contraction_traces( A , posit , conj , lzero_mat , mom , spsq )
     const struct site *__restrict A ;
     const int posit ; 
     const int conj ;
     const int lzero_mat ;
     const double mom[ ND ] ;
     const double spsq ;
{
  const double invspsq = spsq == 0.0 ? 1.0 : 1.0 / spsq ;
  double complex res = 0.0 ;
  GLU_complex tr ;
  int mu , nu , rho ;
  for( mu = 0 ; mu < ND ; mu++ ) {
    for( nu = 0 ; nu < ND ; nu++ ) {
      for( rho = 0 ; rho < ND ; rho++ ) {
	trace_abc( &tr , A[ posit ].O[mu] , A[ conj ].O[nu] , A[ lzero_mat ].O[rho] ) ;
	tr = (mu!=nu) ? (-mom[mu]*mom[nu]*invspsq)*mom[rho]*tr : (1.0-mom[mu]*mom[nu]*invspsq)*mom[rho]*tr ;
	res += tr ;
      }
    }
  }
  return res * invspsq ;
}

// compute psq
static double
psq_calc( double mom[ ND ] ,
	  const int posit )
{
  int n[ ND ] , mu ;
  TwoPI_mpipi_momconv( n , posit , ND ) ;
  
  register double tspsq = 0. ;
  for( mu = 0 ; mu < ND ; mu++ ) {
    #ifdef SIN_MOM
    mom[ mu ] = 2.0 * sin( n[ mu ] * Latt.twiddles[ mu ] * 0.5 ) ;
    #else
    mom[ mu ] = k[ mu ] * Latt.twiddles[ mu ] ;
    #endif
    tspsq += mom[ mu ] * mom[ mu ] ;
  }
  if( tspsq < PREC_TOL ) { tspsq = 1. ; }
  return tspsq ;
}

// Exceptional triple gluon three point function
int
write_exceptional_g2g3_MOMgg( FILE *__restrict Ap , 
			      const struct site *__restrict A , 
			      const struct veclist *__restrict list , 
			      int num_mom[1] )
{  
#ifdef LIE_PROJECTION
  init_generators() ;
  compute_fs_and_ds() ;
#endif

  const double g2_norm = 2.0 / ( ( NCNC - 1 ) * ( ND - 1 ) * LVOLUME ) ;
  const double g0_norm = 2.0 / ( ( NCNC - 1 ) * ( ND ) * LVOLUME ) ;
  const double g3_norm = 4.0 / ( 2.0 * NC * ( NCNC - 1 ) * ( ND - 1 ) *	\
				 LVOLUME ) ;

  const int posit_zero_mat = ( num_mom[0] - 1 ) / 2 ;
  const int lzero_mat = list [ posit_zero_mat ].idx ;
  
  double *g2 = malloc( num_mom[0] * sizeof( double ) ) ;
  double *g3 = malloc( num_mom[0] * sizeof( double ) ) ;

  int i ;
  #pragma omp parallel for private(i)
  PFOR( i = 0 ; i < num_mom[0] ; i++ ) {
    const int posit = list[i].idx ;
    const int conj =  list[ num_mom[0] - i - 1].idx ;
    
    // Two point function calc
    int mu ;
    g2[i] = 0. ;
    for( mu = 0 ; mu < ND ; mu++ ) {	  
      GLU_complex t ;
      trace_ab( &t , A[ posit ].O[ mu ] , A[ conj ].O[mu] ) ;
      g2[i] += (double)creal( t ) ;
    }

    if( likely( i != posit_zero_mat ) ) {
      g2[i] *= g2_norm ;
    } else {
      g2[i] *= g0_norm ;
    }

    // Three point function calc
    double mom[ ND ] ;
    // make this a constant
    const double spsq = psq_calc( mom , posit ) ;

    // helpful little enum
    enum { MATRIX_CONTRACTION , TRACE_CONTRACTION , LIE_CONTRACTION } ; 
    #ifdef LIE_PROJECTION
    const int contraction = LIE_CONTRACTION ;
    #else
    const int contraction = MATRIX_CONTRACTION ; // is the fastest !!
    //const int contraction = TRACE_CONTRACTION ;
    #endif    

    switch( contraction ) 
      {
      case TRACE_CONTRACTION :
	g3[i] = creal( contraction_traces( A , posit , conj , lzero_mat , mom , spsq ) ) * g3_norm ;
	break ;
      case LIE_CONTRACTION :
	g3[i] = creal( contraction_lies( A , posit , conj , lzero_mat , mom , spsq ) ) * g3_norm ;
	break ;
      default :
	g3[i] = creal( contraction_matrices( A , posit , conj , lzero_mat , mom , spsq ) ) * g3_norm ;
	break ;
      }
        
    // end of parallel contractions loop
  }
  // call our writer ...
  write_g2g3_to_list( Ap , g2 , g3 , num_mom ) ;

  free( g2 ) ;
  free( g3 ) ;

#ifdef LIE_PROJECTION
  free_f_and_d( ) ;
  free_generators( ) ;
#endif

  return GLU_SUCCESS ;
}

#ifdef LIE_PROJECTION
  #undef LIE_PROJECTION
#endif
