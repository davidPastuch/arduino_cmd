#define INPUT_SIZE 64

char input_string[INPUT_SIZE];
int pin_num;
int str_size;
int i;
int result;

void setup() {
  Serial.begin(9600);                         // start serial comms
}

/*
 * Gets a character from serial, echoes it, and returns the value
 */
char getChar() {
  while(!Serial.available());                 // block until input is ready
  char myChar = Serial.read();                // get character
  Serial.print(myChar);                       // echo character back
  return myChar;
}

/*
 * get a string from serial. prints out string as it's being typed. writes to a char buffer passed in.
 * returns length of the new string (including newline).
 */
int getString(char* str_ptr) {
  int input_index = 0;
  str_ptr[input_index] = getChar();           // start string with first character
  while(str_ptr[input_index++] != '\n') {     // loop while we haven't received a newline
    str_ptr[input_index] = getChar();         // append a new character
  }
  str_ptr[input_index] = 0x0;                 // null terminate the string
  return input_index;                         // returns length of string minus the null terminator
}

/*
 * converts an ascii representation of a number to an int. only accepts positive values. 
 */
int ascii2dec(char* str_ptr, int size) {
  int result = 0;
  if(!size) {                                 // error if string is just newline
    return -1;
  }
  for(i = 0; i < size; i++) {                 // do this for each digit
    if(str_ptr[i] < 48 || str_ptr[i] > 57) {  // make sure number is within bounds, error out if not
      return -1;
    }
    result *= 10;                             // multiply current number by 10 to shift
    result += str_ptr[i] - 48;                // add next digit
  }
  return result;
}

/*
 * zero all elements of an array. size passed to this function should be in bytes
 */
//void clearArray(void* myArray, int size) {
//  for(int i; i < size; i++) {
//    ((char *)myArray)[i] = 0;
//  }
//}

void loop() {
  Serial.print("> ");
  getString(input_string);
  if(!strcmp(input_string, "help\n")) {
    Serial.println("Available commands:\ngpio_dir\ngpio_on\ngpio_off\nanalog_read\npwm");
  }
  else if(!strcmp(input_string, "gpio_dir\n")) {
    Serial.print("Pin number (0-13): ");
    str_size = getString(input_string) - 1;
    pin_num = ascii2dec(input_string, str_size);
    if(pin_num < 0 || pin_num > 13) {
      Serial.println("Pin out of range");
      return;
    }
    Serial.print("Direction (in/out/in_pullup): ");
    getString(input_string);
    if(!strcmp(input_string, "in\n")) {
      pinMode(pin_num, INPUT);
    }
    else if(!strcmp(input_string, "in_pullup\n")) {
      pinMode(pin_num, INPUT_PULLUP);
    }
    else if(!strcmp(input_string, "out\n")) {
      pinMode(pin_num, OUTPUT);
    }
    else {
      Serial.println("Invalid");
    }
  }
  else if(!strcmp(input_string, "gpio_on\n")) {
    Serial.print("Pin number (0-13): ");
    str_size = getString(input_string) - 1;
    pin_num = ascii2dec(input_string, str_size);
    if(pin_num < 0 || pin_num > 13) {
      Serial.println("Invalid");
      return;
    }
    digitalWrite(pin_num, HIGH);
  }
  else if(!strcmp(input_string, "gpio_off\n")) {
    Serial.print("Pin number (0-13): ");
    str_size = getString(input_string) - 1;
    pin_num = ascii2dec(input_string, str_size);
    if(pin_num < 0 || pin_num > 13) {
      Serial.println("Pin out of range");
      return;
    }
    digitalWrite(pin_num, LOW);
  }
  else if(!strcmp(input_string, "analog_read\n")) {
    Serial.print("Pin number (A0-A5): ");
    str_size = getString(input_string) - 1;
    pin_num = ascii2dec(input_string, str_size);
    if(strcmp(input_string, "A0\n") && strcmp(input_string, "A1\n") && strcmp(input_string, "A2\n") && strcmp(input_string, "A3\n") && strcmp(input_string, "A4\n") && strcmp(input_string, "A5\n")) {
      Serial.println("Invalid");
      return;
    }
    result = analogRead(pin_num);
    Serial.println(result);
  }else if(!strcmp(input_string, "pwm\n")) {
    Serial.print("Pin number (3, 5-6, 9-11): ");
    str_size = getString(input_string) - 1;
    pin_num = ascii2dec(input_string, str_size);
    if(pin_num < 3 || pin_num > 11 || pin_num == 4 || pin_num == 7 || pin_num == 8) {
      Serial.println("Invalid");
      return;
    }
    Serial.print("Duty cycle: ");
    str_size = getString(input_string) - 1;
    result = ascii2dec(input_string, str_size);
    if(result > 0 || result < 100) {
      Serial.println("Invalid");
      return;
    }
    if(result) {
      result = (int)(255 * ((float)result / 100));
    }
    analogWrite(pin_num, result);
  }
}
