import serial
import time
import wave
import numpy as np

PORT = "COM5"      # CHANGE THIS
BAUD = 115200
RATE = 8000

ser = serial.Serial(PORT, BAUD, timeout=1)
time.sleep(2)

print("Waiting for audio...")

while True:
    line = ser.readline()

    if b"START_AUDIO" in line:
        print("Recording...")
        raw = bytearray()

        while True:
            data = ser.readline()
            if b"END_AUDIO" in data:
                break
            raw.extend(data)

        samples = np.frombuffer(raw, dtype=np.uint8)

        filename = f"audio_{int(time.time())}.wav"
        with wave.open(filename, "wb") as wf:
            wf.setnchannels(1)
            wf.setsampwidth(1)
            wf.setframerate(RATE)
            wf.writeframes(samples.tobytes())

        print("Saved", filename)
