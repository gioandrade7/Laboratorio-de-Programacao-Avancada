#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <GL/glut.h>
#include <time.h>

typedef struct ponto{
    float x;
    float y;
}ponto;

typedef struct segmento{
    ponto p1;
    ponto p2;
}segmento;

int largura;
int altura;
int n_segs;
segmento *v;

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    for(int i = 0; i<n_segs; i++){
        glColor3f(0.0, 0.0, 1.0); // Seta a cor do seg. (Red, Green, Blue, entre 0.0 e 1.0)
        glBegin(GL_LINES); // Indica que um segmento será iniciado
        glVertex2f(v[i].p1.x, v[i].p1.y); // Seta a posição inicial do segmento (inteiros)
        glVertex2f(v[i].p2.x, v[i].p2.y); // Seta a posição final do segmento (inteiros)
        glEnd(); 
    }
 
    glFlush();
}

bool ccw(ponto a, ponto b, ponto c){
    return ((c.y - a.y)*(b.x - a.x)) > ((b.y - a.y)*(c.x - a.x));
}

bool intersect(ponto a, ponto b, ponto c, ponto d){
    return (ccw(a,c,d) != ccw(b,c,d)) && (ccw(a,b,c) != ccw(a,b,d));
}

int main(int argc, char **argv){
    glutInit(&argc, argv);
    srand(time(NULL));
    largura = atoi(argv[1]);
    altura = atoi(argv[2]);
    n_segs = atoi(argv[3]);
    bool flag = false;

    v = malloc(n_segs*sizeof(segmento));

    segmento s;
    segmento s2;
    for(int i = 0; i<n_segs; i++){
        s.p1.x = (((float)rand())/RAND_MAX)*largura;
        s.p1.y = (((float)rand())/RAND_MAX)*altura;
        s.p2.x = (((float)rand())/RAND_MAX)*largura;
        s.p2.y = (((float)rand())/RAND_MAX)*altura;

        for(int j = 0; j < i; j++){
            s2 = v[j];
            if(intersect(s.p1, s.p2, s2.p1, s2.p2)){
                flag = true;
                i--;
                break;
            }
        }

        if(!flag){
            v[i] =s;
        }
        flag = false;
    }
    
    glutInitDisplayMode ( GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH );
    // Cria uma janela de tamanho “largura” x “altura”
    glutInitWindowSize(largura, altura);
    glutCreateWindow ("Segmentos Aleatorios");
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, largura, 0.0, altura, -1.0, 1.0);
    // Seta a cor do fundo da janela
    glClearColor(0.5, 0.5, 0.5, 0.5);
    // Seta a função “display” como sendo a função que irá pintar a janela (infinitamente)
    glutDisplayFunc(display);
    glutMainLoop();

    free(v);
}