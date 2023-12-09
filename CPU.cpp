#include "CPU.h"

instruction::instruction(bitset<32> fetch)
{
	//cout << fetch << endl;
	instr = fetch;
	//cout << instr << endl;
	



}

CPU::CPU()
{
	PC = 0; //set PC to 0
	for (int i = 0; i < 4096; i++) //copy instrMEM
	{
		dmemory[i] = (0);
	}

	for (int i = 0; i < 33; i++)
	{
		registers[i] = 0;
	}
}

bitset<32> CPU::Fetch(bitset<8> *instmem) {
	bitset<32> instr = ((((instmem[PC + 3].to_ulong()) << 24)) + ((instmem[PC + 2].to_ulong()) << 16) + ((instmem[PC + 1].to_ulong()) << 8) + (instmem[PC + 0].to_ulong()));  //get 32 bit instruction
	PC += 4;//increment PC
	return instr;
}


bool CPU::Decode(instruction* curr)
{
//cout<<curr->instr<<endl;
bitset<7> opcode = curr->instr.to_ulong();
bitset<5> rd_imm = (curr->instr >> 7).to_ulong();
bitset<5> sw_1 = (curr->instr >> 7).to_ulong() ;
bitset<7> sw_2 = (curr->instr >> 25).to_ulong();

string sw_11 = sw_1.to_string();
string sw_22 = sw_1.to_string();
string st = (sw_11 + sw_22);

int sw_off = stoi(st, 0, 2);


bitset<12> sw_offset = sw_off;

bitset<3> funct3 = (curr->instr >> 12).to_ulong();
bitset<5> rs1 = (curr->instr >> 15).to_ulong();
bitset<12> i_type_imm = (curr->instr >> 20).to_ulong();
bitset<5> rs2 = (curr->instr >> 20).to_ulong();
bitset<7> funct7 = (curr->instr >> 25).to_ulong();

long op_long = opcode.to_ulong();
long funct3_long = funct3.to_ulong();
CPU::Instr instruction_name;
//cout << "op_long:" << op_long << endl;
if (op_long == 19)
{
	//cout << "I-Type" << endl;
	if (funct3_long == 0)
	{
		instruction_name = CPU::Instr::ADDI;
		//cout << "ADDI" << endl;
		CPU::Regfile(instruction_name, rs1, curr);
	}
	else if (funct3_long == 7)
	{
		instruction_name = CPU::Instr::ANDI;
		//cout << "ANDI" << endl;
		CPU::Regfile(instruction_name, rs1, curr);
	}


}
else if (op_long == 51)
{
	//cout << "R-Type" << endl;
	rtype += 1;
	if (funct3_long == 0)
	{
		long funct7_long = funct7.to_ulong();

		if (funct7_long == 32)
		{
			instruction_name = CPU::Instr::SUB;
			//cout << "SUB" << endl;
			CPU::Regfile(instruction_name, rs1, curr, rs2);
		}
		else
		{
			instruction_name = CPU::Instr::ADD;
			//cout << "ADD" << endl;
			CPU::Regfile(instruction_name, rs1, curr, rs2);
		}
	}
	else if (funct3_long == 4)
	{
		instruction_name = CPU::Instr::XOR;
		//cout << "XOR" << endl;
		CPU::Regfile(instruction_name, rs1, curr, rs2);
	}
	else if (funct3_long == 5)
	{
		instruction_name = CPU::Instr::SRA;
		//cout << "SRA" << endl;
		CPU::Regfile(instruction_name, rs1, curr, rs2);
	}
}
else if (op_long == 35)
{
	//cout << "SW" << endl;
	instruction_name = CPU::Instr::SW;
	CPU::Regfile(instruction_name, rs1, curr, rs2);
	 

}
else if (op_long == 3)
{
	//cout << "LW" << endl;
	instruction_name = CPU::Instr::LW;
	CPU::Regfile(instruction_name, rs1, curr, rs2);
}

else if (op_long == 103)
{
	//cout << "JALR" << endl;
	instruction_name = CPU::Instr::JALR;
	CPU::Regfile(instruction_name, rs1, curr, rs2);
}
else if (op_long == 99)
{
	//cout << "BLT" << endl;
	instruction_name = CPU::Instr::BLT;
	CPU::Regfile(instruction_name, rs1, curr, rs2);
}
return true;
}

unsigned long CPU::readPC()
{
	return PC;
}

bool CPU::Regfile(CPU::Instr instrName, bitset<5> rs1, instruction* curr, bitset<5> rs2)
{
	long rs1_long = rs1.to_ulong();
	long rs2_long = rs2.to_ulong();
	long val = 0;
	bitset<12> i_type_imm = (curr->instr >> 20).to_ulong();
	bitset<12> extended_rs1 = (rs1).to_ulong();
	if (instrName == CPU::Instr::ADDI)
	{
		short SX = (short)(curr->instr >> 20).to_ulong() - (((curr->instr >> 20).to_ulong() & 0x800) << 1);
		//cout << "Val: " << SX << endl;
		val = registers[rs1_long] + SX;
	}
	else if (instrName == CPU::Instr::ANDI)
	{
		//cout << "Here" << endl;
		//cout << extended_rs1 << " " << i_type_imm << endl;
		bitset<12> extended_rs1 = registers[rs1.to_ulong()];
		val = (extended_rs1 & (i_type_imm)).to_ulong();
	}
	else if (instrName == CPU::Instr::ADD)
	{
		val = registers[rs1_long] + registers[rs2_long];
	}
	else if (instrName == CPU::Instr::XOR)
	{
		bitset<32> r1 = registers[rs1.to_ulong()];
		bitset<32> r2 = registers[rs2.to_ulong()];
		val = (r1 ^ r2).to_ullong();
	}
	else if (instrName == CPU::Instr::SUB)
	{
		val = registers[rs1_long] - registers[rs2_long];
	}
	else if (instrName == CPU::Instr::SW)
	{
		val = registers[rs2_long];


		bitset<12> rs1_ext = rs1.to_ulong();
		//cout << "Storing: " << val << endl;
		

	}
	else if (instrName == CPU::Instr::LW)
	{
		//bitset<12> rs1_ext = rs1.to_ulong();
		//cout << "Loading from: " << i_type_imm.to_ulong() + registers[rs1_long] << endl;
		//cout << "Loading value: " << dmemory[i_type_imm.to_ulong() + registers[rs1_long]] << endl;//<< " at " << (curr->instr >> 7).to_ulong() << endl;
		val = dmemory[i_type_imm.to_ulong() + registers[rs1_long]];
	}

	else if (instrName == CPU::Instr::JALR)
	{
		val = PC;
	}
	else if (instrName == CPU::Instr::BLT)
	{
		bitset<4> br_1 = (curr->instr >> 8).to_ulong();
		bitset<6> br_2 = (curr->instr >> 25).to_ulong();
		bitset<1> br_3 = (curr->instr >> 7).to_ulong();
		bitset<1> br_4 = (curr->instr >> 30).to_ulong();

		string br_11 = br_1.to_string();
		string br_22 = br_2.to_string();
		string br_33 = br_3.to_string();
		string br_44 = br_4.to_string();

		string br_fin = br_44 + br_33 + br_22 + br_11;
		//cout << "Imm binary: " << br_fin << endl;
		int imm = stoi(br_fin, 0, 2);
		

		//cout << imm << endl;

		if (registers[rs1_long] < registers[rs2_long])
		{
			//cout << "Taking jump: " << endl;
			(PC) += imm;
		}
		else 
		{
			//cout << "No jump" << endl;
		}


	}
	else
	{
		return false;
	}
	//if(instrName != CPU::Instr::BLT)
		WriteBack(instrName, val, curr);
	return true;
}

bool CPU::WriteBack(CPU::Instr instrName, long val, instruction* curr)
{

	if (instrName == CPU::Instr::SW)
	{
		bitset<5> sw_1 = (curr->instr >> 7).to_ulong();
		bitset<7> sw_2 = (curr->instr >> 25).to_ulong();
		bitset<5> rs1 = (curr->instr >> 15).to_ulong();

		string sw_11 = sw_1.to_string();
		string sw_22 = sw_2.to_string();
		string st = (sw_22 + sw_11);

		//cout << "st " << st << endl;
		//cout << "reg 1: " << rs1.to_ulong() << endl;
		int sw_off = stoi(st, 0, 2);
		int regval = registers[rs1.to_ulong()];
		//cout << "regval: " << regval << endl;
		dmemory[sw_off +  regval] = val;
		//cout << "Storing at: " << sw_off + regval << endl; //<< val << " at " << (curr->instr >> 20).to_ulong() + (curr->instr >> 15).to_ulong() << endl;
	}
	else if (instrName == CPU::Instr::BLT)
	{
		
	}
	else if (instrName == CPU::Instr::JALR)
	{
		bitset<5> rs1 = (curr->instr >> 15).to_ulong();
		bitset<5> rd_imm = (curr->instr >> 7).to_ulong();
		bitset<12> i_type_imm = (curr->instr >> 20).to_ulong();
		int dest = rd_imm.to_ulong();

		registers[dest] = val;

		PC = i_type_imm.to_ulong() + registers[rs1.to_ulong()];

	}
	else
	{
		bitset<5> rd_imm = (curr->instr >> 7).to_ulong();

		int dest = rd_imm.to_ulong();

		//cout << "RD: " << dest << endl;
		//cout << "val: " << dest << endl;
		registers[dest] = val;
		//for (int i = 0; i < 33; i++)
		//{
		//	cout << "Register " << i << ": " << registers[i] << endl;
		//}

		//cout << "x10: " << registers[10] << endl;
		//cout << "x11: " << registers[11] << endl;
		//cycles += 1;
	}
	return true;



}

void CPU::Finish()
{
	cout << '(' << registers[10] << ',' << registers[11] << ')' << endl;
}

