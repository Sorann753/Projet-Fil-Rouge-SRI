"""
@file speech_en.py
@brief Script pour récupérer une commande vocale en anglais via le micro.
@author Victor CHALUMEAUX
"""

import speech_recognition as sr

def get_speech_en() -> str:
    """
    @brief Récupère la commande vocale en anglais via le micro.
    @return La chaîne reconnue, ou 'err' en cas d'erreur
    @remarks Utilise Google Speech Recognition. La fonction limite la phrase à 6 secondes.
    """
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
        return text
    except (sr.UnknownValueError, sr.RequestError):
        return "err"

if __name__ == "__main__":
    print(get_speech_en())
