/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 1
* Programming Fundamentals Spring 2023
* Author: Vu Van Tien
* Date: 02.02.2023
*/

//The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_STUDY_IN_PINK_2_H_
#define _H_STUDY_IN_PINK_2_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class MovingObject;
class Position;
class Configuration;
class Map;

class Criminal;
class RobotS; 
class RobotW;
class RobotSW;
class RobotC;

class ArrayMovingObject;
class StudyPinkProgram;

class BaseItem;
class BaseBag;
class SherlockBag;
class WatsonBag;

class TestStudyInPink;

enum ItemType { MAGIC_BOOK, ENERGY_DRINK, FIRST_AID, EXCEMPTION_CARD, PASSING_CARD };
enum ElementType { PATH, WALL, FAKE_WALL };
enum RobotType { C = 0, S, W, SW };

void hplimit(int& hp) { 
    if (hp > 500) { 
        hp = 500; 
    } 
    else if (hp < 0) { 
        hp = 0; 
    } 

} 
void explimit(int& exp) { 
    if (exp > 900) { 
        exp = 900; 
    } 
    else if (exp < 0) { 
        exp = 0; 
    }
} 
string key(string def) { 
    string da_key; 
    for (int i = 0; i < def.length(); i++) { 
        if (def[i] == '=') { 
            da_key = def.substr(0, i); 
            break; 
        } 
    } 
    return da_key; 
}
class MapElement { 

protected: 
    ElementType type; 
public: 
    friend class TestStudyInPink; 
    MapElement(ElementType in_type = ElementType::PATH) { 
        this -> type = in_type; 
    } 
    virtual ~MapElement() {}; 
    virtual ElementType getType() const { 
        return this -> type; 
    } 
    void setType(ElementType ntype) { 
        this -> type = ntype; 
    } 
}; 

class Path : public MapElement { 
public: 
    friend class TestStudyInPink; 
    Path() : MapElement(ElementType::PATH) {} 
}; 
class Wall : public MapElement { 
public: 
    friend class TestStudyInPink; 
    Wall() : MapElement(ElementType::WALL) {} 
}; 
class FakeWall : public MapElement { 
private: 
    int req_exp; 
public: 
    friend class TestStudyInPink; 
    FakeWall(int in_req_exp) : MapElement(ElementType::FAKE_WALL) { 
        this -> req_exp = in_req_exp; 
    } 
    int getReqExp() const { 
        return this->req_exp; 
    } 
}; 
class Position { 
private:
    int r, c; 
public:
    friend class TestStudyInPink; 
    static const Position npos; 
    Position(int r = 0, int c = 0) { 
        this->r = r; 
        this->c = c; 
    } 
    Position(const string& str_pos) { 
        size_t bracket1 = str_pos.find('('); 
        size_t comma = str_pos.find(',', bracket1); 
        size_t bracket2 = str_pos.find(')', comma); 

        if (bracket1 != string::npos && comma != string::npos && bracket2 !=string::npos) { 
            r = stoi(str_pos.substr(bracket1 + 1, comma - bracket1 - 1)); 
            c = stoi(str_pos.substr(comma + 1, bracket2 - comma - 1)); 
        } else { 
            r = c = 0;  
        } 
    } 
    int getRow() const { 
        return r; 
    } 
    int getCol() const { 
        return c; 
    } 
    void setRow(int r) { 
        this->r = r; 
    } 
   void setCol(int c) { 
        this->c = c; 
    } 
    string str() const { 
        return "(" + to_string(r) + "," + to_string(c) + ")"; 
    } 
    bool isEqual(int in_r, int in_c) const { 
        if ((in_r == r) && (in_c == c)) return true; 
        else return false; 
    }; 
}; 
class MovingObject { 
protected: 
    int index; 
    Position pos; 
    Map* map; 
    string name; 
public: 
    friend class TestStudyInPink; 
    MovingObject(int index, const Position pos, Map* map, const string& name = "") { 
        this->index = index;  
        this->pos = pos;  
        this->map = map;  
        this->name = name;  
    } 
    ~MovingObject() {} 
    virtual Position getNextPosition() = 0; 
    virtual Position getCurrentPosition() const = 0; 
    virtual void move() = 0; 
    virtual string str() const = 0; 
    virtual const string getName() = 0; 
    virtual int getEXP() = 0; 
}; 
class Map { 
private: 
    int num_rows, num_cols; 
    MapElement *** map; 
public: 
    friend class TestStudyInPink; 
    Map(int num_rows, int num_cols, int num_walls, Position * array_walls, int num_fake_walls, Position * array_fake_walls) : num_rows(num_rows), num_cols(num_cols){ 
    this->map = new MapElement**[num_rows]; 
        for (int i = 0; i < num_rows; i++) { 
            this->map[i] = new MapElement*[num_cols]; 
            for (int j = 0; j < num_cols; j++) { 
                this->map[i][j] = new Path(); 
            } 

        } 
        for (int i = 0; i < num_walls; i++) { 
            int r = array_walls[i].getRow(); 
            int c = array_walls[i].getCol(); 
            delete this->map[r][c]; 
            this->map[r][c] = new Wall(); 
        } 
        for (int i = 0; i < num_fake_walls; i++) { 
            int r = array_fake_walls[i].getRow(); 
            int c = array_fake_walls[i].getCol(); 
            delete this->map[r][c]; 
            this->map[r][c] = new FakeWall((r * 257 + c * 139 + 89) % 900 + 1);  

        }
    } 
    ~Map() { 
        for(int i = 0; i < num_rows; i++) {
            for(int j = 0; j < num_cols; j++) {
                delete map[i][j];
            }
        }
        for(int i = 0; i < num_rows; i++)
            delete [] map[i];
        delete[] map;

    }  

    bool isValid(const Position& pos, MovingObject* mv_obj) const { 
        if (pos.getRow() >= this->num_rows || pos.getCol() >= this->num_cols || pos.getRow() < 0 || pos.getCol() < 0) { 
            return false; 
        } else { 
             if (mv_obj->getName() == "Watson") { 
                if (map[pos.getRow()][pos.getCol()]->getType() == WALL) { 
                    return false; 
                } else if (map[pos.getRow()][pos.getCol()]->getType() == FAKE_WALL) { 
                    FakeWall *fw = dynamic_cast<FakeWall*>(map[pos.getRow()][pos.getCol()]); 
                    return mv_obj->getEXP() > fw->getReqExp(); 
                } else { 
                    return true; 
                } 
            } else { 
                return map[pos.getRow()][pos.getCol()]->getType() != WALL; 
            } 
        } 
    } 
}; 
const Position Position::npos = Position(-1, -1); 

class Character : public MovingObject { 

public: 
    friend class StudyPinkProgram; 
    friend class TestStudyInPink; 
    Character(int index, const Position& init_pos, Map* map, const string& name) : MovingObject(index, init_pos, map, name) {} 
    virtual ~Character() {}; 
    virtual void setEXP(int exp) = 0; 
    virtual void setHP(int hp) = 0; 
    virtual int getHP() = 0; 
}; 

class Sherlock : public Character { 
private: 
    int health, experience; 
    Position sher_pos; 
    string moving_rule; 
public: 
    friend class TestStudyInPink; 
    Sherlock(int index, const string &moving_rule, const Position &init_pos, Map* map, int init_hp, int init_exp) : Character(index, init_pos, map, "Sherlock"), experience(init_exp), health(init_hp), sher_pos(init_pos), moving_rule(moving_rule) { 
        hplimit(health); 
        explimit(experience); 
    } 
    Position getNextPosition() override { 
        Position currentpos = sher_pos; 
        int step = 0; 
        if (step == (this->moving_rule.length())) step = 0; 
        switch (this->moving_rule[step]) { 
        case 'U': 
            currentpos.setRow((sher_pos.getRow()) - 1); 
            step++; 
            return currentpos; 
        case 'D': 
            currentpos.setRow((sher_pos.getRow()) + 1); 
            step++; 
            return currentpos; 
        case 'L': 
            currentpos.setCol((sher_pos.getCol()) - 1); 
            step++; 
            return currentpos; 
        case 'R': 
            currentpos.setCol((sher_pos.getCol()) + 1); 
            step++; 
            return currentpos; 
        default:
            step++; 
            return Position::npos; 
        } 
    }
    const string getName() override { 
        return name; 
    } 
    int getEXP() override { 
        return this->experience; 
    } 
    int getHP() override { 
        return this->health; 
    }
     void setEXP(int exp) override { 
        this->experience = exp; 
    } 
    void setHP(int hp) override { 
        this->health = hp; 
    } 
    void move() override { 
        Position spos = getNextPosition(); 
        if ((health == 0) || (experience == 0)) { 
            spos = Position::npos; 
        } 
        if (this->map->isValid(spos, this) == false) {} 
            else this->sher_pos = spos;
    } 
    Position getCurrentPosition() const override { 
        return sher_pos; 
    } 
    string str() const override { 
        return "Sherlock[index=" + to_string(index) + ";pos=" + sher_pos.str() + ";" + "moving_rule=" + moving_rule + "]"; 
    } 
}; 
class Watson : public Character { 
private: 
    int whp, wexp; 
    Position wat_pos;  
    string move_rule; 
public: 
    friend class TestStudyInPink; 
    Watson(int index, const string& moving_rule, const Position& init_pos, Map* map, int init_hp, int init_exp) : Character(index, init_pos, map, "Watson"), whp(init_hp), wexp(init_exp), move_rule(moving_rule), wat_pos(init_pos) { 
        hplimit(whp); 
        explimit(wexp); 
    } 
    Position getNextPosition() override { 
        Position currentpos = wat_pos; 
        int step = 0; 
        if (step == (this->move_rule.length())) step = 0; 
        switch (this->move_rule[step]) { 
            case 'U': 
                currentpos.setRow((wat_pos.getRow()) - 1); 
                step++; 
                return currentpos; 
            case 'D': 
                currentpos.setRow((wat_pos.getRow()) + 1); 
                step++; 
                return currentpos; 
            case 'L': 
                currentpos.setCol((wat_pos.getCol()) - 1); 
                step++; 
                return currentpos; 
            case 'R': 
                currentpos.setCol((wat_pos.getCol()) + 1); 
                step++; 
                return currentpos;
            default: 
                step++; 
                return Position::npos; 
        } 
    } 
    const string getName() override { 
        return "Watson";
    } 
    int getEXP() override { 
        return this->wexp;  
    }  
    int getHP() override { 
        return this->whp; 
    } 
    void setEXP(int exp) override { 
        this->wexp = exp; 
    } 
    void setHP(int hp) override { 
        this->whp = hp; 
    } 
    Position getCurrentPosition() const override { 
        return wat_pos; 
    } 
    void move() override { 
        Position wpos = getNextPosition(); 
        if ((whp == 0) || (wexp == 0)) { 
            wpos = Position::npos; 
        }
        if (this->map->isValid(wpos, this) == false) {} 
        else 
            this->wat_pos = wpos; 
    }
    string str() const override { 
        return "Watson[index=" + to_string(index) + ";pos=" + wat_pos.str() + ";" + "moving_rule=" + move_rule + "]"; 
    } 
}; 
int Manhattan(Character *mv, Position pos) { 
    return abs(mv->getCurrentPosition().getRow()-pos.getRow()) + abs(mv->getCurrentPosition().getCol()-pos.getCol()); 

} 

 

class Criminal : public Character { 
private: 
    Position crim_pos, pre_crim_pos; 
    Sherlock * sherlock; 
    Watson * watson; 
    int steps = 0; 
public: 
    friend class TestStudyInPink; 
    Criminal(int index, const Position & init_pos, Map * map, Sherlock * sherlock, Watson * watson) : Character(index, init_pos, map,"Criminal"), sherlock(sherlock), watson(watson), crim_pos(init_pos) {} 
    
    Position getNextPosition() override { 
        Position temP, U, L, D, R; 
        temP = U = L = D = R = this->crim_pos; 
        int bay_ta_hoi_pen = 0; 
        U.setRow(this->crim_pos.getRow()-1); 
        D.setRow(this->crim_pos.getRow()+1); 
        L.setCol(this->crim_pos.getCol()-1); 
        R.setCol(this->crim_pos.getCol()+1); 
        
        if (map->isValid(U,this)) { 
            if ((Manhattan(sherlock, U) + Manhattan(watson, U)) > bay_ta_hoi_pen) {
                temP = U; 
                bay_ta_hoi_pen = (Manhattan(sherlock, U) + Manhattan(watson, U)); 
            } 
        } 

        if (map->isValid(L,this)) { 
            if ((Manhattan(sherlock, L) + Manhattan(watson, L)) > bay_ta_hoi_pen) { 
                temP = L; 
                bay_ta_hoi_pen = (Manhattan(sherlock, L) + Manhattan(watson, L)); 
            } 
        } 

        if (map->isValid(D,this)) { 
            if ((Manhattan(sherlock, D) + Manhattan(watson, D)) > bay_ta_hoi_pen) { 
                temP = D; 
                bay_ta_hoi_pen = (Manhattan(sherlock, D) + Manhattan(watson, D)); 
            }
        } 

        if (map->isValid(R,this)) { 
            if ((Manhattan(sherlock, R) + Manhattan(watson, R)) > bay_ta_hoi_pen) { 
                temP = R; 
                bay_ta_hoi_pen = (Manhattan(sherlock, R) + Manhattan(watson, R)); 
            }   
        }
        return temP; 
    }
    const string getName() override { 
        return "Criminal"; 
    } 
    Position getCurrentPosition() const override { 
        return crim_pos; 
    } 
    void move() override { 
        this->steps++; 
        this->pre_crim_pos = this->crim_pos; 
        this->crim_pos=getNextPosition(); 
    } 
    Position wherehaveubeen() { 
        return this->pre_crim_pos; 
    } 
    int howmanysteps() 
    {
       return this->steps; 
    } 
    string str() const override { 
        return "Criminal[index=" + to_string(index) + ";pos=" + crim_pos.str() + "]"; 
    } 
    int getEXP() override { 
        return 0; 
    } 
    void setEXP(int exp) override {} 
    void setHP(int hp) override {} 
    int getHP() { 
        return 0; 
    } 
}; 

 

class ArrayMovingObject { 
private: 
    int count, capacity; 
    MovingObject** arr_mv_obj; 
public:
    friend class StudyPinkProgram; 
    friend class TestStudyInPink; 
    ArrayMovingObject(int capacity) : capacity(capacity), count(0) { 
        this->arr_mv_obj = new MovingObject * [capacity]; 
    } 
    ~ArrayMovingObject() { 
        delete [] arr_mv_obj; 
    } 
    bool isFull() const { 
        return (capacity == count); 
    } 
    bool add(MovingObject * mv_obj) { 
        if (isFull()==false)  
        { 
            *(arr_mv_obj+count)=mv_obj; 
            count++; 
            return true; 
        } 
        else return false;
    } 
    MovingObject * get(int index) const { 
        return arr_mv_obj[index]; 
    } 
    int size() const { 
        return count; 
    } 
    string str() const { 
        string result= "ArrayMovingObject[count=" + to_string(this->count) + ";capacity=" + to_string(this->capacity)+ ";"; 
        for(int i = 0; i < count; i++) 
        { 
            if (i == count - 1) result += arr_mv_obj[i][0].str()+"]"; 
                else result += arr_mv_obj[i][0].str()+";"; 
        } 
        return result; 
    } 
}; 

 

class Configuration { 
    friend class StudyPinkProgram; 
    friend class TestStudyInPink; 
private: 
    int map_num_rows = 0, map_num_cols = 0, max_num_moving_object = 0; 
    int num_walls = 0, num_fake_walls = 0; 
    Position* arr_walls; 
    Position* arr_fake_walls; 
    int sherlock_init_hp = 0, sherlock_init_exp = 0; 
    int watson_init_hp = 0, watson_init_exp = 0; 
    Position sherlock_init_pos, watson_init_pos, criminal_init_pos; 
    string watson_moving_rule, sherlock_moving_rule; 
    int num_steps = 0; 
public: 
    string array_walls, array_fake_walls; 
    ifstream file; 
    Configuration(const string& filepath) { 
        file.open(filepath); 
        string code, temp; 
        while (getline(file, code)) { 
            string c = key(code);

            if (c == "MAP_NUM_COLS") { 
                for (int i = 0; i < code.length(); i++) { 
                    if (code[i] == '=') { 
                        temp = code.substr(i + 1); 
                        break; 
                    } 
                } 
                this->map_num_cols = stoi(temp); 
            } 

            if (c == "MAP_NUM_ROWS") { 
                for (int i = 0; i < code.length(); i++) { 
                    if (code[i] == '=') { 
                        temp = code.substr(i + 1); 
                        break; 
                    } 
                } 
                this->map_num_rows = stoi(temp); 
            } 

            if (c == "MAX_NUM_MOVING_OBJECTS") { 
                for (int i = 0; i < code.length(); i++) { 
                    if (code[i] == '=') { 
                        temp = code.substr(i + 1); 
                        break; 
                    } 
                } 
                this->max_num_moving_object = stoi(temp); 
            } 

            if (c == "SHERLOCK_INIT_EXP") { 
                for (int i = 0; i < code.length(); i++) { 
                    if (code[i] == '=') { 
                        temp = code.substr(i + 1); 
                        break; 
                    } 
                } 
                this->sherlock_init_exp = stoi(temp); 
            } 

            if (c == "SHERLOCK_INIT_HP") { 
                for (int i = 0; i < code.length(); i++) { 
                    if (code[i] == '=') { 
                        temp = code.substr(i + 1); 
                        break; 
                    } 
                } 
                this->sherlock_init_hp = stoi(temp); 
            } 

            if (c == "WATSON_INIT_HP") { 
                for (int i = 0; i < code.length(); i++) { 
                    if (code[i] == '=') { 
                        temp = code.substr(i + 1); 
                        break; 
                    } 
                } 
                this->watson_init_hp = stoi(temp); 
            } 

 

            if (c == "WATSON_INIT_EXP") { 
                for (int i = 0; i < code.length(); i++) { 
                    if (code[i] == '=') { 
                        temp = code.substr(i + 1); 
                        break; 
                    } 
                    this->watson_init_exp = stoi(temp); 
                } 
            } 

            if (c == "NUM_STEPS") { 
                for (int i = 0; i < code.length(); i++) { 
                    if (code[i] == '=') { 
                        temp = code.substr(i + 1); 
                        break; 
                    } 
                } 
                this->num_steps = stoi(temp); 
            } 

            if (c == "SHERLOCK_INIT_POS") { 
                for (int i = 0; i < code.length(); i++) { 
                    if (code[i] == '=') { 
                        temp = code.substr(i + 1); 
                        break; 
                    } 
                } 
                this->sherlock_init_pos = Position(temp); 
            } 

            if (c == "WATSON_INIT_POS") { 
                for (int i = 0; i < code.length(); i++) { 
                    if (code[i] == '=') { 
                        temp = code.substr(i + 1); 
                        break; 
                    } 
                } 
                this->watson_init_pos = Position(temp); 
            } 

            if (c == "CRIMINAL_INIT_POS") { 
                for (int i = 0; i < code.length(); i++) { 
                    if (code[i] == '=') { 
                        temp = code.substr(i + 1); 
                        break; 
                    } 
                } 
                this->criminal_init_pos = Position(temp);
            } 

            if (c == "SHERLOCK_MOVING_RULE") { 
                for (int i = 0; i < code.length(); i++) { 
                    if (code[i] == '=') { 
                        temp = code.substr(i + 1); 
                        break; 
                    } 
                } 
                this->sherlock_moving_rule = temp; 
            } 

            if (c == "WATSON_MOVING_RULE") { 
                for (int i = 0; i < code.length(); i++) { 
                    if (code[i] == '=') { 
                        temp = code.substr(i + 1); 
                        break; 
                    } 
                } 
                this->watson_moving_rule = temp; 
            } 

            if (c == "ARRAY_WALLS") { 
                size_t t = code.find('['); 
                if (t != string::npos) array_walls = code.substr(t); 
                for (int i = 0; i < code.length(); i++) { 
                    if (code[i] == '=') { 
                        temp = code.substr(i + 2); 
                        break; 
                    } 
                } 
                for (int i = 0; i < temp.length(); i++) { 
                    if (temp[i] == ',') num_walls++; 
                } 
                arr_walls = new Position[num_walls]; 
                int n = 0, pre_smc = 0; 
                string tempos; 

                if (num_walls != 0) { 
                    for (int i = 0; i < temp.length(); i++) { 
                        if (code[i] == ';' || code[i] == ']') { 
                            if (pre_smc == 0) { 
                                tempos = temp.substr(pre_smc,i); 
                            } 
                            else tempos = temp.substr(pre_smc, i - pre_smc - 1); 
                            arr_walls[n] = Position(tempos); 
                            pre_smc = i; 
                            n++; 
                        } 
                    } 
                } 
            } 

            if (c == "ARRAY_FAKE_WALLS") { 
                size_t t = code.find('['); 
                if (t != string::npos) array_fake_walls = code.substr(t); 
                for (int i = 0; i < code.length(); i++) { 
                    if (code[i] == '=') { 
                        temp = code.substr(i + 2); 
                        break; 
                    } 
                } 

                for (int i = 0; i < temp.length(); i++) { 
                    if (temp[i] == ',') num_fake_walls++; 
                }
                arr_fake_walls = new Position[num_fake_walls]; 
                int n = 0, pre_smc = 0; 
                string tempos; 
                if (num_fake_walls != 0) { 
                for (int i = 0; i < temp.length(); i++) { 
                    if (code[i] == ';' || code[i] == ']') { 
                        if (pre_smc == 0) { 
                            tempos = temp.substr(pre_smc, i); 
                        } 
                            else tempos = temp.substr(pre_smc, i - pre_smc - 1); 
                            pre_smc = i; 
                            arr_walls[n] = Position(tempos); 
                            n++; 
                        } 
                    } 
                } 
            } 
        } 
    }
        
    ~Configuration() { 
        if (arr_walls != nullptr) delete arr_walls; 
        if (arr_fake_walls != nullptr) delete arr_fake_walls; 
        file.close(); 
    } 
        
    string str() const { 
        return "Configuration[\nMAP_NUM_ROWS=" + to_string(map_num_rows) + "\nMAP_NUM_COLS=" + to_string(map_num_cols) +  
            "\nMAX_NUM_MOVING_OBJECT=" + to_string(max_num_moving_object) + "\nNUM_WALLS=" + to_string(num_walls) + "\nARRAY_WALLS=" + array_walls +  
                "\nNUM_FAKE_WALLS=" + to_string(num_fake_walls) + "\nARRAY_FAKE_WALLS=" + array_fake_walls + "\nSHERLOCK_MOVING_RULE=" + sherlock_moving_rule + 
                    "\nSHERLOCK_INIT_POS=" + sherlock_init_pos.str() + "\nSHERLOCK_INIT_HP=" + to_string(sherlock_init_hp) + "\nSHERLOCK_INIT_EXP=" + to_string(sherlock_init_exp) +                         
                        "\nWATSON_MOVING_RULE=" + watson_moving_rule + "\nWATSON_INIT_POS=" + watson_init_pos.str() + "\nWATSON_INIT_HP=" + to_string(watson_init_hp) + "\nWATSON_INIT_EXP=" +  
                            to_string(watson_init_exp) + "\nCRIMINAL_INIT_POS=" + criminal_init_pos.str() + "\nNUM_STEPS=" + to_string(num_steps) + "\n]"; 
    } 

 

    int getMapNumRows() const{ 
        return map_num_rows; 
    }; 

    int getMapNumCols() const{ 
        return map_num_cols; 
    }; 

    int getMaxNumMovingObjects() const{ 
        return max_num_moving_object; 
    }; 

    int getNumWalls() const{ 
        return num_walls; 
    }; 

    Position * getArrWalls() const{ 
        return arr_walls; 
    }; 

    int getNumFakeWalls() const{ 
        return num_fake_walls; 
    }; 

    Position * getArrFakeWalls() const{ 
        return arr_fake_walls; 
    }; 

    string getSherlockMovingRules() const{ 
        return sherlock_moving_rule; 
    }; 

    Position getSherlockInitPos() const{ 
        return sherlock_init_pos; 
    }; 

    int getSherlockInitHp() const{ 
        return sherlock_init_hp; 
    }; 

    string getWatsonMovingRules() const{ 
        return watson_moving_rule; 
    }; 

    Position getWatsonInitPos() const{ 
        return watson_init_pos; 
    }; 

    int getWatsonInitHp() const{ 
        return watson_init_hp; 
    }; 

    int getWatsonInitExp() const{ 
        return watson_init_exp; 
    }; 

    Position getCriminalInitPos() const{ 
        return criminal_init_pos; 
    };

    int getNumSteps() const{ 
        return num_steps; 
    }; 
}; 

class Robot : public MovingObject {
private:
    RobotType robot_type;
    BaseItem * item;
public:
    friend class TestStudyInPink;
    Criminal * criminal;
    Sherlock * sherlock;
    Watson * watson;
    Robot(int index, const Position &init_pos, Map *map, RobotType robot_type, Criminal *criminal);
    virtual ~Robot() {}

    RobotType getRobotType() const {
        return this->robot_type;
    }
};

class RobotC : public Robot {
private:
    Position robot_pos;
public:
    friend class TestStudyPink;
    RobotC(int index, const Position &init_pos, Map *map, Criminal *criminal) : Robot(index, init_pos, map, RobotType::C, criminal), robot_pos(init_pos) {}
    ~RobotC() {}
    Position getNextPosition() override {
        if (this->map->isValid(criminal->wherehaveubeen(),this)) 
            return criminal->wherehaveubeen(); 
        return Position::npos;
    }

    int getDistance(Sherlock * sherlock) const {
        return Manhattan(sherlock,robot_pos);
    }

    int getDistance(Watson * watson) const {
        return Manhattan(watson,robot_pos);
    }

    void move () override {
        Position cpos = getNextPosition();
        if (Position::npos.isEqual(cpos.getRow(),cpos.getCol())==true) {}
            else this->robot_pos = getNextPosition();
    }
    
    string str() const override {
        return "Robot[pos=" + robot_pos.str() + ";type=C;dist=]";
    }

    int getEXP() override {
        return 0;
    }
};

class RobotS : public Robot {
private:
    Position robot_pos;
public:
    friend class TestStudyPink;
    RobotS(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock * sherlock) : Robot(index, init_pos, map, RobotType::S, criminal), robot_pos(init_pos) {}
    
    ~RobotS() {}

    Position getNextPosition() override {
        Position temP, U, L, D, R; 
        temP = U = L = D = R = this->robot_pos; 
        int bay_ta_hoi_pen = 10000000000;

        U.setRow(this->robot_pos.getRow()-1); 
        D.setRow(this->robot_pos.getRow()+1); 
        L.setCol(this->robot_pos.getCol()-1); 
        R.setCol(this->robot_pos.getCol()+1); 
        
        if (map->isValid(U,this)) { 
            if ((Manhattan(sherlock, U)) < bay_ta_hoi_pen) {
                temP = U; 
                bay_ta_hoi_pen = (Manhattan(sherlock, U)); 
            } 
        } 

        if (map->isValid(R,this)) { 
            if ((Manhattan(sherlock, R)) < bay_ta_hoi_pen) { 
                temP = R; 
                bay_ta_hoi_pen = Manhattan(sherlock, R); 
            }   
        }

        if (map->isValid(D,this)) { 
            if (Manhattan(sherlock, D) < bay_ta_hoi_pen) { 
                temP = D; 
                bay_ta_hoi_pen = Manhattan(sherlock, D);
            }
        } 

        if (map->isValid(L,this)) { 
            if (Manhattan(sherlock, L) > bay_ta_hoi_pen) { 
                temP = L; 
                bay_ta_hoi_pen = Manhattan(sherlock, L); 
            } 
        } 
        return temP; 
    }

    int getDistance(Sherlock * sherlock) const {
        return Manhattan(sherlock,robot_pos);
    }

    string str() const override {
        return "Robot[pos=" + robot_pos.str() + ";type=S;dist=" + to_string(getDistance(sherlock)) + "]";
    }

    void move() override {
        Position cpos = getNextPosition();
        if (Position::npos.isEqual(cpos.getRow(),cpos.getCol())==true) {}
            else this->robot_pos = getNextPosition();
    }

    int getEXP() override {
        return 0;
    }
};


class RobotW : public Robot {
private:
    Position robot_pos;
public:
    friend class TestStudyInPink;
    RobotW(int index, const Position &init_pos, Map *map, Criminal *criminal, Watson * watson) : Robot(index, init_pos, map, RobotType::W, criminal), robot_pos(init_pos) {}
    ~RobotW() {}

    Position getNextPosition() override {
        Position temP, U, L, D, R; 
        temP = U = L = D = R = this->robot_pos; 
        int bay_ta_hoi_pen = 10000000000;

        U.setRow(this->robot_pos.getRow()-1); 
        D.setRow(this->robot_pos.getRow()+1); 
        L.setCol(this->robot_pos.getCol()-1); 
        R.setCol(this->robot_pos.getCol()+1); 
        
        if (map->isValid(U,this)) { 
            if ((Manhattan(watson, U)) < bay_ta_hoi_pen) {
                temP = U; 
                bay_ta_hoi_pen = (Manhattan(watson, U)); 
            } 
        } 

        if (map->isValid(R,this)) { 
            if ((Manhattan(watson, R)) < bay_ta_hoi_pen) { 
                temP = R; 
                bay_ta_hoi_pen = Manhattan(watson, R); 
            }   
        }

        if (map->isValid(D,this)) { 
            if (Manhattan(watson, D) < bay_ta_hoi_pen) { 
                temP = D; 
                bay_ta_hoi_pen = Manhattan(watson, D);
            }
        } 

        if (map->isValid(L,this)) { 
            if (Manhattan(watson, L) > bay_ta_hoi_pen) { 
                temP = L; 
                bay_ta_hoi_pen = Manhattan(watson, L); 
            } 
        } 
        return temP; 
    }

    int getEXP() override {
        return 0;
    }

    int getDistance(Watson * watson) const {
        return Manhattan(watson,robot_pos);
    }

    string str() const override {
        return "Robot[pos=" + robot_pos.str() + ";type=W;dist=" + to_string(getDistance(watson)) + "]";
    }

    void move() override {
        Position cpos = getNextPosition();
        if (Position::npos.isEqual(cpos.getRow(),cpos.getCol())==true) {}
            else this->robot_pos = getNextPosition();
    }
};

class RobotSW : public Robot {
private:
    Position robot_pos;
public:
    friend class TestStudyInPink;
    RobotSW(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock * sherlock, Watson * watson) : Robot(index, init_pos, map, RobotType::SW, criminal), robot_pos(init_pos) {}
    ~RobotSW() {}

    Position getNextPosition() override {
        Position temP, U, L, D, R; 
        temP = U = L = D = R = this->robot_pos; 
        int bay_ta_hoi_pen = 10000000000;

        U.setRow(this->robot_pos.getRow()-2); 
        D.setRow(this->robot_pos.getRow()+2); 
        L.setCol(this->robot_pos.getCol()-2); 
        R.setCol(this->robot_pos.getCol()+2); 
        
        if (map->isValid(U,this)) { 
            if ((Manhattan(watson, U) + Manhattan(sherlock,U)) < bay_ta_hoi_pen) {
                temP = U; 
                bay_ta_hoi_pen = (Manhattan(watson, U) + Manhattan(sherlock,U)); 
            } 
        } 

        if (map->isValid(R,this)) { 
            if ((Manhattan(watson, R) + Manhattan(sherlock,R)) < bay_ta_hoi_pen) { 
                temP = R; 
                bay_ta_hoi_pen = Manhattan(watson, R) + Manhattan(sherlock,R); 
            }   
        }

        if (map->isValid(D,this)) { 
            if ((Manhattan(watson, D) + Manhattan(sherlock,D)) < bay_ta_hoi_pen) { 
                temP = D; 
                bay_ta_hoi_pen = Manhattan(watson, D) + Manhattan(sherlock,D);
            }
        } 

        if (map->isValid(L,this)) { 
            if ((Manhattan(watson, L) + Manhattan(sherlock,L)) > bay_ta_hoi_pen) { 
                temP = L; 
                bay_ta_hoi_pen = Manhattan(watson, L) + Manhattan(sherlock,L); 
            } 
        } 
        return temP; 
    }

    int getEXP() override {
        return 0;
    }

    int getDistance() const {
        return Manhattan(watson,robot_pos) + Manhattan(sherlock,robot_pos);
    }

    string str() const override {
        return "Robot[pos=" + robot_pos.str() + ";type=SW;dist=" + to_string(getDistance()) + "]";
    }

    void move() override {
        Position cpos = getNextPosition();
        if (Position::npos.isEqual(cpos.getRow(),cpos.getCol())==true) {}
            else this->robot_pos = getNextPosition();
    }
};

/*
class StudyPinkProgram {
private:
    // Sample attributes
    Configuration * config;
    
    Sherlock * sherlock;
    Watson * watson;
    Criminal * criminal;
    
    Map * map;
    ArrayMovingObject * arr_mv_objs;


public:
    StudyPinkProgram(const string & config_file_path);

    bool isStop() const;

    void printResult() const {
        if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
            cout << "Sherlock caught the criminal" << endl;
        }
        else if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
            cout << "Watson caught the criminal" << endl;
        }
        else {
            cout << "The criminal escaped" << endl;
        }
    }

    void printStep(int si) const {
        cout << "Step: " << setw(4) << setfill('0') << si
            << "--"
            << sherlock->str() << "--|--" << watson->str() << "--|--" << criminal->str() << endl;
    }

    void run(bool verbose) {
        // Note: This is a sample code. You can change the implementation as you like.
        // TODO
        for (int istep = 0; istep < config->num_steps; ++istep) {
            for (int i = 0; i < arr_mv_objs->size(); ++i) {
                arr_mv_objs->get(i)->move();
                if (isStop()) {
                    printStep(istep);
                    break;
                }
                if (verbose) {
                    printStep(istep);
                }
            }
        }
        printResult();
    }

    ~StudyPinkProgram();
};
 */
////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* _H_STUDY_IN_PINK_2_H_ */
