//	Program developed by
//	
//	Informatika Fakultatea
//	Euskal Herriko Unibertsitatea
//	http://www.ehu.eus/if
//
// to compile it: gcc dibujar-puntos.c -lGL -lGLU -lglut
//
// This example shows :the use of GL_POINTS
//


#include <GL/glut.h>
#include <stdio.h>
#include "cargar-triangulo.h"
#include <math.h>

// texturaren informazioa

extern void load_ppm(char *file, unsigned char **bufferptr, int *dimxptr, int * dimyptr);
unsigned char *bufferra;
int dimx,dimy;

hiruki *triangulosptr;
int num_triangles;
int indice=0;
int i;

float q,w,e;

unsigned char * color_textura(float u, float v)
{
/* debe devolver un puntero al pixel adecuado, no al primero!! */
int i= (int)(u*(dimx-1))+(int)((1-v)*(dimy-1))*dimx;
return(bufferra+3*i);
}

void calcularbaricentro(punto *p1,punto *p2,punto *p3,float i,float j,float *q,float *w,float *e){

	*q =((p2->x)*(p3->y)+(p3->x)*j+i*(p2->y)-(p2->x)*j-i*(p3->y)-(p3->x)*(p2->y))/((p2->x)*(p3->y)+(p3->x)*(p1->y)+(p1->x)*(p2->y)-(p2->x)*(p1->y)-(p1->x)*(p3->y)-(p3->x)*(p2->y));

	*w =(i*(p3->y)+(p3->x)*(p1->y)+(p1->x)*j-i*(p1->y)-(p1->x)*(p3->y)-(p3->x)*j)/((p2->x)*(p3->y)+(p3->x)*(p1->y)+(p1->x)*(p2->y)-(p2->x)*(p1->y)-(p1->x)*(p3->y)-(p3->x)*(p2->y));

	*e =((p2->x)*j+i*(p1->y)+(p1->x)*(p2->y)-(p2->x)*(p1->y)-(p1->x)*j-i*(p2->y))/((p2->x)*(p3->y)+(p3->x)*(p1->y)+(p1->x)*(p2->y)-(p2->x)*(p1->y)-(p1->x)*(p3->y)-(p3->x)*(p2->y));

}


void calcularuv(punto *p1, punto *p2, punto *p3, float *u, float *v){
	*u = q*(p1->u) + w*(p2->u) + e*(p3->u);
	*v = q*(p1->v) + w*(p2->v) + e*(p3->v);
}

void calcularinterseccion( punto *sup_ptr, punto *mid_ptr, punto *interseccion, float j){
float x;
float y;
float int1;
float int2=j;
float pen1;
float pen2=0;

float u,v,pen3,int3;

if (sup_ptr->y !=mid_ptr->y){
	if(sup_ptr->x!=mid_ptr->x){
		x=(sup_ptr->x-mid_ptr->x);
		y=(sup_ptr->y-mid_ptr->y);
		pen1= y/x;
		int1=( mid_ptr->y-pen1*mid_ptr->x);

		/*u = sup_ptr->u-mid_ptr->u;
		v = sup_ptr->v-mid_ptr->v;
		pen3 = v/u;
		int3= mid_ptr->v-pen3*mid_ptr->u;*/

		if(pen1-pen2!=0){
			interseccion->x=(int2-int1)/(pen1-pen2);
			interseccion->y=pen1*(interseccion->x)+int1;
		}
	}
	else{ //Recta vertical
		interseccion->x= mid_ptr->x;
		interseccion->y=j;
	}
}
}
void intercambiar( punto *p1, punto *p2){
punto p3;
p3=*p1;
*p1=*p2;
*p2=p3;

}

void dibujar_seg(punto *inter1, punto *inter2, punto *sup, punto *mid, punto *inf){
unsigned char* colorv;
unsigned char r,g,b;
float i;
float u,v;


if (inter1->x > inter2->x){
	intercambiar(inter1, inter2);
} 

//Dibujamos los dos puntos de interseccion.
/*glVertex2d(inter1->x, inter1->y);
glVertex2d(inter2->x, inter2->y);*/

	for (i=(inter1->x);i<=(inter2->x);i++) {
		calcularbaricentro(sup,mid,inf,i,inter2->y,&q,&w,&e); 
		calcularuv(sup,mid,inf,&u,&v);
		
		colorv=  color_textura(u, v); 
			r=colorv[0];
			g=colorv[1];
			b=colorv[2]; 
		  
		glBegin( GL_POINTS ); 
		glColor3ub(r,g,b);
		glVertex3f(i,inter1->y,0.);
		glEnd();
	}

glEnd();
}


static void dibujar_triangulo(hiruki *triangulosptr){
punto int1;
punto int2;
punto int3;
punto sup= triangulosptr->p1;
punto mid= triangulosptr->p2;
punto inf= triangulosptr->p3;

int u;
int v;
unsigned char* colorv;
unsigned char r,g,b;

//Ordenamos los puntos del triangulo.
if (sup.y<mid.y){
	intercambiar(&mid, &sup);
}
if (sup.y<inf.y){
	intercambiar(&inf, &sup);
}
if (mid.y<inf.y){
	intercambiar(&inf, &mid);
}
//----------Casos especiales--------------------
glBegin(GL_POINTS);
//Los 3 puntos son el mismo
if ((sup.x==mid.x && mid.x==inf.x) && (sup.y==mid.y && mid.y==inf.y)){ 
		glBegin(GL_POINTS);
		glColor3ub(0,0,0);
		glVertex2d(sup.x, sup.y);
		glEnd();
}
//Recta horizontal
else if (sup.y==mid.y && mid.y==inf.y){
		glBegin(GL_LINES);	
		glColor3ub(0,0,0);
		glVertex2f(sup.x, sup.y);
		glVertex2f(inf.x, inf.y);
		glEnd();
}
//Recta vertical
else if (sup.x==mid.x && mid.x==inf.x){  
	glBegin(GL_POINTS);
	for (i=sup.y; i>= inf.y; i--){ 
		glColor3ub(0,0,0);
		glVertex2d(sup.x, i);
	}
	glEnd();
}
//Dos puntos iguales
else if ((sup.x==mid.x && sup.y==mid.y)||(sup.x==mid.x && sup.y==mid.y) ){
		glBegin(GL_POINTS);
		glColor3ub(0,0,0);
		glVertex2d(sup.x, sup.y);
		glVertex2d(inf.x, inf.y);
		glEnd();

	
}
else if ((sup.y-mid.y)/(sup.x-mid.x)==(mid.y-inf.y)/(mid.x-inf.x)){ 
		glBegin(GL_LINES);	
		glColor3ub(0,0,0);
		glVertex2f(sup.x, sup.y);
		glVertex2f(inf.x, inf.y);
		glEnd();
}
else{
	//Hay que dividir el triangulo en dos
		if(mid.y == sup.y){ 
			int1 = sup;
			int2 = mid;
			dibujar_seg(&int1,&int2,&sup,&mid,&inf);
		} 
		else{	
			for(i= sup.y;i>= mid.y;i--){
				calcularinterseccion(&sup,&mid,&int1,i);
				calcularinterseccion(&sup,&inf,&int2,i);
				dibujar_seg(&int1,&int2,&sup,&mid,&inf);
			}
		}
		for(i= mid.y;i>=inf.y;i--){
			calcularinterseccion(&mid,&inf,&int1,i);
			calcularinterseccion(&sup,&inf,&int2,i);
			dibujar_seg(&int1,&int2,&sup,&mid,&inf);	
		}	
}	
}


static void marraztu(void)
{
printf("He entrado en marraztu");
float u,v;
float i,j;
unsigned char* colorv;
unsigned char r,g,b;

// borramos lo que haya...
glClear( GL_COLOR_BUFFER_BIT );

glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glOrtho(0.0, 500.0, 0.0, 500.0,-250.0, 250.0);

// por ahora dibujamos todos los pixels de la ventana de 500x500 con el color que devuelve la función color_textura
// pero hay que llamar a la función que dibuja un triangulo con la textura mapeada:

dibujar_triangulo(&triangulosptr[indice]);
/*for (i=0;i<500;i++)
    for (j=0;j<500;j++)
        {
        u = i/500.0;
        v = j/500.0;
        colorv=  color_textura(u, v); // si esta función es correcta se ve la foto en la ventana
        r= colorv[0];
        g=colorv[1];
        b=colorv[2];     
        glBegin( GL_POINTS );
        glColor3ub(r,g,b);
        glVertex3f(i,j,0.);
        glEnd();
        }*/
glFlush();
}

// This function will be called whenever the user pushes one key
static void teklatua (unsigned char key, int x, int y)
{
switch(key)
	{
	case 13: 
		printf ("ENTER: que hay que dibujar el siguiente triángulo.\n");
		/* hacer algo para que se dibuje el siguiente triangulo */
		/*
		indice ++;  // pero si es el último? hay que controlarlo!
		*/
		if (indice!=num_triangles-1){
			indice++;
		}else{
			indice=0;
		}
		marraztu;
		break;
	case 27:  // <ESC>
		exit( 0 );
		break;
	default:
		printf("%d %c\n", key, key );
	}

// The screen must be drawn to show the new triangle
glutPostRedisplay();
}

int main(int argc, char** argv)
{
	
	printf("This program draws points in the viewport \n");
	printf("Press <ESC> to finish\n");
	glutInit(&argc,argv);
	glutInitDisplayMode ( GLUT_RGB );
	glutInitWindowSize ( 500, 500 );
	glutInitWindowPosition ( 100, 100 );
	glutCreateWindow( "GL_POINTS" );
	glutDisplayFunc( marraztu );
	glutKeyboardFunc( teklatua );
	/* we put the information of the texture in the buffer pointed by bufferra. The dimensions of the texture are loaded into dimx and dimy */ 
        load_ppm("foto.ppm", &bufferra, &dimx, &dimy);
        // lee la información de los triángulos... pero hay que definir las variables!
        cargar_triangulos(&num_triangles, &triangulosptr);
        
	
	glClearColor( 0.0f, 0.0f, 0.7f, 1.0f );

	glutMainLoop();

	return 0;   
}
