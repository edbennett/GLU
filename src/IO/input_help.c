/*
    Copyright 2013 Renwick James Hudspith

    This file (input_help.c) is part of GLU.

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
   @file input_help.c
   @brief provides some help about the input file
 */

#include "Mainfile.h"

// shortcut for equivalent strings
static int
are_equal( const char *str_1 , const char *str_2 ) { return !strcmp( str_1 , str_2 ) ; }

// function for generating example input files
static void
create_input_file( const char *mode_str , const char *gf_str )
{
  printf( "MODE = %s \n" , mode_str ) ;
  printf( "HEADER = NERSC\n" ) ;
  int mu ;
  for( mu = 0 ; mu < ND ; mu++ ) {
    printf( "    DIM_%d = 16\n" , mu ) ;
  }
  printf( "CONFNO = 0\n" 
	  "RANDOM_TRANSFORM = NO\n" 
	  "SEED = 0\n" ) ;
  printf( "GFTYPE = %s\n" , gf_str ) ;
  printf( "    GF_TUNE = 0.08\n" 
	  "    IMPROVEMENTS = NONE\n" 
	  "    ACCURACY = 14\n"
	  "    MAX_ITERS = 8000\n" ) ;
  printf( "CUTTYPE = STATIC_POTENTIAL\n"
	  "    FIELD_DEFINITION = LINEAR\n"
	  "    MOM_CUT = HYPERCUBIC_CUT\n" 
	  "    MAX_T = 7\n"
	  "    MAXMOM = 80\n" 
	  "    CYL_WIDTH = 2.0\n"
	  "    ANGLE = 60\n"
	  "    OUTPUT = ./\n" ) ;
  printf( "SMEARTYPE = ADAPTWFLOW_STOUT\n" 
	  "    DIRECTION = ALL\n"
	  "    SMITERS = 1000\n" ) ;
  printf( "    ALPHA1 = 0.02\n" ) ;
  for( mu = 2 ; mu < ND ; mu++ ) {
    printf( "ALPHA%d = 0.0\n" , mu ) ;
  }
  printf( "U1_MEAS = U1_RECTANGLE\n" 
	  "    U1_ALPHA = 0.07957753876221914\n" 
	  "    U1_CHARGE = -1.0\n" ) ;
  printf( "CONFIG_INFO = GLU_config\n" ) ;
  printf( "    STORAGE = NERSC_NCxNC\n" ) ;
  return ;
}

// possible cut types
static void
cuttype_types( void )
{
  printf( "CUTTYPE = CONFIGSPACE_GLUE           - Computes the configuration space gluon propagator, must be gauge fixed \n" ) ;
  printf( "CUTTYPE = EXCEPTIONAL                - Computes the gluon propagator g(2) and MOMgg-projected gluon 3 point function g(3) to a\n" 
	  "                                       list which has the format (all in Big Endian binary format)\n"
	  "                                       NMOM\n"
	  "                                       ND ( p_0 , p_1 , ... , p_ND-1 )\n" 
	  "                                       NMOM\n"
	  "                                       g(2)( p )\n"
	  "                                       NMOM\n"
	  "                                       g(3)( p )\n" 
	  "                                       where NMOM is the number of the lists of momenta, g(2) and g(3)\n" ) ;
  printf( "CUTTYPE = FIELDS                     - Writes out full NCxNC momentum-space Lie matrices\n" ) ;
  printf( "CUTTYPE = GLUON_PROPS                - Writes the transverse and longitudinal Landau gauge fixed gluon propagators in the output form of \n" 
	  "                                       EXCEPTIONAL\n") ;
  printf( "CUTTYPE = INSTANTANEOUS_GLUONS       - Writes the momentum-spacespatial and temporal gluon propagators of Coulomb gauge, the output\n"
	  "                                       format is like that of EXCEPTIONAL but with momenta list ND-1 ( p_0 , ... , p_{ND-2}\n" ) ;
  printf( "CUTTYPE = NON_EXCEPTIONAL            - Computes the momentum space gluon propagator and 3 point function for gluons with momenta \n"
	  "                                       p + r + q = 0, p!=r!=q, and p^2=r^2=q^2. Writes to an output file in the format of EXCEPTIONAL\n");
  printf( "CUTTYPE = SMEARED_GLUONS             - Computest the unsmeared and smeared Landau gauge fixed propagators, with the same output as\n"
	  "                                       that of EXCEPTIONAL but with the unsmeared propagator and the smeared propagator written.\n"
	  "                                       This requires the smearing arguments to be set in the input file\n" ) ;
  printf( "CUTTYPE = STATIC_POTENTIAL           - Computes the correlator of Polyakov loops C(r,T) = < L(0,T) L(r,T) > , \n"
          "                                       it computes the singlet C_1(r,t) = < Tr( L(0,T) L(r,T) ) > and the qq C_qq(r,t)\n"
	  "                                       qq : C_qq(r,t) = < Tr( L(0,T) ) Tr( L(r,T) ) >. Storage is Big Endian format\n"
	  "                                       binary data, of the form \n"
	  "                                       NR\n"
	  "                                       ND-1 ( r_0 , r_1 , ... , r_ND-2 )\n" 
	  "                                       NT\n"
	  "                                       NMOM\n"
	  "                                       C_1( r , T )\n"
	  "                                       NMOM\n"
	  "                                       C_qq( r , T )\n"
	  "                                       .... NT pairs of V_1 and Vqq ...\n"
	  "                                       where NR is the number of R-vectors from the origin. This code allows for smearing before\n"
	  "                                       the computation of the polyakov loops, specified by the smearing options in the input file\n" ) ;
  printf( "CUTTYPE = TOPOLOGICAL_SUSCEPTIBILITY - Computes the configuration space correlator of topological charges C(r) = < q(0) q(r) > \n" 
	  "                                       storage is similar to EXCEPTIONAL but instead of momenta there are ND r-vectors from the origin\n"
	  "                                       and again singlet and qq correlators. Also performs smearing of the gauge field, specified by\n"
	  "                                       the smearing options in the input file\n" ) ;
  printf( "CUTTYPE = {ALL ELSE} - Do Nothing \n" ) ;
  printf( "\nCONFIGSPACE_GLUE, EXCEPTIONAL, GLUON_PROPS and NON_EXCEPTIONAL require the gauge field to be fixed to Landau gauge\n" ) ;
  printf( "\nINSTANTANEOUS_GLUONS and STATIC_POTENTIAL  require the gauge field to be fixed to Coulomb gauge\n" ) ;
  return ;
}

// available gauge fixing types
static void
gftype_types( void )
{
  printf( "GFTYPE = COULOMB    - Coulomb gauge fixing using a time-slice by time-slice Cornell method\n" ) ;
  printf( "GFTYPE = LANDAU     - Landau gauge fixing using the Cornell method\n" ) ;
  printf( "GFTYPE = {All ELSE} - Do nothing\n" ) ;
  printf( "\nDepending on what configure flags you have specified these will either be Fourier accelerated or not\n" ) ;
}

// available headers
static void
header_types( void )
{
  printf( "HEADER = HIREP       - Attempts to read a file in HiREP format \n" ) ; 
  printf( "       = ILDG_SCIDAC - Attempts to read an ILDG configuration in SCIDAC form and compares checksums \n" ) ;
  printf( "       = ILDG_BQCD   - Attempts to read an ILDG configuration in BQCD's format and compares checksums \n" ) ;
  printf( "       = INSTANTON   - Generates a BPST instanton field with dimensions specified by DIM_ array\n" ) ;
  printf( "       = LIME        - Attempts to read a lime configuration *caution* does not bother with checksums\n" ) ;
  printf( "       = MILC        - Attempts to read a MILC configuration\n" ) ;
  printf( "       = NERSC       - Attempts to read a NERSC configuration\n" ) ;
  printf( "       = RANDOM      - Generates a random configuration with dimensions specified by DIM_ array\n" ) ;
  printf( "       = UNIT        - Generates an identity-matrix configuration with dimensions specified by DIM_ array\n" ) ;
  printf( "\nAll of these configuration types have been generalised to their obvious NC and ND extensions\n" ) ;
}

// typical usage
static void
help_usage( void )
{
  printf( "\nHelp usage : \n\n./GLU --help={options}\n\n" ) ;
  printf( "The available options are:\n" ) ;
  printf( "MODE, HEADER, DIM , CONFNO, RANDOM_TRANSFORM, \n" ) ;
  printf( "GFTYPE, GF_TUNE, IMPROVEMENTS, ACCURACY, MAX_ITERS, \n" ) ;
  printf( "CUTTYPE, FIELD_DEFINITION, MOM_CUT, MAX_T, MAXMOM, CYL_WIDTH, ANGLE, OUTPUT,\n" ) ;
  printf( "SMEARTYPE, DIRECTION, SMITERS, ALPHA,\n" ) ;
  printf( "U1_MEAS, U1_ALPHA, U1_CHARGE,\n" ) ;
  printf( "CONFIG_INFO, STORAGE,\n" ) ;
  printf( "*caution* in the {input_file} each one of these have to be specified\n"
	  "          once and only once!\n" ) ;
  printf( "\nIf you would like an example input file, try:\n"
	  "\n./GLU --autoin={options}\n\n"
	  "Where options can be COULOMB, LANDAU, STATIC_POTENTIAL, SUNCxU1 or WFLOW\n\n" ) ;
}

// possible gauge fixing improvement types
static void
improvement_types( void )
{
  printf( "IMPOVEMENTS = MAG        - Fixes to the so-called maximal axial gauge before fixing to Landau or Coulomb\n" ) ;
  printf( "            = SMEAR      - Uses the smeared preconditioning of Hettrick and de forcrand using the smearing\n"
	  "                           routines specified in the input file\n") ;
  printf( "            = RESIDUAL   - Fixes the remaining temporal degree of freedom after Coulomb gauge fixing\n" ) ;
  printf( "            = {ALL ELSE} - Do nothing\n" ) ;
}

// standard modes available
static void
mode_types( void ) 
{
  printf( "MODE = CUTTING      - Static Potential, Qsusc correlator and momentum-space gluon correlators\n" ) ;
  printf( "     = GAUGE_FIXING - Landau and Coulomb gauge fixing mode selection\n" ) ;
  printf( "     = SMEARING     - Link smearing mode, overwrites the lattice field with the chosen smearing\n" ) ;
  printf( "     = SUNCxU1      - Quenched SU(N)xU(1) configuration generation\n" ) ;
  printf( "     = {ALL ELSE}   - Default behaviour, computes plaquettes and Polyakov loops\n" ) ;
}

// possible momentum cuts available
static void
momcut_types( void )
{
  printf( "MOM_CUT = CONICAL_CUT    - Retain only momenta within a cylinder (that have n^{2} < MAX_MOM) of width CYL_WIDTH that lie\n"
	  "                           along one of the principle diagonals of the momentum-space lattice, also within a cone\n"
	  "                           whose angle at the apex is specified by ANGLE\n" ) ;
  printf( "        = SPHERICAL_CUT  - Retain only momenta that have n^{2} < MAX_MOM\n" ) ;
  printf( "        = CYLINDER_CUT   - Retain only momenta within a cylinder (that have n^{2} < MAX_MOM) of width CYL_WIDTH that lie\n"
	  "                           along one of the principle diagonals of the momentum-space lattice\n" ) ;
  printf( "        = HYPERCUBIC_CUT - Include individual momenta that satisfy |p| < sqrt(MAX_MOM) \n" ) ;
  printf( "        = {ALL ELSE}     - Do nothing \n" ) ;
  printf( "\nn^{2} is (anisotropy corrected) n_{mu}n_{mu} Fourier mode product, for {mu} being polarisations on the lattice\n" ) ;
}

// smearing possibilities
static void
smeartype_types( void )
{
  printf( "SMEARTYPE = ADAPTWFLOW_LOG   - Adaptive RK4 integration of the flow equation using LOG links\n" ) ;
  printf( "          = ADAPTWFLOW_STOUT - Adaptive RK4 integration of the flow equation using STOUT links\n" ) ;
  printf( "          = APE              - APE smearing U' = ( 1 - \\alpha ) U + \\sum staples \n" ) ;
  printf( "          = HEX              - Hypercubically blocked, STOUT smeared links\n" ) ;
  printf( "          = HYL              - Hypercubically blocked, LOG smeared links\n" ) ;
  printf( "          = HYP              - Hypercubically blocked, APE smeared links\n" ) ;
  printf( "          = LOG              - Logarithmic link smearing uses the exact log definition of gauge fields\n" ) ;
  printf( "          = STOUT            - STOUT link smearing uses the Hermitian projection definition of gauge fields\n" ) ;
  printf( "          = WFLOW_LOG        - RK4 integration of the flow equation using LOG links\n" ) ;
  printf( "          = WFLOW_STOUT      - RK4 integration of the flow equation using STOUT links\n" ) ;
  printf( "          = {ALL ELSE}       - Do nothing\n" ) ;
  printf( "\n*caution* Hypercubically blocked variants for ND > 4 use a very slow recursive definition\n" ) ;
  printf( "\n*caution* Log smearing for NC > 3 requires slow series evaluations and can become unstable\n" ) ;
}

// gauge configuration storage possibilities
static void
storage_types( void )
{
  printf( "STORAGE = HIREP       - Writes out a HiREP format file\n" ) ;
  printf( "        = ILDG        - Writes a SCIDAC_ILDG format file\n" ) ;
  printf( "        = MILC        - Writes a MILC configuration file\n" ) ;
  printf( "        = NERSC_SMALL - (Only for NC < 4! ) writes the smallest possible configuration\n" ) ;
  printf( "        = NERSC_GAUGE - Writes out the top NC-1 rows of the link matrices\n" ) ;
  printf( "        = NERSC_NCxNC - Writes out the full link matrices\n" ) ;
  printf( "        = SCIDAC      - Writes a SCIDAC format configuration\n" ) ;
  printf( "        = {ALL ELSE}  - Defaults to the NERSC_NCxNC format\n" ) ;
  printf( "\n*caution* our MILC configurations are not actually MILC standard, as they don't have a double precision\n"
	  "           output at the moment. We will see if that changes.\n" ) ;
  printf( "\n*caution* if SUNCxU1 is the chosen MODE then NERSC_GAUGE and NERSC_SMALL will not be used\n" ) ;
}

// U(1) possible measurements
static void
U1meas_types( void ) 
{
  printf( "U1_MEAS = U1_RECTANGLE   - measures the non-compact and compact U(1) average trace of the 2x1 Wilson loop\n" ) ;
  printf( "        = U1_TOPOLOGICAL - measures some non-compact topological things such as the Dirac sheet and Dirac string\n" ) ;
  printf( "        = {ALL ELSE}     - measures the non-compact and compact U(1) average trace of the plaquette\n" ) ;
  printf( "\nThe SU(NC) gauge fields will be overwritten by SU(NC)xU(1), this means some configuration output types become unavailable\n" ) ;
}

// yep, that is what I chose to call it : Help functions
int
GLU_helps_those_who_help_themselves( const char *help_str )
{
  if( are_equal( help_str , "--help=MODE" ) ) {
    mode_types( ) ;
  } else if( are_equal( help_str , "--help=HEADER" ) ) {
    header_types( ) ;
  } else if( are_equal( help_str , "--help=DIM" ) ) {
    printf( "DIM_%%d = %%d - Specified lattice dimensions for the UNIT, RANDOM and INSTANTON\n"
	    "              types of HEADER. If we are not using these, lattice\n"
	    "              dimensions are taken from the configuration header\n" ) ;
  } else if( are_equal( help_str , "--help=CONFNO" ) ) {
    printf( "CONFNO = %%d - User-specified configuration number. All configuration types except NERSC do not directly provide\n"
	    "              a configuration number. So you should supply one for the output configuration\n" ) ;
  } else if( are_equal( help_str , "--help=RANDOM_TRANSFORM" ) ) {
    printf( "RANDOM_TRANSFORM = YES    - Perform a lattice-wide random gauge transformation of the fields\n"
	    "                 = {!YES} - Do nothing\n" ) ;
  } else if( are_equal( help_str , "--help=SEED" ) ) {
    printf( "SEED = %%d - User specified RNG seed \n" 
	    "        0 - Generates a SEED from urandom if it is available\n") ;
  } else if( are_equal( help_str , "--help=GFTYPE" ) ) {
    gftype_types( ) ;
  } else if( are_equal( help_str , "--help=GFTUNE" ) ) {
    printf( "GFTUNE = %%lf - User specified tuning parameter for the gauge fixing this should generally be < 0.1\n" ) ;
  } else if( are_equal( help_str , "--help=IMPROVEMENTS" ) ) {
    improvement_types( ) ;
  } else if( are_equal( help_str , "--help=ACCURACY" ) ) {
    printf( "ACCURACY = %%d - Stops the gauge fixing after an average accuracy of better than 10^{-ACCURACY} has been achieved\n" ) ;
  } else if( are_equal( help_str , "--help=MAX_ITERS" ) ) {
    printf( "MAX_ITERS = %%d - After this many iterations the routine restarts from the beginning with a random transformation of the\n"
	    "                 fields and after %d restarts it complains about changing the tuning\n" , GF_GLU_FAILURES ) ;
  } else if( are_equal( help_str , "--help=CUTTYPE" ) ) {
    cuttype_types( ) ;
  } else if( are_equal( help_str , "--help=FIELD_DEFINTION" ) ) {
    printf( "FIELD_DEFINITION = LOG        - Exact logarithm definition of the gauge fields\n" ) ;
    printf( "FIELD_DEFINITION = {ALL ELSE} - Hermitian projection definition of the gauge fields (denoted LINEAR)\n" ) ;
  } else if( are_equal( help_str , "--help=MOM_CUT" ) ) {
    momcut_types( ) ;
  } else if( are_equal( help_str , "--help=MAX_T" ) ) {
    printf( "MAX_T = %%d - serves as the maximum T separation for the Polyakov lines\n"
	    "             in the STATIC_POTENTIAL code. Computes T=1,2,...,MAX_T separations\n" ) ;
  } else if( are_equal( help_str , "--help=MAXMOM" ) ) {
    printf( "MAXMOM = %%d - maximum allowed n_{mu}n_{mu} where n_{mu} is a Fourier mode\n" ) ;
  } else if( are_equal( help_str , "--help=CYL_WIDTH" ) ) {
    printf( "CYL_WIDTH = %%lf - momenta are allowed within the body-diagonal cylinder of width CYL_WIDTH * 2 \\pi / L \n"
	    "                  where L is the smallest lattice direction's length\n" ) ;
  } else if( are_equal( help_str , "--help=ANGLE" ) ) {
    printf( "ANGLE = %%lf - conical cut's angle of the apex of the cone\n" ) ;
  } else if( are_equal( help_str , "--help=OUTPUT" ) ) {
    printf( "OUTPUT = %%s - prefix destination for where the output file for the cut procedure will be written. The actual output file\n"
	    "              is generated within the code, the output format requires CONFNO to be set \n" ) ;
  } else if( are_equal( help_str , "--help=SMEARTYPE" ) ) {
    smeartype_types( ) ;
  } else if( are_equal( help_str , "--help=DIRECTION" ) ) {
    printf( "DIRECTION = SPATIAL    - smears the fields for each time-slice only in the ND-1 polarisation directions\n" ) ;
    printf( "          = {ALL ELSE} - fully smears all the links in all the directions\n" ) ;
  } else if( are_equal( help_str , "--help=SMITERS" ) ) {
    printf( "SMITERS = %%d - maximum number of smearing iterations to perform, some routines such as the Wilson flow will often finish\n"
	    "               before this number is reached\n" ) ;
  } else if( are_equal( help_str , "--help=ALPHA" ) ) {
    printf( "ALPHA_%%d = %%lf - Smearing parameters for each level of smearing, expects ND-1 of these\n"
	    "                 e.g. ALPHA_1, ALPHA_2 .. ALPHA_ND-1 for Hypercubically blocked smearing in\n"
	    "                 all directions. For the Wilson flow and for APE, LOG and STOUT smearings ALPHA_1 is the only relevant\n"
	    "                 parameter and all others are ignored\n" ) ;
  } else if( are_equal( help_str , "--help=U1_MEAS" ) ) {
    U1meas_types( ) ;
  } else if( are_equal( help_str , "--help=U1_ALPHA" ) ) {
    printf( "U1_ALPHA = %%lf - The non-compact bare coupling g^{2} / 4\\pi, beta is 1 / ( \\pi ND U1_ALPHA ) \n" ) ;
    printf( "\n*caution* FFTW must be linked\n" ) ; 
  } else if( are_equal( help_str , "--help=U1_CHARGE" ) ) {
    printf( "U1_CHARGE = %%lf - Charges the U(1) fields upon compactification \n" 
	    "                  U(1)_{mu} = exp( i U1_CHARGE sqrt( 4 \\pi U1_ALPHA ) A_{mu} ) \n" ) ;
  } else if( are_equal( help_str , "--help=CONFIG_INFO" ) ) {
    printf( "CONFIG_INFO = %%s - Provide some small details for when we write out the configuration file\n" ) ;
  } else if( are_equal( help_str , "--help=STORAGE" ) ) {
    storage_types( ) ;
  } else if( are_equal( help_str , "--autoin=LANDAU" ) ) {
    create_input_file( "GAUGE_FIXING" , "LANDAU" ) ;
  } else if( are_equal( help_str , "--autoin=COULOMB" ) ) {
    create_input_file( "GAUGE_FIXING" , "COULOMB" ) ;
  } else if( are_equal( help_str , "--autoin=STATIC_POTENTIAL" ) ) {
    create_input_file( "CUTTING" , "LANDAU" ) ;
  } else if( are_equal( help_str , "--autoin=SUNCxU1" ) ) {
    create_input_file( "SUNCxU1" , "LANDAU" ) ;
  } else if( are_equal( help_str , "--autoin=WFLOW" ) ) {
    create_input_file( "SMEARING" , "LANDAU" ) ;
  } else {
    printf( "[IO] Unrecognised {input_file} query \"%s\" for\n" , help_str ) ;
    help_usage() ;
  }
  return GLU_SUCCESS ;
}

// simple usage information
int
GLUsage( void )
{
  printf( "\nTo run the code use (the last two commands are optional):\n\n"
	  "./GLU -i {input_file} -c {config_file} -o {output_config_name} \n" ) ;
  printf( "\nFor help on various {input_file} options use:\n\n"
	  "./GLU --help={input_file option}\n\n" ) ;
  printf( "To automatically generate a standard input file use:\n\n"
	  "./GLU --autoin={options}\n"
	  "\nWhere {options} can be COULOMB, LANDAU, STATIC_POTENTIAL, SUNCxU1, WFLOW\n\n" ) ;
  return printf( "If using the CG gauge fixing, please cite my paper\n"
		 "\"Fourier Accelerated Conjugate Gradient Lattice Gauge Fixing\"\n"
		 "arxiv...\n\n" ) ;
}
