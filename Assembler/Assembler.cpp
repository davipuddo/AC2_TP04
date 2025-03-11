#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "File.h"
#include "iostream"
#include "cmath"
#include <cstdint>

#define byte uint8_t
#define X mem[0]
#define Y mem[1]
#define W mem[2]

class Assembler
{
	private:
	List* input;
	List* output;
	byte* mem;

	public:

	Assembler (const char* filename)
	{
		input = NULL;
		output = NULL;

		mem = new byte[3];

		for (int i = 0; i < 3; i++)
		{
			mem[i] = 0x0;
		}

		if (filename != "")
		{
			File* f = new File(filename);
			input = f->read();
		}
	}

	void Export (const char* filename)
	{
		if (filename != "")
		{
			File* f = new File(filename);
			f->write(output);
		}
	}

	List* getInput (void)
	{
		return (input->clone());
	}

	List* getOutput (void)
	{
		return (output->clone());
	}

	void printMem (void)
	{
		for (int i = 0; i < 3; i++)
		{
			printf ("[%u] ", mem[i]);
		}
		std::cout << "\n";
	}

	bool isNumber (char c)
	{
		return (c >= '0' && c <= '9');
	}

	bool isLetter (char c)
	{
		return ( (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') );
	}

	bool match (char* x, const char* y)
	{
		return (strcmp(x, y) == 0);
	}

	byte parseByte (char* str, int p, int n)
	{
		byte res = 0x0;
		if (str)
		{
			int e = n-p;

			for (int i = p; i < n; i++)
			{
				res += (byte)(str[i] -48 ) * pow(10, n-i-1);
			}
		}
		return (res);
	}

	byte getInstruction (char* str, int p, int n)
	{
		byte res = 0x0;
		if (str)
		{
			char* min = (char*)calloc(n-p+1, sizeof(char));
			
			for (int i = p; i < n; i++)
			{
				min[i-p] = str[i];
			}
			if (match(min, "zeroL")) res = 0x0;
			else if (match(min, "umL")) res = 0x1;
			else if (match(min, "copiaA")) res = 0x2;
			else if (match(min, "copiaB")) res = 0x3;
			else if (match(min, "nA")) res = 0x4;
			else if (match(min, "nB")) res = 0x5;
			else if (match(min, "AenB")) res = 0x6;
			else if (match(min, "nAeB")) res = 0x7;
			else if (match(min, "AxB")) res = 0x8;
			else if (match(min, "nAxnB")) res = 0x9;
			else if (match(min, "nAxnBn")) res = 0xA;
			else if (match(min, "AeB")) res = 0xB;
			else if (match(min, "AeBn")) res = 0xC;
			else if (match(min, "AoBn")) res = 0xD;
			else if (match(min, "AoB")) res = 0xE;
			else if (match(min, "nAonBn")) res = 0xF;
		}
		return (res);
	}

	void assemble (char* line)
	{
		bool op = false;

		if (line)
		{
			op = false;
			int n = strlen(line);

			byte* cursor = NULL;

			if (line[0] == 'X' || line[0] == 'x')
			{
				cursor = &X;
			}
			else if (line[0] == 'Y' || line[0] == 'y')
			{
				cursor = &Y;
			}
			else if (line[0] == 'W' || line[0] == 'w')
			{
				cursor = &W;
				op = true;
			}

			int s = 0;
			int f = 0;

			int i = 1;

			while (line[i] != '=')
			{
				i++;
			}

			if (!op)
			{
				while (!isNumber(line[i]))
				{
					i++;
				}
				s = i;
	
				while (isNumber(line[i]))
				{
					i++;
				}
				f = i;

				*cursor = parseByte(line, s, f);
			}
			else
			{
				while (!isLetter(line[i]))
				{
					i++;
				}
				s = i;

				while (isLetter(line[i]))
				{
					i++;
				}
				f = i;

				*cursor = getInstruction (line, s, f);
			}
		}
	}

	void assemble (void)
	{
		int n = input->getSize() - 1;

		output = new List();

		for (int i = 1; i < n; i++)
		{
			byte wg = W;

			char* line_in = input->get(i);

			assemble(line_in);

			if (wg != W)
			{
				char* line_out = new char[8];
				snprintf (line_out, 8, "%1X%1X%1X", X,Y,W);
				output->insert(line_out);
			}

			delete(line_in);
		}
	}

};

#endif

int main (int argc, char** argv)
{
	if (argc != 2)
	{
		printf("argc:%d\n",argc);
		std::cerr << "ERRO: A quantidade de parametros e invalida!\n"; 
	}
	else
	{
		char* infile = argv[1];
		int n = strlen(infile);
		char* outfile = new char[n+3];

		int i = 0;
		while (i < n && infile[i] != '.')
		{
			outfile[i] = infile[i];
			i++;
		}
		strcat(outfile, ".hex");
		
		Assembler* as = new Assembler(infile);
		as->assemble();
		as->Export(outfile);
	}
	return (0);
}
