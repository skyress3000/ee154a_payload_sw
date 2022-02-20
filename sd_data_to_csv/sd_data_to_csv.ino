#include <SPI.h>
#include <SD.h>
#include <stdint.h>

File root;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  Serial.println("Initializing SD card...");

  if (!SD.begin(8)) {
    Serial.println("initialization failed!");
    while (1);
  }

  Serial.println("All SD card files:");

  root = SD.open("/");

  printDirectory(root, 0);

  root.close();
}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.println("Choose a file to decode to CSV:");
  while(!Serial.available()) {};
  //char filename[128];
  String filename = Serial.readString();

  // open specified file
  File logfile = SD.open(filename, FILE_READ);
  if(logfile) {
    // add .csv for the decoded file
    String new_filename = filename + ".csv";
    File csvfile = SD.open(new_filename, FILE_WRITE);

    char data_buf[8];
    while(logfile.available()) {
      logfile.read(data_buf, 8);
      // first 4 bytes are 32 bit timestamp
      uint32_t timestamp = data_buf[0] + (data_buf[1] << 8) + (data_buf[2] << 16) + (data_buf[3] << 24);
      // second 4 bytes are float data
      union {
        uint32_t data_bits;
        float data_val;
      } data;
      data.data_bits = data_buf[4] + (data_buf[5] << 8) + (data_buf[6] << 16) + (data_buf[7] << 24);

      // output decoded data point to both serial and csv
      //Serial.print("timestamp: ");
      //Serial.print(timestamp);
      //Serial.print(" data: ");
      //Serial.println(data.data_val);

      csvfile.print(timestamp);
      csvfile.print(",");
      csvfile.println(data.data_val);
    }

    csvfile.close();
    logfile.close();
  } else {
    Serial.print("Failed to open ");
    Serial.println(filename);
  }
}

// from example
void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}