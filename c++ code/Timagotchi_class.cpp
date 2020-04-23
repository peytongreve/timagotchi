#ifndef Timagotchi_h
#define Timagotchi_h
#include <time.h> // checks the time

class Timagotchi {
  int x_pos = 64; // x-coord middle of the timagotchi
  int y_pos = 80; // y-coord middle of the timagotchi
  int radius = 40; // radius of the timagotchi
  float gpa; // between 0.0 and 5.0
  uint8_t happiness; // how happy the timagotchi is (-18 - 15)
  uint8_t hunger; // how hungry the timagotchi is (based off of you feeding it and time, decrements one every 8 hours)
  uint8_t major; // major number, 0 if undeclared
  uint8_t class_units; // number of MIT units the timagotchi is taking
  float total_units; // total number of units completed
  bool has_urop;
  char extracurriculars[200]; // char array separated by commas for extracurriculars
  uint8_t num_ecs; // number of extracurriculars
  char user[];
  time_t last_time_fed;
  public:
  Timagotchi(char* input_user, uint8_t declare_major, uint8_t starting_semester) {
    gpa = 0.0;
    happiness = 10;
    hunger = 10;
    major = declare_major;
    class_units = 0;
    total_units = 0;
    has_urop = 0;
    strcpy(user, input_user);
    last_time_fed = time(NULL);
  }

  void complete_semester(float semester_gpa) {
    float total_gpa = gpa*semester;
    gpa = (total_gpa + semester_gpa) / (semester + 1);
    semester++;
    total_units += class_units;
    class_units = 0;
    has_urop = 0;
    calc_happiness();
  }

  void register_semester(uint8_t units, bool urop) {
    class_units = units;
    has_urop = urop;
    calc_happiness();
  }

  // change the major
  void change_major(uint8_t new_major) {
    major = new_major;
    calc_happiness();
  }

  // adds an extracurricular to the char array
  void add_extracurricular(char* this_ec) {
    char comma[] = ",";
    strcat(extracurriculars, comma);
    strcat(extracurriculars,  this_ec);
    num_ecs++;
    calc_happiness();
  }
  
  // if we successfully remove the extracurrciular, return true, else return false
  bool remove_extracurricular(char* this_ec) {
    char* ptr;
    char delimiter = ',';
    char temp[200] = ",";
    bool res = false;
    ptr = strtok(extracurriculars, &delimiter);
    if (strcmp(ptr, this_ec) == 0) {
      res = true;
      num_ecs--;
    }
    while (ptr != NULL) {
      ptr = strtok(NULL, &delimiter);
      if (strcmp(ptr, this_ec) == 0) {
        res = true;
        num_ecs--;
      } else {
        strcat(temp, delimiter);
        strcat(temp, ptr);
      }
    }
    extracurriculars = temp;
    calc_happiness();
    return res;
  }

  // feed the timagotchi
  void feed(uint8_t food_units) {
    hunger -= food_units;
    last_time_fed = time(NULL);
    calc_happiness();
  }

  // if uses energy to do something, increments hunger by 1
  void use_energy() {
    hunger += 1;
    calc_happiness();
  }

  private:
  // called after every function that changes the rep in order to update happiness
  void calc_happiness() {
    happiness = 0;
    
    if (num_ecs >= 1 && num_ecs <= 3) {
      happiness += 1*num_ecs;
    } else if (num_ecs < 1) {
      happiness -= 1;
    } else {
      happiness -= 2;
    }

    if (gpa > 4.5) {
      happiness += 3
    } else if (gpa > 4.0) {
      happiness += 2;
    } else if (gpa > 3.0) {
      happiness += 1;
    } else if (gpa < 2.0) {
      happiness -= 2;
    }

    if (has_urop) {
      happiness += 1;
    }

    if (class_units > 54) {
      happiness -= 2;
    } else if (class_units >= 48) {
      happiness += 2;
    }

    if (major == 11 || major == 12 || major == 15 || major == 17 || major == 21) {
      happiness += 4;
    } else if (major == 1 || major == 6 || major == 9 || major == 14 || major == 22 || major == 24) {
      happiness += 3;
    } else if (major == 2 || major == 3 || major == 4 || major == 7 || major == 18 || major == 20) {
      happiness += 2;
    } else if (major == 8 || major == 10 || major == 11) {
      happiness += 1;
    } else if (major == 5) {
      happiness -= 10;
    } else {
      happiness -= 1;
    }

    if (hunger <= 4) {
      happiness -= 2;
    } else if (hunger >= 10) {
      happiness -= 1;
    } else {
      happiness += 2;
    }
  }
}