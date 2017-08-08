/*
CS 349 Code Examples: X Windows and XLib

    doublebuffer.cpp   Demos double buffering (using same 
    				   demo as animation.cpp)

- - - - - - - - - - - - - - - - - - - - - -

Commands to compile and run:

    g++ -o doublebuffer doublebuffer.cpp -L/usr/X11R6/lib -lX11 -lstdc++
    ./doublebuffer

Note: the -L option and -lstdc++ may not be needed on some machines.

*/

#include <iostream>
#include <list>
#include <cstdlib>
#include <sys/time.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <sstream>

/*
 * Header files for X functions
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>

using namespace std;
 
const int Border = 5;
const int BufferSize = 10;
int FPS = 30;
int score = 0;
int lives = 3;
int speed = 1;

/*
 * Information to draw on the window.
 */
struct XInfo {
	Display	 *display;
	int		 screen;
	Window	 window;
	GC		 gc[3];
	
	Pixmap	pixmap;		// double buffer
	int		width;		// size of pixmap
	int		height;
};


/*
 * Function to put out a message on error exits.
 */
void error( string str ) {
  cerr << str << endl;
  exit(0);
}


/*
 * An abstract class representing displayable things. 
 */
class Displayable {
	public:
		virtual void paint(XInfo &xinfo) = 0;
		virtual bool isV()=0;
};       


class Paddle : public Displayable {
	public:
		virtual void paint(XInfo &xinfo) {
			XSetForeground(xinfo.display, xinfo.gc[1], XWhitePixel(xinfo.display, xinfo.screen));
            		XSetBackground(xinfo.display, xinfo.gc[1], XBlackPixel(xinfo.display, xinfo.screen));
			XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[1], look_x, y, width, height);						
		}
		
		void lookat(int x) {
			look_x = x;
		}
      
	// constructor
	Paddle(int x, int y, int width, int height):x(x), y(y), width(width), height(height)  {
		look_x = x;
		visiable =true;
	}
      		int getX() {
			return look_x;
		}
		
		int getY() {
			return y;
		}
		bool isV(){
			return visiable;
		}
	private:
		int x;
		int y;
		int width;
		int height;
		int look_x;
		bool visiable;
};

class Ball : public Displayable {
	public:
		virtual void paint(XInfo &xinfo) {
			XFillArc(xinfo.display, xinfo.pixmap, xinfo.gc[1], x, y, diameter, diameter, 0, 360*64);
		}
		
		void move(XInfo &xinfo) {
			if (score == 250) throw(3);
			x = x + directionX;
			y = y - directionY;
			if (x <= 0 || x > xinfo.width - diameter) {
				directionX = -directionX;
			}
			if (y < 0){
				directionY = -directionY;
			}
			if (y > xinfo.height - diameter) {
				directionY = -directionY;
				throw (1);
			}
		}
		void reverse() {
			directionY = -directionY;
		}
		int getX() {
			return x;
		}
		
		int getY() {
			return y;
		}
		
		Ball(int x, int y, int diameter): x(x), y(y), diameter(diameter) {
			directionX = 1 * speed;
			directionY = 5 * speed;
			visiable = true;
		}
		bool isV() {
			return visiable;
		}
	private:
		int x;
		int y;
		int diameter;
		int directionX;
		int directionY;
		bool visiable;
};


class Brick : public Displayable {
	public:
		virtual void paint(XInfo &xinfo) {
			XSetForeground(xinfo.display, xinfo.gc[1], colour);
            		XSetBackground(xinfo.display, xinfo.gc[1], XBlackPixel(xinfo.display, xinfo.screen));
			XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[1], x, y, width, height);						
		}
		Brick(int x, int y, unsigned long colour): x(x), y(y), colour(colour)	{
			width = 30;
			height = 15;
			visiable = true;
		}		
		int getX() {
			return x;
		}
		int getY() {
			return y;
		}
		int getW() {
			return width;
		}
		int getH() {
			return height;
		}
		bool isV(){
			return visiable;
		}
		void notV(){
			visiable = false;
		}
	private:
		int x;
		int y;
		int width;
		int height;
		bool visiable;
		unsigned long colour;
};


list<Displayable *> dList;           // list of Displayables
Paddle paddle(250, 560, 100,10);
Ball ball(300, 500, 10);

list<Brick *>bList;
Brick brick1_1(30, 60, 0xFF0000);
Brick brick1_2(90, 60, 0xFF0000);
Brick brick1_3(150, 60, 0xFF0000);
Brick brick1_4(210, 60, 0xFF0000);
Brick brick1_5(270, 60, 0xFF0000);
Brick brick1_6(330, 60, 0xFF0000);
Brick brick1_7(390, 60, 0xFF0000);
Brick brick1_8(450, 60, 0xFF0000);
Brick brick1_9(510, 60, 0xFF0000);
Brick brick1_10(570, 60, 0xFF0000);
Brick brick2_1(30, 120, 0xFFA500);
Brick brick2_2(90, 120, 0xFFA500);
Brick brick2_3(150, 120, 0xFFA500);
Brick brick2_4(210, 120, 0xFFA500);
Brick brick2_5(270, 120, 0xFFA500);
Brick brick2_6(330, 120, 0xFFA500);
Brick brick2_7(390, 120, 0xFFA500);
Brick brick2_8(450, 120, 0xFFA500);
Brick brick2_9(510, 120, 0xFFA500);
Brick brick2_10(570, 120, 0xFFA500);
Brick brick3_1(30, 180, 0xFFFF00);
Brick brick3_2(90, 180, 0xFFFF00);
Brick brick3_3(150, 180, 0xFFFF00);
Brick brick3_4(210, 180, 0xFFFF00);
Brick brick3_5(270, 180, 0xFFFF00);
Brick brick3_6(330, 180, 0xFFFF00);
Brick brick3_7(390, 180, 0xFFFF00);
Brick brick3_8(450, 180, 0xFFFF00);
Brick brick3_9(510, 180, 0xFFFF00);
Brick brick3_10(570, 180, 0xFFFF00);
Brick brick4_1(30, 240, 0x00FF00);
Brick brick4_2(90, 240, 0x00FF00);
Brick brick4_3(150, 240, 0x00FF00);
Brick brick4_4(210, 240, 0x00FF00);
Brick brick4_5(270, 240, 0x00FF00);
Brick brick4_6(330, 240, 0x00FF00);
Brick brick4_7(390, 240, 0x00FF00);
Brick brick4_8(450, 240, 0x00FF00);
Brick brick4_9(510, 240, 0x00FF00);
Brick brick4_10(570, 240, 0x00FF00);
Brick brick5_1(30, 300, 0x0000FF);
Brick brick5_2(90, 300, 0x0000FF);
Brick brick5_3(150, 300, 0x0000FF);
Brick brick5_4(210, 300, 0x0000FF);
Brick brick5_5(270, 300, 0x0000FF);
Brick brick5_6(330, 300, 0x0000FF);
Brick brick5_7(390, 300, 0x0000FF);
Brick brick5_8(450, 300, 0x0000FF);
Brick brick5_9(510, 300, 0x0000FF);
Brick brick5_10(570, 300, 0x0000FF);
/*
 * Initialize X and create a window
 */
void initX(int argc, char *argv[], XInfo &xinfo) {
	XSizeHints hints;
	unsigned long white, black;

   /*
	* Display opening uses the DISPLAY	environment variable.
	* It can go wrong if DISPLAY isn't set, or you don't have permission.
	*/	
	xinfo.display = XOpenDisplay( "" );
	if ( !xinfo.display )	{
		error( "Can't open display." );
	}
	
   /*
	* Find out some things about the display you're using.
	*/
	xinfo.screen = DefaultScreen( xinfo.display );

	white = XWhitePixel( xinfo.display, xinfo.screen );
	black = XBlackPixel( xinfo.display, xinfo.screen );

	hints.x = 100;
	hints.y = 100;
	hints.width = 630;
	hints.height = 600;
	hints.flags = PPosition | PSize;

	xinfo.window = XCreateSimpleWindow( 
		xinfo.display,				// display where window appears
		DefaultRootWindow( xinfo.display ), // window's parent in window tree
		hints.x, hints.y,			// upper left corner location
		hints.width, hints.height,	// size of the window
		Border,						// width of window's border
		black,						// window border colour
		white );					// window background colour
		
	XSetStandardProperties(
		xinfo.display,		// display containing the window
		xinfo.window,		// window whose properties are set
		"x3_doublebuffer",		// window's title
		"Animate",			// icon's title
		None,				// pixmap for the icon
		argv, argc,			// applications command line args
		&hints );			// size hints for the window

	/* 
	 * Create Graphics Contexts
	 */
	int i = 0;
	xinfo.gc[i] = XCreateGC(xinfo.display, xinfo.window, 0, 0);
	XSetForeground(xinfo.display, xinfo.gc[i], BlackPixel(xinfo.display, xinfo.screen));
	XSetBackground(xinfo.display, xinfo.gc[i], WhitePixel(xinfo.display, xinfo.screen));
	XSetFillStyle(xinfo.display, xinfo.gc[i], FillSolid);
	XSetLineAttributes(xinfo.display, xinfo.gc[i],
	                     1, LineSolid, CapButt, JoinRound);

	// Reverse Video
	i = 1;
	xinfo.gc[i] = XCreateGC(xinfo.display, xinfo.window, 0, 0);
	XSetForeground(xinfo.display, xinfo.gc[i], WhitePixel(xinfo.display, xinfo.screen));
	XSetBackground(xinfo.display, xinfo.gc[i], BlackPixel(xinfo.display, xinfo.screen));
	XSetFillStyle(xinfo.display, xinfo.gc[i], FillSolid);
	XSetLineAttributes(xinfo.display, xinfo.gc[i],
	                     1, LineSolid, CapButt, JoinRound);

	i = 2;
	xinfo.gc[i] = XCreateGC(xinfo.display, xinfo.window, 0, 0);
	XSetForeground(xinfo.display, xinfo.gc[i], WhitePixel(xinfo.display, xinfo.screen));
	XSetBackground(xinfo.display, xinfo.gc[i], BlackPixel(xinfo.display, xinfo.screen));
        XSetLineAttributes(xinfo.display, xinfo.gc[i],
                             1, LineSolid, CapButt, JoinRound);

	int depth = DefaultDepth(xinfo.display, DefaultScreen(xinfo.display));
	xinfo.pixmap = XCreatePixmap(xinfo.display, xinfo.window, hints.width, hints.height, depth);
	xinfo.width = hints.width;
	xinfo.height = hints.height;


	XSelectInput(xinfo.display, xinfo.window, 
		ButtonPressMask | KeyPressMask | PointerMotionMask 
		| EnterWindowMask | LeaveWindowMask
		| StructureNotifyMask   // for resize events
		);

	/*
	 * Don't paint the background -- reduce flickering
	 */
	XSetWindowBackgroundPixmap(xinfo.display, xinfo.window, None);

	/*
	 * Put the window on the screen.
	 */
	XMapRaised( xinfo.display, xinfo.window );
	
	XFlush(xinfo.display);
}

/*
 * Function to repaint a display list
 */
void repaint( XInfo &xinfo) {
	list<Displayable *>::const_iterator begin = dList.begin();
	list<Displayable *>::const_iterator end = dList.end();

	list<Brick *>::const_iterator beginb = bList.begin();
	list<Brick *>::const_iterator endb = bList.end();
	// draw into the buffer
	// note that a window and a pixmap are “drawables”
	XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0], 
		0, 0, xinfo.width, xinfo.height);
	while( begin != end ) {
		Displayable *d = *begin;
		d->paint(xinfo); // the displayables know about the pixmap
		begin++;
	}
	while( beginb != endb ) {
		Brick *b = *beginb;
		if( b->isV() ){
			b->paint(xinfo); // the displayables know about the pixmap
		}
		beginb++;
	}
	string text = "your Score:";
	XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[1], 5, 20, text.c_str(), text.length());
	stringstream ss;
        ss << score;
        string t_score;
        ss >> t_score;
        XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[1], 100, 20, t_score.c_str(), t_score.length());
	
	string life = "lives:";
	XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[1], 150, 20, life.c_str(), life.length());
	stringstream ss2;
        ss2 << lives;
        string t_lives;
        ss2 >> t_lives;
        XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[1], 200, 20, t_lives.c_str(), t_lives.length());
	// copy buffer to window
	XCopyArea(xinfo.display, xinfo.pixmap, xinfo.window, xinfo.gc[0], 
		0, 0, xinfo.width, xinfo.height,  // region of pixmap to copy
		0, 0); // position to put top left corner of pixmap in window

	XFlush( xinfo.display );

}


void handleButtonPress(XInfo &xinfo, XEvent &event) {
	printf("Got button press!\n");
	// dList.push_front(new Text(event.xbutton.x, event.xbutton.y, "Urrp!"));
	// repaint( dList, xinfo );
	
}

void handleKeyPress(XInfo &xinfo, XEvent &event) {
	KeySym key;
	char text[BufferSize];
	
	/*
	 * Exit when 'q' is typed.
	 * This is a simplified approach that does NOT use localization.
	 */
	int i = XLookupString( 
		(XKeyEvent *)&event, 	// the keyboard event
		text, 					// buffer when text will be written
		BufferSize, 			// size of the text buffer
		&key, 					// workstation-independent key symbol
		NULL );					// pointer to a composeStatus structure (unused)
	if ( i == 1) {
		printf("Got key press -- %c\n", text[0]);
		if (text[0] == 'q') {
			error("Terminating normally.");
		}
	}
}

void handleMotion(XInfo &xinfo, XEvent &event, int inside) {
	if (inside) {
		paddle.lookat(event.xbutton.x);
	}
}

void handleResize(XInfo &xinfo, XEvent &event) {
	XConfigureEvent xce = event.xconfigure;
	fprintf(stderr, "Handling resize  w=%d  h=%d\n", xce.width, xce.height);
	if (xce.width > xinfo.width || xce.height > xinfo.height) {
		XFreePixmap(xinfo.display, xinfo.pixmap);
		int depth = DefaultDepth(xinfo.display, DefaultScreen(xinfo.display));
		xinfo.pixmap = XCreatePixmap(xinfo.display, xinfo.window, xce.width, xce.height, depth);
		xinfo.width = xce.width;
		xinfo.height = xce.height;
	}
}

void handleAnimation(XInfo &xinfo, int inside) {
	try{	
		ball.move(xinfo);
	}
	catch(int drop){
		if(drop == 3) throw (3);
		if(drop == 1){
			lives--;
			if (lives == 0) throw (1);
		}
		else throw (0);
			
	}
}

void GameOver(XInfo &xinfo, int gameover) {
   XClearWindow(xinfo.display, xinfo.window);

   string overtype;
   if(gameover == 1) {
      overtype = "GAME OVER";
   }
   else {
      overtype = " YOU WIN";
   }
   XDrawString(xinfo.display, xinfo.window, xinfo.gc[1], 250, 400, overtype.c_str(), overtype.length());


   string t = "Score : ";
   XDrawString(xinfo.display, xinfo.window, xinfo.gc[1], 250, 450, t.c_str(), t.length());

   stringstream ss;
   ss << score;
   string t_score;
   ss >> t_score;
   XDrawString(xinfo.display, xinfo.window, xinfo.gc[1], 300, 450, t_score.c_str(), t_score.length());


   string gg = "Press Any to Quit";
   XDrawString(xinfo.display, xinfo.window, xinfo.gc[1], 250, 500, gg.c_str(), gg.length());

   XFlush(xinfo.display);
   
   XEvent event;
   while(true) {
      XNextEvent(xinfo.display, &event);
      if(event.type == KeyPress) exit(0);
   }
}


unsigned long now() {
	timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}

void eventLoop(XInfo &xinfo) {
	// Add stuff to paint to the display list
	dList.push_front(&ball);
	dList.push_front(&paddle);
	bList.push_front(&brick1_1);
	bList.push_front(&brick1_2);
	bList.push_front(&brick1_3);
	bList.push_front(&brick1_4);
	bList.push_front(&brick1_5);
	bList.push_front(&brick1_6);
	bList.push_front(&brick1_7);
	bList.push_front(&brick1_8);
	bList.push_front(&brick1_9);
	bList.push_front(&brick1_10);
	bList.push_front(&brick2_1);
	bList.push_front(&brick2_2);
	bList.push_front(&brick2_3);
	bList.push_front(&brick2_4);
	bList.push_front(&brick2_5);
	bList.push_front(&brick2_6);
	bList.push_front(&brick2_7);
	bList.push_front(&brick2_8);
	bList.push_front(&brick2_9);
	bList.push_front(&brick2_10);
	bList.push_front(&brick3_1);
	bList.push_front(&brick3_2);
	bList.push_front(&brick3_3);
	bList.push_front(&brick3_4);
	bList.push_front(&brick3_5);
	bList.push_front(&brick3_6);
	bList.push_front(&brick3_7);
	bList.push_front(&brick3_8);
	bList.push_front(&brick3_9);
	bList.push_front(&brick3_10);
	bList.push_front(&brick4_1);
	bList.push_front(&brick4_2);
	bList.push_front(&brick4_3);
	bList.push_front(&brick4_4);
	bList.push_front(&brick4_5);
	bList.push_front(&brick4_6);
	bList.push_front(&brick4_7);
	bList.push_front(&brick4_8);
	bList.push_front(&brick4_9);
	bList.push_front(&brick4_10);
	bList.push_front(&brick5_1);
	bList.push_front(&brick5_2);
	bList.push_front(&brick5_3);
	bList.push_front(&brick5_4);
	bList.push_front(&brick5_5);
	bList.push_front(&brick5_6);
	bList.push_front(&brick5_7);
	bList.push_front(&brick5_8);
	bList.push_front(&brick5_9);
	bList.push_front(&brick5_10);
	XEvent event;
	unsigned long lastRepaint = 0;
	int inside = 0;

	while( true ) {
		/*
		 * This is NOT a performant event loop!  
		 * It needs help!
		 */
		if (XPending(xinfo.display) > 0) {
			XNextEvent( xinfo.display, &event );
			switch( event.type ) {
				case ButtonPress:
					handleButtonPress(xinfo, event);
					break;
				case KeyPress:
					handleKeyPress(xinfo, event);
					break;
				case MotionNotify:
					handleMotion(xinfo, event, inside);
					break;
				case EnterNotify:
					//cout << "enter" << endl;
					inside = 1;
					break;
				case LeaveNotify:
					//cout << "leave" << endl;
					inside = 0;
					break;
				case ConfigureNotify:
					handleResize(xinfo, event);
					break;
			}
		}
		else{
			usleep(1000000/FPS);
			try{
				handleAnimation(xinfo, inside);
			}
			catch(int drop){
				throw drop;
			}
			if(ball.getY() == 560){
				//cout << paddle.getX()<<endl;
				//cout << paddle.getY()<<endl;
				//cout << ball.getX()<<endl;
				//cout << ball.getY()<<endl;
				if( (paddle.getX() <= ball.getX()) && (paddle.getX()+100 >= ball.getX())){
					ball.reverse();
				}
			}
			if(ball.getY() <= 400){
				list<Brick *>::const_iterator beginb = bList.begin();
				list<Brick *>::const_iterator endb = bList.end();
				while( beginb != endb ) {
					//cout << "bListloop running" <<endl;
					Brick *b = *beginb;
						if( (ball.getX() + 5 >= b->getX()) && 
					    	    (b->getX() + 35 >= ball.getX()) && 
					            (b->getY() + 20 >= ball.getY()) &&
					            (b->getY() - 5 <= ball.getY()) && 
					     	    	b->isV() ) {
							b->notV();
							ball.reverse();
							score = score + 5;
							//cout << b->isV()<<endl;				
					}
					beginb++;
				}
				
			}
			repaint(xinfo);
		}
	}
}


/*
 * Start executing here.
 *	 First initialize window.
 *	 Next loop responding to events.
 *	 Exit forcing window manager to clean up - cheesy, but easy.
 */
int main ( int argc, char *argv[] ) {
	if (argc == 1){
		FPS = 30;
		speed = 2;
	}
	else if (argc == 2){
		stringstream ss;
        	ss << argv[1];
        	ss >> FPS;
		//cout << "2 " << FPS<< endl;
	}
	else if (argc == 3){
		stringstream ss;
        	ss << argv[1];
        	ss >> FPS;
		stringstream ss1;
		ss1 << argv[2];
		ss1 >> speed;
		//cout << "3 " <<(int)argv[1]<< (int)argv[2] << endl;

	}
	cout << FPS <<  " FPS" << endl;
	cout << speed<< " speed level" << endl;
 	XInfo xinfo;

	initX(argc, argv, xinfo);
	try{
		eventLoop(xinfo);
	}
	catch(int gameover){
		if ( gameover == 1 || gameover == 3) {
			printf("GAME OVER\n");
			GameOver(xinfo, gameover);
		}
	}
	XCloseDisplay(xinfo.display);
}
