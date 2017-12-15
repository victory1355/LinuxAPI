//模拟登录系统
#include <stdio.h>
//#include <curses.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

//密码的最大长度
#define MAX_SIZE   10
//密码的最小长度
#define MIN_SIZE    6
//按键全局变量
char ch;
//注册模块
int register_re();
//登录模块
int login();

//按键坐标
int x = 1;    
int y = 1;

/*//系统主界面
void display_curse()
{
	initscr();
	noecho();
	if(has_colors())
	{
		start_color();
		init_pair(1, COLOR_RED, COLOR_BLACK);//初始化颜色对
		attron(COLOR_PAIR(1));//前景，背景色设置
		attron(A_REVERSE| A_BOLD);  //字体加亮加粗
	}
	//nocbreak();
	keypad(stdscr, true);
	
	
	box(stdscr, '|', '-');
	attron(A_REVERSE);  //字体反白显示
	mvaddstr(3, COLS/2-8, "welcome to god system\n");
	//按r/R注册，按l或L登录,按esc退出
	mvaddstr(5, COLS/2-8, "press 'r' or 'R' to register\n");
	mvaddstr(6, COLS/2-8, "press 'l' or 'L' to login\n");
	mvaddstr(7, COLS/2-8, "press 'ESC' to exit\n");
	attroff(A_REVERSE);  
	
	attron(A_REVERSE| A_BOLD);  //字体加亮加粗
	mvaddstr(LINES/2, COLS/2-6, "register\n");
	mvaddstr(LINES/2, COLS/2+6, "login\n");
	attroff(A_REVERSE| A_BOLD);  //字体加亮加粗
	
	
	refresh();
	
}*/
void display_nocurses()
{
	printf("+-----------------------------------------------------------------+\n");
	printf("|   	            welcome to system                             |\n");
	printf("|               press 'r' or 'R' to register                      |\n");
	printf("|                press 'l' or 'L' to login                        |\n");
	printf("|                 press 'q' or 'Q' to exit                        |\n");
	printf("+-----------------------------------------------------------------+\n");
}
/*//输入模块
void input()
{
	int ch;
	move(y,x);
	do
	{
		ch = getch();
		switch(ch)
		{
			//case KEY_UP: --y;break;
			case 'r': 
			case 'R': mvprintw(4, 2, "please input user_name:");break;//第四行，第二列打印字符串register_re();getch();break;
			case 'l': 
			case 'L': login();getch();break;
			//case KEY_DOWN:++y;break;
			//case KEY_LEFT:--x;break;
			//case KEY_RIGHT:++x;break;
			//case ' ':x = 0; ++y ;break;
			//case '\t':x += 7;break;
			//case KEY_BACKSPACE:mvaddch(y, --x, ' ');break;
			case 27:endwin();exit(1);
			//default:addch(ch);break;
		}
		move(y, x);  //y是行, x是列
	}while(1);
}*/

//注册模块,如果输入非法字符的话，只有三次注册的机会
int register_re()
{
	FILE *fp;
	char password[50] = {'\0'};
	char user_name[50] = {'\0'};
	int flag_name=0, flag_pass =0;
	//非法字符处理
	char *pass = password;
	char *name = user_name;
	
	//限制注册的次数
	int register_time = 0;
	
	printf("+-----------------------------------------------------------------+\n");
	printf("|                                                                 |\n");
	printf("|                      welcome to register                        |\n");
	printf("|                                                                 |\n");
	printf("+-----------------------------------------------------------------+\n");
	
	//用追加的方式打开文件
	fp = fopen("/mnt/hgfs/workspace/io_operation/login_system/login.log", "a+");
	if(fp == NULL)
	{
		perror("open fail");
		return -1;
	}
	//输入密码和用户名
	//mvprintw(4, 2, "please input user_name:");//第四行，第二列打印字符串
	printf("please input user_name:");
	scanf("%s", user_name);
	printf("\n");
	//mvprintw(5, 2, "\nplease input password:");//第五行，第二列打印字符串
	printf("please input password:");
	scanf("%s", password);
	
	while(1)
	{
		
		while(*pass != '\0')
		{
			//密码长度的判断
			if(strlen(password)>MAX_SIZE)
			{
				printf("\nyou password too long, try again!\n");
				flag_pass = 1;
				break;
			}
			if(strlen(password)<MIN_SIZE)
			{
				printf("\nyou password too short, try again!\n");
				flag_pass = 1;
				break;
			}
			//含有非数字，大小字母之外的字符
			if(*pass<48 ||*pass>58 &&*pass<64 ||*pass>91 &&*pass<97 || *pass>122)
			{
				printf("\nyour password include illegal char!\n");
				flag_pass = 1;
				break;
			}
			
			flag_pass =0;
			pass++;
		}
		//用户名的判断
		while(*name != '\0')
		{	
			//含有非数字，大小字母之外的字符
			if(*name<48 ||*name>58 &&*name<64 ||*name>91 &&*name<97 || *name>122)
			{
				printf("\nyour name include illegal char!\n");
				flag_name = 1;
				break;
			}
			else
				flag_name =0;
			name++;
		}
		if(flag_name == 0 && flag_pass == 0)
			break;
		if(flag_name == 1 || flag_pass == 1)
		{
			register_time++;
			if(register_time >3)
			{
				printf("--------------sorry,you fail to register!---------------\n");
				break;
			}
			while(getchar()!= '\n');
			memset(user_name, '\0', sizeof(user_name));
			printf("\nplease input legal user_name:");
			scanf("%s", user_name);
			//printf("\n");
			memset(password, '\0', sizeof(password));
			printf("\nplease input legal password:");
			scanf("%s", password);
		}
		
	}
	
	fprintf(fp, "%s %s \n", user_name, password);
	
	int fc = fclose(fp);
	if(fc == -1)
	{
		perror("close fail");
		return -1;
	}
	//mvprintw(4, 2,"register successful\n");
	printf("\n------------------register successful-----------------\n");
	
	//返回主界面
	display_nocurses();
	printf("press the word to continue\n");
	printf("press:");
	scanf("%s", &ch);
	
	return 0;
}
//产生验证码
void product_auth(char *auth)
{
	int i, tmp;
	for(i=0; i<6; i++)
	{
		tmp = rand()%10;
		if(tmp<6)
		{
			*auth = 'a' + rand()%26;
		}
		else if(tmp>8)
		{
			*auth = 'A' + rand()%26;
		}
		else
		{
			*auth = '0' + rand()%10;
		}
		auth++;
	}
	return;
	
}

//登录模块,如果账号或者密码输入超过三次，要输入验证码，输入验证码超过三次直接退出
int login()
{
	FILE *fp;
	char password_old[50];
	char password_login[50];
	
	char user_name_old[50];
	char user_name_login[50];
	
	int ps;
	int us;
	//验证码
	char auth[7] = {'\0'};
	char auth_buf[7];
	
	int flag = 0;
	//纪录输入错误的次数
	int login_time = 0;
	//验证码输入错误的次数
	int pass_time = 0;
	
	//剩余的登录次数
	int i = 3;
	
	printf("+-----------------------------------------------------------------+\n");
	printf("|                                                                 |\n");
	printf("|                         welcome to login                        |\n");
	printf("|                                                                 |\n");
	printf("+------------------------------------------------------=====------+\n");
	
	fp = fopen("/mnt/hgfs/workspace/io_operation/login_system/login.log","r");
	if(fp == NULL)
	{
		perror("open fail");
		return -1;
	}
	
	while(1)
	{
		if(login_time<3)
		{
			printf("please input user_name:");
			scanf("%s", user_name_old);
			printf("please input password:");
			scanf("%s", password_old);
			
			while(getchar()!='\n');
			rewind(fp);
			//从密码文档里面读取数据并与当前的的数据匹配
			while(fscanf(fp, "%s%s", user_name_login, password_login) != EOF)
			{
				us = strcmp(user_name_old, user_name_login);
				ps = strcmp(password_old, password_login);
				if(us==0 && ps==0)
				{
					printf("--------------------login successful----------------\n");
					flag  = 1;
				}
			}
		}
		login_time++;
		if(login_time>3)
		{
			while(pass_time <3)
			{
				printf("you just have %d times to login!\n", i--);
				printf("please input user_name:");
				scanf("%s", user_name_old);
				printf("please input password:");
				scanf("%s", password_old);
				//产生验证码
				product_auth(auth);
				printf("please input the auth number:%s\n", auth);
				printf("input:");
				scanf("%s", auth_buf);
				if(strcmp(auth_buf, auth)==0)
				{
					printf("--------------login successful-------------\n");
					flag  = 1;
					break;
				}
				else{
					//printf("you just have %d times to login!\n", i--);
					pass_time++;
					if(pass_time>3)
					{
						printf("sorry!you cannot login anymore\n");
						//返回主界面
						flag = 1;
						break;
					}
				}
			}	
		}
		
		if(flag == 1)
			break;
		if(flag == 0)
		{
			printf("the user not exist ,please check your username and password!\n");
		}
		
		//printf("no user\n");	
	}
	
	int fc = fclose(fp);
	if(fc == -1)
	{
		perror("close fail");
		return -1;
	}
	//返回主界面
	display_nocurses();
	printf("press the word to continue\n");
	printf("press:");
	scanf("%s", &ch);
	
	return 0;
	
}

int main()
{
		//产生验证码
		srand(time(0));
		int flag = 0, ret;
		display_nocurses();
	//	while(getchar()!= '\n');
		while(1)
		{
			ret = scanf("%c", &ch);
			if(ch== 'r'||ch=='R'||ch=='L'||ch=='l'||ch=='q'||ch=='Q')
				break;
			else
			{
				printf("please input word ablow\n");
				while(getchar()!= '\n');
			}
		}
		do
		{	
			
		//	printf("%c\n", ch);
			switch(ch)
			{
				case  'l':
				case  'L':login();break;
				case  'r':
				case  'R':register_re();break;
				case  'q':
				case  'Q':flag = 1;break;
				default :while(getchar()!= '\n');getchar();				
			}
			if((flag)==1)
				break;
		}while(1);
			
		
		return 0;
}









