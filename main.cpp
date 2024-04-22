#include <iostream>
#include <time.h>
#include<stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <termios.h>
#include <unistd.h> // for sleep()


//7 классов.класс игра-1,класс корабль-2,класс игрок-3,класс соперник-4,класс конец игры-5,класс поле-6(для отслеживания координат короблей),класс стрельба-7.

using namespace std;

/*Ошибки проекта:
Не расчитал кол-во классов
Забыл использовать интерфейсы(из-за этого интерфейс просто существует в программе,но на логику не влияет)
Много статических данных,нудно было сделать гараздо большое динамических переменных
Все в одном файле
*/

char symvol = 176;//символ для корабля
char crest = (char)120u;//символ для попадания(крестик)
char ranen = '0';
//char symvol = (char)23u;
char getch() {
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
                perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                perror ("tcsetattr ~ICANON");
        return (buf);
}

class ShipPlacementInterface //интерфейс-пустышка
{
public:
     void virtual PlayerCreateMapShips() = 0;
};

class ManuallyShipPlacementInterface //интерфейс-пустышка
{
public:
     void virtual CreateShips(int sizeship,int countsh)=0;
};
class Visual//визуал игры(карта)сделать приветственную надпись.сделать запись окончания игры.Меню,вывод анимации или просто красиво напечатанных слов о начале и конце игры
{
public:
	
	/*cout << (char)254u;*/ //аски код для кораблей(квадрат)
	
	void CreateMap() {
		char num[] = "abcdefghij";
		char letter[] = "1234567890";
		
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < rows; j++) {
				arr[i][j] = '-';
			}
		}
		arr[0][0] = '~';
		for(int i = 1; i < rows; i++)//заполнение поля цифрами
		{
			int j = 0;
			arr[i][j] = num[i - 1];
		}

		for (int i = 1; i < rows; i++)//заполнение поля буквами
		{
			int j = 0;
			arr[j][i] = letter[i - 1];
		}
	
		

	}
	void PrintMap(char arra[11][11])
	{
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < rows; j++) {
				cout << arra[j][i] << " ";
			}
			cout << endl;
		}
	}
	char(*GetData())[11] {
		return arr;
		}
private:
	static const int rows = 11;
	char arr[rows][rows];
	
};

class Ship:public Visual {
public:
	void setData(char data[11][11]) {
		// копируем данные в массив класса Ship
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {
				this->data[i][j] = data[i][j];
			}
		}
	}
	void setArrop(char data[11][11]) {
		// копируем данные в массив класса Ship
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {
				this->arrop[i][j] = data[i][j];
			}
		}
	}

	void printstringplayer(int j){
		
		for (int i = 0; i < 11; i++)
		{
			cout<<data[i][j]<<" ";
		}
		
	}
	void printstringhide(int j)
	{
		for (int i = 0; i < 11; i++)
		{
			cout<<arrop[i][j]<<" ";
		}
	}
	void printData() {
		PrintMap(data);
	}
	
	void changearray(int x,int y)//изменение карты в зависимости от ходов игрока или соперника.
	{
		deadship=false;
		int x1=x;int y1=y;//начальные координаты
		int tempx[4]={0};//массивы для хранения временных координат раненых кораблей для обрисовки звездами
		int tempy[4]={0};	
		if (data[x][y]==symvol)//если попал в корабль
		{
			if(data[x][y + 1] == symvol &&
						data[x][y - 1] == symvol ||
						data[x + 1][y] == symvol &&
						data[x - 1][y] == symvol)//если вокруг есть два коралбля,то приписывать первую коорду
						{
							firstcordx=x;
							firstcordy=y;
						}
							char num[] = "abcdefghij";
							char letter[] = "1234567890";
			countsquare--;
			if (
					data[x][y + 1] != symvol &&//проверка на корабли
					data[x][y - 1] != symvol &&
					data[x + 1][y] != symvol &&
					data[x - 1][y] != symvol &&
					data[x][y + 1] != ranen &&//проверка на раненые корабли
					data[x][y - 1] != ranen &&
					data[x + 1][y] != ranen &&
					data[x - 1][y] != ranen 
					
					)//проверка на корабли по кресту
					//РАБОЧАЯ ОБРИСОВКА ЗВЕЗД ДЛЯ ОДНОПАЛУБНЫХ КОРАБЛЕЙ
			{
					data[x][y] = crest ;//убийство
					data[x][y + 1] = '*';
					data[x][y - 1]  = '*' ; 
					data[x + 1][y]  = '*' ;
					data[x + 1][y + 1]  = '*' ;
					data[x + 1][y - 1]  = '*' ; 
					data[x - 1][y]  = '*' ;
					data[x - 1][y + 1]  = '*' ;
					data[x - 1][y - 1]  = '*' ;
					//
					arrop[x][y] = crest ;//убийство
					arrop[x][y + 1] = '*';
					arrop[x][y - 1]  = '*' ; 
					arrop[x + 1][y]  = '*' ;
					arrop[x + 1][y + 1]  = '*' ;
					arrop[x + 1][y - 1]  = '*' ; 
					arrop[x - 1][y]  = '*' ;
					arrop[x - 1][y + 1]  = '*' ;
					arrop[x - 1][y - 1]  = '*' ;
					firstcordx=0;
					firstcordy=0;
					
							for(int i = 1; i < 11; i++)//заполнение поля цифрами
							{
								int j = 0;
								data[i][j] = num[i - 1];
								arrop[i][j]=num[i-1];
							}

							for (int i = 1; i < 11; i++)//заполнение поля буквами
							{
								int j = 0;
								data[j][i] = letter[i - 1];
								arrop[j][i]=letter[i-1];
							}
							data[0][0] = '~';
							arrop[0][0]='~';
							deadship=true;
			}
			
			else{
				
				// data[x][y]=ranen;
				// arrop[x][y]=ranen;
				int count=1;//СДЕЛАТЬ ПРОВЕРУ ПРОТИВОПОЛОЖНОЙ СТОРОНЫ ОТНОСТИТЕЛЬНО ПЕРВЫХ КООРДИНАТ
				int tempcount=0;
				while (true)//проверка на ранение по кресту,если ранение есть в какой-то из сторон,то идет запись этой координаты и проверка этой же стороны.
				{
					// if(data[x][y + 1] == symvol ||//проверка на корабли по кресту
					// data[x][y - 1] == symvol ||
					// data[x + 1][y] == symvol ||
					// data[x - 1][y] == symvol ){
					// 	break;
					// }
					if(data[firstcordx][firstcordy + 1] == symvol &&
						data[firstcordx][firstcordy - 1] == symvol )
					{
						data[x][y]=ranen;
						arrop[x][y]=ranen;
						break;
					}
					if(
						data[firstcordx + 1][firstcordy] == symvol &&
						data[firstcordx - 1][firstcordy] == symvol)//если стрелять в центр,то проверять на наличие кораблей вокруг первых коорд
					{
						data[x][y]=ranen;
						arrop[x][y]=ranen;
						break;
					}
					if(data[x][y+1] == ranen && data[x ][y-1] == ranen&&data[x][y+2]!=ranen&&data[x][y-2]!=ranen)//если стрелять в центр последним ходом 3хпалубный корабль
							{
								data[x][y]=ranen;
								arrop[x][y]=ranen;
								y++;
								y1++;
							}
					if(data[x + 1][y] == ranen && data[x - 1][y] == ranen&&data[x+2][y]!=ranen&&data[x-2][y]!=ranen)//если стрелять в центр последним ходом 3хпалубный корабль
							{
								data[x][y]=ranen;
								arrop[x][y]=ranen;
								x++;
								x1++;
							}
					if(data[x + 1][y] == ranen && data[x - 1][y] == ranen&&(data[x+2][y]==ranen||data[x-2][y]==ranen))//если стрелять в центр последним ходом 4хпалубный корабль
							{
								data[x][y]=ranen;
								arrop[x][y]=ranen;
								if(data[x+2][y]==ranen){
								x--;x1--;}
								else{x++;x1++;}
							}
					if(data[x][y+1] == ranen && data[x][y-1] == ranen&&(data[x][y+2]==ranen||data[x][y-2]==ranen))//если стрелять в центр последним ходом 4хпалубный корабль
							{
								data[x][y]=ranen;
								arrop[x][y]=ranen;
								if(data[x][y+2]==ranen){
								y--;y1--;}
								else{y++;y1++;}
							}

					if(data[x-1][y]==ranen&&data[x-2][y]==ranen&&data[x-3][y]==symvol||data[x+1][y]==ranen&&data[x+2][y]==ranen&&data[x+3][y]==symvol)
					{
						data[x][y]=ranen;
						arrop[x][y]=ranen;
						break;
					}
					if(data[x][y-1]==ranen&&data[x][y-2]==ranen&&data[x][y-3]==symvol||data[x][y+1]==ranen&&data[x][y+2]==ranen&&data[x][y+3]==symvol)
					{
						data[x][y]=ranen;
						arrop[x][y]=ranen;
						break;
					}
						data[x][y]=ranen;
						arrop[x][y]=ranen;
						if(data[x][y + 1] == symvol ||//проверка на корабли по кресту
						data[x][y - 1] == symvol ||
						data[x + 1][y] == symvol ||
						data[x - 1][y] == symvol ){
						break;

					}
					if (data[x1][y1+1]==ranen)
					{
						
						for (int i = 0; i < count; i++)
						{
							if(data[x][y+1]==ranen)
							{
								
								y++;tempy[count-1]=y;//запись в массив координату,в которой есть раненый корабль
								tempx[count-1]=x;
								count++;
								tempcount=count;
							}
							else{count--;
							tempcount=count;
							}
						}
						break;
					}
					if (data[x1][y1-1]==ranen)
					{
						for (int i = 0; i < count; i++)
						{
							
							if(data[x][y-1]==ranen)
							{
								y--;tempy[count-1]=y;
								tempx[count-1]=x;
								count++;
								tempcount=count;
							}
							else{count--;
							tempcount=count;
							}
						}
						break;
					}
					if (data[x1+1][y1]==ranen)
					{
						for (int i = 0; i < count; i++)
						{
							if(data[x+1][y]==ranen)
							{
								x++;tempx[count-1]=x;
								tempy[count-1]=y;
								count++;
								tempcount=count;
							}
							else{count--;
							tempcount=count;
							}
						}
						break;
					}
					if (data[x1-1][y1]==ranen)
					{
						for (int i = 0; i < count; i++)
						{
							if(data[x-1][y]==ranen)
							{
								x--;tempx[count-1]=x;
								tempy[count-1]=y;
								count++;
								tempcount=count;
							}
							else{count--;
							tempcount=count;
							}
						}
						break;
					}
					else{break;}
				}
						if(tempcount>0){
							deadship=true;
							data[x1][y1] = crest ;//убийство
							data[x1][y1 + 1] = '*';//если проверка завалится(закончатся палубы кораблей)
						//вокруг записанных координат нарисовать звезды,после заменить записанные координаты крестами(убитый корабль)
							data[x1][y1 - 1]  = '*' ; 
							data[x1 + 1][y1]  = '*' ;
							data[x1 + 1][y1 + 1]  = '*' ;
							data[x1 + 1][y1 - 1]  = '*' ; 
							data[x1 - 1][y1]  = '*' ;
							data[x1 - 1][y1 + 1]  = '*' ;
							data[x1 - 1][y1 - 1]  = '*' ;
							arrop[x1][y1] = crest ;//убийство
							arrop[x1][y1 + 1] = '*';//если проверка завалится(закончатся палубы кораблей)
						//вокруг записанных координат нарисовать звезды,после заменить записанные координаты крестами(убитый корабль)
							arrop[x1][y1 - 1]  = '*' ; 
							arrop[x1 + 1][y1]  = '*' ;
							arrop[x1 + 1][y1 + 1]  = '*' ;
							arrop[x1 + 1][y1 - 1]  = '*' ; 
							arrop[x1 - 1][y1]  = '*' ;
							arrop[x1 - 1][y1 + 1]  = '*' ;
							arrop[x1 - 1][y1 - 1]  = '*' ;
							for (int i = 0; i < tempcount; i++)//цикл для записи вокруг временных точек проверки
							{
								

								data[tempx[i]][tempy[i]+1]='*';
								data[tempx[i]][tempy[i]-1]='*';
								data[tempx[i]+1][tempy[i]]='*';
								data[tempx[i]+1][tempy[i]+1]='*';
								data[tempx[i]+1][tempy[i]-1]='*';
								data[tempx[i]-1][tempy[i]]='*';
								data[tempx[i]-1][tempy[i]+1]='*';
								data[tempx[i]-1][tempy[i]-1]='*';
								data[x1][y1] = crest ;//убийство

								arrop[tempx[i]][tempy[i]+1]='*';
								arrop[tempx[i]][tempy[i]-1]='*';
								arrop[tempx[i]+1][tempy[i]]='*';
								arrop[tempx[i]+1][tempy[i]+1]='*';
								arrop[tempx[i]+1][tempy[i]-1]='*';
								arrop[tempx[i]-1][tempy[i]]='*';
								arrop[tempx[i]-1][tempy[i]+1]='*';
								arrop[tempx[i]-1][tempy[i]-1]='*';
								arrop[x1][y1] = crest ;//убийство
							}
							for (int i = 0; i < tempcount; i++)
							{
								data[tempx[i]][tempy[i]]=crest;
								arrop[tempx[i]][tempy[i]]=crest;
							}
							
							for(int i = 1; i < 11; i++)//заполнение поля цифрами
							{
								int j = 0;
								data[i][j] = num[i - 1];
								arrop[i][j]=num[i-1];
							}

							for (int i = 1; i < 11; i++)//заполнение поля буквами
							{
								int j = 0;
								data[j][i] = letter[i - 1];
								arrop[j][i]=letter[i-1];
							}
							data[0][0] = '~';
							arrop[0][0]='~';
						}
			}
			
		}
		else if (data[x][y]=='-')
		{
			cout<<"You missing"<<endl;
			data[x][y]='*';
			arrop[x][y]='*';
		}
		else{
			cout<<"else"<<endl;
		}
	}
	int GetCountShip(){
		return countsquare;
	}
	void CreateShips(int ships,int square) //метод для расстоновки кораблей автоматически
	{
		int dir;
		int countship = 0;		
			//первичная позиция 
			int x, y;
			
		  while (countship<ships)
		  {
			x = rand() % N;//случайные координаты
			y = rand() % N;
			int tempx = x;
			int tempy = y;
		  
			
			dir = rand() % 4;//случайное направление

			bool setting_is_possible = 1;
			//проверка возможности постановки корабля
			
			for (int i = 0; i < square; i++)
			{	
				if (x < 1 || y < 1 || x >= N || y >= N)
				{
					setting_is_possible = 0;
					break;
				}
				
				if (data[x][y] == symvol ||
					data[x][y + 1] == symvol ||
					data[x][y - 1] == symvol ||
					data[x + 1][y] == symvol ||
					data[x + 1][y + 1] == symvol ||
					data[x + 1][y - 1] == symvol ||
					data[x - 1][y] == symvol ||
					data[x - 1][y + 1] == symvol ||
					data[x - 1][y - 1] == symvol)
				{
					setting_is_possible = 0;//false
					break;
				}
				
				switch (dir)
				{
					case 0:
					x++;
					break;
					case 1:
					y++;
					break;
					case 2:
					x--;
					break;
					case 3:
					y--;
					break;
				}
			}
			//если есть возмжность поставить корабль
			if(setting_is_possible == 1)
			{ 
				x = tempx;
				y = tempy;	
				for (int i = 0; i < square; i++)//создание 5палбуного коробля
				{

					this->data[x][y] = symvol;
					switch (dir)
					{
						case 0:				
						x++;
						break;
						case 1:				
						y++;
						break;
						case 2:				
						x--;
						break;
						case 3:					
						y--;
						break;
					}
				}
				countship++;
			}
		  }
			
		
		
	}
	
	
	void checkfotbot(int x,int y)//проверка массива для бота
	{
		int dir = rand()%4;
		
		while (true)
		{
			if (shipneardy==true)//если рядом есть корабли.Начало второго хода
			{
				
				x=coordX[0];//приравниваю к координатам,в которых есть не однопалубный живой корабль
				y=coordY[0];
				//если не убил корабль на 2 ходе,то прировнять dir=tempdir
				int count=1;
				if(is3ship==false&&is4ship==false)
				{
					for (size_t i = 0; i < count; i++)
					{
								switch (dir)
						{
							case 0:
							x++;
							break;
							case 1:
							y++;
							break;
							case 2:
							x--;
							break;
							case 3:
							y--;
							break;
						}if(x>10||y>10||y<1|x<1){
							x=coordX[0];
							y=coordY[0];
							dir = rand()%4;
							count++;
						}
					}
				}
				if(data[x][y]==symvol&&is3ship==false&&is4ship==false)//если попаду во время 2го хода
				{	
					
					
					changearray(x,y);
					if(deadship==true)//если корабль полностью убит.ЭТО ВТОРОЙ ХОД БОТА
					{
						shipneardy=false;//кораблей рядом нет
						tempdir=0;
						
						break;
					}
					else if(deadship==false)//если на 2ом ходе не попал,значит корабль 3хпалубный
					{
						tempdir=dir;
						is3ship=true;
						coordX[0]=x;coordY[0]=y;
						break;
					}
				}
				if(is3ship==true)//это третий ход по трехпалубному кораблю
				{
					switch (tempdir)//продолжение стрельбы по стороне
						{
						case 0:
						x++;
						break;
						case 1:
						y++;
						break;
						case 2:
						x--;
						break;
						case 3:
						y--;
						break;
					}
						changearray(x,y);
					
					
					if(deadship==false&&data[x][y]!=symvol&&data[x][y]!=ranen)
					{
							switch (tempdir)
							{
								case 0:
									x -= 3; // Увеличиваем x на 3
									break;
								case 1:
									y -= 3; // Увеличиваем y на 3
									break;
								case 2:
									x += 3; // Уменьшаем x на 3
									break;
								case 3:
									y += 3; // Уменьшаем y на 3
									break;
							}
						changearraydir=true;
						changearray(x,y);
						
					}
					if(deadship==true)//если корабль полностью убит на третьем ходе
					{
						//changearray(x,y);
						shipneardy=false;//кораблей рядом нет
						is3ship=false;
						tempdir=0;
						break;
					}
					if(deadship==false&&is3ship==true)
					{
						
						is3ship=false;
						is4ship=true;
						//запись координат
						coordX[0]=x;coordY[0]=y;
						cout<<"это четырехпалубник"<<endl;
						break;
					}
				}
				if(is4ship==true)//начало 4го хода для 4хпалубного
				{
					if(changearraydir==true)
					{
						switch (tempdir)
							{
								case 0:
									x --; // Увеличиваем x на 3
									break;
								case 1:
									y --; // Увеличиваем y на 3
									break;
								case 2:
									x++ ; // Уменьшаем x на 3
									break;
								case 3:
									y++; // Уменьшаем y на 3
									break;
							}
					}
					else if(changearraydir==false){

						switch (tempdir)//продолжение стрельбы по стороне
							{
							case 0:
							x++;
							break;
							case 1:
							y++;
							break;
							case 2:
							x--;
							break;
							case 3:
							y--;
							break;
						}
					}
					
					changearray(x,y);
					if(data[x][y]=='*')
					{
						switch (tempdir)
							{
								case 0:
									x -= 4; // Увеличиваем x на 3
									break;
								case 1:
									y -= 4; // Увеличиваем y на 3
									break;
								case 2:
									x += 4; // Уменьшаем x на 3
									break;
								case 3:
									y += 4; // Уменьшаем y на 3
									break;
							}
							changearray(x,y);
					}
					
					is4ship=false;
					shipneardy=false;
					//tempdir=0;
					changearraydir=false;
					break;
				}
				else if(data[x][y]=='*')//если выбранная сторона уже была обстрелена,то заново пройти цикл while
				{
					//возвращаю координаты на место
					x=coordX[0];
					y=coordY[0];
					dir = rand()%4;
					tempdir = rand()%4;
					
				}
				else if(data[x][y]!=symvol||data[x][y]!='-'&&data[x][y]!='*')//если в выбранной стороне нет корабля,то 
				{
					changearray(x,y);
					cout<<"bot missing"<<endl;
					break;
				}
				else if(data[x][y]=='-'&&deadship==false&&is3ship==true)//если корабль не убили и промазали по направлению,то нужно менять направление на противоположное
						{
								//этот метод будет работать с двухпалубными кораблями,которые не смогли угадать правильное направление
							switch (tempdir)//возвращаю сторону
						{
						case 0:
						x--;
						break;
						case 1:
						y--;
						break;
						case 2:
						x++;
						break;
						case 3:
						y++;
						break;
						}
							switch (tempdir)//меняю направление противоположно
						{
						case 0:
						x--;
						break;
						case 1:
						y--;
						break;
						case 2:
						x++;
						break;
						case 3:
						y++;
						break;
						}
							changearray(x,y);
							break;
						}
					
			}
			else
			{
				if (data[x][y]==symvol)//если рандомные координаты попадут в корабль,то идет срельба по кордам.
				{
					changearray(x,y);
					if(data[x][y + 1] == symvol ||//проверка на корабли
						data[x][y - 1] == symvol ||
						data[x + 1][y] == symvol ||
						data[x - 1][y] == symvol )
					{
						shipneardy=true;//корабли есть рядом
						coordX[0]=x;
						coordY[0]=y;
					}
					break;
				}
				else if(data[x][y]=='-')
				{
					data[x][y]='*';
					arrop[x][y]='*';
					break;
				}
				else if(data[x][y]=='*')
				{
					//ничего не делается,цикл должен работать заново,пока не найдет другую координату
					 x = rand() % (10 - 1 + 1) + 1;
					 y = rand() % (10 - 1 + 1) + 1;
				}
				else if(data[x][y]==crest)
				{
					x = rand() % (10 - 1 + 1) + 1;
					y = rand() % (10 - 1 + 1) + 1;
				}
			}	
			
		}
	}

private:	
	bool is3ship=false;bool is4ship=false;
	char data[11][11];
	char arrop[11][11];//массив соперника который будет скрыт
	int countsquare=20;
	int N = 11;
	int coordX[30]={0};//массив для хранения координат попадания
	int coordY[30]={0};
	bool shipneardy = false;
	int countarray=0;
	int tempdir=0;
	bool deadship=false;
	int firstcordx=0;
	int firstcordy=0;
	bool changearraydir=false;
	int sizeship=4;
	//int ShipId=0;
	
	//int countship = 0;
	
};


class ManuallyCreateShips:public ManuallyShipPlacementInterface//класс для расстоновки кораблей вручную
{
public:
	void gotoxy(int x, int y)
	{
 		printf("%c[%d;%df", 0x1B, y, x);
	}
	void showship(int x,int y,int dir,int sizeship)
	{
		for (int i = 0; i < sizeship; i++)
		{
			
			gotoxy(x+2,y+1);
			cout<<"#";
			switch (dir)
			{
				case 0:
				x+=2;break;
				case 1:
				y++;break;
				case 2:
				x-=2;break;
				case 3:
				y--;break;
			}

		}
		
	}
	void clsrscr(void)
	{
  		system("clear");
	}
	
	void CreateShips(int sizeship,int countsh)override
	{
		int x=1;int y=1;int dir=1;int x1=1;
		//sizeship=4;
		ManuallyCreateShips::countship=countsh;
		int ch;
		clsrscr();
		
		while (true)
		{
			int tempx=x;
			int tempy=y;
			int tempdir=dir;
			PrintData();
			showship(x1 ,y,dir,sizeship);
			//cout<<"\n отработал while"<<endl;
			//ch=getch();
			system("stty raw"); 
			ch=getchar();
			system("stty cooked"); 
			//cout<<endl;
			//изменение коорданты или направления
			switch (ch)
			{
			case 100://d вправо
				x++;x1+=2;break;
			case 115://s down
				y++;break;
			case 97://a left
				x--;x1-=2;break;
			case 119:
				y--;break;
			case 114://r поворот
				dir = !dir;
				break;
			case 13://enter ship
				if(setship(data,x,y,dir,sizeship))//проверка на постановку корабля
				{
					
				}
				
			}
			if(countship==0)
					{
						break;
					}
			if(!shipinmap(x,y,dir,sizeship))
			{
				x=tempx;
				x1=(tempx*2)-1;
				y=tempy;
				dir=tempdir;
			}
			clsrscr();
		}
		
	}
	bool setship(char data[11][11],int x,int y,int dir,int sizeship)
	{
		int tempx=x;
		int tempy=y;
		bool setting_is_possible = 1;			
			for (int i = 0; i < sizeship; i++)
			{	
				if (x < 1 || y < 1 || x >= 11 || y >= 11)
				{
					setting_is_possible = 0;
					break;
				}
				
				if (data[x][y] == symvol ||
					data[x][y + 1] == symvol ||
					data[x][y - 1] == symvol ||
					data[x + 1][y] == symvol ||
					data[x + 1][y + 1] == symvol ||
					data[x + 1][y - 1] == symvol ||
					data[x - 1][y] == symvol ||
					data[x - 1][y + 1] == symvol ||
					data[x - 1][y - 1] == symvol)
				{
					setting_is_possible = 0;//false
					break;
				}
				switch (dir)
				{
					case 0:
					x++;
					break;
					case 1:
					y++;
					break;
					case 2:
					x--;
					break;
					case 3:
					y--;
					break;
				}
			}
			//если есть возмжность поставить корабль
			if(setting_is_possible == 1)
			{ 
				countship--;
				x = tempx;
				y = tempy;	
				for (int i = 0; i < sizeship; i++)//создание 5палбуного коробля
				{
					data[x][y] = symvol;
					switch (dir)
					{
						case 0:				
						x++;
						break;
						case 1:				
						y++;
						break;
						case 2:				
						x--;
						break;
						case 3:					
						y--;
						break;
					}
				}
			}
		return setting_is_possible;
	}
	bool shipinmap(int x,int y,int dir,int sizeship)
	{
		bool inmap = true;
			//проверка возможности постановки корабля
			for (int i = 0; i < sizeship; i++)
			{	
				if (x < 1|| y < 1 || x >= 11 || y >= 11)
				{
					inmap = false;
					break;
				}
				
				switch (dir)
				{
					case 0:
					x++;
					break;
					case 1:
					y++;
					break;
					case 2:
					x--;
					break;
					case 3:
					y--;
					break;
				}
			}
		return inmap;
	}
	void PrintData()
	{
		vsmap.PrintMap(data);
	}
	void setData(char data[11][11]) {
		// копируем данные в массив класса Ship
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {
				this->data[i][j] = data[i][j];
			}
		}
	}
	char(*GetData())[11] {
		return data;
		}
	private:
	Visual vsmap;
	int countship=0;
	char data[11][11];
};

class PlayerGamePlay:public Ship,public ShipPlacementInterface
{	
	//Ship ship;
public:
	
	void PlayerCreateMapShips() override//создает поле для игрока
	{	
		vsmap.CreateMap();
		//cout << "your map:\n" << endl;
		sh.setData(vsmap.GetData());
		sh.CreateShips(1, 4);
		sh.CreateShips(2, 3);
		sh.CreateShips(3, 2);
		sh.CreateShips(4, 1);
		//sh.printDataplayer();
	}
	void PlayerManuallyCreateShips()
	{
		vsmap.CreateMap();
		mcs.setData(vsmap.GetData());
		//vsmap.CreateMap();
		
		mcs.CreateShips(4,1);
		sh.setData(mcs.GetData());
		mcs.CreateShips(3,2);
		sh.setData(mcs.GetData());
		mcs.CreateShips(2,3);
		sh.setData(mcs.GetData());
		mcs.CreateShips(1,4);
		sh.setData(mcs.GetData());
	}
	void ShootToPlayer()
	{
		int x = rand() % (10 - 1 + 1) + 1;
		int y = rand() % (10 - 1 + 1) + 1;
		sh.checkfotbot(x,y);	
	}
	int GetPlayerCountSquare(){
		return sh.GetCountShip();
	}
	void PrintString(int t){
		sh.printstringplayer(t);
			
		
		
		
	}
	private:
	Visual vsmap;
	ManuallyCreateShips mcs;
	Ship sh;
};
class OpponentGamePlay
{
	friend class BotBehavior;
public:
void OppCreateMapShips() //создает поле для игрока
	{	
		vsmap.CreateMap();
		sh.setData(vsmap.GetData());
		sh.CreateShips(1, 4);
		sh.CreateShips(2, 3);
		sh.CreateShips(3, 2);
		sh.CreateShips(4, 1);
		sh.setArrop(vsmap.GetData());//создание поля без видимых кораблей
	}
	int GetOppCountSquare(){
		return sh.GetCountShip();
	}
	void ShootToOpp(int x,int y)
	{
		sh.changearray(x,y);
	}
	void PrintString(int i){
		
			sh.printstringplayer(i);
			cout<<endl;
		
		
		
	}
	void PrintHideString(int i)
	{
		sh.printstringhide(i);
		cout<<endl;
	}
	Ship GetShip(){
		return sh;
	}
	private:
	Visual vsmap;
	Ship sh;
	int i=0;
};


class EndGame
{
public:

	void Win(){

		const char* frames[] = {
			





			"\t██╗░░░██╗░█████╗░██╗░░░██╗  ░██╗░░░░░░░██╗██╗███╗░░██╗\n",//1

			"\t██╗░░░██╗░█████╗░██╗░░░██╗  ░██╗░░░░░░░██╗██╗███╗░░██╗\n"//1
			"\t╚██╗░██╔╝██╔══██╗██║░░░██║  ░██║░░██╗░░██║██║████╗░██║\n",//2

			"\t██╗░░░██╗░█████╗░██╗░░░██╗  ░██╗░░░░░░░██╗██╗███╗░░██╗\n"//1
			"\t╚██╗░██╔╝██╔══██╗██║░░░██║  ░██║░░██╗░░██║██║████╗░██║\n"
			"\t░╚████╔╝░██║░░██║██║░░░██║  ░╚██╗████╗██╔╝██║██╔██╗██║\n",//3
			
			"\t██╗░░░██╗░█████╗░██╗░░░██╗  ░██╗░░░░░░░██╗██╗███╗░░██╗\n"//1
			"\t╚██╗░██╔╝██╔══██╗██║░░░██║  ░██║░░██╗░░██║██║████╗░██║\n"
			"\t░╚████╔╝░██║░░██║██║░░░██║  ░╚██╗████╗██╔╝██║██╔██╗██║\n"//3
			"\t░░╚██╔╝░░██║░░██║██║░░░██║  ░░████╔═████║░██║██║╚████║\n",//4
			
			"\t██╗░░░██╗░█████╗░██╗░░░██╗  ░██╗░░░░░░░██╗██╗███╗░░██╗\n"//1
			"\t╚██╗░██╔╝██╔══██╗██║░░░██║  ░██║░░██╗░░██║██║████╗░██║\n"
			"\t░╚████╔╝░██║░░██║██║░░░██║  ░╚██╗████╗██╔╝██║██╔██╗██║\n"//3
			"\t░░╚██╔╝░░██║░░██║██║░░░██║  ░░████╔═████║░██║██║╚████║\n"//4
			"\t░░╚██╔╝░░██║░░██║██║░░░██║  ░░████╔═████║░██║██║╚████║\n",//5

			"\t██╗░░░██╗░█████╗░██╗░░░██╗  ░██╗░░░░░░░██╗██╗███╗░░██╗\n"//1
			"\t╚██╗░██╔╝██╔══██╗██║░░░██║  ░██║░░██╗░░██║██║████╗░██║\n"
			"\t░╚████╔╝░██║░░██║██║░░░██║  ░╚██╗████╗██╔╝██║██╔██╗██║\n"//3
			"\t░░╚██╔╝░░██║░░██║██║░░░██║  ░░████╔═████║░██║██║╚████║\n"//4
			"\t░░╚██╔╝░░██║░░██║██║░░░██║  ░░████╔═████║░██║██║╚████║\n"//5
			"\t░░░██║░░░╚█████╔╝╚██████╔╝  ░░╚██╔╝░╚██╔╝░██║██║░╚███║\n"//6
			"\t░░░╚═╝░░░░╚════╝░░╚═════╝░  ░░░╚═╝░░░╚═╝░░╚═╝╚═╝░░╚══╝\n",//7
		};

		// Количество кадров анимации
		const int numFrames = sizeof(frames) / sizeof(frames[0]);

		// Время задержки между кадрами (в секундах)
		const int frameDelay = 1;

		// Отрисовка анимации
		
			for (int i = 0; i < numFrames; ++i) {
				// Очищаем консоль
				//std::cout << "\033[2J\033[1;1H"; // Очистка экрана
				system("clear");
				// Выводим текущий кадр
				std::cout << frames[i] << std::endl;

				// Ждем некоторое время перед следующим кадром
				sleep(frameDelay);
			}
	

	}
	void Lose(){

			const char* frames[] = {
			"\t███████╗███╗░░██╗██████╗░  ░██████╗░░█████╗░███╗░░░███╗███████╗\n",//1

			"\t███████╗███╗░░██╗██████╗░  ░██████╗░░█████╗░███╗░░░███╗███████╗\n"//1
			"\t██╔════╝████╗░██║██╔══██╗  ██╔════╝░██╔══██╗████╗░████║██╔════╝\n",//2

			"\t███████╗███╗░░██╗██████╗░  ░██████╗░░█████╗░███╗░░░███╗███████╗\n"//1
			"\t██╔════╝████╗░██║██╔══██╗  ██╔════╝░██╔══██╗████╗░████║██╔════╝\n"//2
			"\t█████╗░░██╔██╗██║██║░░██║  ██║░░██╗░███████║██╔████╔██║█████╗░░\n",//3
			
			"\t███████╗███╗░░██╗██████╗░  ░██████╗░░█████╗░███╗░░░███╗███████╗\n"//1
			"\t██╔════╝████╗░██║██╔══██╗  ██╔════╝░██╔══██╗████╗░████║██╔════╝\n"//2
			"\t█████╗░░██╔██╗██║██║░░██║  ██║░░██╗░███████║██╔████╔██║█████╗░░\n"//3
			"\t██╔══╝░░██║╚████║██║░░██║  ██║░░╚██╗██╔══██║██║╚██╔╝██║██╔══╝░░\n",//4
			
			"\t███████╗███╗░░██╗██████╗░  ░██████╗░░█████╗░███╗░░░███╗███████╗\n"//1
			"\t██╔════╝████╗░██║██╔══██╗  ██╔════╝░██╔══██╗████╗░████║██╔════╝\n"//2
			"\t█████╗░░██╔██╗██║██║░░██║  ██║░░██╗░███████║██╔████╔██║█████╗░░\n"//3
			"\t██╔══╝░░██║╚████║██║░░██║  ██║░░╚██╗██╔══██║██║╚██╔╝██║██╔══╝░░\n"//4
			"\t███████╗██║░╚███║██████╔╝  ╚██████╔╝██║░░██║██║░╚═╝░██║███████╗\n",//5

			"\t███████╗███╗░░██╗██████╗░  ░██████╗░░█████╗░███╗░░░███╗███████╗\n"//1
			"\t██╔════╝████╗░██║██╔══██╗  ██╔════╝░██╔══██╗████╗░████║██╔════╝\n"//2
			"\t█████╗░░██╔██╗██║██║░░██║  ██║░░██╗░███████║██╔████╔██║█████╗░░\n"//3
			"\t██╔══╝░░██║╚████║██║░░██║  ██║░░╚██╗██╔══██║██║╚██╔╝██║██╔══╝░░\n"//4
			"\t███████╗██║░╚███║██████╔╝  ╚██████╔╝██║░░██║██║░╚═╝░██║███████╗\n"//5
			"\t╚══════╝╚═╝░░╚══╝╚═════╝░  ░╚═════╝░╚═╝░░╚═╝╚═╝░░░░░╚═╝╚══════╝\n",//6
		};

		// Количество кадров анимации
		const int numFrames = sizeof(frames) / sizeof(frames[0]);

		// Время задержки между кадрами (в секундах)
		const int frameDelay = 1;

		// Отрисовка анимации
		
			for (int i = 0; i < numFrames; ++i) {
				// Очищаем консоль
				//std::cout << "\033[2J\033[1;1H"; // Очистка экрана
				system("clear");
				// Выводим текущий кадр
				std::cout << frames[i] << std::endl;

				// Ждем некоторое время перед следующим кадром
				sleep(frameDelay);
			}
		}
	
};
class Game
{
public:
	void printTwoMap(string str){
		cout<<"your map:				opponent map:"<<endl;
		if (str=="hide")
		{
			for (int i = 0; i < 11; i++)
			{
				player.PrintString(i);
				cout<<"			";
				opp.PrintHideString(i);
			}
			
		}
		else{
			for (int i = 0; i < 11; i++)
			{
				player.PrintString(i);
				cout<<"			";
				opp.PrintString(i);
			
			}
		}
		
		
	}
	void printbattleship(){
		

			const char* frames[] = {
			"██████╗░░█████╗░████████╗████████╗██╗░░░░░███████╗░██████╗██╗░░██╗██╗██████╗░\n",//1

			"██████╗░░█████╗░████████╗████████╗██╗░░░░░███████╗░██████╗██╗░░██╗██╗██████╗░\n"//1
			"██╔══██╗██╔══██╗╚══██╔══╝╚══██╔══╝██║░░░░░██╔════╝██╔════╝██║░░██║██║██╔══██╗\n",//2

			"██████╗░░█████╗░████████╗████████╗██╗░░░░░███████╗░██████╗██╗░░██╗██╗██████╗░\n"//1
			"██╔══██╗██╔══██╗╚══██╔══╝╚══██╔══╝██║░░░░░██╔════╝██╔════╝██║░░██║██║██╔══██╗\n"//2
			"██████╦╝███████║░░░██║░░░░░░██║░░░██║░░░░░█████╗░░╚█████╗░███████║██║██████╔╝\n",//3
			
			"██████╗░░█████╗░████████╗████████╗██╗░░░░░███████╗░██████╗██╗░░██╗██╗██████╗░\n"//1
			"██╔══██╗██╔══██╗╚══██╔══╝╚══██╔══╝██║░░░░░██╔════╝██╔════╝██║░░██║██║██╔══██╗\n"//2
			"██████╦╝███████║░░░██║░░░░░░██║░░░██║░░░░░█████╗░░╚█████╗░███████║██║██████╔╝\n"//3
			"██╔══██╗██╔══██║░░░██║░░░░░░██║░░░██║░░░░░██╔══╝░░░╚═══██╗██╔══██║██║██╔═══╝░\n",//4
			
			"██████╗░░█████╗░████████╗████████╗██╗░░░░░███████╗░██████╗██╗░░██╗██╗██████╗░\n"//1
			"██╔══██╗██╔══██╗╚══██╔══╝╚══██╔══╝██║░░░░░██╔════╝██╔════╝██║░░██║██║██╔══██╗\n"//2
			"██████╦╝███████║░░░██║░░░░░░██║░░░██║░░░░░█████╗░░╚█████╗░███████║██║██████╔╝\n"//3
			"██╔══██╗██╔══██║░░░██║░░░░░░██║░░░██║░░░░░██╔══╝░░░╚═══██╗██╔══██║██║██╔═══╝░\n"//4
			"██████╦╝██║░░██║░░░██║░░░░░░██║░░░███████╗███████╗██████╔╝██║░░██║██║██║░░░░░\n",//5

			"██████╗░░█████╗░████████╗████████╗██╗░░░░░███████╗░██████╗██╗░░██╗██╗██████╗░\n"//1
			"██╔══██╗██╔══██╗╚══██╔══╝╚══██╔══╝██║░░░░░██╔════╝██╔════╝██║░░██║██║██╔══██╗\n"//2
			"██████╦╝███████║░░░██║░░░░░░██║░░░██║░░░░░█████╗░░╚█████╗░███████║██║██████╔╝\n"//3
			"██╔══██╗██╔══██║░░░██║░░░░░░██║░░░██║░░░░░██╔══╝░░░╚═══██╗██╔══██║██║██╔═══╝░\n"//4
			"██████╦╝██║░░██║░░░██║░░░░░░██║░░░███████╗███████╗██████╔╝██║░░██║██║██║░░░░░\n"//5
			"╚═════╝░╚═╝░░╚═╝░░░╚═╝░░░░░░╚═╝░░░╚══════╝╚══════╝╚═════╝░╚═╝░░╚═╝╚═╝╚═╝░░░░░\n",//6
		};

		// Количество кадров анимации
		const int numFrames = sizeof(frames) / sizeof(frames[0]);

		// Время задержки между кадрами (в секундах)
		const int frameDelay = 1;

		// Отрисовка анимации
		
			for (int i = 0; i < numFrames; ++i) {
				// Очищаем консоль
				//std::cout << "\033[2J\033[1;1H"; // Очистка экрана
				system("clear");
				// Выводим текущий кадр
				std::cout << frames[i] << std::endl;

				// Ждем некоторое время перед следующим кадром
				sleep(frameDelay);
			}
		}
	
	int StartGame() 
	{		
		
		
		int x,y;
			int choise;
			cout<<"\n\t █▀▄▀█ █▀▀ █▄░█ █░█";
			cout<<"\n\t █░▀░█ ██▄ █░▀█ █▄█";
			cout<<"\n\t1.Автоматическая расстоновка";
			cout<<"\n\t2.Ручная расстоновка(ДВИЖЕНИЕ КОРАБЛЯ ЧЕРЕЗ 'WASD',ПОВОРОТ НА 'R',ПОСТАНОВКА КОРАБЛЯ НА 'ENTER')";
			
			while(true)
			{
				cout<<"\nВведите номер:";cin>>choise;
				if(choise==1){
					player.PlayerCreateMapShips();
					cout<<"\n\tИГРА НАЧИНАЕТСЯ"<<endl;
					sleep(1);
					break;
				}
				else if(choise==2){
					cout<<"\n\tДВИЖЕНИЕ КОРАБЛЯ ЧЕРЕЗ 'WASD',ПОВОРОТ НА 'R',ПОСТАНОВКА КОРАБЛЯ НА 'ENTER'";
					
					player.PlayerManuallyCreateShips();
					cout<<"\n\tИГРА НАЧИНАЕТСЯ"<<endl;
					sleep(1);
					break;
				}
				else{}
			}
			system("clear");
			char letter;
			opp.OppCreateMapShips();
			printTwoMap("hide");
		while (player.GetPlayerCountSquare()!=0)
		{
			bool bl=true;
			//bot.Shoot(opp);

			if(player.GetPlayerCountSquare()==0){
				e.Lose();
			}
			if(opp.GetOppCountSquare()==0)
			{
				cout<<"U WIN"<<endl;
				return 0;
			}
			while (bl)
			{
				cout<<"\nВведите букву:"<<endl;
				cin>>letter;
				string str;
					switch (letter)
				{
				case 'a':x=1;bl=false;break;
				case 'b':x=2;bl=false;break;
				case 'c':x=3;bl=false;break;
				case 'd':x=4;bl=false;break;
				case 'e':x=5;bl=false;break;
				case 'f':x=6;bl=false;break;
				case 'g':x=7;bl=false;break;
				case 'h':x=8;bl=false;break;
				case 'i':x=9;bl=false;break;
				case 'j':x=10;bl=false;break;
				case '1':x=1;bl=false;break;
				case '2':x=2;bl=false;break;
				case '3':x=3;bl=false;break;
				case '4':x=4;bl=false;break;
				case '5':x=5;bl=false;break;
				case '6':x=6;bl=false;break;
				case '7':x=7;bl=false;break;
				case '8':x=8;bl=false;break;
				case '9':x=9;bl=false;break;
				case '0':x=10;bl=false;break;
				default:
					cout<<"\nВведена не корректная координата,попробуйте еще раз:"<<endl;
					break;
				}
			}
			system("clear");
			printTwoMap("hide");
			while(true){
				cout<<"\nВведите цифру:"<<endl;
				cin>>y;
				if(y==0){y=10;}
				if(y<=10){break;}//all good
				//if(y>10){}//again while
			}
			
			//cin>>x;


			player.ShootToPlayer();
			opp.ShootToOpp(x,y);
			//printTwoMap("hide");
			system("clear");
			printTwoMap("hide");
			cout<<"стрельба по игроку"<<endl;
			
			if(x==0){return 0;}
			
			//printTwoMap("");
			// printTwoMap("hide");
		}
		e.Lose();
		return 0;
	}
	
private:
	EndGame e;
	Visual vs;
	PlayerGamePlay player;
	OpponentGamePlay opp;
	//BotBehavior bot;
	int countsquareplayer=10;
	int countsquareopp=10;
	
};

int main() 
{
	 setlocale(LC_CTYPE,"");
	srand(time(NULL));
	setlocale(LC_ALL, "ru");
	Game game;
	game.StartGame();
	return 0;
}