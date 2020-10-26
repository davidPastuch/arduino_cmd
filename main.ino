#define INPUT_SIZE 64

char input_string[INPUT_SIZE];
int pin_num;
int str_size;
int i;

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
    Serial.println("help message");
  }
  else if(!strcmp(input_string, "gpio_dir\n")) {
    Serial.print("Pin number:");
    str_size = getString(input_string) - 1;
    pin_num = 0;
  }
  else if(!strcmp(input_string, "gpio_on\n")) {
    Serial.print("Pin number:");
  }
  else if(!strcmp(input_string, "gpio_off\n")) {
    Serial.print("Pin number:");
  }
}
