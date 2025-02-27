#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "avi_nc.h"
#include "v42.h"
#include "decode.h"

// encodage des composantes du vecteur de mouvement et infos annexes
// MSB iXXXXXXXqqYYYYYY LSB
short int encode_vect( int vx, int vy, int iquant, int intra )
{
if	( intra )
	return( (short)0x8000 );
short code;
code  = ( vx & 0x7F ) << 8;
code |= ( iquant & 0x3 ) << 6;
code |= ( vy & 0x3F );
return code;
}

// calcul du residu d'un bloc a partir d'un predicteur (selon ref, px, py)
// et du bloc source (selon src, x, y) avec quantification selon iquant
void diff_bloc( unsigned char * diff, unsigned char * src, unsigned char * ref,
		   int x, int y, int px, int py, int stride, int iquant )
{
int iy, ax, ad, ar, v;
ad =  x * 3 +  y * stride;	// offset pour diff et src
ar = px * 3 + py * stride;	// offset pour predicteur
for	( iy = 0; iy < 8; ++iy )
	{
	for	( ax = 0; ax < (8*3); ++ax )
		{
		v = (int)src[ad+ax] - (int)ref[ar+ax] + 256;
		if	( v < 0   ) gasp("v < 0");
		if	( v > 511 ) gasp("v > 511");
		diff[ad+ax] = quant[iquant][v];
		}
	ad += stride;
	ar += stride;
	}
}


int diff_bloc_estim(unsigned char * src, unsigned char * ref,
		   int x, int y, int px, int py, int stride )
{
int iy, ax, ad, ar, sum=0;
ad =  x * 3 +  y * stride;	// offset pour diff et src
ar = px * 3 + py * stride;	// offset pour predicteur
for	( iy = 0; iy < 8; ++iy )
	{
	for	( ax = 0; ax < (8*3); ++ax )
		{
		sum += abs((int)src[ad+ax] - (int)ref[ar+ax]);
		}
	ad += stride;
	ar += stride;
	}

	return sum;
}


void estim_movement(unsigned char * src, unsigned char * ref, int x, int y, int breadth, int *px, int *py, dims *dim) {

	//Coord* tab = malloc(sizeof(Coord)*breadth*breadth/2);
	//int c = 0;
	int x_min=x, y_min=y, sum_bloc=0, min_bloc=0x7fffffff;

	for (int i=x-breadth/2; i<x+breadth/2; i++) {
		for(int j=y-breadth/4; j<y+breadth/4; j++) {

			if((i >= 0 && j >= 0) && (i < (dim->wi-8)) && (j < (dim->he-8))){
				// diff & stock
					sum_bloc = diff_bloc_estim(src, ref, x, y, i, j, dim->ll);
					if(sum_bloc < min_bloc){
						min_bloc = sum_bloc;
						x_min=i; y_min=j;
					}	
			
			}
		}
	}

	// px py
	*px = x_min;
	*py = y_min;
}

// encodage d'une trame dans diff[], selon trame courante src[] et trame precedente ref[]
// iquant = indice du quantificateur, breadth = portee de l'estimation de mouvement (en x)
int encode_trame( unsigned char * diff, unsigned char * src, unsigned char * ref,
		  dims * dim, short * vectcode, int force_keyframe, int iquant, int breadth )
{
int x, y;		// origine bloc courant
int ivec = 0;		// indice bloc courant (pour chercher vecteur)
int px, py;		// origine bloc predicteur
int vx, vy;		// vecteur de compensation de mouvement

if	( force_keyframe )	// cas particulier : image clef "intra"
	{
	for	( y = 0; y < dim->he; y += 8 )
		for	( x = 0; x < dim->wi; x += 8 )
			{
			copy_bloc( diff, src, x, y, dim->ll );
			vectcode[ivec++] = encode_vect( 0, 0, 0, 1 );	// signaler bloc intra
			}
	return 0;
	}

// cas general
for	( y = 0; y < dim->he; y += 8 )
	for	( x = 0; x < dim->wi; x += 8 )
		{
		// estimation de mouvement : pourrait se faire ici...
		vx = 0; vy = 0;
		estim_movement(src, ref, x, y, breadth, &px, &py, dim);
		diff_bloc( diff, src, ref, x, y, px, py, dim->ll, iquant );
		vx = px - x; vy = py - y;
		vectcode[ivec++] = encode_vect( vx, vy, iquant, 0 );
		}
return 0;
}

int encode_avi( char * video_in, char * video_out, int iquant, int breadth )
{
avipars s, d;
unsigned char *buf1, *buf2;		// buffers de trame
unsigned char *ref, *new_ref;		// pointeurs sur les buffers
unsigned int ifram;			// index de trame
short * vectcodes;			// table des vecteurs
dims dim;				// dimensions utiles
int qlinaux;				// nombre de lignes auxiliaires
double dqlinaux;			// nombre de lignes auxiliaires (approx)
int keyflag;				// decision image clef (intra)

// ouverture AVI source
s.fnam = video_in;
s.data = NULL;		// memoire pas encore allouee

readAVIheaders( &s );
if	( ( s.wi % 8 ) || ( s.he % 8 ) )
	gasp("dimensions non multiples de 8 : %dx%d", s.wi, s.he );
s.tot = s.ll * s.he;

// calcul du nombre de lignes auxiliaires
// t.q. qlinaux >= ( ( s.wi * s.he ) ) / 64 ) * ( 2 / s.ll )
dqlinaux = (double)(s.he * s.wi);
dqlinaux /= ( 32 * s.ll );
qlinaux = 1 + (int)(dqlinaux - 1e-14);
printf("ajout de %d lignes auxiliaires\n", qlinaux );

// ouverture AVI dest
d = s;
d.fnam = video_out;
d.he += qlinaux;	
d.tot = d.ll * d.he;

writeAVIheaders( &d );

// dimensions 'utiles'
dim.wi = s.wi;
dim.he = s.he;
dim.ll = s.ll;
dim.tot = dim.he * dim.ll;

// allocation des buffers pour ref
buf1 = malloc( dim.tot );
buf2 = malloc( dim.tot );
if	( ( buf1 == NULL ) || ( buf2 == NULL ) )
	gasp("malloc %d", dim.tot );

// allocation du buffer pour sortie
d.data = malloc( d.tot );
if 	( d.data == NULL )
	gasp("malloc %d", d.tot );

// localisation des vecteurs dans le buffer de sortie
vectcodes = (short *)( d.data + s.ll * dim.he );

for	( ifram = 0; ifram < s.nframes; ifram++ )
	{
	// permutation buffers "ping-pong"
	if	( ifram & 1 )
		{ ref = buf1; new_ref = buf2; }
	else	{ ref = buf2; new_ref = buf1; }
	// decision image clef
	if	( ifram == 0 )
		keyflag = 1;
	else	keyflag = 0;
	// traitement d'une trame
	// printf("frame %d\n", ifram );

	printf("."); fflush(stdout);
	readRGBframe( &s );	// trame lue dans s.data
	encode_trame( d.data, s.data, ref, &dim, vectcodes, keyflag, iquant, breadth );
	writeRGBframe( &d );	// d.data va etre utilise
	// decodage pour la prochaine ref
	decode_trame( new_ref, d.data, ref, &dim, vectcodes );
	}

writeAVIindex( &d );
return 0;
}

