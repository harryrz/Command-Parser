//
//  parser.cpp
//  lab3
//
//  Modified by Tarek Abdelrahman on 2020-10-04.
//  Created by Tarek Abdelrahman on 2018-08-25.
//  Copyright © 2018-2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.


#include <iostream>
#include <sstream>
#include <string>

using namespace std;

#include "globals.h"
#include "Shape.h"


// This is the shape array, to be dynamically allocated
Shape** shapesArray;

// The number of shapes in the database, to be incremented 
int shapeCount = 0;

// The value of the argument to the maxShapes command
int max_shapes;


// ECE244 Student: you may want to add the prototype of
// helper functions you write here
bool check_val_command(string& command, string commandList[]);
Shape**& set_shape_array(Shape**& shapesArray, stringstream& lineStream, string& command, int& max_shapes, int& shapeCount, int& commandValue);
bool check_val_arg_int(stringstream& lineStream, int& commandValue);
bool check_val_arg_str(stringstream& lineStream, string& commandTerm);
bool check_shape_name(string& commandTerm, string sensitivityList[]);
bool check_name_exists(Shape**& shapesArray, string& commandTerm, int& shapeCount);
bool check_shape_type(string& commandTerm, string shapeTypesList[]);
void del_reset(Shape**& shapesArray, int index);
bool check_loc_sz_value(int& commandValue);
bool check_circle_size(Shape**& shapesArray, int& shapeCount);
bool check_more_arg(stringstream& lineStream, string& commandTerm);
void create_shape(Shape**& shapesArray, stringstream& lineStream, string& command, int& max_shapes, int& shapeCount, int& commandValue, string& commandTerm, string sensitivityList[], string shapeTypesList[]);
int check_find_name(Shape**& shapesArray, string& commandTerm, int& shapeCount);
void move_shape(Shape**& shapesArray, stringstream& lineStream, string& command, int& commandValue, string& commandTerm, string sensitivityList[]);
bool check_rotation_angle(int& commandValue);
void rotate_shape(Shape**& shapesArray, stringstream& lineStream, string& command, int& commandValue, string& commandTerm, string sensitivityList[]);
void retrieve_a_shape(Shape**& shapesArray, string& commandTerm, int& shapeCount);
void draw_shape(Shape**&shapesArray, stringstream& lineStream, string& command, string& commandTerm, int& shapeCount, string sensitivityList[]);
void delete_shape(Shape**& shapesArray, stringstream& lineStream, string& command, string& commandTerm, int& shapeCount, string sensitivityList[]);
bool new_check_name_exists_for_draw(Shape**& shapesArray, string& commandTerm, int& shapeCount);


int main() {

    string line;
    string command;
    int commandValue;
    string commandTerm;
    string commandList[6] = {"maxShapes", "create", "move", "rotate", "draw", "delete"};
    string sensitivityList[11] = {"all", "maxShapes", "create", "move", "rotate", "draw", "delete", "circle", "ellipse", "rectangle","triangle"};

    
    cout << "> ";         // Prompt for input
    getline(cin, line);    // Get a line from standard input

    while ( !cin.eof () ) {
        // Put the line in a linestream for parsing
        // Making a new sstream for each line so flags etc. are cleared
        stringstream lineStream (line);
        
        // Read from string stream into the command
        // The only way this can fail is if the eof is encountered
        lineStream >> command;

        // Check for the command and act accordingly
        // ECE244 Student: Insert your code here
        /* 1. Check if the command entered is valid or not, if it is true, return a true boolean value to commandVal, else false.*/
        
        if(check_val_command(command, commandList)){
            /* 2. Check if maxShapes command is given, we assume valid input for this command, thus no
            need to do error handling, however, if maxShapes command have been given before (the shapesArray is not nullptr),
            then we need to delete all objects and the array on heap, and then reallocate.*/
            //cout << check_val_command(command, commandList) << endl;
            shapesArray = set_shape_array(shapesArray, lineStream, command, max_shapes, shapeCount, commandValue);
            //cout << "The max shape is " << max_shapes << endl;

            /* 3. Creates a function that deals with the "create" command. The create command creates a shape in the database
            in the dynamically allocated array of shapes we declared above, and then we modify the values using the set value functions
            definded in Shape.cpp, and we do error handling accordingly: Possible errors: invalid argument, invalid shape name, shape name
            exists, invalid shape type, invalid value, too many arguments, too few arguments, shape array is full.*/
            create_shape(shapesArray, lineStream, command, max_shapes, shapeCount, commandValue, commandTerm, sensitivityList, shapeTypesList);

            /* 4. Move the Shape to {loc1, loc2}, which needs the implementation of a function that modifies the shapes array,
            specifically, we need to find the shape in the shapes array that matches the name of the name given in the arguments of this
            function, if not find, we need to return a specific error, which is "shape name not found", then we reprompt, if found,
            then we go to set the locations, under valid conditions, to loc1 and loc2.*/
            move_shape(shapesArray, lineStream, command, commandValue, commandTerm, sensitivityList);

            /* 5. Rotate the angle of the Shape to {angle} degrees, this function takes in similar ideology in terms of constructing
            the function as move_shape(), we need to check if the name is valid, and act accodingly, if it is valid, then we need
            to construct an extra "check_angle" function that checks if the angle is between 0-360 degrees, if is, set to rotation,
            lastly we need to check if there are too many arguments as before.*/
            rotate_shape(shapesArray, lineStream, command, commandValue, commandTerm, sensitivityList);

            /* 6. Draw a specific shape, by printing out the information about the shape given a specific name, if the name does not exist,
            we return an error, else, if the shape with {name} is ever created, we have already ensured the validity of the parameters,
            thus just printing them out is good, if commandTerm is all, iterate through the string and draw all shapes;*/
            draw_shape(shapesArray, lineStream, command, commandTerm, shapeCount, sensitivityList);

            /* 7. Delete a specific shape, by given the name of the shape, or by given the command "all". By deleting, we need to free the
            pointer to the shape object, and then set it to null, in the case of delete all, we need to free all the pointers up to shapeCount,
            given that they are not already a nullptr, we free them and set them to null, because we can not free a nullptr */
            delete_shape(shapesArray, lineStream, command, commandTerm, shapeCount, sensitivityList);

        } else {
            cout << "Error: invalid command" << endl;
        }
        
        
        // Once the command has been processed, prompt for the
        // next command
        cout << "> ";          // Prompt for input
        getline(cin, line);
        command = "";
    }  // End input loop until EOF.
    
    return 0;
}

/*int& check_max_shapes(int& max_shapes, stringstream& lineStream, string& command, int& commandValue){
    if (command == "maxShapes"){
        lineStream >> commandValue;
        max_shapes = commandValue;
        return max_shapes;
    } else {
        return max_shapes;
    }
}*/

bool check_val_command(string& command, string commandList[]){
    if(command == ""){
        return false;
    }
    for(int i = 0; i <= 6; i++){
        if(command == commandList[i]){
            return true;
        }
    }
    return false;
}

Shape**& set_shape_array(Shape**& shapesArray, stringstream& lineStream, string& command, int& max_shapes, int& shapeCount, int& commandValue){
    if(command == "maxShapes"){
        if(shapesArray == nullptr){
            lineStream >> commandValue;
            max_shapes = commandValue;
            shapesArray = new Shape*[max_shapes];
            for(int a = 0; a < max_shapes; a++){
                shapesArray[a] = nullptr; //made change
            }
            cout << "New database: max shapes is " << max_shapes << endl;
            return shapesArray;
        } else {
            for(int i = 0; i < max_shapes; i++){
                if(shapesArray[i] != nullptr){
                    delete shapesArray[i];
                    shapesArray[i] == NULL;
                } else {
                    continue;
                }
            }
            delete [] shapesArray;
            shapesArray = NULL;
            lineStream >> commandValue;
            max_shapes = commandValue;
            shapesArray = new Shape*[max_shapes];
            for(int a = 0; a < max_shapes; a++){
                shapesArray[a] = nullptr;
            }
            shapeCount = 0;
            cout << "New database: max shapes is " << max_shapes << endl;
            return shapesArray;
        }
    } else {
        return shapesArray;
    }
}

void create_shape(Shape**& shapesArray, stringstream& lineStream, string& command, int& max_shapes, int& shapeCount, int& commandValue, string& commandTerm, string sensitivityList[], string shapeTypesList[]){
    if(command == "create"){
        //create the shape, delete later if requirements not met;
        shapesArray[shapeCount] = new Shape("", "", 0, 0, 0, 0);
        //Checks if array is full, if it is, return error statement:
            //Sets name
            if(check_val_arg_str(lineStream, commandTerm)){ //checks the validity of name
                if(check_shape_name(commandTerm, sensitivityList)){ //checks the validity of shape name
                    if(check_name_exists(shapesArray, commandTerm, shapeCount)){
                        shapesArray[shapeCount]->setName(commandTerm);
                        //cout << "The name of the shape is: " << shapesArray[shapeCount]->getName() << endl;
                    } else{
                        del_reset(shapesArray, shapeCount);
                        return;
                    }
                } else {
                    del_reset(shapesArray, shapeCount);
                    return;
                }
            } else {
                del_reset(shapesArray, shapeCount);
                return;
            }
            // Sets type:
            if(check_val_arg_str(lineStream, commandTerm)){
                if(check_shape_type(commandTerm, shapeTypesList)){
                    shapesArray[shapeCount]->setType(commandTerm);
                    //cout << "The type of the shape is: " << shapesArray[shapeCount]->getType() << endl;
                } else{
                    del_reset(shapesArray, shapeCount);
                    return;
                }
            } else {
                del_reset(shapesArray, shapeCount);
                return;
            }
            // Sets X-loc:
            if(check_val_arg_int(lineStream, commandValue)){
                if(check_loc_sz_value(commandValue)){
                    shapesArray[shapeCount]->setXlocation(commandValue);
                    //cout << "The x-location of the shape is: " << shapesArray[shapeCount]->getXlocation() << endl;
                } else {
                    del_reset(shapesArray, shapeCount);
                    return;
                }
            } else {
                del_reset(shapesArray, shapeCount);
                return;
            }
            // Sets Y-loc:
            if(check_val_arg_int(lineStream, commandValue)){
                if(check_loc_sz_value(commandValue)){
                    shapesArray[shapeCount]->setYlocation(commandValue);
                    //cout << "The y-location of the shape is: " << shapesArray[shapeCount]->getYlocation() << endl;
                } else {
                    del_reset(shapesArray, shapeCount);
                    return;
                }
            } else {
                del_reset(shapesArray, shapeCount);
                return;
            }
            // Sets X-size:
            if(check_val_arg_int(lineStream, commandValue)){
                if(check_loc_sz_value(commandValue)){
                    shapesArray[shapeCount]->setXsize(commandValue);
                    //cout << "The x-size of the shape is: " << shapesArray[shapeCount]->getXsize() << endl;
                } else {
                    del_reset(shapesArray, shapeCount);
                    return;
                }
            } else {
                del_reset(shapesArray, shapeCount);
                return;
            }

            // Sets Y-size:
            if(check_val_arg_int(lineStream, commandValue)){
                if(check_loc_sz_value(commandValue)){
                    shapesArray[shapeCount]->setYsize(commandValue);
                    //cout << "The y-size of the shape is: " << shapesArray[shapeCount]->getYsize() << endl;
                } else {
                    del_reset(shapesArray, shapeCount);
                    return;
                }
            } else {
                del_reset(shapesArray, shapeCount);
                return;
            }
            //Check circle size conditions and act accordingly:
            if(shapesArray[shapeCount]->getType() == "circle"){
                if(check_circle_size(shapesArray, shapeCount)){
                    del_reset(shapesArray, shapeCount);
                    return;
                }
            }
            //Check if the array has more argument than it should:
            if(check_more_arg(lineStream, commandTerm)){
                  if(shapeCount >= max_shapes){
                    cout << "Error: shape array is full" << endl;
                    return;} 
                    else {
                        cout << "Created " << shapesArray[shapeCount]->getName() << ": " << shapesArray[shapeCount]->getType() << " " << shapesArray[shapeCount]->getXlocation() << " "
                        << shapesArray[shapeCount]->getYlocation() << " " << shapesArray[shapeCount]->getXsize() << " " << shapesArray[shapeCount]->getYsize() << endl;
                        shapeCount++;
                        return;
            }
            } else {
                del_reset(shapesArray, shapeCount);
                return;
            }

            } else {
                return;
            }
    }

bool check_val_arg_int(stringstream& lineStream, int& commandValue){
    lineStream >> commandValue;
    if(lineStream.fail()){
        if(lineStream.eof()){
            cout << "Error: too few arguments" << endl;
            return false;
        } else {
            cout << "Error: invalid argument" << endl;
            return false;
        }
    } else {
        if(lineStream.peek() == '.'){
            cout << "Error: invalid argument" << endl;
            return false;
        } else {
            return true;
        }
    }
}

bool check_val_arg_str(stringstream& lineStream, string& commandTerm){
    lineStream >> commandTerm;
    if(lineStream.fail()){
        if(lineStream.eof()){
            cout << "Error: too few arguments" << endl;
            return false;
        } else {
            cout << "Error: invalid argument" << endl;
            return false;
        }
    } else {
        return true;
    }
}

bool check_shape_name(string& commandTerm, string sensitivityList[]){
    for(int i = 0; i < 11; i++){
        if(commandTerm == sensitivityList[i]){
            cout << "Error: invalid shape name" << endl;
            return false;
        }
    }
    return true;
}

bool check_name_exists(Shape**& shapesArray, string& commandTerm, int& shapeCount){
    for(int i = 0; i < shapeCount; i++){
        if(shapesArray[i] == nullptr){
            continue;
        } else {
            if(commandTerm == (shapesArray[i])->getName()){
            cout << "Error: shape " << commandTerm << " exists" << endl;
            return false;
        }
        }   
    }
    return true;
}

bool check_shape_type(string& commandTerm, string shapeTypesList[]){
    for(int i = 0; i < 4; i++){
        if(commandTerm == shapeTypesList[i]){
            return true;
        }
    }
    cout << "Error: invalid shape type" << endl;
    return false;
}

void del_reset(Shape**& shapesArray, int index){
    delete shapesArray[index];
    shapesArray[index] = nullptr;
}

bool check_loc_sz_value(int& commandValue){
    //Case 1: Setting values for x-loc and y-loc that is below zero, commandValue
    if(commandValue < 0){
        cout << "Error: invalid value" << endl;
        return false;
    }
    return true;
}

bool check_circle_size(Shape**& shapesArray, int& shapeCount){
    if(shapesArray[shapeCount]->getXsize() != shapesArray[shapeCount]->getYsize()){
        cout << "Error: invalid value" << endl;
        return true;
    }
    return false;
}

bool check_more_arg(stringstream& lineStream, string& commandTerm){
    lineStream >> commandTerm;
    if(lineStream.fail()){
        if(lineStream.eof()){
            return true;
        } else {
            return false;
        }
    } else {
        cout << "Error: too many arguments" << endl;
        return false;
    }
}
int check_find_name(Shape**& shapesArray, string& commandTerm, int& shapeCount){
    for(int i = 0; i < shapeCount; i++){
        if(shapesArray[i] == nullptr){
            continue;
        } else {
            if(commandTerm == shapesArray[i]->getName()){
            return i;
        }
        }
    }
    return -1;
}

void move_shape(Shape**& shapesArray, stringstream& lineStream, string& command, int& commandValue, string& commandTerm, string sensitivityList[]){
    if(command == "move"){
        //Check retrived name term:
        if(check_val_arg_str(lineStream, commandTerm)){
            if(check_shape_name(commandTerm, sensitivityList)){
                int index = check_find_name(shapesArray, commandTerm, shapeCount);
                if(index >= 0){
                    if(check_val_arg_int(lineStream, commandValue)){
                        if(check_loc_sz_value(commandValue)){
                            shapesArray[index]->setXlocation(commandValue);
                        } else {
                            return;
                        }  
                    } else {
                        return;
                    }
                    if(check_val_arg_int(lineStream, commandValue)){
                        if(check_loc_sz_value(commandValue)){
                            shapesArray[index]->setYlocation(commandValue);
                        } else {
                            return;
                        }
                    } else {
                        return;
                    }
                    if(check_more_arg(lineStream, commandTerm)){
                        cout << "Moved " << commandTerm << " to " << shapesArray[index]->getXlocation() << " " << shapesArray[index]->getYlocation() << endl;
                        return;
                    } else {
                        return;
                    }
                    
                } else {
                    cout << "Error: shape " << commandTerm << " not found" << endl;
                    return;
                }
            }
            else {
                return;
            } 
        } else {
            return;
        }
    } else {
        return;
    }
}

bool check_rotation_angle(int& commandValue){
    if(commandValue < 0 || commandValue > 360){
        cout << "Error: invalid value" << endl;
        return false;
    } else {
        return true;
    }
}

void rotate_shape(Shape**& shapesArray, stringstream& lineStream, string& command, int& commandValue, string& commandTerm, string sensitivityList[]){
    if(command == "rotate"){
        //check retrived name term:
        if(check_val_arg_str(lineStream, commandTerm)){
            if(check_shape_name(commandTerm, sensitivityList)){
                int index = check_find_name(shapesArray, commandTerm, shapeCount);
                if(index >= 0){
                    if(check_val_arg_int(lineStream, commandValue)){
                        if(check_rotation_angle(commandValue)){
                            shapesArray[index]->setRotate(commandValue);
                        } else {
                            return;
                        }
                        if(check_more_arg(lineStream, commandTerm)){
                        cout << "Rotated " << commandTerm << " by " << commandValue << " degrees" << endl;
                        return;
                        } else {
                        return; 
                        }
                    } else {
                        return;
                    }
                } else {
                    cout << "Error: shape " << commandTerm << " not found" << endl;
                    return;
                }
            }
            else {
                return;
            }
            
        } else {
            return;
        }
    } else {
        return;
    }
}

void retrieve_a_shape(Shape**& shapesArray, string& commandTerm, int& shapeCount){
    int index = check_find_name(shapesArray, commandTerm, shapeCount);
    if(index >= 0){ 
        //index of the shape object with name {commandTerm} is retrieved
        cout << "Drew " << commandTerm << ": " << shapesArray[index]->getType() << " " << shapesArray[index]->getXlocation() << " "
        << shapesArray[index]->getYlocation() << " " << shapesArray[index]->getXsize() << " " << shapesArray[index]->getYsize() << " " << endl;
        return;
    } else {
        return;
    }
}

void draw_shape(Shape**&shapesArray, stringstream& lineStream, string& command, string& commandTerm, int& shapeCount, string sensitivityList[]){
    if(command == "draw"){
        if(check_val_arg_str(lineStream, commandTerm)){
            if(commandTerm == "all"){
                if(check_more_arg(lineStream, commandTerm)){
                    cout << "Drew all shapes ";
                    for(int i = 0; i < shapeCount; i++){
                        if(shapesArray[i] == nullptr){
                            continue;
                        } else {
                            cout << shapesArray[i]->getName() << ": " << shapesArray[i]->getType() << " " << shapesArray[i]->getXlocation() << " "
                            << shapesArray[i]->getYlocation() << " " << shapesArray[i]->getXsize() << " " << shapesArray[i]->getYsize() << " ";
                        }
                    }
                    cout << endl;
                    return;
                } else {
                    return;
                }
                
            } else {
                if(check_shape_name(commandTerm, sensitivityList)){
                    if(new_check_name_exists_for_draw(shapesArray, commandTerm, shapeCount)){
                    string temp_name = commandTerm;
                    if(check_more_arg(lineStream, commandTerm)){
                        retrieve_a_shape(shapesArray, commandTerm, shapeCount);
                        return;
                    } else {
                        return;
                    }
                    } else {
                        return;
                    }
                }
                else {
                return;
            }   

            }
        } else {
            return;
        }
    } else {
        return;
    }
}

void delete_shape(Shape**& shapesArray, stringstream& lineStream, string& command, string& commandTerm, int& shapeCount, string sensitivityList[]){
    if(command == "delete"){
        if(check_val_arg_str(lineStream, commandTerm)){
            if(commandTerm == "all"){
                if(check_more_arg(lineStream, commandTerm)){
                    for(int i = 0; i < shapeCount; i++){
                        if(shapesArray[i] == nullptr){
                            continue;
                        } else {
                            delete shapesArray[i];
                            shapesArray[i] = nullptr;
                        }
                    }
                    cout << "Deleted: all shapes" << endl;
                    return;
                } else {
                    return;
                }
            } else {
                if(check_shape_name(commandTerm, sensitivityList)){
                    int index = check_find_name(shapesArray, commandTerm, shapeCount);
                    if(index >= 0){ // means we have a value shape object to delete
                            delete shapesArray[index];
                            shapesArray[index] = nullptr;
                            cout << "Deleted shape " << commandTerm << endl;
                            return;
                        } else {
                            cout << "Error: shape " << commandTerm << " not found" << endl;
                            return;
                        }
                    if(check_more_arg(lineStream, commandTerm)){
                        return;
                    } else {
                        return;
                    }  
                } else {
                    return;
                }
            }
        } else {
            return;
        }
    } else {
        return;
    }
}

bool new_check_name_exists_for_draw(Shape**& shapesArray, string& commandTerm, int& shapeCount){
    // we want to return true if the name exists, false otherwise, with an error statement;
    for(int i = 0; i < shapeCount; i++){
        if(shapesArray[i] == nullptr){
            continue;
        } else {
            if(commandTerm == (shapesArray[i])->getName()){
            return true;
        }
        }   
    }
    cout << "Error: shape " << commandTerm << " not found" << endl; 
    return false;
}




