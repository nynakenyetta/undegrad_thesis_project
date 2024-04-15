void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0){
    String data;
    data.concate("S1: ");
    Serial.print("3");
    Serial.print("S2: ");
    Serial.print("22");
  }
}
