



class Coordinates{
public:
    Coordinates(int x, int y);
    Coordinates(int x, int y, int z);
    Coordinates(double lat, double lon, double alt);

    ~Coordinates();
    int get_x();
    int get_y();
    int get_z();

    void set_x(int x);
    void set_y(int y);
    void set_z(int z);

    double get_lat();
    double get_lon();
    double get_alt();

    void set_lat(double lat);
    void set_lon(double lon);
    void set_alt(double alt);


private:
    int x, y, z;
    double lat, lon, alt;
    double R = 6371.0;
};

