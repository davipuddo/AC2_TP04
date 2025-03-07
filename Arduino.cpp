#define F0 10
#define F1 11
#define F2 12
#define F3 13

#define PC *PCmem
#define W mem[1].p1
#define X mem[2].p1
#define Y mem[3].p1

struct inst{
    byte p1 : 4;
    byte p2 : 4;
    byte p3 : 4;
};

byte* PCmem;

String in;

void setup(){
    pinMode(F0, OUTPUT);
    pinMode(F1, OUTPUT);
    pinMode(F2, OUTPUT);
    pinMode(F3, OUTPUT);

    Serial.begin(9600);

    inst* mem = new inst[100];
    PCmem = (byte*)((void*)&(mem[0]));

    Serial.println(sizeof(inst));
    PC = 230;
    X = 0xF;
    Serial.println(PC);
    Serial.println(X);
}

void loop(){
    while(Serial.available()>0){
        in = Serial.readString();

        Serial.println(in);

    }
}

