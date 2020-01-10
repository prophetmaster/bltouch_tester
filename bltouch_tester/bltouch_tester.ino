#include <Servo.h>
#include <SerialCommands.h>

#define ledPin 13
#define outputPin 3
#define inputPin 2

char serial_command_buffer_[32];
SerialCommands serial_commands_(&Serial, serial_command_buffer_, sizeof(serial_command_buffer_), "\r\n", " ");
Servo myservo;  // create servo object to control a servo

void cmd_unrecognized(SerialCommands* sender, const char* cmd)
{
  sender->GetSerial()->print("Unrecognized command [");
  sender->GetSerial()->print(cmd);
  sender->GetSerial()->println("]");
}


void cmd_deploy(SerialCommands* sender)
{
  myservo.write(10);
}


void cmd_M119(SerialCommands* sender)
{
 myservo.write(60);
 Serial.println("Alarm Release & Touch SW Mode(M119)");
}


void cmd_stow(SerialCommands* sender)
{
 myservo.write(90);
 Serial.println("Stow probe");
}


void cmd_alarm(SerialCommands* sender)
{
 myservo.write(160);
 Serial.println("Alarm release");
}


void cmd_test(SerialCommands* sender)
{
 myservo.write(120);
 Serial.println("Self test 10 times");
}

void cmd_led_on(SerialCommands* sender)
{
  sender->GetSerial()->println("Led is on");
}

SerialCommand cmd_deploy_("deploy", cmd_deploy);
SerialCommand cmd_M119_("M119", cmd_M119);
SerialCommand cmd_stow_("stow", cmd_stow);
SerialCommand cmd_alarm_("alarm", cmd_alarm);
SerialCommand cmd_test_("test", cmd_test);


void setup() {
  
  Serial.begin(57600);
  
  // put your setup code here, to run once:
  myservo.attach(outputPin);  // attaches the servo on pin 9 to the servo object
  pinMode(inputPin, INPUT_PULLUP);

  serial_commands_.SetDefaultHandler(cmd_unrecognized);
  
  serial_commands_.AddCommand(&cmd_deploy_);
  serial_commands_.AddCommand(&cmd_M119_);
  serial_commands_.AddCommand(&cmd_stow_);
  serial_commands_.AddCommand(&cmd_alarm_);
  serial_commands_.AddCommand(&cmd_test_);
  
  Serial.println("Ready!");
}

void loop() {
  
  serial_commands_.ReadSerial();
  bool probe = digitalRead(inputPin);
  digitalWrite(ledPin, probe);
  
}
