// Libraries
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
// Model and TFlite
#include "model.h" // Include the converted TFLite model
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
// #include "tensorflow/lite/version.h"

Adafruit_MPU6050 mpu;

const char* activities[] = {
    "Cycling",
    "Jogging",
    "Typing",
    "Upstair",
    "Walking",
    "Writing"
};

// Memory allocation for the model
constexpr int kTensorArenaSize = ; // Adjust based on your model size
uint8_t tensor_arena[kTensorArenaSize];

// TensorFlow Lite model variables
const tflite::Model* model;
tflite::MicroInterpreter* interpreter;
TfLiteTensor* input_tensor;

void setup() {

  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  // Load the TensorFlow Lite model, create the resolver and init the interpreter
  


  Serial.println("Setup complete.");
}

void loop() {

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Collect data for the model: accelerometer and gyroscope
  // Check if the required number of samples has been reached
  // If yes then invoke the interpreter
  // Else get more readings to fill a buffer. (Recall from task 1, what is the rate of sampling that should be used?)
 
}
