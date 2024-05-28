#include "study_in_pink2.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

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

size_t findEqual(string def) { 
    return def.find("=");
}

string flip(ItemType type) {
    string str;
    switch (type) {
        case 0: 
            str = "MAGIC_BOOK";
            break;
        
        case 1: 
            str="ENERGY_DRINK";
            break;
        
        case 2: 
            str="FIRST_AID";
            break;
        
        case 3: 
            str="EXCEMPTION_CARD";
            break;
        
        case 4: 
            str="PASSING_CARD";
            break;
    }
    return str;
}

int Manhattan(Character *mv, Position pos) { 
    return abs(mv->getCurrentPosition().getRow()-pos.getRow()) + abs(mv->getCurrentPosition().getCol()-pos.getCol()); 

} 

int findSpecialNumber(int x, int y) {
    int p = x * y;
    int s = 0;

    string pStr = to_string(p);
    
    while (pStr.length() > 1) {
        s = 0;
        for (char c : pStr) {
            s += c - '0';
        }
        pStr = to_string(s);
    }
    
    return stoi(pStr);
}
const Position Position::npos = Position(-1, -1);

Sherlock::Sherlock(int index, const string &moving_rule, const Position &init_pos, Map* map, int init_hp, int init_exp) : Character(index, init_pos, map, "Sherlock"), experience(init_exp), health(init_hp), sher_pos(init_pos), moving_rule(moving_rule) { 
    hplimit(health); 
    explimit(experience); 
} 
Watson::Watson(int index, const string& moving_rule, const Position& init_pos, Map* map, int init_hp, int init_exp) : Character(index, init_pos, map, "Watson"), whp(init_hp), wexp(init_exp), move_rule(moving_rule), wat_pos(init_pos) { 
    hplimit(whp); 
    explimit(wexp); 
}
Position Criminal::getNextPosition() { 
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

Configuration::Configuration(const string& filepath) { 
    file.open(filepath); 
    while (!file.eof()) {
        string data;
        file >> data;
        if (data.find("MAP_NUM_COLS") != string::npos) { 
            this->map_num_cols = stoi(data.substr(findEqual(data) + 1));
        } 

        if (data.find("MAP_NUM_ROWS") != string::npos) { 
            this->map_num_rows = stoi(data.substr(findEqual(data) + 1));
        } 

        if (data.find("MAX_NUM_MOVING_OBJECTS") != string::npos) { 
            this->max_num_moving_objects = stoi(data.substr(findEqual(data) + 1)); 
        } 

        if (data.find("SHERLOCK_INIT_EXP") != string::npos) { 
            this->sherlock_init_exp = stoi(data.substr(findEqual(data) + 1)); 
        } 

        if (data.find("SHERLOCK_INIT_HP") != string::npos) { 
            this->sherlock_init_hp = stoi(data.substr(findEqual(data) + 1)); 
        } 

        if (data.find("WATSON_INIT_HP") != string::npos) { 
            this->watson_init_hp = stoi(data.substr(findEqual(data) + 1));   
        } 

        if (data.find("WATSON_INIT_EXP") != string::npos) { 
            this->watson_init_exp = stoi(data.substr(findEqual(data) + 1));   
        }

        if (data.find("NUM_STEPS") != string::npos) { 
            this->num_steps = stoi(data.substr(findEqual(data) + 1));
        } 

        if (data.find("SHERLOCK_INIT_POS") != string::npos) { 
            this->sherlock_init_pos = Position(data.substr(findEqual(data) + 1)); 
        } 

        if (data.find("WATSON_INIT_POS") != string::npos) { 
            this->watson_init_pos = Position(data.substr(findEqual(data) + 1)); 
        } 

        if (data.find("CRIMINAL_INIT_POS") != string::npos) { 
            this->criminal_init_pos = Position(data.substr(findEqual(data) + 1));
        } 

        if (data.find("SHERLOCK_MOVING_RULE") != string::npos) { 
            this->sherlock_moving_rule = data.substr(findEqual(data) + 1); 
        } 

        if (data.find("WATSON_MOVING_RULE") != string::npos) { 
            this->watson_moving_rule = data.substr(findEqual(data) + 1); 
        } 

        if (data.find("ARRAY_WALLS") != string::npos) { 
            size_t t = data.find('['); 
            if (t != string::npos) array_walls = data.substr(t);

            string temp = data.substr(findEqual(data) + 2);

            for (int i = 0; i < temp.length(); i++) { 
                if (temp[i] == ',') num_walls++; 
            }

            arr_walls = new Position[num_walls]; 
            int n = 0, pre_smc = 0; 
            string tempos; 

            if (num_walls != 0) { 
                for (int i = 0; i < temp.length(); i++) { 
                    if (temp[i] == ';' || temp[i] == ']') { 
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

        if (data.find("ARRAY_FAKE_WALLS") != string::npos) {
            size_t t = data.find('['); 
            if (t != string::npos) array_fake_walls = data.substr(t); 
            string temp = data.substr(findEqual(data) + 2);

            for (int i = 0; i < temp.length(); i++) { 
                if (temp[i] == ',') num_fake_walls++; 
            }
            arr_fake_walls = new Position[num_fake_walls]; 
            int n = 0, pre_smc = 0; 
            string tempos; 
            if (num_fake_walls != 0) { 
            for (int i = 0; i < temp.length(); i++) { 
                if (temp[i] == ';' || temp[i] == ']') { 
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

RobotC::RobotC(int index, const Position &init_pos, Map *map, Criminal *criminal) : Robot(index, init_pos, map, RobotType::C, criminal), robot_pos(init_pos) {
    int s = findSpecialNumber(init_pos.getRow(),init_pos.getCol());
    if (s < 2) this->item=new MagicBook;
    if (s >= 2 && s < 4) this->item=new EnergyDrink;
    if (s >= 4 && s < 6) this->item=new FirstAid;
    if (s >= 6 && s < 8) this->item=new ExcemptionCard;
    if (s >= 8 && s < 10) {
        int t=(init_pos.getRow()*11+init_pos.getCol())%4;
        switch(t) {
            case 0:
                this->item=new PassingCard("RobotS");
                break;
            case 1:
                this->item=new PassingCard("RobotC");
                break;
            case 2:
                this->item=new PassingCard("RobotSW");
                break;
            case 3:
                this->item=new PassingCard("all");
                break;
        }
    }
}

int RobotC::getDistance(Sherlock * sherlock) const {
        return Manhattan(sherlock,robot_pos);
    }

int RobotC::getDistance(Watson * watson) const {
    return Manhattan(watson,robot_pos);
}

RobotS::RobotS(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock * sherlock) : Robot(index, init_pos, map, RobotType::S, criminal), robot_pos(init_pos) {
    int s = findSpecialNumber(init_pos.getRow(),init_pos.getCol());
    if (s < 2) this->item=new MagicBook;
    if (s >= 2 && s < 4) this->item=new EnergyDrink;
    if (s >= 4 && s < 6) this->item=new FirstAid;
    if (s >= 6 && s < 8) this->item=new ExcemptionCard;
    if (s >= 8 && s < 10) {
        int t=(init_pos.getRow()*11+init_pos.getCol())%4;
        switch(t) {
            case 0:
                this->item=new PassingCard("RobotS");
                break;
            case 1:
                this->item=new PassingCard("RobotC");
                break;
            case 2:
                this->item=new PassingCard("RobotSW");
                break;
            case 3:
                this->item=new PassingCard("all");
                break;
        }
    }
}

Position RobotS::getNextPosition()  {
    Position temP, U, L, D, R; 
    temP = U = L = D = R = this->robot_pos; 
    int bay_ta_hoi_pen = 10000;

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

int RobotS::getDistance(Sherlock * sherlock) const {
    return Manhattan(sherlock,robot_pos);
}

RobotW::RobotW(int index, const Position &init_pos, Map *map, Criminal *criminal, Watson * watson) : Robot(index, init_pos, map, RobotType::C, criminal), robot_pos(init_pos) {
    int s = findSpecialNumber(init_pos.getRow(),init_pos.getCol());
    if (s < 2) this->item=new MagicBook;
    if (s >= 2 && s < 4) this->item=new EnergyDrink;
    if (s >= 4 && s < 6) this->item=new FirstAid;
    if (s >= 6 && s < 8) this->item=new ExcemptionCard;
    if (s >= 8 && s < 10) {
        int t=(init_pos.getRow()*11+init_pos.getCol())%4;
        switch(t) {
            case 0:
                this->item=new PassingCard("RobotS");
                break;
            case 1:
                this->item=new PassingCard("RobotC");
                break;
            case 2:
                this->item=new PassingCard("RobotSW");
                break;
            case 3:
                this->item=new PassingCard("all");
                break;
        }
    }
}

Position RobotW::getNextPosition()  {
    Position temP, U, L, D, R; 
    temP = U = L = D = R = this->robot_pos; 
    int bay_ta_hoi_pen = 10000;

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

int RobotW::getDistance() const {
    return Manhattan(watson,robot_pos);
}

RobotSW::RobotSW(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock * sherlock, Watson * watson) : Robot(index, init_pos, map, RobotType::SW, criminal), robot_pos(init_pos) {
    int s = findSpecialNumber(init_pos.getRow(),init_pos.getCol());
    if (s < 2) this->item=new MagicBook;
    if (s >= 2 && s < 4) this->item=new EnergyDrink;
    if (s >= 4 && s < 6) this->item=new FirstAid;
    if (s >= 6 && s < 8) this->item=new ExcemptionCard;
    if (s >= 8 && s < 10) {
        int t=(init_pos.getRow()*11+init_pos.getCol())%4;
        switch(t) {
            case 0:
                this->item=new PassingCard("RobotS");
                break;
            case 1:
                this->item=new PassingCard("RobotC");
                break;
            case 2:
                this->item=new PassingCard("RobotSW");
                break;
            case 3:
                this->item=new PassingCard("all");
                break;
        }
    }
}

Position RobotSW::getNextPosition() {
    Position temP, U, L, D, R, UL, UR, DL, DR; 
    temP = U = L = D = R = UL = UR = DL = DR = this->robot_pos; 
    int bay_ta_hoi_pen = 10000;

    U.setRow(this->robot_pos.getRow()-2);   
    D.setRow(this->robot_pos.getRow()+2); 
    L.setCol(this->robot_pos.getCol()-2); 
    R.setCol(this->robot_pos.getCol()+2);

    UL.setRow(this->robot_pos.getRow()-1);
    UL.setCol(this->robot_pos.getCol()-1);

    UR.setRow(this->robot_pos.getRow()-1);
    UR.setCol(this->robot_pos.getCol()+1);

    DL.setRow(this->robot_pos.getRow()+1);
    DL.setCol(this->robot_pos.getCol()-1);

    DR.setRow(this->robot_pos.getRow()+1);
    DR.setCol(this->robot_pos.getCol()+1);
    
    if (map->isValid(U,this)) { 
        if ((Manhattan(watson, U) + Manhattan(sherlock,U)) < bay_ta_hoi_pen) {
            temP = U; 
            bay_ta_hoi_pen = (Manhattan(watson, U) + Manhattan(sherlock,U)); 
        } 
    } 

    if (map->isValid(UR,this)) { 
        if ((Manhattan(watson, UR) + Manhattan(sherlock,UR)) < bay_ta_hoi_pen) {
            temP = UR; 
            bay_ta_hoi_pen = (Manhattan(watson, UR) + Manhattan(sherlock,UR)); 
        } 
    }

    if (map->isValid(R,this)) { 
        if ((Manhattan(watson, R) + Manhattan(sherlock,R)) < bay_ta_hoi_pen) { 
            temP = R; 
            bay_ta_hoi_pen = Manhattan(watson, R) + Manhattan(sherlock,R); 
        }   
    }

    if (map->isValid(DR,this)) { 
        if ((Manhattan(watson, DR) + Manhattan(sherlock, DR)) < bay_ta_hoi_pen) {
            temP = DR; 
            bay_ta_hoi_pen = (Manhattan(watson, DR) + Manhattan(sherlock, DR)); 
        } 
    }

    if (map->isValid(D,this)) { 
        if ((Manhattan(watson, D) + Manhattan(sherlock,D)) < bay_ta_hoi_pen) { 
            temP = D; 
            bay_ta_hoi_pen = Manhattan(watson, D) + Manhattan(sherlock,D);
        }
    } 

    if (map->isValid(DL,this)) { 
        if ((Manhattan(watson, DL) + Manhattan(sherlock,DL)) < bay_ta_hoi_pen) {
            temP = DL; 
            bay_ta_hoi_pen = (Manhattan(watson, DL) + Manhattan(sherlock,DL)); 
        } 
    }

    if (map->isValid(L,this)) { 
        if ((Manhattan(watson, L) + Manhattan(sherlock,L)) > bay_ta_hoi_pen) { 
            temP = L; 
            bay_ta_hoi_pen = Manhattan(watson, L) + Manhattan(sherlock,L); 
        } 
    } 

    if (map->isValid(UL,this)) { 
        if ((Manhattan(watson, UL) + Manhattan(sherlock,UL)) < bay_ta_hoi_pen) {
            temP = UL; 
            bay_ta_hoi_pen = (Manhattan(watson, UL) + Manhattan(sherlock, UL)); 
        } 
    } 

    return temP;       
}

int RobotSW::getDistance() const {
    return Manhattan(watson,robot_pos) + Manhattan(sherlock,robot_pos);
}

string WatsonBag::str() const {
    string str = "Bag[count=" +to_string(this->count);
    slot * slot = head;
    for (int i = 0; i < count; i++) {
        str += ";" + flip(slot->item->clgv);
        slot=slot->next;
        continue;
    }
    str += "]";
    return str;
}

string SherlockBag::str() const {
    string str = "Bag[count=" +to_string(this->count);
    slot * slot = head;
    for (int i = 0; i < count; i++) {
        str += ";" + flip(slot->item->clgv);
        slot=slot->next;
        continue;
    }
    str += "]";
    return str;
}
////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////