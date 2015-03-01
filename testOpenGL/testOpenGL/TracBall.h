#include <cstdlib>
#include <cmath>
#include <string>
#define M_PI 3.1415926

class TracBall
{
	public:
		TracBall();
		~TracBall();
		void Start(int x, int y, int width, int height);
		void Stop(int x, int y);
		void Motion(int x, int y, int width, int height);
		void Update(void);
		void Apply(void);
	
	private:
		double  RotMat[16];
		float	lastPos[3];
		float 	angle, axis[3], trans[3];
		int		curx, cury;
		int		startX, startY;
		bool 	trackingMouse;
		bool    trackballMove;

	protected:
		void trackball_ptov(int x, int y, int width, int height, float v[3]);

};

