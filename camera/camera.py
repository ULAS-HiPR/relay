from picamera2 import Picamera2, Preview
from datetime import datetime
from libcamera import ColorSpace
import cv2
import numpy as np
import tflite_runtime.interpreter as tflite
from PIL import Image
import time
import os
import math
import threading

#NUM_OF_FRAMES = 8

picam2 = Picamera2()
picam2.configure(picam2.create_video_configuration(
    main={"format": 'XRGB8888', "size": (1000, 1000)})
)
start = time.time()

model = "12_10_SZNet_MINI_01.tflite"
#model = "11_10__MULTICLASS_OUTPUT.tflite"
interpreter = tflite.Interpreter(model_path=f"/home/EanSat/models/{model}")

print(f"Model: {model}")

interpreter.allocate_tensors()

input_details = interpreter.get_input_details()
output_details = interpreter.get_output_details()
end = time.time()

print(f"It took {str(end-start)}s to load the model")

picam2.start()

output_dir = datetime.now().strftime("%Y%m%d_%H%M")

if not os.path.isdir(f'/home/EanSat/results/{output_dir}'):
    os.mkdir(f'/home/EanSat/results/{output_dir}')

if not os.path.isdir(f'/home/EanSat/results/{output_dir}/input'):
    os.mkdir(f'/home/EanSat/results/{output_dir}/input')


if not os.path.isdir(f'/home/EanSat/results/{output_dir}/output'):
    os.mkdir(f'/home/EanSat/results/{output_dir}/output')

#input_writer = cv2.VideoWriter(f'/home/EanSat/results/{output_dir}/input.mp4', cv2.VideoWriter_fourcc(*'mp4v'), 1.0, (500, 500), True)
#output_writer = cv2.VideoWriter(f'/home/EanSat/results/{output_dir}/output.mp4', cv2.VideoWriter_fourcc(*'mp4v'), 1.0, (500, 500), 0)

input_shape = input_details[0]['shape']
#start = time.time()

#num_of_frames = 0

#for i in range(0,NUM_OF_FRAMES):
while True:
    inf_start = time.time()

    im = picam2.capture_array()
 
    im = im[250:750,500:,:]

    cv2.imwrite(f'/home/EanSat/results/{output_dir}/input/{math.floor(inf_start)}.jpg', im[:,:,0:3].astype('uint8'))
 #   input_writer.write(im[:,:,0:3].astype('uint8'))
    interpreter.set_tensor(input_details[0]['index'], np.asarray([np.transpose(im[:,:,0:3], (2,0,1))]).astype('float32'))
    interpreter.invoke()

    output_data = interpreter.get_tensor(output_details[0]['index']) 
   
    #output_writer.write(np.transpose(output_data[0], (1,2,0))[:,:,0].astype('uint8'))
    cv2.imwrite(f'/home/EanSat/results/{output_dir}/output/{math.floor(inf_start)}.jpg', np.transpose(output_data[0], (1,2,0))[:,:,0].astype('uint8'))
    inf_end = time.time()

    print(f"Inference took {str(inf_end-inf_start)}")
    
    #num_of_frames += 1

#end = time.time()

#print(f"It took {str(end-start)}s to capture and run inference on {str(num_of_frames)} images")

