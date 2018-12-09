
//  main.cpp
//  2048
//
//  Created by Ethan Sarif-Kattan on 22/01/2018.
//  Copyright © 2018 Ethan Sarif-Kattan. All rights reserved.
//

#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <ctime>

bool merge_num(std::vector<int>& v, int bi, int ei);
void rotate_anti_clock(std::vector<int>& v);
bool game_over(const std::vector<int>& v);
void print_grid(const std::vector<int>& v);
int rowColToIndex(int row, int col, int sideLength);
void spawnRandomTwo(std::vector<int>& v);

void printVector(const std::vector<int>& v);

int main() {
	srand(time(NULL));
	//get file to vector
	std::vector<int> vector;
	std::string filename;
	std::cout << "plese enter name of file containing the 2048" << std::endl;
	std::cin >> filename;
	
	std::ifstream infile;
	infile.open(filename.c_str());
	
	if(!infile.is_open()){
		std::cout << "file not found, using default start configuration" << std::endl;
		//vector = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2}; //this can be used in cpp11
		for (int i = 0; i < 15; i++){
			vector.push_back(0);
		}
		vector.push_back(2);
		
	}
	
	int tmp;
	
	while(infile >> tmp){
		vector.push_back(tmp);
	}
	
	//std::cout << "original vector\n";
	print_grid(vector);
	
	bool swipeDidChangeGrid = false;
	
	while(!game_over(vector)){//this is the game swiping
		if (swipeDidChangeGrid){
			print_grid(vector);
			
		}
		
		swipeDidChangeGrid = false;
		
		std::string direction;
		std::cin >> direction;
		int sideLength = std::sqrt(vector.size());
		int numberOfTimesRotated = 0;
		
		if (direction == "w" || direction == "a"|| direction == "s" || direction == "d"){
			
			//we need to rotate because merge num is the equivalent of always swiping left.
			if (direction == "w"){
				//swipe up
				rotate_anti_clock(vector);
				numberOfTimesRotated++;
			}
			if (direction == "a"){
				//swipe left
				//do nothing - this is the default behaviour
			}
			if (direction == "s"){
				//swipe down
				rotate_anti_clock(vector);
				rotate_anti_clock(vector);
				rotate_anti_clock(vector);
				numberOfTimesRotated += 3;
			}
			if (direction == "d"){
				//swipe right
				rotate_anti_clock(vector);
				rotate_anti_clock(vector);
				numberOfTimesRotated += 2;
			}
			
			for (int i = 0; i < sideLength; i++){
				if (merge_num(vector, i * sideLength, (i+1) * sideLength)){
					swipeDidChangeGrid = true;
				}
				
			}
			
			//this rotates vector back to original orientation
			for(int i = 4; i > numberOfTimesRotated; i--){
				rotate_anti_clock(vector);
			}
			
			if (swipeDidChangeGrid){
				spawnRandomTwo(vector);
				
			}
			
		}
		
		
		
	}
	print_grid(vector);

	std::cout << "game over" << std::endl;
	return 0;
}

void spawnRandomTwo(std::vector<int>& v){

	std::vector<int> indexesVector;
	
	for (int i = 0; i < v.size(); i++) {
		if (v[i] == 0){
			indexesVector.push_back(i);
		}
	}
	
	int randIndex = indexesVector[rand() % indexesVector.size()];
		v[randIndex] = 2;
	
	
	
	
}

bool game_over(const std::vector<int>& v){
	
	int sideLength = std::sqrt(v.size());
	std::vector<int> vector = v;
	bool gameOver = true;
	
	for ( int j = 0; j < 4; j++){
		
		for (int i = 0; i < sideLength; i++){
			if (merge_num(vector, i * sideLength, (i+1) * sideLength)){
				gameOver = false;
			}
			
		}
		rotate_anti_clock(vector);
	}
	return gameOver;
}

bool merge_num(std::vector<int>& v, int bi, int ei){
	//std::cout << "\n\ninput vector ";
	//printVector(v);
	
	
	std::vector<int> zerolessVector;
	for(int i = bi; i < ei; i++){//create a vector with all the zeroes stripped
		
		if(v[i] != 0){
			zerolessVector.push_back(v[i]);
		}
		
		
	}
	
	//std::cout << "zeroless vector ";
	//printVector(zerolessVector);
	std::vector<int> summedVector;
	bool pairJustFound = false;//this is so we can skip and not add the next index if a pair was just found
	for(int i = 0; i < zerolessVector.size(); i++){ //create a vector with the same numbers summed
		
		if (zerolessVector[i] == zerolessVector[i+1] && !pairJustFound){
			if (i+1 < zerolessVector.size()){//just in case whatever is in the next memory is equal to the last element
				
				summedVector.push_back(zerolessVector[i] + zerolessVector[i + 1]);
				//std::cout << "/pair just found/ ";
				
			}else{
				summedVector.push_back(zerolessVector[i]);
				
			}
			//std::cout << "/*/ ";
			
			pairJustFound = true;
		}else{
			if(!pairJustFound){
				//std::cout << "/pair not found/ ";
				
				summedVector.push_back(zerolessVector[i]);
			}
			
			pairJustFound = false;
			
		}
		
	}
	//std::cout << "summed vector ";
	
	//printVector(summedVector);
	
	std::vector<int> outputVector;
	
	int modifiedElementsAddedCounter = 0;
	for (int i = 0; i < v.size(); i++){//recreate original vector with modifications
		// std::cout << "\ni" << i;
		// std::cout << "\nv[i]" << v[i];
		
		if (i >= bi && i < ei){
			if (modifiedElementsAddedCounter < summedVector.size()){
				outputVector.push_back(summedVector[modifiedElementsAddedCounter]);
				modifiedElementsAddedCounter++;
				
			}else{
				outputVector.push_back(0);
			}
		}else{
			outputVector.push_back(v[i]);
			
		}
		
	}
	//std::cout << "output vector ";
	//printVector(outputVector);
	
	
	if (outputVector == v){
		// std::cout << "returning false";
		
		return false;
	}else{
		// std::cout << "returning true";
		
		v = outputVector;
		return true;
	}
	
	
}

void rotate_anti_clock(std::vector<int>& v){
	int sideLength = std::sqrt(v.size());
	std::vector<int> outputVector;
	
	for(int col = sideLength - 1; col >= 0; col--){
		for(int row = 0; row < sideLength; row++){
			outputVector.push_back(v[rowColToIndex(row, col, sideLength)]);
		}
	}
	//std::cout << "rotated vector\n";
	//printVector(outputVector);
	v = outputVector;
}


void print_grid(const std::vector<int>& v){
	int sideLength = std::sqrt(v.size());
	
	//	int largestNumberOfDigitsInANumber = 0;
	//	for (int i = 0; i < v.size(); i++){
	//	}
	//
	for(int i = 0; i < sideLength; i++){
		for(int j = 0; j < sideLength; j++){
			std::cout << v[rowColToIndex(i,j,sideLength)] << "\t";
		}
		std::cout << std::endl;
		
	}
	std::cout << std::endl;
	
}

void printVector(const std::vector<int>& v){
	for(int i = 0; i < v.size(); i++){
		std::cout << v[i] << ", ";
	}
	std::cout << std::endl;
}

int rowColToIndex(int row, int col, int sideLength){
	return row*sideLength+col;
}



























