#include "folding.h"


Folding::Folding(std::string &sequence)
	: m_Fitness(0),
	m_Overlaps(0)
{
	for (unsigned int i = 0; i < sequence.size(); ++i)
	{

		Element newElement;
		newElement.setDirection(static_cast<Direction>(rand()%3));
		if (sequence.at(i)=='1')
		{
			newElement.setHydrophob(true);
		}
		m_Elements.push_back(newElement);
	}

	calculatePositionsAndDirections();
	calculateFitnessAndOverlaps();
}

Folding::~Folding()
{
}

int Folding::getFitness(void) 
{
	return m_Fitness;
}

Element& Folding::getElement(unsigned int index)
{
    return m_Elements.at(index);
}

unsigned long Folding::getSize(void)
{
    return m_Elements.size();
}

int Folding::getOverlaps(void)
{
    return m_Overlaps;
}

void Folding::browse(std::ostream &outputStream) 
{
	for (unsigned int i=0;i<m_Elements.size();++i)
	{
		if (m_Elements.at(i).getDirection()==Left)
		{
			outputStream << "Links ";
		}
		else if (m_Elements.at(i).getDirection()==Right)
		{
			outputStream << "Rechts ";
		}
		else
		{
			outputStream << "Geradeaus ";
		}

		if (m_Elements.at(i).isHydrophob())
		{
			outputStream << "Black - Hydrophob(1) ";
		}
		else
		{
			outputStream << "White - Hydrophil(0) ";
		}

		outputStream << m_Elements.at(i).getPosition().x << "/" << m_Elements.at(i).getPosition().y;
		outputStream << " Index: " << i;
		outputStream << std::endl;
	}
	outputStream << "Overlaps: " << m_Overlaps << std::endl;
	outputStream << "Fitness: " << m_Fitness << std::endl;
}

void Folding::calculatePositionsAndDirections(void) 
{
	for (unsigned int i = 0; i < m_Elements.size()-1; ++i)
	{
		Vector2i position;
		ViewingDirection viewingDirection = m_Elements.at(i).getViewingDirection();
		Direction direction = m_Elements.at(i).getDirection();

		if (viewingDirection == East && direction == Straight)
		{
			position.x = m_Elements.at(i).getPosition().x + 1;
			position.y = m_Elements.at(i).getPosition().y ;
			m_Elements.at(i+1).setViewingDirection(East);
		}
		else if (viewingDirection == East && direction == Right)
		{
			position.x = m_Elements.at(i).getPosition().x ;
			position.y = m_Elements.at(i).getPosition().y - 1;
			m_Elements.at(i+1).setViewingDirection(South);
		}
		else if (viewingDirection == East && direction == Left)
		{
			position.x = m_Elements.at(i).getPosition().x ;
			position.y = m_Elements.at(i).getPosition().y + 1;
			m_Elements.at(i+1).setViewingDirection(North);
		}
		else if (viewingDirection == North && direction == Straight)
		{
			position.x = m_Elements.at(i).getPosition().x;
			position.y = m_Elements.at(i).getPosition().y + 1;
			m_Elements.at(i+1).setViewingDirection(North);
		}
		else if (viewingDirection == North && direction == Right)
		{
			position.x = m_Elements.at(i).getPosition().x + 1;
			position.y = m_Elements.at(i).getPosition().y;
			m_Elements.at(i+1).setViewingDirection(East);
		}
		else if (viewingDirection == North && direction == Left)
		{
			position.x = m_Elements.at(i).getPosition().x - 1;
			position.y = m_Elements.at(i).getPosition().y;
			m_Elements.at(i+1).setViewingDirection(West);
		}
		else if (viewingDirection == South && direction == Straight)
		{
			position.x = m_Elements.at(i).getPosition().x;
			position.y = m_Elements.at(i).getPosition().y - 1;
			m_Elements.at(i+1).setViewingDirection(South);
		}
		else if (viewingDirection == South && direction == Right)
		{
			position.x = m_Elements.at(i).getPosition().x - 1;
			position.y = m_Elements.at(i).getPosition().y;
			m_Elements.at(i+1).setViewingDirection(West);
		}
		else if (viewingDirection == South && direction == Left)
		{
			position.x = m_Elements.at(i).getPosition().x + 1;
			position.y = m_Elements.at(i).getPosition().y;
			m_Elements.at(i+1).setViewingDirection(East);
		}
		else if (viewingDirection == West && direction == Straight)
		{
			position.x = m_Elements.at(i).getPosition().x - 1;
			position.y = m_Elements.at(i).getPosition().y;
			m_Elements.at(i+1).setViewingDirection(West);
		}
		else if (viewingDirection == West && direction == Right)
		{
			position.x = m_Elements.at(i).getPosition().x;
			position.y = m_Elements.at(i).getPosition().y + 1;
			m_Elements.at(i+1).setViewingDirection(North);
		}
		else if (viewingDirection == West && direction == Left)
		{
			position.x = m_Elements.at(i).getPosition().x;
			position.y = m_Elements.at(i).getPosition().y - 1;
			m_Elements.at(i+1).setViewingDirection(South);
		}
		m_Elements.at(i+1).setPosition(position);
	}
}

void Folding::calculateFitnessAndOverlaps(void) 
{
	m_Fitness = 0;
	m_Overlaps = 0;

	for (unsigned int i=0;i<m_Elements.size();++i)
	{
		Vector2i &actualPosition = m_Elements.at(i).getPosition();
		for (unsigned int j=0;j<m_Elements.size();++j)
		{
			if (i==j)	// Keine Elemente mit sich selber pruefen !
			{
				continue;
			}
			Vector2i &position = m_Elements.at(j).getPosition();

			if ((actualPosition.x == position.x) &&		// Ueberlappungen berechnen
				(actualPosition.y == position.y))
			{
				++m_Overlaps;
			}

			if (m_Elements.at(i).isHydrophob())		// Wenn hydrophob dann direkt Fitness berechnen ( f�r jeden j durchgang geht er dann hier rein )
			{  /// hier direkt auch j hydrophob noch

				if  ((i == j-1) ||	// Wenn j sowieso Sequenznachbar ist brauchen wir ihn auch �berhaupt nicht pr�fen, somit sind Sequenznachbaren auch ausgeschlossen
					 (i == j+1))
				{
					continue;
				}
				else
				{
					if ((actualPosition.x-1 == position.x)	&&			// Links schauen, Theoretisch m�sste man je nach Viewing Direktion nur Oben/unten oder Links/Rechts schauen
						(actualPosition.y == position.y)	&&			// Ich ver�ndere auf meinem Hydrophoben Element die Position immer um 1 und schau dann ob ich auf einem anderen Hydrophoben Element liege
						(m_Elements.at(j).isHydrophob()))
					{
						++m_Fitness;
					}
					else if ((actualPosition.x+1 == position.x) &&		// Rechts schauen
							 (actualPosition.y == position.y)	&&
							 (m_Elements.at(j).isHydrophob()))
					{
						++m_Fitness;
					}
					else if ((actualPosition.y+1 == position.y) &&		// Oben schauen
							 (actualPosition.x == position.x)	&&
							 (m_Elements.at(j).isHydrophob()))
					{
						++m_Fitness;
					}
					else if ((actualPosition.y-1 == position.y) &&		// Unten schauen
							 (actualPosition.x == position.x)	&&
							 (m_Elements.at(j).isHydrophob()))
					{
						++m_Fitness;
					}
				}
			}
		}
	}
	m_Fitness/=2;
	m_Overlaps/=2;

	m_Fitness/=m_Overlaps+1; // Fitness schlechter bewerten wenn Ueberlappungen vorhanden sind
}

void Folding::draw(int offsetX, int offsetY) 
{
	Vector2i position;
	float sphereRadius = 0.25f;


	for (unsigned int i=0; i<m_Elements.size(); ++i) {

		// Kugeln ( Hydrophobe, Hyrophile ) zeichnen

		position = m_Elements.at(i).getPosition();
		position.x += offsetX;
		position.y += offsetY;
		glPushMatrix();
		if (m_Elements.at(i).isHydrophob()) {
			glColor3f(0, 0, 0);
		}
		else
		{
			glColor3f(1, 1, 1);
		}
		glTranslatef((GLfloat)position.x, (GLfloat)position.y , 0);
		glutSolidSphere(sphereRadius, 5, 5);	// um leistung zu sparen
		//glutSolidCube(0.25);
		glPopMatrix();

		if (i==m_Elements.size()-1) { // F�r das letzte Element muss ich keine Verbindung zeichnen
			continue;
		}

		// Verbindungen zeichnen

		ViewingDirection viewingDirection = m_Elements.at(i).getViewingDirection();
		Direction direction = m_Elements.at(i).getDirection();


		glPushMatrix();
		glTranslatef((GLfloat)position.x, (GLfloat)position.y, 0);
		glColor3f(0, 1, 0);
		if (((viewingDirection == East) && (direction == Left)) ||
			((viewingDirection == North) && (direction == Straight)) ||
			((viewingDirection == West) && (direction == Right)))
		{
			glPushMatrix();
			glRotatef(90, 0, 0, 1); // Hoch
			glTranslatef(0.5, 0, 0);
			glScalef((GLfloat)((1/sphereRadius)*sphereRadius), 0.1f, 0.1f);
			glutSolidCube(1);
			glPopMatrix();
		}
		else if (((viewingDirection == East) && (direction == Straight)) ||
			((viewingDirection == South) && (direction == Left)) ||
			((viewingDirection == North) && (direction == Right)))
		{
			glPushMatrix();         // Rechts
			glTranslatef(0.5, 0, 0);
			glScalef((GLfloat)((1/sphereRadius)*sphereRadius), 0.1f, 0.1f);
			glutSolidCube(1);
			glPopMatrix();
		}
		else if (((viewingDirection == East) && (direction == Right)) ||
			((viewingDirection == West) && (direction == Left)) ||
			((viewingDirection == South) && (direction == Straight)))
		{
			glPushMatrix();
			glRotatef(270, 0, 0, 1); // Runter
			glTranslatef(0.5, 0, 0);
			glScalef((GLfloat)((1/sphereRadius)*sphereRadius), 0.1f, 0.1f);
			glutSolidCube(1);
			glPopMatrix();
		}
		else if (((viewingDirection == West) && (direction == Straight)) ||
			((viewingDirection == North) && (direction == Left)) ||
			((viewingDirection == South) && (direction == Right)))
		{
			glPushMatrix();
			glRotatef(180, 0, 0, 1); // links
			glTranslatef(0.5, 0, 0);
			glScalef((GLfloat)((1/sphereRadius)*sphereRadius), 0.1f, 0.1f);
			glutSolidCube(1);
			glPopMatrix();
		}

		glPopMatrix();

	}
}