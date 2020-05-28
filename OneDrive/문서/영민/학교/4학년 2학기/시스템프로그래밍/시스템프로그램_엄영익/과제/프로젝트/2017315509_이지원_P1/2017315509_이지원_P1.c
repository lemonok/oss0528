#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <limits.h> 
#include <math.h>
struct Node {
   struct Node* parent;
   struct Node* child[16];
   char* board;
   int win;
   int all;
   double UCT;
   int visited;
   int child_cnt;
}typedef Node;

Node* selection(Node* root);
int modular(int input, int value);
int end_game(char* board);
int random_play(Node* curr);
double UCTFunction(Node* curr2, double C);
int my_strlen(char* str);
void my_strcpy(char* dest, const char* origin);
void expansion(Node *curr);
void updating(Node* curr, Node* root, int winorlose, double C);
void resultPrint(Node * root);
void root_init(Node* root, int boardsize);
double ln(double x);
double my_sqrt(double input);

int main()
{
   int random_try = 250000;
   double C = 0.5;
   Node* root = (Node*)malloc(sizeof(Node));
   root_init(root,16);
   while (random_try--)
   {
      Node* curr = selection(root);
      expansion(curr); // expansion
      int winorlose = random_play(curr); // evaluation
      updating(curr, root, winorlose, C); // update
      printf("%d번남았습니다\n", random_try);
      resultPrint(root);
   }
   
   return 0;
}

double my_sqrt(double input)
{
   /*
   double x = 2;
   for (int i = 0; i < 10; i++) 
   {
      x = (x + (input / x)) / 2; 
   }
   return x;
   */
   return sqrt(input);
}

void node_init(Node* node)
{
   node->parent = NULL;
   for (int i = 0; i < 16; i++)
      node->child[i] = NULL;
   node->board = (char*)malloc(sizeof(char) * 17);
   node->win = 0;
   node->all = 0;
   node->UCT = INT_MAX;
   node->visited = 0;
   node->child_cnt = 0;

}

double ln(double x)
{
   return log(x);
}


Node* selection(Node* root)
{
   Node* curr = root;
   while (curr->visited)
   {
      double max_UCT = INT_MIN;
      int max_i = -1;
      for (int i = 0; i < curr->child_cnt; i++)
      {
         if (max_UCT < curr->child[i]->UCT)
         {
            max_UCT = curr->child[i]->UCT;
            max_i = i;
         }
      }
      curr = curr->child[max_i];
   }
   return curr;
}

int modular(int input, int value)
{
   int result;
   result = input % value;

   return result;
}

int end_game(char* board)
{
   for (int i = 0; i < 4; i++) // 가로3줄
   {
      if (board[i * 4] == board[i * 4 + 1] && board[i * 4 + 1] == board[i * 4 + 2] && board[i * 4 + 2] == board[i * 4 + 3])
      {
         if (board[i * 4] == 'X')
            return 1;
         else if (board[i * 4] == 'O')
            return 2;
      }
      else if (board[i] == board[4 + i] && board[4 + i] == board[8 + i] && board[8 + i] == board[12 + i])
      {
         if (board[i] == 'X')
            return 1;
         else if (board[i] == 'O')
            return 2;
      }
   }

   if (board[0] == board[5] && board[5] == board[10] && board[10] == board[15])
   {
      if (board[0] == 'X')
         return 1;
      else if (board[0] == 'O')
         return 2;
   }

   if (board[3] == board[6] && board[6] == board[9] && board[9] == board[12])
   {
      if (board[3] == 'X')
         return 1;
      else if (board[3] == 'O')
         return 2;
   }

   return 0;
}

int random_play(Node* curr)
{
   srand(time(NULL));
   char* cur = curr->board;
   while (1)
   {
      int result = end_game(cur);
      if (result == 1)
         return 1;
      else if (result == 2)
         return 0;

      int check = 0;
      for (int i = 0; i < 16; i++)
         if (cur[i] == '-')
            check++;

      if (check == 0)
         return 0;

      char nextPlayer;

      if (modular(check, 2) == 0)
         nextPlayer = 'X';
      else
         nextPlayer = 'O';

      int ran = rand() % 16;

      while (cur[ran] != '-')
         ran = modular(rand(), 16);

      cur[ran] = nextPlayer;


   }
}

double UCTFunction(Node* curr2, double C)
{
   Node* curr = curr2;

   double a, b;
   a = (double)curr->win / (double)curr->all;
   b = 2 * C * sqrt(2 * ln((double)curr->parent->all) / (double)curr->all);

   return a + b;
}

int my_strlen(char* str)
{
   int i = 0;
   for (i = 0; str[i] != '\0'; i++);
   return i;
}

void my_strcpy(char* dest, const char* origin)
{
   int i;
   for (i = 0; i < my_strlen(origin); i++)
      dest[i] = origin[i];
   dest[i] = '\0';
}

void expansion(Node *curr)
{
   int check = 0;
   for (int i = 0; i < 16; i++)
      if (curr->board[i] == '-')
         check++;
   char nextPlayer;

   if (modular(check, 2) == 0)
      nextPlayer = 'X';
   else
      nextPlayer = 'O';

   for (int i = 0; i < 16; i++)
   {
      if (curr->board[i] == 'X' || curr->board[i] == 'O')
         continue;

      Node* child = (Node*)malloc(sizeof(Node));
      node_init(child);
      my_strcpy(child->board, curr->board);
      child->board[i] = nextPlayer;
      child->parent = curr;
      curr->child[curr->child_cnt++] = child;
   } // child들 미리 만들어두기 
   curr->visited = 1;
}

void updating(Node* curr, Node* root, int winorlose, double C)
{
   curr->all += 1;
   curr->win += winorlose;
   while (curr != root)
   {
      curr->parent->all += 1;
      curr->parent->win += winorlose;
      curr = curr->parent;
      if (curr != root)
         curr->UCT = UCTFunction(curr, C);
      for (int i = 0; i < curr->child_cnt; i++)
      {
         Node* child = curr->child[i];
         if (child->all != 0)
            child->UCT = UCTFunction(child, C);
      }
   }

}

void resultPrint(Node * root)
{

   printf("루트 %d %d %lf\n", root->all, root->win, root->UCT);

   double max_UCT = -1;
   int max_i;
   for (int i = 0; i < root->child_cnt; i++)
   {
      printf("%d번째: %d %d %lf\n", i + 1, root->child[i]->all, root->child[i]->win, root->child[i]->UCT);

      if (max_UCT < root->child[i]->UCT)
      {
         max_UCT = root->child[i]->UCT;
         max_i = i;
      }
   }

   printf("Best A Player: %d %lf\n\n", max_i + 1, max_UCT);
   double max_UCT2 = -1;
   int max_i2 = -1;
   for (int i = 0; i < root->child[max_i]->child_cnt; i++)
   {
      printf("%d번째 %d %d %lf\n", i + 1, root->child[max_i]->child[i]->all, root->child[max_i]->child[i]->win, root->child[max_i]->child[i]->UCT);
      if (root->child[max_i]->child[i]->UCT != INT_MAX && max_UCT2 < root->child[max_i]->child[i]->UCT)
      {
         max_UCT2 = root->child[max_i]->child[i]->UCT;
         max_i2 = i;
      }
   }
   printf("Best B Player: %d %lf\n", max_i2 + 1, max_UCT2);

}

void root_init(Node* root, int boardsize)
{
   node_init(root);
   for (int i = 0; i < boardsize; i++)
      root->board[i] = '-';
   root->board[boardsize] = '\0';
   root->visited = 1;
   for (int i = 0; i < boardsize; i++)
   {
      Node* child = (Node*)malloc(sizeof(Node));
      node_init(child);
      my_strcpy(child->board, root->board);
      child->board[i] = 'X';
      child->parent = root;
      root->child[root->child_cnt++] = child;
   }
}