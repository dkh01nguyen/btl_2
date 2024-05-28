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
            string number1, number2;
            for (int i = bracket1 + 1; i < comma; i++) {
                if (str_pos[i] - '0' >= 0 && str_pos[i] - '0' <= 9) number1 += str_pos[i];
            }
            r = stoi(number1);
            for (int i = comma + 1; i < bracket2; i++) {
                if (str_pos[i] - '0' >= 0 && str_pos[i] - '0' <= 9) number2 += str_pos[i];
            }
            c = stoi(number2); 
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

    bool isEqual(Position pos) const { 
        if ((pos.getRow() == r) && (pos.getCol() == c)) return true; 
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
    virtual string getName() const = 0;
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
    int step = 0;
public: 
    friend class TestStudyInPink; 
    Sherlock(int index, const string &moving_rule, const Position &init_pos, Map* map, int init_hp, int init_exp);
    Position getNextPosition() { 
        Position currentpos = sher_pos; 
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
    string getName() const { 
        return name; 
    } 
    int getEXP()  { 
        return this->experience; 
    } 
    int getHP() { 
        return this->health; 
    }
     void setEXP(int exp) { 
        this->experience = exp; 
    } 
    void setHP(int hp) { 
        this->health = hp; 
    } 
    void move() {
        Position spos = getNextPosition(); 
        if ((health == 0) || (experience == 0)) { 
            spos = Position::npos; 
        } 
        if (this->map->isValid(spos, this) == false) {} 
            else this->sher_pos = spos;
    } 
    Position getCurrentPosition() const  { 
        return sher_pos; 
    } 
    string str() const { 
        return "Sherlock[index=" + to_string(index) + ";pos=" + sher_pos.str() + ";" + "moving_rule=" + moving_rule + "]"; 
    } 
}; 
class Watson : public Character { 
private: 
    int whp, wexp; 
    Position wat_pos;  
    string move_rule;
    int step = 0;
public: 
    friend class TestStudyInPink; 
    Watson(int index, const string& moving_rule, const Position& init_pos, Map* map, int init_hp, int init_exp);
    Position getNextPosition() { 
        Position currentpos = wat_pos; 
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
    string getName() const { 
        return "Watson";
    } 
    int getEXP()  { 
        return this->wexp;  
    }  
    int getHP()  { 
        return this->whp; 
    } 
    void setEXP(int exp)  { 
        this->wexp = exp; 
    } 
    void setHP(int hp)  { 
        this->whp = hp; 
    } 
    Position getCurrentPosition() const  { 
        return wat_pos; 
    } 
    void move()  { 
        Position wpos = getNextPosition(); 
        if ((whp == 0) || (wexp == 0)) { 
            wpos = Position::npos; 
        }
        if (this->map->isValid(wpos, this) == false) {} 
        else 
            this->wat_pos = wpos; 
    }
    string str() const  { 
        return "Watson[index=" + to_string(index) + ";pos=" + wat_pos.str() + ";" + "moving_rule=" + move_rule + "]"; 
    } 
}; 
class Criminal : public Character { 
protected:
    Position crim_pos;
    Sherlock * sherlock; 
    Watson * watson; 
    int steps = 0; 
public:
    friend class TestStudyInPink; 
    friend class RobotC;
    Criminal(int index, const Position & init_pos, Map * map, Sherlock * sherlock, Watson * watson) : Character(index, init_pos, map,"Criminal"), sherlock(sherlock), watson(watson), crim_pos(init_pos) {} 
    Position getNextPosition();
    Position pre_crim_pos = Position::npos;
    string getName() const  { 
        return "Criminal"; 
    } 
    Position getCurrentPosition() const  { 
        return crim_pos; 
    } 
    void move()  {
        this->steps++;
        this->pre_crim_pos = this->crim_pos;
        this->crim_pos = this->getNextPosition();
    }

    Position getPreviousPosition() {
        return this->pre_crim_pos;
    }
    int howmanysteps() {
       return this->steps; 
    } 
    string str() const  { 
        return "Criminal[index=" + to_string(index) + ";pos=" + getCurrentPosition().str() + "]"; 
    } 
    int getEXP()  { 
        return 0; 
    } 
    void setEXP(int exp)  {} 
    void setHP(int hp)  {} 
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
    int map_num_rows = 0, map_num_cols = 0, max_num_moving_objects = 0; 
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
    Configuration(const string& filepath);    
    ~Configuration() { 
        delete[] arr_walls; 
        delete[] arr_fake_walls; 
        file.close(); 
    } 
        
    string str() const { 
        return "Configuration[\nMAP_NUM_ROWS=" + to_string(map_num_rows) + "\nMAP_NUM_COLS=" + to_string(map_num_cols) +  
            "\nMAX_NUM_MOVING_OBJECTS=" + to_string(max_num_moving_objects) + "\nNUM_WALLS=" + to_string(num_walls) + "\nARRAY_WALLS=" + array_walls +  
                "\nNUM_FAKE_WALLS=" + to_string(num_fake_walls) + "\nARRAY_FAKE_WALLS=" + array_fake_walls + "\nSHERLOCK_MOVING_RULE=" + sherlock_moving_rule + 
                    "\nSHERLOCK_INIT_POS=" + sherlock_init_pos.str() + "\nSHERLOCK_INIT_HP=" + to_string(sherlock_init_hp) + "\nSHERLOCK_INIT_EXP=" + to_string(sherlock_init_exp) +                         
                        "\nWATSON_MOVING_RULE=" + watson_moving_rule + "\nWATSON_INIT_POS=" + watson_init_pos.str() + "\nWATSON_INIT_HP=" + to_string(watson_init_hp) + "\nWATSON_INIT_EXP=" +  
                            to_string(watson_init_exp) + "\nCRIMINAL_INIT_POS=" + criminal_init_pos.str() + "\nNUM_STEPS=" + to_string(num_steps) + "\n]"; 
    } 

 

    int getMapNumRows() const{ 
        return map_num_rows; 
    }

    int getMapNumCols() const{ 
        return map_num_cols; 
    }

    int getMaxNumMovingObjects() const{ 
        return max_num_moving_objects; 
    }

    int getNumWalls() const{ 
        return num_walls; 
    }

    Position * getArrWalls() const{ 
        return arr_walls; 
    }

    int getNumFakeWalls() const{ 
        return num_fake_walls; 
    }

    Position * getArrFakeWalls() const{ 
        return arr_fake_walls; 
    }

    string getSherlockMovingRules() const{ 
        return sherlock_moving_rule; 
    }

    Position getSherlockInitPos() const{ 
        return sherlock_init_pos; 
    }

    int getSherlockInitHp() const{ 
        return sherlock_init_hp; 
    }

    string getWatsonMovingRules() const{ 
        return watson_moving_rule; 
    }

    Position getWatsonInitPos() const{ 
        return watson_init_pos; 
    }

    int getWatsonInitHp() const{ 
        return watson_init_hp; 
    }

    int getWatsonInitExp() const{ 
        return watson_init_exp; 
    }

    Position getCriminalInitPos() const{ 
        return criminal_init_pos; 
    }

    int getNumSteps() const{ 
        return num_steps; 
    }
}; 

class Robot : public MovingObject {
protected:
    RobotType robot_type;
    BaseItem * item;
public:
    Criminal * criminal;
    Sherlock * sherlock;
    Watson * watson;
    friend class TestStudyInPink;
    Robot(int index, const Position &init_pos, Map *map, RobotType robot_type, Criminal *criminal) : MovingObject(index, init_pos, map, ""){}
    virtual ~Robot() {}
};

class BaseItem {
public:
    ItemType clgv;
    friend class TestStudyinPink;
    virtual void use( Character * obj , Robot * robot ) = 0;
    virtual bool canUse(Character* obj , Robot * robot ) = 0;
};

class MagicBook : public BaseItem {
public:
    friend class TestStudyinPink;
    MagicBook() {
        clgv = MAGIC_BOOK;
    }
    ~MagicBook() {}
    bool canUse(Character * obj, Robot * robot) {
        if (robot == nullptr) {
            if (obj->getEXP() <= 350) return true;
                else return false;
        }
        return false;
    }

    void use(Character * obj, Robot * robot) {
        obj->setEXP((obj->getEXP() * 1.25));     
    }

    ItemType getType() {
        return this->clgv;
    }
};

class EnergyDrink : public BaseItem {
public:
    friend class TestStudyinPink;
    EnergyDrink() {
        clgv = ENERGY_DRINK;
    }
    ~EnergyDrink() {}
    bool canUse(Character * obj, Robot * robot) {
        if (robot == nullptr) {
            if (obj->getHP() <= 100) return true;
                else return false;
        }
        return false;
    }

    void use(Character * obj, Robot * robot) {
        obj->setEXP((obj->getHP() * 1.2));     
    }
};

class FirstAid : public BaseItem {
public:
    friend class TestStudyinPink;
    FirstAid() {
        clgv = FIRST_AID;
    }
    ~FirstAid() {}
    bool canUse(Character * obj, Robot * robot) {
        if (robot == nullptr) {
            if (obj->getEXP() <= 350 || obj->getHP() <= 100) return true;
                else return false;
        }
        return false;
    }

    void use(Character * obj, Robot * robot) {
        obj->setEXP((obj->getHP() * 1.5));     
    }
};

class ExcemptionCard : public BaseItem {
public:
    friend class TestStudyinPink;
    ExcemptionCard() {
        clgv = EXCEMPTION_CARD;
    }
    ~ExcemptionCard() {}
    bool canUse(Character * obj, Robot * robot) {
        if (obj->getName() == "Sherlock" && (obj->getEXP() % 2 != 0)) return true;
            else return false;
    }

    void use(Character * obj, Robot * robot) {}
};

class PassingCard : public BaseItem {
public:
    friend class TestStudyinPink;
    string challenge;
    PassingCard(string challenge) : challenge(challenge) {
        clgv = PASSING_CARD;
    }
    bool canUse(Character * obj, Robot * robot) {
        if (obj->getName() == "Watson" && (obj->getEXP() % 2 == 0)) return true;
            else return false;
    }

    ~PassingCard() {}
    
    void use(Character * obj, Robot * robot) {}
};

class RobotC : public Robot {
private:
    Position robot_pos;
public:
    Criminal * criminal;
    friend class TestStudyPink;
    friend class Criminal;
    RobotC(int index, const Position &init_pos, Map *map, Criminal *criminal);
    ~RobotC() {}
    int getDistance(Sherlock * sherlock) const;
    int getDistance(Watson * watson) const;

    Position getNextPosition() {
        return this->criminal->getPreviousPosition();
    }   

    void move() {
        if (Position::npos.isEqual(this->criminal->getPreviousPosition())) {}
            else {
                this->robot_pos.setRow(this->criminal->getPreviousPosition().getRow());
                this->robot_pos.setCol(this->criminal->getPreviousPosition().getCol());
            }
        cout << robot_pos.str() << endl;
    }

    Position getCurrentPosition() const {
        return robot_pos;
    }
    string str() const  {
        return "Robot[pos=" + this->getCurrentPosition().str() + ";type=C;dist=]";
    }
    string getName() const {
        return "RobotC";
    }
    int getEXP()  {
        return 0;
    }
};

class RobotS : public Robot {
private:
    Position robot_pos;
public:
    Sherlock * sherlock;
    friend class TestStudyPink;
    RobotS(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock * sherlock);    
    ~RobotS() {}
    Position getCurrentPosition() const {
        return robot_pos;
    }
    Position getNextPosition();
    int getDistance(Sherlock * sherlock) const;
    string str() const  {
        return "Robot[pos=" + robot_pos.str() + ";type=S;dist=" + to_string(getDistance(sherlock)) + "]";
    }
    void move()  {
        Position cpos = getNextPosition();
        if (Position::npos.isEqual(cpos.getRow(),cpos.getCol())==true) {}
            else this->robot_pos = getNextPosition();
    }
    string getName() const {
        return "RobotS";
    }
    int getEXP()  {
        return 0;
    }
};


class RobotW : public Robot {
private:
    Position robot_pos;
public:
    Watson * watson;
    friend class TestStudyInPink;
    RobotW(int index, const Position &init_pos, Map *map, Criminal *criminal, Watson * watson);
    ~RobotW() {}
    Position getNextPosition();
    int getDistance() const;

    Position getCurrentPosition() const {
        return robot_pos;
    }
    string getName() const {
        return "RobotW";
    }
    int getEXP()  {
        return 0;
    }
    string str() const  {
        return "Robot[pos=" + robot_pos.str() + ";type=W;dist=" + to_string(getDistance()) + "]";
    }
    void move()  {
        Position cpos = getNextPosition();
        if (Position::npos.isEqual(cpos.getRow(),cpos.getCol())==true) {}
            else this->robot_pos = getNextPosition();
    }
};

class RobotSW : public Robot {
private:
    Position robot_pos;
public:
    Sherlock * sherlock;
    Watson * watson;
    friend class TestStudyInPink;
    RobotSW(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock * sherlock, Watson * watson);
    ~RobotSW() {}
    Position getNextPosition();

    string getName() const {
        return "RobotSW";
    }

    Position getCurrentPosition() const {
        return robot_pos;
    }
    int getEXP()  {
        return 0;
    }
    int getDistance() const;
    string str() const  {
        return "Robot[pos=" + robot_pos.str() + ";type=SW;dist=" + to_string(getDistance()) + "]";
    }
    void move()  {
        Position cpos = getNextPosition();
        if (Position::npos.isEqual(cpos.getRow(),cpos.getCol())==true) {}
            else this->robot_pos = getNextPosition();
    }
};

class BaseBag {
protected:
    struct slot {
        BaseItem * item;
        slot* next;
    };
    Character* obj;

public:
    friend class TestStudyinPink;
    BaseBag(Character* obj) : obj(obj) {}
    virtual ~BaseBag() = default;
    virtual bool insert(BaseItem *item) = 0;
    virtual BaseItem *get() = 0;
    virtual BaseItem *get(ItemType itemType) = 0;
    virtual string str() const = 0;

    slot* makeSlot(BaseItem *item) {
        slot * temp = new slot;
        temp->item = item;
        temp->next = nullptr;
        return temp;
    }
};

class SherlockBag : public BaseBag {
private:
    int capacity = 13;
    Sherlock* sherlock;
    int count = 0;
    slot* head = NULL;
public:

    SherlockBag(Sherlock* sherlock) : BaseBag(sherlock), sherlock(sherlock) {}
    ~SherlockBag() {}

    bool insert(BaseItem* item)  {
        if(count >= capacity || item == NULL) {
            return false;
        } else {
            slot * tam = makeSlot(item);
            
            if (head == nullptr) {
                head = tam;
            }
            else {
                tam->next = head;
                head = tam;
            }
            ++count;
        }
        return true;
    }
    string str() const;
    BaseItem* get() {
        if(count==0) return NULL;
        slot* slot = head;
        while((slot->item->clgv != ItemType::MAGIC_BOOK) && (slot->item->clgv!=ItemType::ENERGY_DRINK) && (slot->item->clgv!=ItemType::FIRST_AID)) {
            if(slot->next != NULL)
                slot = slot->next;
            else return NULL;
        }
        BaseItem* temp = slot->item;
        if (head->item == slot->item) {}
            else { 
                slot->item = head->item;
                head->item = temp;
            }
        head = head->next;
        count--;
        return temp;
    }

    bool exist(ItemType type) {
        if (head == NULL) return false;
        slot* slot = head;
        for(int i = 0; i < 13; i++){
            if(slot->item->clgv == type) return true;
                else if(slot->next==NULL) break;
            slot=slot->next;
        }
        return false;
    }

    BaseItem* get (ItemType type) { 
        if(count==0) return NULL;
        slot* slot = head;
        while (slot->item->clgv != type) {
            if(slot->next!=NULL)
                slot = slot->next;
            else return NULL;
        }
        BaseItem* temp = slot->item;
        if (head->item == slot->item) {}
            else { 
            slot->item = head->item;
            head->item = temp;
            }
        head=head->next;
        count--;
        return temp;
    }
};  

class WatsonBag : public BaseBag {
private:
    int capacity = 15;
    Watson* watson;
    int count = 0;
    slot* head = NULL;
public:

    WatsonBag(Watson *watson) : BaseBag(watson), watson(watson) {}
    ~WatsonBag() {}

    bool insert(BaseItem* item)  {
        if(count >= capacity) {
            return false;
        } else {
            slot * tam = makeSlot(item);
            if (head == nullptr) {
                head = tam;
            }
            else {
                tam->next = head;
                head = tam;
            }
            ++count;
        }
        return true;
    }

    BaseItem* get() {
        if(count==0) return NULL;
        slot* slot = head;
        while((slot->item->clgv != ItemType::MAGIC_BOOK) && (slot->item->clgv!=ItemType::ENERGY_DRINK) && (slot->item->clgv!=ItemType::FIRST_AID)) {
            if(slot->next != NULL)
                slot = slot->next;
            else return NULL;
        }
        BaseItem* temp = slot->item;
        if (head->item == slot->item) {}
            else { 
                slot->item = head->item;
                head->item = temp;
            }
        head = head->next;
        count--;
        return temp;
    }
    string str() const;

    bool exist(ItemType type) {
        if (head == NULL) return false;
        slot* slot = head;
        for(int i = 0; i < 13; i++){
            if(slot->item->clgv == type) return true;
                else if(slot->next==NULL) break;
            slot=slot->next;
        }
        return false;
    }   

    BaseItem* get (ItemType type) { 
        if(count==0) return NULL;
        slot* slot = head;
        while (slot->item->clgv != type) {
            if(slot->next!=NULL)
                slot = slot->next;
            else return NULL;
        }
        BaseItem* temp = slot->item;
        if (head->item == slot->item) {}
            else { 
            slot->item = head->item;
            head->item = temp;
            }
        head=head->next;
        count--;
        return temp;
    }
};


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

    bool isStop() const {
        if (sherlock->getHP() == 1 || sherlock->getHP() == 1 || watson->getCurrentPosition().isEqual(criminal->getCurrentPosition().getRow(),criminal->getCurrentPosition().getCol()) || sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition().getRow(),criminal->getCurrentPosition().getCol())) return true;
        return false;
    }

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

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /*_H_STUDY_IN_PINK_2_H_ */
