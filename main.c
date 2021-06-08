#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>   
#include <time.h>
#include <string.h>
//#include <stdio.h>

struct snake {

  char piece;
  int y,x;
  int y_ant,x_ant;

  struct snake *nextPiece;
};

struct apple {

  char apple;
  int apple_dir_y;
  int apple_dir_x;

};

struct opcion{
    char opc_menu[11];
}n[2];


struct snake *addNewPiece(const char piece,int y,int x) {
  struct snake *newPiece = NULL;
  newPiece = /*(snake *)*/malloc(sizeof(struct snake));

  if (newPiece != NULL) {
    newPiece->nextPiece = NULL;
    newPiece->piece = piece;
    newPiece->y = y;
    newPiece->x = x;
    newPiece->y_ant = newPiece->y;
    newPiece->x_ant = newPiece->x;
  } else {
    endwin();
    printf("mem aloc failure, the program couldn't alloc the memory correctly");
  }
  return newPiece;
}
struct snake *insertAtBeginning(struct snake *head){

  struct snake *temp = malloc(sizeof(struct snake));
  struct snake *c;

  c = head;

  temp->piece = '#';
  temp->y = c->y_ant;
  temp->x = c->x_ant;

  if(head != NULL) temp->nextPiece = head;

  head = temp;

  return head;

}

void printinscreen(struct snake *piece, WINDOW *win, struct apple a, int yMaxWin, int xMaxWin, int dir, int dir_ant);
void moveSnakeCords(WINDOW* win, struct snake *piece, int dir, int* dir_ant);
void checkAppleHit(struct snake *piece, struct apple a, int *applecheck, int *ins);
void checkDeath(struct snake *piece, int yMaxWin, int xMaxWin, int *muerte);
void menu();
void FillMenu();
void CleanUp(struct snake *list);

int main() {

  srand(time(NULL));

  int i = 0;
  int muerte = 0;

  struct snake *first = NULL;
  struct snake *added = NULL;

  initscr();
  noecho();

  int yMaxStd, xMaxStd;

  getmaxyx(stdscr, yMaxStd, xMaxStd);

  WINDOW *win = newwin(20, 50, (yMaxStd / 2) - 10, 10);
  //leaveok(win, true);
  box(win,0,0);
  wrefresh(win);
  curs_set(0);
  keypad(win, TRUE);
  nodelay(win, TRUE);

  int yMaxWin, xMaxWin;

  getmaxyx(win, yMaxWin, xMaxWin);
  int y = (yMaxWin/2) + 3, x = xMaxWin/2 - 1;

  struct apple a;

  wmove(win, yMaxWin / 2, yMaxWin / 2);

  int temp = 0;

  int ins = 0;
  int f = 0;
  int firstloop = 0;

  // llenar lista con 4 #
  while (i <= 4) {
    if(i < 4){
      if (first == NULL) {
        first = addNewPiece('#',y,x);
        y--;
      if (first != NULL) {
        added = first;
      }
      } else {
        added->nextPiece = addNewPiece('#',y,x);
        y--;
        if (added->nextPiece != NULL) {
          added = added->nextPiece;
        }
      }
    } else{
      added->nextPiece = addNewPiece('@',y,x);
      y--;
      if (added->nextPiece != NULL) {
        added = added->nextPiece;
    }
    
  }
    ++i;
  }

  if (first == NULL) {
    endwin();
    printf("Men alloc failure.");
  }

  int dir = 0;
  int dir_ant = 0;
  int applecheck = 0;
  int exit = 0;

  a.apple = 'Q';

  //box(win,0,0);

  exit = 0;

  while(1){

    if(exit == 1){

      i = 0;
      muerte = 0;

      first = NULL;
      added = NULL;

      //initscr();
      //noecho();

      //int yMaxStd, xMaxStd;

      getmaxyx(stdscr, yMaxStd, xMaxStd);

      //WINDOW *win = newwin(20, 50, (yMaxStd / 2) - 10, 10);
      //leaveok(win, true);
      //curs_set(0);
      //keypad(win, TRUE);
      //nodelay(win, TRUE);

      //int yMaxWin, xMaxWin;

      getmaxyx(win, yMaxWin, xMaxWin);
      y = (yMaxWin/2) + 3, x = xMaxWin/2 - 1;

      //struct apple a;

      wmove(win, yMaxWin / 2, yMaxWin / 2);

      temp = 0;

      ins = 0;
      f = 0;
      firstloop = 0;

      // llenar lista con 4 #
      while (i <= 4) {
        if(i < 4){
          if (first == NULL) {
            first = addNewPiece('#',y,x);
            y--;
          if (first != NULL) {
            added = first;
          }
          } else {
            added->nextPiece = addNewPiece('#',y,x);
            y--;
            if (added->nextPiece != NULL) {
              added = added->nextPiece;
            }
          }
        } else{
          added->nextPiece = addNewPiece('@',y,x);
          y--;
          if (added->nextPiece != NULL) {
            added = added->nextPiece;
        }
        
      }
        ++i;
      }

      if (first == NULL) {
        endwin();
        printf("Men alloc failure.");
      }

      dir = 0;
      dir_ant = 0;
      applecheck = 0;
      //exit = 0;

      //box(win,0,0);

      exit = 0;
    }

    int key = wgetch(win);
    int box = 0;
    if (box == 0){
      box(win,0,0);
      box = 1;
    }
    //dir = 1;

    //wclear(win);   

    //box(win, 0, 0);
    //usleep(510000);
    //printinscreen(first, win, dir); 

    
    switch(key){
      case KEY_UP: dir = 1; break;
      case KEY_DOWN: dir = 2; break;
      case KEY_LEFT: dir = 3; break;
      case KEY_RIGHT: dir = 4; break;
    }

    if (applecheck == 0){

      a.apple_dir_y = rand()%yMaxWin;
      a.apple_dir_x = rand()%xMaxWin;

      while(1){
       if(a.apple_dir_y == 0 || a.apple_dir_y == yMaxWin || a.apple_dir_y == yMaxWin - 1){
          a.apple_dir_y = rand()%yMaxWin;
          continue;
       }
       if(a.apple_dir_x == 0 || a.apple_dir_x == xMaxWin || a.apple_dir_x == xMaxWin - 1){
         a.apple_dir_x = rand()%xMaxWin;
          continue;
        }
        break;
     }

     applecheck = 1;
      
    }

    if (f == 0){
      //checkDeath(first, yMaxWin, xMaxWin, &muerte, &firstloop);
      if(muerte == 1){
        //break;
        wclear(win);
        wrefresh(win);
        //box(win, 0, 0);
        menu(&exit);
        if (exit == 1){
          break;
        } else{
          exit = 1;
        }
        
        //wclear(menu);
        //box(win, 0, 0);
        //sleep(2);
      }
      moveSnakeCords(win, first, dir, &dir_ant);
      checkAppleHit(first, a, &applecheck, &ins);
      printinscreen(first, win, a, yMaxWin, xMaxWin, dir, dir_ant); 
      refresh();
      wrefresh(win);
      checkDeath(first, yMaxWin, xMaxWin, &muerte);
      usleep(150000);
      f = 1;
    } else{
      checkDeath(first, yMaxWin, xMaxWin, &muerte);
      if(muerte == 1){
        //break;
        wclear(win);
        wrefresh(win);
        //box(win, 0, 0);
        menu(&exit);
        if (exit == 1){
          break;
        } else{
          exit = 1;
        }
        //break;
        //wclear(menu);
        //box(win, 0, 0);
        //sleep(2);
      }
      checkAppleHit(first, a, &applecheck, &ins);
      if(ins == 1){
        first = insertAtBeginning(first);
        ins = 0;
      }
      moveSnakeCords(win, first, dir, &dir_ant);
      printinscreen(first, win, a, yMaxWin, xMaxWin, dir, dir_ant); 
      refresh();
      wrefresh(win);
      //if (ins != 0){
      firstloop = 1;
      //}
      usleep(150000);
    }
    if(exit == 1){
      CleanUp(first);
      //clear();
      wclear(win);
      //clear();
      //wrefresh(win);
      //wrefresh(win);
      //refresh();
      //continue;
    }
  }

  //getch();
  if(exit == 1){
    endwin();
    CleanUp(first);
    return 0;
  }
  //refresh();
  //printf("has muerto xd");
  //getchar();
  
}

void CleanUp(struct snake *list){

    struct snake *next;
    while(list)
    {
        next = list->nextPiece;
        //printf("Cleaning %d\n", list->age);
        free(list);
        list = next;
    }
}

void FillMenu(){
  strcpy(n[0].opc_menu,"Play again");
  strcpy(n[1].opc_menu,"Exit");
}

void menu(int* exit){

  FillMenu();

  int xMax,yMax;

  getmaxyx(stdscr,yMax,xMax);

  WINDOW* menuwin = newwin(6,xMax-12,yMax-8,5);
  box(menuwin,0,0);
  refresh();
  wrefresh(menuwin);

  keypad(menuwin, TRUE);

  int opc;
  int marca = 0;

  while(1){
    for(int i = 0;i < 2;i++){
        if(i == marca){

            wattron(menuwin, A_REVERSE);

        }

        mvwprintw(menuwin,i+1,1,"%s",n[i].opc_menu);
        wattroff(menuwin, A_REVERSE);
    }

    int c = wgetch(menuwin);

    switch(c){

      case KEY_UP: marca--;
          if(marca == -1){
            marca = 0; 
            break;
          }
          break;

      case KEY_DOWN: marca++;
          if(marca == 2){
            marca = 1;
            break;
          }
          break;

      default: break;

  }
  if(c == 10)
      break;
  }

  if (marca == 1){
    *exit = 1;
  }

  wclear(menuwin);
  wrefresh(menuwin);

  //printw("Has elegido %s",n[marca].opc_menu);

}

void checkDeath(struct snake *piece, int yMaxWin, int xMaxWin, int *muerte){

  struct snake *t;
  struct snake *c;
  struct snake *p;

  t = piece;
  p = piece;
  c = piece;

  if (t == NULL){
    endwin();
  } else{
    while(t->nextPiece != NULL){
      t = t->nextPiece;
    }
  }
  if(t == NULL){
    endwin();
  } else{
    if(t->y == 0 || t->y == yMaxWin - 1){
      //printf("Has muerto\n");
      *muerte = 1;
    }
    if(t->x == 0 || t->x == xMaxWin - 1){
      //  printf("Has muerto\n");
      *muerte = 1;
    }
  }

  if (p == NULL){
    endwin();
  } else{
    while(p->nextPiece != NULL){
      p = p->nextPiece;
    }
    while(c->nextPiece->nextPiece != NULL){
      if (p->y == c->y && p->x == c->x){
        *muerte = 1;
      }
      c = c->nextPiece;
      
    }
  }

}


void checkAppleHit(struct snake *piece, struct apple a, int *applecheck, int *ins){

  struct snake *t;

  t = piece;

  if (t == NULL){
    endwin();
  } else{
    while(t->nextPiece != NULL){
      t = t->nextPiece;
    }
    if(t->y == a.apple_dir_y && t->x == a.apple_dir_x){
    *applecheck = 0;
    *ins = 1;
    }
  }


}



void moveSnakeCords(WINDOW* win, struct snake *piece, int dir, int* dir_ant){

  struct snake *t;
  struct snake *c;

  t = piece;
  c = piece;

  //*dir_ant = dir;
  
  switch(*dir_ant){
    case 1: if(dir == 1){dir = 2;}; break;
    case 2: if(dir == 2){dir = 1;}; break;
    case 3: if(dir == 3){dir = 4;}; break;
    case 4: if(dir == 4){dir = 3;}; break;
  }
  

  switch(dir){
    case 1:
      if (t == NULL){
      endwin();
      } else{
        while(t != NULL){
          if(t->nextPiece != NULL){
            t->y_ant = t->y;
            t->x_ant = t->x;
            t->y = t->nextPiece->y;
            t->x = t->nextPiece->x;
            t = t->nextPiece;
          } else{
            t->y = t->y - 1;
            t = t->nextPiece;
          }
        }
    }; break;
    case 2:
      if (t == NULL){
      endwin();
      } else{
        while(t != NULL){
          if(t->nextPiece != NULL){
            t->y_ant = t->y;
            t->x_ant = t->x;
            t->y = t->nextPiece->y;
            t->x = t->nextPiece->x;
            t = t->nextPiece;
          } else{
            t->y = t->y + 1;
            t = t->nextPiece;
          }
        }
    }; break;
    case 3:
      if (t == NULL){
      endwin();
      } else{
        while(t != NULL){
          if(t->nextPiece != NULL){
            t->y_ant = t->y;
            t->x_ant = t->x;
            t->y = t->nextPiece->y;
            t->x = t->nextPiece->x;
            t = t->nextPiece;
          } else{
            t->x = t->x - 1;
            t = t->nextPiece;
          }
        }
    }; break;
    case 4:
      if (t == NULL){
      endwin();
      } else{
        while(t != NULL){
          if(t->nextPiece != NULL){
            t->y_ant = t->y;
            t->x_ant = t->x;
            t->y = t->nextPiece->y;
            t->x = t->nextPiece->x;
            t = t->nextPiece;
          } else{
            t->x = t->x + 1;
            t = t->nextPiece;
          }
        }
    }; break;
  }
  
  switch(dir){

    case 1: *dir_ant = 2; break;
    case 2: *dir_ant = 1; break;
    case 3: *dir_ant = 4; break;
    case 4: *dir_ant = 3; break;

  }


}

void printinscreen(struct snake *piece, WINDOW *win, struct apple a, int yMaxWin, int xMaxWin, int dir, int dir_ant) {

  struct snake *t;
  struct snake *c;

  c = piece;
  t = piece;
  
  if (t == NULL){
    endwin();
  } else {
    mvwaddch(win, t->y_ant, t->x_ant,' ');
    while(t != NULL){
      mvwprintw(win, t->y, t->x, "%c", t->piece);
      t = t->nextPiece;
    }
  }

  mvwaddch(win, c->y_ant, c->x_ant,' ');

  while(c->nextPiece != NULL){
    c = c->nextPiece;
  }

  mvwprintw(win, a.apple_dir_y, a.apple_dir_x, "%c", a.apple);

  //mvwprintw(win, 0, 0, "Dir: %d  Dir_ant: %d", dir, dir_ant);
  //mvwprintw(win, 1, 0, "Head: X[%d] Y[%d]",c->x,c->y);
  //mvwprintw(win,yMaxWin - 1, 0, "Apple: X[%d] Y[%d]",a.apple_dir_x,a.apple_dir_y);

}


