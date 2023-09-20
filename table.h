    //
    // Created by marck on 18/09/2023.
    //

    #ifndef TAREA_DAMAS_TABLE_H
    #define TAREA_DAMAS_TABLE_H

    #include <iostream>
    #include <limits.h>
    #include <vector>
    #include <map>
    using namespace std;

    int max_deep = 6;

    int queenX[4] = {+1,+1,-1,-1};
    int queenY[4] = {+1,-1,+1,-1};
    char null_char = '-';

    class Node;
    class Table;

    // MAX RED
    // MIN BLACK

    class Tree{

    public:
        Node* root = nullptr;

        Tree();

        void init_tree(Table* tablero);
        int FindBestMove();
        void nextLevel(int mov_opt);
        void reset();

    };


    class Table{

    public:

        Tree MinMaxTree;

        char turn = 'r';
        char **mat = nullptr;
        map< pair <int,int>, char> queens;


        Table();

        ~Table();

        void operator = (Table &table2);


        int MinMaxScore();

        void print_table_console();

        void resetGame();

        char checkWin();


        // 0 = Player vs Player
        // 1 = Player vs Machine
        void initGame(bool game_type);

        void move(string moves);

        bool insideTable(int row, int col);

        // Recursive
        vector<string> checkEatJumps(int start_row, int start_col, int step, char color);

        vector<string> checkEatJumpsQueen(int start_row, int start_col, char color);

        vector<string> calc_moves(char color);

        int userPlay(char user_color);

        int AIPlay(char pc_color);

    };


    class Node{
    public:

        Table table;
        int score = 0;
        vector< Node* > children;
        vector< string > movs;

        Node(Table &table, string instruction, char turn, int deep);

        void cut_childrens();
        int FindBestMove();
        int update(int deep);
        Node* nextLevel(int mov_opt);
        void print();

    };

    Table::Table(){

        resetGame();
        // print_table_console(); // Print Table

    }

    Table::~Table(){

        // cout << "Table destructor called" << endl;

        if (mat){ // Delete mat if exist
            for (int i = 0; i < 8; ++i) delete mat[i];
            delete mat;
        }
    }

    void Table::operator = (Table &table2){

        if (!mat){
            mat = new char*[8];
            for (int i = 0; i < 8; ++i) mat[i] = new char[8];
        }

        for (int r = 0; r < 8; ++r){
            for (int c = 0; c < 8; ++c) mat[r][c] = table2.mat[r][c];
        }

        queens = table2.queens;

    }


    int Table::MinMaxScore(){

        int red = 0, black = 0;

        for (int r = 0; r < 8; ++r){
            for (int c = 0; c < 8; ++c){

                if (queens.find({r,c}) != queens.end()) {
                    if (mat[r][c] == 'r') red += 10;
                    else black += 10;
                }
                else{
                    if (mat[r][c] == 'r') ++red;
                    else if (mat[r][c] == 'b') ++black;
                }
            }
        }

        return red - black;
    }


    void Table::print_table_console(){

        cout << "\n   ";
        for (int i = 0; i < 8; ++i) cout << i << " ";
        cout << endl;

        for (int r = 0; r < 8; ++r){
            cout << r << "  ";
            for (int c = 0; c < 8; ++c){
                if (queens.find({r,c}) != queens.end()) {
                    if (mat[r][c] == 'r') cout << "R ";
                    if (mat[r][c] == 'b') cout << "B ";
                }
                else cout << mat[r][c] << " ";
            }
            cout <<endl;
        }
    }

    void Table::resetGame(){

        if (mat){ // Delete mat if exist
            for (int i = 0; i < 8; ++i) delete mat[i];
            delete mat;
        }

        // Init mat again
        mat = new char*[8];
        for (int i = 0; i < 8; ++i) mat[i] = new char[8];

        // fill mat with initial positions
        for (int r = 0; r < 8; ++r){
            for (int c = 0; c < 8; ++c){
                if ((r + c) % 2) mat[r][c] = null_char;
                else if (r < 3) mat[r][c] = 'b';
                else if (r > 4) mat[r][c] = 'r';
                else mat[r][c] = null_char;
            }
        }


        turn = 'r';
        queens.clear();

        //Reset Tree
        // MinMaxTree.reset();

        return;
    }

    char Table::checkWin(){

        int b_pieces = 0, r_pieces = 0;

        for (int r = 0; r < 8; ++r){
            for (int c = 0; c < 8; ++c){
                if (mat[r][c] == 'r' || mat[r][c] == 'R') ++ r_pieces;
                if (mat[r][c] == 'b' || mat[r][c] == 'B') ++ b_pieces;
            }
        }

        if (r_pieces == 0) return 'b';
        if (b_pieces == 0) return 'r';
        return null_char;

    }


    // 0 = Player vs Player
    // 1 = Player vs Machine
    void Table::initGame(bool game_type){

        int opt;
        char winner = null_char;
        char user_color, enemy_color = 'r';

        // Selecting color
        while(1){
            cout << "\nSelect a color (b/r): ";
            cin >> user_color;

            if (user_color == 'B') user_color = 'b';
            if (user_color == 'R') user_color = 'r';
            if (user_color == 'b' || user_color == 'r') break;
        }

        if (enemy_color == user_color) enemy_color = 'b';

        if (game_type) MinMaxTree.init_tree(this); // init Tree


        // Start game
        while (1){

            print_table_console();

            cout << "\nIts " << turn << " turn!" <<endl;

            if (turn == user_color){
                opt = userPlay(turn);

                if (opt == -1){ // Machine wins
                    cout << endl << endl << "\t\t THE PLAYER " << enemy_color << " WIN THE GAME!!" << endl << endl;
                    break;
                }

                if (game_type) MinMaxTree.nextLevel(opt);
            }
            else if (game_type){
                opt = AIPlay(turn);

                if (opt == -1){ // User wins
                    cout << endl << endl << "\t\t THE PLAYER " << user_color << " WIN THE GAME!!" << endl << endl;
                    break;
                }
            }


            winner = checkWin();

            if (winner != null_char) {
                cout << endl << endl << "\t\t THE PLAYER " << winner << " WIN THE GAME!!" << endl << endl;
                break;
            }

            if (turn == 'r') turn = 'b';
            else turn = 'r';

        }
    }

    void Table::move(string moves){

        int r1, c1, r2, c2, stepX, stepY;

        for (int i = 0; i < moves.size()-2; i+=2){
            r1 = moves[i] - 48;
            c1 = moves[i+1] - 48;
            r2 = moves[i+2] - 48;
            c2 = moves[i+3] - 48;

            mat[r2][c2] = mat[r1][c1];
            mat[r1][c1] = null_char;

            // Piezas intermedias, eliminarlas (0)

            stepX = 1;
            stepY = 1;

            if (r2 < r1) stepY = -1;
            if (c2 < c1) stepX = -1;

            r1 += stepY;
            c1 += stepX;

            while(r1 != r2 && c1 != c2){
                mat[r1][c1] = null_char;
                if (queens.find({r1,c1}) != queens.end()) queens.erase({r1,c1});
                r1 += stepY;
                c1 += stepX;
            }

        }

        r1 = moves[0] - 48;
        c1 = moves[1] - 48;
        r2 = moves[moves.size()-2] - 48;
        c2 = moves[moves.size()-1] - 48;

        // Se movio una queen
        if (queens.find({r1, c1}) != queens.end()){
            queens.erase({r1, c1});
            queens.insert({{r2,c2}, mat[r2][c2]});
        }
        else if ( r2 == 7 || r2 == 0 ){ // Se convierte en queen
            // cout << "NEW QUEEN" << endl;
            queens.insert({{r2,c2}, mat[r2][c2]});
        }

        return;

    }

    bool Table::insideTable(int row, int col){
        if (row >= 0 && row < 8 && col >=0 && col < 8) return true;
        return false;
    }

    // Recursive
    vector<string> Table::checkEatJumps(int start_row, int start_col, int step, char color){

        vector<string> moves;
        vector <string> aux;

        string star_pos = to_string(start_row) + to_string(start_col);

        int row = start_row + step*2;
        int col = start_col + 2;

        if (insideTable(row,col) && mat[row][col] == null_char){ // Se puede saltar
            if (mat[start_row+step][start_col+1] != color && mat[start_row+step][start_col+1] != null_char){ // Hay un enemigo al medio
                // cout << "Enemy found: " << start_row+step << " " <<  start_col+1 << endl;

                aux = checkEatJumps(row,col, step, color);
                if (aux.size() == 0) moves.push_back(to_string(row) + to_string(col));
                else{
                    for (int i = 0; i < aux.size(); ++i) moves.push_back(to_string(row) + to_string(col) + aux[i]);
                }
                aux.clear();

            }
        }

        row = start_row + step*2;
        col = start_col - 2;

        if (insideTable(row,col) && mat[row][col] == null_char){ // Se puede saltar
            if (mat[start_row+step][start_col-1] != color && mat[start_row+step][start_col-1] != null_char){ // Hay un enemigo al medio
                // cout << "Enemy found: " << start_row+step << " " <<  start_col-1 << endl;

                aux = checkEatJumps(row,col, step, color);
                if (aux.size() == 0) moves.push_back(to_string(row) + to_string(col));
                else{
                    for (int i = 0; i < aux.size(); ++i) moves.push_back(to_string(row) + to_string(col) + aux[i]);
                }
                aux.clear();
            }
        }

        return moves;

    }

    vector<string> Table::checkEatJumpsQueen(int start_row, int start_col, char color){

        vector<string> moves;
        vector <string> aux;
        vector <pair <int, int> > eated;

        string init_moves = to_string(start_row)+to_string(start_col);

        int row;
        int col;
        bool found;

        char enemy_color = 'r';
        if (enemy_color == color) enemy_color = 'b';

        for (int i = 0; i < 4; ++i){

            found = 0;

            for (int j = 1; j < 8; ++j) { // Starts in 1, cause the jump is at least one pos

                row = start_row + queenY[i]*j;
                col = start_col + queenX[i]*j;

                if (!insideTable(row, col)) break; // Not inside board
                if (mat[row][col] == color) break; // Team piece
                if (mat[row][col] == enemy_color) { // enemy piece found (Possible multi jumps)
                    found = 1;
                    eated.push_back({row,col});
                    mat[row][col] = null_char; // Delete them, until we finish diag possibilities
                    continue;
                }

                if (mat[row][col] == null_char){ // Find free square
                    if (!found) continue;  // Not enemy piece found yet, continue
                    else { // Possible place to jump

                        // cout << "Possible: " << row << ", " << col << endl;

                        aux = checkEatJumpsQueen(row,col,color);

                        if (aux.size() == 0) moves.push_back(to_string(row) + to_string(col));
                        else{
                            for (int k = 0; k < aux.size(); ++k) moves.push_back(to_string(row) + to_string(col) + aux[k]);
                        }
                    }
                }


            }

            for (int i = 0; i < eated.size(); ++i) mat[eated[i].first][eated[i].second] = enemy_color;
            eated.clear();


        }

        return moves;

    }

    vector<string> Table::calc_moves(char color){

        vector<string> normal_plays; // No eat
        vector<string> forced_plays; // forced
        vector<string> aux;

        string star_pos;

        bool forced_move = 0;
        int nc, nr, step = 1;
        if (color == 'r') step = -1;


        for (int r = 0; r < 8; ++r){
            for (int c = 0; c < 8; ++c){

                if (mat[r][c] != color) continue; // Not my piece

                star_pos = to_string(r) + to_string(c);

                if (queens.find({r,c}) != queens.end()){ // Queen


                    for (int i = 0; i < 4; ++i){
                        for (int j = 1; j < 8; ++j){ // Starts in 1, cause the jump is at least one pos

                            nr = r + queenY[i]*j;
                            nc = c + queenX[i]*j;

                            if (!insideTable(nr, nc) || mat[nr][nc] != null_char) break;

                            normal_plays.push_back(star_pos + to_string(nr) + to_string(nc));
                        }
                    }


                    aux = checkEatJumpsQueen(r,c, color);
                    for (int i = 0; i < aux.size(); ++i) forced_plays.push_back(star_pos + aux[i]);
                    aux.clear();

                    continue;

                }

                nr = r + step;
                nc = c + 1;

                if (insideTable(nr, nc)){ // right diag
                    if (mat[nr][nc] == null_char && !forced_move){ // Free space
                        normal_plays.push_back(star_pos + to_string(nr) + to_string(nc));
                    }
                }

                nc = c - 1;

                if (insideTable(nr, nc)){ // left diag
                    if (mat[nr][nc] == null_char && !forced_move){ // Free space
                        normal_plays.push_back(star_pos + to_string(nr) + to_string(nc));
                    }
                }

                // Check all eat jumps can we do
                aux = checkEatJumps(r,c, step, color);
                for (int i = 0; i < aux.size(); ++i) forced_plays.push_back(star_pos + aux[i]);
                aux.clear();

            }
        }

        // cout << forced_plays.size() << endl;

        if (forced_plays.size() > 0) return forced_plays; // Only include forced_plays on answer
        return normal_plays; // Include just normal plays

    }


    int Table::userPlay(char user_color){

        int opt;

        // Calculate all posible Movs
        vector<string> posible_moves = calc_moves(user_color);

        if (posible_moves.size() == 0) return -1;

        cout << endl;

        for (int i = 0; i < posible_moves.size(); ++i){
            cout << i << ": ";

            cout << "(" << posible_moves[i][0] << ", " << posible_moves[i][1] << ")";


            for (int j = 2; j < posible_moves[i].size(); j+=2){
                cout << " -> (" << posible_moves[i][j] << ", " << posible_moves[i][j+1] << ")";
            }
            cout << endl;

        }

        while(1){
            cout << "\nElije una opcion: "; cin >> opt;
            if (opt > -1 && opt < posible_moves.size()) break;
        }

        move(posible_moves[opt]);

        for (auto it = queens.begin(); it != queens.end(); ++it){
            cout << it->first.first << ", " << it->first.second << " -> " << it->second << endl;
        }

        return opt;

    }

    int Table::AIPlay(char pc_color){

        vector<string> movs = calc_moves(pc_color);

        if (movs.size() == 0) return -1;

        int opt = MinMaxTree.FindBestMove();

        cout << "AI Plays: ";

        cout << "(" << movs[opt][0] << ", " << movs[opt][1] << ")";
        for (int j = 2; j < movs[opt].size(); j+=2) cout << " -> (" << movs[opt][j] << ", " << movs[opt][j+1] << ")";
        cout << endl;

        move(movs[opt]);

        MinMaxTree.nextLevel(opt);

        return 1;

    }



    // Node Definition

    Node::Node(Table &table, string instruction, char turn, int deep) {
        this->table = table;
        this->table.turn = turn;

        if (instruction != "") this->table.move(instruction);

        movs = this->table.calc_moves(turn);

        if (deep == max_deep) return;


        if (turn == 'r') turn = 'b';
        else turn = 'r';

        for (int i = 0; i < movs.size(); ++i)  children.push_back(new Node(this->table, movs[i], turn, deep +1));
    }

    void Node::cut_childrens(){
        while(children.size()) {
            children[children.size()-1]->cut_childrens();
            delete children[children.size()-1];
            children.pop_back();
        }
    }

    int Node::FindBestMove(){

        // cout << "Finding best move for: " << table.turn << endl;

        int best_value, best_index_move, aux;

        if (table.turn == 'r') best_value = INT_MIN;
        else best_value = INT_MAX;

        for (int i = 0; i < children.size(); ++i){

            aux = children[i]->update(1);

            if (table.turn == 'r'){

                if (aux > best_value){
                    best_value = aux;
                    best_index_move = i;
                }

            }
            else if (table.turn == 'b'){

                if (aux < best_value){
                    best_value = aux;
                    best_index_move = i;
                }
            }

        }


        cout << "\nBEST VALUE FOUND: " << best_value << endl;
        cout << "\nBEST MOVE FOUND: " << movs[best_index_move] << endl << endl;

        return best_index_move;

    }

    int Node::update(int deep){

        if (deep == max_deep) { // Calc score and go up

            // cout << "LEAF" << endl;
            score = table.MinMaxScore();
            return score;
        }

        // Create more childrens
        if (children.size() == 0){

            // cout << "Create Childrens" << endl;

            movs = table.calc_moves(table.turn);

            if (movs.size() == 0){ // No more movs for your team
                if (table.turn == 'r') return -100; // No more mvs for red
                else return +100; // No more mvs for black
            }

            char next_turn = 'r';
            if (table.turn == 'r') next_turn = 'b';

            for (int i = 0; i < movs.size(); ++i) children.push_back(new Node(table, movs[i], next_turn, deep +1));

        }

        // get max or min of all children
        int best_value;

        if (table.turn == 'r') best_value = INT_MIN;
        else best_value = INT_MAX;

        // cout << "Sending update levl: " << deep  << endl;


        for (int i = 0; i < children.size(); ++i){

            if (table.turn == 'r') best_value = max(best_value, children[i]->update(deep +1));
            else if (table.turn == 'b') best_value = min(best_value, children[i]->update(deep +1));

        }

        return best_value; // Return the score

    }

    Node* Node::nextLevel(int mov_opt){

        // cout << "Before Next level root:" <<endl;
        // print();


        // cout << "Numb movs: " << movs.size() << endl;

        // for (int i = 0; i < movs.size(); ++i) cout << movs[i] << endl;

        // cout << "Se toma decision: " << mov_opt<< " " << movs[mov_opt] << endl;

        Node* new_root = children[mov_opt];

        // cout << "Nueva root antes de caos: " << endl;
        // new_root->print();

        for (int i = 0; i < children.size(); ++i ){
            if (children[i] == new_root) continue;
            else{
                children[i]->cut_childrens();
                delete children[i];
            }
        }

        while (children.size()) children.pop_back();

        return new_root;

    }

    void Node::print() {

        int cont_r = 0, cont_b = 0;

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (table.mat[i][j] == 'r') cont_r++;
                if (table.mat[i][j] == 'b') cont_b++;
            }
        }

        table.print_table_console();
        cout << endl;

        cout << "Red pieces: " << cont_r << endl;
        cout << "Black pieces: " << cont_b << endl;
    }



    // Tree Definition

    Tree::Tree() {}

    void Tree::init_tree(Table* tablero) {
        root = new Node(*tablero, "", 'r', 0);	//Init root con el tablero y las fichas actuales
    }

    int Tree::FindBestMove(){
        return root->FindBestMove();
    }

    void Tree::nextLevel(int mov_opt){
        Node* old_root = root;

        root = old_root->nextLevel(mov_opt);

        delete old_root;

        // cout << "NEW ROOT: " <<endl;
        // root->print();

    }

    void Tree::reset(){
        root->cut_childrens();
        delete root;
        root = nullptr;
    }

    #endif //TAREA_DAMAS_TABLE_H
