import serial # type: ignore
import time
import wave
import numpy as np # type: ignore
import pyttsx3


PORT = "COM5"
BAUD = 115200
RATE = 8000

ser = serial.Serial(PORT, BAUD, timeout=1)
time.sleep(2)
tts = pyttsx3.init()
tts.setProperty("rate", 170) # speeking speed
tts.setProperty("volume", 1.0) # max volume
def send_state(state):
    msg = f"STATE:{state}\n"
    ser.write(msg.encode("utf-8"))
    ser.flush()
print("Waiting for audio...")
def speak(text):
    send_state("SPEAKING")
    tts.say(text)
    tts.runAndWait()
    send_state("IDLE")


while True:
    line = ser.readline()

    if b"START_AUDIO" in line:
        #send_state("LISTENING")
        print("Recording...")
        raw = bytearray()

        while True:
            data = ser.readline()
            if b"END_AUDIO" in data:
                break
            raw.extend(data)

        send_state("THINKING")
    
        samples = np.frombuffer(raw, dtype=np.uint8)

        filename = f"temp_audio/audio_{int(time.time())}.wav"
        with wave.open(filename, "wb") as wf:
            wf.setnchannels(1)
            wf.setsampwidth(1)
            wf.setframerate(RATE)
            wf.writeframes(samples.tobytes())

        print("Saved", filename)

        send_state("SPEAKING")
        time.sleep(1)
        send_state("IDLE")