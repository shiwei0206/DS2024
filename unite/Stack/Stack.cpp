// 4.1Stack模板类
#include <vector>
template <typename T>
class Stack
{
public:
    void push(const T &item)
    {
        items.push_back(item);
    }

    T pop()
    {
        if (!is_empty())
        {
            T item = items.back();
            items.pop_back();
            return item;
        }
        throw out_of_range("Stack is empty");
    }

    bool is_empty() const
    {
        return items.empty();
    }

private:
    vector<T> items;
};

// // 4.2进制转化算法(递归版)
// void convert(Stack<char> &S, __int64 n, int base)
// {
//     static char digit[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
//     if (0 < n)
//     {
//         convert(S, n / base, base);
//         S.push(digit[n % base]);
//     }
// }

// // 4.3进制转化算法(迭代版)
// void convert(Stack<char> &S, __int64 n, int base)
// {
//     static char digit[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
//     while (n > 0)
//     {
//         int remainder = (int)(n % base);
//         S.push(digit[remainder]);
//         n /= base;
//     }
// }

// // 4.4括号匹配算法(递归版)
// void trim(const char exp[], int &lo, int &hi)
// {
//     while ((lo <= hi) && (exp[lo] != '(') && (exp[lo] != '('))
//         lo++;
//     while ((lo <= hi) && (exp[hi] != '(') && (exp[hi] != ')'))
//         hi--;
// }

// int divide(const char exp[], int lo, int hi)
// {
//     int mi = lo;
//     int crc = 1;
//     while ((0 < crc) && (++mi < hi))
//     {
//         if (exp[mi] == ')')
//             crc--;
//         if (exp[mi] == '(')
//             crc++;
//     }
//     return mi;
// }

// bool paren(const char exp[], int lo, int hi)
// {
//     trim(exp, lo, hi);
//     if (lo > hi)
//         return true;
//     if (exp[lo] != '(')
//         return false;
//     if (exp[hi] != ')')
//         return false;
//     int mi = divide(exp, lo, hi);
//     if (mi > hi)
//         return false;
//     return paren(exp, lo + 1, mi - 1) && paren(exp, mi + 1, hi);
// }
// 4.5括号匹配算法(栈版)
// bool paren(const char exp[], int lo, int hi)
// {
//     Stack<char> S;
//     for (int i = 0; exp[i]; i++)
//         switch (exp[i])
//         {
//         case '(':
//         case '[':
//         case '{':
//             S.push(exp[i]);
//             break;
//         case ')':
//             if ((S.empty()) || ('(' != S.pop()))
//                 return false;
//             break;
//         case ']':
//             if ((S.empty()) || ('[' != S.pop()))
//                 return false;
//             break;
//         case '}':
//             if ((S.empty()) || ('{' != S.pop()))
//                 return false;
//             break;
//         default:
//             break;
//         }
//     return S.empty();
// }

// 4.6运算符优先级关系的定义
#define N_OPTR 9 // 运算符总数
typedef enum
{
    ADD,
    SUB,
    MUL,
    DIV,
    POW,
    FAC,
    L_P,
    R_P,
    EOE
} Operator;
const char pri[N_OPTR][N_OPTR] = {
    '>', '>', '<', '<', '<', '<', '<', '>', '>',
    '>', '>', '<', '<', '<', '<', '<', '>', '>',
    '>', '>', '>', '>', '<', '<', '<', '>', '>',
    '>', '>', '>', '>', '<', '<', '<', '>', '>',
    '>', '>', '>', '>', '>', '<', '<', '>', '>',
    '>', '>', '>', '>', '>', '>', ' ', '>', '>',
    '<', '<', '<', '<', '<', '<', '<', '=', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    '<', '<', '<', '<', '<', '<', '<', ' ', '='};

// // 4.7表达式的求值及RPN转换
//  float evaluate(char *S, char *&RPN)
// {
//     Stack<float> opnd;
//     Stack<char> optr;
//     optr.push('\0');
//      while (!optr.empty())
//     {
//          if (isdigit(*S))
//         {
//              readNumber(S, opnd);
//             append(RPN, opnd.top());

//         }
//         else // 若弼前字符为运算符，则
//              switch (orderBetween(optr.top(), *S))
//         {
//         case '<':
//              optr.push(*S);
//             S++;
//             11 break;
//          case '=':
//              optr.pop();
//             S++;
//              break;
//          case '>':
//         {
//              char op = optr.pop();
//             append(RPN, op);
//              if ('!' == op)
//             {
//                  float pOpnd = opnd.pop();
//                  opnd.push(calcu(op, pOpnd));

//             }
//             else
//             {
//                  float pOpnd2 = opnd.pop(), pOpnd1 = opnd.pop();
//                  opnd.push(calcu(pOpnd1, op, pOpnd2));

//             }
//              break;

//         }
//              default : exit(-1);

//         }

//     }
//      return opnd.pop();
// }

// // 4.8皇后类
// struct Queen
// {
//     int x, y;
//     Queen(int xx = 0, int yy = 0) : x(xx), y(yy) {};
//     bool operator==(Queen const &q) const
//     {
//         return (x == q.x) || (y == q.y) || (x + y == q.x + q.y) || (x - y == q.x - q.y);
//     }
//     bool operator!=(Queen const &q) const { return !(*this == q); }
// };

// // 4.9 N皇后算法
// void placeQueens(int N)
// {
//     Stack<Queen> solu;
//     Queen q(0, 0);
//     do
//     {
//         if (N <= solu.size() || N <= q.y)
//         {
//             q = solu.pop();
//             q.y++;
//         }
//         else
//         {
//             while ((q.y < N) && (0 <= solu.find(q)))
//             {
//                 q.y++;
//                 // nCheck++;
//             }
//             if (q.y < N)
//                 solu.push(q);
//             if (N <= solu.size()) // nSolu++;
//                 q.x++;
//             q.y = 0;
//         }
//     } while ((0 < q.x) || (q.y < N));
// }

// // 4.10迷宫格点法
// typename enum { AVAILABLE,
//                 ROUTE,
//                 BACKTRACKED,
//                 WALL } Status;

// typedef enum
// {
//     UNKNOWN,
//     EAST,
//     SOUTH,
//     WEST,
//     NORTH,
//     NO_WAY
// } ESWN;

// inline ESWN nextESWN(ESWN eswn)
// {
//     return ESWN(eswn + 1);
// }

// struct Cell
// {
//     int x, y;
//     Status status;
//     ESWN incoming, outgoing;
// };

// #define LABY_MAX 24 // 迷宫最大尺寸
// Cell laby[LABY_MAX][LABY_MAX];

// // 4.11查询相邻格点
// inline Cell *neighbor(Cell *cell)
// {
//     switch (cell->outgoing)
//     {
//     case EAST:
//         return cell + LABY_MAX;
//     case SOUTH:
//         return cell + 1;
//     case WEST:
//         return cell - LABY_MAX;
//     case NORTH:
//         return cell - 1;
//     default:
//         exit(-1);
//     }
// }

// 4.12转入相邻格点
// inline Cell *advance(Cell *cell)
// {
//     Cell *next;
//     switch (cell->outgoing)
//     {
//     case EAST:
//         next = cell + LABY_MAX;
//         next->incoming = WEST;
//         break;
//     case SOUTH:
//         next = cell + 1;
//         next->incoming = NORTH;
//         break;
//     case WEST:
//         next = cell - LABY_MAX;
//         next->incoming = EAST;
//         break;
//     case NORTH:
//         next = cell - 1;
//         next->incoming = SOUTH;
//         break;
//     default:
//         exit(-1);
//     }
//     return next;
// }

// // 4.13 迷宫寻径
// bool labyrinth(Cell Laby[LABY_MAX][LABY_MAX], Cell *s, Cell *t)
// {
//     if ((AVAILABLE != s->status) || (AVAILABLE != t->status))
//         return false;
//     Stack<Cell *> path;
//     s->incoming = UNKNOWN;
//     s->status = ROUTE;
//     path.push(s);
//     do
//     {
//         Cell *c = path.top();
//         if (c == t)
//             return true;
//         while (NO_WAY > (c->outgoing = nextESWN(c->outgoing)))
//             if (AVAILABLE == neighbor(c)->status)
//                 break;
//         if (NO_WAY <= c->outgoing)
//         {
//             c->status = BACKTRACKED;
//             c = path.pop();
//         }
//         else
//         {
//             path.push(c = advance(c));
//             c->outgoing = UNKNOWN;
//             c->status = ROUTE;
//         }
//     } while (!path.empty());
//     return false;
// }

// // 4.14Queue模板类
// #include "../List/List.h"
// template <typename T>
// class Queue : public List<T>
// {
// public:
//     void enqueue(T const &e) { insertAsLast(e); }
//     5 T dequeue() { return remove(first()); }
//     T &front() { return first()->data; }
// };

// 算法4.2
//  RoundRobin {
//   Queue Q(clients);
//   while (!ServiceClosed()) {
//   e = Q.dequeue();
//   serve(e);
//   Q.enqueue(e);
//   }
//  }

// 4.15顾客对象
struct Customer
{
    int window;
    unsigned int time;
};

// // 4.16银行服务模拟
// void simulate(int nWin, int servTime)
// {
//     Queue<Customer> *windows = new Queue<Customer>[nWin];
//     for (int now = 0; now < servTime; now++)
//     {
//         if (rand() % (1 + nWin))
//         {
//             Customer c;
//             c.time = 1 + rand() % 98;
//             c.window = bestWindow(windows, nWin);
//             windows[c.window].enqueue(c);
//         }
//         for (int i = 0; i < nWin; i++)
//             if (!windows[i].empty())
//                 if (--windows[i].front().time <= 0)
//                     windows[i].dequeue();
//     }
//     delete[] windows;
// }

// // 4.17查找最短队列
// int bestWindow(Queue<Customer> windows[], int nWin)
// {
//     int minSize = windows[0].size(), optiWin = 0;
//     for (int i = 1; i < nWin; i++)
//         if (minSize > windows[i].size())
//         {
//             minSize = windows[i].size();
//             optiWin = i;
//         }
//     return optiWin;
// }