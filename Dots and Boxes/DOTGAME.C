/* dotgame.c */

# include <graphics.h>
# include <stdlib.h>
# include <stdio.h>
# include <conio.h>
# include <dir.h>
# include <dos.h>

void clearscreen();
void tbox(int,int,int,int,int,int,int);
void tbar(int,int,int,int,int,int);
void hline(int,int,int,int,int);
void vline(int,int,int,int,int);
void dwindow(int,int,int,int,char*,int,int,int,int);
void xyprint(int,int,char*,int,int);
void singlemenuline(int,int,int);
void text_color(int,int);
void cursor_off();
void cursor_on();
void inittextscreen();
void beep();
void togglesound();
void msgbox(char*,char*,int);
void current_directory();
void credits();
void wait();
void titlescreen();
void game();
void menuscreen();
void getkey();
void getmenu();
void getrows();
void getplayernames();
void gamescreen();
void print_dots();
void check_for_boxes();
void print_player();
void print_details();
void initialize_vars();
void quit();
void help();
void initialize_graphics();
void gbox(int,int,int,int,int);
void initialize_mouse();
void show_mouse_ptr();
void hide_mouse_ptr();
void get_mouse_pos();
void set_mouse_pos(int,int);
void set_mouse_horiz_limits(int,int);
void set_mouse_vert_limits(int,int);
int  menu(char[][20],int,int,int,char[][50]);
int  spin(int,int,int,int,int,int,int,int,int);
int  editstr(char*,int,int,int,int,int,int);
int  intlen(int);
int  getscan();
int  getascii();
int  getyesno(int,int);

int  SINGLE_BORDER=0,DOUBLE_BORDER=1;
int  MAXPADS=4,NORMAL=0,EDGED=1,SOUND_ON=1;
int  ASCIIC,SCANC,ESCC=-1,HELPMSG_LINE=25;
int  MAXX,MAXY,theight,twidth;
int  mousex,mousey,mousebutton;
int  LEFTMOUSE=1,RIGHTMOUSE=2;
int  matrix_stx=40,matrix_sty=70,matrix_dis=20;
int  ROWS=10,MINROWS=3,MAXROWS=18,PLAYER=1;
int  t,i,j;
char pl1name[13]="Player 1",pl2name[13]="Player 2";
char tpl1name[13],tpl2name[13];
char CURDIR[MAXPATH];
union REGS ii,oo;

int playerlines[2];
int playerboxes[2];
int playercolor[2]={2,3};
int matrixh[19][19];
int matrixv[19][19];
int matrixb[19][19];

char menustr[][20]={
					"Start the Game",
					"Help",
					"Sound On",
					"-",
					"Credits",
					"Quit"
			    };

char hlpmenustr[][50]={
					"Starts a new Game",
					"Displays Help on the Game",
					"Turns Sound On/Off",
					"-",
					"Displays Credits",
					"Quits the Game"
				  };

void main()
{
	titlescreen();
	getmenu();
}

void titlescreen()
{
	int x,y;
	initialize_graphics();
	gbox(5,5,MAXY-5,MAXX-5,15);
	setcolor(2);
	settextstyle(5,0,8);
	theight=textheight("DOT Game");
	twidth=textwidth("DOT Game");
	x=(MAXX-twidth)/2;
	y=((MAXY-theight)/2)-theight/2;
	for(i=0;i<5;i++) outtextxy(x+i,y+i,"DOT Game");
	setcolor(7);
	settextstyle(2,0,2);
	outtextxy(530,460,"Press <Esc> to continue...");
	while(getscan()!=1);
}

void gbox(int top,int left,int bottom,int right,int color)
{
	setcolor(color);
	line(left,top,right,top);
	line(right,top,right,bottom);
	line(left,bottom,right,bottom);
	line(left,top,left,bottom);
}

void getmenu()
{
	int choice;
	restorecrtmode();
	inittextscreen();
	cursor_off();
	menuscreen();
	choice=menu(menustr,6,4,15,hlpmenustr);
	if(choice==1) game();
	else if(choice==2) help();
	else if(choice==3) togglesound();
	else if(choice==5) credits();
	else if(choice==6) quit(0);
	getmenu();
}

void menuscreen()
{
	clearscreen();
	tbar(1,2,1,79,1,15);
	xyprint(36,1,"DOT Game",1,15);
	tbar(25,2,25,79,7,0);
}

void getrows()
{
	menuscreen();
	tbox(10,28,12,52,DOUBLE_BORDER,0,14);
	xyprint(30,11,"Enter No. of rows:",0,3);
	ROWS=spin(MINROWS,MAXROWS,ROWS,1,2,49,11,7,0);
}

void getplayernames()
{
	menuscreen();
	tbox(10,18,12,62,DOUBLE_BORDER,0,14);
	xyprint(20,11,"Hi,Player 1! Enter your name:",0,3);
	do editstr(pl1name,11,11,50,11,7,0);
	while(pl1name[0]=='\0');
	strcpy(tpl1name,pl1name);
	strcat(tpl1name,":");
	menuscreen();
	tbox(10,18,12,62,DOUBLE_BORDER,0,14);
	xyprint(20,11,"Hi,Player 2! Enter your name:",0,3);
	do editstr(pl2name,11,11,50,11,7,0);
	while(pl2name[0]=='\0');
	strcpy(tpl2name,pl2name);
	strcat(tpl2name,":");
}

void current_directory()
{
   strcpy(CURDIR, "X:\\");
   CURDIR[0] = 'A' + getdisk();
   getcurdir(0,CURDIR+3);
}

void inittextscreen()
{
	textmode(C80);
	clearscreen();
}

void togglesound()
{
	if(SOUND_ON)
	{
		strcpy(menustr[2],"Sound Off");
		SOUND_ON=0;
	}
	else
	{
		strcpy(menustr[2],"Sound On");
		SOUND_ON=1;
	}
}

void clearscreen()
{
	text_color(0,7);
	clrscr();
}

int menu(char str[][20],int no_of_lines,int line,int max_len,char hlp_str[][50])
{
	int i,top,left,bottom,right,ink=0,n=1;
	text_color(0,7);
	top=((25-no_of_lines)/2)-1;
	left=((80-max_len)/2)-3;
	right=left+max_len+5;
	bottom=top+no_of_lines+1;
	tbox(top,left,bottom,right,DOUBLE_BORDER,7,0);
	for(i=1;i<=no_of_lines;i++)
	{
		gotoxy(left+1,top+i);
		cprintf("  %-*s  ",max_len,str[i-1]);
	}
	singlemenuline(top+line,left+1,right-1);
	textcolor(10);
	textbackground(0);
	gotoxy(left+1,top+1);
	cprintf("  %-*s  ",max_len,str[0]);
	textbackground(7);
	textcolor(0);
	gotoxy(2,25);
	cprintf(" %-74s",hlp_str[0]);
	do
	{
		textcolor(0);
		textbackground(7);
		ink=getch();
		if(ink==27&&no_of_lines<6)
		{
			n=no_of_lines;
			break;
		}
		if(ink==72)
		{
			if(n==1)
				n=no_of_lines;
			else
				n--;
		}
		if(ink==80)
		{
			if(n==no_of_lines)
				n=1;
			else
				n++;
		}
		if(n==line&&ink==80)	n++;
		if(n==line&&ink==72)	n--;
		for(i=1;i<=no_of_lines;i++)
		{
			gotoxy(left+1,top+i);
			cprintf("  %-*s  ",max_len,str[i-1]);
		}
		singlemenuline(top+line,left+1,right-1);
		textcolor(10);
		textbackground(0);
		gotoxy(left+1,top+n);
		cprintf("  %-*s  ",max_len,str[n-1]);
		textbackground(7);
		textcolor(0);
		gotoxy(2,25);
		cprintf(" %-74s",hlp_str[n-1]);
	}while(ink!=13);
	return(n);
}

void singlemenuline(int line,int left,int right)
{
	int i;
	gotoxy(left-1,line);
	cprintf("%c",199);
	gotoxy(right+1,line);
	cprintf("%c",182);
	for(i=left;i<=right;i++)
	{
		gotoxy(i,line);
		cprintf("%c",196);
	}
}

void tbox(int top,int left,int bottom,int right,int border,int bgc,int fgc)
{
	int tl,tr,bl,br,hl,vl;
	int i;
	if(border==SINGLE_BORDER)
	{
		tl=218;
		tr=191;
		bl=192;
		br=217;
		hl=196;
		vl=179;
	}
	else if(border==DOUBLE_BORDER)
	{
		tl=201;
		tr=187;
		bl=200;
		br=188;
		hl=205;
		vl=186;
	}
	else return;
	text_color(bgc,fgc);
	gotoxy(left,top);
	cprintf("%c",tl);
	gotoxy(right,top);
	cprintf("%c",tr);
	gotoxy(left,bottom);
	cprintf("%c",bl);
	gotoxy(right,bottom);
	cprintf("%c",br);
	for(i=top+1;i<bottom;i++)
	{
		gotoxy(left,i);
		cprintf("%c",vl);
		gotoxy(right,i);
		cprintf("%c",vl);
	}
	for(i=left+1;i<right;i++)
	{
		gotoxy(i,top);
		cprintf("%c",hl);
		gotoxy(i,bottom);
		cprintf("%c",hl);
	}
}

void text_color(int bgc,int fgc)
{
	textbackground(bgc);
	textcolor(fgc);
}

void cursor_off()
{
	_setcursortype(_NOCURSOR);
}

void cursor_on()
{
	_setcursortype(_NORMALCURSOR);
}

void tbar(int top,int left,int bottom,int right,int bgc,int fgc)
{
	int i;
	text_color(bgc,fgc);
	for(i=top;i<=bottom;i++)
	{
		gotoxy(left,i);
		cprintf("%*s",right-left+1," ");
	}
}

void xyprint(int col,int row,char *str,int bk,int fw)
{
	text_color(bk,fw);
	gotoxy(col,row);
	cprintf("%s",str);
}

void hline(int line,int left,int right,int type,int border)
{
	int i;
	char l,r,h;
	if(border==SINGLE_BORDER)
	{
		l=195;
		r=180;
		h=196;
	}
	else if(border==DOUBLE_BORDER)
	{
		l=204;
		r=185;
		h=205;
	}
	for(i=left;i<=right;i++)
	{
		gotoxy(i,line);
		cprintf("%c",h);
	}
	if(type==EDGED)
	{
		gotoxy(left,line);
		cprintf("%c",l);
		gotoxy(right,line);
		cprintf("%c",r);
	}
}

void vline(int col,int top,int bottom,int type,int border)
{
	int i;
	char t,b,v;
	if(border==SINGLE_BORDER)
	{
		t=194;
		b=193;
		v=179;
	}
	else if(border==DOUBLE_BORDER)
	{
		t=203;
		b=202;
		v=186;
	}
	for(i=top;i<=bottom;i++)
	{
		gotoxy(col,i);
		cprintf("%c",v);
	}
	if(type==EDGED)
	{
		gotoxy(col,top);
		cprintf("%c",t);
		gotoxy(col,bottom);
		cprintf("%c",b);
	}
}

void msgbox(char *str,char *tit,int ascii)
{
	int l=strlen(str),x=((80-l)/2)-2,y=((25-(l/50))/2)-2,i;
	char buffer[4096];
	gettext(1,1,80,25,buffer);
	tbar(y,x,y+4,x+l+5,0,0);
	tbox(y,x,y+4,x+l+5,DOUBLE_BORDER,0,3);
	for(i=x;i<=x+l+5;i++)
	{
		gotoxy(i,y);
		cprintf("%c",219);
	}
	textbackground(3);
	textcolor(0);
	gotoxy(((80-strlen(tit))/2)+1,y);
	cprintf("%s",tit);
	textbackground(0);
	textcolor(15);
	gotoxy(x+3,y+2);
	cprintf("%s",str);
	beep();
	while(getch()!=ascii);
	puttext(1,1,80,25,buffer);
}

void credits()
{
	int i,j,x,y,l,se,left=20,top=9,right=61,bottom=15,a=0,c,snd,t;
	char str[50];
	union REGS ii,oo;
	cursor_off();
	text_color(0,14);
	clrscr();
	tbox(1,3,25,79,DOUBLE_BORDER,0,14);
	text_color(1,15);
	gotoxy(3,1);
	cprintf("");
	delay(100);
	strcpy(str,"A Software Developed by,");
	l=strlen(str);
	text_color(0,10);
	for(snd=0,i=0;i<l;i++)
	{
		for(y=23;y>=10;y--)
		{
			if(str[i]!=32)
			{
				gotoxy(((80-l)/2)+i,y);
				cprintf("%c",str[i]);
				gotoxy(((80-l)/2)+i,y+1);
				cprintf("%c",32);
				snd=i%2==0?1000:12000;
				if(SOUND_ON) sound(snd);
				delay(6);
				if(SOUND_ON) nosound();
			}
			else delay(8);
		}
	}
	delay(1000);
	strcpy(str,"Ritabh Sharma, II B.Tech CSE,                  ");
	l=strlen(str);
	textcolor(15);
	for(t=0,j=0,snd=0,se=0,x=78;x>=31;x--,se++,snd+=100,j++)
	{
		if(se>=l) se=l;
		if(j%2&&j<32) t++;
		for(i=0;i<=se;i++)
		{
			gotoxy(x+i,11);
			cprintf("%c",str[i]);
		}
		if(SOUND_ON) sound(snd);
		if(snd>600) delay(4);
		else delay(8);
		if(SOUND_ON) nosound();
	}
	gotoxy(30,11);
	cprintf(" Ritabh Sharma, II B.Tech CSE, ");
	delay(30);
	gotoxy(30,11);
	cprintf("  Ritabh Sharma, II B.Tech CSE, ");
	delay(30);
	gotoxy(30,11);
	cprintf("  Ritabh Sharma, II B.Tech CSE, ");
	delay(30);
	gotoxy(30,11);
	cprintf(" Ritabh Sharma, II B.Tech CSE, ");
	delay(50);
	gotoxy(30,11);
	cprintf("  Ritabh Sharma, II B.Tech CSE, ");
	delay(800);
	strcpy(str,"Graphic Era Hill University,");
	l=strlen(str);
	textcolor(12);
	for(snd=1500,x=23,i=0;i<l;i++,x++,snd-=20)
	{
		gotoxy(x,13);
		cprintf("%c",str[i]);
		if(SOUND_ON) sound(snd);
		delay(20);
		if(SOUND_ON) nosound();
		gotoxy(23,13);
		cprintf("%*s",l," ");
	}
	delay(20);
	gotoxy(23,13);
	cprintf("%s",str);
	delay(500);
	strcpy(str,"Dehradun");
	l=strlen(str);
	textcolor(7);
	for(x=32,i=0;i<l;i++,x++)
	{
		gotoxy(x,14);
		cprintf("%c",str[i]);
		delay(30);
	}
	textcolor(14);//box
	for(snd=0,i=left+1;i<=right-1;i++,snd+=20)
	{
		gotoxy(i,top);
		cprintf("%c",205);
		if(SOUND_ON) sound(snd);
		delay(15);
		if(SOUND_ON) nosound();
	}
	gotoxy(right,top);
	cprintf("%c",187);
	if(SOUND_ON) sound(snd);
	delay(15);
	if(SOUND_ON) nosound();
	for(i=top+1;i<=bottom-1;i++,snd+=20)
	{
		gotoxy(right,i);
		cprintf("%c",186);
		if(SOUND_ON) sound(snd);
		delay(30);
		if(SOUND_ON) nosound();
	}
	gotoxy(right,bottom);
	cprintf("%c",188);
	if(SOUND_ON) sound(snd);
	delay(15);
	if(SOUND_ON) nosound();
	for(i=right-1;i>left;i--,snd+=20)
	{
		gotoxy(i,bottom);
		cprintf("%c",205);
		if(SOUND_ON) sound(snd);
		delay(15);
		if(SOUND_ON) nosound();
	}
	gotoxy(left,bottom);
	cprintf("%c",200);
	if(SOUND_ON) sound(snd);
	delay(15);
	if(SOUND_ON) nosound();
	for(i=bottom-1;i>top;i--,snd+=20)
	{
		gotoxy(left,i);
		cprintf("%c",186);
		if(SOUND_ON) sound(snd);
		delay(30);
		if(SOUND_ON) nosound();
	}
	gotoxy(left,top);
	cprintf("%c",201);
	delay(700);
	xyprint(25,18," Phone: 8193948406",0,15);
	xyprint(25,19,"e-Mail: ritabhsharma1104@gmail.com",0,15);
	xyprint(25,20,"        rtbh2001@gmail.com",0,15);
	strcpy(str,"Graphic Era Hill University,");
	l=strlen(str);
	c=1;
	t=1;
	while(a!=1)
	{
		while(!kbhit())
		{
			delay(300);
			textcolor(c);
			if(t%2!=0)
			{
				for(snd=1,x=23,i=0;i<l;i++,x++,snd+=5)
				{
					gotoxy(x,13);
					cprintf("%c",str[i]);
					if(SOUND_ON) sound(snd);
					delay(20);
					if(SOUND_ON) nosound();
					gotoxy(23,13);
				}
			}
			else
			{
				for(x=23+l,snd=200,i=l;i>=0;i--,x--,snd-=5)
				{
					gotoxy(x,13);
					cprintf("%c",str[i]);
					if(SOUND_ON) sound(snd);
					delay(20);
					if(SOUND_ON) nosound();
					gotoxy(23,13);
				}
			}
			c++;
			if(c>14) c=1;
			if(c==7) c++;
			if(c==8) c++;
			t++;
		}
		ii.h.ah=0;
		int86(22,&ii,&oo);
		a=oo.h.ah;
	}
	clearscreen();
}

void beep()
{
	if(SOUND_ON)
	{
		sound(512);
		delay(150);
		nosound();
	}
}

void dwindow(int top,int left,int bottom,int right,char str[],int hbgc,int hfgc,int bbgc,int bfgc)
{
	int i;
	tbox(top,left,bottom,right,DOUBLE_BORDER,bbgc,bfgc);
	text_color(hbgc,hfgc);
	gotoxy(left+(((right-left+1)-strlen(str))/2)-4,top);
	cprintf(" %c  %s  %c ",17,str,16);
}

int intlen(int n)
{
	int l=0;
	while(n!=0)
	{
		l++;
		n/=10;
	}
	return(l);
}

int editstr(char *s,int displen,int maxlen,int col,int row,int bgc,int fgc)
{
	int i,c,sc,len=0,ptr=0,j;
	union REGS ii,oo;
	cursor_on();
	text_color(bgc,fgc);
	gotoxy(col,row);
	cprintf("%-*s",maxlen,s);
	gotoxy(col,row);
	for(j=0;s[j]!='\0';j++) len++;
	displen--; displen++;
	while(1)
	{
		getkey(0);
		c=ASCIIC;
		sc=SCANC;
		if(c==13) break;
		if(c==27) break;
		if(c>=32&&c<=127)
		{
			len++;
			ptr++;
			for(j=len;j>=ptr;j--)
				s[j]=s[j-1];
			s[ptr-1]=c;
		}
		if(c==8&&len>=1&&ptr>=1)
		{
			len--;
			ptr--;
			for(j=ptr;j<len;j++)
				s[j]=s[j+1];
		}
		if(c==0)
		{
			if(sc==75&&len>=1&&ptr>=1) ptr--; /* left */
			if(sc==77&&len<=maxlen&&ptr<len) ptr++; /* right */
			if(sc==71) ptr=0;   /* home */
			if(sc==79) ptr=len; /* end  */
			if(sc==83&&len>0)   /* del  */
			{
				len--;
				for(j=ptr;j<len;j++)
					s[j]=s[j+1];
			}
		}
		s[len]='\0';
		gotoxy(col,row);
		cprintf("%-*s",maxlen,s);
		gotoxy(col+ptr,row);
		if(len>=maxlen)
		{
			beep();
			break;
		}
	}
	cursor_off();
	return(c==27?ESCC:1);
}

int getyesno(int col,int row)
{
	int key=0,n=1;
	do
	{
		text_color(0,7);
		gotoxy(col,row);
		cprintf(" Yes   No ");
		text_color(6,15);
		if(n==1)
		{
			gotoxy(col,row);
			cprintf(" Yes ");
		}
		else
		{
			gotoxy(col+6,row);
			cprintf(" No ");
		}
		key=getscan();
		if(key==77) n=2;
		if(key==75) n=1;
	}while(key!=28);
	text_color(6,15);
	if(n==1)
	{
		gotoxy(col,row);
		cprintf(" Yes ");
	}
	else
	{
		gotoxy(col+6,row);
		cprintf(" No ");
	}
	return(n);
}

int spin(int min,int max,int ini,int inc,int len,int col,int row,int bgc,int fgc)
{
	int n=ini,key=0;
	if(n<min) n=min;
	if(n>max) n=max;
	text_color(bgc,fgc);
	gotoxy(col,row);
	cprintf("%*d",len,n);
	while(key!=28&&key!=1)
	{
		if(intlen(n)<len)
		{
			gotoxy(col,row);
			cprintf("%*s",len-intlen(n),"0");
		}
		key=getscan();
		if(key==72) n+=inc;
		if(key==80) n-=inc;
		if(n<=min)  n=min;
		if(n>=max)  n=max;
		text_color(bgc,fgc);
		gotoxy(col,row);
		cprintf("%*d",len,n);
	}
	text_color(fgc,bgc);
	gotoxy(col,row);
	cprintf("%*d",len,n);
	if(intlen(n)<len)
	{
		gotoxy(col,row);
		cprintf("%*s",len-intlen(n),"0");
	}
	return(n);
}

void getkey()
{
	union REGS ii,oo;
	ii.h.ah=0;
	int86(22,&ii,&oo);
	ASCIIC=oo.h.al;
	SCANC=oo.h.ah;
}

int getscan()
{
	ii.h.ah=0;
	int86(22,&ii,&oo);
	return(oo.h.ah);
}

int getascii()
{
	ii.h.ah=0;
	int86(22,&ii,&oo);
	return(oo.h.al);
}

void quit()
{
	cursor_on();
	exit(0);
}

void help()
{
	clearscreen();
	dwindow(1,1,25,79,"DOT Game - Help",1,15,0,14);
	xyprint(10,3,"How to Play:",0,15);
	xyprint(15,5," DOT Game is a two Player game.",0,7);
	xyprint(15,7," A square matrix of dots will be present.",0,7);
	xyprint(15,9," Each Player has to draw lines to form boxes.",0,7);
	xyprint(15,11," Left click of mouse to draw horizontal line.",0,7);
	xyprint(15,13," Right click of mouse to draw vertical line.",0,7);
	xyprint(15,15," The Player who takes more no. of boxes is declared won.",0,7);
	xyprint(10,17,"Rules to be followed:",0,15);
	xyprint(15,19," If a box can be taken, it should be taken.",0,7);
	xyprint(10,21,"About:",0,15);
	xyprint(15,23," This Game is developed with Turboc C++ 3.0.",0,7);
	wait();
}

void initialize_graphics()
{
	int gd=VGA,gm=VGAHI;
	current_directory();
	initgraph(&gd,&gm,CURDIR);
	if(graphresult()!=grOk)
	{
		clearscreen();
		msgbox("BGI Graphics not initialized.","Graphics Error",27);
		msgbox(CURDIR,"CurDir",27);
		quit();
	}
	MAXX=getmaxx();
	MAXY=getmaxy();
}

void wait()
{
	while(!kbhit());
}

void initialize_mouse()
{
	ii.x.ax=0;
	int86(0x33,&ii,&oo);
	if(oo.x.ax==0)
	{
		restorecrtmode();
		msgbox("Cannot initiliaze Mouse.","Mouse Error",27);
		quit();
	}
}

void show_mouse_ptr()
{
	ii.x.ax=1;
	int86(0x33,&ii,&oo);
}

void hide_mouse_ptr()
{
	ii.x.ax=2;
	int86(0x33,&ii,&oo);
}

void get_mouse_pos()
{
	ii.x.ax=3;
	int86(0x33,&ii,&oo);
	mousebutton=oo.x.bx;
	mousex=oo.x.cx;
	mousey=oo.x.dx;
}

void set_mouse_pos(int xcor,int ycor)
{
	ii.x.ax=4;
	ii.x.cx=xcor;
	ii.x.dx=ycor;
	int86(0x33,&ii,&oo);
}

void set_mouse_horiz_limits(int min,int max)
{
	ii.x.ax=7;
	ii.x.cx=min;
	ii.x.dx=max;
	int86(0x33,&ii,&oo);
}

void set_mouse_vert_limits(int min,int max)
{
	ii.x.ax=8;
	ii.x.cx=min;
	ii.x.dx=max;
	int86(0x33,&ii,&oo);
}

void gamescreen()
{
	char ghlpstr[]="Left Mouse - Horiz line.  Right Mouse - Vertical line.";
	setfillstyle(1,1);
	bar(0,7,MAXX,26);
	setcolor(15);
	settextstyle(0,0,2);
	twidth=textwidth("DOT Game");
	outtextxy((MAXX-twidth)/2,10,"DOT Game");
	setfillstyle(1,7);
	bar(0,MAXY-25,MAXX,MAXY-8);
	setcolor(0);
	settextstyle(11,0,10);
	twidth=textwidth(ghlpstr);
	outtextxy((MAXX-twidth)/2,MAXY-20,ghlpstr);
	gbox(50,20,430,400,15);
	gbox(50,410,175,620,15);
	setcolor(10);
	settextstyle(0,0,2);
	outtextxy(420,55,tpl1name);
	settextstyle(2,0,7);
	outtextxy(450,90,"Lines:");
	outtextxy(450,125,"Boxes:");
	gbox(185,410,310,620,15);
	setcolor(11);
	settextstyle(0,0,2);
	outtextxy(420,190,tpl2name);
	settextstyle(2,0,7);
	outtextxy(450,225,"Lines:");
	outtextxy(450,260,"Boxes:");
	bar(410,320,618,428);
	setcolor(15);
	line(410,320,410,430);
	line(411,321,411,429);
	line(410,320,620,320);
	line(411,321,619,321);
	settextstyle(0,0,1);
	setcolor(0);
	outtextxy(433,330,"Program developed by,");
	settextstyle(0,0,3);
	setcolor(4);
	outtextxy(432,345,"A.Prabu");
	settextstyle(0,0,1);
	setcolor(0);
	outtextxy(460,375,"II year B.E.,");
	settextstyle(0,0,1);
	setcolor(1);
	outtextxy(430,390,"Hindusthan College of");
	outtextxy(455,400,"Engg. & Tech.,");
	setcolor(5);
	outtextxy(448,415,"Coimbatore - 32.");
}

void initialize_vars()
{
	for(i=0;i<19;i++)
		for(j=0;j<19;j++)
		{
			matrixh[i][j]=0;
			matrixv[i][j]=0;
			matrixb[i][j]=0;
		}
	for(i=0;i<2;i++)
	{
		playerlines[i]=0;
		playerboxes[i]=0;
	}
	PLAYER=1;
}

void print_dots()
{
	int x,y;
	setfillstyle(1,14);
	for(i=1,y=matrix_sty;i<=ROWS;i++,y+=matrix_dis)
	{
		for(j=1,x=matrix_stx;j<=ROWS;j++,x+=matrix_dis)
		{
			bar(x,y,x+2,y+2);
		}
	}
}

void check_for_boxes()
{
	int x,y,change=1,gochk;
	char plnostr[2],goplstr[30];
	settextstyle(2,0,2);
	for(gochk=0,i=0,y=matrix_sty;i<ROWS;i++,y+=matrix_dis)
	{
		for(j=0,x=matrix_stx;j<ROWS;j++,x+=matrix_dis)
		{
			if(matrixh[i][j]&&matrixh[i+1][j]&&matrixv[i][j]&&matrixv[i][j+1])
			{
				if(matrixb[i][j]!=1)
				{
					sprintf(plnostr,"%1d",PLAYER);
					settextstyle(0,0,1);
					hide_mouse_ptr();
					outtextxy(x+7,y+8,plnostr);
					show_mouse_ptr();
					matrixb[i][j]=1;
					change=0;
					playerboxes[PLAYER-1]++;
				}
				gochk+=matrixb[i][j];
			}
		}
	}
	if(change) PLAYER=PLAYER==1?2:1;
	print_player();
	print_details();
	if(gochk==(ROWS-1)*(ROWS-1))
	{
		settextstyle(0,0,4);
		if(playerboxes[0]==playerboxes[1]) strcpy(goplstr,"Match tied");
		else sprintf(goplstr,"%s wins",playerboxes[0]>playerboxes[1]?pl1name:pl2name);
		twidth=textwidth(goplstr);
		theight=textheight(goplstr);
		hide_mouse_ptr();
		cleardevice();
		outtextxy((MAXX-twidth)/2,((MAXY-theight)/2)-theight/2,goplstr);
		wait();
		getmenu();
	}
}

void print_player()
{
	char playerstr[20];
	setfillstyle(1,0);
	bar(25,40,150,48);
	settextstyle(0,0,1);
	setcolor(playercolor[PLAYER-1]+8);
	sprintf(playerstr,"Player %d to play",PLAYER);
	outtextxy(25,40,playerstr);
}

void print_details()
{
	char lbstr[5];
	setfillstyle(1,0);
	settextstyle(0,0,2);
	setcolor(playercolor[0]+8);
	bar(530,97,600,110);
	sprintf(lbstr,"%-4d",playerlines[0]);
	outtextxy(530,97,lbstr);
	bar(530,132,600,145);
	sprintf(lbstr,"%-4d",playerboxes[0]);
	outtextxy(530,132,lbstr);
	setcolor(playercolor[1]+8);
	bar(530,232,600,245);
	sprintf(lbstr,"%-4d",playerlines[1]);
	outtextxy(530,232,lbstr);
	bar(530,267,600,280);
	sprintf(lbstr,"%-4d",playerboxes[1]);
	outtextxy(530,267,lbstr);
}

void game()
{
	int key=0,x,y;
	getrows();
	getplayernames();
	help();
	initialize_vars();
	setgraphmode(getgraphmode());
	gamescreen();
	print_dots();
	initialize_mouse();
	set_mouse_horiz_limits(21,399);
	set_mouse_vert_limits(51,429);
	set_mouse_pos(41,71);
	show_mouse_ptr();
	print_player();
	print_details();
	while(key!=1)
	{
		while(!kbhit())
		{
			get_mouse_pos();
			if(mousebutton==LEFTMOUSE)
			{
				for(i=0,x=41;i<ROWS-1;i++,x+=matrix_dis)
				{
					for(j=0,y=71;j<ROWS;j++,y+=matrix_dis)
					{
						if(mousex>=x-4&&mousex<=x+4&&mousey>=y-4&&mousey<=y+4)
						{
							if(matrixh[j][i]!=1)
							{
								matrixh[j][i]=1;
								setcolor(playercolor[PLAYER-1]);
								hide_mouse_ptr();
								line(x+2,y-1,x+18,y-1);
								line(x+2,y-0,x+18,y-0);
								line(x+2,y+1,x+18,y+1);
								show_mouse_ptr();
								playerlines[PLAYER-1]++;
								check_for_boxes();
							}
						}
					}
				}
			}
			if(mousebutton==RIGHTMOUSE)
			{
				for(i=0,x=41;i<ROWS;i++,x+=matrix_dis)
				{
					for(j=0,y=71;j<ROWS-1;j++,y+=matrix_dis)
					{
						if(mousex>=x-4&&mousex<=x+4&&mousey>=y-4&&mousey<=y+4)
						{
							if(matrixv[j][i]!=1)
							{
								matrixv[j][i]=1;
								setcolor(playercolor[PLAYER-1]);
								hide_mouse_ptr();
								line(x-1,y+1,x-1,y+18);
								line(x-0,y+1,x-0,y+18);
								line(x+1,y+1,x+1,y+18);
								show_mouse_ptr();
								playerlines[PLAYER-1]++;
								check_for_boxes();
							}
						}
					}
				}
			}
		}
		ii.h.ah=0;
		int86(22,&ii,&oo);
		key=oo.h.ah;
	}
}