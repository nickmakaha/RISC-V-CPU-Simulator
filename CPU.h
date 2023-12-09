#include <iostream>
#include <bitset>
#include <stdio.h>
#include<stdlib.h>
#include <string>
#include <unordered_map>
using namespace std;


class instruction {
public:
	bitset<32> instr;//instruction
	instruction(bitset<32> fetch); // constructor



	enum Type
	{
		Rtype,
		Itype,
		Btype
	};

private:
	string instrName;
	



};

class CPU {
private:
	int dmemory[4096]; //data memory byte addressable in little endian fashion;
	unsigned long PC; //pc 
	unordered_map<int, int> registers;
	

public:
	int cycles = 0;
	int rtype = 0;
	enum Instr {
		ADD,
		SUB,
		ADDI,
		XOR,
		ANDI,
		SRA,
		LW,
		SW,
		BLT,
		JALR
	};
	CPU();
	unsigned long readPC();
	bitset<32> Fetch(bitset<8> *instmem);
	bool Decode(instruction* instr);
	bool Regfile(CPU::Instr instrName, bitset<5> rs1, instruction* curr,  bitset<5> rs2 = 00000);
	bool WriteBack(CPU::Instr instrName, long val, instruction* curr);
	void Finish();
	//bool InstMemory(bitset<32> instr);
	//bool Registers(bool Imm, bitset<5> rs1, bitset<5> rs2 = 00000 );


	
};

// add other functions and objects here
