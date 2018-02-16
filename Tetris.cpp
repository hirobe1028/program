#include "../canvas/canvas.h"
#include <string>
#include <iostream>
#include <windows.h>
#include <sstream>

//block
#define SIZE 20
#define WIDTH 10
#define HEIGHT 20
#define INTERVAL 10

//board
#define X1 30
#define Y1 50
#define W1 SIZE*WIDTH
#define H1 SIZE*HEIGHT
#define X2 X1*2+W1
#define Y2 65
#define W2 SIZE*5
#define H2 SIZE*5

#define ERR -1

BYTE board[WIDTH][HEIGHT]={0};
BYTE block[4][4]={0};
POINT position={0,0};
BYTE next[4][4]={0};
int score = 0;

using namespace canvas;
using namespace std;

void create_board(){
    std::stringstream ss;
    ss << score;
    string str_score = ss.str();
    setColor(0,0,0);
    drawRectangle(X1,Y1,W1,H1);
    drawRectangle(X2,Y2,W2,H2);
    drawString(X1,Y1/2,"SCORE：");
    drawString(X1+50,Y1/2,str_score);
}

void create_block(){
    for(int y=0; y<4; y++){
        for(int x=0; x<4; x++){
            next[x][y]=0;
        }
    }
    switch(rand()%7){
        case 0:
            next[1][1]=1; next[2][1]=1; next[1][2]=1; next[2][2]=1;
            return;
        case 1:
            next[1][0]=1; next[1][1]=1; next[1][2]=1; next[1][3]=1;
            return;
        case 2:
            next[1][1]=1; next[1][2]=1; next[2][2]=1; next[1][3]=1;
            return;
        case 3:
            next[1][1]=1; next[2][1]=1; next[1][2]=1; next[1][3]=1;
            return;
        case 4:
            next[1][1]=1; next[2][1]=1; next[2][2]=1; next[2][3]=1;
            return;
        case 5:
            next[2][1]=1; next[1][2]=1; next[2][2]=1; next[1][3]=1;
            return;
        case 6:
            next[1][1]=1; next[1][2]=1; next[2][2]=1; next[2][3]=1;
            return;
    }
}

void next_block(){
    for(int y=0; y<4; y++){
    	for(int x=0; x<4; x++){
            block[x][y] = next[x][y];
    	}
    }
    position.x = 3;
    position.y = -3;
    create_block();
}

int block_under(){
    for(int y=3; y>=0; y--){
        for(int x=0; x<4; x++){
            if(block[x][y]){
                return y;
            }
        }
    }
    return ERR;
}

int block_left(){
    for(int x=0; x<4; x++){
        for(int y=0; y<4; y++){
            if(block[x][y]){
                return x;
            }
        }
    }
    return ERR;
}

int block_right(){
    for(int x=3; x>=0; x--){
        for(int y=0; y<3; y++){
            if(block[x][y]){
                return x;
            }
        }
    }
    return ERR;
}

BOOL move_block(int move){
    int x,y,left,right,under;
    switch(move){
        case 0:
            left = block_left();
            if((position.x)+left <= 0){
                return FALSE;
            }
            for(y=0; y<4; y++){
                for(x=0; x<4; x++){
                    if(block[x][y] && (position.x)+x-1>=0 && (position.y)+y>=0
                        && board[(position.x)+x-1][(position.y)+y]){
                        return FALSE;
                    }
                }
            }
            position.x--;
            return TRUE;
        case 1:
            right = block_right();
            if((position.x)+right >= WIDTH-1){
                return FALSE;
            }
            for(y=0; y<4; y++){
                for(x=0; x<4; x++){
                    if(block[x][y] && (position.x)+x+1<WIDTH && (position.y)+y>=0
                        && board[(position.x)+x+1][(position.y)+y]){
                        return FALSE;
                    }
                }
            }
            position.x++;
            return TRUE;
        case 2:
            under = block_under();
            if((position.y) + under >= HEIGHT-1){
                return FALSE;
            }
            for(y=0; y<4; y++){
                for(x=0; x<4; x++){
                    if(block[x][y] && (position.y)+y+1>=0 && (position.y)+y+1<HEIGHT
                        && board[(position.x)+x][(position.y)+y+1]){
                        return FALSE;
                    }
                }
            }
            position.y++;
            return TRUE;
    }
    return FALSE;
}


BOOL turn_block(){
    int x,y,xx,yy;
    BYTE turn[4][4];
    for(y=0; y<4; y++){
        for(x=0; x<4; x++){
            turn[3-y][x] = block[x][y];
        }
    }
    for(y=0; y<4; y++){
        for(x=0; x<4; x++){
            if(turn[x][y]){
                xx = position.x + x;
                yy = position.y + y;
                if(xx<0 || xx>=WIDTH || yy>=HEIGHT || (yy>=0 && board[xx][yy])){
                    return FALSE;
                }
            }
        }
    }
    for(y=0; y<4; y++){
        for(x=0; x<4; x++){
            block[x][y] = turn[x][y];
        }
    }
    return TRUE;
}

void box_bottom(){
    for(int y=0; y<4; y++){
        for(int x=0; x<4; x++){
            if(block[x][y] && (position.y)+y>=0){
                board[(position.x)+x][(position.y)+y] = block[x][y];
            }
        }
    }
}

int delete_block(){
    int x,y,delCount = 0;
    for(y=HEIGHT-1; y>=0; y--){
        int lineCount=0;
        for(x=0; x<WIDTH; x++){
            lineCount += board[x][y];
        }
        if(lineCount==0){
            break;
        }
        if(lineCount!=WIDTH){
            continue;
        }
        delCount++;
        for(x=0; x<WIDTH; x++){
            board[x][y] = 0;
        }
    }
    return delCount;
}

void down_block(int delCount){
    for(int i=1; i<=delCount; i++){
		score = score + i*10;
    }
    int x,y;
    for(y=HEIGHT-1; y>=0 && delCount>0;){
        int lineCount=0;
        for(x=0; x<WIDTH; x++){
            lineCount += board[x][y];
        }
        if(lineCount!=0){
            y--;
            continue;
        }
        delCount--;
        for(int i=y; i>=0; i--){
            for(x=0; x<WIDTH; x++){
                if(i-1>=0){
                    board[x][i] = board[x][i-1];
                }else{
                    board[x][0] = 0;
                }
            }
        }
    }
}

void draw(){
    int x,y,ptx,pty;
    for(y=0; y<HEIGHT; y++){
        for(x=0; x<WIDTH; x++){
            ptx = X1+x*SIZE;
            pty = Y1+y*SIZE;
            if(board[x][y]){
            	fillRectangle(ptx,pty,SIZE,SIZE);
            }
        }
    }
    for(y=0; y<4; y++){
        if(position.y+y<0){
            continue;
        }
        for(x=0; x<4; x++){
            ptx = X1+(position.x+x)*SIZE;
            pty = Y1+(position.y+y)*SIZE;
            if(block[x][y]){
                fillRectangle(ptx,pty,SIZE,SIZE);
            }
        }
    }
    for(y=0; y<4; y++){
        for(x=0; x<4; x++){
            ptx = X1+ (WIDTH+2+x)*SIZE;
            pty = Y1+ (y+1)*SIZE;
            if(next[x][y]){
            	fillRectangle(ptx,pty,SIZE,SIZE);
            }
        }
    }
}

bool is_overlaped(){
    for (int y=0; y <4; ++y) {
    	for (int x=0; x <4; ++x) {
    		if( block[x][y] != 0 && board[x+position.x+1][y+position.y+1] != 0 ){
    			return TRUE;
            }
    	}
    }
    return FALSE;
}

void game(){
    int key = 0;
    int keyDown = 0;
    for (int cnt=1; ; ++cnt) {
    	bool update = false;
    	if( cnt % INTERVAL == 0){
    	    if(!move_block(2)){
    	    	key = 0;
    	    	box_bottom();
    	    	down_block(delete_block());
    	    	next_block();
                if( is_overlaped()){
                	return;
                }
                continue;
    	    }
    	    update = true;
    	}
    	if(cnt % INTERVAL == 0){
    		if(key == VK_LEFT){
    			if(move_block(0)){
    				update = true;
    			}
    			key = 0;
    		} else if(key == VK_RIGHT){
    			if(move_block(1)){
    				update = true;
    			}
    			key = 0;
    		} else if(key == VK_DOWN){
    			if(move_block(2)){
    				update = true;
    			}
    			key = 0;
    		}
    	}
    	if( cnt % INTERVAL == 0 ) {
    		if( key == VK_SPACE ) {
    			if(turn_block()) {
    				update = true;
    				key = 0;
                }
    		}
    	}
    	if( update ) {
    		Sleep(100);
    		clear();
    		create_board();
    		draw();
    	}
        if( !keyDown ) {
            if( isKeyPressed(VK_LEFT) ) {
            	key = keyDown = VK_LEFT;
            } else if( isKeyPressed(VK_RIGHT) ) {
                key = keyDown = VK_RIGHT;
            } else if( isKeyPressed(VK_SPACE) ) {
            	key = keyDown = VK_SPACE;
            } else if( isKeyPressed(VK_DOWN) ) {
            	key = keyDown = VK_DOWN;
            }
        } else {
            if( !isKeyPressed(keyDown) ){
                keyDown = 0;
            }
        }
        Sleep(10);
    }
}

int main(){
    show(530,500);
    int x0,y0;
    int tetris[5][23] = {{1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1},
			{1,1,1,0,1,0,0,0,1,1,1,0,1,0,1,0,0,1,0,0,1,0,0},
			{0,1,0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,1,0,0,1,1,1},
			{0,1,0,0,1,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0,0,0,1},
			{0,1,0,0,1,1,1,0,0,1,0,0,1,0,1,0,1,1,1,0,1,1,1}};
    for(int y=0; y<5; y++){
        for(int x=0; x<23; x++){
            x0=(x+1)*SIZE;
            y0=Y1+(y+1)*SIZE;
            if(tetris[y][x]){
                fillRectangle(x0,y0,SIZE,SIZE);
            }
        }
    }
    drawString(200,200,"Enterキーを押すとスタートします");
    for (;;){
    	if( isKeyPressed(VK_RETURN) ) {
    		clear();
    		create_block();
    	    next_block();
    	    while(board[4][0]==0 || board[5][0]==0){
    			game();
   		    }
   		    drawString(X2,300,"GAME OVERです");
   		    drawString(X2,360,"Enterキーを押すと終了します");
   		    if(isKeyPressed(VK_RETURN)){
    			break;
   		    }
    	}
    }
    return 0;
}
