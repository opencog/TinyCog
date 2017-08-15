#!/usr/bin/env python
#  Project: OpenCogER
#  File: stt_server.py
#  Author: Mandeep Singh Bhatia
#  License: AGPL
#  Date: 2017
#

import speech_recognition as sr
import socket
import time

UDP_IP = "127.0.0.1"
UDP_PORT = 5585
sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP

# this is called from the background thread
def callback(recognizer, audio):
    # received audio data, now we'll recognize it using Google Speech Recognition
    try:
        # for testing purposes, we're just using the default API key
        # to use another API key, use `r.recognize_google(audio, key="GOOGLE_SPEECH_RECOGNITION_API_KEY")`
        # instead of `r.recognize_google(audio)`
        #msg = recognizer.recognize_sphinx(audio, keyword_entries=
        #  [("yes", 1.0), ("no", 1.0),("maybe",1.0)])
        #yes works with yeah
        msg = recognizer.recognize_sphinx(audio, grammar='counting.gram')
        print("<stt>"+msg)
        if len(msg)>0:
          try:
            sock.sendto("<stt>"+msg, (UDP_IP, UDP_PORT))
          except:
            print("socket send error")
    except sr.UnknownValueError:
        print("Could not understand audio")
    except sr.RequestError as e:
        print("Sphinx Error: {0}".format(e))

print("udp ip: "+UDP_IP)
print("udp port: "+ str(UDP_PORT))
r = sr.Recognizer()
m = sr.Microphone()
print("wait.. adjusting to noise...")
with m as source:
    r.adjust_for_ambient_noise(source,duration=5)  # we only need to calibrate once, before we start listening
print("energy = "+ str(r.energy_threshold))
#r.energy_threshold = 280
# start listening in the background (note that we don't have to do this inside a `with` statement)
print("start..")
#stop_listening = r.listen_in_background(m, callback,phrase_time_limit = 2)
stop_listening = r.listen_in_background(m, callback,phrase_time_limit = 3)
# `stop_listening` is now a function that, when called, stops background listening

# do some other computation for 5 seconds, then stop listening and keep doing other computations
#for _ in range(50): time.sleep(0.1)  # we're still listening even though the main thread is doing other things
#stop_listening()  # calling this function requests that the background listener stop listening
while True: time.sleep(0.1)