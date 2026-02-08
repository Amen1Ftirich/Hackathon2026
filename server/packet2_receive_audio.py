import serial # type: ignore
import time
import wave
import numpy as np # type: ignore
import pyttsx3
import openai


PORT = "COM5"
BAUD = 115200
RATE = 8000

ser = serial.Serial(PORT, BAUD, timeout=1)
time.sleep(2)
def send_state(state):
    msg = f"STATE:{state}\n"
    ser.write(msg.encode("utf-8"))
    ser.flush()
print("Waiting for audio...")
def speak(text):
    send_state("SPEAKING")

    engine = pyttsx3.init()
    engine.setProperty("rate", 170)
    engine.setProperty("volume", 1.0)

    engine.say(text)
    engine.runAndWait()

    engine.stop()
    del engine

    send_state("IDLE")

def transcribe_audio(filepath):
    with open(filepath, "rb") as audio_file:
        transcript = openai.audio.transcriptions.create(
            model="gpt-4o-mini-transcribe",
            file=audio_file
        )
    return transcript.text
def get_ai_response(user_text):
    response = openai.chat.completions.create(
        model="gpt-4.1-nano",
        messages=[
            {"role": "system", "content": "You are a friendly backpack companion."},
            {"role": "user", "content": user_text}
        ],
        max_tokens=80
    )
    return response.choices[0].message.content

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

        send_state("THINKING")

        user_text = transcribe_audio(filename)
        print("User said:", user_text)

        reply = get_ai_response(user_text)
        print("AI reply:", reply)

        speak(reply)
        


