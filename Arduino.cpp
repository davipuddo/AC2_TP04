#define led0 10
#define led1 11
#define led2 12
#define led3 13

#define PC *PCmem
#define W mem[1]->p1
#define X mem[2]->p1
#define Y mem[3]->p1

struct inst{
    byte p3 : 4;
    byte p2 : 4;
    byte p1 : 4;

    inst(){
        p1 = 0xB;
        p2 = 0xC;
        p3 = 0xB;
    }
  
  	inst(char ca, char cb, char cc){
      	byte cca, ccb, ccc;
    	if(ca>='A' && ca<='F') cca = ca - 55; // from char to hexa
      	else cca = ca - 48;
      	if(cb>='A' && cb<='F') ccb = cb - 55; // from char to hexa
      	else ccb = cb - 48;
      	if(cc>='A' && cc<='F') ccc = cc - 55; // from char to hexa
      	else ccc = cc - 48;
		p1 = cca & 0x0F;
		p2 = ccb & 0x0F;
		p3 = ccc & 0x0F;
    }
};

static bool step = false;
static int waitSecs = 4;

byte* PCmem;
inst** mem;
inst* ins;

int progSize = 0;

int i = 0;
String in;

void setup(){
    pinMode(led0, OUTPUT);
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);

    Serial.begin(9600);

    mem = new inst*[100];
  	for(int u=0; u<100; u++){
    	mem[u] = new inst('0', '0', '0');
    }
    PCmem = (byte*)((void*)&(mem[0]));
  
  	/*
  	PC = 0x04;
  	PC = PC + 0x01;
  	dumpReg();
    Serial.println(PC);
    */
	Serial.println("Insira as instrucoes para a carga do vetor:");
}

void execInst(){
    X = ins->p1;
    Y = ins->p2;

    if((*ins).p3==0x0) W = 0x0;
    else if((*ins).p3==0x1) W = 0x1;
    else if((*ins).p3==0x2) W = X;
    else if((*ins).p3==0x3) W = Y;
    else if((*ins).p3==0x4) W = ((byte)~X)&0x0F;
    else if((*ins).p3==0x5) W = ((byte)~Y)&0x0F;
    else if((*ins).p3==0x6) W = X & ((byte)~Y)&0x0F;
    else if((*ins).p3==0x7) W = Y & ((byte)~X)&0x0F;
    else if((*ins).p3==0x8) W = X ^ Y;
    else if((*ins).p3==0x9) W = (((byte)~X)&0x0F) ^ (((byte)~X)&0x0F);
    else if((*ins).p3==0xA) W = ~((((byte)~X)&0x0F) ^ (((byte)~X)&0x0F)) & 0x0F;
    else if((*ins).p3==0xB) W = X & Y;
    else if((*ins).p3==0xC) W = ~(X & Y) & 0x0F;
    else if((*ins).p3==0xD) W = ~(X | Y) & 0x0F;
    else if((*ins).p3==0xE) W = X | Y;
    else if((*ins).p3==0xF) W = X & X;
    else{
        Serial.println("Instrucao desconhecida");
        W = 0x0;
    }
}

void dumpReg(){
  	for(int u=0; u<4; u++){
    	if(mem[u]->p1 < 10) Serial.print(mem[u]->p1);
      	else Serial.print((char)(mem[u]->p1 + 55));
      	if(mem[u]->p2 < 10) Serial.print(mem[u]->p2);
      	else Serial.print((char)(mem[u]->p2 + 55));
      	if(mem[u]->p3 < 10) Serial.print(mem[u]->p3);
      	else Serial.print((char)(mem[u]->p3 + 55));
      	Serial.print(" | ");
    }
  Serial.println("");
}

void dumpMem(){
  	for(int u=0; u<100; u++){
    	if(mem[u]->p1 < 10) Serial.print(mem[u]->p1);
      	else Serial.print((char)(mem[u]->p1 + 55));
      	if(mem[u]->p2 < 10) Serial.print(mem[u]->p2);
      	else Serial.print((char)(mem[u]->p2 + 55));
      	if(mem[u]->p3 < 10) Serial.print(mem[u]->p3);
      	else Serial.print((char)(mem[u]->p3 + 55));
      	Serial.print(" | ");
    }
  Serial.println("");
}

void execProgram(){
    PC = 0x04;
    while(PC<=progSize){
      	Serial.print("PC(");
        Serial.print(PC);
      	Serial.print(") ");
        ins = mem[PC];
        //ins = new inst();
        execInst();
        digitalWrite(led0, LOW);
        digitalWrite(led1, LOW);
        digitalWrite(led2, LOW);
        digitalWrite(led3, LOW);
        if((W&0b1000)==0b1000) digitalWrite(led0, HIGH);
        if((W&0b0100)==0b0100) digitalWrite(led1, HIGH);
        if((W&0b0010)==0b0010) digitalWrite(led2, HIGH);
        if((W&0b0001)==0b0001) digitalWrite(led3, HIGH);
        PC = PC + 0x01;
      	if(step){
          	while(Serial.available()==0){}
			char x = Serial.read();
        }
      	dumpMem();
      	delay(waitSecs * 1000);
    }
}

void loadMem(){
	int reps = in.length(),
  		progPos = 4;
  	char a = '0',
  		 b = '0',
  		 c = '0';
	for(int j=0; j<reps; j++){
		if(j%4==0) a = in.charAt(j);
      	else if(j%4==1) b = in.charAt(j);
        else if(j%4==2) c = in.charAt(j);
        else mem[progPos++] = new inst(a, b, c);
    }
  	progSize = progPos - 1;
}

void loop(){
    while(Serial.available()>0){
        in = Serial.readStringUntil('\0');
      
      	//Serial.println(in.length());
      	
      	loadMem();
      	//Serial.println(progSize);
      	//dumpMem();
      	execProgram();

        //Serial.print("--");
        //Serial.print(in);
        //Serial.println("--");
      
        Serial.println("Insira as instrucoes para a carga do vetor:");
    }
}
