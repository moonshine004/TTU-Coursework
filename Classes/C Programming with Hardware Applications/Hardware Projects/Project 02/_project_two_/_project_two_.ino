// Hardware Project 2 - 7-Segment Display with 4-bit Binary Input

void setup() 
{ 
  // Setup 7-segment display output pins
  pinMode(13, OUTPUT); // segment g
  pinMode(12, OUTPUT); // segment f
  pinMode(11, OUTPUT); // segment e
  pinMode(10, OUTPUT); // segment d
  pinMode(9, OUTPUT);  // segment c
  pinMode(8, OUTPUT);  // segment b
  pinMode(7, OUTPUT);  // segment a

  // Setup binary input pins
  pinMode(5, INPUT); // bit 3
  pinMode(4, INPUT); // bit 2
  pinMode(3, INPUT); // bit 1
  pinMode(2, INPUT); // bit 0
}

void loop() 
{
  // Read the 4-bit binary input and convert to decimal
  int number = (digitalRead(5) << 3) | (digitalRead(4) << 2) | (digitalRead(3) << 1) | digitalRead(2);

  // Display the corresponding number on the 7-segment display
  if (number == 0)  
  { digitalWrite(13, LOW); digitalWrite(12, HIGH); digitalWrite(11, HIGH); digitalWrite(10, HIGH);
    digitalWrite(9, HIGH); digitalWrite(8, HIGH); digitalWrite(7, HIGH); }
  
  else if (number == 1)  
  { digitalWrite(13, LOW); digitalWrite(12, LOW); digitalWrite(11, LOW); digitalWrite(10, LOW);
    digitalWrite(9, HIGH); digitalWrite(8, HIGH); digitalWrite(7, LOW); }
  
  else if (number == 2)  
  { digitalWrite(13, HIGH); digitalWrite(12, LOW); digitalWrite(11, HIGH); digitalWrite(10, HIGH);
    digitalWrite(9, LOW); digitalWrite(8, HIGH); digitalWrite(7, HIGH); }
  
  else if (number == 3)  
  { digitalWrite(13, HIGH); digitalWrite(12, LOW); digitalWrite(11, LOW); digitalWrite(10, HIGH);
    digitalWrite(9, HIGH); digitalWrite(8, HIGH); digitalWrite(7, HIGH); }
  
  else if (number == 4)  
  { digitalWrite(13, HIGH); digitalWrite(12, HIGH); digitalWrite(11, LOW); digitalWrite(10, LOW);
    digitalWrite(9, HIGH); digitalWrite(8, HIGH); digitalWrite(7, LOW); }
  
  else if (number == 5)   
  { digitalWrite(13, HIGH); digitalWrite(12, HIGH); digitalWrite(11, LOW); digitalWrite(10, HIGH);
    digitalWrite(9, HIGH); digitalWrite(8, LOW); digitalWrite(7, HIGH); }
  
  else if (number == 6)  
  { digitalWrite(13, HIGH); digitalWrite(12, HIGH); digitalWrite(11, HIGH); digitalWrite(10, HIGH);
    digitalWrite(9, HIGH); digitalWrite(8, LOW); digitalWrite(7, HIGH); }
  
  else if (number == 7)  
  { digitalWrite(13, LOW); digitalWrite(12, LOW); digitalWrite(11, LOW); digitalWrite(10, LOW);
    digitalWrite(9, HIGH); digitalWrite(8, HIGH); digitalWrite(7, HIGH); }
  
  else if (number == 8)  
  { digitalWrite(13, HIGH); digitalWrite(12, HIGH); digitalWrite(11, HIGH); digitalWrite(10, HIGH);
    digitalWrite(9, HIGH); digitalWrite(8, HIGH); digitalWrite(7, HIGH); }
  
  else if (number == 9)  
  { digitalWrite(13, HIGH); digitalWrite(12, HIGH); digitalWrite(11, LOW); digitalWrite(10, HIGH);
    digitalWrite(9, HIGH); digitalWrite(8, HIGH); digitalWrite(7, HIGH); }
  
  else  
  { digitalWrite(13, LOW); digitalWrite(12, LOW); digitalWrite(11, LOW); digitalWrite(10, LOW);
    digitalWrite(9, LOW); digitalWrite(8, LOW); digitalWrite(7, LOW); }
}
