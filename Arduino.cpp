#define led0 10
#define led1 11
#define led2 12
#define led3 13

#define PC *PCmem
#define W mem[1].p1
#define X mem[2].p1
#define Y mem[3].p1

struct inst{
    byte p1 : 4;
    byte p2 : 4;
    byte p3 : 4;

    inst(){
        p1 = 0xB;
        p2 = 0xC;
        p3 = 0xB;
    }
};

byte* PCmem;
inst* mem;
inst* ins;

int i = 0;
bool called = false;
String in;

void setup(){
    pinMode(led0, OUTPUT);
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);

    Serial.begin(9600);

    mem = new inst[100];
    PCmem = (byte*)((void*)&(mem[0]));

    mem[4].p1 = 0xB1;
    Serial.println(mem[4].p1, HEX);

    // ____INICIO DO PROGRAMA____

    Serial.println("(*ins)ira as instrucoes para a carga do vetor:");

    // LAURA SUA PARTE AQUI

    //execProgram();
}

void execInst(){
    X = (*ins).p1;
    Y = (*ins).p2;

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
        Serial.println("(*ins)trucao desconhecida");
        W = 0x0;
    }
}

void execProgram(){
    PC = 4;
    /*
     *    while(PC<=100){
     *        //ins = &mem[PC];
     *        (*ins) = new inst();
     *        execInst();
     *        digitalWrite(led0, LOW);
     *        digitalWrite(led1, LOW);
     *        digitalWrite(led2, LOW);
     *        digitalWrite(led3, LOW);
     *        if((W&0b1000)==0b1000) digitalWrite(led0, HIGH);
     *        if((W&0b0100)==0b0100) digitalWrite(led0, HIGH);
     *        if((W&0b0010)==0b0010) digitalWrite(led0, HIGH);
     *        if((W&0b0001)==0b0001) digitalWrite(led0, HIGH);
}
*/

    ins = new inst();
    execInst();
    digitalWrite(led0, LOW);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    if((W&0b1000)==0b1000) digitalWrite(led0, HIGH);
    if((W&0b0100)==0b0100) digitalWrite(led0, HIGH);
    if((W&0b0010)==0b0010) digitalWrite(led0, HIGH);
    if((W&0b0001)==0b0001) digitalWrite(led0, HIGH);

}

void loop(){
    Serial.println("Flag");
    while(i<3 && Serial.available()>0){
        in = Serial.readStringUntil(' ');

        Serial.print("--");
        Serial.print(in);
        Serial.println("--");

        i++;
    }
    if(!called && i>96){
        execProgram();
        called = true;
    }
}

