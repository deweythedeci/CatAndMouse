#include <iostream>
#include <vector>
using namespace std;

struct Board;
struct GameState;
struct Game;

bool contains(vector<GameState> v, GameState a);
bool contains(vector<int> v, int a);
int best_cat_move(Game g, Board b);
int move_outcome(Game g, Board b, int m);
int best_mouse_move(Game g, Board b);

struct Board {
    vector<vector<int>> edges; // graph
    int hole; // position of hole
    Board(vector<vector<int>> e, int h){
        edges = e;
        hole = h;
    }
};

struct GameState {
    int cat, mouse; // the position of the cat and mouse
    GameState(int c, int m){
        cat = c;
        mouse = m;
    }
    bool equals(GameState a){
        if(a.cat != cat){
            return false;
        }
        if(a.mouse != mouse){
            return false;
        }
        return true;
    }
};

struct Game {
    GameState state; // current game state
    vector<GameState> hist_c; // history of states where it's the cat's turn
    vector<GameState> hist_m; // history of states where it's the mouse's turn
    int hole;
    int turn = 1; // 0 = cat's turn, 1 = mouse's turn

    Game(int c, int m, int h) : state(c, m) {
        hole = h;
    }
    
    // returns the resulting game state
    // 0 = no change, 1 = cat win, 2 = mouse win, 3 = draw
    int cat_move(int n){
        state.cat = n;
        turn = 1;
        if(state.cat == state.mouse){
            return 1;
        }
        if(contains(hist_m, state)){
            return 3;
        }
        hist_m.push_back(state);
        return 0;
    }

    // returns the resulting game state
    // 0 = no change, 1 = cat win, 2 = mouse win, 3 = draw
    int mouse_move(int n){
        state.mouse = n;
        turn = 0;
        if(state.mouse == hole){
            return 2;
        }
        if(contains(hist_c, state)){
            return 3;
        }
        if(state.cat == state.mouse){
            return 1;
        }
        hist_c.push_back(state);
        return 0;
    }
};

bool contains(vector<GameState> v, GameState a){
    for(GameState e : v){
        if(e.equals(a)){
            return true;
        }
    }
    return false;
}

bool contains(vector<int> v, int a){
    for(int e : v){
        if(e == a){
            return true;
        }
    }
    return false;
}

// returns the optimal move for the cat
int best_cat_move(Game g, Board b){
    int draw = -1; // move that results in draw
    // go through all possible moves
    for(int m : b.edges.at(g.state.cat)){
        // if it's the hole the cat can't move there
        if(g.hole == m){
            continue;
        }
        // if the move wins pick it
        if(g.state.mouse == m){
            return m;
        }
        else{
            // check the move's outcome
            int out = move_outcome(g, b, m);
            // if the move wins pick it
            if(out == 1){
                return m;
            }
            // if the move draws save it
            else if(out == 3){
                draw = m;
            }
        }
    }
    // if no moves win pick a move that draws
    if(draw != -1){
        return draw;
    }
    // if you can't draw pick a losing move
    else{
        return b.edges.at(g.state.cat).at(0);
    }
}

// returns the outcome a move
// 1 = cat win, 2 = mouse win, 3 = draw
int move_outcome(Game g, Board b, int m){
    if(g.turn){
        int out = g.mouse_move(m);
        if(out){ return out; }
        return move_outcome(g, b, best_cat_move(g, b));
    }
    int out = g.cat_move(m);
    if(out){ return out; }
    return move_outcome(g, b, best_mouse_move(g, b));
}

// returns the optimal move for the mouse
int best_mouse_move(Game g, Board b){
    int draw = -1; // move that results in draw
    // go through all possible moves
    for(int m : b.edges.at(g.state.mouse)){
        // if the move wins pick it
        if(b.hole == m){
            return m;
        }
        else{
            // check the moves outcome
            int out = move_outcome(g, b, m);
            // if the move wins pick it
            if(out == 2){
                return m;
            }
            // if the move draws save it
            else if(out == 3){
                draw = m;
            }
        }
    }
    // if no moves win pick a move that draws
    if(draw != -1){
        return draw;
    }
    // if you can't draw pick a losing move
    else{
        return b.edges.at(g.state.mouse).at(0);
    }
}

int main(){
    vector<vector<int>> vect {{2,5},{3},{0,4,5},{1,4,5},{2,3},{0,2,3}};
    Board b = Board(vect, 0);
    Game g = Game(2, 1, 0);
    int gs = 0;
    while(!gs){
        cout << "mouse's turn" << endl;
        cout << "cat: " << g.state.cat << endl;
        cout << "mouse: " << g.state.mouse << endl << endl;
        gs = g.mouse_move(best_mouse_move(g, b));
        if(gs){ break; }
        cout << "cat's turn" << endl;
        cout << "cat: " << g.state.cat << endl;
        cout << "mouse: " << g.state.mouse << endl << endl;
        gs = g.cat_move(best_cat_move(g, b));
    }
    cout << "cat: " << g.state.cat << endl;
    cout << "mouse: " << g.state.mouse << endl;
    cout << "game state: " << gs << endl;
    cout << "1 = cat win, 2 = mouse win, 3 = draw" << endl;
    return 0;
}