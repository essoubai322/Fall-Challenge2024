#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <unordered_set>
#include <map>

using namespace std;

typedef struct Money
{
    int total;
    int teleporta = 5000;
    int pod = 1000;
} MoneyStruct;

typedef struct astronauts_Types
{
    int id;
    int type;
    int max;
    int flag = 0;
    pair<int,int> pos;
} TypesStruct;

typedef struct Pad
{
    int id;
    int flag = 0;
    int max;
    int type;
    int counter;
    vector<pair<int,int>> astronautv2;
    pair<int,int> pos;
} PadStruct;

struct Selection
{
    int from;
    int to;
    int distance;
    int score;
    int resources;
    int type;
};

struct Point {
    int x;
    int y;
};

struct build {
    int id;
    int type;
    Point cord;
    bool isTeleport;
    map<int, int> astrSum;

};

struct tubeSucess
{
    int from;
    int to;
};

struct teleSucess
{
    int form;
    int to;
    pair<int,int> pos;
    int TeleType;
};

typedef struct Citys 
{   
    vector<PadStruct> pads;
    vector<TypesStruct> typesList;
    vector<Selection> selection;
    map<int, build> builds;
    vector<pair<int, int>> linked;
    vector<tubeSucess> tube_sucess;
    vector<teleSucess> tele_sucess;
} city;

TypesStruct parse_string(const std::string& str) 
{
    TypesStruct build;
    istringstream iss(str);
    
    iss >> build.type >> build.id >> build.pos.first >> build.pos.second;
    build.max = 0;
    return (build);
}

PadStruct parse_string_pad(const std::string& str) 
{
    PadStruct build;
    istringstream iss(str);
    int astronautType;
    unordered_map<int, int> astronautCount;
    
    iss >> build.type >> build.id >> build.pos.first >> build.pos.second >> build.counter;
    while (iss >> astronautType)
    {
        astronautCount[astronautType]++;
    }
    for (const auto& entry : astronautCount)
    {
        build.astronautv2.push_back(make_pair(entry.first, entry.second));
    }
    build.max = 0;
    return (build);
}

double distance(const pair<int, int>& p1, const pair<int, int>& p2) 
{
    return (sqrt(pow(p2.first - p1.first, 2) + pow(p2.second - p1.second, 2)));
}

bool onSegment(Point p, Point q, Point r) {
    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
        q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
        return true;
    return false;
}

int orientation(Point p, Point q, Point r) {
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0;
    return (val > 0) ? 1 : 2;
}

bool doIntersect(Point p1, Point q1, Point p2, Point q2) {
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    if (o1 != o2 && o3 != o4)
        return true;

    if (o1 == 0 && onSegment(p1, p2, q1)) return true;
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false;
}

const double EPSILON = 1e-9;

bool isPointOnLineSegment(const Point& A, const Point& B, const Point& P) {
    double crossProduct = (P.y - A.y) * (B.x - A.x) - (P.x - A.x) * (B.y - A.y);
    if (std::abs(crossProduct) > EPSILON) {
        return false;
    }
    return (std::min(A.x, B.x) <= P.x + EPSILON && P.x <= std::max(A.x, B.x) + EPSILON &&
            std::min(A.y, B.y) <= P.y + EPSILON && P.y <= std::max(A.y, B.y) + EPSILON);
}

bool checkAllIntersection(city& cityData, int from, int to) 
{
    // cerr << "Checking intersection for buildings " << from << " and " << to << endl;

    if (cityData.builds.find(from) == cityData.builds.end() || cityData.builds.find(to) == cityData.builds.end()) 
    {
        // cerr << "Error: Building " << from << " or " << to << " not found in builds map" << endl;
        return false; 
    }

    Point p1 = {cityData.builds.at(from).cord.x, cityData.builds.at(from).cord.y};
    Point q1 = {cityData.builds.at(to).cord.x, cityData.builds.at(to).cord.y};

    for (const auto& link : cityData.linked) 
    {
        if (cityData.builds.find(link.first) == cityData.builds.end() || cityData.builds.find(link.second) == cityData.builds.end()) 
        {
            // cerr << "Error: Building " << link.first << " or " << link.second << " not found in builds map" << endl;
            continue;
        }

        Point p2 = {cityData.builds.at(link.first).cord.x, cityData.builds.at(link.first).cord.y};
        Point q2 = {cityData.builds.at(link.second).cord.x, cityData.builds.at(link.second).cord.y};

        if ((link.first != from && link.first != to) && (link.second != from && link.second != to)) 
        {
            if (doIntersect(p1, q1, p2, q2)) {
                // cerr << "Intersection found between (" << from << "," << to << ") and (" << link.first << "," << link.second << ")" << endl;
                return true;
            }
        }
    }
    for (const auto& buildPair : cityData.pads ) 
    {
        Point q;
        q.x = buildPair.pos.first;
        q.y = buildPair.pos.second;
        if ((std::abs(q.x - p1.x) < EPSILON && std::abs(q.y - p1.y) < EPSILON) || 
            (std::abs(q.x - q1.x) < EPSILON && std::abs(q.y - q1.y) < EPSILON)) {
            continue;
        }
        if (isPointOnLineSegment(p1, q1, q)) 
        {
            return true;
        }
    }
    for (const auto& buildPair : cityData.typesList ) 
    {
        Point q;
        q.x = buildPair.pos.first;
        q.y = buildPair.pos.second;
        if ((std::abs(q.x - p1.x) < EPSILON && std::abs(q.y - p1.y) < EPSILON) || 
            (std::abs(q.x - q1.x) < EPSILON && std::abs(q.y - q1.y) < EPSILON)) {
            continue;
        }
        if (isPointOnLineSegment(p1, q1, q)) 
        {
            return true;
        }
    }
    // cerr << "No intersection found for buildings " << from << " and " << to << endl;
    return false;
}

bool isDuplicateSelection(const vector<Selection>& selections, int from, int to) {
    for (const auto& selection : selections) {
        if (selection.from == from && selection.to == to) {
            return true;  // Duplicate found
        }
    }
    return false;  // No duplicate found
}

bool checkID(city &myCity, int building_id_1, int building_id_2) 
{
    for (const auto& link : myCity.linked) 
    {
        if ((link.first == building_id_1 && link.second == building_id_2) ||
            (link.first == building_id_2 && link.second == building_id_1)) 
            return false;
    }
    int count_1 = 0, count_2 = 0;
    for (const auto& route : myCity.linked)
    {
        if (building_id_1 == route.first || building_id_1 == route.second)
            count_1++;
        if (building_id_2 == route.first || building_id_2 == route.second)
            count_2++;
    }
    return ((count_1 < 5 && count_2 < 5));
}

bool addTravelRoute(city &myCity, int building_id_1, int building_id_2) 
{
    for (const auto& link : myCity.linked) 
    {
        if ((link.first == building_id_1 && link.second == building_id_2) ||
            (link.first == building_id_2 && link.second == building_id_1)) 
            return false;
    }
    myCity.linked.push_back({building_id_1, building_id_2});
    return true;
}

bool checkTube(city &myCity, int from, int to)
{
    for (const auto& check : myCity.tube_sucess)
    {
        if (check.from == from && check.to == to)
            return false;
    }
    return true;
}


int main()
{
    int index = 0;
    city selenia;
    MoneyStruct money;
    while (1) 
    {
        int resources;
        cin >> resources; cin.ignore();
        money.total = resources;
        
        int num_travel_routes;
        cin >> num_travel_routes; cin.ignore();
        
        for (int i = 0; i < num_travel_routes; i++) 
        {
            int building_id_1, building_id_2, capacity;
            cin >> building_id_1 >> building_id_2 >> capacity;
            cin.ignore();
            // if (addTravelRoute(selenia, building_id_1, building_id_2))
            //     cerr << "Travel route added: " << building_id_1 << " <-> " << building_id_2 << endl;
            // else
            //     cerr << "Duplicate travel route: " << building_id_1 << " <-> " << building_id_2 << endl;
        }
        // for (int b = 0;selenia.linked.size() > b ;b++)
        // {
        //     cerr << "FROM : "<< selenia.linked[b].first << " TO : " << selenia.linked[b].second << endl;
        //     b++;
        // }
        int num_pods;
        cin >> num_pods; cin.ignore();
        for (int i = 0; i < num_pods; i++) 
        {
            string pod_properties;
            getline(cin, pod_properties);
        }
        
        int num_new_buildings;
        cin >> num_new_buildings; cin.ignore();

        for (int i = 0; i < num_new_buildings; i++) 
        {
            TypesStruct build;
            PadStruct build_type0;
            string building_properties;
            getline(cin, building_properties);
            build = parse_string(building_properties);
            if (build.type == 0)
            {
                build_type0 = parse_string_pad(building_properties);
                selenia.pads.push_back(build_type0);
            }
            else
                selenia.typesList.push_back(build);
        }
        // for(int i = 0; i < selenia.typesList.size(); i++)
        // {
        //     cerr << "Build ID :"<< selenia.typesList[i].id <<endl;
        //     cerr << "TYPE :"<< selenia.typesList[i].type <<endl;
        //     cerr << "FLAG :" << selenia.typesList[i].flag << endl;
        //     cerr << "X =" << selenia.typesList[i].pos.first << endl;
        //     cerr << "Y =" << selenia.typesList[i].pos.second << endl;
        //     cerr << "#############################################################"<< endl;

        // }
        // for(int i = 0; i < selenia.pads.size(); i++)
        // {
        //     int y = 0;
        //     cerr << "+Build ID :"<< selenia.pads[i].id <<endl;
        //     cerr << "+TYPE :"<< selenia.pads[i].type <<endl;
        //     cerr << "+Counter :"<< selenia.pads[i].counter <<endl;
        //     cerr << "+X =" << selenia.pads[i].pos.first << endl;
        //     cerr << "+Y =" << selenia.pads[i].pos.second << endl;
        //     while(y < (int)selenia.pads[i].astronautv2.size())\
        //     {
        //         cerr << "+astronaut[" << y << "].1 :" << selenia.pads[i].astronautv2[y].first <<endl;
        //         cerr << "+astronaut["<< y << "].2 :"<< selenia.pads[i].astronautv2[y].second <<endl;
        //         cerr << "+++++++++++++++++++++++++++" << endl;
        //         y++;
        //     }
        //     cerr << "#############################################################"<< endl;
        // }

        for (int i = 0; i < selenia.pads.size(); i++) 
        {
            for (int j = 0; j < selenia.pads[i].astronautv2.size(); j++) 
            {
                // cerr << "-----------------------" << endl;
                // cerr << "ID PAD: " << selenia.pads[i].id << endl;
                int astronaut_type = selenia.pads[i].astronautv2[j].first;
                int num_astronauts = selenia.pads[i].astronautv2[j].second;
                // cerr << "Type[" << j << "] = " << astronaut_type <<endl;
                for (int k = 0; k < selenia.typesList.size(); k++) 
                {
                    // cerr << selenia.pads[i].max << " () " << selenia.typesList[k].max << endl;
                    // cerr << "typebuild : " << selenia.typesList[k].type << " = " << astronaut_type << endl;
                    if (selenia.typesList[k].type == astronaut_type) 
                    {
                        Selection temp;
                        selenia.pads[i].max++;
                        selenia.typesList[k].max++;
                        temp.from = selenia.pads[i].id;
                        temp.to = selenia.typesList[k].id;
                        temp.distance = static_cast<int>(round(distance(selenia.pads[i].pos, selenia.typesList[k].pos)));
                        temp.score = num_astronauts * 1000 / (temp.distance + 1); 
                        double dist = hypot(selenia.pads[i].pos.first - selenia.typesList[k].pos.first, selenia.pads[i].pos.second - selenia.typesList[k].pos.second) * 1.1;
                        int travelCost = money.pod + (dist / 0.1);
                        temp.resources = travelCost;
                        temp.type = selenia.typesList[k].type;
                        if (!isDuplicateSelection(selenia.selection, temp.from, temp.to))
                            selenia.selection.push_back(temp);
                    }
                }
            }
        }
        sort(selenia.selection.begin(), selenia.selection.end(), [](const Selection& a, const Selection& b) {
            return a.score > b.score;
        });
        // for (int a = 0;selenia.selection.size() > a ; a++)
        // {
        //     cerr << "----from :" << selenia.selection[a].from << endl;
        //     cerr << "----to :" << selenia.selection[a].to << endl;
        //     cerr << "----distance :" << selenia.selection[a].distance << endl;
        //     cerr << "----score :" << selenia.selection[a].score << endl;
        //     cerr << "----resources :" << selenia.selection[a].resources << endl;
        // }
        
       for (const auto& pad : selenia.pads) {
            selenia.builds[pad.id] = {pad.id, 0, {pad.pos.first, pad.pos.second}, false, {}};
        }
        for (const auto& building : selenia.typesList) {
            selenia.builds[building.id] = {building.id, building.type, {building.pos.first, building.pos.second}, false, {}};
        }

 
        int a = 0;
        bool apa;
        for (int i = 0;selenia.selection.size() > i ; i++)
        {
            bool intersects = checkAllIntersection(selenia, selenia.selection[i].from, selenia.selection[i].to);
            apa = intersects;
            // cerr << "??? :" << selenia.selection[i].from << " | " << selenia.selection[i].to << endl;
            if (checkID(selenia,selenia.selection[i].from,selenia.selection[i].to) && !intersects)
            {
                // cerr << "flos :" << selenia.selection[i].resources << " | " << money.total << endl;
                if (money.total < selenia.selection[i].resources)
                    break;
                money.total -= selenia.selection[i].resources;
                cout << "TUBE " << selenia.selection[i].from << " " << selenia.selection[i].to << ";";
                cout << "POD " << index++ << " ";
                
                for (int j = 0; selenia.pads.size() ; j++)
                {
                    if (selenia.selection[i].from == selenia.pads[j].id)
                    {
                        for (int b = 0 ;selenia.pads[j].astronautv2.size() > b ; b++)
                        {
                            if (selenia.pads[j].astronautv2[b].first == selenia.selection[i].type)
                            {
                                // cerr << selenia.pads[j].astronautv2[b].first << " != "<< selenia.selection[i].type << endl;
                                tubeSucess temp;
                                int count = selenia.pads[j].astronautv2[b].second;
                                cout << selenia.selection[i].from << " " << selenia.selection[i].to << " " << selenia.selection[i].from;
                                temp.from = selenia.selection[i].from;
                                temp.to = selenia.selection[i].to;
                                addTravelRoute(selenia, selenia.selection[i].from, selenia.selection[i].to);
                                a = 1;
                                cout << ";";
                                selenia.tube_sucess.push_back(temp);
                                break;
                            }
                        }
                    }
                    else if (a == 1)
                    {
                        a = 2;
                        break;
                    }
                }
            }
        }
        
    for (const auto& check : selenia.tube_sucess)
    {
        cerr << "check from : " << check.from << endl;
        cerr << "check to :" << check.to << endl;
    }
if ((money.total >= 8500))
{
    int c = 0, k = 0, p = 0;
    bool firstCommand = true;
    while (selenia.pads.size() > k)
    {
        while (selenia.typesList.size() > c && money.total >= 5000)
        {
            //check in pad have most astronaut;
            for (int u = 0; selenia.pads[k].astronautv2.size() > u  ;u++)
            {
                if (selenia.pads[k].astronautv2[u].second > selenia.pads[k].astronautv2[p].second)
                    p = u;
            }
            
            if (selenia.pads[k].astronautv2[p].first == selenia.typesList[c].type && !selenia.pads[k].flag && !selenia.typesList[c].flag && checkTube(selenia, selenia.pads[k].id, selenia.typesList[c].id))
            {
                money.total -= money.teleporta;
                if (!firstCommand) {
                    cout << ";";
                }
                cout << "TELEPORT " << selenia.pads[k].id << " " << selenia.typesList[c].id;
                teleSucess temp;
                temp.form = selenia.pads[k].id;
                temp.to = selenia.typesList[c].id;
                temp.TeleType = selenia.typesList[c].type;
                temp.pos = selenia.pads[k].pos;
                selenia.tele_sucess.push_back(temp); 
                selenia.pads[k].flag = 1;
                selenia.typesList[c].flag = 1;
                a = 1;
                firstCommand = false;
                for (int g = 0; selenia.pads[k].astronautv2.size() > g ;g++)
                {
                    for (int h = 0; selenia.typesList.size() > h ;h++)
                    {
                        if ((selenia.pads[k].astronautv2[g].first == selenia.typesList[h].type) && selenia.typesList[c].id != selenia.typesList[k].id )
                        {
                            cout << ";TUBE " << selenia.typesList[h].id << " " << selenia.typesList[c].id << ";POD " << index++ << " " << selenia.typesList[c].id << " " << selenia.typesList[h].id << " " << selenia.typesList[c].id  << ";";
                        }
                    }
                }
                break;
            }
            c++;
        }
            for (int p = 0; selenia.pads.size() > p ; p++)
            {
                int s = 0;
                if (!selenia.pads[p].flag)
                {
                    for (int o = 0; selenia.tele_sucess.size() > o ;o++)
                    {
                        for (int a = 0; selenia.pads[p].astronautv2.size() > a ;a++)
                        {
                            if (selenia.pads[p].astronautv2[a].first == selenia.tele_sucess[o].TeleType)
                            {
                                cout << "TUBE " << selenia.pads[p].id << " " << selenia.tele_sucess[o].form << ";";
                                cout << "POD " << index++ << " " << selenia.pads[p].id << " " << selenia.tele_sucess[o].form << " " << selenia.pads[p].id << ";";
                                selenia.pads[p].flag = 1;
                                s = 1;
                                break;
                            }
                        }
                        if (s)
                            break;
                    } 
                }
                if (s)
                    break;
            }
        k++;
        c = 0;
    }
}
cerr << "floss: " << money.total << endl;
cerr << "selenia.tele_sucess: " << selenia.tele_sucess.size() << endl;
cerr << "selenia.pads: " << selenia.pads.size() << endl;


    for (int p = 0; selenia.pads.size() > p ; p++)
    {
        if (!selenia.pads[p].flag)
        {
            for (int o = 0; selenia.tele_sucess.size() > o ;o++)
            {
                for (int a = 0; selenia.pads[p].astronautv2.size() > a ;a++)
                {
                    if (selenia.pads[p].astronautv2[a].first == selenia.tele_sucess[o].TeleType)
                    {
                        cout << "TUBE " << selenia.pads[p].id << " " << selenia.tele_sucess[o].form << ";";
                        cout << "POD " << index++ << " " << selenia.pads[p].id << " " << selenia.tele_sucess[o].form << " " << selenia.pads[p].id << ";";
                        selenia.pads[p].flag = 1;
                        break;
                    }
                }
            }
        }
    }


    if (!a)
        cout << "WAIT";
    cout << endl;


        // if (!index)
        // {
        //     cout << "TUBE 0 1;POD 0 0 1 0 1" << endl;
        //     index++;
        // }
        // else
        //     cout << "WAIT" << endl;

    }
}
