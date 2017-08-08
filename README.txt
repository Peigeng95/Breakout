In the command line argument:
1. run with default: FPS=30 speed level=1
2. run with 1 argument: FPS = argv[1]
3. run with 2 argument: FPS = argv[1] speed = argv[2]
eg: ./a1 50 2
	will run with 50 Fps and 2 speed level.

Note: recommand speed level between 1 to 5.

Contral:
1. the paddle is moving with the mouse. this is the only way to move paddle.
2. type 'q' to quit. also responds on other keypress but only print what you press -- nothing changes in the game.

Game end declearation:
1. lose: ever time you miss the ball and it hit the bottom. you will lose one life, However the ball will bounce back. when your lose all your lives you lose and the game stop. you can press any Key to quit.

2. win: when you break every Brick in the game. the game will stop and print you win. you also can press any Key to quit. 

Note: if your speed level is too fast. it is impossible to win the game. speed level from 1 to 3 the designer ensure you can win the game.( I have tested it).

the bounce rate is 1(x): 5(7)
