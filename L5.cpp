
#include "Coordinates.hpp"

#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <vector>
#include <cmath>
#define R_earth 6371.0

using namespace std;

Coordinates::Coordinates(int x, int y) : x(x), y(y)
{
    z = 0;
    lat = 0.0;
    lon = atan2((double)x, (double)y);
}
Coordinates::Coordinates(int x, int y, int z) : x(x), y(y), z(z)
{
    lon = atan2((double)x, (double)y);
    lat = asin((double)z / R);
}

Coordinates::Coordinates(double lat, double lon, double alt = 0){
    this->lat = lat;
    this->lon = lon;
    x = R * cos(lat) * cos(lon);
    y = R * cos(lat) * sin(lon);
    z = R * sin(lat);
    this->alt = alt;
}
Coordinates::~Coordinates(){
}
int Coordinates::get_x(){
    return x;
}
int Coordinates::get_y(){
    return y;
}
int Coordinates::get_z(){
    return z;
}
void Coordinates::set_x(int x){
    this->x = x;
    lon = atan2((double)x, (double)y);
}
void Coordinates::set_y(int y){
    this->y = y;
    lon = atan2((double)x, (double)y);
}
void Coordinates::set_z(int z){
    this->z = z;
    lat = asin((double)this->z / R);
}
double Coordinates::get_lat(){
    return lat;
}
double Coordinates::get_lon(){
    return lon;
}
double Coordinates::get_alt(){
    return alt;
}

void Coordinates::set_lat(double lat){
    this->lat = lat;
    z = R * sin(this->lat);
}
void Coordinates::set_lon(double lon){
    this->lon = lon;
    x = R * cos(this->lat) * cos(this->lon);
    y = R * cos(this->lat) * sin(this->lon);
}
void Coordinates::set_alt(double alt){
    this->alt = alt;

}

int dist_point(Coordinates *A, Coordinates *B){
    return sqrt(pow((B->get_x() - A->get_x()), 2) + pow((B->get_y() - A->get_y()), 2));
}
double distance_points_lat_lon(double lat1, double lon1, double lat2, double lon2){
    double sin1=sin( (lat1-lat2) / 2.0 );
    double sin2=sin( (lon1-lon2) / 2.0 );
    return 2.0 * R_earth * asin( sqrt( sin1 * sin1 + sin2 * sin2 * cos(lat1) * cos(lat2) ) );
}

class Abs_object{
    
    
    int id;
    double sum_coord;
protected:
    Coordinates *position;
    vector <Coordinates*> coords;
public:
    Abs_object(int id1, int x, int y){
        position = new Coordinates(x, y);
        id = id1;
        coords.push_back(position);
        sum_coord = 0.0;
    }
    Abs_object(int id1){
        position = new Coordinates(0, 0);
        id = id1;
        coords.push_back(position);
        sum_coord = 0;
    }
    int get_id(){
        return id;
    }
    int get_sum_coord(){
        return sum_coord;
    }
    vector<Coordinates*> get_coords(){
        return coords;
    }
    Coordinates* get_pos(){
        return position;
    }
    virtual void randomwalk(int limit_x, int limit_y){
        int new_p = (rand() % 60) - 30;
        int xy_r = rand() % 2;
        Coordinates *new_c;
        if(new_p == 0){
            new_p = 100;
        }
        if(xy_r == 1){
            if(xy_r + new_p <= limit_x && xy_r - new_p >= 0){
                new_c = new Coordinates(position->get_x() + new_p, position->get_y());
                coords.push_back(new_c);
                sum_coord += dist_point(position, new_c);
                position = new_c;
            }
        }
        else{
            if(xy_r + new_p <= limit_y && xy_r - new_p >= 0){
                new_c = new Coordinates(position->get_x(), position->get_y() + new_p);
                coords.push_back(new_c);

                sum_coord += dist_point(position, new_c);
                position = new_c;
            }
        }
    }
    int get_count_points(){
        return coords.size();
    }
    int get_rast_coord(Coordinates *C){
        return dist_point(position, C);
    }
    virtual void print_coord(){
        std::cout<<"x = "<<position->get_x()<<"  y = "<<position->get_y()<<"\n";
    }
    void add_sum_coord(double r){
        sum_coord += r;
    }

};
class UserEquipment : public Abs_object{
    double speed_l;
    double scale;

public:
    UserEquipment(int id1, double lat, double lon, double scale) : Abs_object(id1){
        position->set_alt(0);
        position->set_lat(lat);
        position->set_lon(lon);
        this->scale = scale;
        
    }
    void randomwalk(int limit_x, int limit_y){
        int new_p = (rand() % 60) - 30;
        int xy_r = rand() % 2;
        Coordinates *new_c;
        if(new_p == 0){
            new_p = 100;
        }
        if(xy_r == 1){
            if(xy_r + new_p <= limit_x && xy_r - new_p >= 0){
                new_c = new Coordinates(position->get_lat() + (double)new_p * scale, position->get_lon() );
                coords.push_back(new_c);
                add_sum_coord(distance_points_lat_lon(new_c->get_lat(), new_c->get_lon(), position->get_lat(), position->get_lon()) );
                position = new_c;
            }
        }
        else{
            if(xy_r + new_p <= limit_y && xy_r - new_p >= 0){
                new_c = new Coordinates(position->get_lat(), position->get_lon() + (double)new_p * scale);
                coords.push_back(new_c);
                add_sum_coord(distance_points_lat_lon(new_c->get_lat(), new_c->get_lon(), position->get_lat(), position->get_lon()) );
                position = new_c;
            }
        }
    }
    virtual void print_coord(){
        std::cout<<"lat = "<<position->get_lat()<<"  lon = "<<position->get_lon()<<"\n";
    }
};


int main(){
    int map_x = 1000;
    int map_y = 1000;

    double latS = 55.004, lonS = 82.995;
    double latE = 54.9236, lonE = 83.0996;
    vector <Abs_object*> vec_obj;
    int i, i2;
    int count_obj;
    cout<<"Input count object: ";
    cin >> count_obj;
    cout<<"Input moves: ";
    int c_moves;
    cin>>c_moves;
    int i_id = 1;
    Abs_object *i_obj;
    int xp, yp;
    double scale = (lonE - lonS)/(double)map_x;
    for(i = 0; i < count_obj; ++i){
        i_obj = new UserEquipment(i_id, latS + ( (double)(rand() % map_x) * scale), lonS + ( (double)(rand() % map_y) * scale), scale );
        
        vec_obj.push_back(i_obj);
        i_id++;
    }
    for(i = 0; i < c_moves; ++i){
        cout<<"step"<<i+1<<"\n";
        for(i2 = 0; i2 < count_obj; ++i2){
            vec_obj[i2]->randomwalk(map_x, map_y);
            cout<<"\tid = "<<vec_obj[i2]->get_id()<<"\n\t\t";
            vec_obj[i2]->print_coord();
        }
    }
    for(i = 0; i < count_obj; ++i){
        cout<<"id = "<<vec_obj[i]->get_id()<<" path = "<<vec_obj[i]->get_sum_coord()<<"\n";
        cout<<"rast to:\n";
        for(i2 = 0; i2 < count_obj; ++i2){
            if(i2 != i){
                
                cout<<"\tid = "<<vec_obj[i2]->get_id()<<" - "<<vec_obj[i]->get_rast_coord(vec_obj[i2]->get_pos())<<"\n";
            }
        }
        cout<<"Points: ";
        for(i2 = 0; i2 < vec_obj[i]->get_count_points(); ++i2){
            //cout<<" x = "<<vec_obj[i]->get_coords()[i2]->get_x()<<" y = "<<vec_obj[i]->get_coords()[i2]->get_y()<<", ";
        }
        cout<<"\n";
    }
    cout<<"|end|\n";
    return 0;

}
















































