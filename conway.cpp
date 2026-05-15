#include <chrono>
#include <vector>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <functional>

// using namespace std;


#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>

    #define GETCH() _getch()
    #define SLEEP(ms) Sleep(ms)
    #define SET_UTF8() SetConsoleOutputCP(CP_UTF8)

#else
    #include <termios.h>
    #include <unistd.h>

    char getch(){

        char buf = 0;

        termios old = {};
        tcgetattr(STDIN_FILENO, &old);

        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;

        tcsetattr(STDIN_FILENO, TCSANOW, &old);

        read(STDIN_FILENO, &buf, 1);

        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;

        tcsetattr(STDIN_FILENO, TCSADRAIN, &old);

        return buf;
    }

    #define GETCH() getch()
    #define SLEEP(ms) usleep(ms*1000)
    #define SET_UTF8()
#endif



std::mt19937 rng(time(nullptr));
std::uniform_int_distribution<int> dist(0, 1);



struct Cell{
    int y; 
    int x;
};



class conwayGame{
    public:
        int speed=100;
        bool paused=false;
        bool wrapped=false;

        conwayGame(int cols, int rows){
            this->rows=rows;
            this->cols=cols;
            this->matrix=std::vector<std::vector<uint8_t>>(rows, std::vector<uint8_t>(cols, 0));
            for (int j=0; j<rows; j++) {
                for (int i=0; i<cols; i++) {
                    matrix[j][i]=dist(rng);
                }
            }

            this->horizontal_margin=corner;
            for (int i=0; i<2*cols; i++) horizontal_margin+=h_line;
            horizontal_margin+=corner+"\n";
        }



        void game_loop(){
            while (true){
                std::cout<<clear;
                std::cout<<render_game();
                if (!paused){
                    std::vector<std::vector<uint8_t>> next_matrix=next_generation();
                    this->matrix=next_matrix;
                }
                SLEEP(speed);
            }
        }



        void restart(){
            for (int j=0; j<rows; j++) {
                for (int i=0; i<cols; i++) {
                    matrix[j][i]=dist(rng);
                }
            }
            count_generations=0;
        }



    private:
        int rows;
        int cols;
        int count_generations=0;
        
        std::string ascci_cell="██";
        std::string ascci_space="  ";
        std::string v_line="║";
        std::string h_line="═";
        std::string corner="+";
        std::string horizontal_margin="";
        std::string clear="\033[H\033[J";

        std::vector<std::vector<uint8_t>> matrix;
        


        int adyacent_cells(Cell cell){
            int y=cell.y;
            int x=cell.x;
            uint8_t count=0;
            std::vector<Cell> neighbors={
                {y-1, x},       //up
                {y+1, x},       //down
                {y, x-1},       //left
                {y, x+1},       //right
                {y-1, x+1},     //right_up
                {y+1, x+1},     //right_down
                {y-1, x-1},     //left_up
                {y+1, x-1}      //left_down
            };

            for (Cell current_cell: neighbors){
                int current_x=current_cell.x;
                int current_y=current_cell.y;

                if (wrapped){
                    current_x=(current_x+cols)%cols;
                    current_y=(current_y+rows)%rows;
                    count = count + matrix[current_y][current_x];
                }
                else{
                    if (0<=current_y && current_y<rows && 0<=current_x && current_x<cols){
                        count = count + matrix[current_y][current_x];
                    }
                }
            }
            return count;
        }



        bool is_alive(Cell cell){
            int y=cell.y;
            int x=cell.x;
            if (matrix[y][x]==1){
                return true;
            }
            return false;
        }



        bool is_dead(Cell cell){
            return !is_alive(cell);
        }



        bool deserve_to_live(Cell cell){
            int alive_neighbors=adyacent_cells(cell);

            if (is_alive(cell)){
                if (alive_neighbors==2 || alive_neighbors==3){
                    return true;
                }
                else if (alive_neighbors<2 || alive_neighbors>3){
                    return false;
                }
            }

            else if (is_dead(cell) && alive_neighbors==3){
                return true;
            }
            return false;
        }



        std::vector<std::vector<uint8_t>> next_generation(){
            std::vector<std::vector<uint8_t>> next_matrix(rows, std::vector<uint8_t>(cols, 0));

            for (int j=0; j<rows; j++){
                for (int i=0; i<cols; i++){
                    Cell cell;
                    cell.x=i;
                    cell.y=j;
                    if (deserve_to_live(cell)){
                        next_matrix[j][i]=1;
                    }
                    else{
                        next_matrix[j][i]=0;
                    }
                }
            }
            count_generations++;
            return next_matrix;
        }



        std::string render_game(){
            std::string frame="";
            frame+=horizontal_margin;

            for (int j=0; j<rows; j++){
                frame+=v_line;
                for (int i=0; i<cols; i++){
                    uint8_t current_state=matrix[j][i];
                    if(current_state==0){
                        frame+=ascci_space;
                    }
                    else if (current_state==1){
                        frame+=ascci_cell;
                    }
                }
                frame+=v_line+"\n";
            }
            frame+=horizontal_margin;
            frame += "[Gen: " + std::to_string(count_generations) +
            "]  [Speed +/-: " + std::to_string(1000/speed) +
            "%]  [Pause p: " + std::string(paused ? "ON" : "OFF") +
            "]  [Wrap w: " + std::string(wrapped ? "ON" : "OFF") +
            "]  [Restart r]  [Quit q]\n";            
            return frame;
            }
};



void move_speed(conwayGame& game){
    while (true){
        char letter = GETCH();
        if(letter == '+'){
            game.speed *= 0.8;
            if(game.speed < 10){
                game.speed = 10;
            }
        }
        else if(letter == '-'){
            game.speed *= 1.25;
            if(game.speed > 1000){
                game.speed = 1000;
            }
        }
        else if(letter == 'q'){
            exit(0);
        }
        else if(letter=='p'){
            game.paused=!game.paused;
        }
        else if(letter=='w'){
            game.wrapped=!game.wrapped;
        }
        else if(letter=='r'){
            game.restart();
        }
    }
}



int main(int argc, char* argv[]){

    SET_UTF8();

    if(argc!=3){
        std::cout<<"try: ./conway <cols> <rows>";
        return 1;
    }

    int cols=std::atoi(argv[1]);
    int rows=std::atoi(argv[2]);

    conwayGame conway(cols,rows);

    std::thread speed_thread(move_speed, std::ref(conway));
    speed_thread.detach();
    conway.game_loop();

    return 0;
}