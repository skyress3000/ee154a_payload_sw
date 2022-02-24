#include <SPI.h>
#include <SD.h>
#include <stdint.h>

File root;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  Serial.println("Initializing SD card...");

  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }

  Serial.println("All SD card files:");

  root = SD.open("/");

  printDirectory(root, 0);

  root.close();
}

void loop() {
  Serial.println("Choose a file to decode to CSV:");
  // while(!Serial.available()) {}; // wait for data to appear on serial bus
  // String filename = Serial.readString(); // Read the filename
  String filename = "22322/16253030/LOG";
  // open specified file
  File logfile = SD.open(filename, FILE_READ);
  if(logfile) {
    Serial.print("Decoding ");
    Serial.println(filename);

    char data_buf[9];
    while(logfile.available()) {
      logfile.read(data_buf, 9);
      // first 1 byte is identifier
      char id = data_buf[0];
      // next 4 bytes are 32 bit timestamp
      uint32_t timestamp = data_buf[1] + (data_buf[2] << 8) + (data_buf[3] << 16) + (data_buf[4] << 24);
      // second 4 bytes are float data
      union {
        uint32_t data_bits;
        float data_val;
      } data;
      data.data_bits = data_buf[5] + (data_buf[6] << 8) + (data_buf[7] << 16) + (data_buf[8] << 24);

      // Open the corresponding CSV file
      String new_filename = String(id) + ".csv";
      File csvfile = SD.open(new_filename, FILE_WRITE);
      csvfile.print(timestamp);
      csvfile.print(",");
      csvfile.println(data.data_val, 10);
      csvfile.close();
    }

    logfile.close();
  } else {
    Serial.print("Failed to open ");
    Serial.println(filename);
  }
  Serial.println("Done");
  while(1);
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