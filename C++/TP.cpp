#include <iostream>
#include <vector>
#include <cmath>
#include <SDL2/SDL.h>
#define SIMULATION_TIME 10

using namespace std;

class Clock {
    public:
        long double time;
        long double dt;
    Clock () {
        time = 0;
        dt = 0.000125;
    }
    void next(){
        time += dt;
    }
    long double get_previous_time(){
        return time - dt;
    }
    long double get_time(){
        return time;
    }
    void set_time (long double some){
        time = some;
    }
};

long double remap (long double value, long double low1, long double high1, long double low2, long double high2) {
        long double res = low2 + (value - low1) * (high2 - low2) / (high1 - low1);
        return res;
    }

class Heat: public Clock {
    public:
        int Qn;
        long double Re;
        long double ke;
        long double Rf;
        long double dx;
        long double L;
        long double T_entre;
        long double S_ini;
        vector <long double> T;
        vector <long double> T_p;
        vector <long double> Speed;
        Heat(void){
            Qn = 21;
            Re = 1.0605 * 1300.1934;
            Rf = (1.121 * 0.3868);
            ke = 1.031;
            L = 1.6;
            dx = L/(Qn-1);
            T_entre = 900; 
            T.assign(Qn,500);
            T_p.assign(Qn,500);
            T_p[0] = T_entre;
            S_ini = 1;
            Speed.assign(Qn,S_ini);
        }

        void compute_single(void){
            for (int i=1; i < Qn-1; i++) {
                int im1 = i-1;
                int ip1 = i+1;
                Rf = Speed[im1]*Rf;
                long double res = (dt/Re)*((ke/pow(dx,2))*(T_p[ip1] -2*T_p[i] + T_p[im1]) - (Rf/pow(dx,2)*(T_p[i]-T_p[im1]))) +T_p[i];
                T.at(i) = res;
            }
            // Bondary conditions
            T.at(Qn-1) = T[Qn-2]; 
        }
        

        void replace_single(void){
            T_p = T;
        }
        void show(void){
            for (int i = 0; i != Qn; i++){
                cout << T.at(i) << " ";
                if (i%4 == 3){cout << " ";}
            }
            cout << "\n";
        }
        vector <SDL_Point> to_array_of_point(int width, int height){
            // SDL_Point* points = (SDL_Point*)malloc(sizeof(SDL_Point) * n);
            vector <SDL_Point> points;
            int xpadding = round(width/12);
            int ypadding = round(height/12);
            long double ymax =  pow(10,3);
            for (int i = 0; i < Qn; i++) {
                SDL_Point point;
                long double x =  i*dx;
                //remap
                int final_x = round(remap(x, 0, L, xpadding, width-xpadding));
                int final_y = round(remap(T[i], -ymax, ymax, ypadding, height - ypadding));
                //append to array
                point.y = final_y;
                point.x = final_x;
                points.push_back(point);
            }
            return points;
        }
        void test(){
        show();
        replace_single();
        next();
            while(time < SIMULATION_TIME){
                compute_single();
                show();
                replace_single();
                next();
            }
        }

        void plotter(SDL_Renderer* renderer){
        replace_single();
        next();
            while(time < SIMULATION_TIME){
                compute_single();
                show();
                replace_single();
                next();
            }
        }
};


void my_drawer(SDL_Renderer* renderer, vector <SDL_Point> corde){
    SDL_SetRenderDrawColor(renderer,255,255,255,SDL_ALPHA_OPAQUE);
    for (int i = 1; i < corde.size(); i++){
        SDL_RenderDrawLine(renderer,corde[i-1].x, corde[i-1].y, corde[i].x, corde[i].y);
    }
}

int main (void){
    Heat my_guitar;
    my_guitar.replace_single();
    my_guitar.next();
    
    bool done = false;

    SDL_Window* window = SDL_CreateWindow("Corde différences finies",500,200,600,400,SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = nullptr;
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    int j = 0;
    while(!done){

        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT){
                done = true;
            }
        }
        /* if (j%20 == 0) { */
        /*     my_guitar.pluck(5,0.1); */
        /*     my_guitar.replace_single(); */
        /*     my_guitar.next(); */
        /* } */

        vector <SDL_Point> vecpoint = my_guitar.to_array_of_point(600,400);

        SDL_Delay(5);
        SDL_SetRenderDrawColor(renderer,0,0,0,SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        my_drawer(renderer,vecpoint);
        SDL_RenderPresent(renderer);


        my_guitar.compute_single();
        my_guitar.replace_single();
        my_guitar.next();

    }
    SDL_DestroyWindow(window);

    return 0;
}
