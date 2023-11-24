#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#define SIMULATION_TIME 720

using namespace std;

extern "C++" class Clock {
    public:
        long double time;
        long double dt;
    Clock () {
        time = 0;
        dt = 0.1;
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

extern "C++" class Heater: public Clock {
    public:
        int Qn;
        long double Re;
        long double ke;
        long double Rf;
        long double dx;
        long double L;
        long double T_entre;
        long double speed_ini;
        long double speed_final;
        vector <long double> T;
        vector <long double> T_p;
        vector <long double> Speed;
        Heater(void){
            Qn = 21;
            Re = 1.0605 * 1300.1934; //rho cp equivalent
            Rf = (1.121 * 0.3868); // rho cp du fluide
            ke = 1.031; // coeff de conduction equivalent
            L = 1.6; // longueur du milieu
            dx = L/(Qn-1);
            T_entre = 300; 
            T.assign(Qn,900);
            T_p.assign(Qn,900);
            T_p[0] = T_entre;
            T[0] = T_entre;
            speed_ini = 0.1;
            speed_final = 0.2;
            assign_speed();
        }

        void compute_single(int phase /* 1 2 ou 3 */){
            if (phase != 3) {T_entre = 565+273;}
            T.at(0) = T_entre;
            for (int i=1; i < Qn-1; i++) {
                int im1 = i-1;
                int ip1 = i+1;
                Rf = Speed[im1]*Rf;
                long double res = (dt/Re)*((ke/pow(dx,2))*(T_p[ip1] -2*T_p[i] + T_p[im1]) - (Rf/pow(dx,2)*(T_p[i]-T_p[im1]))) + T_p[i];
                T.at(i) = res;
            }
            // Bondary conditions
            T.at(Qn-1) = T[Qn-2]; 
        }
        void assign_speed(void){
            // première approximation, vitesse linéaire avec la distance en x
            // y = mx + b
            Speed.assign(Qn,0);
            for (int i=0; i < Qn ; i++ ) {
                long double res = (i*dx/L)*(speed_final-speed_ini) + speed_ini;
                Speed.at(i) = res;
            }
        }
        void replace_single(void){
            T_p = T;
        }
        void show(void){
            for (int i = 0; i != Qn; i++){
                cout << T.at(i) << " ";
                if (i == Qn -1){ cout << T.at(i);}
                //if (i%4 == 3){cout << " ";}
            }
            cout << "\n";
        }
        
        void test(){
        clean();
        write();
        replace_single();
        next();
            while(time < SIMULATION_TIME){
                compute_single(1);
                write();
                replace_single();
                next();
            }
        }
        void clean(void) {
            ofstream File("data.txt");
            File.close();
        }
        void write(void) {
            ofstream File("data.txt",std::ios_base::app);
            for (int i = 0; i != Qn; i++){
                File << T.at(i) << " ";
                if (i == Qn -1){ File << T.at(i);}
                //if (i%4 == 3){cout << " ";}
                }
            File << "\n";
            File.close();
        }
// vector <vector <long double> > Simulate_to(int time) {
//     vector < vector <long double> > mesh;
//     for (Heater garnissage = Heater(); garnissage.get_time() < time; garnissage.next()){
//         garnissage.compute_single();
//         mesh.push_back(garnissage.T);
//         garnissage.replace_single();
//     }
//     return mesh;
//}

};
int main(int argc, char* argv[]) {
    Heater h = Heater();
    h.test();
}
