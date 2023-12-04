#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <nlohmann/json.hpp>
//#define SIMULATION_TIME 80*3

using namespace std;
class Clock {
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

class Heater: public Clock {
    public:
        int SIMULATION_TIME;
        int Qn;
        int phase; //1 gaz inlet, 2 purge, 3 outlet//
        long double phase_time;
        long double T_entre;
        long double T_bruleur;
        long double Re;
        long double ke;
        long double Rf;
        long double dx;
        long double L;
        long double speed_ini;
        long double speed_final;
        long double rcp_garn;
        int nt;
        vector <vector <long double> > data;
        vector <long double> T;
        vector <long double> T_p;
        vector <long double> Speed;
        Heater(float Length,float si,float sf ,int nt){
            nt = nt;
            phase_time = 120;
            SIMULATION_TIME =  phase_time*nt;
            T_entre = 440 + 273;
            T_bruleur = 820 + 273;
            phase = 1;
            Qn = 60;
            rcp_garn = 2600;
            Rf = (1.121 * 0.3868) ; // rho cp du fluide
            Re = ((rcp_garn+Rf)/2);//1.0605 * 1300.1934; //rho cp equivalent
            ke = 1.031; // coeff de conduction equivalent
            L = Length; // longueur du milieu
            dx = L/(Qn-1);
            cout << "discrétisation (dx) : " << dx << " [m]"<< endl;
            cout << "discrétisation (dt) : " << dt << " [s]"<< endl;
            T.assign(Qn,0);
            T_p.assign(Qn,0);
            speed_ini = si; // 0.3
            speed_final = sf; // 0.6
            Speed.assign(Qn,0.9);
            assign_speed();
            initialize_temp();
            }
        void write_attribute(void){
            ofstream attr( "attribute.json" );
            nlohmann::json j;
            j["Qn"] = Qn;
            j["dt"] = dt;
            j["dx"] = dx;
            j["nt"] = nt;
            j["longueur"] = L;
            j["temps_de_phase"] = phase_time;
            j["temperature_entre"] = T_entre;
            j["temperature_bruleur"] = T_bruleur;
            j["ke"] = ke;
            j["rcp_eq"] = Re;
            j["rcp_garnissage"] = rcp_garn;
            j["rcp_fluide"] = Rf;
            j["vitesse_initiale"] = speed_ini;
            j["vitesse_finale"] = speed_final;
            j["temps_simulation"] = SIMULATION_TIME;
            attr << j ;
            attr.close();
        }
        void initialize_temp(){
            /*Initialize temperature with experimental values*/
            long double A = 819;
            long double B = 920;
            long double C = 1026;
            for (int i=0; i < Qn; i++){
                if (i <= Qn/3) {T_p.at(i) = A; T.at(i) = A;}
                else if (i > Qn/3 && i < Qn*2/3) {T_p.at(i) = B; T.at(i) = B;}
                else {T_p.at(i) = C; T.at(i) = C;}
            }
        }
        void compute_single(){
            assign_phase();
            if (phase != 3/*/ gaz inlet  & purge */) {
                T.at(0) = T_entre;
                for (int i=1; i < Qn-1; i++) {
                    int im1 = i-1;
                    int ip1 = i+1;
                    long double Rf_eq = Speed[i]*Rf;
                    long double res = (dt/Re)*((ke/pow(dx,2))*(T_p[ip1] -2*T_p[i] + T_p[im1]) - (Rf_eq/pow(dx,2)*(T_p[i]-T_p[im1]))) + T_p[i];
                    T.at(i) = res;
                }
                // Bondary conditions
                T.at(Qn-1) = T[Qn-2];
                }
            else { /* gaz outlet */
                T.at(Qn-1) = T_bruleur;
                for (int i=1; i < Qn-1; i++) {
                    int im1 = i-1;
                    int ip1 = i+1;
                    long double Rf_eq = -Speed[i]*Rf;
                    long double res = (dt/Re)*((ke/pow(dx,2))*(T_p[ip1] -2*T_p[i] + T_p[im1]) - (Rf_eq/pow(dx,2)*(T_p[i]-T_p[im1]))) + T_p[i];
                    T.at(i) = res;
                }
                T.at(0) = T[1];
                }
        }
        void assign_speed(void){
            // première approximation, vitesse linéaire avec la distance en x
            // y = mx + b
            
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
        void assign_phase(){ 
            int i = floor(time/phase_time);
                switch ( i % 3)  {
                    case 0:
                        phase = 1;
                        break;
                    case 1:
                        phase = 2;
                        break;
                    default: //case 2
                        phase = 3;
                        break;
            }
        }
        void test(){
            clean();
            write();
            replace_single();
            next();
                while(time < SIMULATION_TIME){
                    compute_single();
                    write();
                    replace_single();
                    next();
                }
            write_attribute();
            }
        void clean(void) {
            ofstream File("data.txt");
            File.close();
        }
        void write() {
            ofstream File("data.txt",std::ios_base::app);
            for (int i = 0; i != Qn; i++){
                File << T.at(i) << " ";
                if (i == Qn -1){ File << T.at(i);}
                //if (i%4 == 3){cout << " ";}
                }
            File << "\n";
            File.close();
        }
        void stock_data (){
            data.push_back(T);
        }
};

int main(int argc, char* argv[]) {
    // ./Engine 300 1.6 -> définir Temperature initiale "300" et Longueur du garnissage "1.6" 
    float L = atof(argv[1]);
    float si = atof(argv[2]); // utilisé 1
    float sf = atof(argv[3]); //  utilisé 1.5
    int nt = atoi(argv[4]);
    // *Debug*
    // float nt = 6;
    // float L = 1.6;
    // float si = 2;
    // float sf = 4;
    cout << "*génération des données* " << endl << " " << endl;
    cout << "longueur du garnisage : " << L << " [m]" << endl;
    cout << "temps de la simulation : " << 120*nt << " [s]" << endl;
    cout << "vitesse initial : " << si << " [m/s]" << endl;
    cout << "vitesse finale : " << sf << " [m/s]" << endl;
    cout << "nombre de changement de phase : " << nt  << endl;
    Heater h = Heater(L,si,sf, nt);
    h.test();
}
