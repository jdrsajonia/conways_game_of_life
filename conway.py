from time import sleep
from random import randint


class conwayGame():
    def __init__(self, cols,rows):
        self.rows, self.cols = rows, cols
        self.matrix=[[randint(0,1) for _ in range(cols)] for _ in range(rows)]

        self.ascci_cell="██"
        self.ascci_space="  "
        self.v_line="║"
        self.h_line="═"
        self.corner="+"
        self.horizontal_margin=self.corner+2*self.h_line*cols+self.corner+"\n"
        

    def _adyacent_cells(self,cell):
        matrix=self.matrix
        y, x=cell
        neighbors=[
            (y-1, x),   # up
            (y+1, x),   # down
            (y, x-1),   # left
            (y, x+1),   # right
            (y-1, x+1), # right_up
            (y+1, x+1), # right_down
            (y-1, x-1), # left_up
            (y+1, x-1), # left_down
        ]

        count=0
        for ny, nx in neighbors:
            if 0 <= ny < self.rows and 0 <= nx < self.cols:
                count += matrix[ny][nx]
        return count


    def _is_alive(self, cell):
        y, x=cell
        return True if self.matrix[y][x]==1 else False


    def _is_dead(self,cell):
        return not self._is_alive(cell)
    

    def _deserve_to_live(self, cell):
        neighbor_length=self._adyacent_cells(cell)
        if self._is_alive(cell):
            if neighbor_length==2 or neighbor_length==3:
                return True
            elif neighbor_length<2 or neighbor_length>3:
                return False
            
        elif self._is_dead(cell) and neighbor_length==3:
            return True
        return False
    

    def _next_generation(self):
        next_matrix=[[0]*self.cols for i in range(self.rows)]

        for j in range(self.rows):
            for i in range(self.cols):
                if self._deserve_to_live((j,i)):
                    next_matrix[j][i]=1
                else:
                    next_matrix[j][i]=0
        return next_matrix
    

    def _render_game(self):
        matrix=self.matrix
        str_frame=""

        str_frame+=self.horizontal_margin
        for j in range(self.rows):
            str_frame+=self.v_line
            for i in range(self.cols):
                actual_place=matrix[j][i]
                if actual_place==0:
                    str_frame+=self.ascci_space
                elif actual_place==1:
                    str_frame+=self.ascci_cell
            str_frame+=self.v_line+"\n"

        str_frame+=self.horizontal_margin
        return str_frame
    

    def game_loop(self):
        clear="\033[H\033[J"
        while True:
            print(clear, end="")
            print(self._render_game())

            next_matrix=self._next_generation()
            self.matrix=next_matrix
            sleep(0.1)


game=conwayGame(100,40)
game.game_loop()