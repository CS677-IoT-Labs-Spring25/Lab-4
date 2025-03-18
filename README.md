# Lab 4

Welcome to the final lab Edge AI with Cloud deployment.
# Dataset

You are provied a modifled version of a Human Activity Recognition Dataset (We added some standing position data to the dataset). This dataset contains data from **33 volunteers** with demographic attributes including **gender**, **height**, **weight**, and **age**. Volunteers wore the wristband named **BandX** on their dominant hand while performing specific activities. The **MPU-6050** sensor module captured data from the **accelerometer** and **gyroscope**. This dataset captures human activity data for the following **seven types of activities**:  

1. **Walking** (Wa)  
2. **Jogging** (J)  
3. **Typing** (T)  
4. **Writing** (Wr)  
5. **Upstairs movement** (U)  
6. **Downstairs movement** (D)  
7. **Cycling** (C) 

We then modified the dataset and added a few samples of **Standing** (S), and removed **Upstairs movement** (U) and **Downstairs movement** (D)   for better working of the application.
The sensor data includes Accelerometer (`ax`, `ay`, `az`) and Gyroscope (`gx`, `gy`, `gz`) data collected using the **MPU-6050** sensor, sampling at **20 Hz**.

The processed dataset has been created by take **2 second samples** of an activity (2 second sample at 20Hz means that each input tensor is of 40 readings).

---
# Task 1

In this task you will train a model using the dataset provide. Please refer to the the python notebook for details on this task.

---
# Task 2

Move the model.h file into the sketch's directory, then load and run the model. Have a look at the boilerplate code.

As distressed on the data card in task 1, the model assumes an input of 2 seconds of activity sampled at 20Hz. You will need to create a buffer to store this data then pass it onto the model. 

To initialize the model in cpp, please look at the guide at the end.

---
# Task 3

Now lets move to the cloud. Normally we would ask you to create and host a backend on some cloud service like OCI or ThingsBoards, but as this lab is getting a bit long, you will just host the backend on your computer.

In this task you will create a web page that shows the current activity reported by your esp32. You need to:

- Store model classifications for 60 seconds (6 classification) and send the most common classification to the web page.
- Web page should display the current activity.
- Approximate the step count. Walk for 60 seconds and count he number steps you take. Each time the esp reports Walking add this many steps to the step count. Repeat for jogging. 

---
## TensorFlow Lite Model Implementation Guide

This part of the manual provides an overview of how to implement a TensorFlow Lite model in C++ for embedded systems. It includes explanations, necessary steps, and code snippets for loading, configuring, and running an inference model.

### TensorFlow Lite Model Variables

```cpp
// TensorFlow Lite model variables
const tflite::Model* model; // Pointer to the TensorFlow Lite model
static tflite::MicroInterpreter* interpreter; // Pointer to the interpreter, used to execute inference
TfLiteTensor* input_tensor; // Pointer to the input tensor
TfLiteTensor* output_tensor; // Pointer to the output tensor
```

These pointers are required to load, initialize, and interact with the model.

### Loading the TensorFlow Lite Model

```cpp
model = tflite::GetModel(model_name); // Get the model from the compiled .h file
if (model->version() != TFLITE_SCHEMA_VERSION) {
    Serial.println("Model schema version mismatch!");
    while (1);
}
```

This step ensures that the model schema version matches the TensorFlow Lite version being used.

### Adding Supported Operators (Resolver)

```cpp
static tflite::MicroMutableOpResolver<11> resolver; // Define resolver with 11 operations
```

To determine which operations (layers) your model requires, use the following Python code:

```python
import tensorflow as tf

with open("model.tflite", "rb") as f:
    model_content = f.read()

tf.lite.experimental.Analyzer.analyze(model_content=model_content)
```

Once you have identified the required layers, add them to the resolver as follows:

```cpp
resolver.AddQuantize();        // Adds support for QUANTIZE operation
resolver.AddExpandDims();      // Adds support for EXPAND_DIMS operation
resolver.AddConv2D();          // Adds support for CONV_2D operation
// Add other necessary operations
```

### Allocating Memory for the Model

```cpp
// Define the memory arena size based on model requirements
constexpr int kTensorArenaSize = <SET_MEMORY_SIZE>;
uint8_t tensor_arena[kTensorArenaSize]; // Allocate memory
```

The `kTensorArenaSize` should be set based on the model's memory requirements. Adjust as needed.

### Initializing the Interpreter

```cpp
static tflite::MicroInterpreter static_interpreter(model, resolver, tensor_arena, kTensorArenaSize);
interpreter = &static_interpreter; // Assign pointer to the interpreter object

if (interpreter->AllocateTensors() != kTfLiteOk) {
    digitalWrite(LED, HIGH);
    Serial.println("Failed to allocate tensors!");
    while (1);
}
```

This step initializes the interpreter and allocates memory for the model's input and output tensors.

### Handling Input and Output Tensors

#### Assigning the Input Tensor

```cpp
TfLiteTensor* input_tensor;
input_tensor = interpreter->input(0);

for (int i = 0; i < kInputSize; i++) {
    input_tensor->data.f[i] = input_data[i]; // Populate input tensor with data
}
```

#### Retrieving the Output Tensor

```cpp
TfLiteTensor* output_tensor;
output_tensor = interpreter->output(0);
float* predictions = output_tensor->data.f; // Returns an array of probability values
```

### Invoking the Model for Inference

```cpp
if (interpreter->Invoke() != kTfLiteOk) {
    Serial.println("Error invoking the model");
    return;
}
```

This step runs inference on the input data. 

