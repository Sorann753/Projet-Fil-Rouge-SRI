import speech_recognition as sr

r = sr.Recognizer()
r.energy_threshold = 300
r.dynamic_energy_threshold = True
r.pause_threshold = 1   
r.phrase_time_limit = 6   

with sr.Microphone() as source:
    r.adjust_for_ambient_noise(source, duration=0.5)
    audio = r.listen(source)

try:
    text = r.recognize_google(audio, language="en-EN")
    print(text)
except sr.UnknownValueError:
    print("err")
except sr.RequestError:
    print("err")
