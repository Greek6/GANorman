#include "algorithm.h"

#include "tournamentSelection.h"
#include "RouletteWheelSelection.h"
#include "doubleEliminationTournament.h"

// benchmark sequences for the 2d HP model
// 0 = hydrophil, "white"
// 1 = hydrophob, "black"
// source: Ron Unger, John Moult: Genetic Algorithms for Protein Folding Simulations,
//         Journal of Molecular Biology, Vol. 231, No. 1, May 1993

std::string SEQ20 = "10100110100101100101";
std::string SEQ24 = "110010010010010010010011";
std::string SEQ25 = "0010011000011000011000011";
std::string SEQ36 = "000110011000001111111001100001100100";
std::string SEQ48 = "001001100110000011111111110000001100110010011111";
std::string SEQ50 = "11010101011110100010001000010001000101111010101011";
std::string SEQ60 = "001110111111110001111111111010001111111111110000111111011010";	// highest:	36
std::string SEQ64 = "1111111111110101001100110010011001100100110011001010111111111111";	// highest:	42
std::string SEQ85 = "1111000011111111111100000011111111111100011111111111100011111111111100010011001100101"; // highest: 53
std::string SEQ100 = "0001100111100111011011011110000000011111100111111000000000101101111111111100111011010010111000000111"; // highest: 50
std::string SEQ100_2 = "0000001011000001110111110110000110011011111011111111110110111111100000000000111111100101110000001011"; // highest: 48


Vector3f eyePos(0,0,5);
Vector3f centerPos(0,0,0);
bool fullscreen = false;

Algorithm myAlgorithm;

void Animate (int value);
void RenderScene();
void Reshape(int width, int height);
void keyPressed(unsigned char key, int mousePosX, int mousePosY);
void init(void);


void keyPressed(unsigned char key, int mousePosX, int mousePosY)
{
	#pragma region handleKeyboardInput
    if (key == 'r')
    {
        if (fullscreen)
        {
            glutReshapeWindow(glutGet(GLUT_INIT_WINDOW_WIDTH),glutGet(GLUT_INIT_WINDOW_HEIGHT));
            glutPositionWindow(glutGet(GLUT_INIT_WINDOW_X),glutGet(GLUT_INIT_WINDOW_Y));
            fullscreen = false;
        }
        else
        {
            glutFullScreen();
            fullscreen = true;
        }
    }
    else if (key == 'w')
	{
        eyePos.y += 3;
        centerPos.y += 3;
	}
    else if (key == 's')
	{
        eyePos.y += -3;
        centerPos.y += -3;
	}
    else if (key == 'a')
	{
        eyePos.x += -3;
        centerPos.x += -3;
	}
    else if (key == 'd')
	{
        eyePos.x += 3;
        centerPos.x += 3;
	}
	else if (key==27) // ESC
	{
		exit(0);
	}
    else if (key == 'k')
    {
        eyePos.z += 3;
    }
    else if (key == 'l')
    {
        eyePos.z += -3;
    }
	else if (key == 'o')
	{
		eyePos.y += +3;
	}
	else if (key == 'p')
	{
		eyePos.y += -3;
	}
	#pragma endregion
}

void Animate (int value)    
{
    
	glutPostRedisplay();
    
	glutTimerFunc(10, Animate, ++value); // ms  
}

void RenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity ();
	
	// Kameraposition ist dynamisch
	gluLookAt ( eyePos.x,  eyePos.y,  eyePos.z,
               centerPos.x,  centerPos.y,  centerPos.z, 
               0., 1., 0.);

	Population& myPopulation = myAlgorithm.getPopulation();

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	//myPopulation.draw(30,5);
    myPopulation.getBestFolding().draw(0, 0);

	glDisable(GL_CULL_FACE);

    glutSwapBuffers();
	glFlush();
}

void Reshape(int width, int height)
{
    GLfloat aspectRatio;
    
	// Matrix f¸r Transformation: Frustum->viewport 
	glMatrixMode(GL_PROJECTION); 
    
	// Aktuelle Transformations-Matrix zuruecksetzen 
	glLoadIdentity ();
    
	// Viewport definieren
	glViewport(0, 0, width, height);
    
	// Frustum definieren (siehe unten) 
    
	aspectRatio = (GLfloat)width/(GLfloat)height;
    
    /*if (width <= height) 
    {
        glOrtho( -2., 2., -2. / aspectRatio , 2. / aspectRatio , 0.0, 150);
    }
    else
    {
        glOrtho( -2. * aspectRatio , 2. * aspectRatio , -2., 2., 0.0, 150);
    }*/
    
    gluPerspective(90.f, aspectRatio, 0.1f, 150.0f);
    
	// Matrix f¸r Modellierung/Viewing 
	glMatrixMode(GL_MODELVIEW);
}

void init(void)
{
    glClearColor(0,0,1, 1.0 );
	glEnable(GL_DEPTH_TEST);

	//myAlgorithm.run(SEQ50, 5000, 250, 0.08f, 0.30f, new DoubleEliminationTournament(2,80));
    myAlgorithm.run(SEQ20, 5000, 250, 0.08f, 0.30f, new RouletteWheelSelection());
    //myAlgorithm.run(SEQ50, 5000, 250, 0.04f, 0.25f); // good values for SEQ50

	//myAlgorithm.browsePopulation(std::cout);

	glClearDepth(1);
}

int main(int argc, char **argv)
{
    
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
    unsigned int width = 800;
    unsigned int height = 600;
    
	glutInitWindowPosition(glutGet(GLUT_SCREEN_WIDTH)/2-width/2,glutGet(GLUT_SCREEN_HEIGHT)/2-height/2);
	glutInitWindowSize(width,height);
	glutCreateWindow("Norman Ackermann");
    
    glutDisplayFunc(RenderScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(keyPressed);
    
	// TimerCallback registrieren; wird nach 10 msec aufgerufen mit Parameter 0  
	glutTimerFunc(10, Animate, 0);
    
	init();

	glutMainLoop();
    
    
}