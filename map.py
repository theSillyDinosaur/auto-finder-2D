from pydoc import resolve
import numpy as np
import math

class Map:
    def __init__(self, x, y, resol):
        self.x = round(x/resol)+1
        self.y = round(y/resol)+1
        self.resol = resol
        self.obstacle = np.zeros((self.x, self.y))
        self.US_detect = np.zeros((self.x, self.y))
        self.x_PoG = np.linspace(0-resol/2, x, self.x)
        self.y_PoG = np.linspace(0-resol/2, y, self.y)
    
    
    
    def record(self, x, y, z, us_a):
        #passby
        x_grid, y_grid = math.floor((x+self.resol/2)/self.resol), math.floor((y+self.resol/2)/self.resol)
        self.obstacle[x_grid][y_grid] = -1
        self.US_detect[x_grid][y_grid] = -1
        #obstacle
        self.obstacle_detect(x, y, z, us_a, 0, 0, 0)
        
    def obstacle_detect(self, x, y, z, d, d_x, d_y, d_z):
        #obstacle
        x_obst, y_obst = x+d_x*math.cos(z+d_z)+d_y*math.sin(z+d_z)+d*math.cos(z), y-d_x*math.sin(z+d_z)+d_y*math.cos(z+d_z)+d*math.sin(z)
        x_grid, y_grid = math.floor(x_obst/self.resol), math.floor(y_obst/self.resol)
        if(x_grid < self.x and y_grid < self.y and x_grid >= 0 and y_grid >= 0 and self.obstacle[x_grid][y_grid] != -1):
            self.obstacle[x_grid][y_grid] += 1
            self.US_detect[x_grid][y_grid] += 1
        #field
        if(x_obst-x == 0):
            for i in range(min(math.floor(y/self.resol), x_grid), max(math.floor(y/self.resol), y_grid)):
                if(i !=y_grid):
                    self.US_detect[x_grid][i] += 1
        else:
            m = (y_obst-y)/(x_obst-x)
            b = y-(m*x)
            for i in range(min(math.floor(x/self.resol), x_grid), max(math.floor(x/self.resol)+1, x_grid+1)):
                for j in range(min(math.floor(y/self.resol), y_grid), max(math.floor(y/self.resol)+1, y_grid+1)):
                    if(i == x_grid and j == y_grid) or (self.obstacle[i][j] == -1):
                        continue
                    print(i, j, m*self.x_PoG[i]+b, self.y_PoG[j])
                    if(m <= 0 and m*self.x_PoG[i+1]+b < m*self.y_PoG[j+1]):
                        print(i, j)
                    if(m <= 0 and m*self.x_PoG[i]+b > self.y_PoG[j] and m*self.x_PoG[i+1]+b < self.y_PoG[j+1]):
                        self.US_detect[i][j] += 1
                    elif(m >= 0 and m*self.x_PoG[i+1]+b > self.y_PoG[j] and m*self.x_PoG[i]+b < self.y_PoG[j+1]):
                        self.US_detect[i][j] += 1
    
    def obstacle_deter(self):
        state = np.zeros((self.x, self.y))
        non = []
        for i in range(self.x):
            for j in range(self.y):
                if(self.obstacle[i][j] == -1):
                    state[i][j] = -1
                elif(self.obstacle[i][j]/self.US_detect[i][j] > 0.3):
                    state[i][j] = 1
                else:
                    non.append([i, j])
    
if __name__ == "__main__":
    test = Map(1, 1, 0.1)
    print(test.x_PoG, test.y_PoG)
    for i in range(100):
        test.record(0.5, 0.5, 2*3.14159*i/100, 0.5)
    print(test.obstacle)
    print(test.US_detect)