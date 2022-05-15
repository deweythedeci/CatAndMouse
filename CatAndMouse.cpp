#include <iostream>
#include <vector>
using namespace std;

struct Board;
struct GameState;
struct Game;

bool contains(vector<GameState> v, GameState a);
bool contains(vector<int> v, int a);
int best_cat_move(Game g, Board b);
int cat_move_outcome(Game g, Board b, int m);
int best_mouse_move(Game g, Board b);
int mouse_move_outcome(Game g, Board b, int m);

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

    Game(int c, int m) : state(c, m) {}
    
    // return 1 if the move results in a draw
    int cat_move(int n){
        state.cat = n;
        if(contains(hist_m, state)){
            return 1;
        }
        hist_m.push_back(state);
        return 0;
    }

    int mouse_move(int n){
        state.mouse = n;
        if(contains(hist_c, state)){
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
        // if the move wins pick it
        if(g.state.mouse == m){
            return m;
        }
        else{
            // check the move's outcome
            int out = cat_move_outcome(g, b, m);
            // if the move wins pick it
            if(out == 1){
                return m;
            }
            // if the move draws save it
            else if(out == 2){
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

// returns the outcome of the cat's move
// 0 = loss, 1 = win, 2 = draw
int cat_move_outcome(Game g, Board b, int m){
    // make the move
    if(g.cat_move(m)){
        return 2;
    }
    // check if the mouse is about to win
    if(contains(b.edges.at(g.state.mouse), b.hole)){
        return 0;
    }
    // make the mouse's best move
    if(g.mouse_move(best_mouse_move(g, b))){
        return 2;
    }
    // check if the cat is about to win
    if(contains(b.edges.at(g.state.cat), g.state.mouse)){
        return 1;
    }
    // make the cat's best move and check that outcome
    return cat_move_outcome(g, b, best_cat_move(g, b));
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
            int out = mouse_move_outcome(g, b, m);
            // if the move wins pick it
            if(out == 1){
                return m;
            }
            // if the move draws save it
            else if(out == 2){
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

// returns the outcome of the mouse's move
// 0 = loss, 1 = win, 2 = draw
int mouse_move_outcome(Game g, Board b, int m){
    // make the move
    if(g.mouse_move(m)){
        return 2;
    }
    // check if the cat is about to win
    if(contains(b.edges.at(g.state.cat), g.state.mouse)){
        return 0;
    }
    // make the cat's best move
    if(g.cat_move(best_cat_move(g, b))){
        return 2;
    }
    // check if the mouse is about to win
    if(contains(b.edges.at(g.state.mouse), b.hole)){
        return 1;
    }
    // make the mouse's best move and check that outcome
    return mouse_move_outcome(g, b, best_mouse_move(g, b));
}

int main(){
    vector<vector<int>> vect;
    vect.at(0).push_back(2);
    vect.at(0).push_back(5);
    vect.at(1).push_back(3);
    vect.at(2).push_back(0);
    vect.at(2).push_back(4);
    vect.at(2).push_back(5);
    vect.at(3).push_back(1);
    vect.at(3).push_back(4);
    vect.at(3).push_back(5);
    vect.at(4).push_back(2);
    vect.at(4).push_back(3);
    vect.at(5).push_back(0);
    vect.at(5).push_back(2);
    vect.at(5).push_back(3);
    Board b = Board(vect, 0);
    Game g = Game(2, 1);
    cout << mouse_move_outcome(g, b, best_mouse_move(g, b)) << endl;
    return 0;
}