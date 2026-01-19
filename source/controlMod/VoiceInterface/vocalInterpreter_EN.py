
import speech_recognition as sr

r = sr.Recognizer()
micro = sr.Microphone()
with micro as source:
    audio_data = r.listen(source)
transcription = r.recognize_google(audio_data, language="en-EN")

print (transcription)

