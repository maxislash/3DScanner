//***********************************************//
//**main.c***************************************//
//**test4****************************************//
//***********************************************//
//**Created by samir on 22/03/16.****************//
//**Copyright © 2016 samir. All rights reserved.*//
//***********************************************//

#include <GLUT/GLUT.h>
#include <math.h>
#include <stdio.h>

#define LARGEUR_BASE 50
#define HAUTEUR_BASE 20

#define LARGEUR_BRAS_1 150
#define HAUTEUR_BRAS_1 15

#define LARGEUR_BRAS_2 50
#define HAUTEUR_BRAS_2 10

#define TAILLE_CAISSE 10

#define LARGEUR_ECRAN (LARGEUR_BASE + LARGEUR_BRAS_1 + HAUTEUR_BRAS_2 + 50)
#define HAUTEUR_ECRAN (HAUTEUR_BASE + LARGEUR_BRAS_1 + HAUTEUR_BRAS_2 + 50)







int angle1 = 45;
int angle3 = 25;
int angle2 = -70;
int longueur = 50;
int window;
float t1 = -5.0;
float hauteurFil = 0.0;
int y = 1;
int LightPos[4] = {0,0,3,1};
int MatSpec [4] = {1,1,1,1};
double r=0; // Angle de rotatioon
double t=0; // Parametre de translation
double e=0; // Parametre de chagement d'echelle
double b = -5;
double a = 4;
double c = 0;
// Data read from the header of the BMP file
unsigned char header[54]; // Each BMP file begins by a 54-bytes header
unsigned int dataPos;     // Position in the file where the actual data begins
unsigned int width=600;
unsigned int height=480;
int abscam = 0, ordcam=0, prfcam=-15;
unsigned int imageSize;   // = width*height*3
// Actual RGB data
unsigned char * data;




//sean barett stb lib

//Gluproject unproject


int loadBMP_custom(const char * imagepath)
{
    
    FILE * file = fopen(imagepath,"rb");
    if (!file)                              {printf("Image could not be opened\n"); return 0;}
    
    if ( fread(header, 1, 54, file)!=54 ){ // If not 54 bytes read : problem
        printf("Not a correct BMP file\n");
        return 1;
    }
    
    if ( header[0]!='B' || header[1]!='M' ){
        printf("Not a correct BMP file\n");
        return 0;
    }
    // Read ints from the byte array
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);
    
    // Some BMP files are misformatted, guess missing information
    if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos==0)      dataPos=54; // The BMP header is done that way --54
    
    // Create a buffer
    unsigned char data [imageSize];
    
    // Read the actual data from the file into the buffer
    fread(data,1,imageSize,file);
    
    //Everything is in memory now, the file can be closed
    fclose(file);
    return 0;
}

GLuint textureID;









void GestionSouris(int button, int state, int x, int y)
{
    printf("Vous avez appuyé sur le bouton %c \n", button);
    printf("La position de la souris est : ");
   
   
    if(0 < x < width/2)
    {
        if(0 < y < height/2)
        {
            abscam--;
            ordcam++;
            prfcam++;
        }
        else
        {
            abscam++;
            ordcam--;
            prfcam--;
        };
    }
    else
    {
        if(0 < y < height/2)
        {
            abscam++;
            ordcam--;
            prfcam--;
        }
        else
        {
            abscam--;
            ordcam++;
            prfcam++;
        };
    };
    
    
    
    
     printf("%d, %d",x , y);
    
    
    //if (button == GLUT_LEFT_BUTTON && state==GLUT_UP) exit(0);
    
}

void GestionEntreSortieSourie()
{
    
}






void GestionToucheSpecial(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_F1 : abscam=0; break;
        case GLUT_KEY_F2 : ordcam=0; break;
        case GLUT_KEY_F3 : prfcam=-15; break;
        case GLUT_KEY_F4 : printf ("F4 "); break;
        case GLUT_KEY_F5 : printf ("F5 "); break;
        case GLUT_KEY_F6 : printf ("F6 "); break;
        case GLUT_KEY_F7 : printf ("F7 "); break;
        case GLUT_KEY_F8 : printf ("F8 "); break;
        case GLUT_KEY_F9 : printf ("F9 "); break;
        case GLUT_KEY_F10 : printf ("F10 "); break;
        case GLUT_KEY_F11 : printf ("F11 "); break;
        case GLUT_KEY_F12 : printf ("F12 "); break;
        case GLUT_KEY_LEFT : r+=1; break;
        case GLUT_KEY_UP : hauteurFil-=0.1; break;
        case GLUT_KEY_RIGHT : r-=1; break;
        case GLUT_KEY_DOWN : hauteurFil+=1; break;
        case GLUT_KEY_PAGE_UP : printf ("Page up "); break;
        case GLUT_KEY_PAGE_DOWN : printf ("Page down "); break;
        case GLUT_KEY_HOME : printf ("Home "); break;
        case GLUT_KEY_END : printf ("End "); break;
        case GLUT_KEY_INSERT : printf ("Insert "); break;
            
    }
}





void GestionClavier(unsigned char key, int x, int y)
{
    printf("Vous avez appue sur %c \n",key);
    printf("La position de la souris : ");
    printf("%d,%d", x, y);
    
    if (key=='e') a-=1;
    if (key=='a') a+=1;
    if (key=='s') b-=1;
    if (key=='z') b+=1;
    if (key=='d') c-=1;
    if (key=='u') c+=1;
    
}




void InitGL() {
    glClearColor(0.5,0.5,0.5,0); // On definit la couleur d'arriere plan
    glEnable(GL_DEPTH_TEST); // On active le test de profondeur pour que les faces caches ne soit pas affiches
    glEnable(GL_COLOR_MATERIAL); // Pour s'assurer que le colorage est active
    glEnable(GL_TEXTURE_2D); // Activer la texture
    
    GLuint Texture = loadBMP_custom("Image003.bmp");//sa load l'image
    
    glGenTextures(1, &textureID);  // Demande a OpenGL de creer un objet Texture et reserver les ressources pour lui.
    
    glBindTexture(GL_TEXTURE_2D, textureID); // On selectionne la texture courante tout en indiquant a OpenGL que la textre est de 2D
    
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Le filtre corrige la minification quand il y'a plusieur texel dans un pixel
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // Le fitre corrige la magnification quand il un texel conteint pluiseurs pixel
    
    glEnable(GL_COLOR_MATERIAL); // Pour s'assurer que le colorage est active
    
    glEnable(GL_LIGHTING);  // Active l'éclairage
    glEnable(GL_LIGHT0);    // Allume la lumière n°1
}

void Reshape(int width, int height)
{
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45,
                   width/height,
                   0.1,
                   100
                   );
    
    glMatrixMode(GL_MODELVIEW);
}

void Draw()
{
    
    
    
    //glClear efface le contenu des deux buffers
    glClear(GL_COLOR_BUFFER_BIT | // Le buffer GL_COLOR_BUFFER_BIT est un espace memoire ou le dessin va etre cree. Ce contenu ne sera affiche qu'a la fin du dessin
            GL_DEPTH_BUFFER_BIT); // Le buffer GL_DEPTH_BUFFER_BIT gere la profondeur des objet a afficher (Z-buffer)
    glMatrixMode(GL_MODELVIEW); //On definit la matrice de modele des objer comme matrice de travail
    glLoadIdentity(); // On initialise la matrice de modelisation a la matrice identite
    
    //Placer la camera, On utilise la meme matrice GL_MODELVIEW
    gluLookAt(abscam,ordcam,prfcam,0,0,0,0,1,0);
    // gluLookAt definit les parametre de posionnement de la camera:
    // Ses paramemtre sont: Vecteur qui definit le point position de la camera,
    //                      Vecteur qui definit le point que vise la camera,
    //                      Vecteur qui definit la vertical pour la camera
    //Dans l'exemple la camera est positionnee au point (0,0,-10), elle vise le centre (0,0,0) et a pour verticale le vecteur (0,1,0)
    
    
    
    /* Je déplace mon répère initial (actuellement
     en bas à gauche de l'écran) */
    glTranslated(a,b,c);
    glRotated(90,0,1,0);
    glColorMaterial ( GL_FRONT_AND_BACK, GL_EMISSION ) ;
    glEnable ( GL_COLOR_MATERIAL ) ;
    glLightiv(GL_LIGHT0,GL_POSITION,LightPos);
    
    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(1.0, 0.0, 0.0);
    
    glTexCoord2i(0,0);      glVertex3i(-2,1,-2);
    glTexCoord2i(1,0);      glVertex3i(-2,1,2);
    glTexCoord2i(1,1);      glVertex3i(2,1,2);
    glTexCoord2i(0,1);      glVertex3i(2,1,-2);
    
    glTexCoord2i(0,0);      glVertex3i(-2,-0.5,-2);
    glTexCoord2i(1,0);      glVertex3i(-2,-0.5,2);
    glTexCoord2i(1,1);      glVertex3i(2,-0.5,2);
    glTexCoord2i(0,1);      glVertex3i(2,-0.5,-2);
    
    glTexCoord2i(0,0);      glVertex3i(-2,1,2);
    glTexCoord2i(1,0);      glVertex3i(-2,-0.5,2);
    glTexCoord2i(1,1);      glVertex3i(2,-0.5,2);
    glTexCoord2i(0,1);      glVertex3i(2,1,2);
    
    glTexCoord2i(0,0);      glVertex3i(-2,1,-2);
    glTexCoord2i(1,0);      glVertex3i(-2,-0.5,-2);
    glTexCoord2i(1,1);      glVertex3i(2,-0.5,-2);
    glTexCoord2i(0,1);      glVertex3i(2,1,-2);
    
    glTexCoord2i(1,0);      glVertex3i(-2,1,-2);
    glTexCoord2i(1,1);      glVertex3i(-2,-0.5,-2);
    glTexCoord2i(0,1);      glVertex3i(-2,-0.5,2);
    glTexCoord2i(0,0);      glVertex3i(-2,1,2);
    
    glTexCoord2i(1,0);      glVertex3i(2,1,2);
    glTexCoord2i(1,1);      glVertex3i(2,-0.5,2);
    glTexCoord2i(0,1);      glVertex3i(2,-0.5,-2);
    glTexCoord2i(0,0);      glVertex3i(2,1,-2);
    
    glPopMatrix();
    glEnd();
    
    glRotated(r,0,1,0);
    
    // La base
    glScaled(-0.5,1,-0.5);
    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3d(0,1,1);
    
    glTexCoord2i(0,0);      glVertex3i(-1,1,-1);
    glTexCoord2i(1,0);      glVertex3i(-1,1,1);
    glTexCoord2i(1,1);      glVertex3i(1,1,1);
    glTexCoord2i(0,1);      glVertex3i(1,1,-1);
    
    glTexCoord2i(0,0);      glVertex3i(-1,10,-1);
    glTexCoord2i(1,0);      glVertex3i(-1,10,1);
    glTexCoord2i(1,1);      glVertex3i(1,10,1);
    glTexCoord2i(0,1);      glVertex3i(1,10,-1);
    
    glTexCoord2i(0,0);      glVertex3i(-1,1,1);
    glTexCoord2i(1,0);      glVertex3i(-1,1,-1);
    glTexCoord2i(1,1);      glVertex3i(-1,10,-1);
    glTexCoord2i(0,1);      glVertex3i(-1,10,1);
    
    glTexCoord2i(0,0);      glVertex3i(-1,1,-1);
    glTexCoord2i(1,0);      glVertex3i(1,1,-1);
    glTexCoord2i(1,1);      glVertex3i(1,10,-1);
    glTexCoord2i(0,1);      glVertex3i(-1,10,-1);
    
    glTexCoord2i(1,0);      glVertex3i(1,1,1);
    glTexCoord2i(1,1);      glVertex3i(1,1,-1);
    glTexCoord2i(0,1);      glVertex3i(1,10,-1);
    glTexCoord2i(0,0);      glVertex3i(1,10,1);
    
    glTexCoord2i(1,0);      glVertex3i(-1,1,1);
    glTexCoord2i(1,1);      glVertex3i(1,1,1);
    glTexCoord2i(0,1);      glVertex3i(1,10,1);
    glTexCoord2i(0,0);      glVertex3i(-1,10,1);
    glPopMatrix();
    glEnd();
    
    //-------------------------------
    
    glTranslated(0,9,-5);
    glScaled(1,0.5,1);
    glRotated(90,1,0,0);
    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3d(0,0,1);
    
    glTexCoord2i(0,0);      glVertex3i(-1,1,-1);
    glTexCoord2i(1,0);      glVertex3i(-1,1,1);
    glTexCoord2i(1,1);      glVertex3i(1,1,1);
    glTexCoord2i(0,1);      glVertex3i(1,1,-1);
    
    glTexCoord2i(0,0);      glVertex3i(-1,22,-1);
    glTexCoord2i(1,0);      glVertex3i(-1,22,1);
    glTexCoord2i(1,1);      glVertex3i(1,22,1);
    glTexCoord2i(0,1);      glVertex3i(1,22,-1);
    
    glTexCoord2i(0,0);      glVertex3i(-1,1,1);
    glTexCoord2i(1,0);      glVertex3i(-1,1,-1);
    glTexCoord2i(1,1);      glVertex3i(-1,22,-1);
    glTexCoord2i(0,1);      glVertex3i(-1,22,1);
    
    glTexCoord2i(0,0);      glVertex3i(-1,1,-1);
    glTexCoord2i(1,0);      glVertex3i(1,1,-1);
    glTexCoord2i(1,1);      glVertex3i(1,22,-1);
    glTexCoord2i(0,1);      glVertex3i(-1,22,-1);
    
    glTexCoord2i(1,0);      glVertex3i(1,1,1);
    glTexCoord2i(1,1);      glVertex3i(1,1,-1);
    glTexCoord2i(0,1);      glVertex3i(1,22,-1);
    glTexCoord2i(0,0);      glVertex3i(1,22,1);
    
    glTexCoord2i(1,0);      glVertex3i(-1,1,1);
    glTexCoord2i(1,1);      glVertex3i(1,1,1);
    glTexCoord2i(0,1);      glVertex3i(1,22,1);
    glTexCoord2i(0,0);      glVertex3i(-1,22,1);
    glPopMatrix();
    glEnd();
    
    //Cube sur le cote haut gauche en ariere du bras
    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(1.0, 0.0, 0.0);
    
    glTexCoord2i(0,0);      glVertex3i(-2,1,-2);
    glTexCoord2i(1,0);      glVertex3i(-2,1,2);
    glTexCoord2i(1,1);      glVertex3i(2,1,2);
    glTexCoord2i(0,1);      glVertex3i(2,1,-2);
    
    glTexCoord2i(0,0);      glVertex3i(-2,-0.5,-2);
    glTexCoord2i(1,0);      glVertex3i(-2,-0.5,2);
    glTexCoord2i(1,1);      glVertex3i(2,-0.5,2);
    glTexCoord2i(0,1);      glVertex3i(2,-0.5,-2);
    
    glTexCoord2i(0,0);      glVertex3i(-2,1,2);
    glTexCoord2i(1,0);      glVertex3i(-2,-0.5,2);
    glTexCoord2i(1,1);      glVertex3i(2,-0.5,2);
    glTexCoord2i(0,1);      glVertex3i(2,1,2);
    
    glTexCoord2i(0,0);      glVertex3i(-2,1,-2);
    glTexCoord2i(1,0);      glVertex3i(-2,-0.5,-2);
    glTexCoord2i(1,1);      glVertex3i(2,-0.5,-2);
    glTexCoord2i(0,1);      glVertex3i(2,1,-2);
    
    glTexCoord2i(1,0);      glVertex3i(-2,1,-2);
    glTexCoord2i(1,1);      glVertex3i(-2,-0.5,-2);
    glTexCoord2i(0,1);      glVertex3i(-2,-0.5,2);
    glTexCoord2i(0,0);      glVertex3i(-2,1,2);
    
    glTexCoord2i(1,0);      glVertex3i(2,1,2);
    glTexCoord2i(1,1);      glVertex3i(2,-0.5,2);
    glTexCoord2i(0,1);      glVertex3i(2,-0.5,-2);
    glTexCoord2i(0,0);      glVertex3i(2,1,-2);
    
    glPopMatrix();
    glEnd();
    
    glRotated(90,1,0,0);
    glTranslated(0,18,-21);
    glColor3ub(255,255,255);
    
    
    glBegin(GL_LINES);
    
    glVertex2i(0,-17);
    
    glVertex2i(0,hauteurFil);
    
    glEnd();
    
    //cube au bout du fil
    glTranslated(0,hauteurFil,0);
    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(1.0, 0.0, 0.0);
    
    glTexCoord2i(0,0);      glVertex3i(-2,1,-2);
    glTexCoord2i(1,0);      glVertex3i(-2,1,2);
    glTexCoord2i(1,1);      glVertex3i(2,1,2);
    glTexCoord2i(0,1);      glVertex3i(2,1,-2);
    
    glTexCoord2i(0,0);      glVertex3i(-2,-0.5,-2);
    glTexCoord2i(1,0);      glVertex3i(-2,-0.5,2);
    glTexCoord2i(1,1);      glVertex3i(2,-0.5,2);
    glTexCoord2i(0,1);      glVertex3i(2,-0.5,-2);
    
    glTexCoord2i(0,0);      glVertex3i(-2,1,2);
    glTexCoord2i(1,0);      glVertex3i(-2,-0.5,2);
    glTexCoord2i(1,1);      glVertex3i(2,-0.5,2);
    glTexCoord2i(0,1);      glVertex3i(2,1,2);
    
    glTexCoord2i(0,0);      glVertex3i(-2,1,-2);
    glTexCoord2i(1,0);      glVertex3i(-2,-0.5,-2);
    glTexCoord2i(1,1);      glVertex3i(2,-0.5,-2);
    glTexCoord2i(0,1);      glVertex3i(2,1,-2);
    
    glTexCoord2i(1,0);      glVertex3i(-2,1,-2);
    glTexCoord2i(1,1);      glVertex3i(-2,-0.5,-2);
    glTexCoord2i(0,1);      glVertex3i(-2,-0.5,2);
    glTexCoord2i(0,0);      glVertex3i(-2,1,2);
    
    glTexCoord2i(1,0);      glVertex3i(2,1,2);
    glTexCoord2i(1,1);      glVertex3i(2,-0.5,2);
    glTexCoord2i(0,1);      glVertex3i(2,-0.5,-2);
    glTexCoord2i(0,0);      glVertex3i(2,1,-2);
    
    glPopMatrix();
    glEnd();
    
    glutSwapBuffers(); // On echange les buffers pour afficher les dessins
    
    glutPostRedisplay(); // On lui demande de recalculer la scene
    
}




int main(int argc, char *argv[], char *envp[])
{
    
    glutInit(&argc, argv); // On initialise la librairie GLUT
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH); // On definit le mode d'affichage que GLUT va utilser
    glutInitWindowSize(680,440); // Optionnel, On definit la taille de la fenetre du graphique
    window=glutCreateWindow("ma fenetre Open GL avec GLUT"); // On cree la fenetre du graphisme
    
    glutReshapeFunc(Reshape); // On definit la fonction Reshape a utiliser
    glutDisplayFunc(Draw); // On definit la fonction Draw a utiliser
    
    InitGL(); // Initialisation supplementaire de OpenGL
    
    glutKeyboardFunc(GestionClavier); // On definit la fonction qui gere le clavier et la position de la souris
    glutSpecialFunc(GestionToucheSpecial); // Touches special: F1, F2, Ctrl, Shift, Alt ...
    
    glutMouseFunc(GestionSouris); // On definit la fonction qui gere la souris
    //glutEntryFunc(GestionEntreSortieSourie); // On definit la fonction qui gere l"entree et la sorie de la souris
    
    
    glutMainLoop(); // On boucle tand qu'on a pas quitter
    
    return 0;
}